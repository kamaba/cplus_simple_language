//****************************************************************************
//  File:      MetaStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaBase.h"
#include "../MetaClass.h"
#include "../MetaFunction.h"
#include "../MetaVariable.h"
#include "MetaBlockStatements.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

class MetaStatements : public MetaBase {
protected:
    MetaVariable* m_TrMetaVariable = nullptr;
    MetaBlockStatements* m_OwnerMetaBlockStatements = nullptr;
    MetaStatements* m_NextMetaStatements = nullptr;
    bool m_IsNeedCastState = false;

public:
    MetaStatements* GetNextMetaStatements() const;
    MetaBlockStatements* GetParentBlockStatements() const;
    MetaVariable* GetTrMetaVariable() const;
    
    virtual MetaClass* GetOwnerMetaClass();
    virtual MetaFunction* GetOwnerMetaFunction();

protected:
    MetaStatements();
    MetaStatements(MetaBlockStatements* mf);

public:
    virtual void SetTRMetaVariable(MetaVariable* mv);
    virtual void SetDeep(int dp) override;
    virtual void SetNextStatements(MetaStatements* ms);
    virtual void UpdateOwnerMetaClass(MetaClass* ownerclass);
};

} // namespace Core
} // namespace SimpleLanguage
