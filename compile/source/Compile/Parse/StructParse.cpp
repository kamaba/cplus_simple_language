//****************************************************************************
//  File:      StructParse.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "../FileMeta/FileMeta.h"
#include "../FileMeta/FileMetaClass.h"
#include "../FileMeta/FileMetaMemberData.h"
#include "../FileMeta/FileMetaMemberFunction.h"
#include "../FileMeta/FileMetaMemberVariable.h"
#include "../FileMeta/FileMetaNamespace.h"
#include "../FileMeta/FileMetaSyntax.h"
#include "../FileMeta/FileMetaCommon.h"
#include "../FileMeta/FileMetatUtil.h"
#include "../FileMeta/FileMetaGlobalSyntax.h"
#include "Node.h"
#include "../../Debug/Log.h"
#include "../../Project/ProjectManager.h"
#include "StructParse.h"
#include <iostream>
#include <algorithm>

using namespace SimpleLanguage::Debug;
using namespace SimpleLanguage::Project;

namespace SimpleLanguage {
namespace Compile {

// SyntaxNodeStruct 方法实现
void StructParse::SyntaxNodeStruct::SetMainKeyNode(Node* _keyNode) {
    keyNode = _keyNode;
    tokenType = _keyNode->token->GetType();
    eSyntaxNodeType = ESyntaxNodeStructType::KeySyntax;
}

void StructParse::SyntaxNodeStruct::AddContent(Node* node) {
    if (tokenType == ETokenType::If
        || tokenType == ETokenType::ElseIf
        || tokenType == ETokenType::Switch
        || tokenType == ETokenType::For
        || tokenType == ETokenType::While
        || tokenType == ETokenType::DoWhile
        || tokenType == ETokenType::Return
        || tokenType == ETokenType::Transience
        || tokenType == ETokenType::Case) {
        keyContent.push_back(node);
    }
    else if (tokenType == ETokenType::Else
        || tokenType == ETokenType::Next
        || tokenType == ETokenType::Break
        || tokenType == ETokenType::Continue) {
        Log::AddInStructFileMeta(EError::None, "Error 解析Else时，不能包含任何代码" + (node->token ? node->token->ToLexemeAllString() : "null"));
    }
    else {
        commonContent.push_back(node);
        eSyntaxNodeType = ESyntaxNodeStructType::CommonSyntax;
    }
}

void StructParse::SyntaxNodeStruct::SetBraceNode(Node* node) {
    blockNode = node;
    if (tokenType == ETokenType::If
        || tokenType == ETokenType::ElseIf
        || tokenType == ETokenType::Else
        || tokenType == ETokenType::Switch
        || tokenType == ETokenType::For
        || tokenType == ETokenType::While || tokenType == ETokenType::DoWhile
        || tokenType == ETokenType::Case
        || tokenType == ETokenType::Default
        || tokenType == ETokenType::Return
        || tokenType == ETokenType::Transience
        || tokenType == ETokenType::Label
        || tokenType == ETokenType::Goto) {
    }
    else {
        Log::AddInStructFileMeta(EError::None, "Error 解析{}时，关键字不支持这种语法结构" + (node->token ? node->token->ToLexemeAllString() : "null"));
    }
}

bool StructParse::SyntaxNodeStruct::IsLineEndBreak() {
    if (tokenType == ETokenType::If
        || tokenType == ETokenType::ElseIf
        || tokenType == ETokenType::Else
        || tokenType == ETokenType::Switch
        || tokenType == ETokenType::For
        || tokenType == ETokenType::While
        || tokenType == ETokenType::DoWhile
        || tokenType == ETokenType::Label) {
        if (blockNode == nullptr) {
            return false;
        }
        return !ProjectManager::GetIsUseForceSemiColonInLineEnd();
    }
    return true;
}

// Condition 方法实现
bool StructParse::Condition::IsMatchTokenType(ETokenType tokenType) {
    return std::find(eTokenTypeList.begin(), eTokenTypeList.end(), tokenType) != eTokenTypeList.end();
}

StructParse::Condition::Condition(ETokenType tokenType) {
    eTokenTypeList.push_back(tokenType);
    if (tokenType == ETokenType::Else || tokenType == ETokenType::ElseIf) {
        isFirstKey = true;
    }
    isCheck = true;
}

void StructParse::Condition::AddTokenTypeList(ETokenType tokenType) {
    eTokenTypeList.push_back(tokenType);
}

// ParseCurrentNodeInfo ���캯��ʵ��
StructParse::ParseCurrentNodeInfo::ParseCurrentNodeInfo(FileMeta* cf) 
    : codeFile(cf), parseType(EParseNodeType::File) {
}

StructParse::ParseCurrentNodeInfo::ParseCurrentNodeInfo(FileMetaNamespace* nsn) 
    : codeNamespace(nsn), parseType(EParseNodeType::Namespace) {
}

StructParse::ParseCurrentNodeInfo::ParseCurrentNodeInfo(FileMetaClass* nsc) 
    : codeClass(nsc), parseType(EParseNodeType::Class) {
}

StructParse::ParseCurrentNodeInfo::ParseCurrentNodeInfo(FileMetaMemberData* fmmd) 
    : codeData(fmmd), parseType(EParseNodeType::DataMemeber) {
}

StructParse::ParseCurrentNodeInfo::ParseCurrentNodeInfo(FileMetaMemberFunction* nsf) 
    : codeFunction(nsf), parseType(EParseNodeType::Function) {
}

StructParse::ParseCurrentNodeInfo::ParseCurrentNodeInfo(FileMetaSyntax* nss) 
    : codeSyntax(nss), parseType(EParseNodeType::Statements) {
}

StructParse::StructParse(FileMeta* fm, Node* node) 
    : m_FileMeta(fm), m_RootNode(node) {
}

StructParse::ParseCurrentNodeInfo* StructParse::GetCurrentNodeInfo() {
    if (m_CurrentNodeInfoStack.empty()) return nullptr;
    return m_CurrentNodeInfoStack.top();
}

void StructParse::AddParseFileNodeInfo() {
    auto pcni = new ParseCurrentNodeInfo(m_FileMeta);
    m_CurrentNodeInfoStack.push(pcni);
}

void StructParse::AddParseNamespaceNodeInfo(FileMetaNamespace* fmn) {
    ParseCurrentNodeInfo* currentNodeInfo = GetCurrentNodeInfo();
    if (currentNodeInfo->parseType == EParseNodeType::File) {
        currentNodeInfo->codeFile->AddFileMetaAllNamespace(fmn);
    } else if (currentNodeInfo->parseType == EParseNodeType::Namespace) {
        currentNodeInfo->codeNamespace->AddFileNamespace(fmn);
    }

    auto pcni = new ParseCurrentNodeInfo(fmn);
    m_CurrentNodeInfoStack.push(pcni);
}

void StructParse::AddParseClassNodeInfo(FileMetaClass* fmc) {
    auto* currentNodeInfo = GetCurrentNodeInfo();
    if (currentNodeInfo->parseType == EParseNodeType::File) {
        currentNodeInfo->codeFile->AddFileMetaClass(fmc);
    } else if (currentNodeInfo->parseType == EParseNodeType::Namespace) {
        currentNodeInfo->codeNamespace->AddFileMetaClass(fmc);
    } else if (currentNodeInfo->parseType == EParseNodeType::Class) {
        currentNodeInfo->codeClass->AddFileMetaClass(fmc);
    } else {
        Log::AddInStructFileMeta(EError::None, "���� !!1 AddParseClassNodeInfo");
        return;
    }
    m_FileMeta->AddFileMetaAllClass(fmc);

    auto pcni = new ParseCurrentNodeInfo(fmc);
    m_CurrentNodeInfoStack.push(pcni);
}

void StructParse::AddParseVariableInfo(FileMetaMemberVariable* csv) {
    auto* currentNodeInfo = GetCurrentNodeInfo();
    if (currentNodeInfo->parseType == EParseNodeType::Class) {
        currentNodeInfo->codeClass->AddFileMemberVariable(csv);
    } else {
        Log::AddInStructFileMeta(EError::None, "���� !!1 AddParseVariableInfo");
        return;
    }
}

void StructParse::AddParseDataInfo(FileMetaMemberData* fmmd) {
    auto* currentNodeInfo = GetCurrentNodeInfo();
    if (currentNodeInfo->parseType == EParseNodeType::Class) {
        currentNodeInfo->codeClass->AddFileMemberData(fmmd);
    } else if (currentNodeInfo->parseType == EParseNodeType::DataMemeber) {
        currentNodeInfo->codeData->AddFileMemberData(fmmd);
    } else {
        Log::AddInStructFileMeta(EError::None, "���� !!1 AddParseFunctionNodeInfo");
        return;
    }

    auto pcni = new ParseCurrentNodeInfo(fmmd);
    m_CurrentNodeInfoStack.push(pcni);
}

void StructParse::AddParseFunctionNodeInfo(FileMetaMemberFunction* fmmf, bool isPush) {
    auto* currentNodeInfo = GetCurrentNodeInfo();
    if (currentNodeInfo->parseType == EParseNodeType::Class) {
        currentNodeInfo->codeClass->AddFileMemberFunction(fmmf);
    } else {
        Log::AddInStructFileMeta(EError::None, "���� !!1 AddParseFunctionNodeInfo");
        return;
    }

    if (isPush) {
        auto pcni = new ParseCurrentNodeInfo(fmmf);
        m_CurrentNodeInfoStack.push(pcni);
    }
}

void StructParse::AddParseSyntaxNodeInfo(FileMetaSyntax* fms, bool isAddParseCurrentNNode) {
    auto* currentNodeInfo = GetCurrentNodeInfo();
    if (currentNodeInfo->parseType == EParseNodeType::Function) {
        currentNodeInfo->codeFunction->AddFileMetaSyntax(fms);
    } else if (currentNodeInfo->parseType == EParseNodeType::Statements) {
        currentNodeInfo->codeSyntax->AddFileMetaSyntax(fms);
    } else {
        Log::AddInStructFileMeta(EError::None, "���� !!1 AddParseFunctionNodeInfo");
        return;
    }

    if (isAddParseCurrentNNode) {
        auto pcni = new ParseCurrentNodeInfo(fms);
        m_CurrentNodeInfoStack.push(pcni);
    }
}

void StructParse::ParseRootNodeToFileMeta() {
    AddParseFileNodeInfo();

    Node* pnode = m_RootNode;
    while (true) {
        if (CheckEnd(pnode)) {
            break;
        }
        auto* node = m_RootNode->childList[pnode->parseIndex];
        if (node->nodeType == ENodeType::LineEnd) {
            pnode->parseIndex++;
            continue;
        }

        if (node->nodeType == ENodeType::Key) {
            switch (node->token->GetType()) {
                case ETokenType::Import:
                    ParseImport(pnode);
                    break;
                case ETokenType::Namespace:
                    ParseNamespace(pnode);
                    break;
                case ETokenType::Const:
                case ETokenType::Data:
                case ETokenType::Enum:
                case ETokenType::Class:
                case ETokenType::Extern:
                case ETokenType::Public:
                case ETokenType::Private:
                case ETokenType::Projected:
                case ETokenType::Partial:
                case ETokenType::DoWhile:
                    ParseNamespaceOrTopClass(pnode);
                    break;
                default:
                    Log::AddInStructFileMeta(EError::None, 
                        "Error 不允许 在File头级目录中出现 : " + node->token->GetLexemeString());
                    break;
            }
        } else if (node->nodeType == ENodeType::IdentifierLink) {
            ParseNamespaceOrTopClass(pnode);
        } else {
            Log::AddInStructFileMeta(EError::None, 
                "Error 不允许 在File头级目录中出现2 : " + (node->token ? node->token->GetLexemeString() : "null"));
        }
    }

    ParseCurrentNodeInfo* fileCode = m_CurrentNodeInfoStack.top();
    m_CurrentNodeInfoStack.pop();

    if (fileCode->parseType == EParseNodeType::File) {
#ifdef DEBUG
        m_FileMeta->SetDeep(0);
#endif
        string log1 = "Code";
        Log::AddProcess(EProcess::ParseNode, EError::None, log1);
    } else {
        Log::AddProcess(EProcess::ParseNode, EError::ParseFileError, "解析出现错误 ParseFile : " + std::to_string(static_cast<int>(GetCurrentNodeInfo()->parseType)));
        return;
    }
}

std::vector<Node*> StructParse::GetAllNodeToSemiColon(Node* pnode, bool isAddSelf) {
    Node* curNode = pnode->GetParseCurrent();

    std::vector<Node*> conNode;
    if (isAddSelf) {
        conNode.push_back(new Node(*curNode));
    }
    if (curNode->nodeType == ENodeType::SemiColon) {
        pnode->parseIndex++;
        return conNode;
    }

    Node* node = pnode->GetParseNode();
    conNode.push_back(new Node(*node));
    bool isEnd = false;
    while (pnode->parseIndex < static_cast<int>(pnode->childList.size())) {
        Node* nextNode = pnode->GetParseNode();
        if (nextNode == nullptr) {
            break;
        }
        if (ProjectManager::GetIsUseForceSemiColonInLineEnd() ) {
            if (nextNode->nodeType == ENodeType::SemiColon) {
                isEnd = true;
            }
        } else {
            if (nextNode->nodeType == ENodeType::SemiColon || nextNode->nodeType == ENodeType::LineEnd) {
                isEnd = true;
            }
        }
        if (isEnd) {
            break;
        } else {
            conNode.push_back(new Node(*nextNode));
        }
    }
    return conNode;
}

bool StructParse::CheckEnd(Node* pnode) {
    return pnode->parseIndex >= static_cast<int>(pnode->childList.size());
}

void StructParse::ParseImport(Node* pnode) {
    auto nodeList = GetAllNodeToSemiColon(pnode);
    auto ist = std::make_unique<FileMetaImportSyntax>(nodeList);
    m_FileMeta->AddFileImportSyntax(ist.get());
}

void StructParse::ParseNamespace(Node* pnode) {
    Node* currentNode = pnode->GetParseNode();

    bool isBlock = false;
    Node* namespaceNode = nullptr;
    while (pnode->parseIndex < static_cast<int>(pnode->childList.size())) {
        Node* nextNode = pnode->GetParseNode();
        if (nextNode == nullptr) {
            break;
        }

        if (nextNode->nodeType == ENodeType::Brace) {
            currentNode->blockNode = nextNode;
            isBlock = true;
            break;
        } else if (nextNode->nodeType == ENodeType::IdentifierLink) {
            if (namespaceNode != nullptr) {
                Log::AddInStructFileMeta(EError::None, 
                    "Error �ڽ���namespace �У���߸�ֵ���ж����ʶ���﷨!!");
            }
            namespaceNode = nextNode;

            if (pnode->parseIndex + 1 < static_cast<int>(pnode->childList.size())) {
                auto* next2Node = pnode->childList[pnode->parseIndex + 1];
                if (next2Node && next2Node->nodeType == ENodeType::LineEnd) {
                    if (pnode->parseIndex + 2 < static_cast<int>(pnode->childList.size())) {
                        auto* next3Node = pnode->childList[pnode->parseIndex + 2];
                        if (next3Node && next3Node->nodeType == ENodeType::Brace) {
                            currentNode->blockNode = next2Node;
                            isBlock = true;
                            pnode->parseIndex += 3;
                            break;
                        }
                    }
                } else if (next2Node && next2Node->nodeType == ENodeType::Brace) {
                    currentNode->blockNode = next2Node;
                    isBlock = true;
                    pnode->parseIndex += 2;
                    break;
                }
            }
        } else if (nextNode->nodeType == ENodeType::LineEnd) {
            if (ProjectManager::GetIsUseForceSemiColonInLineEnd() ) {
                Log::AddInStructFileMeta(EError::None, 
                    "Error �ڽ���namespace �У���Ҫǿ��;����");
                break;
            } else {
                break;
            }
        } else if (nextNode->nodeType == ENodeType::SemiColon) {
            break;
        }
    }
    
    auto fmn = new FileMetaNamespace(currentNode, namespaceNode);

    if (isBlock) { // �Ƿ�ʹ�� namespace N{}�ĸ�ʽ ���ʹ��{}��ʽ������Ϊ�鼶ģʽ
        m_FileMeta->AddFileDefineNamespace(fmn);
        AddParseNamespaceNodeInfo(fmn);
        ParseNamespaceOrTopClass(currentNode->blockNode);
        m_CurrentNodeInfoStack.pop();
    } else {
        m_FileMeta->AddFileSearchNamespace(fmn);
    }
}
// ֻ只解析 在全局文件下的 namespace 下 的 还有就是文件class
void StructParse::ParseNamespaceOrTopClass(Node* pnode) {
    Node* braceNode = pnode->blockNode;
    std::vector<Node*> nodeList;
    size_t index = pnode->parseIndex;
    Node* curNode = nullptr;
    bool isCanAdd = false;
    Node* nextNode = nullptr;

    size_t isClass = 0; // 0 unknows 1 class 2namespace
    for (index = pnode->parseIndex; index < pnode->childList.size();) {
        curNode = pnode->childList[index++];
        pnode->parseIndex = index;
        
        if (curNode->nodeType == ENodeType::Key) {
            if (curNode->token->GetType() == ETokenType::Namespace) {
                isClass = 2;
            } else if (curNode->token->GetType() == ETokenType::Class) {
                isClass = 1;
            }
            nodeList.push_back(curNode);
        } else if (curNode->nodeType == ENodeType::LeftAngle) { // Class1<T> 
            nodeList.push_back(curNode);
        } else if (curNode->nodeType == ENodeType::RightAngle) { // Class1<T>   Func<T>( T t );  array<int> arr1;
            nodeList.push_back(curNode);
        } else if (curNode->nodeType == ENodeType::Comma) {
            nodeList.push_back(curNode);
        } else if (curNode->nodeType == ENodeType::IdentifierLink) { // Class1
            nodeList.push_back(curNode);
        } else if (curNode->nodeType == ENodeType::LineEnd) {
            nextNode = nullptr;
            if (index < pnode->childList.size() ) {
                nextNode = pnode->childList[index];
            }
            if (nextNode && nextNode->nodeType == ENodeType::Brace) {
                if (isClass == 0) {
                    isClass = 1;
                }
                curNode = nextNode;
                index++;
                isCanAdd = true;
                break;
            }
        } else if (curNode->nodeType == ENodeType::Brace) {
            isCanAdd = true;
            if (isClass == 0) {
                isClass = 1;
            }
            break;
        } else {
            Log::AddInStructFileMeta(EError::None, 
                "Error 不允许在解释Class的时候，有错误 的语法--------------------" + 
                (curNode->token ? curNode->token->ToLexemeAllString() : "null"));
        }
    }
    pnode->parseIndex = index;

    if (isCanAdd) {
        if (isClass == 1) {
            AddFileMetaClasss(curNode, nodeList);
            ParseNamespaceOrTopClass(pnode);
        } else if (isClass == 2) {
            if (nodeList.size() == 2) {
                auto fmn = new FileMetaNamespace(nodeList[0], nodeList[1]);
                AddParseNamespaceNodeInfo(fmn);
                if (nodeList[1]->blockNode != nullptr) {
                    m_FileMeta->AddFileDefineNamespace(fmn);
                    ParseNamespaceOrTopClass(nodeList[1]->blockNode);
                } else {
                    m_FileMeta->AddFileSearchNamespace(fmn);
                }
                m_CurrentNodeInfoStack.pop();
                ParseNamespaceOrTopClass(pnode);
            } else {
                Log::AddInStructFileMeta(EError::None, 
                    "Error 对于 namespace A.B{}的格式 多了一个参数!1");
            }
        } else {
            Log::AddInStructFileMeta(EError::None, 
                "Error 没有发现是Class还是Namespace的关键字!");
        }
    }
}

void StructParse::ParseClassNode(Node* pnode) {
    while (true) {
        if (CheckEnd(pnode)) {
            break;
        }
        auto* node = pnode->childList[pnode->parseIndex];
        if (node->nodeType == ENodeType::LineEnd) {
            pnode->parseIndex++;
            continue;
        }

        if (node->nodeType == ENodeType::Key) {
            switch (node->token->GetType()) {
                case ETokenType::Data:
                    ParseDataNode(pnode);
                    break;
                case ETokenType::Enum:
                    ParseEnumNode(pnode);
                    break;
                case ETokenType::Const:
                case ETokenType::Class:
                case ETokenType::Extern:
                case ETokenType::Public:
                case ETokenType::Private:
                case ETokenType::Projected:
                case ETokenType::Partial:
                case ETokenType::Static:
                    ParseNamespaceOrTopClass(pnode);
                    break;
                default:
                    ParseSyntax(pnode);
                    break;
            }
        } else if (node->nodeType == ENodeType::IdentifierLink) {
            ParseSyntax(pnode);
        } else {
            Log::AddInStructFileMeta(EError::None, 
                "Error ����ʱ ��Class�ڲ����ֲ�֧�ֵ��﷨ : " + (node->token ? node->token->GetLexemeString() : "null"));
            pnode->parseIndex++;
        }
    }
}

void StructParse::ParseDataBracketNode(Node* bracketNode) {
    if (bracketNode == nullptr) {
        return;
    }
    
    while (true) {
        if (CheckEnd(bracketNode)) {
            break;
        }
        auto* node = bracketNode->childList[bracketNode->parseIndex];
        if (node->nodeType == ENodeType::LineEnd) {
            bracketNode->parseIndex++;
            continue;
        }

        if (node->nodeType == ENodeType::Key) {
            if (node->token->GetType() == ETokenType::Data) {
                ParseDataNode(bracketNode);
            } else {
                Log::AddInStructFileMeta(EError::None, 
                    "Error ��Data��Bracket�г��ֲ�֧�ֵĹؼ��� : " + node->token->GetLexemeString());
                bracketNode->parseIndex++;
            }
        } else if (node->nodeType == ENodeType::IdentifierLink) {
            auto nodeList = GetAllNodeToSemiColon(bracketNode);
            auto cfmmd = new FileMetaMemberData(m_FileMeta, nodeList);
            AddParseDataInfo(cfmmd);
            m_CurrentNodeInfoStack.pop();
        } else {
            Log::AddInStructFileMeta(EError::None, "Error ��Data��Bracket�г��ֲ�֧�ֵ��﷨");
            bracketNode->parseIndex++;
        }
    }
}

void StructParse::ParseDataNode(Node* pnode) {            
    Node* curParentNode = pnode;
    int index = curParentNode->parseIndex;

    bool isParseEnd = false;
    Node* assignNode = nullptr;
    std::vector<Node*> frontList;
    std::vector<Node*> backList;
    for (index = curParentNode->parseIndex; index < static_cast<int>(curParentNode->childList.size());) {
        auto* curNode = curParentNode->childList[index++];
        Node* nextNode = nullptr;
        if (index < static_cast<int>(curParentNode->childList.size())) {
            nextNode = curParentNode->childList[index];
        }

        if (curNode->nodeType == ENodeType::IdentifierLink) {  //Class1
            if (assignNode == nullptr) {
                frontList.push_back(curNode);
            }
            else {
                backList.push_back(curNode);

                for( int j = index; j < static_cast<int>(curParentNode->childList.size()); ) {
                    auto* next2Node = curParentNode->childList[j++];
                    if (next2Node == nullptr) continue;

                    if (next2Node->nodeType == ENodeType::Par) {   //Class1()
                        curNode->parNode = next2Node;
                        index = j;
                        isParseEnd = true;
                        if ( j < static_cast<int>(curParentNode->childList.size()) ) {
                            auto* next3Node = curParentNode->childList[j];
                            if (next3Node == nullptr) continue;
                            if( next3Node->nodeType == ENodeType::LineEnd ) {
                                if( j + 1 < static_cast<int>(curParentNode->childList.size()) ) {
                                    auto* next4Node = curParentNode->childList[j + 1];
                                    if (next4Node == nullptr) continue;
                                    if (next4Node->nodeType == ENodeType::Brace) {
                                        curNode->blockNode = next4Node;
                                        isParseEnd = true;
                                        index = j + 2;
                                        break;
                                    }
                                    else {
                                        isParseEnd = true;
                                        index = j + 1;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else if (next2Node->nodeType == ENodeType::Brace) {
                        curNode->blockNode = next2Node;
                        isParseEnd = true;
                        index = j + 1;
                        break;
                    }
                    else if (next2Node->nodeType == ENodeType::Brace) {
                        curNode->blockNode = next2Node;
                        index = j;
                        isParseEnd = true;
                        break;
                    }
                    if (ProjectManager::GetIsUseForceSemiColonInLineEnd()) {
                        if (next2Node->nodeType == ENodeType::SemiColon) {
                            isParseEnd = true;
                        }
                    }
                    else {
                        if (next2Node->nodeType == ENodeType::SemiColon) {
                            isParseEnd = true;
                        }
                        else if (next2Node->nodeType == ENodeType::LineEnd) {
                            isParseEnd = true;
                        }
                        else {
                            Log::AddInStructFileMeta(EError::None, "Error Data解析中，不能使用超出自动换行!!" + (curNode->token ? curNode->token->ToLexemeAllString() : "null"));
                        }
                    }
                }

                if (isParseEnd) {
                    auto fmmd = new FileMetaMemberData(m_FileMeta, frontList, assignNode, backList, true, FileMetaMemberData::EMemberDataType::Class);
                    frontList.clear();
                    backList.clear();
                    assignNode = nullptr;
                    isParseEnd = false;
                    AddParseDataInfo(fmmd);
                    m_CurrentNodeInfoStack.pop();
                }
            }
        }
        else if (curNode->nodeType == ENodeType::Symbol) {
            if( assignNode == nullptr ) {
                frontList.push_back(curNode);
            }
            else {
                backList.push_back(curNode);
            }
            continue;
        }
        else if (curNode->nodeType == ENodeType::Assign) { //varname = 1/"1"/-20/{}/[]/Class1(){}/Data1(){}
            assignNode = curNode;
            Node* blockNode = nullptr;

            if( nextNode == nullptr ) {
                Log::AddInStructFileMeta(EError::None, "Error 左边变量名不能为空...");
                continue;
            }

            int parseType = 0;
            //=号后面第一位是identifier 还是 constValue值， 如果是identifier，只支持 \n{}  \
            // a = 10 常量值  
            if (nextNode->nodeType == ENodeType::ConstValue) 
            { 
                index++;
                backList.push_back(nextNode);
            }
            else if(nextNode->nodeType == ENodeType::IdentifierLink ) {
            }
            else if (nextNode->nodeType == ENodeType::Symbol && 
                (nextNode->token->GetType() == ETokenType::Plus
                    || nextNode->token->GetType() == ETokenType::Minus ) ) {
                if( index + 1 < static_cast<int>(curParentNode->childList.size()) ) {
                    auto* next2Node = curParentNode->childList[index + 1];
                    if( next2Node->nodeType == ENodeType::ConstValue ) {
                        index+=2;
                        backList.push_back(nextNode);
                        backList.push_back(next2Node);
                    }
                    else {
                        Log::AddInStructFileMeta(EError::None, "Error ");
                    }
                }
                else {
                    Log::AddInStructFileMeta(EError::None, "Error ");
                }
            }
            else if(nextNode->nodeType == ENodeType::Brace ) {
                index++;
                parseType = 1;
                blockNode = nextNode;
            }
            else if( nextNode->nodeType == ENodeType::Bracket ) {
                index++;
                parseType = 2;
                blockNode = nextNode;
            }
            else if (nextNode->nodeType == ENodeType::LineEnd) {
                index++;
                auto* next2Node = index < static_cast<int>(curParentNode->childList.size()) ? curParentNode->childList[index] : nullptr;
                // a = /n{}  a = /n[]
                if (next2Node && next2Node->nodeType == ENodeType::Brace) {
                    index++;
                    parseType = 1;
                    blockNode = next2Node;
                }
                else if (next2Node && next2Node->nodeType == ENodeType::Bracket) {
                    index++;
                    parseType = 2;
                    blockNode = next2Node;
                }
                else {
                    Log::AddInStructFileMeta(EError::None, "Error ");
                }
            }
            else {
                Log::AddInStructFileMeta(EError::None, "Error ");
            }

            if( parseType > 0 ) {
                FileMetaMemberData::EMemberDataType emdt = parseType == 1 ? FileMetaMemberData::EMemberDataType::Data : FileMetaMemberData::EMemberDataType::Array;
                auto fmmd = new FileMetaMemberData(m_FileMeta, frontList, assignNode, backList, true, emdt);
                frontList.clear();
                isParseEnd = false;
                assignNode = nullptr;
                AddParseDataInfo(fmmd);
                if( parseType == 1 ) {
                    ParseDataNode(blockNode);
                }
                else if( parseType == 2 ) {
                    ParseDataBracketNode(blockNode);
                }
                m_CurrentNodeInfoStack.pop();
            }
            continue;
        }
        else if (curNode->nodeType == ENodeType::LineEnd) {
            isParseEnd = true;
        }
        else if (curNode->nodeType == ENodeType::SemiColon) {
            isParseEnd = true;
        }
        else {
            Log::AddInStructFileMeta(EError::None, "Error 解析时出现 解析Data数据时，不支持的语法错误!" + (curNode->token ? curNode->token->ToLexemeAllString() : "null") );
        }

        if (isParseEnd) {
            if (frontList.size() > 0) {
                auto fmmd = new FileMetaMemberData(m_FileMeta, frontList, assignNode, backList, true, FileMetaMemberData::EMemberDataType::ConstValue );
                frontList.clear();
                backList.clear();
                assignNode = nullptr;
                AddParseDataInfo(fmmd);
                m_CurrentNodeInfoStack.pop();
            }
            isParseEnd = false;
        }
    }
    curParentNode->parseIndex = index;            
}

void StructParse::ParseEnumNode(Node* pnode) {
    auto nodeList = GetAllNodeToSemiColon(pnode);
    auto fmmv = new FileMetaMemberVariable(m_FileMeta, nodeList);
    AddParseVariableInfo(fmmv);
}

void StructParse::ParseSyntax(Node* pnode) {
    Node* node = pnode->GetParseCurrent();
    if (node == nullptr) return;

    if (node->nodeType == ENodeType::Brace) {
        auto cps = new FileMetaBlockSyntax(m_FileMeta, node->token, node->endToken);

        AddParseSyntaxNodeInfo(cps, true);

        m_CurrentNodeInfoStack.pop();

        pnode->parseIndex++;
    }
    else {
        HandleCreateFileMetaSyntaxByPNode(pnode);
    }
    ParseSyntax(pnode);
}

std::vector<Node*> StructParse::HandleBeforeNode(Node* node) {
    std::vector<Node*> result;
    if (node == nullptr || node->childList.empty()) {
        return result;
    }
    
    // ���ƽڵ��б�
    for (size_t i = 0; i < node->childList.size(); i++) {
        result.push_back(node->childList[i]);
    }
    
    // ��������ʽ�ڵ�
    _HandleExpressNodeProcess(node);
    
    return result;
}

std::vector<Node*> StructParse::HandleExpressNode(Node* node) {
    std::vector<Node*> result;
    if (node == nullptr) {
        return result;
    }
    
    // ��������ʽ�ڵ�
    _HandleExpressNodeProcess(node);
    
    // ���ش�����Ľڵ��б�
    if (!node->childList.empty()) {
        for (size_t i = 0; i < node->childList.size(); i++) {
            result.push_back(node->childList[i]);
        }
    }
    
    return result;
}

void StructParse::DelHandleNostList(Node* node) {
    if (node == nullptr) {
        return;
    }
    
    // ��������Ҫ�Ľڵ�
    for (size_t i = 0; i < node->childList.size(); i++) {
        Node* child = node->childList[i];
        if (child != nullptr) {
            // �ݹ鴦���ӽڵ�
            DelHandleNostList(child);
        }
    }
}

bool StructParse::IsCommonExpressNode(Node* node) {
    if (node == nullptr) {
        return false;
    }
    
    // �ж��Ƿ��ǳ����ı���ʽ�ڵ�����
    switch (node->nodeType) {
        case ENodeType::IdentifierLink:
        case ENodeType::Key:
        case ENodeType::LeftAngle:
        case ENodeType::RightAngle:
        case ENodeType::Comma:
        case ENodeType::SemiColon:
        case ENodeType::LineEnd:
            return true;
        default:
            return false;
    }
}
void StructParse::_HandleExpressNodeProcess(Node* node, Node* inputFinaleNode) {
    if (node->parseIndex < 0 || node->parseIndex >= static_cast<int>(node->childList.size())) {
        return;
    }

    int index = node->parseIndex;
    Node* currentExpressNode = node->GetParseCurrent();
    if (inputFinaleNode == nullptr) {
        node->parseIndex++;
        _HandleExpressNodeProcess(node, currentExpressNode);
        return;
    }

    Node* finalNode = inputFinaleNode->GetFinalNode();
    if (finalNode && (finalNode->nodeType == ENodeType::IdentifierLink
        || finalNode->token->GetType() == ETokenType::New)) {
        if (currentExpressNode->nodeType == ENodeType::LeftAngle) {
            HandleAngleExpressNode(node, finalNode);
            _HandleExpressNodeProcess(node, finalNode);
            return;
        }
        else if (currentExpressNode->nodeType == ENodeType::Par) {
            node->parseIndex++;
            finalNode->parNode = currentExpressNode;
            currentExpressNode->SetIsDel( true );
            if (currentExpressNode->GetExtendLinkNodeList().size() > 0) {
                finalNode->SetLinkNode(currentExpressNode->GetExtendLinkNodeList() );
                _HandleExpressNodeProcess(node, currentExpressNode);
                return;
            }
            else {
                _HandleExpressNodeProcess(currentExpressNode, nullptr);
                DelHandleNostList(currentExpressNode);
                _HandleExpressNodeProcess(node, finalNode);
                return;
            }
        }
        else if (currentExpressNode->nodeType == ENodeType::Brace) {
            node->parseIndex++;
            finalNode->blockNode = currentExpressNode;
            currentExpressNode->SetIsDel( true );
            _HandleExpressNodeProcess(currentExpressNode, nullptr);
            return;
        }
        else if (currentExpressNode->nodeType == ENodeType::Bracket) {
            node->parseIndex++;
            finalNode->bracketNode = currentExpressNode;
            currentExpressNode->SetIsDel(true);

            if (currentExpressNode->GetExtendLinkNodeList().size() > 0) {
                finalNode->SetLinkNode(currentExpressNode->GetExtendLinkNodeList() );
                _HandleExpressNodeProcess(currentExpressNode, nullptr);
                return;
            }
            else {
                _HandleExpressNodeProcess(currentExpressNode, nullptr);
                return;
            }
        }
    }
    node->parseIndex++;
    _HandleExpressNodeProcess(node, currentExpressNode);
}
void StructParse::HandleAngleExpressNode(Node* node, Node* parentNode) {
    while (node->parseIndex < static_cast<int>(node->childList.size())) {
        Node* cnode = node->childList[node->parseIndex];
        if (cnode->nodeType == ENodeType::LeftAngle) {
            cnode->SetIsDel( true );
            node->parseIndex++;
            parentNode->angleNode = cnode;
        }
        else if (cnode->nodeType == ENodeType::RightAngle) {
            cnode->SetIsDel( true );
            parentNode->angleNode->endToken = cnode->token;
            node->parseIndex++;
            if (cnode->GetExtendLinkNodeList().size() > 0) {
                parentNode->SetLinkNode(cnode->GetExtendLinkNodeList() );
            }
            return;
        }
        else if (cnode->nodeType == ENodeType::Comma) {
            cnode->SetIsDel( true );
            node->parseIndex++;
            continue;
        }
        else if (cnode->nodeType == ENodeType::IdentifierLink) {
            node->parseIndex++;
            if (parentNode->angleNode != nullptr) {
                cnode->SetIsDel(true);
                parentNode->angleNode->AddChild(cnode);
            }
            if (node->parseIndex < static_cast<int>(node->childList.size())) {
                Node* nextNode = node->childList[node->parseIndex];
                if (nextNode->nodeType == ENodeType::LeftAngle) {
                    HandleAngleExpressNode(node, cnode);
                }
                else if (nextNode->nodeType == ENodeType::Key
                    && nextNode->token->GetType() == ETokenType::Colon) {
                    if (node->parseIndex + 1 < static_cast<int>(node->childList.size())) {
                        nextNode->SetIsDel(true);
                        Node* nextNode2 = node->childList[node->parseIndex + 1];
                        nextNode2->SetIsDel(true);
                        cnode->AddChild(nextNode2);
                        node->parseIndex += 2;
                    }
                }
                else if (nextNode->nodeType == ENodeType::RightAngle) {
                    HandleAngleExpressNode(node, parentNode);
                    return;
                }
                else if (nextNode->nodeType == ENodeType::Comma) {
                    continue;
                }
                else if (nextNode->nodeType == ENodeType::Par) {
                    parentNode->parNode = nextNode;
                    nextNode->SetIsDel(true);
                    _HandleExpressNodeProcess(nextNode, nullptr);

                    if (nextNode->GetExtendLinkNodeList().size() > 0) {
                        parentNode->SetLinkNode(nextNode->GetExtendLinkNodeList() );
                        _HandleExpressNodeProcess(node, parentNode);
                        return;
                    }
                    else {
                        parentNode = parentNode->GetFinalNode();
                        if (node->parseIndex < static_cast<int>(node->childList.size())) {
                            Node* next2ExpressNode = node->childList[node->parseIndex];
                            if (next2ExpressNode->nodeType == ENodeType::Brace) {
                                parentNode->blockNode = next2ExpressNode;
                                next2ExpressNode->SetIsDel( true );
                                _HandleExpressNodeProcess(node, parentNode);
                                return;
                            }
                        }
                    }
                }
                else {
                    HandleAngleExpressNode(node, parentNode);
                    return;
                }
            }
        }
        else {
            node->parseIndex++;
            if (parentNode->angleNode != nullptr) {
                parentNode->angleNode->AddChild(cnode);
            }
            else {
                parentNode->AddChild(cnode);
            }
        }
    }
}

void StructParse::AddFileMetaFunctionVariable(Node* pnode, Node* blockNode, const std::vector<Node*>& nodeList) {
    if (nodeList.empty()) {
        return;
    }
    
    // ����FileMetaMemberFunction����
    auto fmmf = new FileMetaMemberFunction(m_FileMeta, nullptr, nodeList);
    
    // ���ӵ�����ջ
    AddParseFunctionNodeInfo(fmmf, true);
    
    // ����п�ڵ㣬����������
    if (blockNode != nullptr) {
        ParseSyntax(blockNode);
    }
    
    // ����ջ�е�����
    m_CurrentNodeInfoStack.pop();
}

void StructParse::AddFileMetaClasss(Node* blockNode, const std::vector<Node*>& nodeList) {
    auto fmc = new FileMetaClass(m_FileMeta, nodeList);
    AddParseClassNodeInfo(fmc);
    if (blockNode != nullptr) {
        ParseClassNode(blockNode);
    }
    m_CurrentNodeInfoStack.pop();
}

StructParse::SyntaxNodeStruct StructParse::GetOneSyntax(Node* pnode, Condition* condition) {
    SyntaxNodeStruct keynodeStruct;
    if (pnode->parseIndex >= static_cast<int>(pnode->childList.size())) {
        return keynodeStruct;
    }
    int index = 0;
    int tCurIndex = 0;
    Node* curNode = nullptr;
    Token* curToken = nullptr;
    ENodeType curNodeType = ENodeType::None;
    while (pnode->parseIndex < static_cast<int>(pnode->childList.size())) {
        tCurIndex = pnode->parseIndex + index++;
        curNode = nullptr;
        if (tCurIndex < static_cast<int>(pnode->childList.size())) {
            curNode = pnode->childList[tCurIndex];
        }
        if (curNode == nullptr) {
            break;
        }
        curNodeType = curNode->nodeType;
        curToken = curNode->token;
        if (curToken == nullptr) {
            break;
        }

        if (curNodeType == ENodeType::LineEnd) {
            if (keynodeStruct.IsLineEndBreak()) {
                if (ProjectManager::GetIsUseForceSemiColonInLineEnd()) {
                    Log::AddInStructFileMeta(EError::None, "warning");// 使用的强制分号结束语句方式，注意在节点继承下不匹配" + (curToken ? curToken->ToLexemeAllString() : "null"));
                }
                break;
            }
        }
        else if (curNodeType == ENodeType::SemiColon) {
            break;
        }
        else if (curNodeType == ENodeType::Brace) {
            bool isMustContactBrace = false;
            ETokenType ttt = keynodeStruct.tokenType;
            if (ttt == ETokenType::If
                || ttt == ETokenType::ElseIf
                || ttt == ETokenType::Else
                || ttt == ETokenType::For
                || ttt == ETokenType::While
                || ttt == ETokenType::DoWhile
                || ttt == ETokenType::Switch
                || ttt == ETokenType::Case
                || ttt == ETokenType::Label) { // ClassName(){}
                isMustContactBrace = true;
            }

            if (isMustContactBrace) {
                keynodeStruct.SetBraceNode(curNode);
            }
            else {
                if (keynodeStruct.eSyntaxNodeType == ESyntaxNodeStructType::CommonSyntax) {
                    keynodeStruct.AddContent(curNode);
                }
            }
            break;
        }
        else if (curNodeType == ENodeType::Key) {
            if (condition != nullptr && condition->isCheck) {
                if (!condition->isFirstKey) {
                    index = 0;
                    break;
                }
                if (!condition->IsMatchTokenType(curToken->GetType())) {
                    index = 0;
                    break;
                }
                condition->isCheck = false;
            }

            ETokenType ttt = curNode->token->GetType();
            if (ttt == ETokenType::If
                || ttt == ETokenType::ElseIf
                || ttt == ETokenType::Else
                || ttt == ETokenType::Switch
                || ttt == ETokenType::For
                || ttt == ETokenType::While || curNode->token->GetType() == ETokenType::DoWhile
                || ttt == ETokenType::Case
                || ttt == ETokenType::Default
                || ttt == ETokenType::Return
                || ttt == ETokenType::Transience
                || ttt == ETokenType::Next
                || ttt == ETokenType::Break
                || ttt == ETokenType::Continue
                || ttt == ETokenType::Label
                || ttt == ETokenType::Goto
                || ttt == ETokenType::Const) {
                keynodeStruct.SetMainKeyNode(curNode);
            }
            else if (ttt == ETokenType::Data) {
                keynodeStruct.AddContent(curNode);
            }
            else if (ttt == ETokenType::Var) {
                keynodeStruct.AddContent(curNode);
            }
            else if (ttt == ETokenType::In) {
                keynodeStruct.AddContent(curNode);
            }
            else if (ttt == ETokenType::Dynamic) {
                keynodeStruct.AddContent(curNode);
            }
            else if (ttt == ETokenType::This
                || ttt == ETokenType::Base
                || ttt == ETokenType::New) {
                keynodeStruct.AddContent(curNode);
            }
            else {
                Log::AddInStructFileMeta(EError::None, "Error parse exception keyworks");
            }
        }
        else {
            if (condition != nullptr && condition->isCheck) {
                if (condition->isFirstKey) {
                    index = 0;
                    break;
                }
            }
            keynodeStruct.AddContent(curNode);
        }
    }
    keynodeStruct.moveIndex = index;
    return keynodeStruct;
}

FileMetaSyntax* StructParse::HandleCreateFileMetaSyntaxByPNode(Node* pnode) {
    FileMetaSyntax* fms = nullptr;
    SyntaxNodeStruct akss = GetOneSyntax(pnode);
    pnode->parseIndex += akss.moveIndex;
    if (akss.eSyntaxNodeType == ESyntaxNodeStructType::None) {
    }
    else if (akss.eSyntaxNodeType == ESyntaxNodeStructType::CommonSyntax) {
        if (akss.commonContent.size() > 0) {
            fms = CrateFileMetaSyntaxNoKey(akss.commonContent);
            AddParseSyntaxNodeInfo(fms, false);
        }
    }
    else if (akss.eSyntaxNodeType == ESyntaxNodeStructType::KeySyntax) {
        if (akss.tokenType == ETokenType::If) {
            while (true) {
                Condition condition(ETokenType::ElseIf);
                condition.AddTokenTypeList(ETokenType::Else);
                SyntaxNodeStruct cakss = GetOneSyntax(pnode, &condition);
                if (cakss.eSyntaxNodeType == ESyntaxNodeStructType::None) {
                    if (cakss.moveIndex == 0)
                        break;
                    pnode->parseIndex += cakss.moveIndex;
                    continue;
                }

                if (cakss.tokenType == ETokenType::ElseIf) {
                    pnode->parseIndex += cakss.moveIndex;
                    akss.followKeySyntaxStructList.push_back(new SyntaxNodeStruct(cakss));
                    continue;
                }
                else if (cakss.tokenType == ETokenType::Else) {
                    pnode->parseIndex += cakss.moveIndex;
                    akss.followKeySyntaxStructList.push_back(new SyntaxNodeStruct(cakss));
                }
                break;
            }
            // 这里需要实现FileMetaKeyIfSyntax::ParseIfSyntax
            // fms = FileMetaKeyIfSyntax::ParseIfSyntax(m_FileMeta, akss);
            AddParseSyntaxNodeInfo(fms, false);
        }
        else if (akss.tokenType == ETokenType::Switch) {
            while (true) {
                Condition condition(ETokenType::Case);
                condition.AddTokenTypeList(ETokenType::Default);
                SyntaxNodeStruct cakss = GetOneSyntax(akss.blockNode, &condition);
                if (cakss.eSyntaxNodeType == ESyntaxNodeStructType::None)
                    break;
                akss.childrenKeySyntaxStructList.push_back(new SyntaxNodeStruct(cakss));
            }
            // 这里需要实现ParseSwitchSyntax
            // fms = ParseSwitchSyntax(m_FileMeta, akss);
            AddParseSyntaxNodeInfo(fms, false);
        }
        else if (akss.tokenType == ETokenType::For) {
            // 这里需要实现ParseForSyntax
            // fms = ParseForSyntax(m_FileMeta, akss);
            AddParseSyntaxNodeInfo(fms, false);
        }
        else if (akss.tokenType == ETokenType::While || akss.tokenType == ETokenType::DoWhile) {
            // 这里需要实现ParseConditionSyntax
            // fms = ParseConditionSyntax(m_FileMeta, akss);
            AddParseSyntaxNodeInfo(fms, false);
        }
        else if (akss.tokenType == ETokenType::Return || akss.tokenType == ETokenType::Transience) {
            // 这里需要实现FileMetaKeyReturnSyntax
            // fms = new FileMetaKeyReturnSyntax(m_FileMeta, akss.keyNode->token, conditionExpress);
            AddParseSyntaxNodeInfo(fms, false);
        }
        else if (akss.tokenType == ETokenType::Label || akss.tokenType == ETokenType::Goto) {
            Token* labelToken = nullptr;
            if (akss.keyContent.size() != 1) {
                Log::AddInStructFileMeta(EError::None, "Error 解析Goto Label语法，只支持 goto id; 这种语法!!");
            }
            else {
                labelToken = akss.keyContent[0]->token;
                if (labelToken->GetType() != ETokenType::Identifier) {
                    Log::AddInStructFileMeta(EError::None, "Error parse GotoLabel right label must used origenal keyworks");
                }
            }
            // 这里需要实现FileMetaKeyGotoLabelSyntax
            // fms = new FileMetaKeyGotoLabelSyntax(m_FileMeta, akss.keyNode->token, labelToken);
            AddParseSyntaxNodeInfo(fms, false);
        }
        else if (akss.tokenType == ETokenType::Break || akss.tokenType == ETokenType::Continue) {
            // 这里需要实现FileMetaKeyOnlySyntax
            // fms = new FileMetaKeyOnlySyntax(m_FileMeta, akss.keyNode->token, nullptr);
            AddParseSyntaxNodeInfo(fms, false);
        }
    }
    return fms;
}

FileMetaSyntax* StructParse::CrateFileMetaSyntaxNoKey(const std::vector<Node*>& pNodeList) {
    std::vector<Node*> beforeNodeList;
    Node* assignNode = nullptr;
    Node* opAssignNode = nullptr;
    ETokenType tet = ETokenType::None;
    std::vector<Node*> afterNodeList;
    
    for (size_t j = 0; j < pNodeList.size(); j++) {
        auto* cnode = pNodeList[j];
        if (cnode->nodeType == ENodeType::Assign) {
            if (assignNode == nullptr && opAssignNode == nullptr) {
                assignNode = cnode;
                continue;
            }
        }
        else {
            tet = cnode->token->GetType();
            if (tet == ETokenType::PlusAssign
                || tet == ETokenType::MinusAssign
                || tet == ETokenType::MultiplyAssign
                || tet == ETokenType::DivideAssign
                || tet == ETokenType::ModuloAssign
                || tet == ETokenType::InclusiveOrAssign
                || tet == ETokenType::CombineAssign
                || tet == ETokenType::XORAssign
                || tet == ETokenType::DoublePlus
                || tet == ETokenType::DoubleMinus) {
                if (assignNode == nullptr && opAssignNode == nullptr) {
                    opAssignNode = cnode;
                    continue;
                }
            }
        }
        if (assignNode != nullptr || opAssignNode != nullptr)
            afterNodeList.push_back(cnode);
        else {
            beforeNodeList.push_back(cnode);
        }
    }
    
    if (beforeNodeList.empty()) {
        Log::AddInStructFileMeta(EError::None, "Error parse find sign express not include pre-viarble!");
        return nullptr;
    }

    Token* staticToken = nullptr;
    Token* dynamicToken = nullptr;
    Token* varToken = nullptr;
    Token* dataToken = nullptr;
    Token* nameToken = nullptr;
    Compile::FileMetaClassDefine* classRef = nullptr;
    Compile::FileMetaCallLink* varRef = nullptr;

    Node parseNode(nullptr);
    parseNode.childList = beforeNodeList;
    parseNode.parseIndex = 0;
    auto handleBeforeList = HandleBeforeNode(&parseNode);

    std::vector<Node*> defineNodeList;
    for (size_t i = 0; i < handleBeforeList.size(); i++) {
        auto* cnode = handleBeforeList[i];
        if (cnode->nodeType == ENodeType::IdentifierLink) {
            defineNodeList.push_back(cnode);
        }
        else {
            Token* token = cnode->token;
            if (token->GetType() == ETokenType::This || token->GetType() == ETokenType::Base) {
                defineNodeList.push_back(cnode);
            }
            else if (token->GetType() == ETokenType::Static) {
                if (staticToken != nullptr) {
                    Log::AddInStructFileMeta(EError::None, "Error 重复Static!!");
                }
                staticToken = token;
            }
            else if (token->GetType() == ETokenType::Type || token->GetType() == ETokenType::String) {
                defineNodeList.push_back(cnode);
            }
            else if (token->GetType() == ETokenType::Dynamic) {
                if (varToken != nullptr || dynamicToken != nullptr || dataToken != nullptr) {
                    Log::AddInStructFileMeta(EError::None, "Error 重复Dynamic!!");
                }
                dynamicToken = token;
                defineNodeList.push_back(cnode);
            }
            else if (token->GetType() == ETokenType::Var) {
                if (varToken != nullptr || dynamicToken != nullptr || dataToken != nullptr) {
                    Log::AddInStructFileMeta(EError::None, "Error 重复Var!!");
                }
                varToken = token;
                defineNodeList.push_back(cnode);
            }
            else if (token->GetType() == ETokenType::Data) {
                if (varToken != nullptr || dynamicToken != nullptr || dataToken != nullptr) {
                    Log::AddInStructFileMeta(EError::None, "Error 重复Data!!");
                }
                dataToken = token;
                defineNodeList.push_back(cnode);
            }
            else {
                Log::AddInStructFileMeta(EError::None, "Error 解析时出现没有该节点!!" + (token ? token->ToLexemeAllString() : "null"));
            }
        }
    }
    
    if (defineNodeList.empty() || defineNodeList.size() > 3) {
        Log::AddInStructFileMeta(EError::None, "Error 解析时出现错误1");
        return nullptr;
    }
    else if (defineNodeList.size() == 1) {
        nameToken = defineNodeList[0]->token;
        varRef = new FileMetaCallLink(m_FileMeta, defineNodeList[0]);
    }
    else if (defineNodeList.size() == 2) {
        if (varToken != nullptr || dynamicToken != nullptr || dataToken != nullptr) {
            nameToken = defineNodeList[1]->token;
            varRef = new FileMetaCallLink(m_FileMeta, defineNodeList[1]);
        }
        else {
            classRef = new FileMetaClassDefine(m_FileMeta, defineNodeList[0]);
            auto* node2 = defineNodeList[1];
            nameToken = node2->token;
        }
    }

    FileMetaBaseTerm* fme = nullptr;
    if (assignNode != nullptr && afterNodeList.size() > 0) {
        if (afterNodeList[0]->nodeType == ENodeType::Key
            && afterNodeList[0]->token->GetType() != ETokenType::This
            && afterNodeList[0]->token->GetType() != ETokenType::Base
            && afterNodeList[0]->token->GetType() != ETokenType::New) {
            Log::AddInStructFileMeta(EError::None, "Error 暂不支持 a = if/switch{}语法");
        }
    }

    if (fme == nullptr) {
        fme = FileMetatUtil::CreateFileMetaExpress(m_FileMeta, afterNodeList, EExpressType::Common);
    }

    if (assignNode != nullptr) {
        if (nameToken == nullptr) {
            Log::AddInStructFileMeta(EError::None, "Error 作为赋值的时候，名字不能为空!!");
            return nullptr;
        }
        if (classRef != nullptr) {
            auto* fmdvs = new FileMetaDefineVariableSyntax(m_FileMeta, classRef,
                nameToken, assignNode->token, staticToken, fme);
            return fmdvs;
        }
        if (varRef != nullptr) {
            auto* fms = new FileMetaOpAssignSyntax(varRef, assignNode->token, dynamicToken, dataToken, varToken, fme, true);
            return fms;
        }
    }
    else if (opAssignNode != nullptr) {
        if (varRef == nullptr) {
            Log::AddInStructFileMeta(EError::None, "Error 作为赋值的时候，名字不能为空!!");
            return nullptr;
        }
        auto* fms = new FileMetaOpAssignSyntax(varRef, opAssignNode->token, dynamicToken, dataToken, varToken, fme);
        return fms;
    }
    else {
        if (nameToken == nullptr) {
            Log::AddInStructFileMeta(EError::None, "Error 作为赋值的时候，名字不能为空!!");
            return nullptr;
        }
        if (classRef != nullptr) {
            auto* fmdvs = new FileMetaDefineVariableSyntax(m_FileMeta, classRef, nameToken, staticToken, nullptr, nullptr);
            return fmdvs;
        }
        else {
            auto* fmcs = new FileMetaCallSyntax(varRef);
            return fmcs;
        }
    }
    return nullptr;
}

} // namespace Compile
} // namespace SimpleLanguage