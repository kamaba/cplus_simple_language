//****************************************************************************
//  File:      MetaWhileDoWhileStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description:  Handle for loop statements syntax and while/dowhile loop statements syntax !
//****************************************************************************

#pragma once

#include "MetaStatements.h"
#include <string>

namespace SimpleLanguage {
    namespace Compile {
        class FileMetaKeyForSyntax;
        class FileMetaConditionExpressSyntax;
        class FileMetaDefineVariableSyntax;
        class FileMetaOpAssignSyntax;
        class FileMetaCallSyntax;
        class Token;
    }
    namespace Core {
        class MetaVariable;
        class MetaIteratorVariable;
        class MetaBlockStatements;
        class MetaDefineVarStatements;
        class MetaAssignStatements;
        class MetaExpressNode;
        class MetaType;
        class CreateExpressParam;
        class AllowUseSettings;
        class MetaMemberFunction;
    }
}

namespace SimpleLanguage {
namespace Core {

class MetaForStatements : public MetaStatements {
private:
    bool m_IsForIn = false;
    MetaVariable* m_ForMetaVariable = nullptr;
    MetaVariable* m_ForInContent = nullptr;
    Compile::FileMetaKeyForSyntax* m_FileMetaKeyForSyntax = nullptr;
    MetaBlockStatements* m_ThenMetaStatements = nullptr;
    MetaDefineVarStatements* m_DefineVarStatements = nullptr;
    MetaAssignStatements* m_AssignStatements = nullptr;
    MetaExpressNode* m_ConditionExpress = nullptr;
    MetaAssignStatements* m_StepStatements = nullptr;

public:
    MetaForStatements(MetaBlockStatements* mbs, Compile::FileMetaKeyForSyntax* fmkfs);
    
    virtual void SetDeep(int dp) override;
    virtual std::string ToFormatString() const override;

private:
    void Parse();
};

class MetaWhileDoWhileStatements : public MetaStatements {
private:
    Compile::FileMetaConditionExpressSyntax* m_FileMetaKeyWhileSyntax = nullptr;
    MetaExpressNode* m_ConditionExpress = nullptr;
    MetaBlockStatements* m_ThenMetaStatements = nullptr;
    bool m_IsWhile = false;

public:
    MetaWhileDoWhileStatements(MetaBlockStatements* mbs, Compile::FileMetaConditionExpressSyntax* whileStatements);
    
    virtual void SetDeep(int dp) override;
    virtual std::string ToFormatString() const override;

private:
    void Parse();
};

} // namespace Core
} // namespace SimpleLanguage