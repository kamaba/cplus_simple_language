#pragma once

#include "../../Define.h"
#include <vector>
#include <string>
#include <sstream>


namespace SimpleLanguage {
namespace Compile {

// Forward declarations
class FileMeta;
class NamespaceStatementBlock;
class Node;

class FileMetaImportSyntax : public FileMetaBase {
public:
    FileMetaImportSyntax(const ::std::vector<Node*>& _nodeList);
    virtual ~FileMetaImportSyntax() = default;

    // Properties
    Token* AsToken() const { return m_AsToken; }
    Token* AsNameToken() const { return m_AsNameToken; }
    const ::std::vector<Token*>& ImportNameListToken() const { return m_ImportNameListToken; }
    NamespaceStatementBlock* NamespaceStatement() const { return m_NamespaceStatement; }
    NamespaceStatementBlock* AsNameStatement() const { return m_AsNameStatement; }

    // Methods
    bool ParseImportSyntax();
    virtual ::std::string ToFormatString() const override;

private:
    Token* m_AsToken = nullptr;
    Token* m_AsNameToken = nullptr;
    ::std::vector<Token*> m_ImportNameListToken;
    ::std::vector<Node*> m_NodeList;
    NamespaceStatementBlock* m_NamespaceStatement = nullptr;
    NamespaceStatementBlock* m_AsNameStatement = nullptr;
};

class FileMetaGlobalSyntax : public FileMetaBase {
public:
    FileMetaGlobalSyntax(FileMeta* fm, const ::std::vector<Node*>& nodeList);
    virtual ~FileMetaGlobalSyntax() = default;

    // Properties
    const ::std::vector<FileMetaImportSyntax*>& importSyntaxList() const { return m_ImportSyntaxList; }
    const ::std::vector<FileMetaNamespace*>& namespaceSyntaxList() const { return m_NamespaceSyntaxList; }
    const ::std::vector<FileMetaClass*>& classSyntaxList() const { return m_ClassSyntaxList; }

    // Methods
    void AddImportSyntax(FileMetaImportSyntax* importSyntax);
    void AddNamespaceSyntax(FileMetaNamespace* namespaceSyntax);
    void AddClassSyntax(FileMetaClass* classSyntax);
    virtual ::std::string ToFormatString() const override;

private:
    ::std::vector<FileMetaImportSyntax*> m_ImportSyntaxList;
    ::std::vector<FileMetaNamespace*> m_NamespaceSyntaxList;
    ::std::vector<FileMetaClass*> m_ClassSyntaxList;
    ::std::vector<Node*> m_NodeList;
};

} // namespace Compile
} // namespace SimpleLanguage
