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
#include "../FileMeta/FileMetatUtil.h"
#include "../../Core/MetaClass.h"
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
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error ���� !!!");
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

        if (cnode->nodeType == SimpleLanguage::Compile::ENodeType::IdentifierLink) {
            if (m_SufInterfaceToken != nullptr || m_ExtendsToken != nullptr) {
                ::std::vector<FileMetaClassDefine*> fcdList;
                addCount = ReadClassDefineStruct(addCount - 1, m_NodeList, fcdList);
                if (m_ExtendsToken != nullptr && m_SufInterfaceToken == nullptr) {
                    if (m_FileMetaExtendClass != nullptr) {
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ���м̳���,������ؼ̳�!");
                    }
                    if (fcdList.empty()) {
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error �̳йؼ��ֺ��û����Ӧ������!");
                    }
                    if (fcdList.size() > 1) {
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error �̳�ֻ�ܵ��̳У����ܶ�̳�!!");
                    }
                    m_FileMetaExtendClass = fcdList[0];
                } else if (m_SufInterfaceToken != nullptr) {
                    if (fcdList.empty()) {
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "�ӿڹؼ��ֺ��û����Ӧ������!");
                    }
                    m_InterfaceClassList.insert(m_InterfaceClassList.end(), fcdList.begin(), fcdList.end());
                }
            } else {
                if (!classNameTokenList.empty()) {
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructClassNameRepeat, "Error �ַ����θ�ֵ 107");
                    for (size_t i = 0; i < classNameTokenList.size(); i++) {
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, classNameTokenList[i]->GetLexemeString());
                    }
                }
                classNameTokenList = cnode->GetLinkTokenList();

                Node* nnode = nullptr;
                if (addCount < static_cast<int>(m_NodeList.size())) {
                    nnode = m_NodeList[addCount];
                }

                if (nnode != nullptr && nnode->nodeType == SimpleLanguage::Compile::ENodeType::LeftAngle) {
                    int cAddCount = addCount + 1;
                    bool templateInExtends = false;
                    Node* templateNode = nullptr;
                    Node* templateExtendsNode = nullptr;
                    while (cAddCount < static_cast<int>(m_NodeList.size())) {
                        auto cnode2 = m_NodeList[cAddCount++];
                        if (cnode2->nodeType == SimpleLanguage::Compile::ENodeType::RightAngle) {
                            if (templateNode == nullptr) {
                                auto ld = SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "û���ҵ�ģ�嶨��T");
                                ld->filePath = cnode2->token->GetPath();
                                ld->sourceBeginLine = cnode2->token->GetSourceBeginLine();
                                break;
                            }
                            auto fmtd = new FileMetaTemplateDefine(m_FileMeta, templateNode, templateExtendsNode);
                            m_TemplateDefineList.push_back(fmtd);
                            break;
                        } else if (cnode2->nodeType == SimpleLanguage::Compile::ENodeType::Comma) {
                            auto fmtd = new FileMetaTemplateDefine(m_FileMeta, templateNode, templateExtendsNode);
                            m_TemplateDefineList.push_back(fmtd);
                            templateNode = nullptr;
                            continue;
                        } else if (cnode2->nodeType == SimpleLanguage::Compile::ENodeType::Key && 
                                  cnode2->token != nullptr && 
                                  cnode2->token->GetType() == SimpleLanguage::ETokenType::Colon) {
                            templateInExtends = true;
                            continue;
                        } else if (cnode2->nodeType == SimpleLanguage::Compile::ENodeType::IdentifierLink) {
                            if (templateInExtends) {
                                templateExtendsNode = cnode2;
                            } else {
                                templateNode = cnode2;
                            }
                        } else {
                            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��֧��������ʽ ���������ģ���޶���!");
                        }
                    }
                    addCount = cAddCount;
                }
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
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��Ȩ��!!");
                }
            } else if (token->GetType() == SimpleLanguage::ETokenType::Const) {
                if (m_ConstToken == nullptr) {
                    m_ConstToken = token;
                } else {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��Const!!");
                }
            } else if (token->GetType() == SimpleLanguage::ETokenType::Partial) {
                if (m_PartialToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��Class!!");
                }
                m_PartialToken = token;
            } else if (token->GetType() == SimpleLanguage::ETokenType::Class) {
                if (m_EnumToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��Enum!!");
                }
                if (m_DataToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��data!!");
                }
                if (m_ClassToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��Class!!");
                }
                m_ClassToken = token;
            } else if (token->GetType() == SimpleLanguage::ETokenType::Enum) {
                if (m_EnumToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��Enum!!");
                }
                if (m_DataToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��data!!");
                }
                if (m_ClassToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��Class!!");
                }
                m_EnumToken = token;
            } else if (token->GetType() == SimpleLanguage::ETokenType::Data) {
                if (m_EnumToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��Enum!!");
                } else {
                    if (m_DataToken != nullptr) {
                        isError = true;
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��data!!");
                    }
                    if (m_ClassToken != nullptr) {
                        isError = true;
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��Class!!");
                    }
                    m_DataToken = token;
                }
            } else if (token->GetType() == SimpleLanguage::ETokenType::Extends) {
                if (m_ExtendsToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��Extend!!");
                }
                m_ExtendsToken = token;
            } else if (token->GetType() == SimpleLanguage::ETokenType::Interface) {
                if (m_EnumToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��Enum!!");
                }
                if (m_DataToken != nullptr) {
                    isError = true;
                    SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ��������һ��data!!");
                }
                if (!classNameTokenList.empty()) {    //����
                    if (m_PreInterfaceToken != nullptr) {
                        isError = true;
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ������ʱ���ѷ����ù�interface��ǣ������ظ�ʹ�øñ��");
                    }
                    if (m_SufInterfaceToken != nullptr) {
                        isError = true;
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ������ʱ���ѷ����ù�interface��ǣ������ظ�ʹ�øñ��");
                    }
                    m_SufInterfaceToken = token;
                } else {
                    if (m_ClassToken != nullptr) {
                        isError = true;
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ����interface��class��������ʱ����!!");
                    }
                    if (m_PreInterfaceToken != nullptr) {
                        isError = true;
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ������ʱ���ѷ����ù�interface��ǣ������ظ�ʹ�øñ��");
                    }
                    if (m_SufInterfaceToken != nullptr) {
                        isError = true;
                        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error ������ʱ���ѷ����ù�interface��ǣ������ظ�ʹ�øñ��");
                    }
                    m_PreInterfaceToken = token;
                }
            } else if (token->GetType() == SimpleLanguage::ETokenType::Comma) {
                commaToken = token;
            } else {
                isError = true;
                SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart,"Error������δ֪������class��");
                break;
            }
        }
        lastNode = cnode;
    }

    if (m_EnumToken != nullptr) {
        if (m_PreInterfaceToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Enum��ʽ����enumͬ����������ͬʱ����");
            return false;
        }
        if (m_SufInterfaceToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Enum��ʽ����֧�ֽӿڷ�ʽ");
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "");
            return false;
        }
        if (permissionToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Enum��ʽ����֧��Ȩ�޵�ʹ��!!");
            return false;
        }
        if (m_PartialToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Enum��ʽ����֧��partial��ʹ��!!");
            return false;
        }
    } else if (m_DataToken != nullptr) {
        if (m_PreInterfaceToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Enum��ʽ����dataͬ����������ͬʱ����");
            return false;
        }
        if (m_SufInterfaceToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Enum��ʽ����֧�ֽӿڷ�ʽ");
            return false;
        }
        if (permissionToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Data��ʽ����֧��Ȩ�޵�ʹ��!!");
            return false;
        }
        if (m_PartialToken != nullptr) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error Data��ʽ����֧��partial��ʹ��!!");
            return false;
        }
    } else {
        if (classNameTokenList.empty()) {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::StructFileMetaStart, "Error �����������ƴ���!!");
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
        
        if (cnode2->nodeType == SimpleLanguage::Compile::ENodeType::RightAngle) {
            if (curPST == nullptr) {
                cAddCount++;
                break;
            }
            if (curPST->angleNode != nullptr) {
                curPST->angleNode->endToken = (cnode2->token);
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
        } else if (cnode2->nodeType == SimpleLanguage::Compile::ENodeType::Comma) {
            cAddCount++;
            continue;
        } else if (cnode2->nodeType == SimpleLanguage::Compile::ENodeType::IdentifierLink ||
                  (cnode2->nodeType == SimpleLanguage::Compile::ENodeType::Key &&
                   cnode2->token != nullptr &&
                   cnode2->token->GetType() == SimpleLanguage::ETokenType::Object)) {
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
                if (nextNode->nodeType == SimpleLanguage::Compile::ENodeType::LeftAngle) {
                    cAddCount++;
                    newpst->angleNode = nextNode;
                    curPST = newpst;
                }
            }
        } else if (cnode2->nodeType == SimpleLanguage::Compile::ENodeType::Key && 
                  cnode2->token != nullptr && 
                  cnode2->token->GetType() == SimpleLanguage::ETokenType::Interface) {
            cAddCount++;
            break;
        } else {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error ��֧��������ʽ ���������ģ���޶���!");
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
        [&name](FileMetaMemberData* fmmd) { return fmmd->Name() == name; });
    
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
        auto list = fmn->GetNamespaceStatementBlock()->NamespaceList();
        if (list.empty()) {
            return;
        }
        if (m_NamespaceBlock != nullptr) {
            std::string lastName = m_NamespaceBlock->NamespaceList()[m_NamespaceBlock->NamespaceList().size() - 1];
            if (list[list.size() - 1] == lastName) {
                auto namespaceNameNode = new Node(fmn->NamespaceNameNode()->token);
                std::vector<Node*> extendLinkNodeList;
                for (size_t i = 0; i < fmn->NamespaceNameNode()->GetExtendLinkNodeList().size() - 2; i++) {
                    extendLinkNodeList.push_back(fmn->NamespaceNameNode()->GetExtendLinkNodeList()[i]);
                }
                namespaceNameNode->SetLinkNode(extendLinkNodeList);
                auto fmnnew = new FileMetaNamespace(fmn->NamespaceNode(), namespaceNameNode);
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

void FileMetaClass::SetMetaClass( Core::MetaClass* mc) {
    m_MetaClass = mc;
}

void FileMetaClass::AddFileMetaClass(FileMetaClass* fmc) {
    fmc->m_Deep = Deep() + 1;
    fmc->SetFileMetaClass(this);
    m_ChildrenClassList.push_back(fmc);
}

void FileMetaClass::AddInterfaceClass(FileMetaClassDefine* fmcv) {
    m_InterfaceClassList.push_back(fmcv);
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
    node->angleNode = angleNode;
    for (size_t i = 0; i < angleContentNodeList.size(); i++) {
        node->angleNode->AddChild(angleContentNodeList[i]->nameNode);
        if (!angleContentNodeList[i]->angleContentNodeList.empty()) {
            angleContentNodeList[i]->GenFileInputTemplateNode(angleContentNodeList[i]->nameNode, fm);
        }
    }
}
} // namespace Compile
} // namespace SimpleLanguage

