//****************************************************************************
//  File:      MetaIfStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: handle if-elif-else statements, splite a one 'if' syntax statements and more
//  'elif' syntax  statements and one 'else' syntax statements
//****************************************************************************

#pragma once

#include "MetaStatements.h"
#include <string>
#include <vector>

namespace SimpleLanguage {
    namespace Compile {
        class FileMetaKeyIfSyntax;
        class FileMetaConditionExpressSyntax;
        class FileMetaKeyOnlySyntax;
    }
namespace Core {

    // 前向声明
    class MetaExpressNode;
    class MetaVariable;
    class MetaBlockStatements;
    class MetaAssignManager;
    class MetaClass;
    class CreateExpressParam;
    class AllowUseSettings;

class MetaIfStatements : public MetaStatements {
public:
    enum class IfElseState {
        Null,
        If,
        ElseIf,
        Else
    };
    
    class MetaElseIfStatements {
    private:
        Compile::FileMetaKeyOnlySyntax* m_ElseKeySyntax = nullptr;
        Compile::FileMetaConditionExpressSyntax* m_IfOrElseIfKeySyntax = nullptr;
        MetaVariable* m_BoolConditionVariable = nullptr;
        MetaExpressNode* m_Express = nullptr;
        MetaExpressNode* m_FinalExpress = nullptr;
        MetaBlockStatements* m_ThenMetaStatements = nullptr;
        MetaAssignManager* m_MetaAssignManager = nullptr;
        IfElseState m_IfElseState = IfElseState::Null;
        int m_Deep = 0;

    public:
        MetaBlockStatements* GetThenMetaStatements() const;
        IfElseState GetIfElseState() const;
        Compile::FileMetaKeyOnlySyntax* GetElseKeySyntax() const;
        MetaExpressNode* GetFinalExpress() const;
        MetaAssignManager* GetMetaAssignManager() const;
        MetaVariable* GetBoolConditionVariable() const;
        
        MetaElseIfStatements(MetaBlockStatements* mbs, Compile::FileMetaConditionExpressSyntax* ifexpress, MetaExpressNode* conditionExpress);
        MetaElseIfStatements(MetaBlockStatements* mbs, Compile::FileMetaKeyOnlySyntax* elseKeySyntax);
        
        void ParseDefine(MetaVariable* mv);
        void SetIfElseState(IfElseState ieState);
        void SetDeep(int d);
        
        std::string ToFormatString();

    private:
        void Parse();
    };

private:
    std::vector<MetaElseIfStatements*> m_MetaElseIfStatements;
    Compile::FileMetaKeyIfSyntax* m_FileMetaKeyIfSyntax = nullptr;
    MetaVariable* m_MetaVariable = nullptr;

public:
    std::vector<MetaElseIfStatements*> GetMetaElseIfStatements() const;
    Compile::FileMetaKeyIfSyntax* GetFileMetaKeyIfSyntax() const;
    
    MetaIfStatements(MetaBlockStatements* mbs, Compile::FileMetaKeyIfSyntax* fm, MetaVariable* mv = nullptr);
    
    virtual void UpdateOwnerMetaClass(MetaClass* ownerclass) override;
    virtual void SetDeep(int dp) override;
    virtual void SetTRMetaVariable(MetaVariable* mv) override;
    virtual std::string ToFormatString() const override;

private:
    void Parse();
    void AddIfElseStateStatements(MetaElseIfStatements* meis, IfElseState ife);
};

} // namespace Core
} // namespace SimpleLanguage