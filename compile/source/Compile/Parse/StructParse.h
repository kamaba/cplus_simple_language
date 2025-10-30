//****************************************************************************
//  File:      StructParse.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include <vector>
#include <memory>
#include <stack>
#include <string>
#include "Node.h"

namespace SimpleLanguage {
namespace Compile {

    class Node;
    class FileMeta;
    class FileMetaNamespace;
    class FileMetaClass;
    class FileMetaMemberData;
    class FileMetaMemberVariable;
    class FileMetaMemberFunction;
    class FileMetaSyntax;

class StructParse {
public:
    enum class EParseNodeType {
        Null,
        File,
        Namespace,
        Class,
        Function,
        Statements,
        DataMemeber
    };

    enum class ESyntaxNodeStructType {
        None = 0,
        KeySyntax,
        CommonSyntax,
    };

    class SyntaxNodeStruct {
    public:
        ESyntaxNodeStructType eSyntaxNodeType = ESyntaxNodeStructType::None;
        ETokenType tokenType = ETokenType::None;
        ENodeType curNodeType = ENodeType::None;
        int moveIndex = 0;
        Node* keyNode = nullptr;
        std::vector<Node*> keyContent;                //关键字和括号内容  if () switch() for()
        Node* blockNode = nullptr;
        std::vector<Node*> commonContent;             //普通调用内容    Class.CalFun()
        std::vector<SyntaxNodeStruct*> childrenKeySyntaxStructList; //关键字嵌套内容, 如switch
        std::vector<SyntaxNodeStruct*> followKeySyntaxStructList; //关键字跟随内容if/elif/elif/else  

        SyntaxNodeStruct() = default;
        void SetMainKeyNode(Node* _keyNode);
        void AddContent(Node* node);
        void SetBraceNode(Node* node);
        bool IsLineEndBreak();
    };

    class Condition {
    public:
        bool isFirstKey = false;
        bool isCheck = true;
        std::vector<ETokenType> eTokenTypeList;

        bool IsMatchTokenType(ETokenType tokenType);
        Condition(ETokenType tokenType);
        void AddTokenTypeList(ETokenType tokenType);
    };

    struct ParseCurrentNodeInfo {
        EParseNodeType parseType;
        FileMeta* codeFile = nullptr;
        FileMetaNamespace* codeNamespace = nullptr;
        FileMetaClass* codeClass = nullptr;
        FileMetaMemberData* codeData = nullptr;
        FileMetaMemberFunction* codeFunction = nullptr;
        FileMetaSyntax* codeSyntax = nullptr;

        ParseCurrentNodeInfo(FileMeta* cf);
        ParseCurrentNodeInfo(FileMetaNamespace* nsn);
        ParseCurrentNodeInfo(FileMetaClass* nsc);
        ParseCurrentNodeInfo(FileMetaMemberData* fmmd);
        ParseCurrentNodeInfo(FileMetaMemberFunction* nsf);
        ParseCurrentNodeInfo(FileMetaSyntax* nss);
    };

    StructParse(FileMeta* fm, Node* node);
    virtual ~StructParse() = default;
    
    Node* GetRootNode() { return m_RootNode; }
    void ParseRootNodeToFileMeta();
    std::vector<Node*> GetAllNodeToSemiColon(Node* pnode, bool isAddSelf = false);
    void ParseImport(Node* pnode);
    void ParseNamespace(Node* pnode);
    static bool CheckEnd(Node* pnode);
    void ParseNamespaceOrTopClass(Node* pnode);
    void ParseClassNode(Node* pnode);
    void ParseDataBracketNode(Node* bracketNode);
    void ParseDataNode(Node* pnode);
    void ParseEnumNode(Node* pnode);
    void ParseSyntax(Node* pnode);
    
    SyntaxNodeStruct GetOneSyntax(Node* pnode, Condition* condition = nullptr);
    FileMetaSyntax* HandleCreateFileMetaSyntaxByPNode(Node* pnode);
    FileMetaSyntax* CrateFileMetaSyntaxNoKey(const std::vector<Node*>& pNodeList);
    
    static std::vector<Node*> HandleBeforeNode(Node* node);
    static std::vector<Node*> HandleExpressNode(Node* node);
    static void DelHandleNostList(Node* node);
    static bool IsCommonExpressNode(Node* node);
    static void _HandleExpressNodeProcess(Node* node, Node* inputFinaleNode = nullptr);
    static void HandleAngleExpressNode(Node* node, Node* parentNode);

protected:
    ParseCurrentNodeInfo* GetCurrentNodeInfo();
    void AddParseFileNodeInfo();
    void AddParseNamespaceNodeInfo(FileMetaNamespace* fmn);
    void AddParseClassNodeInfo(FileMetaClass* fmc);
    void AddParseVariableInfo(FileMetaMemberVariable* csv);
    void AddParseDataInfo(FileMetaMemberData* fmmd);
    void AddParseFunctionNodeInfo(FileMetaMemberFunction* fmmf, bool isPush = true);
    void AddParseSyntaxNodeInfo(FileMetaSyntax* fms, bool isAddParseCurrentNNode = false);
    
    void AddFileMetaFunctionVariable(Node* pnode, Node* blockNode, const std::vector<Node*>& nodeList);
    void AddFileMetaClasss(Node* blockNode, const std::vector<Node*>& nodeList);

private:
    FileMeta* m_FileMeta;
    std::stack<ParseCurrentNodeInfo*> m_CurrentNodeInfoStack;
    Node* m_RootNode;
};

} // namespace Compile
} // namespace SimpleLanguage