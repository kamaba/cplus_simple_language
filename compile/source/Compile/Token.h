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
    // ���ͱ�ǩö��
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
        String,  // �ַ������洢�� char �����У�
        Object,
    };
    // ����ǩ�Ľṹ��
    struct MultiData
    {
        DataType type = DataType::None;
        DataUnion data;

        MultiData() {};
        // ���캯����ʡ�ԣ�ͬ��һ���ظ���
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
        // ���ķ��������ڲ�����ת��Ϊ std::string
        std::string ToString() const
        {
            std::stringstream ss;  // ���ַ�������ʽ����ͬ����

            switch (type) {
            case DataType::Byte:
                // byte ͨ����ʮ���ƻ�ʮ��������������ﰴʮ���ƣ�
                ss << data.byte_val;  // ת��Ϊ int ���ⱻ�����ַ�
                break;
            case DataType::SByte:
                // byte ͨ����ʮ���ƻ�ʮ��������������ﰴʮ���ƣ�
                ss << data.byte_val;  // ת��Ϊ int ���ⱻ�����ַ�
                break;
            case DataType::Short:
                // char ֱ������ַ�����
                ss << data.short_val;
                break;
            case DataType::UShort:
                // char ֱ������ַ�����
                ss << (uint16_t)data.short_val;
                break;
            case DataType::Int:
                // char ֱ������ַ�����
                ss << data.int_val;
                break;
            case DataType::UInt:
                // char ֱ������ַ�����
                ss << (uint32_t)data.int_val;
                break;
            case DataType::Long:
                // char ֱ������ַ�����
                ss << data.long_val;
                break;
            case DataType::ULong:
                // char ֱ������ַ�����
                ss << (uint64_t)data.long_val;
                break;
            case DataType::String:
                // ֱ�ӽ��ַ�����ת��Ϊ string
                ss << data.string_val;
                break;
            default:
                throw std::runtime_error("Unknown data type");
            }

            return ss.str();  // ���ظ�ʽ������ַ���
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

        // ���Է�����
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

        // ���÷���
        std::string GetLexemeString() const;
        void CreateHashCode();
        void SetSourceEnd(int endSourceLine, int endSourceChar);
        void SetType(ETokenType type) { m_Type = type; }
        void SetLexeme(const MultiData& lexeme) { m_Lexeme = lexeme; }
        void SetLexeme(const MultiData& lexeme, ETokenType tokenType);
        void SetExtend(const MultiData& extend) { m_Extend = extend; }
        void SetBindFilePath(const std::string& path) { m_Path = path; }

        // ��Token����
        void AddChildrenToken(Token* token);
        const std::vector<Token*>& GetChildrenTokens() const { return m_ChildrenTokensList; }

        // ���߷���
        std::string ToString() const;
        std::string ToAllString() const;
        std::string ToLexemeAllString() const;
        EType GetEType() const;
        std::string ToConstString() const;

    private:
        uint32_t m_HashCode;             //hashCode
        std::string m_Path;               //�ļ�·��
        ETokenType m_Type;               //�������
        MultiData m_Lexeme;            //���ֵ
        MultiData m_Extend;            //������ǣ���Ϊ��
        int m_SourceBeginLine;           //��ʼ������
        int m_SourceBeginChar;           //��ʼ������
        int m_SourceEndLine;             //����������
        int m_SourceEndChar;             //����������

        std::vector<Token*> m_ChildrenTokensList;
    };

} // namespace Compile
} // namespace SimpleLanguage
