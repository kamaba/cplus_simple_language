//****************************************************************************
//  File:      MetaExpressBase.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "MetaExpressBase.h"
#include "../MetaClass.h"
#include "../MetaType.h"
#include "../MetaTypeFactory.h"

namespace SimpleLanguage {
namespace Core {

int MetaExpressNode::GetOpLevel() const {
    return MetaTypeFactory::GetOpLevelByMetaType(m_MetaDefineType);
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
} // namespace Core
} // namespace SimpleLanguage
