#pragma once

#include "../../Define.h"
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>
#include "FileMetaBase.h"

namespace SimpleLanguage {
namespace Compile {
// Forward declarations
class FileMeta;
class FileMetaClassDefine;
class FileMetaTemplateDefine;
class FileMetaBlockSyntax;
class FileMetaBaseTerm;
class FileMetaSyntax;
class Token;
class Node;

class FileMetaParamterDefine : public FileMetaBase {
public:
    FileMetaParamterDefine(FileMeta* fileMeta, const std::vector<Node*>& list);
    virtual ~FileMetaParamterDefine() = default;

    // Properties
    FileMetaClassDefine* GetClassDefineRef() const { return m_ClassDefineRef; }
    FileMetaBaseTerm* GetExpress() const { return m_Express; }
    Token* GetParamsToken() const { return m_ParamsToken; }

    // Methods
    bool ParseBuildMetaParamter(const std::vector<Node*>& inputNodeList);
    bool GetNameAndTypeNode(const std::vector<Node*>& listDefieNode, Node*& nameNode, Node*& typeNode, Token*& paramstoken);
    virtual std::string ToFormatString() const override;

private:
    Token* m_AssignToken = nullptr;
    Token* m_ParamsToken = nullptr;
    FileMetaClassDefine* m_ClassDefineRef = nullptr;
    FileMetaBaseTerm* m_Express = nullptr;
};

class FileMetaFunction : public FileMetaBase {
public:
    FileMetaFunction();
    virtual ~FileMetaFunction() = default;

    // Properties
    FileMetaClassDefine* GetDefineMetaClass() const { return m_DefineMetaClass; }
    FileMetaBlockSyntax* GetFileMetaBlockSyntax() const { return m_FileMetaBlockSyntax; }
    const std::vector<FileMetaParamterDefine*>& GetMetaParamtersList() const { return m_MetaParamtersList; }
    const std::vector<FileMetaTemplateDefine*>& GetMetaTemplatesList() const { return m_MetaTemplatesList; }

    // Methods
    void AddMetaParamter(FileMetaParamterDefine* fmp);
    void AddFileMetaSyntax(FileMetaSyntax* fms);
    void AddMetaTemplate(FileMetaTemplateDefine* fmtd);

protected:
    std::vector<FileMetaParamterDefine*> m_MetaParamtersList;
    std::vector<FileMetaTemplateDefine*> m_MetaTemplatesList;
    FileMetaClassDefine* m_DefineMetaClass = nullptr;
    FileMetaBlockSyntax* m_FileMetaBlockSyntax = nullptr;
};

class FileMetaMemberFunction : public FileMetaFunction {
public:
    FileMetaMemberFunction(FileMeta* fm, Node* block, const std::vector<Node*>& nodeList);
    virtual ~FileMetaMemberFunction() = default;

    // Properties
    Token* GetInterfaceToken() const { return m_InterfaceToken; }
    Token* GetStaticToken() const { return m_StaticToken; }
    Token* GetOverrideToken() const { return m_OverrideToken; }
    Token* GetPermissionToken() const { return m_PermissionToken; }
    Token* GetGetToken() const { return m_GetToken; }
    Token* GetSetToken() const { return m_SetToken; }
    Token* GetFinalToken() const { return m_FinalToken; }

    // Methods
    bool ParseFunction(const std::vector<Node*>& nodeList);
    void ParseParam(Node* parNode);
    void ParseTemplate(Node* node);
    virtual void SetDeep(int _deep) override;
    virtual std::string ToFormatString() const override;

private:
    Token* m_InterfaceToken = nullptr;
    Token* m_StaticToken = nullptr;
    Token* m_FinalToken = nullptr;
    Token* m_GetToken = nullptr;
    Token* m_SetToken = nullptr;
    Token* m_OverrideToken = nullptr;
    Token* m_PermissionToken = nullptr;
    Token* m_LeftBraceToken = nullptr;
    Token* m_RightBraceToken = nullptr;
    Node* m_BlockNode = nullptr;
};

} // namespace Compile
} // namespace SimpleLanguage
