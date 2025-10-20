//****************************************************************************
//  File:      IRBase.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "IRData.h"
#include "IRMethod.h"
#include <vector>
#include <string>

namespace SimpleLanguage {
namespace IR {

class IRMethod;

class IRBase {
protected:
    IRMethod* m_IRMethod = nullptr;
    std::vector<IRData*> m_IRDataList;

public:
    IRBase();
    IRBase(IRData* irdata);
    IRBase(IRMethod* irMethod);
    virtual ~IRBase() = default;

    std::vector<IRData*>& GetIRDataList() { return m_IRDataList; }
    const std::vector<IRData*>& GetIRDataList() const { return m_IRDataList; }

    void AddIRData(IRData* irData);
    void AddIRRangeData(const std::vector<IRData*>& ienumData);
    virtual std::string ToIRString();
};

} // namespace IR
} // namespace SimpleLanguage