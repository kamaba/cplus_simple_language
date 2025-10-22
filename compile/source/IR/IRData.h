//****************************************************************************
//  File:      IRData.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../Compile/Token.h"
#include "../Core/MetaType.h"
#include "IRMethod.h"
#include <string>

namespace SimpleLanguage {
namespace IR {

class IRMethod;

struct DebugInfo {
    std::string path;
    std::string name;
    int beginLine = 0;
    int beginChar = 0;
    int endLine = 0;
    int endChar = 0;
};

enum class EIROpCode {
    Nop,
    LoadConstByte,
    LoadConstSByte,
    LoadConstBoolean,
    LoadConstInt16,
    LoadConstUInt16,
    LoadConstInt32,
    LoadConstUInt32,
    LoadConstInt64,
    LoadConstUInt64,
    LoadConstFloat,
    LoadConstDouble,
    LoadConstString,
    LoadConstNull,
    Label,
    Br,
    BrFalse,
    BrTrue,
    BrLabel
};

class IRData {
public:
    int id = 0;
    EIROpCode opCode = EIROpCode::Nop;
    void* opValue = nullptr;
    int index = 0;
    DebugInfo debugInfo;

    IRData();
    
    void SetDebugInfoByValue(const DebugInfo& info);
    void SetDebugInfoByToken(Token* token);
    std::string ToString() const;
};

} // namespace IR
} // namespace SimpleLanguage