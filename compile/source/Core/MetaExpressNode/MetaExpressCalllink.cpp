//****************************************************************************
//  File:      MetaExpressCalllink.cpp
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/8/5 12:00:00
//  Description:  
//****************************************************************************

#include "MetaExpressCalllink.h"
#include "../MetaCallLink.h"
#include "../MetaVariable.h"
#include "../MetaClass.h"
#include "../MetaBlockStatements.h"
#include "../AllowUseSettings.h"
#include "../MetaType.h"
#include "../Compile/FileMeta/FileMetaCallLink.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

MetaCallLink* MetaCallLinkExpressNode::GetMetaCallLink() const {
    return m_MetaCallLink;
}

MetaCallLinkExpressNode::MetaCallLinkExpressNode(Compile::FileMetaCallLink* fmcl, MetaClass* mc, MetaBlockStatements* mbs, MetaVariable* mv) {
    m_OwnerMetaClass = mc;
    m_OwnerMetaBlockStatements = mbs;
    m_EqualMetaVariable = mv;
    if (fmcl != nullptr) {
        m_MetaCallLink = new MetaCallLink(fmcl, mc, mbs, mv ? mv->GetMetaDefineType() : nullptr, mv);
    }
}

MetaCallLinkExpressNode::MetaCallLinkExpressNode(MetaCallLink* mcl) {
    m_MetaCallLink = mcl;
}

void MetaCallLinkExpressNode::Parse(AllowUseSettings* auc) {
    if (m_MetaCallLink != nullptr) {
        m_MetaCallLink->Parse(auc);
    }
}

int MetaCallLinkExpressNode::CalcParseLevel(int level) {
    if (m_MetaCallLink != nullptr) {
        level = m_MetaCallLink->CalcParseLevel(level);
    }
    return level;
}

void MetaCallLinkExpressNode::CalcReturnType() {
    if (m_MetaCallLink != nullptr) {
        m_MetaCallLink->CalcReturnType();
    }
    m_MetaDefineType = GetReturnMetaDefineType();
}

MetaVariable* MetaCallLinkExpressNode::GetMetaVariable() {
    if (m_MetaCallLink != nullptr) {
        return m_MetaCallLink->ExecuteGetMetaVariable();
    }
    return nullptr;
}

MetaType* MetaCallLinkExpressNode::GetReturnMetaDefineType() {
    if (m_MetaDefineType != nullptr) {
        return m_MetaDefineType;
    }
    if (m_MetaCallLink == nullptr) {
        return nullptr;
    }
    m_MetaDefineType = m_MetaCallLink->GetMetaDefineType();
    return m_MetaDefineType;
}

MetaExpressNode* MetaCallLinkExpressNode::ConvertConstExpressNode() {
    if (m_MetaCallLink == nullptr) {
        return nullptr;
    }
    return m_MetaCallLink->GetMetaExpressNode();
}

std::string MetaCallLinkExpressNode::ToFormatString() {
    if (m_MetaCallLink != nullptr) {
        return m_MetaCallLink->ToFormatString();
    }
    return "ExpressCallLink Error!!";
}

std::string MetaCallLinkExpressNode::ToTokenString() {
    std::string result;
    if (m_MetaCallLink != nullptr) {
        result = m_MetaCallLink->ToTokenString();
    }
    return result;
}

} // namespace Core
} // namespace SimpleLanguage