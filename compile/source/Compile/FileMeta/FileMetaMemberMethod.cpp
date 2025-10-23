#include "FileMetaMemberMethod.h"
#include "FileMeta.h"
#include "FileMetaSyntax.h"
#include "../FileMeta/FileMetatUtil.h"
#include "../CompilerUtil.h"
#include "../../Debug/Log.h"
#include "../../Define.h"
#include "../Parse/StructParse.h"
#include <algorithm>
#include <unordered_set>

namespace SimpleLanguage {
namespace Compile {

// FileMetaParamterDefine implementation
FileMetaParamterDefine::FileMetaParamterDefine(FileMeta* fileMeta, const std::vector<Node*>& list) 
{
    m_FileMeta = fileMeta;
    ParseBuildMetaParamter(list);
}
bool FileMetaParamterDefine::ParseBuildMetaParamter(const std::vector<Node*>& inputNodeList) {
    if (inputNodeList.empty()) return false;

    std::vector<Node*> listDefieNode;
    std::vector<Node*> valueNodeList;
    Node* beforeNode = new Node(nullptr);
    beforeNode->childList = inputNodeList;
    beforeNode->parseIndex = 0;
    auto nodeList = SimpleLanguage::Compile::StructParse::HandleBeforeNode(beforeNode);
    
    if (!SimpleLanguage::Compile::FileMetatUtil::SplitNodeList(nodeList, listDefieNode, valueNodeList, m_AssignToken)) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error 解析NodeList出现错误~~~");
        return false;
    }
    if (!valueNodeList.empty())
        m_Express = SimpleLanguage::Compile::FileMetatUtil::CreateFileMetaExpress(m_FileMeta, valueNodeList, 
            EExpressType::ParamVariable);

    Node* nameNode = nullptr;
    Node* typeNode = nullptr;
    if (!GetNameAndTypeNode(listDefieNode, nameNode, typeNode, m_ParamsToken)) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::UnMatchChar, 
            "Error 没有找到该定义名称 必须使用例: X = 102; 的格式");
        return false;
    }
    if (nameNode == nullptr) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, 
            "Error 没有找到该定义名称 必须使用例: X = 101; 的格式");
        return false;
    }
    m_Token = nameNode->token;

    if (typeNode != nullptr)
        m_ClassDefineRef = new FileMetaClassDefine(m_FileMeta, typeNode);

    return true;
}

bool FileMetaParamterDefine::GetNameAndTypeNode(const std::vector<Node*>& listDefieNode, Node*& nameNode, Node*& typeNode, Token*& paramstoken) {
    std::vector<Node*> removeNodeList;
    for (size_t i = 0; i < listDefieNode.size() - 1; i++) {
        auto curNode = listDefieNode[i];
        Node* nextNode = listDefieNode[i + 1];
        if (nextNode->nodeType == SimpleLanguage::Compile::ENodeType::Bracket) {
            curNode->bracketNode = (nextNode);
            removeNodeList.push_back(nextNode);
        } else if (curNode->nodeType == SimpleLanguage::Compile::ENodeType::Key && 
                  curNode->token != nullptr && 
                  curNode->token->GetType() == SimpleLanguage::ETokenType::Params) {
            typeNode = curNode;
            paramstoken = curNode->token;
            removeNodeList.push_back(curNode);
        }
    }
    
    for (size_t i = 0; i < removeNodeList.size(); i++) {
        auto it = std::find(listDefieNode.begin(), listDefieNode.end(), removeNodeList[i]);
        if (it != listDefieNode.end()) {
            //listDefieNode.erase(it);
        }
    }

    if (listDefieNode.size() == 2) {
        typeNode = listDefieNode[0];
        nameNode = listDefieNode[1];
    } else if (listDefieNode.size() == 1) {
        nameNode = listDefieNode[0];
    } else {
        return false;
    }
    return true;
}

std::string FileMetaParamterDefine::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < Deep(); i++)
        sb << SimpleLanguage::Global::tabChar;
    if (m_ClassDefineRef != nullptr)
        sb << " " << m_ClassDefineRef->ToFormatString();
    sb << " " << (m_Token ? m_Token->GetLexemeString() : "");
    if (m_AssignToken != nullptr) {
        sb << " " << m_AssignToken->GetLexemeString();
        sb << " " << (m_Express ? m_Express->ToFormatString() : "");
    }
    return sb.str();
}

// FileMetaFunction implementation
FileMetaFunction::FileMetaFunction() {
}

void FileMetaFunction::AddMetaParamter(FileMetaParamterDefine* fmp) {
    m_MetaParamtersList.push_back(fmp);
    fmp->SetFileMeta(m_FileMeta);
}

