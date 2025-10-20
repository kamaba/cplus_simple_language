//****************************************************************************
//  File:      MetaExpressCalllink.h
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/8/5 12:00:00
//  Description:  
//****************************************************************************

#pragma once

#include "MetaExpressBase.h"
#include "../MetaCallLink.h"
#include "../MetaVariable.h"
#include "../Compile/CoreFileMeta/FileMetaCallLink.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

class MetaCallLinkExpressNode : public MetaExpressNode {
private:
    MetaCallLink* m_MetaCallLink = nullptr;
    MetaVariable* m_EqualMetaVariable = nullptr;

public:
    MetaCallLink* GetMetaCallLink() const { return m_MetaCallLink; }
    
    MetaCallLinkExpressNode(FileMetaCallLink* fmcl, MetaClass* mc, MetaBlockStatements* mbs, MetaVariable* mv) {
        m_OwnerMetaClass = mc;
        m_OwnerMetaBlockStatements = mbs;
        m_EqualMetaVariable = mv;
        if (fmcl != nullptr) {
            m_MetaCallLink = new MetaCallLink(fmcl, mc, mbs, mv ? mv->GetMetaDefineType() : nullptr, mv);
        }
    }
    
    MetaCallLinkExpressNode(MetaCallLink* mcl) {
        m_MetaCallLink = mcl;
    }
    
    virtual void Parse(AllowUseSettings* auc) override {
        if (m_MetaCallLink != nullptr) {
            m_MetaCallLink->Parse(auc);
        }
    }
    
    virtual int CalcParseLevel(int level) override {
        if (m_MetaCallLink != nullptr) {
            level = m_MetaCallLink->CalcParseLevel(level);
        }
        return level;
    }
    
    virtual void CalcReturnType() override {
        if (m_MetaCallLink != nullptr) {
            m_MetaCallLink->CalcReturnType();
        }
        m_MetaDefineType = GetReturnMetaDefineType();
    }
    
    MetaVariable* GetMetaVariable() {
        if (m_MetaCallLink != nullptr) {
            return m_MetaCallLink->ExecuteGetMetaVariable();
        }
        return nullptr;
    }
    
    virtual MetaType* GetReturnMetaDefineType() override {
        if (m_MetaDefineType != nullptr) {
            return m_MetaDefineType;
        }
        if (m_MetaCallLink == nullptr) {
            return nullptr;
        }
        m_MetaDefineType = m_MetaCallLink->GetMetaDefineType();
        return m_MetaDefineType;
    }
    
    MetaExpressNode* ConvertConstExpressNode() {
        if (m_MetaCallLink == nullptr) {
            return nullptr;
        }
        return m_MetaCallLink->GetMetaExpressNode();
    }
    
    virtual std::string ToFormatString() override {
        if (m_MetaCallLink != nullptr) {
            return m_MetaCallLink->ToFormatString();
        }
        return "ExpressCallLink Error!!";
    }
    
    virtual std::string ToTokenString() override {
        std::string result;
        if (m_MetaCallLink != nullptr) {
            result = m_MetaCallLink->ToTokenString();
        }
        return result;
    }
};

} // namespace Core
} // namespace SimpleLanguage
