#pragma once

#include "FileMetaBase.h"
#include "../Token.h"
#include "../../Define.h"
#include "../../Debug/Log.h"
#include <vector>
#include <string>
#include <sstream>
#include <limits>

namespace SimpleLanguage {
namespace Compile {

// Forward declarations
class Node;
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
class FileMetaKeyIfSyntax;
class FileMetaCallLink;
class FileMetaSyntax;
class FileMetaClassDefine;
class FileMetaDefineVariableSyntax;
class FileMetaOpAssignSyntax;
class FileMetaKeySwitchSyntax;
class FileMetatUtil;
class StructParse;

enum class EExpressType {
    Common,
    MemberVariable,
    ParamVariable,
};

class FileMetaBaseTerm : public FileMetaBase {
public:
    FileMetaBaseTerm(FileMeta* fm);
    virtual ~FileMetaBaseTerm() = default;

    // Properties
    bool GetDirty() const { return m_IsDirty; }
    void SetDirty(bool value) { m_IsDirty = value; }
    
    int GetPriority() const { return m_Priority; }
    void SetPriority(int value) { m_Priority = value; }
    
    bool IsOnlyOne() const;
    const std::vector<FileMetaBaseTerm*>& GetFileMetaExpressList() const { return m_FileMetaExpressList; }
    std::vector<FileMetaBaseTerm*> SplitParamList();

    void ClearDirty();
    
    FileMetaBaseTerm* GetLeft() const { return m_Left; }
    void SetLeft(FileMetaBaseTerm* value);
    
    FileMetaBaseTerm* GetRight() const { return m_Right; }
    void SetRight(FileMetaBaseTerm* value);
    
    virtual FileMetaBaseTerm* GetRoot() const {return m_Root;}
    virtual void BuildAST() = 0;
    virtual std::string ToFormatString() const override;
    
    // Additional methods used in implementation
    void AddFileMetaTerm(FileMetaBaseTerm* fmn);
    void AddRangeFileMetaTerm(const std::vector<FileMetaBaseTerm*>& fmn);
    std::string ToTokenString() const;

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

    Token* symbolToken() const { return m_SymbolToken; }
    Token* GetToken() const;
    ETokenType GetSymbolType() const;
    void SetPriority();
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;
    std::string ToTokenString() const;

private:
    Token* m_SymbolToken = nullptr;
    Token* m_Token = nullptr;
};

class FileMetaConstValueTerm : public FileMetaBaseTerm {
public:
    FileMetaConstValueTerm(FileMeta* fm, Token* token);
    FileMetaConstValueTerm(FileMeta* fm, Token* valueToken, Token* signalToken);
    virtual ~FileMetaConstValueTerm() = default;

    Token* ValueToken() const { return m_ValueToken; }
    Token* SignalToken() const { return m_SignalToken; }
    Token* GetValueToken() const;
    Token* GetPlusOrMinusToken() const;
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;
    std::string ToTokenString() const;

private:
    Token* m_ValueToken = nullptr;
    Token* m_SignalToken = nullptr;
    Token* m_PlusOrMinusToken = nullptr;
};

class FileMetaCallTerm : public FileMetaBaseTerm {
public:
    FileMetaCallTerm(FileMeta* fm, Node* node);
    virtual ~FileMetaCallTerm() = default;

    Node* CallNode() const { return m_CallNode; }
    FileMetaCallLink* GetCallLink() const;
    Node* GetCallNode() const;
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;
    std::string ToTokenString() const;

private:
    Node* m_CallNode = nullptr;
    FileMetaCallLink* m_CallLink = nullptr;
};

class FileMetaParTerm : public FileMetaBaseTerm {
public:
    FileMetaParTerm(FileMeta* fm, Node* node, EExpressType expressType);
    virtual ~FileMetaParTerm() = default;

    Node* ParNode() const { return m_ParNode; }
    Token* EndToken() const { return m_EndToken; }
    EExpressType ExpressType() const { return m_ExpressType; }
    Node* GetParNode() const;
    Token* GetEndToken() const;
    EExpressType GetExpressType() const;
    void ClearDirty();
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;
    std::string ToTokenString() const;

private:
    Node* m_ParNode = nullptr;
    Token* m_EndToken = nullptr;
    EExpressType m_ExpressType;
    Node* m_Node = nullptr;
};

class FileMetaBraceTerm : public FileMetaBaseTerm {
public:
    FileMetaBraceTerm(FileMeta* fm, Node* node);
    virtual ~FileMetaBraceTerm() = default;

