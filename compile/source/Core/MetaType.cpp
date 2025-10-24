//****************************************************************************
//  File:      MetaType.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta type class
//****************************************************************************

#include "MetaType.h"
#include "MetaClass.h"
#include "MetaParam.h"
#include "MetaTemplate.h"
#include "MetaGenTemplateClass.h"
#include "MetaMemberFunction.h"
#include "MetaMemberEnum.h"
#include "MetaExpressNode/MetaExpressBase.h"
#include "MetaTemplate.h"
#include "MetaEnum.h"
#include "MetaData.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "../Debug/Log.h"
#include <sstream>

using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Core {

// Constructors
MetaType::MetaType() {
    m_EType = EMetaTypeType::None;
    m_MetaClass = nullptr;
    m_TypeInferenceClass = nullptr;
    m_ParentMetaType = nullptr;
    m_MetaTemplate = nullptr;
    m_DefaultExpressNode = nullptr;
    m_EnumValue = nullptr;
    m_IsDefineMetaClass = false;
}

MetaType::MetaType(MetaTemplate* mt) {
    m_EType = EMetaTypeType::Template;
    m_MetaTemplate = mt;
    m_MetaClass = mt->GetExtendsMetaClass();
}

MetaType::MetaType(MetaGenTemplateClass* mgtc, const std::vector<MetaType*>& mtList) {
    m_EType = EMetaTypeType::MetaGenClass;
    m_MetaClass = mgtc;
    m_TemplateMetaTypeList = mtList;
}

MetaType::MetaType(MetaClass* mc) {
    if (mc == nullptr) {
        Log::Write("Error MetaDefineType RetMetaClass is Null MetaMemberVariable Only MetaClass");
    }
    m_IsDefineMetaClass = false;
    m_MetaClass = mc;
    m_EType = EMetaTypeType::MetaClass;
}

MetaType::MetaType(MetaClass* mc, const std::vector<MetaType*>& mtList) {
    if (mc == nullptr) {
        Log::Write("Error MetaDefineType RetMetaClass is Null MetaMemberVariable Only MetaClass");
    }
    m_IsDefineMetaClass = false;
    m_MetaClass = mc;
    m_TemplateMetaTypeList = mtList;
    m_EType = EMetaTypeType::TemplateClassWithTemplate;
}

MetaType::MetaType(MetaClass* mc, MetaClass* templatemc, MetaInputTemplateCollection* mitc) {
    if (mc == nullptr) {
        Log::Write("Error MetaDefineType RetMetaClass is Null MetaMemberVariable Only MetaClass");
    }
    m_IsDefineMetaClass = false;
    if (mitc == nullptr) {
        m_MetaClass = mc;
    } else {
        m_MetaClass = templatemc;
        m_TemplateMetaTypeList = mitc->GetMetaTemplateParamsList();
    }
}

MetaType::MetaType(const MetaType& mt) : MetaBase(mt) {
    m_MetaClass = mt.m_MetaClass;
    m_TypeInferenceClass = mt.m_TypeInferenceClass;
    m_ParentMetaType = mt.m_ParentMetaType;
    m_MetaTemplate = mt.m_MetaTemplate;
    m_DefaultExpressNode = mt.m_DefaultExpressNode;
    m_EnumValue = mt.m_EnumValue;
    m_IsDefineMetaClass = mt.m_IsDefineMetaClass;
    m_EType = mt.m_EType;
    
    for (size_t i = 0; i < mt.m_TemplateMetaTypeList.size(); i++) {
        MetaType* mtc = new MetaType(*mt.m_TemplateMetaTypeList[i]);
        m_TemplateMetaTypeList.push_back(mtc);
    }
}

// Property implementations
std::string MetaType::GetName() const {
    return m_MetaClass != nullptr ? m_MetaClass->GetAllClassName() : "";
}

bool MetaType::IsEnum() const {
    return dynamic_cast<MetaEnum*>(m_MetaClass) != nullptr;
}

bool MetaType::IsData() const {
    return dynamic_cast<MetaData*>(m_MetaClass) != nullptr;
}

bool MetaType::IsArray() const {
    return m_MetaClass != nullptr && m_MetaClass->GetEType() == EType::Array;
}

bool MetaType::IsDynamicClass() const {
    return m_MetaClass == CoreMetaClassManager::GetInstance().GetDynamicMetaClass();
}

bool MetaType::IsDynamicData() const {
    return m_MetaClass == CoreMetaClassManager::GetInstance().GetDynamicMetaData();
}

// Method implementations
bool MetaType::IsCanForIn() const {
    if (dynamic_cast<MetaEnum*>(m_MetaClass)) {
        return true;
    }
    if (m_MetaClass != nullptr && 
        (m_MetaClass->GetEType() == EType::Array || m_MetaClass->GetEType() == EType::Range)) {
        return true;
    }
    return false;
}

MetaClass* MetaType::GetTemplateMetaClass(bool& isGTC) const {
    isGTC = false;
    if (auto mgtc = dynamic_cast<MetaGenTemplateClass*>(m_MetaClass)) {
        isGTC = true;
        return mgtc->GetMetaTemplateClass();
    }
    return m_MetaClass;
}

MetaClass* MetaType::GetTemplateMetaClass() const {
    if (auto mgtc = dynamic_cast<MetaGenTemplateClass*>(m_MetaClass)) {
        return mgtc->GetMetaTemplateClass();
    }
    return m_MetaClass;
}

bool MetaType::IsIncludeTemplate() const {
    for (size_t i = 0; i < m_TemplateMetaTypeList.size(); i++) {
        auto tmt = m_TemplateMetaTypeList[i];
        if (tmt->IsIncludeTemplate()) {
            return true;
        }
    }
    return m_MetaTemplate != nullptr;
}

bool MetaType::IsIncludeClassTemplate(MetaClass* ownerClass) const {
    if (m_MetaTemplate != nullptr && ownerClass->IsTemplateClass()) {
        //return ownerClass->GetMetaTemplateList().IndexOf(m_MetaTemplate) != -1;
    }
    for (size_t i = 0; i < m_TemplateMetaTypeList.size(); i++) {
        auto tmt = m_TemplateMetaTypeList[i];
        if (tmt->IsIncludeClassTemplate(ownerClass)) {
            return true;
        }
    }
    return false;
}

bool MetaType::IsIncludeFunctionTemplate(MetaMemberFunction* mmf) const {
    if (m_EType == EMetaTypeType::Template) {
        if (m_MetaTemplate != nullptr && mmf->IsTemplateFunction()) {
            //return mmf->GetMetaMemberTemplateCollection()->GetMetaTemplateList().IndexOf(m_MetaTemplate) != -1;
        }
    } else if (m_EType == EMetaTypeType::TemplateClassWithTemplate) {
        for (size_t i = 0; i < m_TemplateMetaTypeList.size(); i++) {
            auto tmt = m_TemplateMetaTypeList[i];
            if (tmt->IsIncludeFunctionTemplate(mmf)) {
                return true;
            }
        }
        return false;
    }
    return false;
}

MetaMemberFunction* MetaType::FindTemplateFunctionTemplate(MetaMemberFunction* mmf) const {
    if (m_EType == EMetaTypeType::TemplateClassWithTemplate) {
        if (m_TemplateMetaTypeList.size() == 0) return nullptr;
        for (size_t i = 0; i < m_TemplateMetaTypeList.size(); i++) {
            auto tmt = m_TemplateMetaTypeList[i];
            if (tmt->IsIncludeFunctionTemplate(mmf)) {
                return mmf;
            }
        }
        return nullptr;
    }
    return nullptr;
}

void MetaType::AddTemplateMetaType(MetaType* mt) {
    mt->m_ParentMetaType = this;
    m_TemplateMetaTypeList.push_back(mt);
}

MetaMemberFunction* MetaType::GetMetaMemberConstructFunction(MetaInputParamCollection* input) const {
    return m_MetaClass != nullptr ? m_MetaClass->GetMetaMemberConstructFunction(input) : nullptr;
}

bool MetaType::EqualMetaDefineType(const MetaType* mdtL, const MetaType* mdtR) {
    if (mdtL == nullptr || mdtR == nullptr) {
        return false;
    }

    if (mdtL->m_EType != mdtR->m_EType) {
        return false;
    }

    if (mdtL->m_EType == EMetaTypeType::Template) {
        if (mdtL->m_MetaTemplate == mdtR->m_MetaTemplate) {
            return true;
        }
    } else if (mdtL->m_EType == EMetaTypeType::MetaClass) {
        if (mdtL->m_MetaClass == mdtR->m_MetaClass) {
            return true;
        }
    } else {
        if (mdtL->m_MetaClass != mdtR->m_MetaClass) {
            return false;
        }
        if (mdtL->m_TemplateMetaTypeList.size() != mdtR->m_TemplateMetaTypeList.size()) {
            return false;
        }
        for (size_t i = 0; i < mdtL->m_TemplateMetaTypeList.size(); i++) {
            auto lv = mdtL->m_TemplateMetaTypeList[i];
            auto rv = mdtR->m_TemplateMetaTypeList[i];
            if (EqualMetaDefineType(lv, rv) == false) {
                return false;
            }
        }
        return true;
    }

    return false;
}

void MetaType::SetMetaClass(MetaClass* mc) {
    m_MetaClass = mc;
    m_IsDefineMetaClass = true;
    m_EType = EMetaTypeType::MetaClass;
}

void MetaType::SetMetaTemplate(MetaTemplate* mt) {
    m_MetaTemplate = mt;
    if (mt != nullptr) {
        m_EType = EMetaTypeType::Template;
    }
}

void MetaType::SetTypeInferenceClass(MetaClass* mc) {
    m_TypeInferenceClass = mc;
}

void MetaType::SetTemplateMetaClass(MetaClass* mc) {
    m_MetaClass = mc;
    m_EType = EMetaTypeType::TemplateClassWithTemplate;
}

void MetaType::UpdateMetaClassByRawMetaClassAndInputTemplateCollection() {
    // Implementation for UpdateMetaClassByRawMetaClassAndInputTemplateCollection
}

MetaClass* MetaType::UpdateMetaGenTemplate(const std::vector<MetaGenTemplate*>& metaGenTemplateList) {
    if (m_EType == EMetaTypeType::Template) {
        if (m_MetaTemplate != nullptr) {
            for (size_t i = 0; i < metaGenTemplateList.size(); i++) {
                auto cmgt = metaGenTemplateList[i];
                if (cmgt->GetMetaTemplate() == m_MetaTemplate) {
                    return cmgt->GetMetaType()->GetMetaClass();
                }
            }
        }
    } else if (m_EType == EMetaTypeType::TemplateClassWithTemplate) {
        std::vector<MetaClass*> mcList;
        for (size_t i = 0; i < m_TemplateMetaTypeList.size(); i++) {
            auto mgt = m_TemplateMetaTypeList[i];
            if (mgt->GetEType() == EMetaTypeType::MetaClass) {
                mcList.push_back(mgt->GetMetaClass());
            } else {
                auto mc = mgt->UpdateMetaGenTemplate(metaGenTemplateList);
                if (mc == nullptr) {
                    Log::Write("注册生成类是空!");
                    return nullptr;
                }
                mcList.push_back(mc);
            }
        }
        return m_MetaClass->AddInstanceMetaClass(mcList);
    }
    return nullptr;
}

MetaType* MetaType::GetMetaInputTemplateByIndex(int index) const {
    if (index < 0 || index >= static_cast<int>(m_TemplateMetaTypeList.size())) return nullptr;
    return m_TemplateMetaTypeList[index];
}

MetaExpressNode* MetaType::GetDefaultExpressNode() const {
    if (m_DefaultExpressNode != nullptr) {
        return m_DefaultExpressNode;
    } else {
        return m_MetaClass != nullptr ? m_MetaClass->GetDefaultExpressNode() : nullptr;
    }
}

std::string MetaType::ToFormatString() const {
    std::ostringstream sb;
    
    if (m_MetaClass != nullptr) {
        sb << m_MetaClass->GetAllClassName();
    }
    
    return sb.str();
}

std::string MetaType::ToString() const {
    std::ostringstream sb;
    
    if (m_MetaTemplate != nullptr) {
        sb << m_MetaTemplate->GetName();
    } else {
        if (auto mgtc = dynamic_cast<MetaGenTemplateClass*>(m_MetaClass)) {
            sb << mgtc->GetAllClassName();
        } else {
            if (m_MetaClass != nullptr) {
                sb << m_MetaClass->GetAllClassName();
            }
            
            if (m_TemplateMetaTypeList.size() > 0) {
                sb << "<";
                
                for (size_t i = 0; i < m_TemplateMetaTypeList.size(); i++) {
                    sb << m_TemplateMetaTypeList[i]->ToString();
                    if (i < m_TemplateMetaTypeList.size() - 1) {
                        sb << ",";
                    }
                }
                sb << ">";
            }
        }
    }
    
    return sb.str();
}

} // namespace Core
} // namespace SimpleLanguage