//****************************************************************************
//  File:      IRStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/21 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../IRBase.h"
#include "../IRMethod.h"
#include <vector>

namespace SimpleLanguage {
namespace IR {

class IRStatements {
public:
    IRMethod* irMethod = nullptr;
    std::vector<IRBase*>& GetIRStatements() { return m_IRStatements; }
    const std::vector<IRBase*>& GetIRStatements() const { return m_IRStatements; }

protected:
    std::vector<IRBase*> m_IRStatements;
};

} // namespace IR
} // namespace SimpleLanguage