    Node* BraceNode() const { return m_BraceNode; }
    const std::vector<FileMetaSyntax*>& GetFileMetaAssignSyntaxList() const;
    const std::vector<FileMetaCallLink*>& GetFileMetaCallLinkList() const;
    bool IsArray() const;
    void SetIsArray(bool value);
    void HandleBraceTerm();
    void ClearDirty();
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;
    std::string ToTokenString() const;

private:
    Node* m_BraceNode = nullptr;
    Node* m_Node = nullptr;
    Token* m_BraceEndToken = nullptr;
    std::vector<FileMetaSyntax*> m_FileMetaAssignSyntaxList;
    std::vector<FileMetaCallLink*> m_FileMetaCallLinkList;
    bool m_IsArray = false;
};

class FileMetaBracketTerm : public FileMetaBaseTerm {
public:
    FileMetaBracketTerm(FileMeta* fm, Node* node, int bracketType = 0);
    virtual ~FileMetaBracketTerm() = default;

    Node* BracketNode() const { return m_BracketNode; }
    int BracketType() const { return m_BracketType; }
    Token* EndToken() { return m_BracketeEndToken;  }
    Node* GetBracketNode() const;
    int GetBracketType() const;
    Token* GetEndToken() const;
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;
    std::string ToTokenString() const;

private:
    Token* m_BracketeEndToken;
    Node* m_BracketNode = nullptr;
    int m_BracketType = 0;
};

class FileMetaIfSyntaxTerm : public FileMetaBaseTerm {
public:
    FileMetaIfSyntaxTerm(FileMeta* fm, Node* node);
    virtual ~FileMetaIfSyntaxTerm() = default;

    Node* GetIfNode() const;
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;
    std::string ToTokenString() const;

    FileMetaKeyIfSyntax* GetIfSyntax();
    FileMetaKeyIfSyntax* GetIfSyntax() const;
    void SetDeep(int deep);

private:
    Node* m_IfNode = nullptr;
    FileMetaKeyIfSyntax* m_IfSyntax = nullptr;
    int m_Deep = 0;
};

class FileMetaThreeItemSyntaxTerm : public FileMetaBaseTerm {
public:
    FileMetaThreeItemSyntaxTerm(FileMeta* fm, Node* node);
    virtual ~FileMetaThreeItemSyntaxTerm() = default;

    Node* ThreeItemNode() const { return m_ThreeItemNode; }
    Node* GetThreeItemNode() const;
    FileMetaBaseTerm* GetReturn1Term() const;
    void SetReturn1Term(FileMetaBaseTerm* value);
    FileMetaBaseTerm* GetReturn2Term() const;
    void SetReturn2Term(FileMetaBaseTerm* value);
    FileMetaBaseTerm* GetConditionTerm() const;
    void SetConditionTerm(FileMetaBaseTerm* value);
    void SetItemTerm(int i, FileMetaBaseTerm* fmbt);
    void SetDeep(int deep);
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    Node* m_ThreeItemNode = nullptr;
    FileMetaBaseTerm* m_Return1Term = nullptr;
    FileMetaBaseTerm* m_Return2Term = nullptr;
    FileMetaBaseTerm* m_ConditionTerm = nullptr;
    int m_Deep = 0;
};

class FileMetaMatchSyntaxTerm : public FileMetaBaseTerm {
public:
    FileMetaMatchSyntaxTerm(FileMeta* fm, Node* node);
    virtual ~FileMetaMatchSyntaxTerm() = default;

    Node* MatchNode() const { return m_MatchNode; }
    Node* GetMatchNode() const;
    FileMetaKeySwitchSyntax* GetSwitchSyntax() const;
    void SetDeep(int deep);
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    Node* m_MatchNode = nullptr;
    FileMetaKeySwitchSyntax* m_SwitchSyntax = nullptr;
    int m_Deep = 0;
};

class FileMetaTermExpress : public FileMetaBaseTerm {
public:

    FileMetaTermExpress(FileMeta* fm, const std::vector<Node*>& nodeList, EExpressType expressType);
    virtual ~FileMetaTermExpress() = default;

    const std::vector<Node*>& NodeList() const { return m_NodeList; }
    EExpressType ExpressType() const { return m_ExpressType; }
    const std::vector<Node*>& GetNodeList() const;
    EExpressType GetExpressType() const;
    bool GetCanUseDoublePlusOrMinus() const;
    void SetCanUseDoublePlusOrMinus(bool value);
    void SetDeep(int deep);
    void CreateFileMetaExpressByChildList(const std::vector<Node*>& nodeList);
    bool BuildTst(std::vector<FileMetaBaseTerm*>& list);
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;
    std::string ToTokenString() const;

private:
    std::vector<Node*> m_NodeList;
    EExpressType m_ExpressType;
    bool m_CanUseDoublePlusOrMinus = false;
    int m_Deep = 0;
};

} // namespace Compile
} // namespace SimpleLanguage
