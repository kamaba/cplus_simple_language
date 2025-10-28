//****************************************************************************
//  File:      MetaTemplate.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta template class
//****************************************************************************

#include "MetaTemplate.h"
#include "MetaClass.h"
#include "MetaType.h""
#include "MetaParam.h"
#include "../Compile/FileMeta/FileMetaCommon.h"
#include "../Debug/Log.h"
#include "ClassManager.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include <sstream>
#include <algorithm>

namespace SimpleLanguage {
namespace Core {

    // MetaTemplate implementation
    MetaTemplate::MetaTemplate(MetaClass* mc, Compile::FileMetaTemplateDefine* fmtd, int index) {
        m_Name = fmtd->GetName();
        m_FileMetaTemplateDefine = fmtd;
        m_OwnerClass = mc;
        this->m_Index = index;
    }

    MetaTemplate::MetaTemplate(MetaClass* mc, const std::string& name) {
        m_Name = name;
        m_OwnerClass = mc;
    }

    void MetaTemplate::SetIndex(int index) {
        this->m_Index = index;
    }

    void MetaTemplate::ParseInConstraint() {
        if (m_FileMetaTemplateDefine != nullptr) {
            if (m_FileMetaTemplateDefine->InClassNameTemplateNode() != nullptr) {
                m_ExtendsMetaClass = ClassManager::GetInstance().GetMetaClassByInputTemplateAndFileMeta(m_OwnerClass, m_FileMetaTemplateDefine->InClassNameTemplateNode());
            } else {
                m_ExtendsMetaClass = CoreMetaClassManager::objectMetaClass;
            }
        }
    }

    void MetaTemplate::AddBindConstructFunction(MetaMethodCall* mmc) {
        if (std::find(m_BindConstructFunction.begin(), m_BindConstructFunction.end(), mmc) == m_BindConstructFunction.end()) {
            m_BindConstructFunction.push_back(mmc);
        }
    }

    void MetaTemplate::SetInConstraintMetaClass(MetaClass* mc) {
        m_ExtendsMetaClass = mc;
    }

    bool MetaTemplate::IsInConstraintMetaClass(MetaClass* mc) {
        return m_ExtendsMetaClass != nullptr;
    }

    std::string MetaTemplate::ToFormatString() const {
        std::stringstream sb;
        sb << m_Name;
        if (m_ExtendsMetaClass != nullptr) {
            sb << " extends ";
            sb << m_ExtendsMetaClass->GetAllClassName();
        }
        return sb.str();
    }

    // MetaGenTemplate implementation
    MetaGenTemplate::MetaGenTemplate(MetaTemplate* mt) {
        m_MetaTemplate = mt;
    }

    MetaGenTemplate::MetaGenTemplate(MetaTemplate* mt, MetaType* mtype) {
        m_MetaTemplate = mt;
        m_MetaType = mtype;
    }

    bool MetaGenTemplate::EqualWithMetaType(MetaType* mt) {
        return m_MetaType->GetMetaClass()->GetAllClassName() == mt->GetMetaClass()->GetAllClassName();
    }

    void MetaGenTemplate::SetMetaType(MetaType* mt) {
        m_MetaType = mt;
    }

    std::string MetaGenTemplate::ToDefineTypeString() {
        std::stringstream sb;
        if (m_MetaType->IsTemplate()) {
            sb << m_MetaType->GetMetaTemplate()->GetName();
        } else {
            sb << m_MetaType->GetMetaClass()->ToString();
        }
        return sb.str();
    }

    std::string MetaGenTemplate::ToFormatString() {
        std::stringstream sb;
        sb << m_MetaType->GetMetaClass()->GetName();
        return sb.str();
    }
    std::string MetaGenTemplate::ToString()
    {
        return ToFormatString();
    }

    // MetaDefineTemplateCollection implementation
    MetaDefineTemplateCollection::MetaDefineTemplateCollection() {
    }

    MetaDefineTemplateCollection::MetaDefineTemplateCollection(const MetaDefineTemplateCollection& mdtc) {
        for (size_t i = 0; i < mdtc.m_MetaTemplateList.size(); i++) {
            m_MetaTemplateList.push_back(mdtc.m_MetaTemplateList[i]);
        }
    }

    MetaTemplate* MetaDefineTemplateCollection::GetMetaDefineTemplateByName(const std::string& _name) {
        for (size_t i = 0; i < m_MetaTemplateList.size(); i++) {
            if (m_MetaTemplateList[i]->GetName() == _name) {
                return m_MetaTemplateList[i];
            }
        }
        return nullptr;
    }

    bool MetaDefineTemplateCollection::IsEqualMetaDefineTemplateCollection(MetaDefineTemplateCollection* mpc) {
        if (mpc == nullptr) {
            return m_MetaTemplateList.size() == 0;
        }
        if (m_MetaTemplateList.size() == mpc->m_MetaTemplateList.size()) {
            if (m_MetaTemplateList.size() == 0) {
                return true;
            }

            for (size_t i = 0; i < m_MetaTemplateList.size(); i++) {
                MetaTemplate* a = m_MetaTemplateList[i];
                MetaTemplate* b = mpc->m_MetaTemplateList[i];
                if (MatchMetaDefineTemplate(a, b)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool MetaDefineTemplateCollection::IsEqualMetaInputTemplateCollection(MetaInputTemplateCollection* mpc) {
        if (mpc == nullptr) {
            return m_MetaTemplateList.size() == 0;
        }

        if (m_MetaTemplateList.size() == mpc->GetMetaTemplateParamsList().size()) {
            for (size_t i = 0; i < m_MetaTemplateList.size(); i++) {
                MetaTemplate* a = m_MetaTemplateList[i];
                MetaType* b = mpc->GetMetaTemplateParamsList()[i];
                if (MatchMetaInputTemplate(a, b)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool MetaDefineTemplateCollection::MatchMetaInputTemplate(MetaTemplate* a, MetaType* b) {
        if (a->IsInConstraintMetaClass(b->GetMetaClass())) {
            return true;
        }
        return false;
    }

    bool MetaDefineTemplateCollection::MatchMetaDefineTemplate(MetaTemplate* a, MetaTemplate* b) {
        if (a->GetName() == b->GetName()) {
            return true;
        }
        return false;
    }

    void MetaDefineTemplateCollection::AddMetaDefineTemplate(MetaTemplate* defineTemplate) {
        m_MetaTemplateList.push_back(defineTemplate);
    }

    std::string MetaDefineTemplateCollection::ToFormatString() {
        return "";
    }

} // namespace Core
} // namespace SimpleLanguage