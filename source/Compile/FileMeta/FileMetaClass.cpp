//****************************************************************************
//  File:      FileMeta.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "FileMetaClass.h"
#include "FileMeta.h"
#include "FileMetaNamespace.h"
#include "FileMetaMemberVariable.h"
#include "FileMetaMemberMethod.h"
#include "FileMetaMemberData.h"
#include "FileMetaCommon.h"
#include "../../Debug/Log.h"
#include "../..//Define.h"
#include "../Parse/StructParse.h"
#include "../FileMetatUtil.h"
#include <algorithm>
#include <sstream>

namespace SimpleLanguage {
namespace Compile {

FileMetaClass::FileMetaClass(FileMeta* fm, const ::std::vector<Node*>& listNode) : m_NodeList(listNode) {
    m_FileMeta = fm;
    m_Id = ++s_IdCount;
    Parse();
}

bool FileMetaClass::Parse() {
    if (m_NodeList.empty()) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error 错误 !!!");
        return false;
    }

    Token* permissionToken = nullptr;
    Token* m_ExtendsToken = nullptr;
    Token* commaToken = nullptr;
    bool isError = false;
    ::std::vector<Token*> classNameTokenList;
    ::std::vector<Token*> list;

    Node* angleNode = nullptr;
    Node* lastNode = nullptr;
    int addCount = 0;
    
    while (addCount < static_cast<int>(m_NodeList.size())) {
        Node* cnode = m_NodeList[addCount++];

        if (cnode->NodeType() == SimpleLanguage::Compile::Parse::ENodeType::IdentifierLink) {
            if (m_SufInterfaceToken != nullptr || m_ExtendsToken != nullptr) {
                ::std::vector<FileMetaClassDefine*> fcdList;
                addCount = ReadClassDefineStruct(addCount - 1, m_NodeList, fcdList);
                if (m_ExtendsToken != nullptr && m_SufInterfaceToken == nullptr) {
                    if (m_FileMetaExtendClass != nullptr) {
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 已有继承类,请勿多重继承!");
                    }
                    if (fcdList.empty()) {
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 继承关键字后边没有相应的内容!");
                    }
                    if (fcdList.size() > 1) {
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 继承只能单继承，不能多继承!!");
                    }
                    m_FileMetaExtendClass = fcdList[0];
                } else if (m_SufInterfaceToken != nullptr) {
                    if (fcdList.empty()) {
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "接口关键字后边没有相应的内容!");
                    }
                    m_InterfaceClassList.insert(m_InterfaceClassList.end(), fcdList.begin(), fcdList.end());
                }
            } else {
                if (!classNameTokenList.empty()) {
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructClassNameRepeat, "Error 字符两次赋值 107");
                    for (size_t i = 0; i < classNameTokenList.size(); i++) {
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, classNameTokenList[i]->GetLexeme());
                    }
                }
                classNameTokenList = cnode->linkTokenList();

                Node* nnode = nullptr;
                if (addCount < static_cast<int>(m_NodeList.size())) {
                    nnode = m_NodeList[addCount];
                }

