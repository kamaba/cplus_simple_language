#pragma once

#include "FileMetaBase.h"
#include "../Parse/Node.h"
#include "../../Define.h"
#include "../../Debug/Log.h"
#include <vector>
#include <string>
#include <sstream>

namespace SimpleLanguage {
namespace Compile {

// Forward declarations
class FileMeta;
class FileMetaBlockSyntax;
class FileMetaAssignStatements;
class FileMetaDefineVarStatements;
class FileMetaCallStatements;
class FileMetaReturnStatements;
class FileMetaIfStatements;
class FileMetaWhileDoWhileStatements;
class FileMetaSwitchStatements;
class FileMetaBreakContinueGoStatements;
class FileMetaOtherPlatformStatement;
class Node;

class FileMetaSyntax : public FileMetaBase {
public:
    FileMetaSyntax();
    virtual ~FileMetaSyntax() = default;

    // Properties
    int GetParseIndex() const { return m_ParseIndex; }
    void SetParseIndex(int value) { m_ParseIndex = value; }
    
    bool IsAppendSemiColon() const { return m_IsAppendSemiColon; }
    void SetAppendSemiColon(bool value) { m_IsAppendSemiColon = value; }
    
    const ::std::vector<FileMetaSyntax*>& GetFileMetaSyntax() const { return m_FileMetaSyntax; }

    // Methods
    bool IsNotEnd() const;
    FileMetaSyntax* GetCurrentSyntaxAndMove(int moveIndex = 1);
    void AddFileMetaSyntax(FileMetaSyntax* fms);
    virtual ::std::string ToFormatString() const override;

protected:
    ::std::vector<FileMetaSyntax*> m_FileMetaSyntax;
    ::std::vector<Node*> m_NodeList;
    int m_ParseIndex = 0;
    bool m_IsAppendSemiColon = true;
};

class FileMetaBlockSyntax : public FileMetaSyntax {
public:
    FileMetaBlockSyntax(FileMeta* fm, Token* leftBraceToken, Token* rightBraceToken);
    virtual ~FileMetaBlockSyntax() = default;

    Token* GetLeftBraceToken() const { return m_LeftBraceToken; }
    Token* GetRightBraceToken() const { return m_RightBraceToken; }
    virtual std::string ToFormatString() const override;

private:
    Token* m_LeftBraceToken = nullptr;
    Token* m_RightBraceToken = nullptr;
};

class FileMetaAssignStatements : public FileMetaSyntax {
public:
    FileMetaAssignStatements(FileMeta* fm, const std::vector<Node*>& nodeList);
    virtual ~FileMetaAssignStatements() = default;

    virtual ::std::string ToFormatString() const override;

private:
    std::vector<Node*> m_NodeList;
};

class FileMetaDefineVarStatements : public FileMetaSyntax {
public:
    FileMetaDefineVarStatements(FileMeta* fm, const std::vector<Node*>& nodeList);
    virtual ~FileMetaDefineVarStatements() = default;

    virtual ::std::string ToFormatString() const override;

private:
    ::std::vector<Node*> m_NodeList;
};

class FileMetaCallStatements : public FileMetaSyntax {
public:
    FileMetaCallStatements(FileMeta* fm, const ::std::vector<Node*>& nodeList);
    virtual ~FileMetaCallStatements() = default;

    virtual ::std::string ToFormatString() const override;

private:
    ::std::vector<Node*> m_NodeList;
};

class FileMetaReturnStatements : public FileMetaSyntax {
public:
    FileMetaReturnStatements(FileMeta* fm, const ::std::vector<Node*>& nodeList);
    virtual ~FileMetaReturnStatements() = default;

    virtual ::std::string ToFormatString() const override;

private:
    ::std::vector<Node*> m_NodeList;
};

class FileMetaIfStatements : public FileMetaSyntax {
public:
    FileMetaIfStatements(FileMeta* fm, const ::std::vector<Node*>& nodeList);
    virtual ~FileMetaIfStatements() = default;

    virtual ::std::string ToFormatString() const override;

private:
    ::std::vector<Node*> m_NodeList;
};

class FileMetaWhileDoWhileStatements : public FileMetaSyntax {
public:
    FileMetaWhileDoWhileStatements(FileMeta* fm, const ::std::vector<Node*>& nodeList);
    virtual ~FileMetaWhileDoWhileStatements() = default;

    virtual ::std::string ToFormatString() const override;

private:
    ::std::vector<Node*> m_NodeList;
};

class FileMetaSwitchStatements : public FileMetaSyntax {
public:
    FileMetaSwitchStatements(FileMeta* fm, const ::std::vector<Node*>& nodeList);
    virtual ~FileMetaSwitchStatements() = default;

    virtual ::std::string ToFormatString() const override;

private:
    ::std::vector<Node*> m_NodeList;
};

class FileMetaBreakContinueGoStatements : public FileMetaSyntax {
public:
    FileMetaBreakContinueGoStatements(FileMeta* fm, const ::std::vector<Node*>& nodeList);
    virtual ~FileMetaBreakContinueGoStatements() = default;

    virtual ::std::string ToFormatString() const override;

private:
    ::std::vector<Node*> m_NodeList;
};

class FileMetaOtherPlatformStatement : public FileMetaSyntax {
public:
    FileMetaOtherPlatformStatement(FileMeta* fm, const ::std::vector<Node*>& nodeList);
    virtual ~FileMetaOtherPlatformStatement() = default;

    virtual ::std::string ToFormatString() const override;

private:
    ::std::vector<Node*> m_NodeList;
};

} // namespace Compile
} // namespace SimpleLanguage
