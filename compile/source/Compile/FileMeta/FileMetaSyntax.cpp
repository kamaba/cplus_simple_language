#include "FileMetaSyntax.h"
#include "FileMeta.h"
#include "../../Debug/Log.h"
#include "../../Define.h"
#include <algorithm>

namespace SimpleLanguage {
namespace Compile {

// FileMetaSyntax implementation
FileMetaSyntax::FileMetaSyntax() {
}

bool FileMetaSyntax::IsNotEnd() const {
    return m_ParseIndex < static_cast<int>(m_FileMetaSyntax.size());
}

FileMetaSyntax* FileMetaSyntax::GetCurrentSyntaxAndMove(int moveIndex) {
    if (m_ParseIndex < static_cast<int>(m_FileMetaSyntax.size())) {
        FileMetaSyntax* fms = m_FileMetaSyntax[m_ParseIndex];
        m_ParseIndex += moveIndex;
        return fms;
    }
    return nullptr;
}

void FileMetaSyntax::AddFileMetaSyntax(FileMetaSyntax* fms) {
    m_FileMetaSyntax.push_back(fms);
}

std::string FileMetaSyntax::ToFormatString() const {
    return "";
}

// FileMetaBlockSyntax implementation
FileMetaBlockSyntax::FileMetaBlockSyntax(FileMeta* fm, Token* leftBraceToken, Token* rightBraceToken)
    :  m_LeftBraceToken(leftBraceToken), m_RightBraceToken(rightBraceToken) {
    m_FileMeta = fm;
}

std::string FileMetaBlockSyntax::ToFormatString() const {
    std::ostringstream sb;
    sb << "{";
    for (auto syntax : m_FileMetaSyntax) {
        sb << std::endl << syntax->ToFormatString();
    }
    sb << std::endl << "}";
    return sb.str();
}

// FileMetaAssignStatements implementation
FileMetaAssignStatements::FileMetaAssignStatements(FileMeta* fm, const std::vector<Node*>& nodeList)
    : m_NodeList(nodeList) {
    m_FileMeta = fm;
}

std::string FileMetaAssignStatements::ToFormatString() const {
    std::ostringstream sb;
    for (size_t i = 0; i < m_NodeList.size(); i++) {
        if (i > 0) sb << " ";
        //sb << (m_NodeList[i]->GetToken() != nullptr ? m_NodeList[i]->GetToken()->lexeme().ToString() : "");
    }
    if (IsAppendSemiColon()) {
        sb << ";";
    }
    return sb.str();
}

// FileMetaDefineVarStatements implementation
FileMetaDefineVarStatements::FileMetaDefineVarStatements(FileMeta* fm, const std::vector<Node*>& nodeList)
    :  m_NodeList(nodeList) {
    m_FileMeta = fm;
}

std::string FileMetaDefineVarStatements::ToFormatString() const {
    std::ostringstream sb;
    for (size_t i = 0; i < m_NodeList.size(); i++) {
        if (i > 0) sb << " ";
        //sb << (m_NodeList[i]->token() ? m_NodeList[i]->token()->lexeme().ToString() : "");
    }
    if (IsAppendSemiColon()) {
        sb << ";";
    }
    return sb.str();
}

// FileMetaCallStatements implementation
FileMetaCallStatements::FileMetaCallStatements(FileMeta* fm, const std::vector<Node*>& nodeList)
    : m_NodeList(nodeList) {
    m_FileMeta = fm;
}

std::string FileMetaCallStatements::ToFormatString() const {
    std::ostringstream sb;
    for (size_t i = 0; i < m_NodeList.size(); i++) {
        if (i > 0) sb << " ";
        //sb << (m_NodeList[i]->token() ? m_NodeList[i]->token()->lexeme().ToString() : "");
    }
    if (IsAppendSemiColon()) {
        sb << ";";
    }
    return sb.str();
}

// FileMetaReturnStatements implementation
FileMetaReturnStatements::FileMetaReturnStatements(FileMeta* fm, const std::vector<Node*>& nodeList)
    : m_NodeList(nodeList) {
    m_FileMeta = fm;
}

std::string FileMetaReturnStatements::ToFormatString() const {
    std::ostringstream sb;
    sb << "return";
    for (size_t i = 0; i < m_NodeList.size(); i++) {
        //sb << " " << (m_NodeList[i]->token() ? m_NodeList[i]->token()->lexeme().ToString() : "");
    }
    if (IsAppendSemiColon()) {
        sb << ";";
    }
    return sb.str();
}

// FileMetaIfStatements implementation
FileMetaIfStatements::FileMetaIfStatements(FileMeta* fm, const std::vector<Node*>& nodeList)
    : m_NodeList(nodeList) {
    m_FileMeta = fm;
}

std::string FileMetaIfStatements::ToFormatString() const {
    std::ostringstream sb;
    sb << "if (";
    for (size_t i = 0; i < m_NodeList.size(); i++) {
        if (i > 0) sb << " ";
        //sb << (m_NodeList[i]->token() ? m_NodeList[i]->token()->lexeme().ToString() : "");
    }
    sb << ") {";
    // Add if body formatting
    sb << "}";
    return sb.str();
}

// FileMetaWhileDoWhileStatements implementation
FileMetaWhileDoWhileStatements::FileMetaWhileDoWhileStatements(FileMeta* fm, const std::vector<Node*>& nodeList)
    : m_NodeList(nodeList) {
    m_FileMeta = fm;
}

std::string FileMetaWhileDoWhileStatements::ToFormatString() const {
    std::ostringstream sb;
    sb << "while (";
    for (size_t i = 0; i < m_NodeList.size(); i++) {
        if (i > 0) sb << " ";
        //sb << (m_NodeList[i]->token() ? m_NodeList[i]->token()->lexeme().ToString() : "");
    }
    sb << ") {";
    // Add while body formatting
    sb << "}";
    return sb.str();
}

// FileMetaSwitchStatements implementation
FileMetaSwitchStatements::FileMetaSwitchStatements(FileMeta* fm, const std::vector<Node*>& nodeList)
    : m_NodeList(nodeList) {
    m_FileMeta = fm;
}

std::string FileMetaSwitchStatements::ToFormatString() const {
    std::ostringstream sb;
    sb << "switch (";
    for (size_t i = 0; i < m_NodeList.size(); i++) {
        if (i > 0) sb << " ";
        //sb << (m_NodeList[i]->token() ? m_NodeList[i]->token()->lexeme().ToString() : "");
    }
    sb << ") {";
    // Add switch cases formatting
    sb << "}";
    return sb.str();
}

// FileMetaBreakContinueGoStatements implementation
FileMetaBreakContinueGoStatements::FileMetaBreakContinueGoStatements(FileMeta* fm, const std::vector<Node*>& nodeList)
    : m_NodeList(nodeList) {
    m_FileMeta = fm;
}

std::string FileMetaBreakContinueGoStatements::ToFormatString() const {
    std::ostringstream sb;
    for (size_t i = 0; i < m_NodeList.size(); i++) {
        if (i > 0) sb << " ";
        //sb << (m_NodeList[i]->token() ? m_NodeList[i]->token()->lexeme().ToString() : "");
    }
    if (IsAppendSemiColon()) {
        sb << ";";
    }
    return sb.str();
}

// FileMetaOtherPlatformStatement implementation
FileMetaOtherPlatformStatement::FileMetaOtherPlatformStatement(FileMeta* fm, const std::vector<Node*>& nodeList)
    :  m_NodeList(nodeList) {
    m_FileMeta = fm;
}

std::string FileMetaOtherPlatformStatement::ToFormatString() const {
    std::ostringstream sb;
    for (size_t i = 0; i < m_NodeList.size(); i++) {
        if (i > 0) sb << " ";
        //sb << (m_NodeList[i]->token() ? m_NodeList[i]->token()->lexeme().ToString() : "");
    }
    if (IsAppendSemiColon()) {
        sb << ";";
    }
    return sb.str();
}

} // namespace Compile
} // namespace SimpleLanguage
