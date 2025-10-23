//****************************************************************************
//  File:      MetaReturnStatements.h
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
        class FileMetaKeyReturnSyntax;
    }
    namespace Core {
        class MetaExpressNode;
        class MetaType;
        class MetaClass;
        class MetaBlockStatements;
        class MetaFunction;
        class CreateExpressParam;
        class AllowUseSettings;
        class ExpressManager;
        class CoreMetaClassManager;
    }
}

namespace SimpleLanguage {
namespace Core {

class MetaReturnStatements : public MetaStatements {
private:
    Compile::FileMetaKeyReturnSyntax* m_FileMetaReturnSyntax = nullptr;
    MetaType* m_ReturnMetaDefineType = nullptr;
    MetaExpressNode* m_Express = nullptr;

public:
    MetaExpressNode* GetExpress() const;
    
    MetaReturnStatements(MetaBlockStatements* mbs, Compile::FileMetaKeyReturnSyntax* fmrs);
    virtual ~MetaReturnStatements() = default;
    
    virtual std::string ToFormatString() const override;
};

class MetaTRStatements : public MetaStatements {
public:
    MetaClass* returnMetaClass = nullptr;
    MetaExpressNode* m_Express = nullptr;

private:
    Compile::FileMetaKeyReturnSyntax* m_FileMetaReturnSyntax = nullptr;

public:
    MetaTRStatements(MetaBlockStatements* mbs, Compile::FileMetaKeyReturnSyntax* fmrs);
    virtual ~MetaTRStatements() = default;
    
    virtual std::string ToFormatString() const override;
};

} // namespace Core
} // namespace SimpleLanguage