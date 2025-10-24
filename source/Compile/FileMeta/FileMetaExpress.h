#pragma once

#include "FileMetaBase.h"
#include "../Token.h"
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
class FileMetaTermExpress;
class FileMetaSymbolTerm;
class FileMetaConstValueTerm;
class FileMetaCallTerm;
class FileMetaParTerm;
class FileMetaBraceTerm;
class FileMetaBracketTerm;
class FileMetaIfSyntaxTerm;
class FileMetaThreeItemSyntaxTerm;
class FileMetaMatchSyntaxTerm;
class FileMetaCallLink;
class FileMetaSyntax;
class FileMetaKeyIfSyntax;
class FileMetaKeySwitchSyntax;

class FileMetaBaseTerm : public FileMetaBase {
public:
    FileMetaBaseTerm(FileMeta* fm);
    virtual ~FileMetaBaseTerm() = default;

    // Properties
    bool GetDirty() const;
    void SetDirty(bool value);
    
    int GetPriority() const;
    void SetPriority(int value);
    
    bool IsOnlyOne() const;
    const std::vector<FileMetaBaseTerm*>& GetFileMetaExpressList() const;
    
    FileMetaBaseTerm* GetLeft() const;
    void SetLeft(FileMetaBaseTerm* value);
    
    FileMetaBaseTerm* GetRight() const;
    void SetRight(FileMetaBaseTerm* value);
    
    virtual FileMetaBaseTerm* GetRoot() const;
    
    // Methods
    std::vector<FileMetaBaseTerm*> SplitParamList();
    virtual void ClearDirty();
    virtual void AddFileMetaTerm(FileMetaBaseTerm* fmn);
    virtual void AddRangeFileMetaTerm(const std::vector<FileMetaBaseTerm*>& fmn);
    virtual bool BuildAST();
    virtual std::string ToFormatString() const override;
    virtual std::string ToTokenString() const;

protected:
    bool m_IsDirty = false;
    int m_Priority = std::numeric_limits<int>::max();
    std::vector<FileMetaBaseTerm*> m_FileMetaExpressList;
    FileMetaBaseTerm* m_Left = nullptr;
    FileMetaBaseTerm* m_Right = nullptr;
    FileMetaBaseTerm* m_Root = nullptr;
};

class FileMetaSymbolTerm : public FileMetaBaseTerm {
public:
    FileMetaSymbolTerm(FileMeta* fm, Token* token);
    virtual ~FileMetaSymbolTerm() = default;

    ETokenType GetSymbolType() const;
    Token* GetToken() const;
    
    virtual bool BuildAST() override;
    virtual std::string ToFormatString() const override;
    virtual std::string ToTokenString() const override;

private:
    void SetPriority();
    Token* m_Token = nullptr;
};

class FileMetaConstValueTerm : public FileMetaBaseTerm {
public:
    FileMetaConstValueTerm(FileMeta* fm, Token* token);
    FileMetaConstValueTerm(FileMeta* fm, Token* valueToken, Token* plusMinusToken);
    virtual ~FileMetaConstValueTerm() = default;

    Token* GetValueToken() const;
    Token* GetPlusOrMinusToken() const;
    
    virtual bool BuildAST() override;
    virtual std::string ToFormatString() const override;
    virtual std::string ToTokenString() const override;

private:
    Token* m_ValueToken = nullptr;
    Token* m_PlusOrMinusToken = nullptr;
};

class FileMetaCallTerm : public FileMetaBaseTerm {
public:
    FileMetaCallTerm(FileMeta* fm, Node* node);
    virtual ~FileMetaCallTerm() = default;

    FileMetaCallLink* GetCallLink() const;
    Node* GetCallNode() const;
    
    virtual bool BuildAST() override;
    virtual std::string ToFormatString() const override;
    virtual std::string ToTokenString() const override;

private:
    FileMetaCallLink* m_CallLink = nullptr;
};

class FileMetaParTerm : public FileMetaBaseTerm {
public:
    FileMetaParTerm(FileMeta* fm, Node* node, FileMetaTermExpress::EExpressType expressType);
    virtual ~FileMetaParTerm() = default;

    Node* GetParNode() const;
    Token* GetEndToken() const;
    FileMetaTermExpress::EExpressType GetExpressType() const;
    
    virtual void ClearDirty() override;
    virtual bool BuildAST() override;
    virtual std::string ToFormatString() const override;
    virtual std::string ToTokenString() const override;

private:
    Node* m_Node = nullptr;
    Token* m_EndToken = nullptr;
    FileMetaTermExpress::EExpressType m_ExpressType;
};

