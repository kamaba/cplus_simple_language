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
        , m_SourceBeginLine(0)
        , m_SourceBeginChar(0)
        , m_SourceEndLine(0)
        , m_SourceEndChar(0)
    {
    }

    Token::Token(const std::string& path, ETokenType tokenType, const std::string& lexeme,
        int sourceLine, int sourceChar)
    {

    }
    Token::Token(const std::string& path, ETokenType tokenType, const MultiData& lexeme,
        int sourceLine, int sourceChar )
        : m_Path(path)
        , m_Type(tokenType)
        , m_SourceBeginLine(sourceLine + 1)
        , m_SourceBeginChar(sourceChar)
        , m_SourceEndLine(0)
        , m_SourceEndChar(0)
    {
        m_Path = path;
        m_Lexeme = lexeme;
    }
    Token::Token(const std::string& path, ETokenType tokenType, const MultiData& lexeme,
        int sourceLine, int sourceChar, const MultiData* extend = nullptr )
        : m_Path(path)
        , m_Type(tokenType)
        , m_SourceBeginLine(sourceLine + 1)
        , m_SourceBeginChar(sourceChar)
        , m_SourceEndLine(0)
        , m_SourceEndChar(0)
    {
        m_Path = path;
        m_Lexeme = lexeme;
        if (extend != nullptr)
        {
            m_Extend = *extend;
        }
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

    void Token::SetLexeme(const MultiData& lexeme, ETokenType tokenType)
    {
        m_Lexeme = lexeme;
        m_Type = tokenType;
    }

    std::string Token::GetLexemeString() const
    {
        std::ostringstream ss;
        switch (m_Lexeme.type)
        {
        case DataType::Byte:
        {
            ss << m_Lexeme.data.byte_val;
        }break;
        case DataType::Short:
        {
            ss << m_Lexeme.data.short_val;
        }break;
        case DataType::Int:
        {
            ss << m_Lexeme.data.int_val;
        }break;
        case DataType::Long:
        {
            ss << m_Lexeme.data.long_val;
        }break;
        case DataType::String:
        {
            ss << m_Lexeme.data.string_val;
        }break;
        }
        return ss.str();
    }

    void Token::AddChildrenToken(Token* token)
    {
        m_ChildrenTokensList.push_back(token);
    }

    std::string Token::ToString() const
    {
        return GetLexemeString();
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
        const char* input = this->GetLexemeString().c_str();
        oss << "Lex: " << input << " Path:" << m_Path << " Line: " << m_SourceBeginLine << " Pos: " << m_SourceBeginChar << "  ";
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
                std::string extend_string = m_Extend.toString();
                if (extend_string == "Byte") etype = EType::Byte;
                else if (extend_string == "SByte") etype = EType::SByte;
                else if (extend_string == "Int16") etype = EType::Int16;
                else if (extend_string == "UInt16") etype = EType::UInt16;
                else if (extend_string == "Int32") etype = EType::Int32;
                else if (extend_string == "UInt32") etype = EType::UInt32;
                else if (extend_string == "Int64") etype = EType::Int64;
                else if (extend_string == "UInt64") etype = EType::UInt64;
                else if (extend_string == "Float32") etype = EType::Float32;
                else if (extend_string == "Float64") etype = EType::Float64;
                else etype = EType::Int32; // 默认
            }
            break;
            case ETokenType::Type:
            {
                // 类似上面的解析逻辑
                std::string extend_string = m_Extend.toString();
                if (extend_string == "Byte") etype = EType::Byte;
                else if (extend_string == "SByte") etype = EType::SByte;
                else if (extend_string == "Int16") etype = EType::Int16;
                else if (extend_string == "UInt16") etype = EType::UInt16;
                else if (extend_string == "Int32") etype = EType::Int32;
                else if (extend_string == "UInt32") etype = EType::UInt32;
                else if (extend_string == "Int64") etype = EType::Int64;
                else if (extend_string == "UInt64") etype = EType::UInt64;
                else if (extend_string == "Float32") etype = EType::Float32;
                else if (extend_string == "Float64") etype = EType::Float64;
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

        std::stringstream ss;
        
        if (m_Type == ETokenType::Number)
        {
            EType etype = GetEType();
            switch (etype)
            {
                case EType::Byte:
                case EType::SByte:
                {
                    ss << m_Lexeme.data.byte_val;
                    //val = m_Lexeme;
                }
                break;
                case EType::Int16:
                {
                    ss << m_Lexeme.data.short_val << "s";
                    //val = m_Lexeme;
                    types = "s";
                }
                break;
                case EType::UInt16:
                {
                    ss << (uint16_t)m_Lexeme.data.short_val << "us";
                    //val = m_Lexeme;
                    types = "us";
                }
                break;
                case EType::Int32:
                {
                    ss << (int32_t)m_Lexeme.data.int_val << "i";
                    //val = m_Lexeme;
                    types = "i";
                }
                break;
                case EType::UInt32:
                {
                    ss << (uint32_t)m_Lexeme.data.int_val << "ui";
                    //val = m_Lexeme;
                    types = "ui";
                }
                break;
                case EType::Int64:
                {
                    ss << (int64_t)m_Lexeme.data.long_val << "uL";
                    //val = m_Lexeme;
                    types = "uL";
                }
                break;
                case EType::UInt64:
                {
                    ss << (uint64_t)m_Lexeme.data.long_val << "L";
                    //val = m_Lexeme;
                    types = "L";
                }
                break;
                case EType::Float32:
                {
                    ss << (float_t)m_Lexeme.data.float_val << "f";
                    //val = m_Lexeme;
                    types = "f";
                }
                break;
                case EType::Float64:
                {
                    ss << (double_t)m_Lexeme.data.float_val << "d";
                    //val = m_Lexeme;
                    types = "d";
                }
                break;
                case EType::String:
                {
                    ss << m_Lexeme.data.string_val;
                    //val = m_Lexeme;
                    //types = "d";
                }
                break;
                default:
                {
                    //val = m_Lexeme;
                }
                break;
            }
        }
        else if (m_Type == ETokenType::String)
        {
            ss << "\"" << m_Lexeme.data.string_val << "\"";
            //return "\"" + m_Lexeme + "\"";
        }
        else
        {
            //return m_Lexeme + types;
        }
        return ss.str();
    }

} // namespace Compile
} // namespace SimpleLanguage
