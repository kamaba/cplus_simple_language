//****************************************************************************
//  File:      FileMeta.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../../Define.h"
#include "../Token.h"
#include <vector>
#include <string>

namespace SimpleLanguage {
namespace Compile {
// Forward declarations
class FileMeta;
class FileMetaClassDefine;
class FileMetaCallLink;
class FileMetaTemplateDefine;
class FileMetaBaseTerm;
class FileMetaParTerm;
class FileMetaBraceTerm;
class FileMetaBracketTerm;
class FileMetaSyntax;
class FileMetaDefineVariableSyntax;
class FileMetaOpAssignSyntax;
class Node;

class NamespaceStatementBlock 
{
public:
    NamespaceStatementBlock(const std::vector<Token*>& token);
    virtual ~NamespaceStatementBlock() = default;

    std::string NamespaceString() const;
    const std::vector<std::string>& NamespaceStackList() const;
    const std::vector<std::string>& NamespaceList() const;
    const std::vector<Token*>& GetTokenList() const { return m_TokenList; }

    static NamespaceStatementBlock* CreateStateBlock(const std::vector<Token*>& token);
    std::string ToString() const;
    std::string ToFormatString() const;

private:
    mutable std::string m_NamespaceString;
    mutable std::vector<std::string> m_NamespaceList;
    mutable std::vector<std::string> m_NamespaceStackList;
    std::vector<Token*> m_TokenList;
};

class FileInputParamNode {
public:
    FileInputParamNode(FileMetaBaseTerm* fmbt);
    virtual ~FileInputParamNode() = default;

    FileMetaBaseTerm* Express() const { return m_Express; }
    std::string ToFormatString() const;

private:
    FileMetaBaseTerm* m_Express = nullptr;
};

class FileInputTemplateNode {
public:
    FileInputTemplateNode(FileMeta* fm, Node* node);
    virtual ~FileInputTemplateNode() = default;

    FileMeta* GetFileMeta() const { return m_FileMeta; }
    FileMetaCallLink* DefineClassCallLink() const { return m_DefineClassCallLink; }
    std::vector<std::string> GetNameList() const;
    int InputTemplateCount() const;
    std::string ToFormatString() const;

private:
    FileMetaCallLink* m_DefineClassCallLink = nullptr;
    FileMeta* m_FileMeta = nullptr;
    Node* m_Node = nullptr;
};

class FileMetaCallNode {
public:
    FileMetaCallNode(FileMeta* fm, Node* _node);
    virtual ~FileMetaCallNode() = default;

    std::string Name() const;
    bool IsBrace() const { return m_FileMetaBraceTerm != nullptr; }
    const std::vector<FileInputTemplateNode*>& InputTemplateNodeList() const { return m_InputTemplateNodeList; }
    FileMetaParTerm* GetFileMetaParTerm() const { return m_FileMetaParTerm; }
    FileMetaBraceTerm* GetFileMetaBraceTerm() const { return m_FileMetaBraceTerm; }
    FileMetaBracketTerm* GetFileMetaBracketTerm() const { return m_FileMetaBracketTerm; }
    const std::vector<FileMetaCallLink*>& arrayNodeList() const { return m_ArrayNodeList; }
    Token* GetToken() const { return m_Token; }
    Token* AtToken() const { return m_AtToken; }
    bool IsCallFunction() const { return m_IsCallFunction; }
    bool IsTemplate() const { return m_IsTemplate; }
    bool IsArray() const { return m_IsArray; }
    Node* GetNode() const { return m_Node; }
    FileMeta* GetFileMeta() const { return m_FileMeta; }

    std::string ToFormatString() const;
    std::string ToTokenString() const;

private:
    void CreateFileMetaCallNode();

