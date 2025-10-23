//****************************************************************************
//  File:      MetaCallStatements.h
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
        class FileMetaCallSyntax;
    }
    namespace Core {
        class MetaCallLink;
        class MetaClass;
        class MetaBlockStatements;
        class AllowUseSettings;
    }
}

namespace SimpleLanguage {
namespace Core {

class MetaCallStatements : public MetaStatements {
private:
    MetaCallLink* m_MetaCallLink = nullptr;
    Compile::FileMetaCallSyntax* m_FileMetaCallSyntax = nullptr;
    AllowUseSettings* m_AllowUseSettings = nullptr;

public:
    MetaCallLink* GetMetaCallLink() const;
    
    MetaCallStatements(MetaBlockStatements* mbs, Compile::FileMetaCallSyntax* fmcl);
    virtual ~MetaCallStatements() = default;
    
    virtual void UpdateOwnerMetaClass(MetaClass* ownerclass) override;
    virtual std::string ToFormatString() const override;
    std::string ToTokenString();
};

} // namespace Core
} // namespace SimpleLanguage