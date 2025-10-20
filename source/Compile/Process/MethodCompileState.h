//****************************************************************************
//  File:      MethodCompileState.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "CompileStateBase.h"

namespace SimpleLanguage {
namespace Compile {
namespace Process {

class MethodCompileState : public CompileStateBase {
public:
    MethodCompileState() = default;
    virtual ~MethodCompileState() = default;
};

} // namespace Process
} // namespace Compile
} // namespace SimpleLanguage
