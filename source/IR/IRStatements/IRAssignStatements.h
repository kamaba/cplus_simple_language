//****************************************************************************
//  File:      IRAssignStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/11 12:00:00
//  Description:  handle assign statements syntax to instruction r!
//****************************************************************************

#pragma once

#include "IRStatements.h"
#include "../IRExpress.h"
#include "../IRStoreVariable.h"
#include "../Core/IRMetaCallLink.h"
#include "../../Core/Statements/MetaAssignStatements.h"
#include "../../Debug/Log.h"

namespace SimpleLanguage {
namespace IR {

class IRAssignStatements : public IRStatements {
private:
    IRExpress* m_IRExpress = nullptr;
    IRStoreVariable* m_StoreVariable = nullptr;

public:
    IRAssignStatements(IRMethod* method);
    
    void ParseIRStatements(MetaAssignStatements* ms);
};

} // namespace IR
} // namespace SimpleLanguage
