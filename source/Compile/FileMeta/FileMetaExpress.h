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

class FileMetaBaseTerm : public FileMetaBase {
public:
    FileMetaBaseTerm(FileMeta* fm);
    virtual ~FileMetaBaseTerm() = default;

    // Properties
    bool GetDirty() const { return m_IsDirty; }
    void SetDirty(bool value) { m_IsDirty = value; }
    
    int GetPriority() const { return m_Priority; }
    void SetPriority(int value) { m_Priority = value; }
    
    bool IsOnlyOne() const { return GetLeft() == nullptr && GetRight() == nullptr; }
    const std::vector<FileMetaBaseTerm*>& fileMetaExpressList() const { return m_FileMetaExpressList; }
    
    FileMetaBaseTerm* GetLeft() const { return m_Left; }
    void setLeft(FileMetaBaseTerm* value) {
        m_Left = value;
        m_IsDirty = true;
    }
    
    FileMetaBaseTerm* GetRight() const { return m_Right; }
    void SetRight(FileMetaBaseTerm* value) {
        m_Right = value;
        m_IsDirty = true;
    }
    
    virtual FileMetaBaseTerm* Root() const;
    virtual void BuildAST() = 0;
    virtual std::string ToFormatString() const override;

protected:
    bool m_IsDirty = false;
    int m_Priority = std::numeric_limits<int>::max();
    std::vector<FileMetaBaseTerm*> m_FileMetaExpressList;
    FileMetaBaseTerm* m_Left = nullptr;
    FileMetaBaseTerm* m_Right = nullptr;
};

class FileMetaSymbolTerm : public FileMetaBaseTerm {
public:
    FileMetaSymbolTerm(FileMeta* fm, Token* token);
    virtual ~FileMetaSymbolTerm() = default;

    Token* symbolToken() const { return m_SymbolToken; }
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    Token* m_SymbolToken = nullptr;
};

class FileMetaConstValueTerm : public FileMetaBaseTerm {
public:
    FileMetaConstValueTerm(FileMeta* fm, Token* token);
    FileMetaConstValueTerm(FileMeta* fm, Token* valueToken, Token* signalToken);
    virtual ~FileMetaConstValueTerm() = default;

    Token* ValueToken() const { return m_ValueToken; }
    Token* SignalToken() const { return m_SignalToken; }
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    Token* m_ValueToken = nullptr;
    Token* m_SignalToken = nullptr;
};

class FileMetaCallTerm : public FileMetaBaseTerm {
public:
    FileMetaCallTerm(FileMeta* fm, Node* node);
    virtual ~FileMetaCallTerm() = default;

    Node* CallNode() const { return m_CallNode; }
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    Node* m_CallNode = nullptr;
};

class FileMetaParTerm : public FileMetaBaseTerm {
public:
    FileMetaParTerm(FileMeta* fm, Node* node, FileMetaTermExpress::EExpressType expressType);
    virtual ~FileMetaParTerm() = default;

    Node* ParNode() const { return m_ParNode; }
    Token* EndToken() const { return m_EndToken; }
    FileMetaTermExpress::EExpressType ExpressType() const { return m_ExpressType; }
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    Node* m_ParNode = nullptr;
    Token* m_EndToken = nullptr;
    FileMetaTermExpress::EExpressType m_ExpressType;
};

class FileMetaBraceTerm : public FileMetaBaseTerm {
public:
    FileMetaBraceTerm(FileMeta* fm, Node* node);
    virtual ~FileMetaBraceTerm() = default;

    Node* BraceNode() const { return m_BraceNode; }
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    Node* m_BraceNode = nullptr;
};

class FileMetaBracketTerm : public FileMetaBaseTerm {
public:
    FileMetaBracketTerm(FileMeta* fm, Node* node, int bracketType = 0);
    virtual ~FileMetaBracketTerm() = default;

    Node* BracketNode() const { return m_BracketNode; }
    int BracketType() const { return m_BracketType; }
    Token* EndToken() { return m_BracketeEndToken;  }
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    Token* m_BracketeEndToken;
    Node* m_BracketNode = nullptr;
    int m_BracketType = 0;
};

class FileMetaIfSyntaxTerm : public FileMetaBaseTerm {
public:
    FileMetaIfSyntaxTerm(FileMeta* fm, Node* node);
    virtual ~FileMetaIfSyntaxTerm() = default;

    Node* IfNode() const { return m_IfNode; }
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    Node* m_IfNode = nullptr;
};

class FileMetaThreeItemSyntaxTerm : public FileMetaBaseTerm {
public:
    FileMetaThreeItemSyntaxTerm(FileMeta* fm, Node* node);
    virtual ~FileMetaThreeItemSyntaxTerm() = default;

    Node* ThreeItemNode() const { return m_ThreeItemNode; }
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    Node* m_ThreeItemNode = nullptr;
};

class FileMetaMatchSyntaxTerm : public FileMetaBaseTerm {
public:
    FileMetaMatchSyntaxTerm(FileMeta* fm, Node* node);
    virtual ~FileMetaMatchSyntaxTerm() = default;

    Node* MatchNode() const { return m_MatchNode; }
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    Node* m_MatchNode = nullptr;
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

    const std::vector<Node*>& NodeList() const { return m_NodeList; }
    EExpressType ExpressType() const { return m_ExpressType; }
    virtual void BuildAST() override;
    virtual std::string ToFormatString() const override;

private:
    std::vector<Node*> m_NodeList;
    EExpressType m_ExpressType;
};

} // namespace Compile
} // namespace SimpleLanguage
