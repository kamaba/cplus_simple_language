//****************************************************************************
//  File:      TokenParse.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "TokenParse.h"
#include "../FileMeta/FileMeta.h"
#include <iostream>
#include <sstream>

namespace SimpleLanguage {
namespace Compile {
namespace Parse {

TokenParse::TokenParse(FileMeta& fm, const std::vector<std::unique_ptr<Token>>& list)
    : m_FileMeta(fm), m_TokensList(list), m_TokenCount(static_cast<int>(list.size())) {
    m_RootNode = std::make_unique<Node>(nullptr);
    m_RootNode->nodeType = ENodeType::Root;
    currentNode = m_RootNode.get();
    currentNodeStack.push(m_RootNode.get());
}

void TokenParse::BuildStruct() {
    ParseToken();
}

void TokenParse::BuildEnd() {
    std::cout << "---------------File:" << m_FileMeta.GetPath() << "  Token节点  开始:-------------------------" << std::endl;
    std::cout << m_RootNode->ToFormatString() << std::endl;
    std::cout << "---------------File:" << m_FileMeta.GetPath() << "  Token节点  结束:-------------------------" << std::endl;
}

void TokenParse::AddImportNode(Token& token) {
    auto nnode = std::make_unique<Node>(&token);
    nnode->nodeType = ENodeType::Key;
    m_TokenIndex++;
    currentNode->AddChild(std::move(nnode));
}

void TokenParse::AddNamespaceNode(Token& token) {
    auto nnode = std::make_unique<Node>(&token);
    nnode->nodeType = ENodeType::Key;
    m_TokenIndex++;
    currentNode->AddChild(std::move(nnode));
}

void TokenParse::AddIdentifier(Token& code) { // Print/Function
    auto node = std::make_unique<Node>(&code);
    node->nodeType = ENodeType::IdentifierLink;

    if (currentNode->linkToken != nullptr) {
        auto node2 = std::make_unique<Node>(currentNode->linkToken.get());
        node2->nodeType = ENodeType::Period;

        currentNode->AddLinkNode(std::move(node2));
        currentNode->AddLinkNode(std::move(node));
        if (currentNode->atToken != nullptr) {
            node->atToken = std::move(currentNode->atToken);
            currentNode->atToken = nullptr;
        }
        currentNode->linkToken = nullptr;
    } else {
        currentNode->AddChild(std::move(node));
    }
    m_TokenIndex++;
}

void TokenParse::AddAnnotation(Token& code) {
    m_TokenIndex++;

    auto ntoken = std::make_unique<Token>(code);
    ntoken->SetType(ETokenType::LineEnd);
    auto node = std::make_unique<Node>(std::move(ntoken));
    node->nodeType = ENodeType::LineEnd;

    currentNode->AddChild(std::move(node));
}

std::unique_ptr<Node> TokenParse::AddKeyNode(Token& token) {
    auto node = std::make_unique<Node>(&token);
    node->nodeType = ENodeType::Key;
    currentNode->AddChild(std::move(node));
    m_TokenIndex++;
    return std::make_unique<Node>(&token);
}

std::unique_ptr<Node> TokenParse::AddAtOpSign(Token& token) {
    if (currentNode->linkToken != nullptr) {
        currentNode->atToken = std::make_unique<Token>(token);
    } else {
        std::cout << "现在@符必须使用.@方式!!" << std::endl;
    }
    m_TokenIndex++;
    return nullptr;
}

std::unique_ptr<Node> TokenParse::AddSymbol(Token& token) {
    auto node = std::make_unique<Node>(&token);
    node->nodeType = ENodeType::Symbol;
    currentNode->AddChild(std::move(node));
    m_TokenIndex++;
    return std::make_unique<Node>(&token);
}

void TokenParse::AddPlusMinus(Token& code) {
    auto node = AddSymbol(code);
    node->priority = SignComputePriority::Level2_LinkOp;
}

void TokenParse::AddDoublePlusMinus(Token& code) {
    auto node = AddSymbol(code);
    node->priority = SignComputePriority::Level2_LinkOp;
}

void TokenParse::AddLeftToRightEqualSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->priority = SignComputePriority::Level11_Assign;
}

void TokenParse::AddBitMoveOperatorSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->priority = SignComputePriority::Level5_BitMoveOp;
}

void TokenParse::AddDXCompareSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->priority = SignComputePriority::Level6_Compare;
}

void TokenParse::AddCompareNotOrEqualSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->priority = SignComputePriority::Level7_EqualAb;
}

void TokenParse::AddBitAndSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->priority = SignComputePriority::Level8_BitAndOp;
}

void TokenParse::AddBitXOrOpSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->priority = SignComputePriority::Level8_BitXOrOp;
}

void TokenParse::AddBitOrOpSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->priority = SignComputePriority::Level8_BitOrOp;
}

void TokenParse::AddHightComputeSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->priority = SignComputePriority::Level3_Hight_Compute;
}

void TokenParse::AddSingleSign(Token& code) {
    auto node = AddSymbol(code);
    node->priority = SignComputePriority::Level2_LinkOp;
}

void TokenParse::AddAndCompareSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->priority = SignComputePriority::Level9_And;
}

void TokenParse::AddOrCompareSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->priority = SignComputePriority::Level9_Or;
}

void TokenParse::ParseToken() {
    while (true) {
        auto& tempToken = *m_TokensList[m_TokenIndex];
        if (tempToken.GetType() == ETokenType::Finished) { 
            break; 
        }
        ParseDetailToken(tempToken);
    }
}

void TokenParse::ParseDetailToken(Token& token) {
    if (currentNode == nullptr) {
        std::cout << "Error CurrentNode is NULL!!" << token.ToLexemeAllString() << std::endl;
        return;
    }
    
    switch (token.GetType()) {
        case ETokenType::Identifier: // Identifier
            AddIdentifier(token);
            break;
        case ETokenType::Type:
            AddIdentifier(token);
            break;
        case ETokenType::LeftBrace: // {
            {
                auto node = std::make_unique<Node>(&token);
                node->nodeType = ENodeType::Brace;
                m_TokenIndex++;
                currentNodeStack.push(node.get());
                
                currentNode->AddChild(std::move(node));
                currentNode = currentNodeStack.top();
            }
            break;
        case ETokenType::RightBrace: // }
            {
                auto cnode = currentNodeStack.top();
                currentNodeStack.pop();

                if (cnode->nodeType == ENodeType::Brace) {
                    cnode->endToken = std::make_unique<Token>(token);
                    m_TokenIndex++;
                    currentNode = cnode->parent;
                } else {
                    std::cout << "Error 不对称{}" << std::endl;
                }
            }
            break;
        case ETokenType::Less: // <
            {
                auto node = std::make_unique<Node>(&token);
                node->nodeType = ENodeType::LeftAngle;
                m_TokenIndex++;
                currentNode->AddChild(std::move(node));
            }
            break;
        case ETokenType::Greater: // >
            {
                auto node = std::make_unique<Node>(&token);
                node->nodeType = ENodeType::RightAngle;
                currentNode->AddChild(std::move(node));
                m_TokenIndex++;
            }
            break;
        case ETokenType::LeftPar: // (
            {
                auto node = std::make_unique<Node>(&token);
                node->nodeType = ENodeType::Par;
                m_TokenIndex++;
                currentNodeStack.push(node.get());

                currentNode->AddChild(std::move(node));
                currentNode = currentNodeStack.top();
            }
            break;
        case ETokenType::RightPar: // )
            {
                auto cnode = currentNodeStack.top();
                currentNodeStack.pop();
                if (cnode != nullptr && cnode->nodeType == ENodeType::Par) {
                    cnode->endToken = std::make_unique<Token>(token);
                    m_TokenIndex++;
                    currentNode = cnode->parent;
                } else {
                    std::cout << "Error 不对称()" << std::endl;
                }
            }
            break;
        case ETokenType::LeftBracket: // [
            {
                auto node = std::make_unique<Node>(&token);
                node->nodeType = ENodeType::Bracket;
                m_TokenIndex++;
                currentNodeStack.push(node.get());

                currentNode->AddChild(std::move(node));
                currentNode = currentNodeStack.top();
            }
            break;
        case ETokenType::RightBracket: // ]
            {
                auto cnode = currentNodeStack.top();
                currentNodeStack.pop();
                if (cnode != nullptr && cnode->nodeType == ENodeType::Bracket) {
                    cnode->endToken = std::make_unique<Token>(token);
                    m_TokenIndex++;
                    currentNode = cnode->parent;
                } else {
                    std::cout << "Error 不对称[]" << std::endl;
                }
            }
            break;
        case ETokenType::Period: // .
            {
                currentNode->linkToken = std::make_unique<Token>(token);
                m_TokenIndex++;
            }
            break;
        case ETokenType::Comma: // ,
            {
                auto node = std::make_unique<Node>(&token);
                node->nodeType = ENodeType::Comma;
                currentNode->AddChild(std::move(node));
                m_TokenIndex++;
            }
            break;
        case ETokenType::QuestionMark: // ?
            {
                AddKeyNode(token);
            }
            break;
        case ETokenType::Colon: // :
            {
                AddKeyNode(token);
            }
            break;
        case ETokenType::SemiColon: // ;
            {
                auto node = std::make_unique<Node>(&token);
                node->nodeType = ENodeType::SemiColon;
                currentNode->AddChild(std::move(node));
                m_TokenIndex++;
            }
            break;
        case ETokenType::LineEnd: // \n
            {
                auto node = std::make_unique<Node>(&token);
                node->nodeType = ENodeType::LineEnd;
                currentNode->AddChild(std::move(node));
                m_TokenIndex++;
            }
            break;
        case ETokenType::Assign: // =
            {
                auto node = std::make_unique<Node>(&token);
                node->nodeType = ENodeType::Assign;
                currentNode->AddChild(std::move(node));
                m_TokenIndex++;
            }
            break;
        case ETokenType::Plus: // +
        case ETokenType::Minus: // -
            {
                AddPlusMinus(token);
            }
            break;
        case ETokenType::DoublePlus: // ++
        case ETokenType::DoubleMinus: // --
            {
                AddDoublePlusMinus(token);
            }
            break;
        case ETokenType::Multiply: // *
        case ETokenType::Divide: // /
        case ETokenType::Modulo: // %
            {
                AddHightComputeSymbol(token);
            }
            break;
        case ETokenType::Not: // !
        case ETokenType::Negative: // ~
            {
                AddSingleSign(token);
            }
            break;
        case ETokenType::Shi: // <<
            {
                AddBitMoveOperatorSymbol(token);
            }
            break;
        case ETokenType::Shr: // >>
            {
                AddBitMoveOperatorSymbol(token);
            }
            break;
        case ETokenType::GreaterOrEqual: // >=
        case ETokenType::LessOrEqual: // <=
            {
                AddDXCompareSymbol(token);
            }
            break;
        case ETokenType::Equal: // ==
        case ETokenType::NotEqual: // !=
            {
                AddCompareNotOrEqualSymbol(token);
            }
            break;
        case ETokenType::Combine: // &
            {
                AddBitAndSymbol(token);
            }
            break;
        case ETokenType::InclusiveOr: // |
            {
                AddBitOrOpSymbol(token);
            }
            break;
        case ETokenType::XOR: // ^
            {
                AddBitXOrOpSymbol(token);
            }
            break;
        case ETokenType::Or: // ||
            {
                AddOrCompareSymbol(token);
            }
            break;
        case ETokenType::And: // &&  
            {
                AddAndCompareSymbol(token);
            }
            break;
        case ETokenType::PlusAssign: // +=
        case ETokenType::MinusAssign: // -=
        case ETokenType::MultiplyAssign: // *=
        case ETokenType::DivideAssign: // /=
        case ETokenType::ModuloAssign: // %=
        case ETokenType::InclusiveOrAssign: // |=
        case ETokenType::XORAssign: // ^=                
            {
                AddLeftToRightEqualSymbol(token);
            }
            break;
        case ETokenType::Sharp: // #
            {
                AddAnnotation(token);
            }
            break;
        case ETokenType::Number:
        case ETokenType::String:
        case ETokenType::BoolValue:
        case ETokenType::NumberArrayLink:
        case ETokenType::Null:
            {
                auto node = std::make_unique<Node>(&token);
                node->nodeType = ENodeType::ConstValue;
                if (currentNode->linkToken != nullptr) {
                    auto node2 = std::make_unique<Node>(currentNode->linkToken.get());
                    node2->nodeType = ENodeType::Period;

                    currentNode->AddLinkNode(std::move(node2));
                    currentNode->AddLinkNode(std::move(node));
                    currentNode->linkToken = nullptr;
                    if (currentNode->atToken != nullptr) {
                        node->atToken = std::move(currentNode->atToken);
                        currentNode->atToken = nullptr;
                    }
                } else {
                    currentNode->AddChild(std::move(node));
                }
                m_TokenIndex++;
            }
            break;

        case ETokenType::Import:
            {
                AddImportNode(token);
            }
            break;
        case ETokenType::As:
            {
                AddKeyNode(token);
            }
            break;
        case ETokenType::Namespace:
            {
                AddNamespaceNode(token);
            }
            break;
        case ETokenType::Enum:
        case ETokenType::Data:
        case ETokenType::Class:
        case ETokenType::Dynamic:
            {
                AddKeyNode(token);
            }
            break;
        case ETokenType::Extern:
        case ETokenType::Public:
        case ETokenType::Projected:
        case ETokenType::Private:
            {
                AddKeyNode(token);
            }
            break;
        case ETokenType::Base: // base
        case ETokenType::This: // this
        case ETokenType::Object:
        case ETokenType::Boolean:
        case ETokenType::Global:
        case ETokenType::Range:
        case ETokenType::Const:
        case ETokenType::Mut:
        case ETokenType::Final:
        case ETokenType::Static:
        case ETokenType::Override:
        case ETokenType::Partial:
        case ETokenType::Void:
        case ETokenType::Get:
        case ETokenType::Set:
        case ETokenType::Interface:
        case ETokenType::Extends:
        case ETokenType::If:
        case ETokenType::ElseIf:
        case ETokenType::For:
        case ETokenType::While:
        case ETokenType::DoWhile:
        case ETokenType::Case:
        case ETokenType::Return:
        case ETokenType::Goto:
        case ETokenType::Transience:
        case ETokenType::Label:
        case ETokenType::Else:
        case ETokenType::In:
        case ETokenType::Switch:
        case ETokenType::Continue:
        case ETokenType::Break:
        case ETokenType::Default:
        case ETokenType::Var:
        case ETokenType::Next:
        case ETokenType::Params:
        case ETokenType::New:
            {
                AddKeyNode(token);
            }
            break;
        case ETokenType::At: // @
            {
                AddAtOpSign(token);
            }
            break;
        case ETokenType::Dollar:
            {
                AddAtOpSign(token);
            }
            break;
        case ETokenType::Space:
            {
                m_TokenIndex++;
            }
            break;
        default:
            {
                std::cout << "Line:" << token.GetSourceBeginLine() << " Source: " << token.GetSourceBeginChar() << std::endl;
                throw std::exception("不支持的语法");
            }
    }
}

} // namespace Parse
} // namespace Compile
} // namespace SimpleLanguage
