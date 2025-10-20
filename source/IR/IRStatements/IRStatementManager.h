//****************************************************************************
//  File:      IRStatementManager.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/21 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../IRMethod.h"
#include "../Core/IRMetaVariable.h"
#include "../../Core/MetaVariable.h"

namespace SimpleLanguage {
namespace IR {

class IRStatementManager {
public:
    static IRMetaVariable* CreateMethodIRVariable(IRMethod* irMethod, MetaVariable* mv);
};

} // namespace IR
} // namespace SimpleLanguage
