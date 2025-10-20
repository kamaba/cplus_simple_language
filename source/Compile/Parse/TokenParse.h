//****************************************************************************
//  File:      TokenParse.h
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
namespace Parse {

class TokenParse {
public:
    Node* GetRootNode() const { return m_RootNode.get(); }

    TokenParse(FileMeta& fm, const std::vector<std::unique_ptr<Token>>& list);
    
    void BuildStruct();
    void BuildEnd();

private:
    void ParseToken();
    void ParseDetailToken(Token& token);
    
    void AddImportNode(Token& token);
    void AddNamespaceNode(Token& token);
    void AddIdentifier(Token& code);
    void AddAnnotation(Token& code);
    std::unique_ptr<Node> AddKeyNode(Token& token);
    std::unique_ptr<Node> AddAtOpSign(Token& token);
    std::unique_ptr<Node> AddSymbol(Token& token);
    void AddPlusMinus(Token& code);
    void AddDoublePlusMinus(Token& code);
    void AddLeftToRightEqualSymbol(Token& code);
    void AddBitMoveOperatorSymbol(Token& code);
    void AddDXCompareSymbol(Token& code);
    void AddCompareNotOrEqualSymbol(Token& code);
    void AddBitAndSymbol(Token& code);
    void AddBitXOrOpSymbol(Token& code);
    void AddBitOrOpSymbol(Token& code);
    void AddHightComputeSymbol(Token& code);
    void AddSingleSign(Token& code);
    void AddAndCompareSymbol(Token& code);
    void AddOrCompareSymbol(Token& code);

private:
    FileMeta& m_FileMeta;
    const std::vector<std::unique_ptr<Token>>& m_TokensList;
    int m_TokenIndex = 0;
    int m_TokenCount = 0;

    std::unique_ptr<Node> m_RootNode;
    std::stack<Node*> currentNodeStack;
    Node* currentNode = nullptr;
};

} // namespace Parse
} // namespace Compile
} // namespace SimpleLanguage
