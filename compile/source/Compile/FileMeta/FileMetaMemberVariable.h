#pragma once

#include "FileMetaBase.h"
#include "FileMetaCommon.h"
#include "FileMetaExpress.h"
#include "FileMetatUtil.h"
#include "../CompilerUtil.h"
#include "../Parse/Node.h"
#include "../../Define.h"
#include "../../Debug/Log.h"
#include "../Parse/StructParse.h"
#include <vector>
#include <string>
#include <sstream>

namespace SimpleLanguage {
namespace Compile {

// Forward declarations
class FileMeta;
class FileMetaClassDefine;
class FileMetaBaseTerm;
class FileMetaCallTerm;
class FileMetaConstValueTerm;
class FileMetaBracketTerm;
class Token;

class FileMetaMemberVariable : public FileMetaBase {
public:
    enum class EMemberDataType {
        None,
        NameClass,
        Array,
        KeyValue,
        ConstVariable
    };

    FileMetaMemberVariable(FileMeta* fm, const ::std::vector<Node*>& list);
    FileMetaMemberVariable(FileMeta* fm, Node* brace);
    FileMetaMemberVariable(FileMeta* fm, Node* _beforeNode, Node* _afterNode, EMemberDataType dataType);
    virtual ~FileMetaMemberVariable() = default;

    // Properties
    FileMetaClassDefine* classDefineRef() const { return m_ClassDefineRef; }
    Token* PermissionToken() const { return m_PermissionToken; }
    Token* StaticToken() const { return m_StaticToken; }
    Token* NameToken() const { return m_Token; }
    FileMetaBaseTerm* GetExpress() const { return m_Express; }
    Token* AssignToken() const { return m_AssignToken; }
    const ::std::vector<FileMetaMemberVariable*>& GetFileMetaMemberVariable() const { return m_FileMetaMemberVariableList; }
    FileMetaCallTerm* GetFileMetaCallTermValue() const { return m_FileMetaCallTermValue; }
    EMemberDataType DataType() const { return m_MemberDataType; }

    // Methods
    bool ParseBuildMetaVariable();
    void ParseBracketContrent(Node* pnode);
    void ParseFileMemberVariableContent();
    bool GetNameAndTypeToken(const ::std::vector<Node*>& defineNodeList, Node*& typeNode, Node*& mutNode,
                             Node*& nameNode, Token*& permissionToken, Token*& staticToken);
    void AddFileMemberVariable(FileMetaMemberVariable* fmmd);
    FileMetaMemberVariable* GetFileMetaMemberDataByName(const ::std::string& name);
    virtual void SetDeep(int _deep) override;
    virtual ::std::string ToString() const override;
    virtual ::std::string ToFormatString() const override;

private:
    FileMetaClassDefine* m_ClassDefineRef = nullptr;
    Token* m_AssignToken = nullptr;
    Token* m_PermissionToken = nullptr;
    Token* m_StaticToken = nullptr;
    FileMetaBaseTerm* m_Express = nullptr;
    ::std::vector<FileMetaMemberVariable*> m_FileMetaMemberVariableList;
    EMemberDataType m_MemberDataType = EMemberDataType::None;
    FileMetaCallTerm* m_FileMetaCallTermValue = nullptr;
    ::std::vector<Node*> m_NodeList;
};

} // namespace Compile
} // namespace SimpleLanguage
