//****************************************************************************
//  File:      Token.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "Token.h"
#include <sstream>

namespace SimpleLanguage {
namespace Compile {

    Token::Token()
        : m_Type(ETokenType::None)
        , m_Lexeme("")
        , m_SourceBeginLine(0)
        , m_SourceBeginChar(0)
        , m_SourceEndLine(0)
        , m_SourceEndChar(0)
    {
    }

    Token::Token(const std::string& path, ETokenType tokenType, const std::string& lexeme, 
                 int sourceLine, int sourceChar, const std::string& extend)
        : m_Path(path)
        , m_Type(tokenType)
        , m_Lexeme(lexeme)
        , m_SourceBeginLine(sourceLine + 1)
        , m_SourceBeginChar(sourceChar)
        , m_Extend(extend)
        , m_SourceEndLine(0)
        , m_SourceEndChar(0)
    {
    }

    Token::Token(const Token& token)
        : m_Path(token.m_Path)
        , m_Type(token.m_Type)
        , m_Lexeme(token.m_Lexeme)
        , m_Extend(token.m_Extend)
        , m_SourceBeginLine(token.m_SourceBeginChar)
        , m_SourceBeginChar(token.m_SourceBeginLine)
        , m_SourceEndChar(token.m_SourceEndChar)
        , m_SourceEndLine(token.m_SourceEndLine)
    {
    }

    void Token::SetSourceEnd(int endSourceLine, int endSourceChar)
    {
        m_SourceEndLine = endSourceLine;
        m_SourceEndChar = endSourceChar;
    }

    void Token::SetLexeme(const std::string& lexeme, ETokenType tokenType)
    {
        m_Lexeme = lexeme;
        m_Type = tokenType;
    }

    void Token::AddChildrenToken(std::shared_ptr<Token> token)
    {
        m_ChildrenTokensList.push_back(token);
    }

    std::string Token::ToString() const
    {
        return m_Lexeme;
    }

    std::string Token::ToAllString() const
    {
        std::ostringstream oss;
        oss << "Path:" << m_Path << " Line: " << m_SourceBeginLine << " Pos: " << m_SourceBeginChar << "  ";
        return oss.str();
    }

    std::string Token::ToLexemeAllString() const
    {
        std::ostringstream oss;
        oss << "Lex: " << m_Lexeme << " Path:" << m_Path << " Line: " << m_SourceBeginLine << " Pos: " << m_SourceBeginChar << "  ";
        return oss.str();
    }

    EType Token::GetEType() const
    {
        EType etype = EType::None;
        switch (m_Type)
        {
            case ETokenType::Number:
            {
                // 这里需要根据extend字符串解析EType
                // 简化实现，实际需要更复杂的解析逻辑
                if (m_Extend == "Byte") etype = EType::Byte;
                else if (m_Extend == "SByte") etype = EType::SByte;
                else if (m_Extend == "Int16") etype = EType::Int16;
                else if (m_Extend == "UInt16") etype = EType::UInt16;
                else if (m_Extend == "Int32") etype = EType::Int32;
                else if (m_Extend == "UInt32") etype = EType::UInt32;
                else if (m_Extend == "Int64") etype = EType::Int64;
                else if (m_Extend == "UInt64") etype = EType::UInt64;
                else if (m_Extend == "Float32") etype = EType::Float32;
                else if (m_Extend == "Float64") etype = EType::Float64;
                else etype = EType::Int32; // 默认
            }
            break;
            case ETokenType::Type:
            {
                // 类似上面的解析逻辑
                if (m_Extend == "Byte") etype = EType::Byte;
                else if (m_Extend == "SByte") etype = EType::SByte;
                else if (m_Extend == "Int16") etype = EType::Int16;
                else if (m_Extend == "UInt16") etype = EType::UInt16;
                else if (m_Extend == "Int32") etype = EType::Int32;
                else if (m_Extend == "UInt32") etype = EType::UInt32;
                else if (m_Extend == "Int64") etype = EType::Int64;
                else if (m_Extend == "UInt64") etype = EType::UInt64;
                else if (m_Extend == "Float32") etype = EType::Float32;
                else if (m_Extend == "Float64") etype = EType::Float64;
                else etype = EType::Int32; // 默认
            }
            break;
            case ETokenType::Boolean:
            case ETokenType::BoolValue:
            {
                etype = EType::Boolean;
            }
            break;
            case ETokenType::String:
                etype = EType::String;
                break;
            case ETokenType::Null:
                etype = EType::Null;
                break;
            case ETokenType::NumberArrayLink:
                etype = EType::Array;
                break;
            default:
                etype = EType::Class;
                break;
        }
        return etype;
    }

    std::string Token::ToConstString() const
    {
        std::string types = "";
        std::string val = "";
        
        if (m_Type == ETokenType::Number)
        {
            EType etype = GetEType();
            switch (etype)
            {
                case EType::Byte:
                case EType::SByte:
                {
                    val = m_Lexeme;
                }
                break;
                case EType::Int16:
                {
                    val = m_Lexeme;
                    types = "s";
                }
                break;
                case EType::UInt16:
                {
                    val = m_Lexeme;
                    types = "us";
                }
                break;
                case EType::Int32:
                {
                    val = m_Lexeme;
                    types = "i";
                }
                break;
                case EType::UInt32:
                {
                    val = m_Lexeme;
                    types = "ui";
                }
                break;
                case EType::Int64:
                {
                    val = m_Lexeme;
                    types = "uL";
                }
                break;
                case EType::UInt64:
                {
                    val = m_Lexeme;
                    types = "L";
                }
                break;
                case EType::Float32:
                {
                    val = m_Lexeme;
                    types = "f";
                }
                break;
                case EType::Float64:
                {
                    val = m_Lexeme;
                    types = "d";
                }
                break;
                default:
                {
                    val = m_Lexeme;
                }
                break;
            }
            return val + types;
        }
        else if (m_Type == ETokenType::String)
        {
            return "\"" + m_Lexeme + "\"";
        }
        else
        {
            return m_Lexeme + types;
        }
    }

} // namespace Compile
} // namespace SimpleLanguage
