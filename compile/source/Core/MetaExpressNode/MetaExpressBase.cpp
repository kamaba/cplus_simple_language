//****************************************************************************
//  File:      MetaExpressBase.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "MetaExpressBase.h"
#include "../MetaTypeFactory.h"

namespace SimpleLanguage {
namespace Core {

MetaExpressNode::MetaExpressNode() {
    m_OwnerMetaClass = nullptr;
    m_OwnerMetaBlockStatements = nullptr;
    m_MetaDefineType = nullptr;
}

int MetaExpressNode::GetOpLevel() const {
    return MetaTypeFactory::GetOpLevelByMetaType(m_MetaDefineType);
}

int MetaExpressNode::CalcParseLevel(int level) {
    return level;
}

void MetaExpressNode::CalcReturnType() {
    // 基类默认实现为空
}

void MetaExpressNode::Parse(AllowUseSettings* auc) {
    // 基类默认实现为空
}

MetaClass* MetaExpressNode::GetReturnMetaClass() {
    if (m_MetaDefineType == nullptr) {
        GetReturnMetaDefineType();
    }
    return m_MetaDefineType ? m_MetaDefineType->GetMetaClass() : nullptr;
}

void MetaExpressNode::SetMetaType(MetaType* mt) {
    m_MetaDefineType = mt;
}

MetaType* MetaExpressNode::GetReturnMetaDefineType() {
    return m_MetaDefineType;
}

std::string MetaExpressNode::ToFormatString() {
    return "";
}

std::string MetaExpressNode::ToTokenString() {
    return "";
}

} // namespace Core
} // namespace SimpleLanguage
