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

    struct ParseCurrentNodeInfo 
    {
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

    StructParse(FileMeta& fm, Node& node);
    
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
    FileMetaSyntax* HandleCreateFileMetaSyntaxByPNode(Node* pnode);

private:
    FileMeta& m_FileMeta;
    std::stack<ParseCurrentNodeInfo*> m_CurrentNodeInfoStack;
    Node& m_RootNode;
};

} // namespace Compile
} // namespace SimpleLanguage
