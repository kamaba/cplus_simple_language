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

Node::Node(Token* _token) : token(_token ) {
}

Node* Node::GetParseCurrent() {
    if (parseIndex >= childList.size() ) {
        return nullptr;
    }
    return childList[parseIndex];
}

Node* Node::GetFinalNode() {
    if (!m_ExtendLinkNodeList.empty()) {
        return m_ExtendLinkNodeList.back();
    }
    return this;
}

std::vector<Node*> Node::GetLinkNodeList(bool isIncludeSelf) {
    std::vector<Node*> tlist;
    if (isIncludeSelf) {
        tlist.push_back(this);
    }
    for (auto& node : m_ExtendLinkNodeList) {
        tlist.push_back(node);
    }
    return tlist;
}

std::vector<Token*> Node::GetLinkTokenList() {
    std::vector<Token*> tlist;
    if (token) {
        tlist.push_back(token);
    }
    for (auto& node : m_ExtendLinkNodeList) {
        if (node->token) {
            tlist.push_back(node->token);
        }
    }
    return tlist;
}

Node* Node::GetParseNode(int index, bool isAddIndex) {
    if (parseIndex >= static_cast<int>(childList.size())) {
        return nullptr;
    }
    auto node = childList[parseIndex];
    if (isAddIndex) {
        parseIndex += index;
    }
    return node;
}

void Node::AddLinkNode(Node* node) {
    if (lastNode == nullptr) return;
    lastNode->m_ExtendLinkNodeList.push_back(node);
}

void Node::SetLinkNode(const std::vector<Node*>& nodeList) {
    m_ExtendLinkNodeList.clear();
    for (const auto& node : nodeList) {
        m_ExtendLinkNodeList.push_back(node);
    }
}

void Node::SetLinkNode(std::vector<Node*>&& nodeList) {
    m_ExtendLinkNodeList = nodeList;
}

void Node::AddChild(Node* c, bool setParent) {
    if (setParent) {
        c->parent = this;
    }
    this->childList.push_back(c);
    this->lastNode = c;
}

void Node::AddSyntax(Node* c) {
    this->childList.push_back(c);
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
        sb << (token != nullptr ? token->GetLexemeString() : "");
        for (const auto& node : m_ExtendLinkNodeList) {
            sb << (node->token != nullptr ? node->token->GetLexemeString() : "");
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
            sb << "\"" << token->GetLexemeString() << "\"";
        } else {
            sb << (token != nullptr ? token->GetLexemeString() : "");
        }
        for (const auto& node : m_ExtendLinkNodeList) {
            sb << (node->token != nullptr ? node->token->GetLexemeString() : "");
            if (node->parNode != nullptr) {
                sb << node->parNode->ToFormatString();
            }
        }
    } else if (nodeType == ENodeType::Brace) {
        sb << (token != nullptr ? token->GetLexemeString() : "");
        if (parNode != nullptr) {
            sb << parNode->ToFormatString();
        }
        for (const auto& child : childList) {
            sb << child->ToFormatString() << " ";
        }
        sb << (endToken != nullptr ? endToken->GetLexemeString() : "");
    } else if (nodeType == ENodeType::Bracket) {
        sb << (token != nullptr ? token->GetLexemeString() : "") << " ";
        for (const auto& child : childList) {
            sb << child->ToFormatString() << " ";
        }
        sb << (endToken != nullptr ? endToken->GetLexemeString() : "");
        for (const auto& node : m_ExtendLinkNodeList) {
            sb << (node->token != nullptr ? node->token->GetLexemeString() : "");
            if (node->parNode != nullptr) {
                sb << node->parNode->ToFormatString();
            }
        }
    } else if (nodeType == ENodeType::Par) {
        sb << (token != nullptr ? token->GetLexemeString() : "") << " ";
        for (const auto& child : childList) {
            sb << child->ToFormatString();
        }
        sb << (endToken != nullptr ? endToken->GetLexemeString() : "");
        for (const auto& node : m_ExtendLinkNodeList) {
            sb << (node->token != nullptr ? node->token->GetLexemeString() : "");
            if (node->parNode != nullptr) {
                sb << node->parNode->ToFormatString();
            }
        }
    } else if (nodeType == ENodeType::LeftAngle) {
        sb << (token != nullptr ? token->GetLexemeString() : "") << " ";
        for (const auto& child : childList) {
            sb << child->ToFormatString();
        }
        sb << (endToken != nullptr ? endToken->GetLexemeString() : "");
    } else if (nodeType == ENodeType::RightAngle) {
        sb << (token != nullptr ? token->GetLexemeString() : "") << " ";
        for (const auto& child : childList) {
            sb << child->ToFormatString();
        }
        sb << (endToken != nullptr ? endToken->GetLexemeString() : "");
    } else if (nodeType == ENodeType::Key) {
        sb << (token != nullptr ? token->GetLexemeString() : "");
        if (blockNode != nullptr) {
            sb << " " << blockNode->ToFormatString();
        }
        for (const auto& child : childList) {
            sb << child->ToFormatString();
        }
        if (!m_ExtendLinkNodeList.empty()) {
            for (const auto& node : m_ExtendLinkNodeList) {
                sb << (node->token != nullptr ? node->token->GetLexemeString() : "");
            }
        }
    } else if (nodeType == ENodeType::LineEnd) {
        // 不添加任何内容
    } else {
        sb << (token != nullptr ? token->GetLexemeString() : "") << " ";
        for (const auto& child : childList) {
            sb << child->ToFormatString();
        }
    }
    sb << " ";

    return sb.str();
}

} // namespace Compile
} // namespace SimpleLanguage
