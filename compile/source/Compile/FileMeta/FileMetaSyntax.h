#pragma once

#include "FileMetaBase.h"
#include <vector>
#include <string>

namespace SimpleLanguage {
    namespace Compile {
        class FileMeta;
        class FileMetaBlockSyntax;
        class FileMetaBaseTerm;
        class FileMetaCallLink;
        class FileMetaClassDefine;
        class FileMetaConstValueTerm;
        class Node;
        class Token;
    }
}

namespace SimpleLanguage {
namespace Compile {

// 前向声明
class FileMetaConditionExpressSyntax;
class FileMetaKeyIfSyntax;
class FileMetaKeySwitchSyntax;
class FileMetaKeyMatchSyntax;
class FileMetaKeyForSyntax;
class FileMetaKeyOnlySyntax;
class FileMetaKeyReturnSyntax;
class FileMetaKeyGotoLabelSyntax;
class FileMetaCallSyntax;
class FileMetaDefineVariableSyntax;
class FileMetaOpAssignSyntax;
class FileMetaBlockSyntax;

class FileMetaSyntax : public FileMetaBase {
public:
    FileMetaSyntax();
    virtual ~FileMetaSyntax() = default;

    // Properties
    int GetParseIndex() const;
    void SetParseIndex(int value);
    
    bool IsAppendSemiColon() const;
    void SetAppendSemiColon(bool value);
    
    const std::vector<FileMetaSyntax*>& GetFileMetaSyntax() const;

    // Methods
    bool IsNotEnd() const;
    FileMetaSyntax* GetCurrentSyntaxAndMove(int moveIndex = 1);
    void AddFileMetaSyntax(FileMetaSyntax* fms);
    virtual std::string ToFormatString() const override;

protected:
    std::vector<FileMetaSyntax*> m_FileMetaSyntax;
    std::vector<Node*> m_NodeList;
    int m_ParseIndex = 0;
    bool m_IsAppendSemiColon = true;
};

class FileMetaConditionExpressSyntax : public FileMetaSyntax {
public:
    FileMetaConditionExpressSyntax(FileMeta* fm, Token* ifToken, FileMetaBaseTerm* condition, FileMetaBlockSyntax* executeBlockSyntax);
    virtual ~FileMetaConditionExpressSyntax() = default;

    FileMetaBaseTerm* GetConditionExpress() const;
    FileMetaBlockSyntax* GetExecuteBlockSyntax() const;
    
    virtual void SetDeep(int deep) override;
    virtual std::string ToFormatString() const override;

private:
    FileMetaBaseTerm* m_ConditionExpress = nullptr;
    FileMetaBlockSyntax* m_ExecuteBlockSyntax = nullptr;
};

class FileMetaKeyIfSyntax : public FileMetaSyntax {
public:
    FileMetaKeyIfSyntax(FileMeta* fm);
    virtual ~FileMetaKeyIfSyntax() = default;

    FileMetaConditionExpressSyntax* GetIfExpressSyntax() const;
    const std::vector<FileMetaConditionExpressSyntax*>& GetElseIfExpressSyntax() const;
    FileMetaKeyOnlySyntax* GetElseExpressSyntax() const;
    
    void SetFileMetaConditionExpressSyntax(FileMetaConditionExpressSyntax* fmces);
    void SetElseExpressSyntax(FileMetaKeyOnlySyntax* fmces);
    void AddElseIfExpressSyntax(FileMetaConditionExpressSyntax* fmces);
    
    virtual void SetDeep(int deep) override;
    virtual std::string ToFormatString() const override;

    // Static method
    static FileMetaKeyIfSyntax* ParseIfSyntax(FileMeta* fm, void* sns); // StructParse.SyntaxNodeStruct

private:
    FileMetaConditionExpressSyntax* m_IfExpressSyntax = nullptr;
    std::vector<FileMetaConditionExpressSyntax*> m_ElseIfExpressSyntax;
    FileMetaKeyOnlySyntax* m_ElseExpressSyntax = nullptr;
};

class FileMetaKeySwitchSyntax : public FileMetaSyntax {
public:
    class FileMetaKeyCaseSyntax {
    public:
        FileMetaKeyCaseSyntax(FileMeta* fm, Token* castToken);
        virtual ~FileMetaKeyCaseSyntax() = default;