class FileMetaBraceTerm : public FileMetaBaseTerm {
public:
    FileMetaBraceTerm(FileMeta* fm, Node* node);
    virtual ~FileMetaBraceTerm() = default;

    const std::vector<FileMetaSyntax*>& GetFileMetaAssignSyntaxList() const;
    const std::vector<FileMetaCallLink*>& GetFileMetaCallLinkList() const;
    bool IsArray() const;
    void SetIsArray(bool value);
    
    virtual void ClearDirty() override;
    virtual bool BuildAST() override;
    virtual std::string ToFormatString() const override;
    virtual std::string ToTokenString() const override;

private:
    void HandleBraceTerm();
    
    std::vector<FileMetaSyntax*> m_FileMetaAssignSyntaxList;
    std::vector<FileMetaCallLink*> m_FileMetaCallLinkList;
    bool m_IsArray = false;
    Token* m_BraceEndToken = nullptr;
    Node* m_Node = nullptr;
};

class FileMetaBracketTerm : public FileMetaBaseTerm {
public:
    FileMetaBracketTerm(FileMeta* fm, Node* node);
    FileMetaBracketTerm(FileMeta* fm, Node* node, int bracketType);
    virtual ~FileMetaBracketTerm() = default;

    Node* GetBracketNode() const;
    int GetBracketType() const;
    Token* GetEndToken() const;
    
    virtual bool BuildAST() override;
    virtual std::string ToFormatString() const override;
    virtual std::string ToTokenString() const override;

private:
    Token* m_BracketeEndToken = nullptr;
    Node* m_BracketNode = nullptr;
    int m_BracketType = 0;
};

class FileMetaIfSyntaxTerm : public FileMetaBaseTerm {
public:
    FileMetaIfSyntaxTerm(FileMeta* fm, FileMetaKeyIfSyntax* ifSyntax);
    virtual ~FileMetaIfSyntaxTerm() = default;

    FileMetaKeyIfSyntax* GetIfSyntax() const;
    
    virtual void SetDeep(int deep) override;
    virtual bool BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    FileMetaKeyIfSyntax* m_IfSyntax = nullptr;
};

class FileMetaThreeItemSyntaxTerm : public FileMetaBaseTerm {
public:
    FileMetaThreeItemSyntaxTerm(FileMeta* fm);
    virtual ~FileMetaThreeItemSyntaxTerm() = default;

    FileMetaBaseTerm* GetReturn1Term() const;
    void SetReturn1Term(FileMetaBaseTerm* value);
    
    FileMetaBaseTerm* GetReturn2Term() const;
    void SetReturn2Term(FileMetaBaseTerm* value);
    
    FileMetaBaseTerm* GetConditionTerm() const;
    void SetConditionTerm(FileMetaBaseTerm* value);
    
    void SetItemTerm(int i, FileMetaBaseTerm* fmbt);
    
    virtual void SetDeep(int deep) override;
    virtual bool BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    FileMetaBaseTerm* m_Return1Term = nullptr;
    FileMetaBaseTerm* m_Return2Term = nullptr;
    FileMetaBaseTerm* m_ConditionTerm = nullptr;
};

class FileMetaMatchSyntaxTerm : public FileMetaBaseTerm {
public:
    FileMetaMatchSyntaxTerm(FileMeta* fm, FileMetaKeySwitchSyntax* switchSyntax);
    virtual ~FileMetaMatchSyntaxTerm() = default;

    FileMetaKeySwitchSyntax* GetSwitchSyntax() const;
    
    virtual void SetDeep(int deep) override;
    virtual bool BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    FileMetaKeySwitchSyntax* m_SwitchSyntax = nullptr;
};

class FileMetaTermExpress : public FileMetaBaseTerm {
public:
    enum class EExpressType {
        Common,
        MemberVariable,
        ParamVariable,
    };

    FileMetaTermExpress(FileMeta* fm, const std::vector<Node*>& nodeList, EExpressType expressType);
    virtual ~FileMetaTermExpress() = default;

    const std::vector<Node*>& GetNodeList() const;
    EExpressType GetExpressType() const;
    bool GetCanUseDoublePlusOrMinus() const;
    void SetCanUseDoublePlusOrMinus(bool value);
    
    virtual void SetDeep(int deep) override;
    virtual bool BuildAST() override;
    virtual std::string ToFormatString() const override;
    virtual std::string ToTokenString() const override;

private:
    void CreateFileMetaExpressByChildList(const std::vector<Node*>& nodeList);
    bool BuildTst(std::vector<FileMetaBaseTerm*>& list);
    
    std::vector<Node*> m_NodeList;
    EExpressType m_ExpressType;
    bool m_CanUseDoublePlusOrMinus = false;
};

} // namespace Compile
} // namespace SimpleLanguage
