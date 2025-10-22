//****************************************************************************
//  File:      IRBase.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#include "IRBase.h"
#include "IRData.h"
#include "IRMethod.h"
#include <sstream>

namespace SimpleLanguage {
namespace IR {

IRBase::IRBase() {
}

IRBase::IRBase(IRData* irdata) {
    if (irdata != nullptr) {
        m_IRDataList.push_back(irdata);
    }
}

IRBase::IRBase(IRMethod* irMethod) {
    m_IRMethod = irMethod;
}

void IRBase::AddIRData(IRData* irData) {
    if (irData == nullptr) {
        return;
    }
    m_IRDataList.push_back(irData);
}

void IRBase::AddIRRangeData(const std::vector<IRData*>& ienumData) {
    for (auto v : ienumData) {
        if (v == nullptr) {
            continue;
        }
        m_IRDataList.push_back(v);
    }
}

std::string IRBase::ToIRString() {
    std::ostringstream sb;

    for (size_t i = 0; i < m_IRDataList.size(); i++) {
        if (m_IRDataList[i] == nullptr) {
            continue;
        }
        sb << m_IRDataList[i]->ToString() << std::endl;
    }
    return sb.str();
}

} // namespace IR
} // namespace SimpleLanguage