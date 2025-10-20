//****************************************************************************
//  File:      IRCall.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "IRBase.h"
#include "IRData.h"
#include "../Core/MetaMethodCall.h"
#include <string>

namespace SimpleLanguage {
namespace IR {

class IRMethod;
class IRMetaType;
class IRMetaClass;
class IRMethodCall;

class IRCallFunction : public IRBase {
public:
    int paramCount = 0;
    bool target = false;

private:
    void* m_MethodInfo = nullptr;  // MethodInfo equivalent
    IRMethod* m_IRRuntimeMethod = nullptr;

public:
    IRCallFunction(IRMethod* _irMethod);
    virtual ~IRCallFunction() = default;

    void Parse(MetaMethodCall* mfc);
    void* InvokeCSharp(void* target, void** csParamObjs);
    std::string ToIRString() override;
};

} // namespace IR
} // namespace SimpleLanguage