                if (nnode != nullptr && nnode->nodeType() == SimpleLanguage::Compile::Parse::ENodeType::LeftAngle) {
                    int cAddCount = addCount + 1;
                    bool templateInExtends = false;
                    Node* templateNode = nullptr;
                    Node* templateExtendsNode = nullptr;
                    while (cAddCount < static_cast<int>(m_NodeList.size())) {
                        auto cnode2 = m_NodeList[cAddCount++];
                        if (cnode2->nodeType() == SimpleLanguage::Compile::Parse::ENodeType::RightAngle) {
                            if (templateNode == nullptr) {
                                auto ld = SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "没有找到模板定义T");
                                ld->filePath = cnode2->token()->path();
                                ld->sourceBeginLine = cnode2->token()->sourceBeginLine();
                                break;
                            }
                            auto fmtd = new FileMetaTemplateDefine(m_FileMeta, templateNode, templateExtendsNode);
                            m_TemplateDefineList.push_back(fmtd);
                            break;
                        } else if (cnode2->nodeType() == SimpleLanguage::Compile::Parse::ENodeType::Comma) {
                            auto fmtd = new FileMetaTemplateDefine(m_FileMeta, templateNode, templateExtendsNode);
                            m_TemplateDefineList.push_back(fmtd);
                            templateNode = nullptr;
                            continue;
                        } else if (cnode2->nodeType() == SimpleLanguage::Compile::Parse::ENodeType::Key && 
                                  cnode2->token() != nullptr && 
                                  cnode2->token()->type() == SimpleLanguage::ETokenType::Colon) {
                            templateInExtends = true;
                            continue;
                        } else if (cnode2->nodeType() == SimpleLanguage::Compile::Parse::ENodeType::IdentifierLink) {
                            if (templateInExtends) {
                                templateExtendsNode = cnode2;
                            } else {
                                templateNode = cnode2;
                            }
                        } else {
                            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 不支持其它格式 在类后续的模板限定中!");
                        }
                    }
                    addCount = cAddCount;
                }
            }
        } else {
            auto token = cnode->token();
            if (token->type() == SimpleLanguage::ETokenType::Public ||
                token->type() == SimpleLanguage::ETokenType::Private ||
                token->type() == SimpleLanguage::ETokenType::Projected ||
                token->type() == SimpleLanguage::ETokenType::Extern) {
                if (permissionToken == nullptr) {
                    permissionToken = token;
                } else {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次权限!!");
                }
            } else if (token->type() == SimpleLanguage::ETokenType::Const) {
                if (m_ConstToken == nullptr) {
                    m_ConstToken = token;
                } else {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次Const!!");
                }
            } else if (token->type() == SimpleLanguage::ETokenType::Partial) {
                if (m_PartialToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次Class!!");
                }
                m_PartialToken = token;
            } else if (token->type() == SimpleLanguage::ETokenType::Class) {
                if (m_EnumToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次Enum!!");
                }
                if (m_DataToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次data!!");
                }
                if (m_ClassToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次Class!!");
                }
                m_ClassToken = token;
            } else if (token->type() == SimpleLanguage::ETokenType::Enum) {
                if (m_EnumToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次Enum!!");
                }
                if (m_DataToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次data!!");
                }
                if (m_ClassToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次Class!!");
                }
                m_EnumToken = token;
            } else if (token->type() == SimpleLanguage::ETokenType::Data) {
                if (m_EnumToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次Enum!!");
                } else {
                    if (m_DataToken != nullptr) {
                        isError = true;
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次data!!");
                    }
                    if (m_ClassToken != nullptr) {
                        isError = true;
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次Class!!");
                    }
                    m_DataToken = token;
                }
            } else if (token->type() == SimpleLanguage::ETokenType::Extends) {
                if (m_ExtendsToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次Extend!!");
                }
                m_ExtendsToken = token;
            } else if (token->type() == SimpleLanguage::ETokenType::Interface) {
                if (m_EnumToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次Enum!!");
                }
                if (m_DataToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析过了一次data!!");
                }
                if (!classNameTokenList.empty()) {    //后置
                    if (m_PreInterfaceToken != nullptr) {
                        isError = true;
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析类时，已发现用过interface标记，不可重复使用该标记");
                    }
                    if (m_SufInterfaceToken != nullptr) {
                        isError = true;
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析类时，已发现用过interface标记，不可重复使用该标记");
                    }
                    m_SufInterfaceToken = token;
                } else {
                    if (m_ClassToken != nullptr) {
                        isError = true;
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析interface与class不可以周时出现!!");
                    }
                    if (m_PreInterfaceToken != nullptr) {
                        isError = true;
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析类时，已发现用过interface标记，不可重复使用该标记");
                    }
                    if (m_SufInterfaceToken != nullptr) {
                        isError = true;
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析类时，已发现用过interface标记，不可重复使用该标记");
                    }
                    m_PreInterfaceToken = token;
                }
            } else if (token->type() == SimpleLanguage::ETokenType::Comma) {
                commaToken = token;
            } else {
                isError = true;
                SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 有其它未知类型在class中");
                break;
            }
        }
        lastNode = cnode;
    }

    if (m_EnumToken != nullptr) {
        if (m_PreInterfaceToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Enum方式，与enum同级，不允许同时出现");
            return false;
        }
        if (m_SufInterfaceToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Enum方式，不支持接口方式");
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "");
            return false;
        }
        if (permissionToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Enum方式，不支持权限的使用!!");
            return false;
        }
        if (m_PartialToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Enum方式，不支持partial的使用!!");
            return false;
        }
    } else if (m_DataToken != nullptr) {
        if (m_PreInterfaceToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Enum方式，与data同级，不允许同时出现");
            return false;
        }
        if (m_SufInterfaceToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Enum方式，不支持接口方式");
            return false;
        }
        if (permissionToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Data方式，不支持权限的使用!!");
            return false;
        }
        if (m_PartialToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Data方式，不支持partial的使用!!");
            return false;
        }
    } else {
        if (classNameTokenList.empty()) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error 解析类型名称错误!!");
        }
    }
    
    if (!classNameTokenList.empty()) {
        m_Token = classNameTokenList[classNameTokenList.size() - 1];
        if (classNameTokenList.size() > 1) {
            std::vector<Token*> rangeTokens(classNameTokenList.begin(), classNameTokenList.end() - 1);
            m_NamespaceBlock = NamespaceStatementBlock::CreateStateBlock(rangeTokens);
        }
    }
    SetPermissionToken(permissionToken);

    return true;
}

