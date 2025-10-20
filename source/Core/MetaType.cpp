//****************************************************************************
//  File:      MetaType.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta type class
//****************************************************************************

#include "MetaType.h"
#include "MetaClass.h"
#include "MetaTemplate.h"
#include <sstream>

namespace SimpleLanguage {
namespace Core {

MetaType::MetaType() {
    m_EType = EMetaTypeType::MetaClass;
    m_MetaClass = nullptr;
    m_IsTemplate = false;
    m_IsIncludeTemplate = false;
}

MetaType::MetaType(MetaClass* metaClass) {
    m_EType = EMetaTypeType::MetaClass;
    m_MetaClass = metaClass;
    m_IsTemplate = false;
    m_IsIncludeTemplate = false;
}

MetaType::MetaType(const MetaType& mt) {
    m_EType = mt.m_EType;
    m_MetaClass = mt.m_MetaClass;
    m_TemplateMetaTypeList = mt.m_TemplateMetaTypeList;
    m_IsTemplate = mt.m_IsTemplate;
    m_IsIncludeTemplate = mt.m_IsIncludeTemplate;
}

std::string MetaType::ToFormatString() const {
    std::stringstream ss;
    
    if (m_MetaClass != nullptr) {
        ss << m_MetaClass->GetName();
    }
    
    if (m_IsTemplate && !m_TemplateMetaTypeList.empty()) {
        ss << "<";
        for (size_t i = 0; i < m_TemplateMetaTypeList.size(); ++i) {
            if (i > 0) {
                ss << ", ";
            }
            ss << m_TemplateMetaTypeList[i]->ToFormatString();
        }
        ss << ">";
    }
    
    return ss.str();
}

bool MetaType::IsIncludeClassTemplate(MetaClass* metaClass) const {
    if (m_MetaClass == metaClass) {
        return true;
    }
    
    for (const auto& templateType : m_TemplateMetaTypeList) {
        if (templateType->IsIncludeClassTemplate(metaClass)) {
            return true;
        }
    }
    
    return false;
}

bool MetaType::EqualMetaDefineType(const MetaType* v, const MetaType* mt) {
    if (v == nullptr || mt == nullptr) {
        return v == mt;
    }
    
    if (v->m_EType != mt->m_EType) {
        return false;
    }
    
    if (v->m_MetaClass != mt->m_MetaClass) {
        return false;
    }
    
    if (v->m_IsTemplate != mt->m_IsTemplate) {
        return false;
    }
    
    if (v->m_IsIncludeTemplate != mt->m_IsIncludeTemplate) {
        return false;
    }
    
    if (v->m_TemplateMetaTypeList.size() != mt->m_TemplateMetaTypeList.size()) {
        return false;
    }
    
    for (size_t i = 0; i < v->m_TemplateMetaTypeList.size(); ++i) {
        if (!EqualMetaDefineType(v->m_TemplateMetaTypeList[i], mt->m_TemplateMetaTypeList[i])) {
            return false;
        }
    }
    
    return true;
}

} // namespace Core
} // namespace SimpleLanguage