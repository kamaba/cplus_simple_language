//****************************************************************************
//  File:      IRMetaType.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2025/9/5 12:00:00
//  Description: Meta class's ir attribute
//****************************************************************************

#include "IRMetaType.h"
#include "../IRManager.h"
#include <sstream>

namespace SimpleLanguage {
namespace IR {

IRMetaType::IRMetaType(MetaType* type) {
    if (type->GetEType() == EMetaTypeType::MetaClass) {
        m_IRMetaClass = IRManager::GetInstance().GetIRMetaClassById(type->GetTemplateMetaClass()->GetHashCode());
    }
    else if (type->GetEType() == EMetaTypeType::Template) {
        m_TemplateIndex = type->GetMetaTemplate()->GetIndex();
        m_IRMetaClass = IRManager::GetInstance().GetIRMetaClassById(type->GetTemplateMetaClass()->GetHashCode());
    }
    else {
        m_IRMetaClass = IRManager::GetInstance().GetIRMetaClassById(type->GetTemplateMetaClass()->GetHashCode());
    }
    
    auto templateMetaTypeList = type->GetTemplateMetaTypeList();
    for (size_t i = 0; i < templateMetaTypeList.size(); i++) {
        m_IRMetaTypeList.push_back(new IRMetaType(templateMetaTypeList[i]));
    }
}

IRMetaType::IRMetaType(IRMetaClass* irmc, const std::vector<IRMetaType*>& irlist) {
    m_IRMetaClass = irmc;
    m_IRMetaTypeList = irlist;
}

std::string IRMetaType::ToString() {
    std::stringstream sb;
    sb << m_IRMetaClass->GetIrName();
    return sb.str();
}

} // namespace IR
} // namespace SimpleLanguage
