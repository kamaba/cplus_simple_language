#include "FileMetaMemberVariable.h"
#include "FileMeta.h"
#include "FileMetaClass.h"
#include "FileMetaExpress.h"
#include "FileMetatUtil.h"
#include "../CompilerUtil.h"
#include "../../Debug/Log.h"
#include "../../Define.h"
#include "../Parse/StructParse.h"
#include <algorithm>

namespace SimpleLanguage {
namespace Compile {

FileMetaMemberVariable::FileMetaMemberVariable(FileMeta* fm, const std::vector<Node*>& list) 
    : m_NodeList(list) {
    m_FileMeta = fm;
    ParseBuildMetaVariable();
}

FileMetaMemberVariable::FileMetaMemberVariable(FileMeta* fm, Node* brace) {
    // ���� {}�е����ݣ�һ�����ڽ�����������

    m_FileMeta = fm;
    m_Token = brace->token;

    std::vector<Node*> list2;
    for (size_t i = 0; i < brace->childList.size(); i++) {
        Node* c = brace->childList[i];
        if (c->nodeType == SimpleLanguage::Compile::ENodeType::LineEnd || 
            c->nodeType == SimpleLanguage::Compile::ENodeType::SemiColon) {
            if (list2.empty()) {
                continue;
            }
            auto cfm = new FileMetaMemberVariable(m_FileMeta, list2);
            list2.clear();

            AddFileMemberVariable(cfm);
            continue;
        }
        list2.push_back(c);
    }
}

FileMetaMemberVariable::FileMetaMemberVariable(FileMeta* fm, Node* _beforeNode, Node* _afterNode, EMemberDataType dataType)
    : m_MemberDataType(dataType) {
    m_FileMeta = fm;
    m_Token = _beforeNode->token;
    
    bool isParseBuild = true;
    if (dataType == EMemberDataType::NameClass) {
        // Implementation for NameClass
    } else if (dataType == EMemberDataType::KeyValue) {
        m_Express = new FileMetaConstValueTerm(m_FileMeta, _afterNode->token);
    } else if (dataType == EMemberDataType::Array) {
        isParseBuild = false;
        m_Express = new FileMetaBracketTerm(m_FileMeta, _beforeNode, 1);
    } else if (dataType == EMemberDataType::ConstVariable) {
        m_Express = new FileMetaConstValueTerm(m_FileMeta, _beforeNode->token);
    }

    if (isParseBuild) {
        ParseBuildMetaVariable();
    }
}

bool FileMetaMemberVariable::ParseBuildMetaVariable() {
    std::vector<Node*> bedoreNodeList;
    std::vector<Node*> afterNodeList;

    if (!FileMetatUtil::SplitNodeList(m_NodeList, bedoreNodeList, afterNodeList, m_AssignToken)) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error ����NodeList���ִ���~~~");
        return false;
    }
    if (bedoreNodeList.empty()) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error listDefieNode ����Ϊ��~");
        return false;
    }

    Node* beforeNode = new Node(nullptr);
    beforeNode->childList = bedoreNodeList;
    //beforeNode->setParseIndex(0);
    auto defineNodeList = StructParse::HandleBeforeNode(beforeNode);

    Node* nameNode = nullptr;
    Node* typeNode = nullptr;
    Node* mutNode = nullptr;
    if (!GetNameAndTypeToken(defineNodeList, typeNode, mutNode, nameNode, m_PermissionToken, m_StaticToken)) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error û���ҵ��ö������� ����ʹ����: X = 103; �ĸ�ʽ");
        return false;
    }

    if (nameNode == nullptr) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error û���ҵ��ö������� ����ʹ����: X = 104; �ĸ�ʽ");
        return false;
    }
    if (!nameNode->GetExtendLinkNodeList().empty()) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error û���ҵ��ö������� ����ʹ����: X = 105; �ĸ�ʽ");
        return false;
    }
    m_Token = nameNode->token;

    if (typeNode != nullptr) {
        m_ClassDefineRef = new FileMetaClassDefine(m_FileMeta, typeNode, mutNode);
        m_MemberDataType = EMemberDataType::ConstVariable;
    }

    if (!afterNodeList.empty()) {
        if (afterNodeList[0]->nodeType == SimpleLanguage::Compile::ENodeType::Bracket) {
            m_MemberDataType = EMemberDataType::Array;
            ParseBracketContrent(afterNodeList[0]);
        } else {
            m_MemberDataType = EMemberDataType::ConstVariable;
            m_Express = SimpleLanguage::Compile::FileMetatUtil::CreateFileMetaExpress(m_FileMeta, afterNodeList, 
                EExpressType::MemberVariable);
        }
    }

    return true;
}