int FileMetaClass::ReadClassDefineStruct(int cAddCount, const std::vector<Node*>& m_NodeList, std::vector<FileMetaClassDefine*>& fcdList) {
    std::vector<ParseStructTemp*> rootPST;
    ParseStructTemp* curPST = nullptr;
    
    while (cAddCount < static_cast<int>(m_NodeList.size())) {
        auto cnode2 = m_NodeList[cAddCount];
        
        if (cnode2->nodeType() == SimpleLanguage::Compile::Parse::ENodeType::RightAngle) {
            if (curPST == nullptr) {
                cAddCount++;
                break;
            }
            if (curPST->angleNode != nullptr) {
                curPST->angleNode->setEndToken(cnode2->token());
                if (curPST->parentPSt == nullptr) {
                    cAddCount++;
                    break;
                } else {
                    curPST = curPST->parentPSt;
                }
            } else {
                if (curPST->parentPSt == nullptr) {
                    cAddCount++;
                    break;
                } else {
                    curPST = curPST->parentPSt;
                }
            }
        } else if (cnode2->NodeType() == SimpleLanguage::ENodeType::Comma) {
            cAddCount++;
            continue;
        } else if (cnode2->NodeType() == SimpleLanguage::ENodeType::IdentifierLink ||
                  (cnode2->NodeType() == SimpleLanguage::ENodeType::Key &&
                   cnode2->GetToken() != nullptr && 
                   cnode2->GetToken()->type() == SimpleLanguage::ETokenType::Object)) {
            ParseStructTemp* newpst = nullptr;
            if (curPST == nullptr) {
                newpst = new ParseStructTemp();
                newpst->nameNode = cnode2;
                rootPST.push_back(newpst);
            } else {
                newpst = new ParseStructTemp();
                newpst->nameNode = cnode2;
                curPST->AddParseStructTemplate(newpst);
            }
            
            if (cAddCount + 1 < static_cast<int>(m_NodeList.size())) {
                auto nextNode = m_NodeList[cAddCount + 1];
                if (nextNode->nodeType() == SimpleLanguage::Compile::Parse::ENodeType::LeftAngle) {
                    cAddCount++;
                    newpst->angleNode = nextNode;
                    curPST = newpst;
                }
            }
        } else if (cnode2->nodeType() == SimpleLanguage::Compile::Parse::ENodeType::Key && 
                  cnode2->token() != nullptr && 
                  cnode2->token()->type() == SimpleLanguage::ETokenType::Interface) {
            cAddCount++;
            break;
        } else {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error 不支持其它格式 在类后续的模板限定中!");
        }
        cAddCount++;
    }
    
    for (size_t i = 0; i < rootPST.size(); i++) {
        auto pst = rootPST[i];
        Node* nameNode = pst->nameNode;
        pst->GenFileInputTemplateNode(nameNode, m_FileMeta);
        auto fmcd = new FileMetaClassDefine(m_FileMeta, pst->nameNode, nullptr);
        fcdList.push_back(fmcd);
    }

    return cAddCount;
}

void FileMetaClass::AddFileMemberData(FileMetaMemberData* fmmd) {
    m_MemberDataList.push_back(fmmd);
    fmmd->SetFileMeta(m_FileMeta);
}

