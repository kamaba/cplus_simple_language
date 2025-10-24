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
#include "../FileMeta/FileMetaMemberMethod.h"
#include "../FileMeta/FileMetaMemberVariable.h"
#include "../FileMeta/FileMetaNamespace.h"
#include "../FileMeta/FileMetaSyntax.h"
#include "Node.h"
#include "../../Debug/Log.h"
#include "../../Project/ProjectManager.h"
#include "StructParse.h"
#include <iostream>

using namespace SimpleLanguage::Debug;
using namespace SimpleLanguage::Project;

namespace SimpleLanguage {
namespace Compile {

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
                        "Error ����ʱ ��Fileͷ��Ŀ¼�г��� : " + node->token->GetLexemeString());
                    break;
            }
        } else if (node->nodeType == ENodeType::IdentifierLink) {
            ParseNamespaceOrTopClass(pnode);
        } else {
            Log::AddInStructFileMeta(EError::None, 
                "Error ����ʱ ��Fileͷ��Ŀ¼�г���2 : " + (node->token ? node->token->GetLexemeString() : "null"));
        }
    }

    auto fileCode = std::move(m_CurrentNodeInfoStack.top());
    m_CurrentNodeInfoStack.pop();

    if (fileCode->parseType == EParseNodeType::File) {
#ifdef DEBUG
        m_FileMeta->SetDeep(0);
#endif
        Log::AddProcess(EProcess::ParseNode, EError::None, 
            "����Code�ļ��ṹ�ļ��ɹ�!!! ��һ����ʼ ����Meta�ļ� \n ����FileMeta�ļ��ɹ�!!! ��һ����ʼ �﷨����");
    } else {
        Log::AddProcess(EProcess::ParseNode, EError::ParseFileError, 
            "����ʱ���ִ��� ParseFile : " + std::to_string(static_cast<int>(GetCurrentNodeInfo()->parseType)));
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
        if (ProjectManager::isUseForceSemiColonInLineEnd) {
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
            if (ProjectManager::isUseForceSemiColonInLineEnd) {
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
// ֻ���� ȫ���ļ��µ� namespace �� ȫ���ļ�class
void StructParse::ParseNamespaceOrTopClass(Node* pnode) {
    Node* braceNode = pnode->blockNode;
    std::vector<Node*> nodeList;
    int index = pnode->parseIndex;
    Node* curNode = nullptr;
    bool isCanAdd = false;
    Node* nextNode = nullptr;

    int isClass = 0; // 0 unknows 1 class 2namespace
    for (index = pnode->parseIndex; index < static_cast<int>(pnode->childList.size());) {
        curNode = pnode->childList[index++];
        pnode->parseIndex = index;
        
        Node* newnode = new Node(*curNode);
        if (curNode->nodeType == ENodeType::Key) {
            if (curNode->token->GetType() == ETokenType::Namespace) {
                isClass = 2;
            } else if (curNode->token->GetType() == ETokenType::Class) {
                isClass = 1;
            }
            nodeList.push_back(newnode);
        } else if (curNode->nodeType == ENodeType::LeftAngle) { // Class1<T> 
            nodeList.push_back(newnode);
        } else if (curNode->nodeType == ENodeType::RightAngle) { // Class1<T>   Func<T>( T t );  array<int> arr1;
            nodeList.push_back(newnode);
        } else if (curNode->nodeType == ENodeType::Comma) {
            nodeList.push_back(newnode);
        } else if (curNode->nodeType == ENodeType::IdentifierLink) { // Class1
            nodeList.push_back(newnode);
        } else if (curNode->nodeType == ENodeType::LineEnd) {
            nextNode = nullptr;
            if (index < static_cast<int>(pnode->childList.size())) {
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
                "Error ����ʱ�ڽ���Classʱ���ִ��� �﷨--------------------" + 
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
                    "Error ���� namespace A.B{}�ĸ�ʽ ֻ����һ����ʶ��!1");
            }
        } else {
            Log::AddInStructFileMeta(EError::None, 
                "Error û�з���Class����Namespace�Ĺؼ���!");
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
    auto nodeList = GetAllNodeToSemiColon(pnode);
    auto fmmd = new FileMetaMemberData(m_FileMeta, nodeList);
    AddParseDataInfo(fmmd);
    
    for (size_t i = 0; i < fmmd->GetFileMetaMemberData().size(); i++) {
        auto cfmmd = fmmd->GetFileMetaMemberData()[i];
        if (cfmmd->GetMemberDataType() == FileMetaMemberData::EMemberDataType::Data) {
            //ParseDataBracketNode(cfmmd->node);
        }
    }
    m_CurrentNodeInfoStack.pop();
}

void StructParse::ParseEnumNode(Node* pnode) {
    auto nodeList = GetAllNodeToSemiColon(pnode);
    auto fmmv = new FileMetaMemberVariable(m_FileMeta, nodeList);
    AddParseVariableInfo(fmmv);
}

void StructParse::ParseSyntax(Node* pnode) {
    auto fms = HandleCreateFileMetaSyntaxByPNode(pnode);
    if (fms != nullptr) {
        AddParseSyntaxNodeInfo(fms, false);
    }
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
    if (node == nullptr) {
        return;
    }
    
    // ��������ʽ�ڵ�
    for (size_t i = 0; i < node->childList.size(); i++) {
        Node* child = node->childList[i];
        if (child != nullptr) {
            // �����Ƕȱ���ʽ�ڵ�
            if (child->nodeType == ENodeType::LeftAngle || child->nodeType == ENodeType::RightAngle) {
                HandleAngleExpressNode(child, node);
            }
            
            // �ݹ鴦���ӽڵ�
            _HandleExpressNodeProcess(child, inputFinaleNode);
        }
    }
}

void StructParse::HandleAngleExpressNode(Node* node, Node* parentNode) {
    if (node == nullptr || parentNode == nullptr) {
        return;
    }
    
    // �����Ƕȱ���ʽ�ڵ㣨ģ����� <T>��
    if (node->nodeType == ENodeType::LeftAngle) {
        // ������ǶȽڵ�
        for (size_t i = 0; i < node->childList.size(); i++) {
            Node* child = node->childList[i];
            if (child != nullptr && child->nodeType == ENodeType::IdentifierLink) {
                // ����ģ�����
                // �����������ģ������Ĵ����߼�
            }
        }
    } else if (node->nodeType == ENodeType::RightAngle) {
        // �����ҽǶȽڵ�
        // �����������ģ����������Ĵ����߼�
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

FileMetaSyntax* StructParse::HandleCreateFileMetaSyntaxByPNode(Node* pnode) {
    auto nodeList = GetAllNodeToSemiColon(pnode);
    if (nodeList.empty()) {
        return nullptr;
    }
    
    // 根据第一个节点判断是什么类型的语句
    Node* firstNode = nodeList[0];
    if (firstNode->nodeType == ENodeType::Key && firstNode->token != nullptr) {
        switch (firstNode->token->GetType()) {
            case ETokenType::If:
                return FileMetaKeyIfSyntax::ParseIfSyntax(m_FileMeta, pnode);
            case ETokenType::While:
            case ETokenType::DoWhile:
                return new FileMetaKeyOnlySyntax(m_FileMeta, firstNode->token, nullptr);
            case ETokenType::For:
                return new FileMetaKeyForSyntax(m_FileMeta, firstNode->token, nullptr);
            case ETokenType::Switch:
                return new FileMetaKeySwitchSyntax(m_FileMeta, firstNode->token, nullptr, nullptr, nullptr);
            case ETokenType::Return: {
                FileMetaBaseTerm* returnExpr = nodeList.size() > 1 ? nullptr : nullptr; // 需要解析表达式
                return new FileMetaKeyReturnSyntax(m_FileMeta, firstNode->token, returnExpr);
            }
            case ETokenType::Break:
            case ETokenType::Continue:
                return new FileMetaKeyOnlySyntax(m_FileMeta, firstNode->token, nullptr);
            case ETokenType::Goto:
            case ETokenType::Label: {
                Token* labelToken = nodeList.size() > 1 ? nodeList[1]->token : nullptr;
                return new FileMetaKeyGotoLabelSyntax(m_FileMeta, firstNode->token, labelToken);
            }
            default:
                // 其他关键字作为普通语句处理
                return new FileMetaCallSyntax(nullptr);
        }
    }
    
    // 默认为调用语句处理
    return new FileMetaCallSyntax(nullptr);
}

} // namespace Compile
} // namespace SimpleLanguage