//****************************************************************************
//  File:      IRMethodCall.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../IRMetaType.h"
#include "../IRMethod.h"
#include <vector>
#include <string>

namespace SimpleLanguage {
namespace IR {

class IRMethodCall {
private:
    std::vector<IRMetaType*> m_IrTemplateMetaType;
    IRMetaType* m_MetaType = nullptr;
    IRMethod* m_IRMethod = nullptr;
    int m_ParamCount = 0;

public:
    std::vector<IRMetaType*>& GetIrTemplateMetaType() { return m_IrTemplateMetaType; }
    IRMetaType* GetMetaType() const { return m_MetaType; }
    IRMethod* GetIrMethod() const { return m_IRMethod; }
    int GetParamCount() const { return m_ParamCount; }
    
    IRMethodCall(IRMetaType* mt, const std::vector<IRMetaType*>& mtList, IRMethod* irmethod, int paramCount);
    
    std::string ToString();
};

} // namespace IR
} // namespace SimpleLanguage