        FileMeta* GetFileMeta() const;
        bool IsContinueNextCastSyntax() const;
        void SetContinueNextCastSyntax(bool value);
        Token* GetVariableToken() const;
        FileMetaCallLink* GetDefineClassCallLink() const;
        FileMetaBlockSyntax* GetExecuteBlockSyntax() const;
        const std::vector<FileMetaConstValueTerm*>& GetConstValueTokenList() const;

        void SetDefineClassNode(Node* defineClassNode);
        void SetVariableToken(Token* variableToken);
        void SetExecuteBlockSyntax(FileMetaBlockSyntax* ebs);
        void AddConstValueTokenList(FileMetaConstValueTerm* fmcvt);
        void SetDeep(int deep);
        std::string ToFormatString() const;

    private:
        FileMeta* m_FileMeta = nullptr;
        Token* m_Token = nullptr;
        FileMetaCallLink* m_DefineClassToken = nullptr;
        Token* m_VariableToken = nullptr;
        std::vector<FileMetaConstValueTerm*> m_ConstValueTokenList;
        FileMetaBlockSyntax* m_ExecuteBlockSyntax = nullptr;
        int m_Deep = 0;
        bool m_IsContinueNextCastSyntax = false;
    };

    FileMetaKeySwitchSyntax(FileMeta* fm, Token* switchToken, Token* leftBraceToken, Token* rightBraceToken, FileMetaCallLink* cl);
    virtual ~FileMetaKeySwitchSyntax() = default;

    FileMetaCallLink* GetFileMetaVariableRef() const;
    FileMetaBlockSyntax* GetDefaultExecuteBlockSyntax() const;
    const std::vector<FileMetaKeyCaseSyntax*>& GetFileMetaKeyCaseSyntaxList() const;
    FileMetaBlockSyntax* GetExecuteBlockSyntax() const;

    void AddFileMetaKeyCaseSyntaxList(FileMetaKeyCaseSyntax* keyCase);
    void SetDefaultExecuteBlockSyntax(FileMetaBlockSyntax* fmbs);
    
    virtual void SetDeep(int deep) override;
    virtual std::string ToFormatString() const override;

private:
    Token* m_LeftBraceToken = nullptr;
    Token* m_RightBraceToken = nullptr;
    FileMetaCallLink* m_FileMetaVariableRef = nullptr;
    FileMetaBlockSyntax* m_DefaultExecuteBlockSyntax = nullptr;
    std::vector<FileMetaKeyCaseSyntax*> m_FileMetaKeyCaseSyntaxList;
};

class FileMetaKeyMatchSyntax : public FileMetaSyntax {
public:
    class FileMetaKeyCaseSyntax 
    {
    public:
        FileMetaKeyCaseSyntax(FileMeta* fm, Token* castToken);
        virtual ~FileMetaKeyCaseSyntax() = default;

        FileMeta* GetFileMeta() const;
        bool IsContinueNextCastSyntax() const;
        void SetContinueNextCastSyntax(bool value);
        Token* GetVariableToken();
        FileMetaCallLink* GetDefineClassCallLink() const;
        FileMetaBlockSyntax* GetExecuteBlockSyntax() const;
        std::vector<FileMetaConstValueTerm*>& GetConstValueTokenList() const;

        void SetDefineClassNode(Node* defineClassNode);
        void SetVariableToken(Token* variableToken);
        void SetExecuteBlockSyntax(FileMetaBlockSyntax* ebs);
        void AddConstValueTokenList(FileMetaConstValueTerm* fmcvt);
        void SetDeep(int deep);
        std::string ToFormatString() const;

