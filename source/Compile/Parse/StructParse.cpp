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
#include "../FileMeta/FileMetaSyntax.h"
#include "../../Debug/Log.h"
#include "../../Project/ProjectManager.h"
#include "StructParse.h"
#include <iostream>

using namespace SimpleLanguage::Debug;
using namespace SimpleLanguage::Project;

namespace SimpleLanguage {
namespace Compile {

// ParseCurrentNodeInfo 构造函数实现
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

StructParse::StructParse(FileMeta& fm, Node& node) 
    : m_FileMeta(fm), m_RootNode(node) {
}

StructParse::ParseCurrentNodeInfo* StructParse::GetCurrentNodeInfo() {
    if (m_CurrentNodeInfoStack.empty()) return nullptr;
    return m_CurrentNodeInfoStack.top();
}

void StructParse::AddParseFileNodeInfo() {
    auto pcni =new ParseCurrentNodeInfo(&m_FileMeta);
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
        Log::AddInStructFileMeta(EError::None, "错误 !!1 AddParseClassNodeInfo");
        return;
    }
    m_FileMeta.AddFileMetaAllClass(fmc);

    auto pcni = new ParseCurrentNodeInfo(fmc);
    m_CurrentNodeInfoStack.push(pcni);
}

void StructParse::AddParseVariableInfo(FileMetaMemberVariable* csv) {
    auto* currentNodeInfo = GetCurrentNodeInfo();
    if (currentNodeInfo->parseType == EParseNodeType::Class) {
        currentNodeInfo->codeClass->AddFileMemberVariable(csv);
    } else {
        Log::AddInStructFileMeta(EError::None, "错误 !!1 AddParseVariableInfo");
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
        Log::AddInStructFileMeta(EError::None, "错误 !!1 AddParseFunctionNodeInfo");
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
        Log::AddInStructFileMeta(EError::None, "错误 !!1 AddParseFunctionNodeInfo");
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
        Log::AddInStructFileMeta(EError::None, "错误 !!1 AddParseFunctionNodeInfo");
        return;
    }

    if (isAddParseCurrentNNode) {
        auto pcni = new ParseCurrentNodeInfo(fms);
        m_CurrentNodeInfoStack.push(pcni);
    }
}

void StructParse::ParseRootNodeToFileMeta() {
    AddParseFileNodeInfo();

    Node* pnode = &m_RootNode;
    while (true) {
        if (CheckEnd(pnode)) {
            break;
        }
        auto* node = m_RootNode.childList[pnode->parseIndex];
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
                    ParseNamespaceOrTopClass(pnode);
                    break;
                default:
                    Log::AddInStructFileMeta(EError::None, 
                        "Error 解析时 在File头部目录中出现 : " + node->token->GetLexemeString());
                    break;
            }
        } else if (node->nodeType == ENodeType::IdentifierLink) {
            ParseNamespaceOrTopClass(pnode);
        } else {
            Log::AddInStructFileMeta(EError::None, 
                "Error 解析时 在File头部目录中出现2 : " + (node->token ? node->token->GetLexemeString() : "null"));
        }
    }

    auto fileCode = std::move(m_CurrentNodeInfoStack.top());
    m_CurrentNodeInfoStack.pop();

    if (fileCode->parseType == EParseNodeType::File) {
#ifdef DEBUG
        m_FileMeta.SetDeep(0);
#endif
        Log::AddProcess(EProcess::ParseNode, EError::None, 
            "解析Code文件结构文件成功!!! 下一步进行 解析Meta文件 \n 解析FileMeta文件成功!!! 下一步进行 语法解析");
    } else {
        Log::AddProcess(EProcess::ParseNode, EError::ParseFileError, 
            "解析时出现错误 ParseFile : " + std::to_string(static_cast<int>(GetCurrentNodeInfo()->parseType)));
        return;
    }
}

std::vector<Node*> StructParse::GetAllNodeToSemiColon(Node* pnode, bool isAddSelf) {
    Node* curNode = pnode->GetParseCurrent();

    std::vector<Node*> conNode;
    if (isAddSelf) {
        conNode.push_back( new Node(*curNode));
    }
    if (curNode->nodeType == ENodeType::SemiColon) {
        pnode->parseIndex++;
        return conNode;
    }

    Node* node = pnode->GetParseNode();
    conNode.push_back( new Node(*node));
    bool isEnd = false;
    while (pnode->parseIndex < static_cast<int>(pnode->childList.size())) {
        Node* nextNode = pnode->GetParseNode();
        if (nextNode == nullptr) {
            break;
        }
        if (ProjectManager::isUseForceSemiColonInLineEnd ) {
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
            conNode.push_back( new Node(*nextNode));
        }
    }
    return conNode;
}

