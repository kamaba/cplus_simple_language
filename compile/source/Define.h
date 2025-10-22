//****************************************************************************
//  File:      Define.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include <cstdint>
#include <string>

namespace SimpleLanguage {

    //前置权限
    enum class EPermission : uint8_t
    {
        Null = 0,
        Export = 1,
        Public = 2,
        Protected = 3,
        Private = 4
    };

    //前置类型
    enum class EType : uint8_t
    {
        None = 0,
        Null = 1,
        Void = 2,
        Class = 3,
        Enum = 4,
        Data = 5,
        Boolean = 6,
        Bit = 7,
        Byte = 8,
        SByte = 9,
        Int16 = 10,
        UInt16 = 11,
        Int32 = 12,
        UInt32 = 13,
        Float16 = 14,
        Float32 = 15,
        Int64 = 16,
        UInt64 = 17,
        Float64 = 18,
        Int128 = 19,
        UInt128 = 20,
        Array = 21,
        Range = 22,
        String = 23,
        Object = 24,
        Float2 = 25
    };

    //token类型
    enum class ETokenType : uint16_t
    {
        /// <summary>  </summary>
        None = 0,
        /// <summary> space  </summary>
        Space = 1,
        /// <summary> void </summary>
        Void = 2,
        /// <summary>
        /// int/int32/int16/short/long/half/float/double/float3x3
        /// </summary>
        Type = 3,
        /// <summary> { </summary>
        LeftBrace = 4,
        /// <summary> } </summary>
        RightBrace = 5,
        /// <summary> ( </summary>
        LeftPar = 6,
        /// <summary> ) </summary>
        RightPar = 7,
        /// <summary> [ </summary>
        LeftBracket = 8,
        /// <summary> ] </summary>
        RightBracket = 9,
        /// <summary> . </summary>
        Period = 10,
        /// <summary> @ </summary>
        At = 11,
        /// <summary> $ </summary>
        Dollar = 12,
        /// <summary> & </summary>
        Address = 13,
        /// <summary> , </summary>
        Comma = 14,
        /// <summary> : </summary>
        Colon = 15,
        /// <summary> ; </summary>
        SemiColon = 16,
        /// <summary> \n </summary>
        LineEnd = 17,
        /// <summary> ? </summary>
        QuestionMark = 18,
        /// <summary> ?? </summary>
        EmptyRet = 19,
        /// <summary> ?. </summary>
        QuestionMarkDot = 20,
        /// <summary> + </summary>
        Plus = 21,
        ///<summary> ++ </summary>
        DoublePlus = 22,
        /// <summary> += </summary>
        PlusAssign = 23,
        /// <summary> - </summary>
        Minus = 24,
        ///<summary> ++ </summary>
        DoubleMinus = 25,
        /// <summary> -= </summary>
        MinusAssign = 26,
        /// <summary> * </summary>
        Multiply = 27,
        /// <summary> *= </summary>
        MultiplyAssign = 28,
        /// <summary> / </summary>
        Divide = 29,
        /// <summary> /= </summary>
        DivideAssign = 30,
        /// <summary> % 模运算 </summary>
        Modulo = 31,
        /// <summary> %= </summary>
        ModuloAssign = 32,
        /// <summary> | 或运算 </summary>
        InclusiveOr = 33,
        /// <summary> |= </summary>
        InclusiveOrAssign = 34,
        /// <summary> || </summary>
        Or = 35,
        /// <summary> & 并运算 </summary>
        Combine = 36,
        /// <summary> &= </summary>
        CombineAssign = 37,
        /// <summary> && </summary>
        And = 38,
        /// <summary> ^ 异或 </summary>
        XOR = 39,
        /// <summary> ^= </summary>
        XORAssign = 40,
        /// <summary>  ~ 取反操作 </summary>
        Negative = 41,
        /// <summary> << 左移 </summary>
        Shi = 42,
        /// <summary> >> 右移 </summary>
        Shr = 43,
        /// <summary> # </summary>
        Sharp = 44,
        /// <summary> ! </summary>
        Not = 45,
        /// <summary> = </summary>
        Assign = 46,
        /// <summary> == </summary>
        Equal = 47,
        /// <summary> === </summary>
        ValueEqual = 48,
        /// <summary> != </summary>
        NotEqual = 49,
        /// <summary> !== </summary>
        ValueNotEqual = 50,
        /// <summary> > </summary>
        Greater = 51,
        /// <summary> >= </summary>
        GreaterOrEqual = 52,
        /// <summary>  < </summary>
        Less = 53,
        /// <summary> <= </summary>
        LessOrEqual = 54,
        /// <summary> 1..2 </summary>
        NumberArrayLink = 55,
        /// <summary> params </summary>
        Params = 56,
        /// <summary> => </summary>
        Lambda = 57,
        /// <summary> if </summary>
        If = 58,
        /// <summary> else </summary>
        Else = 59,
        /// <summary> elif </summary>
        ElseIf = 60,
        /// <summary> !if </summary>
        MacroIf = 61,
        /// <summary> !else </summary>
        MacroElse = 62,
        /// <summary> !endif </summary>
        MacroEndif = 63,
        /// <summary> import </summary>
        Import = 64,
        /// <summary> as </summary>
        As = 65,
        /// <summary> is </summary>
        Is = 66,
        /// <summary> switch </summary>
        Switch = 67,
        /// <summary> case </summary>
        Case = 68,
        /// <summary> default </summary>
        Default = 69,
        /// <summary> extern  </summary>
        Extern = 70,
        /// <summary> public </summary>
        Public = 71,
        /// <summary> projected </summary>
        Projected = 72,
        /// <summary> private</summary>
        Private = 73,
        /// <summary> Interface </summary>
        Interface = 74,
        /// <summary> extends </summary>
        Extends = 75,
        /// <summary> virtual </summary>
        //Virtual,
        /// <summary> override </summary>
        Override = 76,
        /// <summary> const </summary>
        Const = 77,
        /// <summary> mut </summary>
        Mut = 78,
        /// <summary> final </summary>
        Final = 79,
        /// <summary> static </summary>
        Static = 80,
        /// <summary> get </summary>
        Get = 81,
        /// <summary> set </summary>
        Set = 82,
        /// <summary> let </summary>
        Let = 83,
        /// <summary> new </summary>
        New = 84,
        /// <summary> partial </summary>
        Partial = 85,
        /// <summary> namespace </summary>
        Namespace = 86,
        /// <summary> class </summary>
        Class = 87,
        /// <summary> enum </summary>
        Enum = 88,
        /// <summary> data </summary>
        Data = 89,
        /// <summary> dynamic </summary>
        Dynamic = 90,
        /// <summary> break </summary>
        Break = 91,
        /// <summary> next </summary>
        Next = 92,
        /// <summary> continue </summary>
        Continue = 93,
        /// <summary> goto </summary>
        Goto = 94,
        /// <summary> transience </summary>
        Transience = 95,
        /// <summary> return </summary>
        Return = 96,
        /// <summary> global </summary>
        Global = 97,
        /// <summary> label </summary>
        Label = 98,
        /// <summary> while </summary>
        While = 99,
        /// <summary> dowhile </summary>
        DoWhile = 100,
        /// <summary> for </summary>
        For = 101,
        /// <summary> in </summary>
        In = 102,
        /// <summary> function </summary>
        Function = 103,
        /// <summary> try </summary>
        Try = 104,
        /// <summary> catch </summary>
        Catch = 105,
        /// <summary> throw </summary>
        Throw = 106,
        /// <summary> BoolValue </summary>
        BoolValue = 107,
        /// <summary> number </summary>
        Number = 108,
        /// <summary> string </summary>
        String = 109,
        /// <summary> null </summary>
        Null = 110,
        /// <summary> var </summary>
        Var = 111,
        /// <summary> object </summary>
        Object = 112,
        /// <summary> this </summary>
        This = 113,
        /// <summary> base </summary>
        Base = 114,
        /// <summary> array </summary>     
        Array = 115,
        /// <summary> range </summary>     
        Range = 116,
        /// <summary> boolean </summary>   
        Boolean = 117,
        /// <summary> complex </summary>
        Complex = 118,
        /// <summary> 标识符 </summary>
        Identifier = 119,

