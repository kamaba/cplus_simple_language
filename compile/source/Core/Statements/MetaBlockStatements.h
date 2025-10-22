//****************************************************************************
//  File:      MetaBlockStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description:  this's a statement in function! same link table model!
//****************************************************************************

#pragma once

#include "MetaStatements.h"
#include "../MetaFunction.h"
#include "../MetaVariable.h"
#include "../MetaDefineParamCollection.h"
#include "../Compile/CoreFileMeta/FileMetaBlockSyntax.h"
#include "../Global.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace SimpleLanguage {
namespace Core {

class MetaBlockStatements : public MetaStatements {
private:
    MetaFunction* m_OwnerMetaFunction = nullptr;
    bool m_IsOnFunction = false;  // on function or inner function
    
    std::unordered_map<std::string, MetaVariable*> m_MetaVariableDict;
    std::vector<MetaBlockStatements*> m_ChildrenMetaBlockStatementsList;
    MetaStatements* m_OwnerMetaStatements = nullptr;
    FileMetaBlockSyntax* m_FileMetaBlockSyntax = nullptr;

public:
    MetaBlockStatements* parent = nullptr;
    
    virtual MetaFunction* GetOwnerMetaFunction() const override;
    MetaStatements* GetOwnerMetaStatements() const;
    FileMetaBlockSyntax* GetFileMetaBlockSyntax() const;
    
    MetaBlockStatements(MetaBlockStatements* mbs);
    MetaBlockStatements(MetaFunction* mf);
    MetaBlockStatements(MetaFunction* mf, FileMetaBlockSyntax* fmbs);
    MetaBlockStatements(MetaBlockStatements* mbs, FileMetaBlockSyntax* fmbs);
    
    void SetFileMetaBlockSyntax(FileMetaBlockSyntax* blockSyntax);
    void SetOwnerMetaStatements(MetaStatements* ms);
    virtual void SetNextStatements(MetaStatements* ms) override;
    
    MetaStatements* FindNearestMetaForStatementsOrMetaWhileOrDoWhileStatements();
    virtual void SetDeep(int dp) override;
    
    MetaVariable* GetMetaVariable(const std::string& name);
    bool AddMetaVariable(MetaVariable* mv);
    void AddFrontStatements(MetaStatements* ms);
    void AddFrontToEndStatements(MetaStatements* ms);
    bool UpdateMetaVariableDict(MetaVariable* mv);
    void GetCalcMetaVariableList(std::vector<MetaVariable*>& list);
    bool GetIsMetaVariable(const std::string& name, bool isFromParent = true);
    bool AddOnlyNameMetaVariable(const std::string& name);
    MetaVariable* GetMetaVariableByName(const std::string& name, bool isFromParent = true);
    void SetMetaMemberParamCollection(MetaDefineParamCollection* mmpc);
    
    virtual std::string ToFormatString() override;
};

} // namespace Core
} // namespace SimpleLanguage
