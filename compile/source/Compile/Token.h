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
#include <sstream>
#include <memory>
#include <any>

namespace SimpleLanguage {
namespace Compile {

    union DataUnion 
    {
        int8_t sbyte_val;
        uint8_t byte_val;
        int16_t short_val;
        uint16_t ushort_val;
        int32_t int_val;
        uint32_t uint_val;
        int64_t long_val;
        uint64_t ulong_val;
        float float_val;
        double double_val;
        const char* string_val;
        void* object_valptr;
    };
    // 类型标签枚举
    enum class DataType {
        None,
        SByte,  //
        Byte,    // uint8_t
        Short,      //
        UShort, 
        Int,     // int
        UInt,
        Long,     // long
        ULong,
        Float,      //float
        Double,     //Double
        String,  // 字符串（存储在 char 数组中）
        Object,
    };
    // 带标签的结构体
    struct MultiData
    {
        DataType type = DataType::None;
        DataUnion data;

        MultiData() {};
        // 构造函数（省略，同上一条回复）
        MultiData(int8_t val) : type(DataType::SByte) { data.sbyte_val = val; }
        MultiData(uint8_t val) : type(DataType::Byte) { data.byte_val = val; }
        MultiData(int16_t val) : type(DataType::Short) { data.short_val = val; }
        MultiData(uint16_t val) : type(DataType::UShort) { data.ushort_val = val; }
        MultiData(int32_t val) : type(DataType::Int) { data.int_val = val; }
        MultiData(uint32_t val) : type(DataType::UInt) { data.uint_val = val; }
        MultiData(int64_t val) : type(DataType::Long) { data.long_val = val; }
        MultiData(uint64_t val) : type(DataType::ULong) { data.ulong_val = val; }
        MultiData(float_t val) : type(DataType::Float) { data.float_val = val; }
        MultiData(double_t val) : type(DataType::Double) { data.double_val = val; }
        MultiData(const std::string& val) : type(DataType::String) {data.string_val = val.c_str();}
        MultiData(const char* cchar) :type(DataType::String) { data.string_val = cchar; }
        MultiData(void* ptr) :type(DataType::Object) { data.object_valptr = ptr; }
        // 核心方法：将内部数据转换为 std::string
        std::string ToString() const
        {
            std::stringstream ss;  // 用字符串流格式化不同类型

            switch (type) {
            case DataType::Byte:
                // byte 通常按十进制或十六进制输出（这里按十进制）
                ss << data.byte_val;  // 转换为 int 避免被当作字符
                break;
            case DataType::SByte:
                // byte 通常按十进制或十六进制输出（这里按十进制）
                ss << data.byte_val;  // 转换为 int 避免被当作字符
                break;
            case DataType::Short:
                // char 直接输出字符本身
                ss << data.short_val;
                break;
            case DataType::UShort:
                // char 直接输出字符本身
                ss << (uint16_t)data.short_val;
                break;
            case DataType::Int:
                // char 直接输出字符本身
                ss << data.int_val;
                break;
            case DataType::UInt:
                // char 直接输出字符本身
                ss << (uint32_t)data.int_val;
                break;
            case DataType::Long:
                // char 直接输出字符本身
                ss << data.long_val;
                break;
            case DataType::ULong:
                // char 直接输出字符本身
                ss << (uint64_t)data.long_val;
                break;
            case DataType::String:
                // 直接将字符数组转换为 string
                ss << data.string_val;
                break;
            default:
                throw std::runtime_error("Unknown data type");
            }

            return ss.str();  // 返回格式化后的字符串
        }
    };

    class Token
    {
    public:
        Token();
        Token(const std::string& path, ETokenType tokenType, const std::string& lexeme,
            int sourceLine, int sourceChar, EType extendType );
        Token(const std::string& path, ETokenType tokenType, const MultiData& lexeme,
            int sourceLine, int sourceChar);
        Token(const std::string& path, ETokenType tokenType, const MultiData& lexeme,
              int sourceLine, int sourceChar, const MultiData* extend );
        Token(const Token& token);
        virtual ~Token() = default;

        // 属性访问器
        std::string GetHashCodeString() { return std::to_string(m_HashCode); }
        uint32_t GetHashCode() { return m_HashCode; }
        std::string GetPath() const { return m_Path; }
        ETokenType GetType() const { return m_Type; }
        MultiData GetLexeme() const { return m_Lexeme; }
        MultiData GetExtend() const { return m_Extend; }
        int GetSourceBeginLine() const { return m_SourceBeginLine; }
        int GetSourceBeginChar() const { return m_SourceBeginChar; }
        int GetSourceEndLine() const { return m_SourceEndLine; }
        int GetSourceEndChar() const { return m_SourceEndChar; }

        // 设置方法
        std::string GetLexemeString() const;
        void CreateHashCode();
        void SetSourceEnd(int endSourceLine, int endSourceChar);
        void SetType(ETokenType type) { m_Type = type; }
        void SetLexeme(const MultiData& lexeme) { m_Lexeme = lexeme; }
        void SetLexeme(const MultiData& lexeme, ETokenType tokenType);
        void SetExtend(const MultiData& extend) { m_Extend = extend; }
        void SetBindFilePath(const std::string& path) { m_Path = path; }

        // 子Token管理
        void AddChildrenToken(Token* token);
        const std::vector<Token*>& GetChildrenTokens() const { return m_ChildrenTokensList; }

        // 工具方法
        std::string ToString() const;
        std::string ToAllString() const;
        std::string ToLexemeAllString() const;
        EType GetEType() const;
        std::string ToConstString() const;

    private:
        uint32_t m_HashCode;             //hashCode
        std::string m_Path;               //文件路径
        ETokenType m_Type;               //标记类型
        MultiData m_Lexeme;            //标记值
        MultiData m_Extend;            //辅助标记，可为空
        int m_SourceBeginLine;           //开始所在行
        int m_SourceBeginChar;           //开始所在列
        int m_SourceEndLine;             //结束所在行
        int m_SourceEndChar;             //结束所在行

        std::vector<Token*> m_ChildrenTokensList;
    };

} // namespace Compile
} // namespace SimpleLanguage
