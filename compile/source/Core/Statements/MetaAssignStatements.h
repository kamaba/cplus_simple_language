//****************************************************************************
//  File:      MetaAssignStatements.h
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
        class Token;
        class FileMetaOpAssignSyntax;
        class FileMetaDefineVariableSyntax;
    }
namespace Core {

    // 前向声明
    class MetaExpressNode;
    class MetaCallLinkExpressNode;
    class MetaVariable;
    class MetaType;
    class MetaBlockStatements;
    class MetaClass;
    class MetaMemberFunction;
    class MetaInputParam;
    class AllowUseSettings;
    class CreateExpressParam;

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
    Compile::FileMetaOpAssignSyntax* m_FileMetaOpAssignSyntax = nullptr;
    Compile::FileMetaDefineVariableSyntax* m_FileMetaDefineVariableSyntax = nullptr;
    
    MetaVariable* m_MetaVariable = nullptr;
    ELeftRightOpSign m_AutoAddExpressOpSign;
    Compile::Token* m_SignToken = nullptr;
    bool m_IsSetStatements = false;
    bool m_IsAssign = false;
    bool m_IsNeedCastState = false; // 添加缺失的成员变量
    
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
    MetaAssignStatements(MetaBlockStatements* mbs, Compile::FileMetaOpAssignSyntax* fmos);
    MetaAssignStatements(MetaBlockStatements* mbs, Compile::FileMetaDefineVariableSyntax* fmos);
    
private:
    void Parse();

public:
    virtual void UpdateOwnerMetaClass(MetaClass* ownerclass) override;
    virtual std::string ToFormatString() const override;
    virtual std::string GetFormatString();
};

} // namespace Core
} // namespace SimpleLanguage