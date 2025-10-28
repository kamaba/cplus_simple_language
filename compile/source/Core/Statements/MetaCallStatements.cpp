//****************************************************************************
//  File:      MetaCallStatements.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#include "../../Compile/FileMeta/FileMetaSyntax.h"
#include "MetaCallStatements.h"
#include "../MetaCallLink.h"
#include "../AllowUseSettings.h"
#include "../MetaClass.h"
#include "MetaBlockStatements.h"
#include "../../Define.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

MetaCallLink* MetaCallStatements::GetMetaCallLink() const {
    return m_MetaCallLink;
}

MetaCallStatements::MetaCallStatements(MetaBlockStatements* mbs, Compile::FileMetaCallSyntax* fmcl) : MetaStatements(mbs) {
    m_FileMetaCallSyntax = fmcl;
    
    m_AllowUseSettings = new AllowUseSettings();
    m_AllowUseSettings->SetUseNotStatic(false);
    m_AllowUseSettings->SetUseNotConst(false);
    m_AllowUseSettings->SetCallConstructFunction(true);
    m_AllowUseSettings->SetCallFunction(true);
    
    m_MetaCallLink = new MetaCallLink(fmcl->GetVariableRef(), mbs->GetOwnerMetaClass(), mbs, nullptr, nullptr);
    m_MetaCallLink->Parse(m_AllowUseSettings);
    m_MetaCallLink->CalcReturnType();
}

void MetaCallStatements::UpdateOwnerMetaClass(MetaClass* ownerclass) {
    MetaStatements::UpdateOwnerMetaClass(ownerclass);
}

std::string MetaCallStatements::ToFormatString() const {
    std::string result;
    for (int i = 0; i < GetRealDeep(); i++) {
        result += Global::GetTabChar();
    }
    result += m_MetaCallLink ? m_MetaCallLink->ToFormatString() : "";
    result += "\n";
    if (m_NextMetaStatements != nullptr) {
        result += m_NextMetaStatements->ToFormatString();
    }
    return result;
}

std::string MetaCallStatements::ToTokenString() {
    std::string result;
    // result += m_FileMetaCallLink->ToTokenString();
    return result;
}

} // namespace Core
} // namespace SimpleLanguage