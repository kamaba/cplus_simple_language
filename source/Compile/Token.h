//****************************************************************************
//  File:      Token.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../Define.h"
#include <string>
#include <vector>
#include <memory>

namespace SimpleLanguage {
namespace Compile {

    class Token
    {
    public:
        Token();
        Token(const std::string& path, ETokenType tokenType, const std::string& lexeme, 
              int sourceLine, int sourceChar, const std::string& extend = "");
        Token(const Token& token);
        virtual ~Token() = default;

        // 属性访问器
        std::string GetPath() const { return m_Path; }
        ETokenType GetType() const { return m_Type; }
        std::string GetLexeme() const { return m_Lexeme; }
        std::string GetExtend() const { return m_Extend; }
        int GetSourceBeginLine() const { return m_SourceBeginLine; }
        int GetSourceBeginChar() const { return m_SourceBeginChar; }
        int GetSourceEndLine() const { return m_SourceEndLine; }
        int GetSourceEndChar() const { return m_SourceEndChar; }

        // 设置方法
        void SetSourceEnd(int endSourceLine, int endSourceChar);
        void SetType(ETokenType type) { m_Type = type; }
        void SetLexeme(const std::string& lexeme) { m_Lexeme = lexeme; }
        void SetLexeme(const std::string& lexeme, ETokenType tokenType);
        void SetExtend(const std::string& extend) { m_Extend = extend; }
        void SetBindFilePath(const std::string& path) { m_Path = path; }

        // 子Token管理
        void AddChildrenToken(std::shared_ptr<Token> token);
        const std::vector<std::shared_ptr<Token>>& GetChildrenTokens() const { return m_ChildrenTokensList; }

        // 工具方法
        std::string ToString() const;
        std::string ToAllString() const;
        std::string ToLexemeAllString() const;
        EType GetEType() const;
        std::string ToConstString() const;

    private:
        std::string m_Path;               //文件路径
        ETokenType m_Type;               //标记类型
        std::string m_Lexeme;            //标记值
        std::string m_Extend;            //辅助标记，可为空
        int m_SourceBeginLine;           //开始所在行
        int m_SourceBeginChar;           //开始所在列
        int m_SourceEndLine;             //结束所在行
        int m_SourceEndChar;             //结束所在行

        std::vector<std::shared_ptr<Token>> m_ChildrenTokensList;
    };

} // namespace Compile
} // namespace SimpleLanguage
