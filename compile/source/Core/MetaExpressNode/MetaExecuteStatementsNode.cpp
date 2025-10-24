//****************************************************************************
//  File:      MetaExecuteStatementsNode.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "MetaExecuteStatementsNode.h"

namespace SimpleLanguage {
namespace Core {

MetaExecuteStatementsNode::MetaExecuteStatementsNode(MetaType* mdt, MetaClass* ownerMC, MetaBlockStatements* mbs, MetaIfStatements* ifstate) {
    m_MetaDefineType = mdt;
    m_OwnerMetaClass = ownerMC;
    m_OwnerMetaBlockStatements = mbs;
    m_MetaIfStatements = ifstate;
}

MetaExecuteStatementsNode::MetaExecuteStatementsNode(MetaType* mdt, MetaClass* ownerMC, MetaBlockStatements* mbs, MetaSwitchStatements* switchstate) {
    m_MetaDefineType = mdt;
    m_OwnerMetaClass = ownerMC;
    m_OwnerMetaBlockStatements = mbs;
    m_MetaSwitchStatements = switchstate;
}

void MetaExecuteStatementsNode::UpdateTrMetaVariable(MetaVariable* trmv) {
    if (m_MetaIfStatements != nullptr) {
        m_MetaIfStatements->SetTRMetaVariable(trmv);
    } else if (m_MetaSwitchStatements != nullptr) {
        m_MetaSwitchStatements->SetTRMetaVariable(trmv);
    }
}

void MetaExecuteStatementsNode::SetDeep(int dp) {
    if (m_MetaIfStatements != nullptr) {
        m_MetaIfStatements->SetDeep(dp);
    } else if (m_MetaSwitchStatements != nullptr) {
        m_MetaSwitchStatements->SetDeep(dp);
    }
}

MetaType* MetaExecuteStatementsNode::GetReturnMetaDefineType() {
    if (m_MetaDefineType != nullptr) {
        return m_MetaDefineType;
    }
    if (m_MetaIfStatements != nullptr || m_MetaSwitchStatements != nullptr) {
        m_MetaDefineType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    }
    return m_MetaDefineType;
}

std::string MetaExecuteStatementsNode::ToFormatString() {
    std::string result;
    
    if (m_MetaIfStatements != nullptr) {
        result = m_MetaIfStatements->ToFormatString();
    } else if (m_MetaSwitchStatements != nullptr) {
        result = m_MetaSwitchStatements->ToFormatString();
    } else {
        result = MetaExpressNode::ToFormatString();
    }
    return result;
}

MetaExecuteStatementsNode* MetaExecuteStatementsNode::CreateMetaExecuteStatementsNodeByIfExpress(
    MetaType* mdt, MetaClass* ownerMC, MetaBlockStatements* mbs, Compile::FileMetaKeyIfSyntax* ifStatements) {
    if (ifStatements == nullptr) return nullptr;
    
    MetaIfStatements* newIfStatements = new MetaIfStatements(mbs, ifStatements);
    MetaExecuteStatementsNode* mesn = new MetaExecuteStatementsNode(mdt, ownerMC, mbs, newIfStatements);
    
    return mesn;
}

MetaExecuteStatementsNode* MetaExecuteStatementsNode::CreateMetaExecuteStatementsNodeBySwitchExpress(
    MetaType* mdt, MetaClass* ownerMC, MetaBlockStatements* mbs, Compile::FileMetaKeySwitchSyntax* switchStatements) {
    if (switchStatements == nullptr) return nullptr;
    
    MetaSwitchStatements* newSwitchStatements = new MetaSwitchStatements(mbs, switchStatements);
    MetaExecuteStatementsNode* mesn = new MetaExecuteStatementsNode(mdt, ownerMC, mbs, newSwitchStatements);
    
    return mesn;
}

} // namespace Core
} // namespace SimpleLanguage