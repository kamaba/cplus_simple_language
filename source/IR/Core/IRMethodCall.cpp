//****************************************************************************
//  File:      IRMethodCall.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#include "IRMethodCall.h"
#include <sstream>

namespace SimpleLanguage {
namespace IR {

IRMethodCall::IRMethodCall(IRMetaType* mt, const std::vector<IRMetaType*>& mtList, IRMethod* irmethod, int paramCount) {
    m_MetaType = mt;
    m_IrTemplateMetaType = mtList;
    m_IRMethod = irmethod;
    m_ParamCount = paramCount;
}

std::string IRMethodCall::ToString() {
    std::stringstream sb;

    if (m_MetaType != nullptr) {
        sb << "[" << m_MetaType->GetIrMetaClass()->GetIrName();

        if (m_MetaType->GetIrMetaTypeList().size() > 0) {
            sb << "<";
            for (size_t i = 0; i < m_MetaType->GetIrMetaTypeList().size(); i++) {
                sb << m_MetaType->GetIrMetaTypeList()[i]->ToString();
                if (i < m_MetaType->GetIrMetaTypeList().size() - 1) {
                    sb << ",";
                }
            }
            sb << ">";
        }
        sb << "] ";
    }
    
    if (m_IRMethod != nullptr) {
        sb << m_IRMethod->GetId();
    }

    if (m_IrTemplateMetaType.size() > 0) {
        sb << "<";
        for (size_t i = 0; i < m_IrTemplateMetaType.size(); i++) {
            sb << m_IrTemplateMetaType[i]->ToString();
            if (i < m_IrTemplateMetaType.size() - 1) {
                sb << ",";
            }
        }
        sb << ">";
    }

    return sb.str();
}

} // namespace IR
} // namespace SimpleLanguage
