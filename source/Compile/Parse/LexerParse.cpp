//****************************************************************************
//  File:      LexerParse.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: word lexer parse to token
//****************************************************************************

#include "LexerParse.h"
#include <cctype>
#include <sstream>
#include <algorithm>

namespace SimpleLanguage {
namespace Compile {
namespace Parse {

LexerParse::LexerParse(const std::string& path, const std::string& buffer)
    : m_Path(path), m_Buffer(buffer), m_Length(static_cast<int>(buffer.length())),
      m_SourceChar(0), m_SourceLine(0), m_Index(0) {
}

std::vector<std::unique_ptr<Token>> LexerParse::GetListTokensWidthEnd() {
    std::vector<std::unique_ptr<Token>> withEndList;
    for (auto& token : m_ListTokens) {
        withEndList.push_back(std::make_unique<Token>(*token));
    }
    withEndList.push_back(std::make_unique<Token>(m_Path, ETokenType::Finished, END_CHAR, m_SourceLine, m_SourceChar));
    return withEndList;
}

void LexerParse::SetSourcePosition(int line, int character) {
    m_SourceLine = line;
    m_SourceChar = character;
}

char LexerParse::ReadChar() {
    ++m_Index;
    ++m_SourceChar;
    if (m_Index < m_Length) {
        return m_Buffer[m_Index];
    } else if (m_Index == m_Length) {
        return END_CHAR;
    }
    return END_CHAR;
}

char LexerParse::PeekChar() {
    int index = m_Index + 1;
    if (index < m_Length) {
        return m_Buffer[index];
    } else if (index == m_Length) {
        return END_CHAR;
    }
    return static_cast<char>(0);
}

void LexerParse::UndoChar() {
    if (m_Index == 0) {
        CompileManager::GetInstance().AddCompileError("Error Cannot undo char beyond start of source.");
        return;
    }
    --m_Index;
    --m_SourceChar;
}

void LexerParse::AddLine() {
    m_SourceChar = 0;
    ++m_SourceLine;
}

void LexerParse::AddToken(ETokenType type) {
    AddToken(type, std::string(1, m_CurChar));
}

void LexerParse::AddToken(ETokenType type, const std::string& lexeme) {
    AddToken(type, lexeme, m_SourceLine, m_SourceChar);
}

void LexerParse::AddToken(ETokenType type, const std::string& lexeme, EType extend) {
    AddToken(type, lexeme, extend, m_SourceLine, m_SourceChar);
}

void LexerParse::AddToken(ETokenType type, const std::string& lexeme, int sourceLine, int sourceChar) {
    m_CurrentToken = std::make_unique<Token>(m_Path, type, lexeme, sourceLine, sourceChar);
    m_ListTokens.push_back(std::move(m_CurrentToken));
    m_Builder.clear();
}

void LexerParse::AddToken(ETokenType type, const std::string& lexeme, EType extend, int sourceLine, int sourceChar) {
    m_CurrentToken = std::make_unique<Token>(m_Path, type, lexeme, sourceLine, sourceChar, extend);
    m_ListTokens.push_back(std::move(m_CurrentToken));
    m_Builder.clear();
}

void LexerParse::AddChildrenToken(ETokenType type, const std::string& lexeme) {
    auto token = std::make_unique<Token>(m_Path, type, lexeme, m_SourceLine, m_SourceChar);
    AddChildrenToken(std::move(token));
}

void LexerParse::AddChildrenToken(std::unique_ptr<Token> token) {
    if (m_CurrentToken != nullptr) {
        m_CurrentToken->AddChildrenToken(std::move(token));
    }
}

bool LexerParse::IsHexDigit(char c) {
    if (std::isdigit(c)) return true;
    if (c >= 'a' && c <= 'f') return true;
    if (c >= 'A' && c <= 'F') return true;
    return false;
}

bool LexerParse::IsIdentifier(char ch) {
    return (ch == '_' || std::isalnum(ch));
}

void LexerParse::ReadPlus() {
    m_TempChar = ReadChar();
    if (m_TempChar == '=') {
        AddToken(ETokenType::PlusAssign, "+=");
    } else if (m_TempChar == '+') {
        AddToken(ETokenType::DoublePlus, "++");
    } else {
        AddToken(ETokenType::Plus, "+");
        UndoChar();
    }
}

void LexerParse::ReadMinus() {
    m_TempChar = ReadChar();
    if (m_TempChar == '=') {
        AddToken(ETokenType::MinusAssign, "-=");
    } else if (m_TempChar == '-') {
        AddToken(ETokenType::DoubleMinus, "--");
    } else {
        AddToken(ETokenType::Minus, "-");
        UndoChar();
    }
}

void LexerParse::ReadMultiply() {
    m_TempChar = ReadChar();
    if (m_TempChar == '=') {
        AddToken(ETokenType::MultiplyAssign, "*=");
    } else {
        AddToken(ETokenType::Multiply, "*");
        UndoChar();
    }
}

void LexerParse::ReadDivide() {
    m_TempChar = ReadChar();
    if (m_TempChar == '=') {
        AddToken(ETokenType::DivideAssign, "/=");
    } else {
        AddToken(ETokenType::Divide, "/");
        UndoChar();
    }
}

void LexerParse::ReadModulo() {
    m_TempChar = ReadChar();
    if (m_TempChar == '=') {
        AddToken(ETokenType::ModuloAssign, "%=");
    } else {
        AddToken(ETokenType::Modulo, "%");
        UndoChar();
    }
}

void LexerParse::ReadAnd() {
    m_TempChar = ReadChar();
    if (m_TempChar == '&') {
        AddToken(ETokenType::And, "&&");
    } else if (m_TempChar == '=') {
        AddToken(ETokenType::CombineAssign, "&=");
    } else {
        AddToken(ETokenType::Combine, "&");
        UndoChar();
    }
}

void LexerParse::ReadOr() {
    m_TempChar = ReadChar();
    if (m_TempChar == '|') {
        AddToken(ETokenType::Or, "||");
    } else if (m_TempChar == '=') {
        AddToken(ETokenType::InclusiveOrAssign, "|=");
    } else {
        AddToken(ETokenType::InclusiveOr, "|");
        UndoChar();
    }
}

void LexerParse::ReadNot() {
    m_TempChar = ReadChar();
    if (m_TempChar == '=') {
        if (ReadChar() == '=') {
            AddToken(ETokenType::ValueNotEqual, "!==");
        } else {
            AddToken(ETokenType::NotEqual, "!=");
            UndoChar();
        }
    } else {
        AddToken(ETokenType::Not, "!");
        UndoChar();
    }
}

void LexerParse::ReadAssign() {
    m_TempChar = ReadChar();
    if (m_TempChar == '=') {
        if (ReadChar() == '=') {
            AddToken(ETokenType::ValueEqual, "===");
        } else {
            AddToken(ETokenType::Equal, "==");
            UndoChar();
        }
    } else if (m_TempChar == '>') {
        AddToken(ETokenType::Lambda, "=>");
    } else {
        AddToken(ETokenType::Assign, "=");
        UndoChar();
    }
}

void LexerParse::ReadGreater() {
    m_TempChar = ReadChar();
    if (m_TempChar == '=') {
        AddToken(ETokenType::GreaterOrEqual, ">=");
    } else if (m_TempChar == '>') {
        AddToken(ETokenType::Shr, ">>");
    } else {
        AddToken(ETokenType::Greater, ">");
        UndoChar();
    }
}

void LexerParse::ReadLess() {
    m_TempChar = ReadChar();
    if (m_TempChar == '=') {
        AddToken(ETokenType::LessOrEqual, "<=");
    } else if (m_TempChar == '<') {
        AddToken(ETokenType::Shi, "<<");
        UndoChar();
    } else {
        AddToken(ETokenType::Less, "<");
        UndoChar();
    }
}

void LexerParse::ReadXor() {
    if (ReadChar() == '=') {
        AddToken(ETokenType::XORAssign, "^=");
    } else {
        AddToken(ETokenType::XOR, "^");
        UndoChar();
    }
}

void LexerParse::ReadQuestionMark() {
    switch (ReadChar()) {
        default:
            AddToken(ETokenType::QuestionMark, "?");
            UndoChar();
            return;
    }
}

void LexerParse::ReadAt() {
    char ch = ReadChar();
    if (std::isdigit(ch) || std::isalpha(ch)) {
        AddToken(ETokenType::At, "@");
    } else {
        // Debug.Write("Error 不允许@后边加其它符号!!");
    }
}

void LexerParse::ReadRightBrace() {
    AddToken(ETokenType::RightBrace, "}");
}

void LexerParse::ReadOrigenString() {
    m_Builder.clear();
    do {
        m_TempChar = ReadChar();
        if (m_TempChar == '\'') {
            AddToken(ETokenType::String, m_Builder, EType::String);
            m_Index++;
            m_SourceChar++;
            break;
        } else {
            m_Builder += m_TempChar;
        }
    } while (true);
}

void LexerParse::ReadString(bool isWithAtOp) {
    std::string stringBuilder = m_Builder;
    AddToken(ETokenType::String, "");
    
    do {
        m_TempChar = ReadChar();
        if (m_TempChar == '\\') {
            m_TempChar = ReadChar();
            switch (m_TempChar) {
                case '\'': m_Builder += '\''; break;
                case '\"': m_Builder += '\"'; break;
                case '\\': m_Builder += '\\'; break;
                case '$': m_Builder += '$'; break;
                case 'a': m_Builder += '\a'; break;
                case 'b': m_Builder += '\b'; break;
                case 'f': m_Builder += '\f'; break;
                case 'n': m_Builder += '\n'; break;
                case 'r': m_Builder += '\r'; break;
                case 't': m_Builder += '\t'; break;
                case 'v': m_Builder += '\v'; break;
                case '0': m_Builder += '\0'; break;
                case '/': m_Builder += "/"; break;
                case '{': m_Builder += "{"; break;
                case '}': m_Builder += "}"; break;
                case 'u': {
                    std::string hex;
                    for (int i = 0; i < 4; i++) {
                        hex += ReadChar();
                    }
                    m_Builder += static_cast<char>(std::stoi(hex, nullptr, 16));
                    break;
                }
                default:
                    // Debug.Write("Error 读字符的时候，不支持当前的符号!! : |" + m_CurChar);
                    break;
            }
        } else if (m_TempChar == '\n') {
            // Debug.Write("Error NotInterrupt 读字符的时候，不允许换行，请使用/r/t 一类的换行符!!");
            m_Builder += m_TempChar;
        } else if (m_TempChar == '"') {
            stringBuilder += m_Builder;
            m_CurrentToken->SetLexeme(stringBuilder);
            m_Index++;
            m_SourceChar++;
            break;
        } else {
            m_Builder += m_TempChar;
        }
    } while (true);
}

void LexerParse::SharpLevel(int topLevel) {
    m_Builder.clear();
    char schar = 0;
    int checkBracket = 1;
    bool isBracket = false;
    std::string bracketStringBuild;
    
    while (true) {
        int index = m_Index + checkBracket;
        if (index < m_Length) {
            schar = m_Buffer[index];
        } else {
            // Debug.Write("读取Sharp中[]内容出错!!!");
            break;
        }

        if (checkBracket == 1) {
            if (schar == '[') {
                isBracket = true;
            } else {
                break;
            }
        } else {
            if (isBracket) {
                if (schar == ']') {
                    break;
                } else {
                    bracketStringBuild += schar;
                }
            } else {
                break;
            }
        }
        checkBracket++;
    }
    
    int offset = m_Index++;
    if (isBracket) {
        offset = offset + checkBracket + 1;
    }

    int offset2 = 0;
    int curTopLevel = 0;
    bool isEnd = false;
    
    while (true) {
        if (offset >= m_Length) {
            // Debug.Write("注释没有结尾!!");
            break;
        }
        
        m_TempChar = m_Buffer[offset];
        if (m_TempChar == '!') {
            offset2 = 1;
            curTopLevel = topLevel;
            while (true) {
                schar = m_Buffer[offset + offset2++];
                if (schar == '#') {
                    if (offset + offset2 >= m_Length) {
                        isEnd = true;
                        break;
                    }
                    curTopLevel--;
                    if (curTopLevel <= 0) {
                        if (m_Buffer[offset + offset2] == '#') {
                            break;
                        }
                        isEnd = true;
                        break;
                    }
                } else {
                    break;
                }
            }
            if (!isEnd) {
                m_Builder += m_TempChar;
            }
        } else {
            if (m_TempChar == '\n') {
                m_SourceLine++;
            }
            m_Builder += m_TempChar;
        }
        if (isEnd) {
            m_Index = offset + 1;
            break;
        }
        offset++;
    }

    AddToken(ETokenType::Sharp, m_Builder, bracketStringBuild);
}

void LexerParse::ReadDollar() {
    char ch = ReadChar();
    if (ch == '"') {
        ReadString(true);
    } else if (std::isdigit(ch) || std::isalpha(ch)) {
        AddToken(ETokenType::Dollar, "$");
    } else {
        // Debug.Write("Error 不允许@后边加其它符号!!");
    }
}

void LexerParse::ReadSharp() {
    int topLevel = 1;
    while (true) {
        m_TempChar = ReadChar();
        if (m_TempChar == '!') {
            SharpLevel(topLevel);
            break;
        } else if (m_TempChar == '#') {
            topLevel++;
            m_Builder += m_TempChar;
        } else {
            do {
                if (m_TempChar != '\n') {
                    m_Builder += m_TempChar;
                } else {
                    break;
                }
                m_TempChar = ReadChar();
                if (m_TempChar == END_CHAR) {
                    break;
                }
            } while (true);
            AddToken(ETokenType::Sharp, m_Builder, "#");
            m_Index++;
            m_SourceChar++;
            m_SourceLine++;
            m_Builder.clear();
            break;
        }
    }
}

void LexerParse::ReadIdentifier() {
    m_Builder += m_CurChar;
    do {
        m_TempChar = ReadChar();
        if (IsIdentifier(m_TempChar)) {
            m_Builder += m_TempChar;
        } else {
            UndoChar();
            break;
        }
    } while (true);
    
    ETokenType tokenType;
    EType extend = EType::None;
    
    if (m_Builder == "import") {
        tokenType = ETokenType::Import;
    } else if (m_Builder == "as") {
        tokenType = ETokenType::As;
    } else if (m_Builder == "is") {
        tokenType = ETokenType::Is;
    } else if (m_Builder == "namespace") {
        tokenType = ETokenType::Namespace;
    } else if (m_Builder == "class") {
        tokenType = ETokenType::Class;
        extend = EType::Class;
    } else if (m_Builder == "extends") {
        tokenType = ETokenType::Extends;
    } else if (m_Builder == "enum") {
        tokenType = ETokenType::Enum;
        extend = EType::Enum;
    } else if (m_Builder == "data") {
        tokenType = ETokenType::Data;
        extend = EType::Data;
    } else if (m_Builder == "dynamic") {
        tokenType = ETokenType::Dynamic;
    } else if (m_Builder == "void") {
        tokenType = ETokenType::Void;
    } else if (m_Builder == "object" || m_Builder == "Object") {
        tokenType = ETokenType::Type;
        extend = EType::Object;
    } else if (m_Builder == "byte") {
        tokenType = ETokenType::Type;
        extend = EType::Byte;
    } else if (m_Builder == "sbyte") {
        tokenType = ETokenType::Type;
        extend = EType::SByte;
    } else if (m_Builder == "short") {
        tokenType = ETokenType::Type;
        extend = EType::Int16;
    } else if (m_Builder == "ushort") {
        tokenType = ETokenType::Type;
        extend = EType::UInt16;
    } else if (m_Builder == "int") {
        tokenType = ETokenType::Type;
        extend = EType::Int32;
    } else if (m_Builder == "uint") {
        tokenType = ETokenType::Type;
        extend = EType::UInt32;
    } else if (m_Builder == "bool") {
        tokenType = ETokenType::Type;
        extend = EType::Boolean;
    } else if (m_Builder == "long") {
        tokenType = ETokenType::Type;
        extend = EType::Int64;
    } else if (m_Builder == "ulong") {
        tokenType = ETokenType::Type;
        extend = EType::UInt64;
    } else if (m_Builder == "half") {
        tokenType = ETokenType::Type;
        extend = EType::Float16;
    } else if (m_Builder == "float") {
        tokenType = ETokenType::Type;
        extend = EType::Float32;
    } else if (m_Builder == "double") {
        tokenType = ETokenType::Type;
        extend = EType::Float64;
    } else if (m_Builder == "string") {
        tokenType = ETokenType::Type;
        extend = EType::String;
    } else if (m_Builder == "get") {
        tokenType = ETokenType::Get;
    } else if (m_Builder == "set") {
        tokenType = ETokenType::Set;
    } else if (m_Builder == "if") {
        tokenType = ETokenType::If;
    } else if (m_Builder == "elif") {
        tokenType = ETokenType::ElseIf;
    } else if (m_Builder == "else") {
        tokenType = ETokenType::Else;
    } else if (m_Builder == "while") {
        tokenType = ETokenType::While;
    } else if (m_Builder == "dowhile") {
        tokenType = ETokenType::DoWhile;
    } else if (m_Builder == "const") {
        tokenType = ETokenType::Const;
    } else if (m_Builder == "mut") {
        tokenType = ETokenType::Mut;
    } else if (m_Builder == "final") {
        tokenType = ETokenType::Final;
    } else if (m_Builder == "static") {
        tokenType = ETokenType::Static;
    } else if (m_Builder == "partial") {
        tokenType = ETokenType::Partial;
    } else if (m_Builder == "for") {
        tokenType = ETokenType::For;
    } else if (m_Builder == "in") {
        tokenType = ETokenType::In;
    } else if (m_Builder == "switch") {
        tokenType = ETokenType::Switch;
    } else if (m_Builder == "case") {
        tokenType = ETokenType::Case;
    } else if (m_Builder == "default") {
        tokenType = ETokenType::Default;
    } else if (m_Builder == "next") {
        tokenType = ETokenType::Next;
    } else if (m_Builder == "continue") {
        tokenType = ETokenType::Continue;
    } else if (m_Builder == "break") {
        tokenType = ETokenType::Break;
    } else if (m_Builder == "goto") {
        tokenType = ETokenType::Goto;
    } else if (m_Builder == "extern") {
        tokenType = ETokenType::Extern;
    } else if (m_Builder == "public") {
        tokenType = ETokenType::Public;
    } else if (m_Builder == "protected") {
        tokenType = ETokenType::Projected;
    } else if (m_Builder == "private") {
        tokenType = ETokenType::Private;
    } else if (m_Builder == "interface") {
        tokenType = ETokenType::Interface;
    } else if (m_Builder == "override") {
        tokenType = ETokenType::Override;
    } else if (m_Builder == "params") {
        tokenType = ETokenType::Params;
    } else if (m_Builder == "tr") {
        tokenType = ETokenType::Transience;
    } else if (m_Builder == "ret") {
        tokenType = ETokenType::Return;
    } else if (m_Builder == "label") {
        tokenType = ETokenType::Label;
    } else if (m_Builder == "global") {
        tokenType = ETokenType::Global;
    } else if (m_Builder == "try") {
        tokenType = ETokenType::Try;
    } else if (m_Builder == "catch") {
        tokenType = ETokenType::Catch;
    } else if (m_Builder == "throw") {
        tokenType = ETokenType::Throw;
    } else if (m_Builder == "null") {
        tokenType = ETokenType::Null;
    } else if (m_Builder == "var") {
        tokenType = ETokenType::Var;
    } else if (m_Builder == "this") {
        tokenType = ETokenType::This;
    } else if (m_Builder == "base") {
        tokenType = ETokenType::Base;
    } else if (m_Builder == "true" || m_Builder == "false") {
        tokenType = ETokenType::BoolValue;
        extend = EType::Boolean;
    } else if (m_Builder == "new") {
        tokenType = ETokenType::New;
    } else if (m_Builder == "range") {
        tokenType = ETokenType::Identifier;
        extend = EType::Range;
    } else if (m_Builder == "array") {
        tokenType = ETokenType::Identifier;
        extend = EType::Array;
    } else {
        tokenType = ETokenType::Identifier;
    }

    if (tokenType == ETokenType::Null) {
        AddToken(tokenType, "null", m_SourceLine, m_SourceChar);
    } else {
        AddToken(tokenType, m_Builder, extend, m_SourceLine, m_SourceChar);
    }
}

void LexerParse::ReadNumber() {
    m_Builder += m_CurChar;

    int endPoint = 0;
    char tfrontChar = 0;
    do {
        m_TempChar = ReadChar();
        if (std::isdigit(m_TempChar)) {
            m_Builder += m_TempChar;
            tfrontChar = m_TempChar;
            continue;
        } else if (m_TempChar == '.') {
            endPoint++;
            m_Builder += m_TempChar;
        } else if (m_TempChar == 's') {
            if (endPoint == 0) { // 1s
                AddToken(ETokenType::Number, m_Builder, EType::Int16);
                break;
            } else if (endPoint == 1) { // 1.s
                m_Builder.pop_back();
                AddToken(ETokenType::Number, m_Builder, EType::Int32);
                UndoChar();
                UndoChar();
                break;
            }
        } else if (m_TempChar == 'i') {
            if (endPoint == 0) { // 13i
                AddToken(ETokenType::Number, m_Builder, EType::Int32);
                break;
            } else if (endPoint == 1) { // 1.i
                m_Builder.pop_back();
                AddToken(ETokenType::Number, m_Builder, EType::Int32);
                UndoChar();
                UndoChar();
                break;
            }
        } else if (m_TempChar == 'f') {
            if (endPoint == 0) { // 2f
                Log::GetInstance().AddInHandleToken(m_Path, m_SourceLine, m_SourceChar, EError::None, "读取浮点形必须有小数点!!!");
                AddToken(ETokenType::Number, m_Builder, EType::Float32);
                break;
            } else if (endPoint == 1) {
                if (std::isdigit(tfrontChar)) { // 2.0f.
                    AddToken(ETokenType::Number, m_Builder, EType::Float32);
                    break;
                } else { // 2.f
                    m_Builder.pop_back();
                    AddToken(ETokenType::Number, m_Builder, EType::Int32);
                    UndoChar();
                    UndoChar();
                    break;
                }
            }
        } else if (m_TempChar == 'd') {
            if (endPoint == 1) {
                if (std::isdigit(tfrontChar)) {
                    AddToken(ETokenType::Number, m_Builder, EType::Float64);
                    break;
                } else {
                    m_Builder.pop_back();
                    AddToken(ETokenType::Number, m_Builder, EType::Int32);
                    UndoChar();
                    UndoChar();
                    break;
                }
            }
        } else if (m_TempChar == 'L') {
            if (endPoint == 0) { // 13L
                AddToken(ETokenType::Number, m_Builder, EType::Int64);
                break;
            } else if (endPoint == 1) { // 1.L
                m_Builder.pop_back();
                AddToken(ETokenType::Number, m_Builder, EType::Int32);
                UndoChar();
                UndoChar();
                break;
            }
        } else if (m_TempChar == 'u') {
            char m_TempChar2 = ReadChar();
            if (m_TempChar2 == 's') { // 1us
                if (endPoint == 0) { // 1us
                    AddToken(ETokenType::Number, m_Builder, EType::UInt16);
                } else if (endPoint == 1) { // 1.us
                    m_Builder.erase(m_Builder.length() - 2, 2);
                    AddToken(ETokenType::Number, m_Builder, EType::Int32);
                    UndoChar();
                    UndoChar();
                    UndoChar();
                    break;
                }
            } else if (m_TempChar2 == 'i') {
                if (endPoint == 0) {
                    AddToken(ETokenType::Number, m_Builder, EType::UInt32);
                } else if (endPoint == 1) {
                    m_Builder.erase(m_Builder.length() - 2, 2);
                    AddToken(ETokenType::Number, m_Builder, EType::Int32);
                    UndoChar();
                    UndoChar();
                    UndoChar();
                    break;
                }
            } else if (m_TempChar2 == 'L') {
                if (endPoint == 0) {
                    AddToken(ETokenType::Number, m_Builder, EType::UInt64);
                } else if (endPoint == 1) {
                    m_Builder.erase(m_Builder.length() - 2, 2);
                    AddToken(ETokenType::Number, m_Builder, EType::Int32);
                    UndoChar();
                    UndoChar();
                    UndoChar();
                    break;
                }
            } else {
                UndoChar();
            }
            break;
        } else {
            if (endPoint > 2) {
                // Debug.Write("Error ReadNumber ... !!!");
            } else if (endPoint == 2) {
                AddToken(ETokenType::NumberArrayLink, m_Builder, EType::Array);
                UndoChar();
            } else if (endPoint == 1) {
                if (std::isalpha(m_TempChar)) {
                    char frontChar = m_Builder[m_Builder.length() - 1];
                    if (frontChar == '.') {
                        // Debug.Write("Error 不允许直接使用 number.function的方式，而是必须使用数据识别符才可以使用，例: 2.0f.ToString()");
                        m_Builder.pop_back();
                        AddToken(ETokenType::Number, m_Builder, EType::Int32);
                        AddToken(ETokenType::Period, std::string(1, frontChar));
                        UndoChar();
                    } else {
                        AddToken(ETokenType::Number, m_Builder, EType::Float32);
                        UndoChar();
                    }
                } else {
                    AddToken(ETokenType::Number, m_Builder, EType::Float32);
                    UndoChar();
                }
            } else {
                AddToken(ETokenType::Number, m_Builder, EType::Int32);
                UndoChar();
            }
            break;
        }
        tfrontChar = m_TempChar;
    } while (true);

    m_Index++;
    m_SourceChar++;
}

void LexerParse::ReadNumberOrHexOrOctOrBinNumber() {
    char t = ReadChar();
    if (t == 'x') {
        do {
            m_TempChar = ReadChar();
            if (IsHexDigit(m_TempChar)) {
                m_Builder += m_TempChar;
            } else if (m_TempChar == '_') {
                // 忽略下划线
            } else {
                if (m_Builder.empty()) {
                    m_Builder = "0";
                }
                AddToken(ETokenType::Number, std::to_string(std::stoi(m_Builder, nullptr, 16)), EType::Int32);
                break;
            }
        } while (true);
    } else if (t == 'o') {
        do {
            m_TempChar = ReadChar();
            if (m_TempChar >= '0' && m_TempChar <= '7') {
                m_Builder += m_TempChar;
            } else if (m_TempChar == '_') {
                // 忽略下划线
            } else {
                if (m_Builder.empty()) {
                    m_Builder = "0";
                }
                AddToken(ETokenType::Number, std::to_string(std::stoi(m_Builder, nullptr, 8)), EType::Int32);
                break;
            }
        } while (true);
    } else if (t == 'b') {
        do {
            m_TempChar = ReadChar();
            if (m_TempChar == '1' || m_TempChar == '0') {
                m_Builder += m_TempChar;
            } else if (m_TempChar == '_') {
                // 忽略下划线
            } else {
                if (m_Builder.empty()) {
                    m_Builder = "0";
                }
                AddToken(ETokenType::Number, std::to_string(std::stoi(m_Builder, nullptr, 2)), EType::Int32);
                break;
            }
        } while (true);
    } else {
        UndoChar();
        ReadNumber();
    }
}

void LexerParse::ParseToTokenList() {
    m_CurChar = END_CHAR;
    m_Builder.clear();
    m_ListTokens.clear();
    m_Index = 0;
    
    while (m_Index < m_Length) {
        m_CurChar = m_Buffer[m_Index];
        if (m_CurChar == '\n') {
            AddToken(ETokenType::LineEnd);
            m_Index++;
            AddLine();
            continue;
        } else if (m_CurChar == ' ') {
            int num = 1;
            int bline = m_SourceLine;
            int bchar = m_SourceChar++;
            m_Index++;
            while (m_Index < m_Length) {
                if (m_Buffer[m_Index] != ' ') {
                    break;
                }
                m_SourceChar++;
                m_Index++;
                num++;
            }

            auto spacetoken = std::make_unique<Token>(m_Path, ETokenType::Space, "", bline, bchar);
            spacetoken->SetSrouceEnd(m_SourceLine, m_SourceChar);
            spacetoken->SetExtend(num);
            m_ListTokens.push_back(std::move(spacetoken));
        } else if (m_CurChar == '\t' || m_CurChar == '\r') {
            m_Index++;
            m_SourceChar++;
            continue;
        } else if (m_CurChar == '.') {
            AddToken(ETokenType::Period, ".");
            m_Index++;
            m_SourceChar++;
        } else if (m_CurChar == ';') {
            AddToken(ETokenType::SemiColon);
            m_Index++;
            m_SourceChar++;
        } else {
            switch (m_CurChar) {
                case '(':
                    AddToken(ETokenType::LeftPar);
                    m_Index++;
                    m_SourceChar++;
                    break;
                case ')':
                    AddToken(ETokenType::RightPar);
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '[':
                    AddToken(ETokenType::LeftBracket);
                    m_Index++;
                    m_SourceChar++;
                    break;
                case ']':
                    AddToken(ETokenType::RightBracket);
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '{':
                    AddToken(ETokenType::LeftBrace);
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '}':
                    ReadRightBrace();
                    m_Index++;
                    m_SourceChar++;
                    break;
                case ',':
                    AddToken(ETokenType::Comma);
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '~':
                    AddToken(ETokenType::Negative);
                    m_Index++;
                    m_SourceChar++;
                    break;
                case ':':
                    AddToken(ETokenType::Colon);
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '?':
                    ReadQuestionMark();
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '+':
                    ReadPlus();
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '-':
                    ReadMinus();
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '*':
                    ReadMultiply();
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '/':
                    ReadDivide();
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '%':
                    ReadModulo();
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '=':
                    ReadAssign();
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '&':
                    ReadAnd();
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '|':
                    ReadOr();
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '!':
                    ReadNot();
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '>':
                    ReadGreater();
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '<':
                    ReadLess();
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '^':
                    ReadXor();
                    m_Index++;
                    m_SourceChar++;
                    break;
                case '#':
                    ReadSharp();
                    break;
                case '$':
                    ReadDollar();
                    break;
                case '@':
                    ReadAt();
                    break;
                case '"':
                    ReadString(false);
                    break;
                case '\'':
                    ReadOrigenString();
                    break;
                case '0':
                    ReadNumberOrHexOrOctOrBinNumber();
                    break;
                default:
                    if (std::isdigit(m_CurChar)) {
                        ReadNumber();
                    } else if (IsIdentifier(m_CurChar)) {
                        ReadIdentifier();
                        m_Index++;
                        m_SourceChar++;
                    } else {
                        Log::GetInstance().AddInHandleToken(m_Path, m_SourceLine, m_SourceChar, EError::UnMatchChar, 
                            "解析错误，无法解析这种类型的字符[ " + std::string(1, m_CurChar) + " ]");
                    }
                    break;
            }
        }
    }
}

} // namespace Parse
} // namespace Compile
} // namespace SimpleLanguage
