//****************************************************************************
//  File:      MetaExpressCalllink.h
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/8/5 12:00:00
//  Description:  
//****************************************************************************

#pragma once

#include "MetaExpressBase.h"
#include <string>

namespace SimpleLanguage {
    namespace Compile {
        class FileMetaCallLink;
    }
    namespace Core {
        class MetaCallLink;
        class MetaVariable;
        class MetaClass;
        class MetaBlockStatements;
        class AllowUseSettings;
        class MetaType;
        class MetaExpressNode;
    }
}

namespace SimpleLanguage {
namespace Core {

class MetaCallLinkExpressNode : public MetaExpressNode {
private:
    MetaCallLink* m_MetaCallLink = nullptr;
    MetaVariable* m_EqualMetaVariable = nullptr;

public:
    MetaCallLink* GetMetaCallLink() const;
    
    MetaCallLinkExpressNode(Compile::FileMetaCallLink* fmcl, MetaClass* mc, MetaBlockStatements* mbs, MetaVariable* mv);
    MetaCallLinkExpressNode(MetaCallLink* mcl);
    
    virtual void Parse(AllowUseSettings* auc) override;
    virtual int CalcParseLevel(int level) override;
    virtual void CalcReturnType() override;
    
    MetaVariable* GetMetaVariable();
    virtual MetaType* GetReturnMetaDefineType() override;
    MetaExpressNode* ConvertConstExpressNode();
    
    virtual std::string ToFormatString() override;
    virtual std::string ToTokenString() override;
};

} // namespace Core
} // namespace SimpleLanguage