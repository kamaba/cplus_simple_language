//****************************************************************************
//  File:      MetaStatements.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#include "MetaStatements.h"
#include "../MetaClass.h"
#include "MetaBlockStatements.h"
#include "../MetaFunction.h"
#include "../MetaVariable.h"

namespace SimpleLanguage {
namespace Core {

// MetaStatements implementation
MetaStatements* MetaStatements::GetNextMetaStatements() const {
    return m_NextMetaStatements;
}

MetaBlockStatements* MetaStatements::GetParentBlockStatements() const {
    return m_OwnerMetaBlockStatements;
}

MetaVariable* MetaStatements::GetTrMetaVariable() const {
    return m_TrMetaVariable;
}

MetaClass* MetaStatements::GetOwnerMetaClass() {
    return GetOwnerMetaFunction() ? GetOwnerMetaFunction()->GetOwnerMetaClass() : nullptr;
}

MetaFunction* MetaStatements::GetOwnerMetaFunction() {
    return m_OwnerMetaBlockStatements ? m_OwnerMetaBlockStatements->GetOwnerMetaFunction() : nullptr;
}

MetaStatements::MetaStatements() {
}

MetaStatements::MetaStatements(MetaBlockStatements* mf) {
    m_OwnerMetaBlockStatements = mf;
}

void MetaStatements::SetTRMetaVariable(MetaVariable* mv) {
    m_TrMetaVariable = mv;
    if (m_NextMetaStatements != nullptr) {
        m_NextMetaStatements->SetTRMetaVariable(mv);
    }
}

void MetaStatements::SetDeep(int dp) {
    m_Deep = dp;
    if (m_NextMetaStatements != nullptr) {
        m_NextMetaStatements->SetDeep(m_Deep);
    }
}

void MetaStatements::SetNextStatements(MetaStatements* ms) {
    m_NextMetaStatements = ms;
}

void MetaStatements::UpdateOwnerMetaClass(MetaClass* ownerclass) {
    if (m_TrMetaVariable != nullptr) {
        m_TrMetaVariable->SetOwnerMetaClass(ownerclass);
    }
    if (m_NextMetaStatements != nullptr) {
        m_NextMetaStatements->UpdateOwnerMetaClass(ownerclass);
    }
}

} // namespace Core
} // namespace SimpleLanguage
