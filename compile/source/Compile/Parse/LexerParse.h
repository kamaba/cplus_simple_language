//****************************************************************************
//  File:      LexerParse.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: word lexer parse to token
//****************************************************************************

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../../Define.h"

namespace SimpleLanguage {
namespace Compile {

class Token;
class LexerParse {
public:
    LexerParse(const std::string& path, const std::string& buffer);
    
    Token* GetCurrentToken() const { return m_CurrentToken; }
    const std::vector<Token*>& GetListTokens() const { return m_ListTokens; }
    std::vector<Token*> GetListTokensWidthEnd();
    
    void SetSourcePosition(int line, int character);
    void ParseToTokenList();

private:
    const char END_CHAR = 0xFF; // 结尾字符
    
    char ReadChar();
    char PeekChar();
    void UndoChar();
    void AddLine();
    
    void AddToken(ETokenType type);
    void AddToken(ETokenType type, const std::string& lexeme);
    void AddToken(ETokenType type, const std::string& lexeme, const std::string& extend);
    void AddToken(ETokenType type, const std::string& lexeme, EType extend);
    void AddToken(ETokenType type, const std::string& lexeme, int sourceLine, int sourceChar);
    void AddToken(ETokenType type, const std::string& lexeme, EType extend, int sourceLine, int sourceChar);
    void AddChildrenToken(ETokenType type, const std::string& lexeme);
    void AddChildrenToken(Token* token);
    
    bool IsHexDigit(char c);
    bool IsIdentifier(char ch);
    
    // 各种符号的读取方法
    void ReadPlus();
    void ReadMinus();
    void ReadMultiply();
    void ReadDivide();
    void ReadModulo();
    void ReadAnd();
    void ReadOr();
    void ReadNot();
    void ReadAssign();
    void ReadGreater();
    void ReadLess();
    void ReadXor();
    void ReadNumber();
    void ReadNumberOrHexOrOctOrBinNumber();
    void ReadQuestionMark();
    void ReadAt();
    void ReadRightBrace();
    void ReadOrigenString();
    void ReadString(bool isWithAtOp);
    void SharpLevel(int topLevel);
    void ReadDollar();
    void ReadSharp();
    void ReadIdentifier();

private:
    char m_CurChar;                              // 当前字符
    char m_TempChar;                             // 临时字符
    std::string m_Builder;                       // 字符串构建器
    std::vector<Token*> m_ListTokens;
    Token* m_CurrentToken;
    std::string m_Buffer;
    int m_Length;
    int m_SourceLine;                            // 解析到当前的行数
    int m_SourceChar;                            // 解析到当前行中的位置
    int m_Index;
    std::string m_Path;
};

} // namespace Compile
} // namespace SimpleLanguage
