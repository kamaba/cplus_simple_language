//****************************************************************************
//  File:      IRStatementManager.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/21 12:00:00
//  Description: 
//****************************************************************************

#include "IRStatementManager.h"

namespace SimpleLanguage {
namespace IR {

IRMetaVariable* IRStatementManager::CreateMethodIRVariable(IRMethod* irMethod, MetaVariable* mv) {
    IRMetaVariable* irmv = new IRMetaVariable(mv);
    return nullptr;
}

} // namespace IR
} // namespace SimpleLanguage
