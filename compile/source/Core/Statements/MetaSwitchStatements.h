//****************************************************************************
//  File:      MetaSwitchStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description:  Metadata Switch statements 
//****************************************************************************

#pragma once

#include "MetaStatements.h"
#include <string>
#include <vector>

namespace SimpleLanguage {
    namespace Compile {
        class Token;
        class FileMetaKeySwitchSyntax;
        class FileMetaKeyCaseSyntax;
    }
    namespace Core {
        class MetaConstExpressNode;
        class MetaCallLinkExpressNode;
        class MetaCallLink;
        class MetaBlockStatements;
        class MetaVariable;
        class MetaClass;
        class MetaType;
        class MetaMemberFunction;
        class AllowUseSettings;
    }
}

namespace SimpleLanguage {
namespace Core {

class MetaSwitchStatements : public MetaStatements {
public:
    enum class SwitchMatchType {
        None,
        ClassType,
        ClassValue,
        ConstValue
    };
    
    class MetaNextStatements : public MetaStatements {
    public:
        Compile::Token* token = nullptr;
        
        MetaNextStatements(Compile::Token* _token);
        virtual ~MetaNextStatements() = default;
        virtual std::string ToFormatString() const override;
    };
    
    class MetaCaseStatements : public MetaStatements {
    public:
        enum class SwitchCaseType {
            None,
            Const,
            ClassType
        };
        
        SwitchCaseType switchCaseType = SwitchCaseType::None;
        std::vector<MetaConstExpressNode*> constExpressList;
        MetaClass* matchTypeClass = nullptr;
        MetaVariable* matchMetaVariable = nullptr;
        MetaVariable* defineMetaVariable = nullptr;
        MetaBlockStatements* thenMetaStatements = nullptr;
        bool isContinueNext = false;

    private:
        Compile::FileMetaKeyCaseSyntax* m_FileMetaKeyCaseSyntax = nullptr;

    public:
        MetaCaseStatements(Compile::FileMetaKeyCaseSyntax* fmkcs, MetaBlockStatements* mbs);
        virtual ~MetaCaseStatements() = default;
        
    private:
        void Parse();

    public:
        virtual void SetDeep(int dp) override;
        void SetMatchMetaVariable(MetaVariable* matchMV);
        virtual std::string ToFormatString() const override;
    };

private:
    SwitchMatchType m_MatchType;
    std::vector<MetaCaseStatements*> metaCaseStatements;
    MetaBlockStatements* defaultMetaStatements = nullptr;
    MetaVariable* matchMV = nullptr;
    MetaCallLink* m_MetaCallLink = nullptr;
    Compile::FileMetaKeySwitchSyntax* m_FileMetaKeySwitchSyntax = nullptr;

public:
    MetaSwitchStatements(MetaBlockStatements* mbs, Compile::FileMetaKeySwitchSyntax* fmkss, MetaVariable* retMv = nullptr);
    virtual ~MetaSwitchStatements() = default;
    
private:
    void Parse();

public:
    virtual void SetDeep(int dp) override;
    virtual void SetTRMetaVariable(MetaVariable* mv) override;
    virtual std::string ToFormatString() const override;
};

} // namespace Core
} // namespace SimpleLanguage