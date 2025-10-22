//****************************************************************************
//  File:      FileMeta.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../../Define.h"
#include <vector>
#include <string>
#include <stack>
#include <sstream>
#include "FileMetaBase.h"

namespace SimpleLanguage {
namespace Compile {

// Forward declarations
class FileMeta;
class FileMetaClass;
class NamespaceStatementBlock;
class Node;

class FileMetaNamespace : public FileMetaBase {
public:
    FileMetaNamespace(FileMetaNamespace* fmn);
    FileMetaNamespace(Node* namespaceNode, Node* namespaceNameNode);
    virtual ~FileMetaNamespace() = default;

    // Properties
    bool IsSearchNamespace() const { return m_IsSearchNamespace; }
    Node* NamespaceNode() const { return m_NamespaceNode; }
    Node* NamespaceNameNode() const { return m_NamespaceNameNode; }
    virtual std::string Name() const override;
    NamespaceStatementBlock* GetNamespaceStatementBlock() const { return m_NamespaceStateBlock; }
    FileMetaNamespace* topLevelFileMetaNamespace = nullptr;
    std::stack<FileMetaNamespace*> NamespaceStack() const;

    // Methods
    FileMetaNamespace* AddFileNamespace(FileMetaNamespace* dln);
    void AddFileMetaClass(FileMetaClass* mc);
    virtual std::string ToString() const override;
    virtual void SetDeep(int _deep) override;
    virtual std::string ToFormatString() const override;

private:
    Node* m_NamespaceNode = nullptr;
    Node* m_NamespaceNameNode = nullptr;
    Token* m_BraceBeginToken = nullptr;
    Token* m_BraceEndToken = nullptr;
    bool m_IsSearchNamespace = false;
    NamespaceStatementBlock* m_NamespaceStateBlock = nullptr;
    std::vector<FileMetaNamespace*> m_MetaNamespaceList;
    std::vector<FileMetaClass*> m_ChildrenClassList;
    mutable std::stack<FileMetaNamespace*> s_MetaNamespaceStack;
};

} // namespace Compile
} // namespace SimpleLanguage

