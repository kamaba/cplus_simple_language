//****************************************************************************
//  File:      FileMeta.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "../Token.h"
#include "FileMetaCommon.h"
#include "FileMeta.h"
#include "FileMetaClass.h"
#include "FileMetaExpress.h"
#include "FileMetaSyntax.h"
#include "FileMetatUtil.h"
#include "../Parse/Node.h"
#include "../../Debug/Log.h"
#include "../../Define.h"
#include "../../Core/MetaNode.h"
#include <sstream>
#include <algorithm>

namespace SimpleLanguage {
namespace Compile {

// NamespaceStatementBlock implementation
NamespaceStatementBlock::NamespaceStatementBlock(const std::vector<Token*>& token) {
    m_TokenList = token;
}

std::string NamespaceStatementBlock::NamespaceString() const {
    if (m_NamespaceString.empty()) {
        std::ostringstream sb;
        for (size_t i = 0; i < m_TokenList.size(); i++) {
            sb << m_TokenList[i]->GetLexeme();
            if (i != m_TokenList.size() - 1) {
                sb << ".";
            }
        }
        m_NamespaceString = sb.str();
    }
    return m_NamespaceString;
}

const std::vector<std::string>& NamespaceStatementBlock::NamespaceStackList() const {
    if (m_NamespaceStackList.empty()) {
        std::string add = "";
        for (size_t i = 0; i < m_TokenList.size(); i++) {
            m_NamespaceStackList.push_back(add + m_TokenList[i]->GetLexeme());
            if (add.empty()) {
                add = m_TokenList[i]->GetLexeme() + ".";
            } else {
                add = add + m_TokenList[i]->GetLexeme() + ".";
            }
        }
    }
    return m_NamespaceStackList;
}

const std::vector<std::string>& NamespaceStatementBlock::NamespaceList() const {
    if (m_NamespaceList.empty()) {
        for (size_t i = 0; i < m_TokenList.size(); i++) {
            m_NamespaceList.push_back(m_TokenList[i]->GetLexeme());
        }
    }
    return m_NamespaceList;
}

NamespaceStatementBlock* NamespaceStatementBlock::CreateStateBlock(const std::vector<Token*>& token) {
    bool isIdentifier = true;
    std::vector<Token*> tokenList;
    for (size_t i = 0; i < token.size(); i++) {
        if (isIdentifier) {
            if (token[i]->GetType() == SimpleLanguage::ETokenType::Identifier || 
                token[i]->GetType() == SimpleLanguage::ETokenType::Type) {
                tokenList.push_back(token[i]);
                isIdentifier = false;
            } else {
                SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error 命名空间有误，必须为X.xx.X 类似的格式!");
                return nullptr;
            }
        } else {
            if (token[i]->GetType() != SimpleLanguage::ETokenType::Period) {
                SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error 命名空间有误，必须为X.xx.X 类似的格式!");
                return nullptr;
            }
            isIdentifier = true;
        }
    }

    return new NamespaceStatementBlock(tokenList);
}

std::string NamespaceStatementBlock::ToString() const {
    if (m_NamespaceString.empty())
        return NamespaceString();
    return m_NamespaceString;
}

std::string NamespaceStatementBlock::ToFormatString() const {
    return ToString();
}

// FileInputParamNode implementation
FileInputParamNode::FileInputParamNode(FileMetaBaseTerm* fmbt) {
    m_Express = fmbt;
    //m_Express->ClearDirty();
    m_Express->BuildAST();
}

std::string FileInputParamNode::ToFormatString() const {
    return m_Express ? m_Express->ToFormatString() : "";
}

// FileInputTemplateNode implementation
FileInputTemplateNode::FileInputTemplateNode(FileMeta* fm, Node* node) {
    m_FileMeta = fm;
    m_Node = node;
    m_DefineClassCallLink = new FileMetaCallLink(fm, node);
}

std::vector<std::string> FileInputTemplateNode::NameList() const {
    std::vector<std::string> _nameList;
    if (m_DefineClassCallLink != nullptr) {
        for (size_t i = 0; i < m_DefineClassCallLink->CallNodeList().size(); i++) {
            _nameList.push_back(m_DefineClassCallLink->CallNodeList()[i]->Name());
        }
    }
    return _nameList;
}

int FileInputTemplateNode::InputTemplateCount() const {
    int templateCount = 0;
    if (m_DefineClassCallLink != nullptr) {
        int cn = m_DefineClassCallLink->CallNodeList().size();
        if (cn > 0) {
            return m_DefineClassCallLink->CallNodeList()[cn - 1]->InputTemplateNodeList().size();
        }
    }
    return templateCount;
}

std::string FileInputTemplateNode::ToFormatString() const {
    return m_DefineClassCallLink ? m_DefineClassCallLink->ToFormatString() : "";
}

// FileMetaCallNode implementation
FileMetaCallNode::FileMetaCallNode(FileMeta* fm, Node* _node)  {
    m_FileMeta = fm;
    m_Node = _node;
    m_IsCallFunction = false;
    CreateFileMetaCallNode();
}

void FileMetaCallNode::CreateFileMetaCallNode() {
    m_Token = m_Node->token;
    m_AtToken = m_Node->atToken;

    if (m_Node->nodeType == SimpleLanguage::Compile::ENodeType::Par) {
        m_FileMetaParTerm = new FileMetaParTerm(m_FileMeta, m_Node, FileMetaTermExpress::EExpressType::Common);
        m_BeginParToken = m_FileMetaParTerm->GetToken();
        m_EndParToken = m_FileMetaParTerm->EndToken();
    }
    if (m_Node->parNode != nullptr) {
        m_IsCallFunction = true;
        m_FileMetaParTerm = new FileMetaParTerm(m_FileMeta, m_Node->parNode, FileMetaTermExpress::EExpressType::Common);
        m_BeginParToken = m_FileMetaParTerm->GetToken();
        m_EndParToken = m_FileMetaParTerm->EndToken();
    }
    if (m_Node->angleNode != nullptr) {
        m_IsTemplate = true;
        m_BeginAngleToken = m_Node->angleNode->token;
        m_EndAngleToken = m_Node->angleNode->endToken;
        const auto& list = m_Node->angleNode->childList;
        for (size_t i = 0; i < list.size(); i++) {
            if (list[i]->nodeType == SimpleLanguage::Compile::ENodeType::Comma) {
                continue;
            }
            auto aa = new FileInputTemplateNode(m_FileMeta, list[i]);
            m_InputTemplateNodeList.push_back(aa);
        }
    }
    if (m_Node->bracketNode != nullptr) {
        m_IsArray = true;
        m_FileMetaBracketTerm = new FileMetaBracketTerm(m_FileMeta, m_Node);
        m_BeginBracketToken = m_FileMetaBracketTerm->GetToken();
        m_EndBracketToken = m_FileMetaBracketTerm->EndToken();
        
        const auto& list = m_Node->bracketNode->childList;
        for (size_t i = 0; i < list.size(); i++) {
            if (list[i]->nodeType == SimpleLanguage::Compile::ENodeType::Comma) {
                if (i == list.size() - 1) {
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Warning [1,2,3,]有多余逗号出现??");
                }
                continue;
            }
            auto aa = new FileMetaCallLink(m_FileMeta, list[i]);
            m_ArrayNodeList.push_back(aa);
        }
    }
    if (m_Node->blockNode != nullptr) {
        m_FileMetaBraceTerm = new FileMetaBraceTerm(m_FileMeta, m_Node->blockNode );
    }
}

/*
bool FileMetaCallNode::IsOnlyName() const {
    if (m_CallNodeList.size() == 1) {
        if (!m_CallNodeList[0]->isCallFunction())
            return true;
    }
    return false;
}*/

std::string FileMetaCallNode::Name() const {
   if (m_Token != nullptr ) {
       return m_Token->GetLexeme();
    }
    return "";
}

std::string FileMetaCallNode::ToFormatString() const {
    std::ostringstream sb;

    if (m_FileMetaParTerm != nullptr) {
        sb << (token != nullptr ? token->GetLexeme() : "");
        sb << m_FileMetaParTerm->ToFormatString();
        if (m_FileMetaBraceTerm != nullptr) {
            sb << m_FileMetaBraceTerm->ToFormatString();
        }
    } else if (m_FileMetaBracketTerm != nullptr) {
        sb << m_FileMetaBracketTerm->ToFormatString();
    } else if (m_FileMetaBraceTerm != nullptr) {
        sb << m_FileMetaBraceTerm->ToFormatString();
    } else {
        sb << (m_AtToken ? m_AtToken->GetLexeme() : "");
        sb << (token != nullptr ? token->ToConstString() : "");
        if (m_IsArray) {
            sb << (m_BeginBracketToken ? m_BeginBracketToken->GetLexeme() : "");
            for (size_t i = 0; i < m_ArrayNodeList.size(); i++) {
                sb << m_ArrayNodeList[i]->ToFormatString();
                if (i < m_ArrayNodeList.size() - 1)
                    sb << ",";
            }
            sb << (m_EndBracketToken ? m_EndBracketToken->GetLexeme() : "");
        }
        if (m_IsTemplate) {
            sb << (m_BeginAngleToken ? m_BeginAngleToken->GetLexeme() : "");
            for (size_t i = 0; i < m_InputTemplateNodeList.size(); i++) {
                sb << m_InputTemplateNodeList[i]->ToFormatString();
                if (i < m_InputTemplateNodeList.size() - 1)
                    sb << ",";
            }
            sb << (m_EndAngleToken ? m_EndAngleToken->GetLexeme() : "");
        }
        if (m_IsCallFunction) {
            sb << (m_BeginParToken ? m_BeginParToken->GetLexeme() : "");
            sb << (m_EndParToken ? m_EndParToken->GetLexeme() : "");
        }
        if (m_FileMetaBraceTerm != nullptr) {
            sb << m_FileMetaBraceTerm->ToFormatString();
        }
    }
    return sb.str();
}

std::string FileMetaCallNode::ToTokenString() const {
    std::ostringstream sb;

    /*if (m_FileMetaParTerm != nullptr) {
        sb << m_FileMetaParTerm->ToFormatString();
    } else {
        sb << (token() ? token()->lexeme().ToString() : "");
        sb << "在文件:" << (token() ? token()->path() : "") << " 行: " << (token() ? std::to_string(token()->sourceBeginLine()) : "") << " 位置: " << (token() ? std::to_string(token()->sourceBeginChar()) : "");
        if (m_IsCallFunction) {
            sb << (m_BeginParToken ? m_BeginParToken->lexeme().ToString() : "");
            sb << (m_EndParToken ? m_EndParToken->lexeme().ToString() : "");
        }
        if (m_FileMetaBraceTerm != nullptr) {
            sb << m_FileMetaBraceTerm->ToFormatString();
        }
    }*/
    return sb.str();
}

// FileMetaCallLink implementation
FileMetaCallLink::FileMetaCallLink(FileMeta* fm, Node* node) : m_Node(node), m_FileMeta(fm) {
    AddChildExtendLinkList(m_Node, true);
}

void FileMetaCallLink::AddChildExtendLinkList(Node* cnode, bool isIncludeSelf) {
    auto childNodeList = cnode->GetLinkNodeList(isIncludeSelf);
    for (size_t i = 0; i < childNodeList.size(); i++) {
        auto cnode1 = childNodeList[i];
        auto fmcn = new FileMetaCallNode(m_FileMeta, cnode1);
        m_CallNodeList.push_back(fmcn);
        if (i == childNodeList.size() - 1) {
            if (cnode1->GetExtendLinkNodeList().size() > 0) {
                AddChildExtendLinkList(cnode1, false);
            }
        }
    }
}

bool FileMetaCallLink::IsOnlyName() const {
    if (m_CallNodeList.size() == 1) {
        //if (!m_CallNodeList[0]->isCallFunction())
            return true;
    }
    return false;
}

std::string FileMetaCallLink::Name() const {
    if (m_CallNodeList.size() >= 1) {
        return m_CallNodeList[0]->Name();
    }
    return "";
}

std::string FileMetaCallLink::ToFormatString() const {
    std::ostringstream sb;
    for (size_t i = 0; i < m_CallNodeList.size(); i++) {
        sb << m_CallNodeList[i]->ToFormatString();
    }
    return sb.str();
}

std::string FileMetaCallLink::ToTokenString() const {
    std::ostringstream sb;
    for (size_t i = 0; i < m_CallNodeList.size(); i++) {
        sb << m_CallNodeList[i]->ToTokenString();
    }
    return sb.str();
}

// FileMetaClassDefine implementation
FileMetaClassDefine::FileMetaClassDefine(FileMeta* fm, const std::vector<Token*>& _tokenList) : m_FileMeta(fm), m_TokenList(_tokenList) {
    m_ClassNameToken = _tokenList[_tokenList.size() - 1];
}

FileMetaClassDefine::FileMetaClassDefine(FileMeta* fm, Node* node, Node* mutNode) : m_FileMeta(fm) {
    m_TokenList = node->GetLinkTokenList();
    m_ClassNameToken = m_TokenList[m_TokenList.size() - 1];
    m_MutToken = mutNode ? mutNode->token : nullptr;

    if (node->angleNode != nullptr) {
        m_IsInputTemplateData = true;
        m_AngleTokenBegin = node->angleNode->token;
        m_AngleTokenEnd = node->angleNode->endToken;
        for (size_t i = 0; i < node->angleNode->childList.size(); i++) {
            auto cnode = node->angleNode->childList[i];
            if (cnode->nodeType == SimpleLanguage::Compile::ENodeType::Comma)
                continue;
            auto fmcn = new FileInputTemplateNode(fm, cnode);
            m_InputTemplateNodeList.push_back(fmcn);
        }
    }
    if (node->bracketNode != nullptr) {
        m_IsArray = true;
        m_BracketTokenBegin = node->bracketNode->token;
        m_BracketTokenEnd = node->bracketNode->endToken;

        Token* frontToken = m_BracketTokenBegin;
        const auto& cl = node->bracketNode->childList;
        if (cl.empty()) {
            auto token = new Token(*frontToken);
            token->SetLexeme("-1", SimpleLanguage::ETokenType::Number);
            //token->SetExtend(SimpleLanguage::EType::Int32);
            m_ArrayTokenList.push_back(token);
        } else {
            std::vector<Token*> tlist;
            bool isOnlyComma = true;
            for (size_t i = 0; i < cl.size(); i++) {
                auto cnode = cl[i];
                if (cnode->nodeType == SimpleLanguage::Compile::ENodeType::Comma) {
                    if (isOnlyComma) {
                        auto t = new Token(*cnode->token);
                        t->SetLexeme("-1", SimpleLanguage::ETokenType::Number);
                        string str = "";
                        t->SetExtend("Int32");// SimpleLanguage::EType::Int32) );
                        m_ArrayTokenList.push_back(t);
                    }
                } else {
                    isOnlyComma = false;
                    m_ArrayTokenList.push_back(cnode->token);
                }
            }
        }
    }
}

std::vector<std::string> FileMetaClassDefine::StringList() const {
    return FileMetatUtil::GetLinkStringMidPeriodList(m_TokenList);
}

std::string FileMetaClassDefine::AllName() const {
    std::ostringstream sb;
    for (size_t i = 0; i < m_TokenList.size(); i++) {
        sb << (m_TokenList[i] ? m_TokenList[i]->GetLexeme() : "");
    }
    return sb.str();
}

std::string FileMetaClassDefine::Name() const {
    if (m_ClassNameToken != nullptr) {
        return m_ClassNameToken->GetLexeme();
    }
    return "";
}

SimpleLanguage::Core::MetaNode* FileMetaClassDefine::GetChildrenMetaNode(SimpleLanguage::Core::MetaNode* mb) {
    if (mb == nullptr) return nullptr;
    SimpleLanguage::Core::MetaNode* mb2 = mb;
    auto list = StringList();
    for (size_t i = 0; i < list.size(); i++) {
        mb2 = mb2->GetChildrenMetaNodeByName(list[i]);
        if (mb2 == nullptr)
            break;
    }
    return mb2;
}

std::string FileMetaClassDefine::ToString() const {
    return AllName();
}

std::string FileMetaClassDefine::ToTokenString() const {
    return AllName() + " Token File:[" + (m_ClassNameToken ? m_ClassNameToken->GetPath() : "") + "] Line:[" + 
           (m_ClassNameToken ? std::to_string(m_ClassNameToken->GetSourceBeginLine()) : "") + "]  Char:[" + 
           (m_ClassNameToken ? std::to_string(m_ClassNameToken->GetSourceBeginChar()) : "") + "]";
}

std::string FileMetaClassDefine::ToFormatString() const {
    std::ostringstream sb;
    sb << AllName();
    if (m_IsInputTemplateData) {
        sb << (m_AngleTokenBegin ? m_AngleTokenBegin->GetLexeme() : "");
        for (size_t i = 0; i < m_InputTemplateNodeList.size(); i++) {
            sb << m_InputTemplateNodeList[i]->ToFormatString();
            if (i < m_InputTemplateNodeList.size() - 1) {
                sb << ",";
            }
        }
        sb << (m_AngleTokenEnd ? m_AngleTokenEnd->GetLexeme() : "");
    }
    if (m_IsArray) {
        sb << (m_BracketTokenBegin ? m_BracketTokenBegin->GetLexeme() : "");
        for (size_t i = 0; i < m_ArrayTokenList.size(); i++) {
            sb << m_ArrayTokenList[i]->GetLexeme();
            if (i < m_ArrayTokenList.size() - 1)
                sb << ",";
        }
        sb << (m_BracketTokenEnd ? m_BracketTokenEnd->GetLexeme() : "");
    }
    return sb.str();
}

void FileMetaClassDefine::AddError2(int errorId, const std::string& pfile, const std::string& pfunction, int line) {
    std::string str = "";
    switch (errorId) {
        case 0: {
            str = "判断接口的时候没有发现[" + AllName() + "]类";
        }
        break;
        default: break;
    }
    str = str + " \n Token: 在文件:" + (m_ClassNameToken ? m_ClassNameToken->GetPath() : "") + " 开始行号:" + 
          (m_ClassNameToken ? std::to_string(m_ClassNameToken->GetSourceBeginLine()) : "") + "开始位置: " +
          (m_ClassNameToken ? std::to_string(m_ClassNameToken->GetSourceBeginChar()) : "");
    str = str + " \n 在代码中文件:" + pfile + "   函数:" + pfunction + "行号: " + std::to_string(line);
    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, str);
}

// FileMetaTemplateDefine implementation
FileMetaTemplateDefine::FileMetaTemplateDefine(FileMeta* fm, Node* node) : m_Node(node) {
    m_FileMeta = fm;
    m_Token = node->token;
    if (node->childList.size() > 0) {
        m_ExtendsNode = node->childList[0];
    }
}

FileMetaTemplateDefine::FileMetaTemplateDefine(FileMeta* fm, Node* node, Node* extendsNode) : m_Node(node), m_ExtendsNode(extendsNode) {
    m_FileMeta = fm;
    m_Token = node->token;
}

FileMetaTemplateDefine::FileMetaTemplateDefine(FileMeta* fm, const std::vector<Node*>& nodeList) {
    m_FileMeta = fm;
    if (nodeList.empty()) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error 在<>中没有发现元素!!");
        return;
    }
    m_Token = nodeList[0]->token;
    if (nodeList.size() == 2) {
        m_InToken = nodeList[1]->token;
        m_InClassNameTemplateNode = new FileInputTemplateNode(fm, nodeList[2]);
    } else if (nodeList.size() == 2) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error 在<T in> or <T []> or <T ClassName> 使用方法不正确,请使用 <T in []>或者是 <T in ClassName> !!");
    }
}

void FileMetaTemplateDefine::Parse() {
    // Implementation for parsing
}

std::string FileMetaTemplateDefine::ToFormatString() const {
    std::ostringstream sb;
    sb << (m_Token ? m_Token->GetLexeme() : "");
    if (m_InToken != nullptr) {
        sb << " " + m_InToken->GetLexeme() + " ";
    }
    if (m_InClassNameTemplateNode != nullptr) {
        sb << m_InClassNameTemplateNode->ToFormatString();
    }
    return sb.str();
}

} // namespace Compile
} // namespace SimpleLanguage