FileMetaMemberData* FileMetaClass::GetFileMemberData(const std::string& name) {
    auto it = std::find_if(m_MemberDataList.begin(), m_MemberDataList.end(),
        [&name](FileMetaMemberData* fmmd) { return fmmd->name() == name; });
    
    if (it != m_MemberDataList.end()) {
        return *it;
    }
    return nullptr;
}

void FileMetaClass::AddFileMemberVariable(FileMetaMemberVariable* fmv) {
    m_MemberVariableList.push_back(fmv);
    fmv->SetFileMeta(m_FileMeta);
}

void FileMetaClass::AddFileMemberFunction(FileMetaMemberFunction* fmmf) {
    m_MemberFunctionList.push_back(fmmf);
    fmmf->SetFileMeta(m_FileMeta);
}

void FileMetaClass::SetMetaNamespace(FileMetaNamespace* mn) {
    m_TopLevelFileMetaNamespace = mn;
}

void FileMetaClass::AddExtendMetaNamespace(FileMetaNamespace* fmn) {
    if (m_TopLevelFileMetaNamespace != nullptr) {
        // Implementation for extending namespace
    } else {
        auto list = fmn->namespaceStatementBlock()->namespaceList();
        if (list.empty()) {
            return;
        }
        if (m_NamespaceBlock != nullptr) {
            std::string lastName = m_NamespaceBlock->namespaceList()[m_NamespaceBlock->namespaceList().size() - 1];
            if (list[list.size() - 1] == lastName) {
                auto namespaceNameNode = new Node(fmn->namespaceNameNode()->token());
                std::vector<Node*> extendLinkNodeList;
                for (size_t i = 0; i < fmn->namespaceNameNode()->extendLinkNodeList().size() - 2; i++) {
                    extendLinkNodeList.push_back(fmn->namespaceNameNode()->extendLinkNodeList()[i]);
                }
                namespaceNameNode->SetLinkNode(extendLinkNodeList);
                auto fmnnew = new FileMetaNamespace(fmn->namespaceNode(), namespaceNameNode);
                m_TopLevelFileMetaNamespace = fmnnew;
            }
        }
    }
}

void FileMetaClass::SetPartialToken(Token* partialToken) {
    m_PartialToken = partialToken;
}

void FileMetaClass::SetPermissionToken(Token* permissionToken) {
    m_PermissionToken = permissionToken;
}

void FileMetaClass::SetMetaClass(SimpleLanguage::Core::MetaClass* mc) {
    m_MetaClass = mc;
}

void FileMetaClass::AddFileMetaClass(FileMetaClass* fmc) {
    fmc->m_Deep = deep() + 1;
    fmc->SetFileMetaClass(this);
    m_ChildrenClassList.push_back(fmc);
}

void FileMetaClass::AddInterfaceClass(FileMetaClassDefine* fmcv) {
    m_InterfaceClassList.push_back(fmc);
}

void FileMetaClass::SetFileMetaClass(FileMetaClass* fmc) {
    m_TopLevelFileMetaClass = fmc;
    m_InnerClass = true;
}

void FileMetaClass::SetDeep(int _deep) {
    m_Deep = _deep;
    for (auto v : m_ChildrenClassList) {
        v->SetDeep(m_Deep + 1);
    }
    for (auto v : m_MemberVariableList) {
        v->SetDeep(m_Deep + 1);
    }
    for (auto v : m_MemberFunctionList) {
        v->SetDeep(m_Deep + 1);
    }
}

std::string FileMetaClass::ToString() const {
    return Name();
}