    Node* m_Node = nullptr;
    Token* m_Token = nullptr;
    Token* m_AtToken = nullptr;
    FileMeta* m_FileMeta = nullptr;
    FileMetaParTerm* m_FileMetaParTerm = nullptr;
    FileMetaBraceTerm* m_FileMetaBraceTerm = nullptr;
    FileMetaBracketTerm* m_FileMetaBracketTerm = nullptr;
    Token* m_BeginParToken = nullptr;
    Token* m_EndParToken = nullptr;
    Token* m_BeginAngleToken = nullptr;
    Token* m_EndAngleToken = nullptr;
    Token* m_BeginBracketToken = nullptr;
    Token* m_EndBracketToken = nullptr;
    std::vector<FileInputTemplateNode*> m_InputTemplateNodeList;
    std::vector<FileMetaCallLink*> m_ArrayNodeList;
    bool m_IsCallFunction = false;
    bool m_IsTemplate = false;
    bool m_IsArray = false;
};

class FileMetaCallLink {
public:
    FileMetaCallLink(FileMeta* fm, Node* node);
    virtual ~FileMetaCallLink() = default;

    bool IsOnlyName() const;
    std::string Name() const;
    const std::vector<FileMetaCallNode*>& CallNodeList() const { return m_CallNodeList; }

    std::string ToFormatString() const;
    std::string ToTokenString() const;

private:
    void AddChildExtendLinkList(Node* cnode, bool isIncludeSelf);

    FileMeta* m_FileMeta = nullptr;
    Node* m_Node = nullptr;
    std::vector<FileMetaCallNode*> m_CallNodeList;
};

class FileMetaClassDefine {
public:
    FileMetaClassDefine(FileMeta* fm, const std::vector<Token*>& _tokenList);
    FileMetaClassDefine(FileMeta* fm, Node* node, Node* mutNode = nullptr);
    virtual ~FileMetaClassDefine() = default;

    std::vector<std::string> StringList() const;
    std::string AllName() const;
    std::string Name() const;
    FileMeta* GetFileMeta() const { return m_FileMeta; }
    Token* ClassNameToken() const { return m_ClassNameToken; }
    bool IsInputTemplateData() const { return m_IsInputTemplateData; }
    bool IsArray() const { return m_IsArray; }
    const std::vector<FileInputTemplateNode*>& InputTemplateNodeList() const { return m_InputTemplateNodeList; }
    const std::vector<Token*>& ArrayTokenList() const { return m_ArrayTokenList; }

    SimpleLanguage::Core::MetaNode* GetChildrenMetaNode(SimpleLanguage::Core::MetaNode* mb);
    std::string ToString() const;
    std::string ToTokenString() const;
    std::string ToFormatString() const;
    void AddError2(int errorId, const std::string& pfile = "", const std::string& pfunction = "", int line = 0);

private:
    FileMeta* m_FileMeta = nullptr;
    Token* m_ClassNameToken = nullptr;
    Token* m_AngleTokenBegin = nullptr;
    Token* m_AngleTokenEnd = nullptr;
    Token* m_BracketTokenBegin = nullptr;
    Token* m_BracketTokenEnd = nullptr;
    Token* m_MutToken = nullptr;
    std::vector<Token*> m_ArrayTokenList;
    std::vector<FileInputTemplateNode*> m_InputTemplateNodeList;
    std::vector<Token*> m_TokenList;
    bool m_IsInputTemplateData = false;
    bool m_IsArray = false;
};

class FileMetaTemplateDefine : public FileMetaBase {
public:
    FileMetaTemplateDefine(FileMeta* fm, Node* node);
    FileMetaTemplateDefine(FileMeta* fm, Node* node, Node* extendsNode);
    FileMetaTemplateDefine(FileMeta* fm, const std::vector<Node*>& nodeList);
    virtual ~FileMetaTemplateDefine() = default;

    Token* InToken() const { return m_InToken; }
    FileInputTemplateNode* InClassNameTemplateNode() const { return m_InClassNameTemplateNode; }
    Node* ExtendNode() const { return m_ExtendsNode; }

    void Parse();
    virtual std::string ToFormatString() const override;

private:
    Token* m_InToken = nullptr;
    FileInputTemplateNode* m_InClassNameTemplateNode = nullptr;
    Node* m_Node = nullptr;
    Node* m_ExtendsNode = nullptr;
};

} // namespace Compile
} // namespace SimpleLanguage

