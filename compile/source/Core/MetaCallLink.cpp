//****************************************************************************
//  File:      MetaCallLink.cpp
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/7/29 12:00:00
//  Description:  this's a common node handles
//****************************************************************************

#include "MetaCallNode.h"
#include "MetaCallLink.h"
#include "../Compile/FileMeta/FileMetaCommon.h"
#include <iostream>

namespace SimpleLanguage {
namespace Core {

MetaCallLink::MetaCallLink( Compile::FileMetaCallLink* fmcl, MetaClass* metaClass, MetaBlockStatements* mbs, MetaType* frontDefineMt, MetaVariable* mv) {
    m_FileMetaCallLink = fmcl;
    m_OwnerMetaClass = metaClass;
    m_OwnerMetaBlockStatements = mbs;
    CreateCallLinkNode(frontDefineMt, mv);
}

void MetaCallLink::CreateCallLinkNode(MetaType* frontDefineMt, MetaVariable* mv) {
    MetaCallNode* frontMetaNode = nullptr;
    if (m_FileMetaCallLink->GetCallNodeList().size() > 0) {
        Compile::FileMetaCallNode* fmcn = m_FileMetaCallLink->GetCallNodeList()[0];
        auto firstNode = new MetaCallNode(nullptr, fmcn, m_OwnerMetaClass, m_OwnerMetaBlockStatements, frontDefineMt);
        frontMetaNode = firstNode;
        m_CallNodeList.push_back(firstNode);
    }
    
    for (size_t i = 1; i < m_FileMetaCallLink->GetCallNodeList().size(); i += 2) {
        auto cn1 = m_FileMetaCallLink->GetCallNodeList()[i];
        auto cn2 = m_FileMetaCallLink->GetCallNodeList()[i + 1];
        
        if (cn1 != nullptr && cn2 != nullptr) {
            auto metaNode = new MetaCallNode(cn1, cn2, m_OwnerMetaClass, m_OwnerMetaBlockStatements, frontDefineMt);
            m_CallNodeList.push_back(metaNode);
        }
    }
}

void MetaCallLink::Parse(AllowUseSettings* auc) {
    m_AllowUseSettings = auc;
    ParseCallNodeList(auc);
    CreateVisitNodeList();
    ParseVisitNodeList(auc);
    CreateFinalCallNode();
    ParseFinalCallNode(auc);
}

void MetaCallLink::ParseCallNodeList(AllowUseSettings* auc) {
    for (auto& callNode : m_CallNodeList) {
        if (callNode != nullptr) {
            callNode->Parse();
        }
    }
}

void MetaCallLink::CalcReturnType() {
    CalcCallNodeListReturnType();
    CalcVisitNodeListReturnType();
    CalcFinalCallNodeReturnType();
}

void MetaCallLink::CalcCallNodeListReturnType() {
    for (auto& callNode : m_CallNodeList) {
        if (callNode != nullptr) {
            callNode->CalcReturnType();
        }
    }
}

void MetaCallLink::CreateVisitNodeList() {
    // 创建访问节点列表
    for (auto& callNode : m_CallNodeList) {
        if (callNode != nullptr) {
            // 这里需要根据实际的 MetaVisitNode 类来创建
            // MetaVisitNode* visitNode = new MetaVisitNode(callNode);
            // m_VisitNodeList.push_back(visitNode);
        }
    }
}

void MetaCallLink::ParseVisitNodeList(AllowUseSettings* auc) {
    for (auto& visitNode : m_VisitNodeList) {
        if (visitNode != nullptr) {
            // visitNode->parse(auc);
        }
    }
}

void MetaCallLink::CalcVisitNodeListReturnType() {
    for (auto& visitNode : m_VisitNodeList) {
        if (visitNode != nullptr) {
            // visitNode->calcReturnType();
        }
    }
}

void MetaCallLink::CreateFinalCallNode() {
    if (!m_VisitNodeList.empty()) {
        m_FinalCallNode = m_VisitNodeList.back();
    }
}

void MetaCallLink::ParseFinalCallNode(AllowUseSettings* auc) {
    if (m_FinalCallNode != nullptr) {
        // m_FinalCallNode->parse(auc);
    }
}

void MetaCallLink::CalcFinalCallNodeReturnType() {
    if (m_FinalCallNode != nullptr) {
        // m_FinalCallNode->calcReturnType();
    }
}

std::string MetaCallLink::ToFormatString() const {
    std::string result;
    
    for (auto& callNode : m_CallNodeList) {
        if (callNode != nullptr) {
            result += callNode->ToFormatString();
        }
    }
    
    return result;
}

std::string MetaCallLink::ToTokenString() const {
    std::string result;
    
    for (auto& callNode : m_CallNodeList) {
        if (callNode != nullptr) {
            result += callNode->ToTokenString();
        }
    }
    
    return result;
}

MetaCallLink* MetaCallLink::Clone() const {
    MetaCallLink* cloned = new MetaCallLink(m_FileMetaCallLink, m_OwnerMetaClass, m_OwnerMetaBlockStatements, nullptr, nullptr);
    
    // 克隆调用节点列表
    for (auto& callNode : m_CallNodeList) {
        if (callNode != nullptr) {
            cloned->m_CallNodeList.push_back(callNode->Clone());
        }
    }
    
    // 克隆访问节点列表
    for (auto& visitNode : m_VisitNodeList) {
        if (visitNode != nullptr) {
            // cloned->m_VisitNodeList.push_back(visitNode->clone());
        }
    }
    
    cloned->m_FinalCallNode = m_FinalCallNode;
    cloned->m_AllowUseSettings = m_AllowUseSettings;
    
    return cloned;
}

} // namespace Core
} // namespace SimpleLanguage

