//****************************************************************************
//  File:      TokenParse.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "TokenParse.h"
#include "../FileMeta/FileMeta.h"
#include "../Token.h"
#include "Node.h"
#include <iostream>
#include <sstream>

namespace SimpleLanguage {
namespace Compile {

TokenParse::TokenParse(FileMeta* fm, const std::vector<Token*>& list):m_TokensList(list)
{
    m_FileMeta = fm;
    m_TokenCount = list.size();
    m_RootNode = new Node( new Token() );
    m_RootNode->nodeType = ENodeType::Root;
    m_CurrentNode = m_RootNode;
    m_CurrentNodeStack.push(m_RootNode);
}

void TokenParse::BuildStruct() {
    ParseToken();
}

void TokenParse::BuildEnd() {
    std::cout << "---------------File:" << m_FileMeta->GetPath() << "  Token节点  开始:-------------------------" << std::endl;
    std::cout << m_RootNode->ToFormatString() << std::endl;
    std::cout << "---------------File:" << m_FileMeta->GetPath() << "  Token节点  结束:-------------------------" << std::endl;
}

void TokenParse::AddImportNode(Token& token) {
    auto nnode = new Node(&token);
    nnode->nodeType = ENodeType::Key;
    m_TokenIndex++;
    m_CurrentNode->AddChild(std::move(nnode));
}

void TokenParse::AddNamespaceNode(Token& token) {
    auto nnode = new Node(&token);
    nnode->nodeType = ENodeType::Key;
    m_TokenIndex++;
    m_CurrentNode->AddChild(nnode);
}

void TokenParse::AddIdentifier(Token& code) { // Print/Function
    auto node = new Node(&code);
    node->nodeType = ENodeType::IdentifierLink;

    if (m_CurrentNode->linkToken != nullptr) {
        auto node2 = new Node(m_CurrentNode->linkToken);
        node2->nodeType = ENodeType::Period;

        m_CurrentNode->AddLinkNode(node2);
        m_CurrentNode->AddLinkNode(node);
        if (m_CurrentNode->atToken != nullptr) {
            node->atToken = std::move(m_CurrentNode->atToken);
            m_CurrentNode->atToken = nullptr;
        }
        m_CurrentNode->linkToken = nullptr;
    } else {
        m_CurrentNode->AddChild(node);
    }
    m_TokenIndex++;
}

void TokenParse::AddAnnotation(Token& code) {
    m_TokenIndex++;

    auto ntoken = &code;
    ntoken->SetType(ETokenType::LineEnd);
    auto node = new Node(ntoken);
    node->nodeType = ENodeType::LineEnd;

    m_CurrentNode->AddChild(node);
}

Node* TokenParse::AddKeyNode(Token& token) {
    auto node = new Node(&token);
    node->nodeType = ENodeType::Key;
    m_CurrentNode->AddChild(node);
    m_TokenIndex++;
    return new Node(&token);
}

Node* TokenParse::AddAtOpSign(Token& token) {
    if (m_CurrentNode->linkToken != nullptr) {
        m_CurrentNode->atToken = new Token(token);
    } else {
        std::cout << "现在@符必须使用.@方式!!" << std::endl;
    }
    m_TokenIndex++;
    return nullptr;
}

Node* TokenParse::AddSymbol(Token& token) {
    auto node = new Node(&token);
    node->nodeType = ENodeType::Symbol;
    m_CurrentNode->AddChild(node);
    m_TokenIndex++;
    return new Node(&token);
}

void TokenParse::AddPlusMinus(Token& code) {
    auto node = AddSymbol(code);
    node->SetPriority( SignComputePriority::Level2_LinkOp );
}

void TokenParse::AddDoublePlusMinus(Token& code) {
    auto node = AddSymbol(code);
    node->SetPriority(SignComputePriority::Level2_LinkOp);
}

void TokenParse::AddLeftToRightEqualSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->SetPriority(SignComputePriority::Level11_Assign);
}

void TokenParse::AddBitMoveOperatorSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->SetPriority(SignComputePriority::Level5_BitMoveOp);
}

void TokenParse::AddDXCompareSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->SetPriority(SignComputePriority::Level6_Compare);
}

void TokenParse::AddCompareNotOrEqualSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->SetPriority(SignComputePriority::Level7_EqualAb);
}

void TokenParse::AddBitAndSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->SetPriority(SignComputePriority::Level8_BitAndOp);
}

void TokenParse::AddBitXOrOpSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->SetPriority(SignComputePriority::Level8_BitXOrOp);
}

void TokenParse::AddBitOrOpSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->SetPriority(SignComputePriority::Level8_BitOrOp);
}

void TokenParse::AddHightComputeSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->SetPriority(SignComputePriority::Level3_Hight_Compute);
}

