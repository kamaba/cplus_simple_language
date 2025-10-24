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
#include "../Statements/MetaBlockStatements.h"
#include "../Statements/MetaIfStatements.h"
#include "../Statements/MetaSwitchStatements.h"
#include "../BaseMetaClass/CoreMetaClassManager.h"
#include "../../Compile/FileMeta/FileMetaSyntax.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

class MetaExecuteStatementsNode : public MetaExpressNode {
private:
    MetaIfStatements* m_MetaIfStatements = nullptr;
    MetaSwitchStatements* m_MetaSwitchStatements = nullptr;

public:
    // Constructors
    MetaExecuteStatementsNode(MetaType* mdt, MetaClass* ownerMC, MetaBlockStatements* mbs, MetaIfStatements* ifstate);
    MetaExecuteStatementsNode(MetaType* mdt, MetaClass* ownerMC, MetaBlockStatements* mbs, MetaSwitchStatements* switchstate);
    
    // Member methods
    void UpdateTrMetaVariable(MetaVariable* trmv);
    void SetDeep(int dp);
    virtual MetaType* GetReturnMetaDefineType() override;
    virtual std::string ToFormatString() override;
    
    // Static factory methods
    static MetaExecuteStatementsNode* CreateMetaExecuteStatementsNodeByIfExpress(
        MetaType* mdt, MetaClass* ownerMC, MetaBlockStatements* mbs, Compile::FileMetaKeyIfSyntax* ifStatements);
    
    static MetaExecuteStatementsNode* CreateMetaExecuteStatementsNodeBySwitchExpress(
        MetaType* mdt, MetaClass* ownerMC, MetaBlockStatements* mbs, Compile::FileMetaKeySwitchSyntax* switchStatements);
};

} // namespace Core
} // namespace SimpleLanguage

