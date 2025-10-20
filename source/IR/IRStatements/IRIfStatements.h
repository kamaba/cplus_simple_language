//****************************************************************************
//  File:      IRIfStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/13 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "IRStatements.h"
#include "../IRExpress.h"
#include "../IRBranch.h"
#include "../IRNop.h"
#include "../IRStoreVariable.h"
#include "../IRLoadVariable.h"
#include "../IRMetaType.h"
#include "../IRMetaClass.h"
#include "../Core/Statements/MetaIfStatements.h"
#include <vector>
#include <string>

namespace SimpleLanguage {
namespace IR {

class IRIfStatements : public IRStatements {
public:
    IRIfStatements(IRMethod* method);
    virtual ~IRIfStatements() = default;

    void ParseIRStatements(MetaIfStatements* ifstatements);

public:
    class MetaIRElseIfStatements {
    public:
        std::vector<IRBase*> conditionStatList;
        std::vector<IRBase*> thenStatList;

        IRBranch* ifEndBrach = nullptr;
        IRBranch* ifFalseBreach = nullptr;
        IRNop* startNop = nullptr;

    private:
        IRExpress* m_IrExpress = nullptr;

    public:
        void ParseIRStatements(IRMethod* _irMethod, MetaIfStatements::MetaElseIfStatements* mires);
        std::string ToIRString();
    };
};

} // namespace IR
} // namespace SimpleLanguage