        Float2 = 120, Float3 = 121, Float4 = 122,
        /// <summary> float extent </summary>
        Float2x2 = 123, 
        Float2x3 = 124, Float3x2 = 125, Float3x3 = 126,
        Float4x2 = 127, Float4x3 = 128, Float4x4 = 129, Float2x4 = 130, Float3x4 = 131,
        /// <summary> floatNxN extent </summary>
        Double2 = 132, Double3 = 133, Double4 = 134,
        /// <summary> double extent </summary>
        Double2x2 = 135,
        Double2x3 = 136, Double3x2 = 137, Double3x3 = 138,
        Double4x2 = 139, Double4x3 = 140, Double4x4 = 141, Double2x4 = 142, Double3x4 = 143,
        /// <summary> doubleNxN extent </summary>
        Matrix2x2 = 144,
        Matrix2x3 = 145, Matrix3x2 = 146, Matrix3x3 = 147,
        Matrix4x2 = 148, Matrix2x4 = 149, Matrix3x4 = 150, Matrix4x3 = 151, Matrix4x4 = 152,

        /// <summary> 结束 </summary>
        Finished = 153
    };

    enum class EOpSign : uint8_t
    {
        Plus = 0,
        Minus = 1,
        Multiply = 2,
        Divide = 3,
        Modulo = 4,
        InclusiveOr = 5,
        Or = 6,
        Combine = 7,
        And = 8,
        XOR = 9, 
        Negative = 10,
        Shi = 11,
        Shr = 12, 
        Not = 13
    };

    enum class EParseState : uint8_t
    {
        Null = 0,
        Begin = 1,
        End = 2
    };

    class SignComputePriority
    {
    public:
        static const int Level1 = 1;                         //(a+b) [] . 优先操作，对象操作等
        static const int Level2_LinkOp = 2;                        // -负号 (int)强转 ++x x++ -- ! ~ 
        static const int Level3_Hight_Compute = 3;                 // / * % 
        static const int Level3_Low_Compute = 4;                   // + - 
        static const int Level5_BitMoveOp = 5;              // << >> 
        static const int Level6_Compare = 6;                //< > <= >=
        static const int Level7_EqualAb = 7;                // == !=
        static const int Level8_BitAndOp = 81;           // &
        static const int Level8_BitXOrOp = 82;            // ^
        static const int Level8_BitOrOp = 83;               // |
        static const int Level9_And = 91;                   // &&
        static const int Level9_Or = 92;                    // ||
        static const int Level10_ThirdOp = 100;             // ? : 
        static const int Level11_Assign = 120;              // = /= *= %= += -= <<= >>= &= ^= |= 
        static const int Level12_Split = 130;                //,
    };

    class Global
    {
    public:
        static const std::string tabChar;
    };

} // namespace SimpleLanguage
