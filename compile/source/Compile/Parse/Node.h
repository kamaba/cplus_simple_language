//****************************************************************************
//  File:      Node.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include <vector>
#include <memory>
#include <string>
#include "../Token.h"

namespace SimpleLanguage {
namespace Compile {

class SignComputePriority {
public:
    static const int Level1 = 1;                         // (a+b) [] . 优先操作，对象操作等
    static const int Level2_LinkOp = 2;                  // -负号 (int)强转 ++x x++ -- ! ~ 
    static const int Level3_Hight_Compute = 3;          // / * % 
    static const int Level3_Low_Compute = 4;            // + - 
    static const int Level5_BitMoveOp = 5;              // << >> 
    static const int Level6_Compare = 6;                // < > <= >=
    static const int Level7_EqualAb = 7;                // == !=
    static const int Level8_BitAndOp = 81;              // &
    static const int Level8_BitXOrOp = 82;               // ^
    static const int Level8_BitOrOp = 83;                // |
    static const int Level9_And = 91;                    // &&
    static const int Level9_Or = 92;                     // ||
    static const int Level10_ThirdOp = 100;             // ? : 
    static const int Level11_Assign = 120;              // = /= *= %= += -= <<= >>= &= ^= |= 
    static const int Level12_Split = 130;                // ,
};

enum class ENodeType {
    None,
    Root,
    Brace,
    LeftAngle,
    RightAngle,
    Par,
    Bracket,
    Symbol,
    Period,
    Comma,
    SemiColon,
    LineEnd,
    Assign,
    ConstValue,
    PrefixAtSymbol,
    PrefixDollerSymbol,
    IdentifierLink,
    Key,
    End
};
using namespace std;
class Node {
public:

    Token* token = nullptr;              // 
    Token* endToken = nullptr;           // )}]
    Node* parent = nullptr;                              // 父节点
    Node* parNode = nullptr;             // (小括号的节点
    Node* blockNode = nullptr;           // {大括号的节点
    Node* bracketNode = nullptr;         // [中括号的节点
    Node* angleNode = nullptr;
    Token* linkToken = nullptr;                    // .节点
    Token* atToken = nullptr;                      // $节点
    Node* lastNode = nullptr;            // 最后处理的节点

    ENodeType nodeType = ENodeType::None;
    ::std::vector<Node*> m_ExtendLinkNodeList;
    ::std::vector<Node*> childList;       // 子内容节点

    int parseIndex = 0;

    Node(Token& token);
    Node(Token* token);
    
    Node* GetParseCurrent();
    Node* GetFinalNode();
    const ::std::vector<Node*>& GetExtendLinkNodeList() const { return m_ExtendLinkNodeList; }
    ::std::vector<Node*> GetLinkNodeList(bool isIncludeSelf = true);
    ::std::vector<Token*> GetLinkTokenList();

    int GetPriority() { return m_Priority; }
    void SetPriority(int _pri) { m_Priority = _pri; }
    int GetIsDel() { return m_IsDel; }
    void SetIsDel(bool _del) { m_IsDel = _del; }
    Node* GetParseNode(int index = 1, bool isAddIndex = true);
    void AddLinkNode(Node* node);
    void SetLinkNode(const ::std::vector<Node*>& nodeList);
    void AddChild(Node* c, bool setParent = true);
    void AddSyntax(Node* c);
    
    ::std::string ToString() const;
    ::std::string ToFormatString() const;

private:
    int m_Priority = -1;
    bool m_IsDel = false;
    void SetLinkNode(::std::vector<Node*>&& nodeList);
};

} // namespace Compile
} // namespace SimpleLanguage
