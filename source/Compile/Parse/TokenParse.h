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
#include "../Token.h"

namespace SimpleLanguage {
namespace Compile {

    class Node;
    class FileMeta;

class TokenParse {
public:
    Node* GetRootNode() const { return m_RootNode; }

    TokenParse(FileMeta* fm, const std::vector<Token*>& list);
    
    void BuildStruct();
    void BuildEnd();

private:
    void ParseToken();
    void ParseDetailToken(Token& token);
    
    void AddImportNode(Token& token);
    void AddNamespaceNode(Token& token);
    void AddIdentifier(Token& code);
    void AddAnnotation(Token& code);
    Node* AddKeyNode(Token& token);
    Node* AddAtOpSign(Token& token);
    Node* AddSymbol(Token& token);
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
    FileMeta* m_FileMeta;
    std::vector<Token*>& m_TokensList;
    int m_TokenIndex = 0;
    int m_TokenCount = 0;

    Node* m_RootNode;
    std::stack<Node*> currentNodeStack;
    Node* currentNode = nullptr;
};

} // namespace Compile
} // namespace SimpleLanguage
