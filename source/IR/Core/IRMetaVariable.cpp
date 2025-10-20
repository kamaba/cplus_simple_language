//****************************************************************************
//  File:      IRMetaVariable.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/11/15 12:00:00
//  Description: Meta class's ir attribute
//****************************************************************************

#include "IRMetaVariable.h"
#include <sstream>

namespace SimpleLanguage {
namespace IR {

IRMetaVariable::IRMetaVariable(MetaVariable* mv, int index) {
    m_MetaVariable = mv;
    m_Id = mv->GetHashCode();
    m_Index = index;
    m_Name = (mv->GetOwnerMetaBlockStatements() ? mv->GetOwnerMetaBlockStatements()->GetOwnerMetaFunction()->GetName() : "") + 
             (mv->IsStatic() ? "_static" : "_local") + "[" + mv->GetName() + "]";
    
    if (mv->GetVariableFrom() == MetaVariable::EVariableFrom::Member) {
        if (mv->IsStatic()) {
            m_IRMetaVariableFrom = IRMetaVariableFrom::Static;
        } else {
            m_IRMetaVariableFrom = IRMetaVariableFrom::Member;
        }
    }
    else if (mv->GetVariableFrom() == MetaVariable::EVariableFrom::Argument) {
        m_IRMetaVariableFrom = IRMetaVariableFrom::Argument;
    }
    else if (mv->GetVariableFrom() == MetaVariable::EVariableFrom::LocalStatement) {
        m_IRMetaVariableFrom = IRMetaVariableFrom::LocalStatement;
    }
    else if (mv->GetVariableFrom() == MetaVariable::EVariableFrom::Global) {
        m_IRMetaVariableFrom = IRMetaVariableFrom::Global;
    }
    else {
        Log::AddGenIR(EError::None, "IRMetaVariable 没有找到对应的from ");
    }
    
    m_IRMetaType = new IRMetaType(mv->GetMetaDefineType());
}

IRMetaVariable::IRMetaVariable(IRMetaClass* irmc, MetaMemberEnum* mme) {
    m_MetaVariable = mme;
    m_Id = mme->GetHashCode();
    m_Name = mme->GetOwnerMetaClass()->GetAllClassName() + "." + mme->GetName();
    m_ExpressNode = mme->GetExpress();
    m_IRMetaVariableFrom = IRMetaVariableFrom::Static;
}

IRMetaVariable::IRMetaVariable(IRMetaClass* irmc, MetaMemberData* mmd) {
    m_MetaVariable = mmd;
    m_Id = mmd->GetHashCode();
    m_Name = mmd->GetOwnerMetaClass()->GetAllClassName() + "." + mmd->GetName();
    m_ExpressNode = mmd->GetExpressNode();
    m_IRMetaVariableFrom = mmd->IsStatic() ? IRMetaVariableFrom::Static : IRMetaVariableFrom::Member;
}

IRMetaVariable::IRMetaVariable(IRMetaClass* irmc, MetaMemberVariable* mmv, int index) {
    m_MetaVariable = mmv;
    m_Id = mmv->GetHashCode();
    m_Index = index;
    m_Name = mmv->GetOwnerMetaClass()->GetAllClassName() + "." + mmv->GetName();
    m_ExpressNode = mmv->GetExpress();
    if (mmv->IsStatic() || mmv->IsConst()) {
        m_IRMetaVariableFrom = IRMetaVariableFrom::Static;
    } else {
        m_IRMetaVariableFrom = IRMetaVariableFrom::Member;
    }

    m_IRMetaType = new IRMetaType(mmv->GetMetaDefineType());
}

void IRMetaVariable::SetExpress(MetaExpressNode* men) {
    m_ExpressNode = men;
}

void IRMetaVariable::SetIRDataList(const std::vector<IRData*>& list) {
    m_IRDataList = list;
}

std::string IRMetaVariable::ToString() {
    return m_Name;
}

} // namespace IR
} // namespace SimpleLanguage
