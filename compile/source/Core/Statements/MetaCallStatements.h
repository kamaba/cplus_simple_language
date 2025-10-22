//****************************************************************************
//  File:      MetaCallStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "MetaStatements.h"
#include "../MetaCallLink.h"
#include "../AllowUseSettings.h"
#include "../Compile/CoreFileMeta/FileMetaCallSyntax.h"
#include "../Global.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

class MetaCallStatements : public MetaStatements {
private:
    MetaCallLink* m_MetaCallLink = nullptr;
    FileMetaCallSyntax* m_FileMetaCallSyntax = nullptr;
    AllowUseSettings* m_AllowUseSettings = new AllowUseSettings();

public:
    MetaCallLink* GetMetaCallLink() const { return m_MetaCallLink; }
    
    MetaCallStatements(MetaBlockStatements* mbs, FileMetaCallSyntax* fmcl) : MetaStatements(mbs) {
        m_FileMetaCallSyntax = fmcl;
        
        m_AllowUseSettings->SetUseNotStatic(false);
        m_AllowUseSettings->SetUseNotConst(false);
        m_AllowUseSettings->SetCallConstructFunction(true);
        m_AllowUseSettings->SetCallFunction(true);
        
        m_MetaCallLink = new MetaCallLink(fmcl->GetVariableRef(), mbs->GetOwnerMetaClass(), mbs, nullptr, nullptr);
        m_MetaCallLink->Parse(m_AllowUseSettings);
        m_MetaCallLink->CalcReturnType();
    }
    
    virtual void UpdateOwnerMetaClass(MetaClass* ownerclass) override {
        MetaStatements::UpdateOwnerMetaClass(ownerclass);
    }
    
    virtual std::string ToFormatString() override {
        std::string result;
        for (int i = 0; i < m_RealDeep; i++) {
            result += Global::GetTabChar();
        }
        result += m_MetaCallLink ? m_MetaCallLink->ToFormatString() : "";
        result += "\n";
        if (m_NextMetaStatements != nullptr) {
            result += m_NextMetaStatements->ToFormatString();
        }
        return result;
    }
    
    std::string ToTokenString() {
        std::string result;
        // result += m_FileMetaCallLink->ToTokenString();
        return result;
    }
};

} // namespace Core
} // namespace SimpleLanguage
