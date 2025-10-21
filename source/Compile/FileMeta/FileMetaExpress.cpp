#include "FileMetaExpress.h"
#include "FileMeta.h"
#include "FileMetatUtil.h"
#include "../../Debug/Log.h"
#include "../../Define.h"
#include <algorithm>
#include <limits>

namespace SimpleLanguage {
namespace Compile {

// FileMetaBaseTerm implementation
FileMetaBaseTerm::FileMetaBaseTerm(FileMeta* fm) 
{
    m_FileMeta = fm;
}

FileMetaBaseTerm* FileMetaBaseTerm::Root() const {
    const FileMetaBaseTerm* current = this;
    while (current->GetLeft() != nullptr) {
        current = current->GetLeft();
    }
    return const_cast<FileMetaBaseTerm*>(current);
}

std::string FileMetaBaseTerm::ToFormatString() const {
    return "";
}

// FileMetaSymbolTerm implementation
FileMetaSymbolTerm::FileMetaSymbolTerm(FileMeta* fm, Token* token) : FileMetaBaseTerm(fm), m_SymbolToken(token) {
    if (token != nullptr) {
        m_Priority = SimpleLanguage::SignComputePriority::GetPriority(token->type());
    }
}

void FileMetaSymbolTerm::BuildAST() {
    // Implementation for building AST for symbol term
}

std::string FileMetaSymbolTerm::ToFormatString() const {
    return m_SymbolToken ? m_SymbolToken->GetLexemeString() : "";
}

// FileMetaConstValueTerm implementation
FileMetaConstValueTerm::FileMetaConstValueTerm(FileMeta* fm, Token* token) 
    : FileMetaBaseTerm(fm), m_ValueToken(token) {
    m_Priority = SimpleLanguage::SignComputePriority::Level1;
}

FileMetaConstValueTerm::FileMetaConstValueTerm(FileMeta* fm, Token* valueToken, Token* signalToken)
    : FileMetaBaseTerm(fm), m_ValueToken(valueToken), m_SignalToken(signalToken) {
    m_Priority = SimpleLanguage::SignComputePriority::Level1;
}

void FileMetaConstValueTerm::BuildAST() {
    // Implementation for building AST for const value term
}

std::string FileMetaConstValueTerm::ToFormatString() const {
    std::ostringstream sb;
    if (m_SignalToken != nullptr) {
        sb << m_SignalToken->GetLexemeString();
    }
    if (m_ValueToken != nullptr) {
        sb << m_ValueToken->GetLexemeString();
    }
    return sb.str();
}

// FileMetaCallTerm implementation
FileMetaCallTerm::FileMetaCallTerm(FileMeta* fm, Node* node) : FileMetaBaseTerm(fm), m_CallNode(node) {
    m_Priority = SimpleLanguage::Core::SignComputePriority::Level1;
}

void FileMetaCallTerm::BuildAST() {
    // Implementation for building AST for call term
}

std::string FileMetaCallTerm::ToFormatString() const {
    if (m_CallNode == nullptr) return "";
    
    std::ostringstream sb;
    if (m_CallNode->token != nullptr) {
        sb << m_CallNode->token->GetLexemeString();
    }
    
    if (m_CallNode->parNode != nullptr) {
        sb << "(";
        // Add parameter formatting
        sb << ")";
    }
    
    if (m_CallNode->bracketNode != nullptr) {
        sb << "[";
        // Add bracket content formatting
        sb << "]";
    }
    
    return sb.str();
}

// FileMetaParTerm implementation
FileMetaParTerm::FileMetaParTerm(FileMeta* fm, Node* node, FileMetaTermExpress::EExpressType expressType)
    : FileMetaBaseTerm(fm), m_ParNode(node), m_ExpressType(expressType) {
    m_Priority = SimpleLanguage::Core::SignComputePriority::Level1;
}

void FileMetaParTerm::BuildAST() {
    if (m_ParNode == nullptr) return;
    
    // Create expression from par node content
    std::vector<Node*> parContent;
    for (auto child : m_ParNode->childList()) {
        if (child->nodeType() != SimpleLanguage::Compile::Parse::ENodeType::LineEnd &&
            child->nodeType() != SimpleLanguage::Compile::Parse::ENodeType::SemiColon) {
            parContent.push_back(child);
        }
    }
    
    if (!parContent.empty()) {
        m_Express = SimpleLanguage::Compile::FileMetatUtil::CreateFileMetaExpress(m_FileMeta, parContent, m_ExpressType);
    }
}

std::string FileMetaParTerm::ToFormatString() const {
    std::ostringstream sb;
    sb << "(";
    if (m_Express != nullptr) {
        sb << m_Express->ToFormatString();
    }
    sb << ")";
    return sb.str();
}

// FileMetaBraceTerm implementation
FileMetaBraceTerm::FileMetaBraceTerm(FileMeta* fm, Node* node) : FileMetaBaseTerm(fm), m_BraceNode(node) {
    m_Priority = SimpleLanguage::SignComputePriority::Level1;
}

void FileMetaBraceTerm::BuildAST() {
    // Implementation for building AST for brace term
}

std::string FileMetaBraceTerm::ToFormatString() const {
    std::ostringstream sb;
    sb << "{";
    // Add brace content formatting
    sb << "}";
    return sb.str();
}

// FileMetaBracketTerm implementation
FileMetaBracketTerm::FileMetaBracketTerm(FileMeta* fm, Node* node, int bracketType)
    : FileMetaBaseTerm(fm), m_BracketNode(node), m_BracketType(bracketType) {
    m_Priority = SimpleLanguage::SignComputePriority::Level1;
}

void FileMetaBracketTerm::BuildAST() {
    // Implementation for building AST for bracket term
}

std::string FileMetaBracketTerm::ToFormatString() const {
    std::ostringstream sb;
    sb << "[";
    // Add bracket content formatting
    sb << "]";
    return sb.str();
}

// FileMetaIfSyntaxTerm implementation
FileMetaIfSyntaxTerm::FileMetaIfSyntaxTerm(FileMeta* fm, Node* node) : FileMetaBaseTerm(fm), m_IfNode(node) {
    m_Priority = SimpleLanguage::SignComputePriority::Level1;
}

void FileMetaIfSyntaxTerm::BuildAST() {
    // Implementation for building AST for if syntax term
}

std::string FileMetaIfSyntaxTerm::ToFormatString() const {
    std::ostringstream sb;
    sb << "if (";
    // Add if condition formatting
    sb << ") {";
    // Add if body formatting
    sb << "}";
    return sb.str();
}

// FileMetaThreeItemSyntaxTerm implementation
FileMetaThreeItemSyntaxTerm::FileMetaThreeItemSyntaxTerm(FileMeta* fm, Node* node) 
    : FileMetaBaseTerm(fm), m_ThreeItemNode(node) {
    m_Priority = SimpleLanguage::SignComputePriority::Level1;
}

void FileMetaThreeItemSyntaxTerm::BuildAST() {
    // Implementation for building AST for three item syntax term
}

std::string FileMetaThreeItemSyntaxTerm::ToFormatString() const {
    std::ostringstream sb;
    sb << "? ";
    // Add three item formatting
    sb << " : ";
    return sb.str();
}

// FileMetaMatchSyntaxTerm implementation
FileMetaMatchSyntaxTerm::FileMetaMatchSyntaxTerm(FileMeta* fm, Node* node) : FileMetaBaseTerm(fm), m_MatchNode(node) {
    m_Priority = SimpleLanguage::SignComputePriority::Level1;
}

void FileMetaMatchSyntaxTerm::BuildAST() {
    // Implementation for building AST for match syntax term
}

std::string FileMetaMatchSyntaxTerm::ToFormatString() const {
    std::ostringstream sb;
    sb << "match (";
    // Add match expression formatting
    sb << ") {";
    // Add match cases formatting
    sb << "}";
    return sb.str();
}

// FileMetaTermExpress implementation
FileMetaTermExpress::FileMetaTermExpress(FileMeta* fm, const std::vector<Node*>& nodeList, EExpressType expressType)
    : FileMetaBaseTerm(fm), m_NodeList(nodeList), m_ExpressType(expressType) {
}

void FileMetaTermExpress::BuildAST() {
    if (m_NodeList.empty()) return;
    
    // Convert nodes to terms
    std::vector<FileMetaBaseTerm*> terms;
    for (auto node : m_NodeList) {
        auto term = SimpleLanguage::Compile::FileMetatUtil::CreateFileOneTerm(m_FileMeta, node, m_ExpressType);
        if (term != nullptr) {
            terms.push_back(term);
        }
    }
    
    if (terms.empty()) return;
    
    // Build AST from terms
    // This is a simplified implementation
    if (terms.size() == 1) {
        m_Left = terms[0];
    } else {
        // Build binary tree from terms
        for (size_t i = 0; i < terms.size() - 1; i++) {
            if (i == 0) {
                m_Left = terms[i];
            } else {
                auto current = m_Left;
                while (current->right() != nullptr) {
                    current = current->right();
                }
                current->setRight(terms[i]);
            }
        }
        m_Right = terms[terms.size() - 1];
    }
}

std::string FileMetaTermExpress::ToFormatString() const {
    std::ostringstream sb;
    if (m_Left != nullptr) {
        sb << m_Left->ToFormatString();
    }
    if (m_Right != nullptr) {
        sb << " " << m_Right->ToFormatString();
    }
    return sb.str();
}

} // namespace Compile
} // namespace SimpleLanguage