void FileMetaMemberVariable::ParseBracketContrent(Node* pnode) {
    int type = -1;
    for (size_t index = 0; index < pnode->childList.size(); index++) {
        auto curNode = pnode->childList[index];
        if (curNode->nodeType == SimpleLanguage::Compile::ENodeType::LineEnd ||
            curNode->nodeType == SimpleLanguage::Compile::ENodeType::SemiColon ||
            curNode->nodeType == SimpleLanguage::Compile::ENodeType::Comma) {
            continue;
        }
        if (curNode->nodeType == SimpleLanguage::Compile::ENodeType::IdentifierLink) {
            // aaa(){},aaa(){}
        }
        if (curNode->nodeType == SimpleLanguage::Compile::ENodeType::Brace) {  // Class1 [{},{}]
            if (type == 2 || type == 3) {
                SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, 
                    "Error Data������ []�У���֧�ָ����͵�����" + (curNode->token != nullptr ? curNode->token->ToLexemeAllString() : ""));
                continue;
            }

            type = 1;

            auto fmmd = new FileMetaMemberVariable(m_FileMeta, curNode, nullptr, EMemberDataType::Array);
            AddFileMemberVariable(fmmd);
        } else if (curNode->nodeType == SimpleLanguage::Compile::ENodeType::ConstValue) {  // ["stringValue","Stvlue"]
            if (type == 1 || type == 3) {
                SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, 
                    "Error Data������ []�У���֧�ָ����͵�����" + (curNode->token != nullptr ? curNode->token->ToLexemeAllString() : ""));
                continue;
            }

            type = 2;

            auto fmmd = new FileMetaMemberVariable(m_FileMeta, curNode, nullptr, EMemberDataType::ConstVariable);
            AddFileMemberVariable(fmmd);
        } else if (curNode != nullptr && curNode->nodeType == SimpleLanguage::Compile::ENodeType::Bracket) {  // [[],[]]
            if (type == 1 || type == 2) {
                SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, 
                    "Error Data������ []�У���֧�ָ����͵�����" + (curNode->token != nullptr ? curNode->token->ToLexemeAllString() : ""));
                continue;
            }

            type = 3;

            auto fmmd = new FileMetaMemberVariable(m_FileMeta, curNode, nullptr, EMemberDataType::Array);
            AddFileMemberVariable(fmmd);
        } else {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, 
                "Error Data������ []�У���֧�ָ����͵�����" + (curNode->token != nullptr ? curNode->token->ToLexemeAllString() : ""));
            continue;
        }
    }
}

void FileMetaMemberVariable::ParseFileMemberVariableContent() {
    // Implementation for parsing file member variable content
    // This method contains commented out code in the original C# version
}

bool FileMetaMemberVariable::GetNameAndTypeToken(const std::vector<Node*>& defineNodeList, Node*& typeNode, Node*& mutNode, 
                                                 Node*& nameNode, Token*& permissionToken, Token*& staticToken) {
    bool isError = false;

    std::vector<Node*> nodeList;
    for (size_t i = 0; i < defineNodeList.size(); i++) {
        auto cnode = defineNodeList[i];

        if (cnode->nodeType == SimpleLanguage::Compile::ENodeType::IdentifierLink) {
            nodeList.push_back(cnode);
        } else {
            auto token = cnode->token;
            if (token->GetType() == SimpleLanguage::ETokenType::Public ||
                token->GetType() == SimpleLanguage::ETokenType::Projected ||
                token->GetType() == SimpleLanguage::ETokenType::Extern ||
                token->GetType() == SimpleLanguage::ETokenType::Private) {
                if (permissionToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error ���ض������Ƶ�Ȩ�޶���!!");
                }
                permissionToken = token;
            } else if (token->GetType() == SimpleLanguage::ETokenType::Static) {
                if (staticToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error ���ض������Ƶľ�̬����!!");
                }
                staticToken = token;
            } else if (token->GetType() == SimpleLanguage::ETokenType::Type) {
                nodeList.push_back(cnode);
            } else if (token->GetType() == SimpleLanguage::ETokenType::Mut) {
                if (mutNode != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error ���ض������Ƶ�Mut����!!");
                }
                mutNode = cnode;
            } else {
                SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, 
                    "Error ���������У�����������ʹ���!!" + token->ToLexemeAllString());
            }
        }
    }

    if (nodeList.size() == 1) {
        nameNode = nodeList[0];
    } else if (nodeList.size() == 2) {
        typeNode = nodeList[0];
        nameNode = nodeList[1];
    }

    return !isError;
}

