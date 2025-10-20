//****************************************************************************
//  File:      Node.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "Node.h"
#include <sstream>

namespace SimpleLanguage {
namespace Compile {
namespace Parse {

Node::Node(std::shared_ptr<Token> _token) 
{
    this->token = _token.get();
}

Node::Node(Token* _token) : token(_token ) {
}

Node* Node::GetParseCurrent() {
    if (parseIndex >= static_cast<int>(childList.size())) {
        return nullptr;
    }
    return childList[parseIndex].get();
}

Node* Node::GetFinalNode() {
    if (!m_ExtendLinkNodeList.empty()) {
        return m_ExtendLinkNodeList.back().get();
    }
    return this;
}

std::vector<std::shared_ptr<Node>> Node::GetLinkNodeList(bool isIncludeSelf) {
    std::vector<std::shared_ptr<Node>> tlist;
    if (isIncludeSelf) {
        tlist.push_back(std::make_unique<Node>(*this));
    }
    for (auto& node : m_ExtendLinkNodeList) {
        tlist.push_back(std::make_unique<Node>(*node));
    }
    return tlist;
}

std::vector<std::shared_ptr<Token>> Node::GetLinkTokenList() {
    std::vector<std::shared_ptr<Token>> tlist;
    if (token) {
        tlist.push_back(std::make_unique<Token>(*token));
    }
    for (auto& node : m_ExtendLinkNodeList) {
        if (node->token) {
            tlist.push_back(std::make_unique<Token>(*node->token));
        }
    }
    return tlist;
}

Node* Node::GetParseNode(int index, bool isAddIndex) {
    if (parseIndex >= static_cast<int>(childList.size())) {
        return nullptr;
    }
    auto* node = childList[parseIndex].get();
    if (isAddIndex) {
        parseIndex += index;
    }
    return node;
}

void Node::AddLinkNode(std::shared_ptr<Node> node) {
    if (lastNode == nullptr) return;
    lastNode->m_ExtendLinkNodeList.push_back(std::move(node));
}

void Node::SetLinkNode(const std::vector<std::shared_ptr<Node>>& nodeList) {
    m_ExtendLinkNodeList.clear();
    for (const auto& node : nodeList) {
        m_ExtendLinkNodeList.push_back(std::make_unique<Node>(*node));
    }
}

void Node::SetLinkNode(std::vector<std::shared_ptr<Node>>&& nodeList) {
    m_ExtendLinkNodeList = std::move(nodeList);
}

void Node::AddChild(std::shared_ptr<Node> c, bool setParent) {
    if (setParent) {
        c->parent = this;
    }
    this->childList.push_back(c);
    this->lastNode = c.get();
}

void Node::AddSyntax(std::shared_ptr<Node> c) {
    this->childList.push_back(std::move(c));
}

std::string Node::ToString() const {
    std::ostringstream sb;
    switch (nodeType) {
        case ENodeType::Root:
            sb << "root";
            break;
        case ENodeType::IdentifierLink:
            sb << (token != nullptr ? token->ToString() : "Node");
            break;
        case ENodeType::Brace:
            sb << "  {    }  ";
            break;
        case ENodeType::Bracket:
            sb << "  [   ]  ";
            break;
        case ENodeType::LeftAngle:
            sb << " < ";
            break;
        case ENodeType::RightAngle:
            sb << " > ";
            break;
        case ENodeType::Par:
            sb << "   (     )   ";
            break;
        case ENodeType::Key:
            sb << (token != nullptr ? token->ToString() : "Key");
            break;
        case ENodeType::LineEnd:
            sb << "换行\n";
            break;
        case ENodeType::SemiColon:
            sb << ";\n";
            break;
        default:
            sb << (token != nullptr ? token->ToString() : "Node");
            break;
    }
    return sb.str();
}

::std::string Node::ToFormatString() const {
    ::std::ostringstream sb;

    if (nodeType == ENodeType::IdentifierLink) {
        sb << (token != nullptr ? token->GetLexeme() : "");
        for (const auto& node : m_ExtendLinkNodeList) {
            sb << (node->token != nullptr ? node->token->GetLexeme() : "");
            if (node->parNode != nullptr) {
                sb << node->parNode->ToFormatString();
            }
        }
        if (parNode != nullptr) {
            sb << parNode->ToFormatString();
        }
        if (blockNode != nullptr) {
            sb << " " << blockNode->ToFormatString();
        }
    } else if (nodeType == ENodeType::ConstValue) {
        if (token != nullptr && token->GetType() == ETokenType::String) {
            sb << "\"" << token->GetLexeme() << "\"";
        } else {
            sb << (token != nullptr ? token->GetLexeme() : "");
        }
        for (const auto& node : m_ExtendLinkNodeList) {
            sb << (node->token != nullptr ? node->token->GetLexeme() : "");
            if (node->parNode != nullptr) {
                sb << node->parNode->ToFormatString();
            }
        }
    } else if (nodeType == ENodeType::Brace) {
        sb << (token != nullptr ? token->GetLexeme() : "");
        if (parNode != nullptr) {
            sb << parNode->ToFormatString();
        }
        for (const auto& child : childList) {
            sb << child->ToFormatString() << " ";
        }
        sb << (endToken != nullptr ? endToken->GetLexeme() : "");
    } else if (nodeType == ENodeType::Bracket) {
        sb << (token != nullptr ? token->GetLexeme() : "") << " ";
        for (const auto& child : childList) {
            sb << child->ToFormatString() << " ";
        }
        sb << (endToken != nullptr ? endToken->GetLexeme() : "");
        for (const auto& node : m_ExtendLinkNodeList) {
            sb << (node->token != nullptr ? node->token->GetLexeme() : "");
            if (node->parNode != nullptr) {
                sb << node->parNode->ToFormatString();
            }
        }
    } else if (nodeType == ENodeType::Par) {
        sb << (token != nullptr ? token->GetLexeme() : "") << " ";
        for (const auto& child : childList) {
            sb << child->ToFormatString();
        }
        sb << (endToken != nullptr ? endToken->GetLexeme() : "");
        for (const auto& node : m_ExtendLinkNodeList) {
            sb << (node->token != nullptr ? node->token->GetLexeme() : "");
            if (node->parNode != nullptr) {
                sb << node->parNode->ToFormatString();
            }
        }
    } else if (nodeType == ENodeType::LeftAngle) {
        sb << (token != nullptr ? token->GetLexeme() : "") << " ";
        for (const auto& child : childList) {
            sb << child->ToFormatString();
        }
        sb << (endToken != nullptr ? endToken->GetLexeme() : "");
    } else if (nodeType == ENodeType::RightAngle) {
        sb << (token != nullptr ? token->GetLexeme() : "") << " ";
        for (const auto& child : childList) {
            sb << child->ToFormatString();
        }
        sb << (endToken != nullptr ? endToken->GetLexeme() : "");
    } else if (nodeType == ENodeType::Key) {
        sb << (token != nullptr ? token->GetLexeme() : "");
        if (blockNode != nullptr) {
            sb << " " << blockNode->ToFormatString();
        }
        for (const auto& child : childList) {
            sb << child->ToFormatString();
        }
        if (!m_ExtendLinkNodeList.empty()) {
            for (const auto& node : m_ExtendLinkNodeList) {
                sb << (node->token != nullptr ? node->token->GetLexeme() : "");
            }
        }
    } else if (nodeType == ENodeType::LineEnd) {
        // 不添加任何内容
    } else {
        sb << (token != nullptr ? token->GetLexeme() : "") << " ";
        for (const auto& child : childList) {
            sb << child->ToFormatString();
        }
    }
    sb << " ";

    return sb.str();
}

} // namespace Parse
} // namespace Compile
} // namespace SimpleLanguage