std::string FileMetaClass::ToFormatString() const {
    /*stringBuilder.str("");
    stringBuilder.clear();
    
    for (int i = 0; i < GetDeep(); i++)
        stringBuilder << SimpleLanguage::Global::tabChar;

    if (m_DataToken != nullptr) {
        if (m_ConstToken != nullptr) {
            stringBuilder << m_ConstToken->GetLexeme() << " ";
        }
        stringBuilder << m_DataToken->GetLexeme() << " ";
        stringBuilder << Name();
        stringBuilder << std::endl;
        for (int i = 0; i < GetDeep(); i++)
            stringBuilder << SimpleLanguage::Global::tabChar;
        stringBuilder << "{" << std::endl;

        for (auto v : m_MemberDataList) {
            stringBuilder << v->ToFormatString() << std::endl;
        }

        for (int i = 0; i < GetDeep(); i++)
            stringBuilder << SimpleLanguage::Global::tabChar;
        stringBuilder << "}";
    } else if (m_EnumToken != nullptr) {
        if (m_ConstToken != nullptr) {
            stringBuilder << m_ConstToken->GetLexeme() << " ";
        }
        stringBuilder << m_EnumToken->GetLexeme() << " ";
        stringBuilder << name();
        stringBuilder << std::endl;
        for (int i = 0; i < deep(); i++)
            stringBuilder << SimpleLanguage::Global::tabChar;
        stringBuilder << "{" << std::endl;

        for (auto v : m_MemberVariableList) {
            stringBuilder << v->ToFormatString() << std::endl;
        }
        if (!m_MemberVariableList.empty())
            stringBuilder << std::endl;

        for (int i = 0; i < deep(); i++)
            stringBuilder << SimpleLanguage::Global::tabChar;
        stringBuilder << "}";
    } else {
        stringBuilder << (m_PermissionToken != nullptr ? m_PermissionToken->lexeme().ToString() : "_public");
        stringBuilder << " ";
        if (m_PartialToken != nullptr)
            stringBuilder << m_PartialToken->lexeme().ToString() << " ";
        else
            stringBuilder << "_partial ";
        if (m_ClassToken != nullptr) {
            stringBuilder << m_ClassToken->lexeme().ToString();
            stringBuilder << " ";
        } else {
            stringBuilder << "_class" << " ";
        }

        if (m_NamespaceBlock != nullptr) {
            stringBuilder << m_NamespaceBlock->ToFormatString();
            stringBuilder << ".";
        }
        stringBuilder << name();

        if (!m_TemplateDefineList.empty()) {
            stringBuilder << "<";
            for (size_t i = 0; i < m_TemplateDefineList.size(); i++) {
                stringBuilder << m_TemplateDefineList[i]->ToFormatString();
                if (i < m_TemplateDefineList.size() - 1) {
                    stringBuilder << ",";
                }
            }
            stringBuilder << ">";
        }

        if (m_FileMetaExtendClass != nullptr) {
            stringBuilder << " extends " << m_FileMetaExtendClass->ToFormatString();
        }
        if (!interfaceClassList().empty()) {
            stringBuilder << "  interface";
        }
        for (size_t i = 0; i < interfaceClassList().size(); i++) {
            stringBuilder << " " << interfaceClassList()[i]->ToFormatString();
            if (i < interfaceClassList().size() - 1)
                stringBuilder << ",";
        }
        stringBuilder << std::endl;
        for (int i = 0; i < deep(); i++)
            stringBuilder << SimpleLanguage::Global::tabChar;
        stringBuilder << "{" << std::endl;
        for (auto v : m_ChildrenClassList) {
            stringBuilder << v->ToFormatString() << std::endl;
        }
        if (!m_ChildrenClassList.empty())
            stringBuilder << std::endl;

        for (auto v : m_MemberVariableList) {
            stringBuilder << v->ToFormatString() << std::endl;
        }
        if (!m_MemberVariableList.empty())
            stringBuilder << std::endl;

        for (auto v : m_MemberFunctionList) {
            stringBuilder << v->ToFormatString() << std::endl;
        }
        for (int i = 0; i < deep(); i++)
            stringBuilder << SimpleLanguage::Global::tabChar;
        stringBuilder << "}";
    }*/

    return stringBuilder.str();
}

// ParseStructTemp implementation
void FileMetaClass::ParseStructTemp::AddParseStructTemplate(ParseStructTemp* pst) {
    angleContentNodeList.push_back(pst);
    pst->parentPSt = this;
}
void FileMetaClass::ParseStructTemp::GenFileInputTemplateNode(Node* node, FileMeta* fm) {
    node->setAngleNode(angleNode);
    for (size_t i = 0; i < angleContentNodeList.size(); i++) {
        node->angleNode()->AddChild(angleContentNodeList[i]->nameNode);
        if (!angleContentNodeList[i]->angleContentNodeList.empty()) {
            angleContentNodeList[i]->GenFileInputTemplateNode(angleContentNodeList[i]->nameNode, fm);
        }
    }
}
} // namespace Compile
} // namespace SimpleLanguage