    private:
        FileMeta* m_FileMeta = nullptr;
        Token* m_Token = nullptr;
        FileMetaCallLink* m_DefineClassToken = nullptr;
        Token* m_VariableToken = nullptr;
        std::vector<FileMetaConstValueTerm*>& m_ConstValueTokenList;
        FileMetaBlockSyntax* m_ExecuteBlockSyntax = nullptr;
        int m_Deep = 0;
        bool m_IsContinueNextCastSyntax = false;
    };

    FileMetaKeyMatchSyntax(FileMeta* fm, Token* switchToken, Token* leftBraceToken, Token* rightBraceToken, FileMetaCallLink* cl);
    virtual ~FileMetaKeyMatchSyntax() = default;

    FileMetaCallLink* GetFileMetaVariableRef() const;
    FileMetaBlockSyntax* GetDefaultExecuteBlockSyntax() const;
    const std::vector<FileMetaKeyCaseSyntax*>& GetFileMetaKeyCaseSyntaxList() const;
    FileMetaBlockSyntax* GetExecuteBlockSyntax() const;

    void AddFileMetaKeyCaseSyntaxList(FileMetaKeyCaseSyntax* keyCase);
    void SetDefaultExecuteBlockSyntax(FileMetaBlockSyntax* fmbs);
    
    virtual void SetDeep(int deep) override;
    virtual std::string ToFormatString() const override;

private:
    Token* m_LeftBraceToken = nullptr;
    Token* m_RightBraceToken = nullptr;
    FileMetaCallLink* m_FileMetaVariableRef = nullptr;
    FileMetaBlockSyntax* m_DefaultExecuteBlockSyntax = nullptr;
    std::vector<FileMetaKeyCaseSyntax*> m_FileMetaKeyCaseSyntaxList;
};

class FileMetaKeyForSyntax : public FileMetaSyntax {
public:
    FileMetaKeyForSyntax(FileMeta* fm, Token* forToken, FileMetaBlockSyntax* fmbs);
    virtual ~FileMetaKeyForSyntax() = default;

    FileMetaBlockSyntax* GetExecuteBlockSyntax() const;
    FileMetaSyntax* GetFileMetaClassDefine() const;
    FileMetaBaseTerm* GetConditionExpress() const;
    FileMetaOpAssignSyntax* GetStepFileMetaOpAssignSyntax() const;
    bool IsInFor() const;

    void SetFileMetaClassDefine(FileMetaSyntax* fmdvs);
    void SetInKeyAndArrayVariable(Token* inToken, FileMetaBaseTerm* cn);
    void SetConditionExpress(FileMetaBaseTerm* fmte);
    void SetStepFileMetaOpAssignSyntax(FileMetaOpAssignSyntax* fmoas);
    
    virtual void SetDeep(int deep) override;
    virtual std::string ToFormatString() const override;

private:
    FileMetaBlockSyntax* m_ExecuteBlockSyntax = nullptr;
    FileMetaSyntax* m_FileMetaClassDefine = nullptr;
    FileMetaBaseTerm* m_ConditionExpress = nullptr;
    FileMetaOpAssignSyntax* m_StepFileMetaOpAssignSyntax = nullptr;
    Token* m_InToken = nullptr;
};

class FileMetaKeyOnlySyntax : public FileMetaSyntax {
public:
    FileMetaKeyOnlySyntax(FileMeta* fm, Token* token, FileMetaBlockSyntax* executeBlockSyntax);
    virtual ~FileMetaKeyOnlySyntax() = default;

    FileMetaBlockSyntax* GetExecuteBlockSyntax() const;
    
    virtual void SetDeep(int deep) override;
    virtual std::string ToFormatString() const override;

private:
    FileMetaBlockSyntax* m_ExecuteBlockSyntax = nullptr;
};

class FileMetaKeyReturnSyntax : public FileMetaSyntax {
public:
    FileMetaKeyReturnSyntax(FileMeta* fm, Token* token, FileMetaBaseTerm* express);
    virtual ~FileMetaKeyReturnSyntax() = default;

