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

namespace SimpleLanguage {
    namespace Compile {
        class FileMetaDefineVariableSyntax;
        class FileMetaOpAssignSyntax;
        class FileMetaCallSyntax;
        class FileMetaBaseTerm;
    }
    namespace Core {
        class MetaVariable;
        class MetaExpressNode;
        class MetaBlockStatements;
        class MetaType;
        class MetaClass;
        class MetaMemberFunction;
        class MetaGenTemplateClass;
        class MetaConstExpressNode;
        class MetaCallLinkExpressNode;
        class MetaExecuteStatementsNode;
        class ClassManager;
        class TypeManager;
        class ExpressManager;
        class CreateExpressParam;
        class AllowUseSettings;
        class Token;
    }
}

namespace SimpleLanguage {
namespace Core {

class MetaDefineVarStatements : public MetaStatements {
private:
    Compile::FileMetaDefineVariableSyntax* m_FileMetaDefineVariableSyntax = nullptr;
    Compile::FileMetaOpAssignSyntax* m_FileMetaOpAssignSyntax = nullptr;
    Compile::FileMetaCallSyntax* m_FileMetaCallSyntax = nullptr;
    
    MetaVariable* m_DefineVarMetaVariable = nullptr;
    MetaExpressNode* m_ExpressNode = nullptr;
    bool m_IsNeedCastStatements = false;

public:
    // 属性访问器
    MetaExpressNode* GetExpressNode() const;
    MetaVariable* GetDefineVarMetaVariable() const;
    
    // 构造函数
    MetaDefineVarStatements(MetaBlockStatements* mbs);
    MetaDefineVarStatements(MetaBlockStatements* mbs, Compile::FileMetaDefineVariableSyntax* fmdvs);
    MetaDefineVarStatements(MetaBlockStatements* mbs, Compile::FileMetaOpAssignSyntax* fmoas);
    MetaDefineVarStatements(MetaBlockStatements* mbs, Compile::FileMetaCallSyntax* callSyntax);
    virtual ~MetaDefineVarStatements() = default;
    
    // 重写的方法
    virtual void SetTRMetaVariable(MetaVariable* mv) override;
    virtual void SetDeep(int dp) override;
    virtual std::string ToFormatString() const override;

private:
    void Parse();
};

} // namespace Core
} // namespace SimpleLanguage