//****************************************************************************
//  File:      FileMetaExpress.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "FileMetaExpress.h"
#include "FileMeta.h"
#include "FileMetaCommon.h"
#include "FileMetaSyntax.h"
#include "FileMetatUtil.h"
#include "../../Debug/Log.h"
#include "../../Define.h"
#include "../Parse/StructParse.h"
#include "../Parse/Node.h"
#include <algorithm>
#include <limits>
#include <sstream>

using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Compile {

// FileMetaBaseTerm implementation
FileMetaBaseTerm::FileMetaBaseTerm(FileMeta* fm) 
{
    m_FileMeta = fm;
    m_Root = this;
}

bool FileMetaBaseTerm::IsOnlyOne() const {
    return GetLeft() == nullptr && GetRight() == nullptr;
}

void FileMetaBaseTerm::SetLeft(FileMetaBaseTerm* value) {
    m_Left = value;
    m_IsDirty = true;
}

void FileMetaBaseTerm::SetRight(FileMetaBaseTerm* value) {
    m_Right = value;
    m_IsDirty = true;
}

std::vector<FileMetaBaseTerm*> FileMetaBaseTerm::SplitParamList() {
    std::vector<FileMetaBaseTerm*> paramFileMetaTermList;
    
    std::vector<std::vector<FileMetaBaseTerm*>> fmbtListList;
    std::vector<FileMetaBaseTerm*> fmbtList;
    
    bool isComma = false;
    for (size_t i = 0; i < m_FileMetaExpressList.size(); i++) {
        auto fmen = m_FileMetaExpressList[i];
        auto fmst = dynamic_cast<FileMetaSymbolTerm*>(fmen);
        if (fmst != nullptr && fmst->GetToken() != nullptr && 
            fmst->GetToken()->GetType() == ETokenType::Comma) {
            if (isComma) {
                Log::AddInStructFileMeta(EError::None, "Error 多重逗号，导致解析无法解析!!");
                break;
            }
            if (fmbtList.empty()) {
                Log::AddInStructFileMeta(EError::None, "Error 首符号不能为逗号");
                break;
            }
            isComma = true;
            fmbtListList.push_back(fmbtList);
            fmbtList.clear();
        } else {
            isComma = false;
            fmbtList.push_back(fmen);
        }
    }
    if (fmbtList.empty()) {
        return paramFileMetaTermList;
    }
    fmbtListList.push_back(fmbtList);
    
    for (size_t i = 0; i < fmbtListList.size(); i++) {
        auto fmbt2 = fmbtListList[i];
        
        if (fmbt2.size() == 1) {
            paramFileMetaTermList.push_back(fmbt2[0]);
        } else {
            paramFileMetaTermList.insert(paramFileMetaTermList.end(), fmbt2.begin(), fmbt2.end());
        }
    }
    return paramFileMetaTermList;
}

void FileMetaBaseTerm::ClearDirty() {
    m_IsDirty = false;
    for (size_t i = 0; i < m_FileMetaExpressList.size(); i++) {
        auto fme = m_FileMetaExpressList[i];
        fme->ClearDirty();
    }
}

void FileMetaBaseTerm::AddFileMetaTerm(FileMetaBaseTerm* fmn) {
    if (fmn != nullptr) {
        fmn->SetFileMeta(m_FileMeta);
        m_FileMetaExpressList.push_back(fmn);
    }
}

void FileMetaBaseTerm::AddRangeFileMetaTerm(const std::vector<FileMetaBaseTerm*>& fmn) {
    for (size_t i = 0; i < fmn.size(); i++) {
        if (fmn[i] != nullptr) {
            fmn[i]->SetFileMeta(m_FileMeta);
        }
    }
    m_FileMetaExpressList.insert(m_FileMetaExpressList.end(), fmn.begin(), fmn.end());
}

std::string FileMetaBaseTerm::ToFormatString() const {
    if (m_Token != nullptr) {
        return m_Token->GetLexemeString();
    }
    return "";
}

std::string FileMetaBaseTerm::ToTokenString() const {
    std::ostringstream sb;
    
    if (m_Token != nullptr) {
        sb << m_Token->ToLexemeAllString();
    }
    
    if (GetLeft() != nullptr) {
        // Handle left side
    }
    if (GetRight() != nullptr) {
        // Handle right side
    }
    
    for (size_t i = 0; i < m_FileMetaExpressList.size(); i++) {
        auto fme = m_FileMetaExpressList[i];
        sb << " " << fme->ToTokenString();
    }
    return sb.str();
}

// FileMetaParTerm implementation
FileMetaParTerm::FileMetaParTerm(FileMeta* fm, Node* node, EExpressType expressType) 
    : FileMetaBaseTerm(fm), m_Node(node), m_ExpressType(expressType) {
    m_Token = node->token;
    m_EndToken = node->endToken;
    m_ParNode = node;

    auto& childList = node->childList;
    
    std::vector<std::vector<Node*>> nodeListList;
    std::vector<Node*> tempNodeList;
    
    for (size_t j = 0; j < childList.size(); j++) {
        auto c2node = childList[j];
        if (c2node->nodeType == ENodeType::Comma || 
            c2node->nodeType == ENodeType::SemiColon) {
            nodeListList.push_back(tempNodeList);
            tempNodeList.clear();
        } else {
            tempNodeList.push_back(c2node);
        }
    }
    
    if (!tempNodeList.empty()) {
        nodeListList.push_back(tempNodeList);
    }

    for (size_t i = 0; i < nodeListList.size(); i++) {
        auto& nodeList = nodeListList[i];
        if (nodeList.empty()) {
            Log::AddInStructFileMeta(EError::None, "Error nodeList.Count == 0 ");
            continue;
        } else if (nodeList.size() == 1) {
            auto cnode = nodeList[0];
            if (cnode->nodeType == ENodeType::ConstValue) {     // Fun( 1 )
                auto fileMetaConstValueTerm = new FileMetaConstValueTerm(m_FileMeta, cnode->token);
                fileMetaConstValueTerm->SetPriority(cnode->GetPriority());
                AddFileMetaTerm(fileMetaConstValueTerm);
            } else if (cnode->nodeType == ENodeType::Bracket) {       // Fun( [1] )
                auto fileMetaBracketTerm = new FileMetaBracketTerm(m_FileMeta, cnode);
                fileMetaBracketTerm->SetPriority(SignComputePriority::Level1);
                AddFileMetaTerm(fileMetaBracketTerm);
            } else if (cnode->nodeType == ENodeType::Comma) {
                auto fileMetaSymbolTerm = new FileMetaSymbolTerm(m_FileMeta, cnode->token);
                fileMetaSymbolTerm->SetPriority();
                AddFileMetaTerm(fileMetaSymbolTerm);
            } else if (cnode->nodeType == ENodeType::Brace) {  // Enum.Value( {} );
                auto fileMetaBraceTerm = new FileMetaBraceTerm(m_FileMeta, cnode);
                AddFileMetaTerm(fileMetaBraceTerm);
            } else {
                auto fileMetaCallTerm = new FileMetaCallTerm(m_FileMeta, cnode);
                fileMetaCallTerm->SetPriority(SignComputePriority::Level1);
                AddFileMetaTerm(fileMetaCallTerm);
            }
        } else {
            auto fileMetaCallTerm = new FileMetaTermExpress(m_FileMeta, nodeList, expressType);
            fileMetaCallTerm->SetPriority(SignComputePriority::Level1);
            AddFileMetaTerm(fileMetaCallTerm);
        }
    }
}

void FileMetaParTerm::ClearDirty() {
    for (size_t i = 0; i < m_FileMetaExpressList.size(); i++) {
        m_FileMetaExpressList[i]->ClearDirty();
    }
}

void FileMetaParTerm::BuildAST() {
    if (m_FileMetaExpressList.size() == 1) {
        FileMetaBaseTerm* fmbt = m_FileMetaExpressList[0];
        if (fmbt == nullptr) return;
        fmbt->BuildAST();
        SetDirty(true);
        m_Root = fmbt->GetRoot();
    } else {
        for (size_t i = 0; i < m_FileMetaExpressList.size(); i++) {
            auto fme = m_FileMetaExpressList[i];
            fme->BuildAST();
        }
        m_Root = this;
    }
}

std::string FileMetaParTerm::ToFormatString() const {
    std::ostringstream stringBuilder;
    if (m_Token != nullptr) {
        stringBuilder << m_Token->GetLexemeString();
    }
    for (size_t i = 0; i < m_FileMetaExpressList.size(); i++) {
        stringBuilder << m_FileMetaExpressList[i]->ToFormatString();
        if (i < m_FileMetaExpressList.size() - 1) {
            stringBuilder << ",";
        }
    }
    if (m_EndToken != nullptr) {
        stringBuilder << m_EndToken->GetLexemeString();
    }
    return stringBuilder.str();
}

std::string FileMetaParTerm::ToTokenString() const {
    std::ostringstream sb;
    
    if (m_Token != nullptr) {
        sb << "BeginParToken:" << m_Token->ToLexemeAllString();
    }
    if (m_EndToken != nullptr) {
        sb << "EndParToken:" << m_EndToken->ToLexemeAllString();
    }
    
    return sb.str();
}

Node* FileMetaParTerm::GetParNode() const {
    return m_ParNode;
}

Token* FileMetaParTerm::GetEndToken() const {
    return m_EndToken;
}

EExpressType FileMetaParTerm::GetExpressType() const {
    return m_ExpressType;
}

// FileMetaSymbolTerm implementation
FileMetaSymbolTerm::FileMetaSymbolTerm(FileMeta* fm, Token* token) : FileMetaBaseTerm(fm) {
    m_FileMeta = fm;
    m_Token = token;
    m_SymbolToken = token;
    m_Root = this;
    SetPriority();
}

Token* FileMetaSymbolTerm::GetToken() const {
    return m_Token;
}

ETokenType FileMetaSymbolTerm::GetSymbolType() const {
    if (m_Token != nullptr) {
        return m_Token->GetType();
    }
    return ETokenType::None;
}

void FileMetaSymbolTerm::SetPriority() {
    if (m_Token == nullptr) return;
    
    switch (m_Token->GetType()) {
        case ETokenType::Plus:
        case ETokenType::Minus:
            m_Priority = SignComputePriority::Level2_LinkOp;
            break;
        case ETokenType::Multiply:
        case ETokenType::Divide:
            m_Priority = SignComputePriority::Level2_LinkOp;
            break;
        case ETokenType::DoublePlus:     //++
        case ETokenType::DoubleMinus:    //--
            m_Priority = SignComputePriority::Level2_LinkOp;
            break;
        case ETokenType::Modulo:          // %
        case ETokenType::Not:             // !
        case ETokenType::Negative:        // ~
            m_Priority = SignComputePriority::Level3_Hight_Compute;
            break;
        case ETokenType::Shi:               //  <<
        case ETokenType::Shr:               //  >>
            m_Priority = SignComputePriority::Level5_BitMoveOp;
            break;
        case ETokenType::Less:            // >
        case ETokenType::GreaterOrEqual:  // >=
        case ETokenType::Greater:         // <
        case ETokenType::LessOrEqual:     // <=
            m_Priority = SignComputePriority::Level6_Compare;
            break;
        case ETokenType::Equal:           // ==
        case ETokenType::NotEqual:        // !=
            m_Priority = SignComputePriority::Level7_EqualAb;
            break;
        case ETokenType::Combine:         // &
            m_Priority = SignComputePriority::Level8_BitAndOp;
            break;
        case ETokenType::InclusiveOr:     // |
            m_Priority = SignComputePriority::Level8_BitOrOp;
            break;
        case ETokenType::XOR:             //  ^
            m_Priority = SignComputePriority::Level8_BitXOrOp;
            break;
        case ETokenType::Or:              // ||
            m_Priority = SignComputePriority::Level9_Or;
            break;
        case ETokenType::And:             // &&  
            m_Priority = SignComputePriority::Level9_And;
            break;
        case ETokenType::PlusAssign:             // +=
        case ETokenType::MinusAssign:            // -=
        case ETokenType::MultiplyAssign:         // *=
        case ETokenType::DivideAssign:           // /=
        case ETokenType::ModuloAssign:           // %=
        case ETokenType::InclusiveOrAssign:      // |=
        case ETokenType::XORAssign:              // ^=
            m_Priority = SignComputePriority::Level3_Hight_Compute;
            break;
        case ETokenType::Comma:                  // ,
            m_Priority = SignComputePriority::Level12_Split;
            break;
        default:
            m_Priority = SignComputePriority::Level2_LinkOp;
            break;
    }
}

void FileMetaSymbolTerm::BuildAST() {
    // Implementation for BuildAST
}

std::string FileMetaSymbolTerm::ToFormatString() const {
    if (m_Token != nullptr) {
        return m_Token->GetLexemeString();
    }
    return "";
}

std::string FileMetaSymbolTerm::ToTokenString() const {
    std::ostringstream sb;
    if (m_Token != nullptr) {
        sb << m_Token->GetLexemeString();
    }
    return sb.str();
}

// FileMetaConstValueTerm implementation
FileMetaConstValueTerm::FileMetaConstValueTerm(FileMeta* fm, Token* token) : FileMetaBaseTerm(fm) {
    m_FileMeta = fm;
    m_Token = token;
    m_ValueToken = token;
    m_Root = this;
}

FileMetaConstValueTerm::FileMetaConstValueTerm(FileMeta* fm, Token* valueToken, Token* signalToken) : FileMetaBaseTerm(fm) {
    m_FileMeta = fm;
    m_Token = valueToken;
    m_ValueToken = valueToken;
    m_SignalToken = signalToken;
    m_PlusOrMinusToken = signalToken;
    m_Root = this;
}

Token* FileMetaConstValueTerm::GetValueToken() const {
    return m_ValueToken;
}

Token* FileMetaConstValueTerm::GetPlusOrMinusToken() const {
    return m_PlusOrMinusToken;
}

void FileMetaConstValueTerm::BuildAST() {
    // Implementation for BuildAST
}

std::string FileMetaConstValueTerm::ToFormatString() const {
    if (m_Token != nullptr) {
        return m_Token->ToConstString();
    }
    return "";
}

std::string FileMetaConstValueTerm::ToTokenString() const {
    std::ostringstream sb;
    if (m_Token != nullptr) {
        sb << m_Token->ToLexemeAllString();
    }
    return sb.str();
}

// FileMetaCallTerm implementation
FileMetaCallTerm::FileMetaCallTerm(FileMeta* fm, Node* node) : FileMetaBaseTerm(fm) {
    m_FileMeta = fm;
    m_Root = this;
    m_CallNode = node;
    m_CallLink = new FileMetaCallLink(fm, node);
}

FileMetaCallLink* FileMetaCallTerm::GetCallLink() const {
    return m_CallLink;
}

Node* FileMetaCallTerm::GetCallNode() const {
    return m_CallNode;
}

void FileMetaCallTerm::BuildAST() {
    if (m_CallLink != nullptr) {
        for (size_t j = 0; j < m_CallLink->GetCallNodeList().size(); j++) {
            auto clc = m_CallLink->GetCallNodeList()[j];
            if (clc->GetFileMetaParTerm() != nullptr) {
                clc->GetFileMetaParTerm()->BuildAST();
            }
        }
    }
}

std::string FileMetaCallTerm::ToFormatString() const {
    std::ostringstream sb;
    if (m_CallLink != nullptr) {
        sb << m_CallLink->ToFormatString();
    }
    return sb.str();
}

std::string FileMetaCallTerm::ToTokenString() const {
    std::ostringstream sb;
    if (m_CallLink != nullptr) {
        sb << m_CallLink->ToTokenString();
    }
    return sb.str();
}

// FileMetaBraceTerm implementation
FileMetaBraceTerm::FileMetaBraceTerm(FileMeta* fm, Node* node) : FileMetaBaseTerm(fm) {
    m_FileMeta = fm;
    m_Root = this;
    m_Node = node;
    m_BraceNode = node;
    m_Token = m_Node->token;
    m_BraceEndToken = m_Node->endToken;
    HandleBraceTerm();

    if (m_BraceEndToken == nullptr) {
        Log::AddInStructFileMeta(EError::None, "Error FileMetaBraceTerm--");
    }
}

const std::vector<FileMetaSyntax*>& FileMetaBraceTerm::GetFileMetaAssignSyntaxList() const {
    return m_FileMetaAssignSyntaxList;
}

const std::vector<FileMetaCallLink*>& FileMetaBraceTerm::GetFileMetaCallLinkList() const {
    return m_FileMetaCallLinkList;
}

bool FileMetaBraceTerm::IsArray() const {
    return m_IsArray;
}

void FileMetaBraceTerm::SetIsArray(bool value) {
    m_IsArray = value;
}

void FileMetaBraceTerm::HandleBraceTerm() {
    // { a = 10, b = 20, c = Class1() }
    std::vector<std::vector<Node*>> nodeListList;
    std::vector<Node*> tempNodeList;
    
    for (size_t j = 0; j < m_Node->childList.size(); j++) {
        auto c2node = m_Node->childList[j];
        if (c2node->nodeType == ENodeType::Comma) {
            nodeListList.push_back(tempNodeList);
            tempNodeList.clear();
        } else if (c2node->nodeType == ENodeType::LineEnd) {
            continue;
        } else {
            tempNodeList.push_back(c2node);
        }
    }
    
    if (!tempNodeList.empty()) {
        nodeListList.push_back(tempNodeList);
    }

    size_t nodeListCount = nodeListList.size();
    for (size_t i = 0; i < nodeListCount; i++) {
        auto& nodeList = nodeListList[i];
        std::vector<Node*> defineNodeList;
        std::vector<Node*> valueNodeList;
        Token* assignToken = nullptr;
        
        for (size_t j = 0; j < nodeList.size(); j++) {
            auto nl2 = nodeList[j];
            if (nl2->nodeType == ENodeType::Assign) {
                if (assignToken == nullptr) {
                    assignToken = nl2->token;
                    continue;
                } else {
                    Log::AddInStructFileMeta(EError::None, " Errorr FileMetaBraceTerm.HandleBraceTerm 解析{ a = ?} 时，多个=号 Token: " + assignToken->ToLexemeAllString());
                }
            } else {
                if (assignToken == nullptr) {
                    defineNodeList.push_back(nl2);
                } else {
                    valueNodeList.push_back(nl2);
                }
            }
        }
        
        if (i == 0) {
            if (defineNodeList.size() == 1 && valueNodeList.empty()) {
                m_IsArray = true;
            } else {
                m_IsArray = false;
            }
        }

        if (m_IsArray) {
            if (defineNodeList.size() >= 1 && valueNodeList.empty()) {
                FileMetaCallLink* fmcl = new FileMetaCallLink(m_FileMeta, defineNodeList[0]);
                m_FileMetaCallLinkList.push_back(fmcl);
            } else {
                Log::AddInStructFileMeta(EError::None, "Error 在解析为{}中，数组形式 解析有问题!!");
                continue;
            }
        } else {
            if ((defineNodeList.size() != 1 && defineNodeList.size() != 2) || valueNodeList.size() < 1) {
                Log::AddInStructFileMeta(EError::None, "Error 在解析为{}中，赋值= 解析有问题!!");
                continue;
            }
            if (defineNodeList.size() == 2) {
                Token* nameToken = defineNodeList[1]->token;
                auto classRef = new FileMetaClassDefine(m_FileMeta, defineNodeList[0]);
                FileMetaBaseTerm* fmel = FileMetatUtil::CreateFileMetaExpress(m_FileMeta, valueNodeList, EExpressType::Common);
                FileMetaDefineVariableSyntax* fmdvs = new FileMetaDefineVariableSyntax(m_FileMeta, classRef, nameToken, assignToken, nullptr, fmel);
                // fmdvs->SetIsAppendSemiColon(false); // Method not available
                m_FileMetaAssignSyntaxList.push_back(fmdvs);
            } else {
                FileMetaCallLink* fmcl = new FileMetaCallLink(m_FileMeta, defineNodeList[0]);
                FileMetaBaseTerm* fmel = FileMetatUtil::CreateFileMetaExpress(m_FileMeta, valueNodeList, EExpressType::Common);
                FileMetaOpAssignSyntax* fmoas = new FileMetaOpAssignSyntax(fmcl, assignToken, nullptr, nullptr, nullptr, fmel, true);
                // fmoas->SetIsAppendSemiColon(false); // Method not available
                m_FileMetaAssignSyntaxList.push_back(fmoas);
            }
        }
    }
}

void FileMetaBraceTerm::ClearDirty() {
    FileMetaBaseTerm::ClearDirty();
}

void FileMetaBraceTerm::BuildAST() {
    // Implementation for BuildAST
}

std::string FileMetaBraceTerm::ToFormatString() const {
    std::ostringstream stringBuilder;
    if (m_Token != nullptr) {
        stringBuilder << m_Token->GetLexemeString();
    }
    if (m_IsArray) {
        for (size_t i = 0; i < m_FileMetaCallLinkList.size(); i++) {
            stringBuilder << m_FileMetaCallLinkList[i]->ToFormatString();
            if (i < m_FileMetaCallLinkList.size() - 1) {
                stringBuilder << ",";
            }
        }
    } else {
        for (size_t i = 0; i < m_FileMetaAssignSyntaxList.size(); i++) {
            stringBuilder << m_FileMetaAssignSyntaxList[i]->ToFormatString();
            if (i < m_FileMetaAssignSyntaxList.size() - 1) {
                stringBuilder << ",";
            }
        }
    }
    if (m_BraceEndToken != nullptr) {
        stringBuilder << m_BraceEndToken->GetLexemeString();
    }
    return stringBuilder.str();
}

std::string FileMetaBraceTerm::ToTokenString() const {
    std::ostringstream sb;
    if (m_Token != nullptr) {
        sb << "BeginBraceToken:" << m_Token->ToLexemeAllString();
    }
    if (m_BraceEndToken != nullptr) {
        sb << "EndBraceToken:" << m_BraceEndToken->ToLexemeAllString();
    }
    return sb.str();
}

// FileMetaBracketTerm implementation
FileMetaBracketTerm::FileMetaBracketTerm(FileMeta* fm, Node* node, int bracketType) : FileMetaBaseTerm(fm) {
    m_FileMeta = fm;
    m_Root = this;
    m_Token = node->token;
    m_BracketeEndToken = node->endToken;
    m_BracketNode = node;
    m_BracketType = bracketType;

    for (size_t i = 0; i < node->childList.size(); i++) {
        auto cnode = node->childList[i];
        if (cnode->nodeType == ENodeType::ConstValue) {
            auto fileMetaConstValueTerm = new FileMetaConstValueTerm(m_FileMeta, cnode->token);
            AddFileMetaTerm(fileMetaConstValueTerm);
        } else if (cnode->nodeType == ENodeType::Bracket) {
            auto fileMetaBracketTerm = new FileMetaBracketTerm(m_FileMeta, cnode);
            AddFileMetaTerm(fileMetaBracketTerm);
        } else if (cnode->nodeType == ENodeType::Comma) {
            auto fileMetaSymbolTerm = new FileMetaSymbolTerm(m_FileMeta, cnode->token);
            AddFileMetaTerm(fileMetaSymbolTerm);
        } else if (cnode->nodeType == ENodeType::Par) {
            Log::AddInStructFileMeta(EError::None, "Error 不支持在[]中解析()的逻辑!!");
            continue;
        } else if (cnode->nodeType == ENodeType::Key) {
            Log::AddInStructFileMeta(EError::None, "Error 不支持在[]中解析Key的逻辑!!");
            continue;
        } else if (cnode->nodeType == ENodeType::Brace) {
            auto fileMetaBraceTerm = new FileMetaBraceTerm(m_FileMeta, cnode);
            AddFileMetaTerm(fileMetaBraceTerm);
            continue;
        } else {
            auto fileMetaCallTerm = new FileMetaCallTerm(m_FileMeta, node);
            AddFileMetaTerm(fileMetaCallTerm);
        }
    }
}

Node* FileMetaBracketTerm::GetBracketNode() const {
    return m_BracketNode;
}

int FileMetaBracketTerm::GetBracketType() const {
    return m_BracketType;
}

Token* FileMetaBracketTerm::GetEndToken() const {
    return m_BracketeEndToken;
}

void FileMetaBracketTerm::BuildAST() {
    // Implementation for BuildAST
}

std::string FileMetaBracketTerm::ToFormatString() const {
    std::ostringstream stringBuilder;
    stringBuilder << "[";
    for (size_t i = 0; i < m_FileMetaExpressList.size(); i++) {
        stringBuilder << m_FileMetaExpressList[i]->ToFormatString();
    }
    stringBuilder << "]";
    return stringBuilder.str();
}

std::string FileMetaBracketTerm::ToTokenString() const {
    std::ostringstream sb;
    if (m_Token != nullptr) {
        sb << "BeginBraceToken:" << m_Token->ToLexemeAllString();
    }
    if (m_BracketeEndToken != nullptr) {
        sb << "EndBraceToken:" << m_BracketeEndToken->ToLexemeAllString();
    }
    return sb.str();
}

// FileMetaTermExpress implementation
FileMetaTermExpress::FileMetaTermExpress(FileMeta* fm, const std::vector<Node*>& nodeList, EExpressType expressType) : FileMetaBaseTerm(fm) {
    m_FileMeta = fm;
    m_NodeList = nodeList;
    m_ExpressType = expressType;
    
    Node* tn = new Node(nullptr);
    tn->childList = nodeList;
    auto childList = StructParse::HandleExpressNode(tn);
    
    CreateFileMetaExpressByChildList(childList);
}

const std::vector<Node*>& FileMetaTermExpress::GetNodeList() const {
    return m_NodeList;
}

EExpressType FileMetaTermExpress::GetExpressType() const {
    return m_ExpressType;
}

bool FileMetaTermExpress::GetCanUseDoublePlusOrMinus() const {
    return m_CanUseDoublePlusOrMinus;
}

void FileMetaTermExpress::SetCanUseDoublePlusOrMinus(bool value) {
    m_CanUseDoublePlusOrMinus = value;
}

void FileMetaTermExpress::SetDeep(int deep) {
    m_Deep = deep;
    for (size_t i = 0; i < m_FileMetaExpressList.size(); i++) {
        m_FileMetaExpressList[i]->SetDeep(deep);
    }
}

void FileMetaTermExpress::CreateFileMetaExpressByChildList(const std::vector<Node*>& nodeList) {
    if (nodeList.empty()) return;
    
    FileMetaBaseTerm* fmbt = nullptr;
    
    for (size_t i = 0; i < nodeList.size(); i++) {
        auto node = nodeList[i];
        if (node->nodeType == ENodeType::Symbol) {
            FileMetaSymbolTerm* fmn = new FileMetaSymbolTerm(m_FileMeta, node->token);
            fmn->SetPriority();
            AddFileMetaTerm(fmn);
            fmbt = nullptr;
        } else if (node->nodeType == ENodeType::LeftAngle || node->nodeType == ENodeType::RightAngle) {
            FileMetaSymbolTerm* fmn = new FileMetaSymbolTerm(m_FileMeta, node->token);
            fmn->SetPriority();
            AddFileMetaTerm(fmn);
            fmbt = nullptr;
        } else if (node->nodeType == ENodeType::Brace) {
            if (fmbt != nullptr) {
                Log::AddInStructFileMeta(EError::None, "Error 表达式不允许多个自定义元素存在!!" + fmbt->ToTokenString());
            }
            fmbt = new FileMetaCallTerm(m_FileMeta, node);
            fmbt->SetPriority(std::numeric_limits<int>::max());
            AddFileMetaTerm(fmbt);
        } else if (node->nodeType == ENodeType::ConstValue) {
            if (node->GetExtendLinkNodeList().size() > 0) {
                fmbt = new FileMetaCallTerm(m_FileMeta, node);
                fmbt->SetPriority(std::numeric_limits<int>::max());
            } else {
                fmbt = new FileMetaConstValueTerm(m_FileMeta, node->token);
                fmbt->SetPriority(std::numeric_limits<int>::max());
            }
            AddFileMetaTerm(fmbt);
        } else if (node->nodeType == ENodeType::Key && 
                   (node->token->GetType() == ETokenType::This ||
                    node->token->GetType() == ETokenType::Base ||
                    node->token->GetType() == ETokenType::New)) {
            if (fmbt != nullptr) {
                Log::AddInStructFileMeta(EError::None, "Error 表达式不允许多个自定义元素存在!!" + fmbt->ToTokenString());
            }
            fmbt = new FileMetaCallTerm(m_FileMeta, node);
            fmbt->SetPriority(std::numeric_limits<int>::max());
            AddFileMetaTerm(fmbt);
        } else if (node->nodeType == ENodeType::IdentifierLink) {
            if (fmbt != nullptr) {
                Log::AddInStructFileMeta(EError::None, "Error 表达式不允许多个自定义元素存在!!" + fmbt->ToTokenString());
            }
            fmbt = new FileMetaCallTerm(m_FileMeta, node);
            fmbt->SetPriority(std::numeric_limits<int>::max());
            AddFileMetaTerm(fmbt);
        } else if (node->nodeType == ENodeType::Par) {
            if (node->GetExtendLinkNodeList().size() > 0) {
                fmbt = new FileMetaCallTerm(m_FileMeta, node);
                fmbt->SetPriority(std::numeric_limits<int>::max());
            } else {
                fmbt = new FileMetaParTerm(m_FileMeta, node, m_ExpressType);
                fmbt->SetPriority(SignComputePriority::Level1);
            }
            AddFileMetaTerm(fmbt);
        } else if (node->nodeType == ENodeType::Key && node->token->GetType() == ETokenType::QuestionMark) {
            if (fmbt == nullptr) {
                Log::AddInStructFileMeta(EError::None, "如果使用了三元表达式，必须在第一位需要放置返回式: " + std::to_string(static_cast<int>(node->token->GetType())) + " 位置: " + node->token->ToLexemeAllString());
            }
            auto nfmbt = new FileMetaThreeItemSyntaxTerm(m_FileMeta, node);
            nfmbt->SetPriority(SignComputePriority::Level1);
            AddFileMetaTerm(nfmbt);
        } else {
            Log::AddInStructFileMeta(EError::None, "没有找到该类型: " + std::to_string(static_cast<int>(node->token->GetType())) + " 位置: " + node->token->ToLexemeAllString());
        }
    }
}

bool FileMetaTermExpress::BuildTst(std::vector<FileMetaBaseTerm*>& list) {
    if (list.empty()) return false;
    
    if (list.size() == 1) {
        m_Root = list[0];
        return true;
    }
    
    int maxLevel = std::numeric_limits<int>::max();
    int index = -1;
    
    for (size_t i = 0; i < list.size(); i++) {
        if (maxLevel > list[i]->GetPriority() && !list[i]->GetDirty()) {
            maxLevel = list[i]->GetPriority();
            index = static_cast<int>(i);
        }
    }
    
    if (index >= 0 && index < static_cast<int>(list.size())) {
        FileMetaBaseTerm* currentTerm = list[index];
        FileMetaBaseTerm* listFrontTerm = nullptr;
        FileMetaBaseTerm* listNextTerm = nullptr;
        
        if (index > 0) {
            listFrontTerm = list[index - 1]->GetRoot();
        }
        if (index < static_cast<int>(list.size()) - 1) {
            listNextTerm = list[index + 1]->GetRoot();
        }
        
        if (currentTerm->GetPriority() == SignComputePriority::Level2_LinkOp) {
            ETokenType ett = currentTerm->GetToken()->GetType();
            if (!m_CanUseDoublePlusOrMinus && (ett == ETokenType::DoubleMinus || ett == ETokenType::DoublePlus)) {
                Log::AddInStructFileMeta(EError::None, "Error 只有在语句中，可以使用i++ 等语法，变量与传参是禁止使用i++ Token 位置:" + currentTerm->GetToken()->ToAllString());
                return false;
            }
            
            if (ett == ETokenType::DoubleMinus || ett == ETokenType::DoublePlus) {
                bool con1 = listFrontTerm != nullptr && 
                           (!dynamic_cast<FileMetaSymbolTerm*>(listFrontTerm) || listFrontTerm->GetDirty());
                if (listNextTerm == nullptr && con1) {
                    currentTerm->SetLeft(listFrontTerm);
                    list.erase(list.begin() + index - 1);
                }
            } else if (ett == ETokenType::Minus || ett == ETokenType::Not || ett == ETokenType::Negative) {
                if (listNextTerm == nullptr) {
                    Log::AddInStructFileMeta(EError::None, "Error 表达式解析错误!! FileMetaExpress 575");
                    return false;
                }
                currentTerm->SetRight(listNextTerm);
                if (listNextTerm != nullptr) {
                    list.erase(list.begin() + index + 1);
                }
            } else {
                Log::AddInStructFileMeta(EError::None, "Error 不能使用错误符号 !! FileMetaExpress 698" + currentTerm->GetToken()->ToLexemeAllString());
                return false;
            }
        } else {
            if (listFrontTerm != nullptr && listNextTerm != nullptr) {
                currentTerm->SetLeft(listFrontTerm);
                currentTerm->SetRight(listNextTerm);
                if (listFrontTerm != nullptr) {
                    list.erase(list.begin() + index - 1);
                }
                if (listNextTerm != nullptr) {
                    list.erase(list.begin() + index);
                }
            } else {
                Log::AddInStructFileMeta(EError::None, "Error BuildTst 表达式解析错误!! 604");
                return false;
            }
        }
        
        if (list.size() == 1) {
            m_Root = list[0];
            return true;
        }
    } else {
        Log::AddInStructFileMeta(EError::None, "选择已经超出来范围!!");
        return false;
    }
    
    return BuildTst(list);
}

void FileMetaTermExpress::BuildAST() {
    std::vector<FileMetaBaseTerm*> buildASTList = m_FileMetaExpressList;
    
    // Check length
    for (size_t i = 0; i < buildASTList.size(); i++) {
        auto fmst = dynamic_cast<FileMetaSymbolTerm*>(buildASTList[i]);
        if (fmst != nullptr) {
            auto ttoken = fmst->GetToken();
            if (ttoken != nullptr && (ttoken->GetType() == ETokenType::Plus || ttoken->GetType() == ETokenType::Minus)) {
                if (i > 0 && i != buildASTList.size() - 1) {
                    auto fmst1 = dynamic_cast<FileMetaSymbolTerm*>(buildASTList[i - 1]);
                    if ((fmst1 != nullptr && fmst1->GetPriority() == SignComputePriority::Level2_LinkOp) || (fmst1 == nullptr)) {
                        fmst->SetPriority();
                    }
                } else if (i < buildASTList.size() - 1 && i != 0) {
                    auto fmst1 = dynamic_cast<FileMetaSymbolTerm*>(buildASTList[i + 1]);
                    if ((fmst1 != nullptr && fmst1->GetPriority() == SignComputePriority::Level2_LinkOp) || (fmst1 == nullptr)) {
                        fmst->SetPriority();
                    }
                }
            }
        }
        buildASTList[i]->BuildAST();
    }
    
    m_Root = nullptr;
    BuildTst(buildASTList);
}

std::string FileMetaTermExpress::ToFormatString() const {
    std::ostringstream sb;
    FileMetaBaseTerm* beforeFMTE = nullptr;
    
    for (size_t i = 0; i < m_FileMetaExpressList.size(); i++) {
        auto cur = m_FileMetaExpressList[i];
        if (beforeFMTE != nullptr) {
            sb << " ";
        }
        sb << cur->ToFormatString();
        beforeFMTE = cur;
    }
    return sb.str();
}

std::string FileMetaTermExpress::ToTokenString() const {
    std::ostringstream sb;
    sb << "Express Tokens: ";
    for (size_t i = 0; i < m_FileMetaExpressList.size(); i++) {
        auto fme = m_FileMetaExpressList[i];
        sb << " " << fme->ToTokenString();
    }
    return sb.str();
}

// FileMetaIfSyntaxTerm implementation
FileMetaIfSyntaxTerm::FileMetaIfSyntaxTerm(FileMeta* fm, Node* node) : FileMetaBaseTerm(fm) {
    m_FileMeta = fm;
    m_IfNode = node;
    // Note: m_IfSyntax initialization would need FileMetaKeyIfSyntax constructor
}

Node* FileMetaIfSyntaxTerm::GetIfNode() const {
    return m_IfNode;
}

void FileMetaIfSyntaxTerm::BuildAST() {
    // Implementation for BuildAST
}

std::string FileMetaIfSyntaxTerm::ToFormatString() const {
    std::ostringstream sb;
    sb << "\n";
    if (m_IfSyntax != nullptr) {
        sb << m_IfSyntax->ToFormatString();
    }
    return sb.str();
}

FileMetaKeyIfSyntax* FileMetaIfSyntaxTerm::GetIfSyntax() {
    return m_IfSyntax;
}

FileMetaKeyIfSyntax* FileMetaIfSyntaxTerm::GetIfSyntax() const {
    return m_IfSyntax;
}

void FileMetaIfSyntaxTerm::SetDeep(int deep) {
    m_Deep = deep;
    if (m_IfSyntax != nullptr) {
        m_IfSyntax->SetDeep(deep);
    }
}

// FileMetaThreeItemSyntaxTerm implementation
FileMetaThreeItemSyntaxTerm::FileMetaThreeItemSyntaxTerm(FileMeta* fm, Node* node) : FileMetaBaseTerm(fm) {
    m_FileMeta = fm;
    m_ThreeItemNode = node;
}

Node* FileMetaThreeItemSyntaxTerm::GetThreeItemNode() const {
    return m_ThreeItemNode;
}

FileMetaBaseTerm* FileMetaThreeItemSyntaxTerm::GetReturn1Term() const {
    return m_Return1Term;
}

void FileMetaThreeItemSyntaxTerm::SetReturn1Term(FileMetaBaseTerm* value) {
    m_Return1Term = value;
}

FileMetaBaseTerm* FileMetaThreeItemSyntaxTerm::GetReturn2Term() const {
    return m_Return2Term;
}

void FileMetaThreeItemSyntaxTerm::SetReturn2Term(FileMetaBaseTerm* value) {
    m_Return2Term = value;
}

FileMetaBaseTerm* FileMetaThreeItemSyntaxTerm::GetConditionTerm() const {
    return m_ConditionTerm;
}

void FileMetaThreeItemSyntaxTerm::SetConditionTerm(FileMetaBaseTerm* value) {
    m_ConditionTerm = value;
}

void FileMetaThreeItemSyntaxTerm::SetItemTerm(int i, FileMetaBaseTerm* fmbt) {
    if (i == 1) {
        m_Return1Term = fmbt;
    } else if (i == 2) {
        m_ConditionTerm = fmbt;
    } else if (i == 3) {
        m_Return2Term = fmbt;
    }
}

void FileMetaThreeItemSyntaxTerm::SetDeep(int deep) {
    m_Deep = deep;
}

void FileMetaThreeItemSyntaxTerm::BuildAST() {
    // Implementation for BuildAST
}

std::string FileMetaThreeItemSyntaxTerm::ToFormatString() const {
    std::ostringstream sb;
    sb << "\n";
    return sb.str();
}

// FileMetaMatchSyntaxTerm implementation
FileMetaMatchSyntaxTerm::FileMetaMatchSyntaxTerm(FileMeta* fm, Node* node) : FileMetaBaseTerm(fm) {
    m_FileMeta = fm;
    m_MatchNode = node;
    // Note: m_SwitchSyntax initialization would need FileMetaKeySwitchSyntax constructor
}

Node* FileMetaMatchSyntaxTerm::GetMatchNode() const {
    return m_MatchNode;
}

FileMetaKeySwitchSyntax* FileMetaMatchSyntaxTerm::GetSwitchSyntax() const {
    return m_SwitchSyntax;
}

void FileMetaMatchSyntaxTerm::SetDeep(int deep) {
    m_Deep = deep;
    if (m_SwitchSyntax != nullptr) {
        m_SwitchSyntax->SetDeep(deep);
    }
}

void FileMetaMatchSyntaxTerm::BuildAST() {
    // Implementation for BuildAST
}

std::string FileMetaMatchSyntaxTerm::ToFormatString() const {
    std::ostringstream sb;
    sb << "\n";
    if (m_SwitchSyntax != nullptr) {
        sb << m_SwitchSyntax->ToFormatString();
    }
    return sb.str();
}

} // namespace Compile
} // namespace SimpleLanguage
