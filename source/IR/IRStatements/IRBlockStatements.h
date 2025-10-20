//****************************************************************************
//  File:      IRBlockStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "IRStatements.h"
#include "../IRNop.h"
#include "../Core/Statements/MetaBlockStatements.h"

namespace SimpleLanguage {
namespace IR {

class IRNop;
class IRDefineVarStatements;
class IRAssignStatements;
class IRBreakStatements;
class IRContinueStatements;
class IRGotoLabelStatements;
class IRIfStatements;
class IRReturnStatements;
class IRSwitchStatements;
class IRForStatements;
class IRWhileDoWhileStatements;
class IRCallStatements;

class IRBlockStatements : public IRStatements {
public:
    IRNop* GetBlockStart() const { return m_BlockStart; }

private:
    IRNop* m_BlockStart = nullptr;

public:
    IRBlockStatements(IRMethod* irmthod);
    virtual ~IRBlockStatements() = default;

    void ParseAllIRStatements(MetaBlockStatements* ms);
};

} // namespace IR
} // namespace SimpleLanguage