void StructParse::ParseImport(Node* pnode) {
    auto nodeList = GetAllNodeToSemiColon(pnode);
    auto ist = std::make_unique<FileMetaImportSyntax>(nodeList);
    m_FileMeta.AddFileImportSyntax(ist.get());
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
                    "Error 在解析namespace 中，左边赋值不能有多个标识符语法!!");
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
            if (ProjectManager::isUseForceSemiColonInLineEnd ) {
                Log::AddInStructFileMeta(EError::None, 
                    "Error 在解析namespace 中，需要强制;号结束");
                break;
            } else {
                break;
            }
        } else if (nextNode->nodeType == ENodeType::SemiColon) {
            break;
        }
    }
    
    auto fmn = new FileMetaNamespace(currentNode, namespaceNode);

    if (isBlock) { // 是否使用 namespace N{}的格式 还是使用{}格式，这里为块级模式
        m_FileMeta.AddFileDefineNamespace(fmn);
        AddParseNamespaceNodeInfo(fmn);
        ParseNamespaceOrTopClass(currentNode->blockNode);
        m_CurrentNodeInfoStack.pop();
    } else {
        m_FileMeta.AddFileSearchNamespace(fmn);
    }
}

bool StructParse::CheckEnd(Node* pnode) {
    return pnode->parseIndex >= static_cast<int>(pnode->childList.size());
}

// 只解析 全局文件下的 namespace 和 全局文件class
void StructParse::ParseNamespaceOrTopClass(Node* pnode) {
    Node* braceNode = pnode->blockNode;
    std::vector<Node> nodeList;
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
            nodeList.push_back(*newnode);
        } else if (curNode->nodeType == ENodeType::LeftAngle) { // Class1<T> 
            nodeList.push_back(*newnode);
        } else if (curNode->nodeType == ENodeType::RightAngle) { // Class1<T>   Func<T>( T t );  array<int> arr1;
            nodeList.push_back(*newnode);
        } else if (curNode->nodeType == ENodeType::Comma) {
            nodeList.push_back(*newnode);
        } else if (curNode->nodeType == ENodeType::IdentifierLink) { // Class1
            nodeList.push_back(*newnode);
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
                "Error 解析时在解析Class时出现错误 语法--------------------" + 
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
                auto fmn = new FileMetaNamespace( &nodeList[0], &nodeList[1]);
                AddParseNamespaceNodeInfo(fmn);
                if (nodeList[1].blockNode != nullptr) {
                    m_FileMeta.AddFileDefineNamespace(fmn);
                    ParseNamespaceOrTopClass(nodeList[1].blockNode);
                } else {
                    m_FileMeta.AddFileSearchNamespace(fmn);
                }
                m_CurrentNodeInfoStack.pop();
                ParseNamespaceOrTopClass(pnode);
            } else {
                Log::AddInStructFileMeta(EError::None, 
                    "Error 解析 namespace A.B{}的格式 只有一个标识符!1");
            }
        } else {
            Log::AddInStructFileMeta(EError::None, 
                "Error 没有发现Class或者Namespace的关键字!");
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
                "Error 解析时 在Class内部出现不支持的语法 : " + (node->token ? node->token->GetLexemeString() : "null"));
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
                    "Error 在Data的Bracket中出现不支持的关键字 : " + node->token->GetLexemeString());
                bracketNode->parseIndex++;
            }
        } else if (node->nodeType == ENodeType::IdentifierLink) {
            auto nodeList = GetAllNodeToSemiColon(bracketNode);
            auto cfmmd = new FileMetaMemberData(&m_FileMeta, nodeList);
            AddParseDataInfo(cfmmd);
            m_CurrentNodeInfoStack.pop();
        } else {
            Log::AddInStructFileMeta(EError::None, 
                "Error 在Data的Bracket中出现不支持的语法");
            bracketNode->parseIndex++;
        }
    }
}

void StructParse::ParseDataNode(Node* pnode) {
    auto nodeList = GetAllNodeToSemiColon(pnode);
    auto fmmd = new FileMetaMemberData(&m_FileMeta, nodeList);
    AddParseDataInfo(fmmd);
    
    for (size_t i = 0; i < fmmd->GetFileMetaMemberData().size(); i++) {
        auto cfmmd = fmmd->GetFileMetaMemberData()[i];
        if (cfmmd->GetDataType() == FileMetaMemberData::EMemberDataType::Data) {
            ParseDataBracketNode(cfmmd->GetBracketNode());
        }
    }
    m_CurrentNodeInfoStack.pop();
}