void FileMetaMemberVariable::AddFileMemberVariable(FileMetaMemberVariable* fmmd) {
    m_FileMetaMemberVariableList.push_back(fmmd);
}

FileMetaMemberVariable* FileMetaMemberVariable::GetFileMetaMemberDataByName(const std::string& name) {
    auto it = std::find_if(m_FileMetaMemberVariableList.begin(), m_FileMetaMemberVariableList.end(),
        [&name](FileMetaMemberVariable* fmmd) { return fmmd->Name() == name; });
    
    if (it != m_FileMetaMemberVariableList.end()) {
        return *it;
    }
    return nullptr;
}

void FileMetaMemberVariable::SetDeep(int _deep) {
    m_Deep = _deep;

    for (size_t i = 0; i < m_FileMetaMemberVariableList.size(); i++) {
        m_FileMetaMemberVariableList[i]->SetDeep(m_Deep + 1);
    }
}

std::string FileMetaMemberVariable::ToString() const {
    return m_Token != nullptr ? m_Token->GetLexemeString() : "";
}

std::string FileMetaMemberVariable::ToFormatString() const {
    std::ostringstream sb;
    /*for (int i = 0; i < deep(); i++)
        sb << SimpleLanguage::Global::tabChar;
    
    SimpleLanguage::Core::EPermission permis = SimpleLanguage::Compile::CompilerUtil::GetPerMissionByString(
        m_PermissionToken ? m_PermissionToken->lexeme().ToString() : "");
    if (permis == SimpleLanguage::Core::EPermission::Null) {
        sb << "_public ";
    } else {
        sb << SimpleLanguage::Compile::CompilerUtil::ToFormatString(permis);
    }
    if (m_StaticToken != nullptr) {
        sb << " " << m_StaticToken->lexeme().ToString();
    }

    if (m_MemberDataType == EMemberDataType::NameClass) {
        for (int i = 0; i < deep(); i++)
            sb << SimpleLanguage::Global::tabChar;
        sb << name() << std::endl;
        for (int i = 0; i < deep(); i++)
            sb << SimpleLanguage::Global::tabChar;
        sb << "{" << std::endl;
        for (size_t i = 0; i < m_FileMetaMemberVariableList.size(); i++) {
            sb << m_FileMetaMemberVariableList[i]->ToFormatString() << std::endl;
        }
        for (int i = 0; i < deep(); i++)
            sb << SimpleLanguage::Global::tabChar;
        sb << "}";

        if (m_ClassDefineRef != nullptr)
            sb << " " << m_ClassDefineRef->ToFormatString();
        sb << " " << name();
        if (m_AssignToken != nullptr) {
            sb << " " << m_AssignToken->lexeme().ToString();
            sb << " " << (m_Express ? m_Express->ToFormatString() : "");
        }
        sb << ";";
    } else if (m_MemberDataType == EMemberDataType::KeyValue) {
        for (int i = 0; i < deep(); i++)
            sb << SimpleLanguage::Global::tabChar;
        sb << name() << " = ";
        sb << (m_Express ? m_Express->ToFormatString() : "");
        sb << ";";
    } else if (m_MemberDataType == EMemberDataType::Array) {
        for (int i = 0; i < deep(); i++)
            sb << SimpleLanguage::Global::tabChar;
        sb << name() << " = [";
        for (size_t i = 0; i < m_FileMetaMemberVariableList.size(); i++) {
            sb << m_FileMetaMemberVariableList[i]->ToFormatString();
            if (i < m_FileMetaMemberVariableList.size() - 1) {
                sb << ",";
            }
        }
        sb << "]";
        sb << ";";
    } else if (m_MemberDataType == EMemberDataType::ConstVariable) {
        sb << (m_ClassDefineRef ? m_ClassDefineRef->ToFormatString() : "");
        sb << " " << name() << " = ";
        sb << (m_Express ? m_Express->ToFormatString() : "");
    } else {
        sb << "û�в��MemberDataType";
    }*/
    return sb.str();
}

} // namespace Compile
} // namespace SimpleLanguage