void TokenParse::AddSingleSign(Token& code) {
    auto node = AddSymbol(code);
    node->SetPriority(SignComputePriority::Level2_LinkOp);
}

void TokenParse::AddAndCompareSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->SetPriority(SignComputePriority::Level9_And);
}

void TokenParse::AddOrCompareSymbol(Token& code) {
    auto node = AddSymbol(code);
    node->SetPriority(SignComputePriority::Level9_Or );
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
    if (m_CurrentNode == nullptr) {
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
                auto node = new Node(&token);
                node->nodeType = ENodeType::Brace;
                m_TokenIndex++;
                m_CurrentNodeStack.push(node);
                
                m_CurrentNode->AddChild(node);
                m_CurrentNode = m_CurrentNodeStack.top();
            }
            break;
        case ETokenType::RightBrace: // }
            {
                auto cnode = m_CurrentNodeStack.top();
                m_CurrentNodeStack.pop();

                if (cnode->nodeType == ENodeType::Brace) {
                    cnode->endToken = new Token(token);
                    m_TokenIndex++;
                    m_CurrentNode = cnode->parent;
                } else {
                    std::cout << "Error 不对称{}" << std::endl;
                }
            }
            break;
        case ETokenType::Less: // <
            {
                auto node = new Node(&token);
                node->nodeType = ENodeType::LeftAngle;
                m_TokenIndex++;
                m_CurrentNode->AddChild(node);
            }
            break;
        case ETokenType::Greater: // >
            {
                auto node = new Node(&token);
                node->nodeType = ENodeType::RightAngle;
                m_CurrentNode->AddChild(node);
                m_TokenIndex++;
            }
            break;
        case ETokenType::LeftPar: // (
            {
                auto node = new Node(&token);
                node->nodeType = ENodeType::Par;
                m_TokenIndex++;
                m_CurrentNodeStack.push(node);

                m_CurrentNode->AddChild(node);
                m_CurrentNode = m_CurrentNodeStack.top();
            }
            break;
        case ETokenType::RightPar: // )
            {
                auto cnode = m_CurrentNodeStack.top();
                m_CurrentNodeStack.pop();
                if (cnode != nullptr && cnode->nodeType == ENodeType::Par) {
                    cnode->endToken = new Token(token);
                    m_TokenIndex++;
                    m_CurrentNode = cnode->parent;
                } else {
                    std::cout << "Error 不对称()" << std::endl;
                }
            }
            break;
        case ETokenType::LeftBracket: // [
            {
                auto node = new Node(&token);
                node->nodeType = ENodeType::Bracket;
                m_TokenIndex++;
                m_CurrentNodeStack.push(node);

                m_CurrentNode->AddChild(node);
                m_CurrentNode = m_CurrentNodeStack.top();
            }
            break;
        case ETokenType::RightBracket: // ]
            {
                auto cnode = m_CurrentNodeStack.top();
                m_CurrentNodeStack.pop();
                if (cnode != nullptr && cnode->nodeType == ENodeType::Bracket) {
                    cnode->endToken = new Token(token);
                    m_TokenIndex++;
                    m_CurrentNode = cnode->parent;
                } else {
                    std::cout << "Error 不对称[]" << std::endl;
                }
            }
            break;
        case ETokenType::Period: // .
            {
                m_CurrentNode->linkToken = new Token(token);
                m_TokenIndex++;
            }
            break;
        case ETokenType::Comma: // ,
            {
                auto node = new Node(&token);
                node->nodeType = ENodeType::Comma;
                m_CurrentNode->AddChild(node);
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
                auto node = new Node(&token);
                node->nodeType = ENodeType::SemiColon;
                m_CurrentNode->AddChild(node);
                m_TokenIndex++;
            }
            break;
        case ETokenType::LineEnd: // \n
            {
                auto node = new Node(&token);
                node->nodeType = ENodeType::LineEnd;
                m_CurrentNode->AddChild(node);
                m_TokenIndex++;
            }
            break;
        case ETokenType::Assign: // =
            {
                auto node = new Node(&token);
                node->nodeType = ENodeType::Assign;
                m_CurrentNode->AddChild(node);
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
                auto node = new Node(&token);
                node->nodeType = ENodeType::ConstValue;
                if (m_CurrentNode->linkToken != nullptr) {
                    //auto node2 = currentNode->linkToken;
                    //node2->nodeType = ENodeType::Period;

                    //currentNode->AddLinkNode(node2);
                    m_CurrentNode->AddLinkNode(node);
                    m_CurrentNode->linkToken = nullptr;
                    if (m_CurrentNode->atToken != nullptr) {
                        node->atToken = m_CurrentNode->atToken;
                        m_CurrentNode->atToken = nullptr;
                    }
                } else {
                    m_CurrentNode->AddChild(node);
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

} // namespace Compile
} // namespace SimpleLanguage