void FileMetaFunction::AddFileMetaSyntax(FileMetaSyntax* fms) {
    if (m_FileMetaBlockSyntax != nullptr) {
        m_FileMetaBlockSyntax->AddFileMetaSyntax(fms);
    }
    fms->SetFileMeta(m_FileMeta);
}

void FileMetaFunction::AddMetaTemplate(FileMetaTemplateDefine* fmtd) {
    m_MetaTemplatesList.push_back(fmtd);
    fmtd->SetFileMeta(m_FileMeta);
}

// FileMetaMemberFunction implementation
FileMetaMemberFunction::FileMetaMemberFunction(FileMeta* fm, Node* block, const std::vector<Node*>& nodeList) 
    : m_BlockNode(block) {
    m_FileMeta = fm;
    ParseFunction(nodeList);
}

bool FileMetaMemberFunction::ParseFunction(const std::vector<Node*>& nodeList) {
    Token* permissionToken = nullptr;
    Token* virtualToken = nullptr;
    int addCount = 0;
    bool isError = false;
    Node* returnClassNameNode = nullptr;
    Token* interfaceToken = nullptr;
    Token* staticToken = nullptr;
    Token* getToken = nullptr;
    Token* setToken = nullptr;
    Token* finalToken = nullptr;
    std::vector<Token*> inheritNameTokenList;
    std::vector<Token*> interfaceNameTokenList;
    std::vector<std::vector<Token*>> interfaceTokenList;
    std::vector<Token*> list;
    Node* funNameNode = nullptr;
    Node* node = new Node(nullptr);
    node->childList = nodeList;
    auto nodeList2 = SimpleLanguage::Compile::StructParse::HandleBeforeNode(node);
    
    while (addCount < static_cast<int>(nodeList2.size())) {
        auto cnode = nodeList2[addCount++];

        if (cnode->nodeType == SimpleLanguage::Compile::ENodeType::IdentifierLink) {
            if (cnode->parNode != nullptr) {
                if (funNameNode != nullptr) {
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::UnMatchChar, 
                        "Error 已有函数实体，不能同时出现两个函数实体!");
                }
                funNameNode = cnode;
            } else {
                returnClassNameNode = cnode;
            }
        } else {
            auto token = cnode->token;
            if (token->GetType() == SimpleLanguage::ETokenType::Public ||
                token->GetType() == SimpleLanguage::ETokenType::Private ||
                token->GetType() == SimpleLanguage::ETokenType::Projected ||
                token->GetType() == SimpleLanguage::ETokenType::Extern) {
                if (permissionToken == nullptr) {
                    permissionToken = token;
                } else {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error 解析过了一次权限!!");
                }
            } else if (token->GetType() == SimpleLanguage::ETokenType::Override) {
                if (virtualToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error 解析过了一次Override!!");
                }
                virtualToken = token;
            } else if (token->GetType() == SimpleLanguage::ETokenType::Static) {
                staticToken = token;
            } else if (token->GetType() == SimpleLanguage::ETokenType::Get) {
                if (getToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, " Error 解析类型多个get");
                }
                getToken = token;
            } else if (token->GetType() == SimpleLanguage::ETokenType::Set) {
                if (setToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, " Error 解析类型多个set");
                }
                setToken = token;
            } else if (token->GetType() == SimpleLanguage::ETokenType::Type ||
                      token->GetType() == SimpleLanguage::ETokenType::Void) {
                returnClassNameNode = cnode;
            } else if (token->GetType() == SimpleLanguage::ETokenType::Interface) {
                if (interfaceToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, 
                        "Error 解析interface已使用过一次，不允许重复使用!!");
                }
                interfaceToken = token;
            } else if (token->GetType() == SimpleLanguage::ETokenType::Final) {
                if (finalToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, " Error 解析类型多个final");
                }
                finalToken = token;
            } else {
                isError = true;
                SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, 
                    "Error 有其它未知类型在class中", token);
                break;
            }
        }
    }
    
    if (funNameNode == nullptr) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, 
            "Eror 没有找到合适的函数类型: 位置: " + (nodeList.empty() ? "" : (nodeList[0]->token != nullptr ? nodeList[0]->token->ToLexemeAllString() : "")));
        return false;
    }

    ParseParam(funNameNode->parNode);
    ParseTemplate(funNameNode->angleNode);

    m_Token = funNameNode->token;
    if (m_BlockNode != nullptr) {
        m_LeftBraceToken = m_BlockNode->token;
        m_RightBraceToken = m_BlockNode->endToken;
        m_FileMetaBlockSyntax = new FileMetaBlockSyntax(m_FileMeta, m_LeftBraceToken, m_RightBraceToken);
    }
    if (isError) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "ParseFunction 解析函数");
    }
    m_OverrideToken = virtualToken;
    m_PermissionToken = permissionToken;
    m_StaticToken = staticToken;
    m_GetToken = getToken;
    m_SetToken = setToken;
    m_FinalToken = finalToken;
    if (returnClassNameNode != nullptr) {
        m_DefineMetaClass = new FileMetaClassDefine(m_FileMeta, returnClassNameNode);
    }
    return true;
}

