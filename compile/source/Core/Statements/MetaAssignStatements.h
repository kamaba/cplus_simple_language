//****************************************************************************
//  File:      MetaAssignStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "MetaStatements.h"
#include "../MetaExpressNode/MetaExpressBase.h"
#include "../MetaExpressNode/MetaExpressCalllink.h"
#include "../MetaExpressNode/MetaExpressConst.h"
#include "../MetaExpressNode/MetaExpressOperator.h"
#include "../MetaExpressNode/MetaExecuteStatementsNode.h"
#include "../MetaVariable.h"
#include "../MetaType.h"
#include "../MetaClass.h"
#include "../MetaBlockStatements.h"
#include "../MetaCallLink.h"
#include "../BaseMetaClass/CoreMetaClassManager.h"
#include "../ClassManager.h"
#include "../ExpressManager.h"
#include "../AllowUseSettings.h"
#include "../Compile/CoreFileMeta/FileMetaOpAssignSyntax.h"
#include "../Compile/CoreFileMeta/FileMetaDefineVariableSyntax.h"
#include "../Compile/Token.h"
#include "../Debug/Log.h"
#include "../Global.h"
#include <string>
#include <sstream>

namespace SimpleLanguage {
namespace Core {

class MetaAssignManager {
private:
    MetaExpressNode* m_ExpressNode = nullptr;
    MetaVariable* m_JudgmentValueMetaVariable = nullptr;
    bool m_IsNeedSetMetaVariable = false;
    MetaBlockStatements* m_MetaBlockStatements = nullptr;
    MetaType* m_MetaDefineType = nullptr;

public:
    MetaVariable* GetJudgmentValueMetaVariable() const;
    MetaExpressNode* GetExpressNode() const;
    bool IsNeedSetMetaVariable() const;
    
    MetaAssignManager(MetaExpressNode* expressNode, MetaBlockStatements* mbs, MetaType* defaultMdt);
    void CreateMetaVariable();
};

class MetaAssignStatements : public MetaStatements {
private:
    FileMetaOpAssignSyntax* m_FileMetaOpAssignSyntax = nullptr;
    FileMetaDefineVariableSyntax* m_FileMetaDefineVariableSyntax = nullptr;
    
    MetaVariable* m_MetaVariable = nullptr;
    ELeftRightOpSign m_AutoAddExpressOpSign;
    Token* m_SignToken = nullptr;
    bool m_IsSetStatements = false;
    bool m_IsAssign = false;
    
    MetaExpressNode* m_ExpressNode = nullptr;
    MetaCallLinkExpressNode* m_LeftMetaExpress = nullptr;
    MetaExpressNode* m_FinalMetaExpress = nullptr;

public:
    MetaExpressNode* GetFinalMetaExpress() const;
    MetaVariable* GetMetaVariable() const;
    MetaExpressNode* GetExpressNode() const;
    MetaCallLinkExpressNode* GetLeftMetaExpress() const;
    bool IsNewStatements() const;
    
    MetaAssignStatements(MetaBlockStatements* mbs);
    MetaAssignStatements(MetaBlockStatements* mbs, FileMetaOpAssignSyntax* fmos);
    MetaAssignStatements(MetaBlockStatements* mbs, FileMetaDefineVariableSyntax* fmos);
    
private:
    void Parse();

public:
    virtual void UpdateOwnerMetaClass(MetaClass* ownerclass) override;
    virtual std::string ToFormatString() override;
    virtual std::string GetFormatString();
};

} // namespace Core
} // namespace SimpleLanguage
