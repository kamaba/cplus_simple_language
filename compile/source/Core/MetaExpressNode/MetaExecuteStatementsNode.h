//****************************************************************************
//  File:      MetaExecuteStatementsNode.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "MetaExpressBase.h"
#include "../MetaType.h"
#include "../MetaClass.h"
#include "../MetaBlockStatements.h"
#include "../Statements/MetaIfStatements.h"
#include "../Statements/MetaSwitchStatements.h"
#include "../BaseMetaClass/CoreMetaClassManager.h"
#include "../Compile/CoreFileMeta/FileMetaKeyIfSyntax.h"
#include "../Compile/CoreFileMeta/FileMetaKeySwitchSyntax.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

class MetaExecuteStatementsNode : public MetaExpressNode {
private:
    MetaIfStatements* m_MetaIfStatements = nullptr;
    MetaSwitchStatements* m_MetaSwitchStatements = nullptr;

public:
    MetaExecuteStatementsNode(MetaType* mdt, MetaClass* ownerMC, MetaBlockStatements* mbs, MetaIfStatements* ifstate) {
        m_MetaDefineType = mdt;
        m_OwnerMetaClass = ownerMC;
        m_OwnerMetaBlockStatements = mbs;
        m_MetaIfStatements = ifstate;
    }
    
    MetaExecuteStatementsNode(MetaType* mdt, MetaClass* ownerMC, MetaBlockStatements* mbs, MetaSwitchStatements* switchstate) {
        m_MetaDefineType = mdt;
        m_OwnerMetaClass = ownerMC;
        m_OwnerMetaBlockStatements = mbs;
        m_MetaSwitchStatements = switchstate;
    }
    
    void UpdateTrMetaVariable(MetaVariable* trmv) {
        if (m_MetaIfStatements != nullptr) {
            m_MetaIfStatements->SetTRMetaVariable(trmv);
        } else if (m_MetaSwitchStatements != nullptr) {
            m_MetaSwitchStatements->SetTRMetaVariable(trmv);
        }
    }
    
    void SetDeep(int dp) {
        if (m_MetaIfStatements != nullptr) {
            m_MetaIfStatements->SetDeep(dp);
        } else if (m_MetaSwitchStatements != nullptr) {
            m_MetaSwitchStatements->SetDeep(dp);
        }
    }
    
    virtual MetaType* GetReturnMetaDefineType() override {
        if (m_MetaDefineType != nullptr) {
            return m_MetaDefineType;
        }
        if (m_MetaIfStatements != nullptr || m_MetaSwitchStatements != nullptr) {
            m_MetaDefineType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        }
        return m_MetaDefineType;
    }
    
    virtual std::string ToFormatString() override {
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
    
    static MetaExecuteStatementsNode* CreateMetaExecuteStatementsNodeByIfExpress(MetaType* mdt, MetaClass* ownerMC, MetaBlockStatements* mbs, FileMetaKeyIfSyntax* ifStatements) {
        if (ifStatements == nullptr) return nullptr;
        
        MetaIfStatements* newIfStatements = new MetaIfStatements(mbs, ifStatements);
        MetaExecuteStatementsNode* mesn = new MetaExecuteStatementsNode(mdt, ownerMC, mbs, newIfStatements);
        
        return mesn;
    }
    
    static MetaExecuteStatementsNode* CreateMetaExecuteStatementsNodeBySwitchExpress(MetaType* mdt, MetaClass* ownerMC, MetaBlockStatements* mbs, FileMetaKeySwitchSyntax* switchStatements) {
        if (switchStatements == nullptr) return nullptr;
        
        MetaSwitchStatements* newSwitchStatements = new MetaSwitchStatements(mbs, switchStatements);
        MetaExecuteStatementsNode* mesn = new MetaExecuteStatementsNode(mdt, ownerMC, mbs, newSwitchStatements);
        
        return mesn;
    }
};

} // namespace Core
} // namespace SimpleLanguage
