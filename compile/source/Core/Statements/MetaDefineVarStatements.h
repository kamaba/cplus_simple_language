//****************************************************************************
//  File:      MetaDefineVarStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "MetaStatements.h"
#include <string>
#include <iostream>


namespace SimpleLanguage {
    class Compile::FileMetaCallSyntax;
namespace Core {

class MetaDefineVarStatements : public MetaStatements {
private:
    FileMetaDefineVariableSyntax* m_FileMetaDefineVariableSyntax = nullptr;
    FileMetaOpAssignSyntax* m_FileMetaOpAssignSyntax = nullptr;
    Compile::FileMetaCallSyntax* m_FileMetaCallSyntax = nullptr;
    
    MetaVariable* m_DefineVarMetaVariable = nullptr;
    MetaExpressNode* m_ExpressNode = nullptr;
    bool m_IsNeedCastStatements = false;

public:
    // 属性访问器
    MetaExpressNode* GetExpressNode() const { return m_ExpressNode; }
    MetaVariable* GetDefineVarMetaVariable() const { return m_DefineVarMetaVariable; }
    
    // 构造函数
    MetaDefineVarStatements(MetaBlockStatements* mbs);
    MetaDefineVarStatements(MetaBlockStatements* mbs, FileMetaDefineVariableSyntax* fmdvs);
    MetaDefineVarStatements(MetaBlockStatements* mbs, FileMetaOpAssignSyntax* fmoas);
    MetaDefineVarStatements(MetaBlockStatements* mbs, FileMetaCallSyntax* callSyntax);
    
    // 重写的方法
    virtual void SetTRMetaVariable(MetaVariable* mv) override;
    virtual void SetDeep(int dp) override;
    virtual std::string ToFormatString() override;

private:
    void Parse();
};

} // namespace Core
} // namespace SimpleLanguage