    FileMetaBaseTerm* GetReturnExpress() const;
    
    virtual std::string ToFormatString() const override;

    // Static method
    static FileMetaKeyReturnSyntax* ParseIfSyntax(FileMeta* fm, void* akss); // StructParse.SyntaxNodeStruct

private:
    FileMetaBaseTerm* m_ReturnExpress = nullptr;
};

class FileMetaKeyGotoLabelSyntax : public FileMetaSyntax {
public:
    FileMetaKeyGotoLabelSyntax(FileMeta* fm, Token* token, Token* label);
    virtual ~FileMetaKeyGotoLabelSyntax() = default;

    Token* GetLabelToken() const;
    
    virtual std::string ToFormatString() const override;

    // Static method
    static FileMetaKeyGotoLabelSyntax* ParseIfSyntax(FileMeta* fm, void* akss); // StructParse.SyntaxNodeStruct

private:
    Token* m_LabelToken = nullptr;
};

class FileMetaCallSyntax : public FileMetaSyntax {
public:
    FileMetaCallSyntax(FileMetaCallLink* fmrv);
    virtual ~FileMetaCallSyntax() = default;

    FileMetaCallLink* GetVariableRef() const;
    
    virtual std::string ToFormatString() const override;

private:
    FileMetaCallLink* m_FileMetaVariableRef = nullptr;
};

class FileMetaDefineVariableSyntax : public FileMetaSyntax {
public:
    FileMetaDefineVariableSyntax(FileMeta* fm, FileMetaClassDefine* fmcd, Token* nameToken, Token* assignToken, Token* staticToken, FileMetaBaseTerm* express);
    virtual ~FileMetaDefineVariableSyntax() = default;

    Token* GetNameToken() const;
    Token* GetStaticToken() const;
    Token* GetAssignToken() const;
    FileMetaClassDefine* GetFileMetaClassDefine() const;
    FileMetaBaseTerm* GetExpress() const;
    
    virtual std::string ToFormatString() const override;

private:
    FileMetaClassDefine* m_FileMetaClassDefine = nullptr;
    Token* m_StaticToken = nullptr;
    Token* m_AssignToken = nullptr;
    FileMetaBaseTerm* m_FileMetaExpress = nullptr;
};

class FileMetaOpAssignSyntax : public FileMetaSyntax {
public:
    FileMetaOpAssignSyntax(FileMetaCallLink* fileMetaVariableRef, Token* opAssignToken, Token* dynamicClassToken, Token* dynamicDataToken, Token* varToken, FileMetaBaseTerm* fme, bool flag = false);
    virtual ~FileMetaOpAssignSyntax() = default;

    FileMetaCallLink* GetVariableRef() const;
    FileMetaBaseTerm* GetExpress() const;
    Token* GetAssignToken() const;
    Token* GetDynamicToken() const;
    Token* GetConstToken() const;
    Token* GetStaticToken() const;
    bool HasDefine() const;
    
    virtual void SetDeep(int deep) override;
    virtual std::string ToFormatString() const override;

private:
    FileMetaCallLink* m_VariableRef = nullptr;
    FileMetaBaseTerm* m_Express = nullptr;
    Token* m_AssignToken = nullptr;
    Token* m_DynamicToken = nullptr;
    Token* m_DataToken = nullptr;
    Token* m_VarToken = nullptr;
    Token* m_ConstToken = nullptr;
    Token* m_StaticToken = nullptr;
};

class FileMetaBlockSyntax : public FileMetaSyntax {
public:
    FileMetaBlockSyntax(FileMeta* fm, Token* bblock, Token* eblock);
    virtual ~FileMetaBlockSyntax() = default;

    Token* GetBeginBlock() const;
    Token* GetEndBlock() const;
    
    virtual void SetDeep(int deep) override;
    virtual std::string ToFormatString() const override;

private:
    Token* m_BeginBlock = nullptr;
    Token* m_EndBlock = nullptr;
};

} // namespace Compile
} // namespace SimpleLanguage