//****************************************************************************
//  File:      CoreCompileState.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "CompileStateBase.h"

namespace SimpleLanguage {
namespace Compile {

class CoreCompileState : public CompileStateBase {
public:
    CoreCompileState() = default;
    virtual ~CoreCompileState() = default;
};

} // namespace Compile
} // namespace SimpleLanguage
