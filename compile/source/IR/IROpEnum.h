//****************************************************************************
//  File:      IROpEnum.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#pragma once

namespace SimpleLanguage {
namespace IR {

enum class EIROpCode : unsigned char {
    Nop,

    LoadBegin = 1,
    LoadConstNull,
    LoadConstByte,
    LoadConstSByte,
    LoadConstInt16,
    LoadConstUInt16,
    LoadConstInt32,
    LoadConstUInt32,
    LoadConstInt64,         
    LoadConstUInt64,  
    LoadConstFloat,
    LoadConstDouble,
    LoadConstBoolean,
    LoadConstString,
    
    LoadArgument,
    LoadLocal,  
    LoadNotStaticField,
    LoadStaticField,
    LocalGlobal,

    NewObject,
    NewTemplateClass,
    NewArray,
    Dup,
    Pop,

    StoreLocal,
    StoreNotStaticField1,
    StoreNotStaticField2,
    StoreStaticField,
    StoreGlobal,
    StoreReturn,

    // ÔËËãÖ¸Áî
    Add,                   // +
    Add_Un,
    Minus,                  // -
    Minus_Un,
    Multiply,               // *
    Multiply_Un,
    Divide,                 // /
    Divide_Un,
    Modulo,                 // %
    Module_Un,
    InclusiveOr,            // |
    InclusiveOr_Un,
    Combine,                // &
    Combine_Un,         
    XOR,                    // ^
    XOR_Un,
    Shr,                    // >>
    Shr_Un,
    Shi,                    // <<
    Shi_Un,
    Not,                    //!
    Neg,                    //-

    Ceq,                    // == 
    Ceq_Un,                 // unsign ==
    Cne,                    // !=
    Cne_Un,                 // unsign !=
    Cgt,                    // >
    Cgt_Un,                 // unsign >
    Cge,                    // >=
    Cge_Un,                 // unsign >=
    Clt,                    // <
    Clt_Un,                 // < unsign 
    Cle,                    // <=
    Cle_Un,                 // unsign <=

    And,                    //&&
    Or,                     //||

    Label,   
    Beq,                    // if x1 == x2 then execute(code) equal move instruct index
    Beq_Un,                 // same top but value is unsign!
    Bge,                    // if x1 >= x2 then execute(code)
    Bge_un,                 // same top but value is unsign!
    Bgt,                    // if x1 > x2 then execute(code)
    Bgt_Un,                 // same top but value is unsign!
    Ble,                    // if x1 <= x2 then execute(code)
    Ble_Un,                 // same top but value is unsign!
    Bne,                    // if x1 != x2 then execute(code)
    Bne_Un,                 // same top but value is unsign!
    Br,                     // 
    Break,                  //
    Jmp,
    BrLabel,
    BrFalse,
    BrTrue,
    Switch,
        
    CallStatic,
    CallDynamic,
    CallVirt,
    Calli,
    CallCSharpMethod,
    CastClass,

    Convert_I8,
    Convert_SI8,
    Convert_I16,
    Convert_UI16,
    Convert_I32,
    Convert_UI32,
    Convert_I64,
    Convert_UI64,
    Convert_R4,
    Convert_R8,

    Ret,          
};

} // namespace IR
} // namespace SimpleLanguage