void StructParse::ParseEnumNode(Node* pnode) {
    auto nodeList = GetAllNodeToSemiColon(pnode);
    auto fmmv = new FileMetaMemberVariable(&m_FileMeta, nodeList);
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
    
    // 复制节点列表
    for (size_t i = 0; i < node->childList.size(); i++) {
        result.push_back(node->childList[i]);
    }
    
    // 处理表达式节点
    _HandleExpressNodeProcess(node);
    
    return result;
}

std::vector<Node*> StructParse::HandleExpressNode(Node* node) {
    std::vector<Node*> result;
    if (node == nullptr) {
        return result;
    }
    
    // 处理表达式节点
    _HandleExpressNodeProcess(node);
    
    // 返回处理后的节点列表
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
    
    // 清理不需要的节点
    for (size_t i = 0; i < node->childList.size(); i++) {
        Node* child = node->childList[i];
        if (child != nullptr) {
            // 递归清理子节点
            DelHandleNostList(child);
        }
    }
}

bool StructParse::IsCommonExpressNode(Node* node) {
    if (node == nullptr) {
        return false;
    }
    
    // 检查是否是常见的表达式节点类型
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
    
    // 处理表达式节点
    for (size_t i = 0; i < node->childList.size(); i++) {
        Node* child = node->childList[i];
        if (child != nullptr) {
            // 处理角度表达式节点
            if (child->nodeType == ENodeType::LeftAngle || child->nodeType == ENodeType::RightAngle) {
                HandleAngleExpressNode(child, node);
            }
            
            // 递归处理子节点
            _HandleExpressNodeProcess(child, inputFinaleNode);
        }
    }
}

void StructParse::HandleAngleExpressNode(Node* node, Node* parentNode) {
    if (node == nullptr || parentNode == nullptr) {
        return;
    }
    
    // 处理角度表达式节点（如模板参数 <T>）
    if (node->nodeType == ENodeType::LeftAngle) {
        // 处理左角度括号
        for (size_t i = 0; i < node->childList.size(); i++) {
            Node* child = node->childList[i];
            if (child != nullptr && child->nodeType == ENodeType::IdentifierLink) {
                // 处理模板参数
                // 这里可以添加模板参数的处理逻辑
            }
        }
    } else if (node->nodeType == ENodeType::RightAngle) {
        // 处理右角度括号
        // 这里可以添加模板参数结束的处理逻辑
    }
}

void StructParse::AddFileMetaFunctionVariable(Node* pnode, Node* blockNode, const std::vector<Node*>& nodeList) {
    if (nodeList.empty()) {
        return;
    }
    
    // 创建FileMetaMemberFunction对象
    auto fmmf = new FileMetaMemberFunction(&m_FileMeta, nodeList);
    
    // 添加到解析栈
    AddParseFunctionNodeInfo(fmmf, true);
    
    // 如果有块节点，解析函数体
    if (blockNode != nullptr) {
        ParseSyntax(blockNode);
    }
    
    // 从栈中弹出
    m_CurrentNodeInfoStack.pop();
}

void StructParse::AddFileMetaClasss(Node* blockNode, const std::vector<Node*>& nodeList) {
    auto fmc = new FileMetaClass(&m_FileMeta, nodeList);
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
    
    // 检查第一个节点的类型来决定创建什么类型的语句
    Node* firstNode = nodeList[0];
    if (firstNode->nodeType == ENodeType::Key && firstNode->token != nullptr) {
        switch (firstNode->token->GetType()) {
            case ETokenType::If:
                return new FileMetaIfStatements(&m_FileMeta, nodeList);
            case ETokenType::While:
            case ETokenType::DoWhile:
                return new FileMetaWhileDoWhileStatements(&m_FileMeta, nodeList);
            case ETokenType::Switch:
                return new FileMetaSwitchStatements(&m_FileMeta, nodeList);
            case ETokenType::Return:
                return new FileMetaReturnStatements(&m_FileMeta, nodeList);
            case ETokenType::Break:
            case ETokenType::Continue:
            case ETokenType::Goto:
                return new FileMetaBreakContinueGoStatements(&m_FileMeta, nodeList);
            default:
                // 其他关键字作为普通语句处理
                return new FileMetaCallStatements(&m_FileMeta, nodeList);
        }
    }
    
    // 默认作为调用语句处理
    return new FileMetaCallStatements(&m_FileMeta, nodeList);
}

} // namespace Compile
} // namespace SimpleLanguage