void FileMetaMemberFunction::ParseParam(Node* parNode) {
    if (parNode == nullptr) return;

    std::vector<std::vector<Node*>> tparamList;
    std::vector<Node*> tempList;

    for (size_t i = 0; i < parNode->childList.size(); i++) {
        auto pnode = parNode->childList[i];
        if (pnode->nodeType == SimpleLanguage::Compile::ENodeType::Comma) {
            tparamList.push_back(tempList);
            tempList.clear();
        } else {
            tempList.push_back(pnode);
        }
    }
    if (!tempList.empty()) {
        tparamList.push_back(tempList);
    }

    std::unordered_set<std::string> nameSet;
    for (size_t i = 0; i < tparamList.size(); i++) {
        auto nodelist = tparamList[i];
        auto cdp = new FileMetaParamterDefine(m_FileMeta, nodelist);
        if (nameSet.find(cdp->GetName()) != nameSet.end()) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error 参数名称有重名!!!");
        }
        AddMetaParamter(cdp);
    }
}

void FileMetaMemberFunction::ParseTemplate(Node* node) {
    if (node == nullptr) return;

    std::vector<Node*> tempList;
    for (size_t i = 0; i < node->childList.size(); i++) {
        auto cnode = node->childList[i];
        if (cnode->nodeType == SimpleLanguage::Compile::ENodeType::Comma) {
            continue;
        } else {
            auto cdp = new FileMetaTemplateDefine(m_FileMeta, cnode);
            auto it = std::find_if(m_MetaTemplatesList.begin(), m_MetaTemplatesList.end(),
                [&cdp](FileMetaTemplateDefine* fmtd) { return fmtd->GetName() == cdp->GetName(); });
            if (it != m_MetaTemplatesList.end()) {
                SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error 参数名称有重名!!!");
                continue;
            }
            AddMetaTemplate(cdp);
        }
    }
}

void FileMetaMemberFunction::SetDeep(int _deep) {
    m_Deep = _deep;
    if (m_FileMetaBlockSyntax != nullptr) {
        m_FileMetaBlockSyntax->SetDeep(m_Deep);
    }
}

std::string FileMetaMemberFunction::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < Deep(); i++)
        sb << SimpleLanguage::Global::tabChar;

    SimpleLanguage::EPermission permis = SimpleLanguage::Compile::CompilerUtil::GetPerMissionByString(
        m_PermissionToken ? m_PermissionToken->GetLexemeString() : "");
    if (permis == SimpleLanguage::EPermission::Null) {
        sb << "_public";
    } else {
        sb << SimpleLanguage::Compile::CompilerUtil::ToFormatString(permis);
    }
    if (m_StaticToken != nullptr) {
        sb << " " << m_StaticToken->GetLexeme().ToString();
    }
    if (m_InterfaceToken != nullptr) {
        sb << " " << m_InterfaceToken->GetLexeme().ToString();
    }
    if (m_OverrideToken != nullptr) {
        sb << " " << m_OverrideToken->GetLexeme().ToString();
    }
    if (m_DefineMetaClass != nullptr) {
        sb << " " << m_DefineMetaClass->ToFormatString();
    }
    if (!m_MetaTemplatesList.empty()) {
        sb << "<";
        for (size_t i = 0; i < m_MetaTemplatesList.size(); i++) {
            sb << m_MetaTemplatesList[i]->ToFormatString();
            if (i < m_MetaTemplatesList.size() - 1) {
                sb << ",";
            }
        }
        sb << ">";
    }
    sb << " " << (m_Token != nullptr ? m_Token->GetLexeme().ToString() : "") << "(";
    for (size_t i = 0; i < m_MetaParamtersList.size(); i++) {
        sb << m_MetaParamtersList[i]->ToFormatString();
        if (i < m_MetaParamtersList.size() - 1)
            sb << ", ";
    }
    sb << " )" << std::endl;

    if (m_FileMetaBlockSyntax != nullptr) {
        sb << m_FileMetaBlockSyntax->ToFormatString();
    }

    return sb.str();
}

} // namespace Compile
} // namespace SimpleLanguage
