#pragma once

#include "FileMetaBase.h"
#include "FileMetaCommon.h"
#include "FileMetaExpress.h"
#include "../FileMetatUtil.h"
#include "../CompilerUtil.h"
#include "../Parse/Node.h"
#include "../../Define.h"
#include "../../Debug/Log.h"
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>

namespace SimpleLanguage {
namespace Compile {
// Forward declarations
class FileMeta;
class FileMetaClassDefine;
class FileMetaTemplateDefine;
class FileMetaBlockSyntax;
class FileMetaBaseTerm;

class FileMetaParamterDefine : public FileMetaBase {
public:
    FileMetaParamterDefine(FileMeta* fileMeta, const std::vector<Node*>& list);
    virtual ~FileMetaParamterDefine() = default;

    // Properties
    FileMetaClassDefine* classDefineRef() const { return m_ClassDefineRef; }
    FileMetaBaseTerm* express() const { return m_Express; }
    Token* paramsToken() const { return m_ParamsToken; }

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
    FileMetaClassDefine* defineMetaClass() const { return m_DefineMetaClass; }
    FileMetaBlockSyntax* fileMetaBlockSyntax() const { return m_FileMetaBlockSyntax; }
    const std::vector<FileMetaParamterDefine*>& metaParamtersList() const { return m_MetaParamtersList; }
    const std::vector<FileMetaTemplateDefine*>& metaTemplatesList() const { return m_MetaTemplatesList; }

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
    Token* interfaceToken() const { return m_InterfaceToken; }
    Token* staticToken() const { return m_StaticToken; }
    Token* overrideToken() const { return m_OverrideToken; }
    Token* permissionToken() const { return m_PermissionToken; }
    Token* getToken() const { return m_GetToken; }
    Token* setToken() const { return m_SetToken; }
    Token* finalToken() const { return m_FinalToken; }

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
