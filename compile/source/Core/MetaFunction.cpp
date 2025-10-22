//****************************************************************************
//  File:      MetaMethod.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/30 12:00:00
//  Description: Meta method's attribute
//****************************************************************************

#include "MetaFunction.h"
#include "MetaType.h"
#include "MetaVariable.h"
#include "MetaClass.h"
#include "MetaParam.h"
#include "Statements/MetaBlockStatements.h"
#include "Statements/MetaStatements.h"
#include "MetaTemplate.h"
#include "../Compile/Token.h"
#include "../Debug/Log.h"
#include <sstream>
#include <algorithm>

using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Core {

MetaFunction::MetaFunction(MetaClass* mc)
{
    m_MetaMemberParamCollection = new MetaDefineParamCollection(false, true);
    SetOwnerMetaClass(mc);
}

    MetaFunction::MetaFunction(const MetaFunction& mf) : MetaBase(mf) {
    m_IsParsed = mf.m_IsParsed;
    m_FunctionAllName = "";
    m_PintTokenList = mf.m_PintTokenList;
    m_VirtualFunctionName = mf.m_VirtualFunctionName;

    m_OwnerMetaClass = mf.m_OwnerMetaClass;
    m_MetaBlockStatements = mf.m_MetaBlockStatements;
    if (mf.m_ThisMetaVariable != nullptr) {
        m_ThisMetaVariable = new MetaVariable(*mf.m_ThisMetaVariable);
    }
    if (mf.m_ReturnMetaVariable != nullptr) {
        m_ReturnMetaVariable = new MetaVariable(*mf.m_ReturnMetaVariable);
    }
    m_MetaMemberParamCollection = new MetaDefineParamCollection(*mf.m_MetaMemberParamCollection);
    m_MetaMemberTemplateCollection = new MetaDefineTemplateCollection(*mf.m_MetaMemberTemplateCollection);
    m_MethodCallType = mf.m_MethodCallType;
    m_LabelDataList = mf.m_LabelDataList;
    m_IsStatic = mf.m_IsStatic;
}

MetaType* MetaFunction::GetMetaDefineType() const {
    if (m_ReturnMetaVariable != nullptr) {
        return m_ReturnMetaVariable->GetMetaDefineType();
    }
    return nullptr;
}

Compile::Token* MetaFunction::GetPingToken() const {
    return m_PintTokenList.size() > 0 ? m_PintTokenList[0] : nullptr;
}

std::string MetaFunction::GetFunctionAllName() const {
    if (m_FunctionAllName.empty()) {
        std::stringstream sb;
        if (m_OwnerMetaClass != nullptr) {
            sb << m_OwnerMetaClass->GetAllClassName();
            sb << ".";
        }
        sb << GetName();
        if (m_MetaMemberTemplateCollection != nullptr && m_MetaMemberTemplateCollection->GetMetaTemplateList().size() > 0) {
            sb << "<";
            for (size_t i = 0; i < m_MetaMemberTemplateCollection->GetMetaTemplateList().size(); i++) {
                auto mtl = m_MetaMemberTemplateCollection->GetMetaTemplateList()[i];
                sb << mtl->GetName();
                if (i < m_MetaMemberTemplateCollection->GetMetaTemplateList().size() - 1) {
                    sb << ",";
                }
            }
            sb << ">";
        }
        if (m_MetaMemberParamCollection != nullptr && m_MetaMemberParamCollection->GetMaxParamCount() > 0) {
            sb << "_";
            sb << std::to_string(m_MetaMemberParamCollection->GetMaxParamCount());
            sb << "_";
            sb << m_MetaMemberParamCollection->ToParamTypeName();
        }
        m_FunctionAllName = sb.str();
    }
    return m_FunctionAllName;
}

void MetaFunction::SetDeep(int deep) {
    MetaBase::SetDeep(deep);
    if (m_MetaBlockStatements != nullptr) {
        m_MetaBlockStatements->SetDeep(deep);
    }
}

void MetaFunction::SetOwnerMetaClass(MetaClass* ownerclass) {
    if (ownerclass == nullptr) {
        return;
    }
    if (ownerclass == m_OwnerMetaClass) {
        return;
    }
    m_OwnerMetaClass = ownerclass;
    if (m_MetaBlockStatements != nullptr) {
        m_MetaBlockStatements->UpdateOwnerMetaClass(ownerclass);
    }
    if (m_ThisMetaVariable != nullptr) {
        m_ThisMetaVariable->SetOwnerMetaClass(ownerclass);
    }
    if (m_ReturnMetaVariable != nullptr) {
        m_ReturnMetaVariable->SetOwnerMetaClass(ownerclass);
    }
    if (m_MetaMemberParamCollection != nullptr) {
        m_MetaMemberParamCollection->SetOwnerMetaClass(ownerclass);
    }
}

void MetaFunction::AddFrontMetaStatements(MetaStatements* state) {
    if (m_MetaBlockStatements != nullptr) {
        m_MetaBlockStatements->AddFrontStatements(state);
    }
}

std::vector<MetaVariable*> MetaFunction::GetCalcMetaVariableList(bool isIncludeArgument) {
    std::vector<MetaVariable*> metaVarList;
    if (isIncludeArgument && m_MetaMemberParamCollection != nullptr) {
        for (size_t i = 0; i < m_MetaMemberParamCollection->GetMetaDefineParamList().size(); i++) {
            auto mdp = m_MetaMemberParamCollection->GetMetaDefineParamList()[i];
            if (mdp != nullptr) {
                metaVarList.push_back(mdp->GetMetaVariable());
            }
        }
    }
    if (m_MetaBlockStatements != nullptr) {
        m_MetaBlockStatements->GetCalcMetaVariableList(metaVarList);
    }
    return metaVarList;
}

LabelData* MetaFunction::GetLabelDataById(const std::string& label) {
    auto it = std::find_if(m_LabelDataList.begin(), m_LabelDataList.end(),
        [&label](const LabelData* ld) { return ld->label == label; });
    return (it != m_LabelDataList.end()) ? *it : nullptr;
}

LabelData* MetaFunction::AddLabelData(const std::string& label, MetaStatements* nextState) {
    auto ld = new LabelData(label, nextState);
    m_LabelDataList.push_back(ld);
    return ld;
}

void MetaFunction::UpdateLabelData(LabelData* newld) {
    auto ld = GetLabelDataById(newld->label);
    if (ld != nullptr) {
        ld->frontStatements = newld->frontStatements;
        ld->nextStatements = newld->nextStatements;
    }
}

void MetaFunction::UpdateFunctionName() {
    m_FunctionAllName = "";
    m_FunctionAllName = GetFunctionAllName();
}

bool MetaFunction::Parse() {
    return true;
}

void MetaFunction::SetReturnMetaClass(MetaClass* metaClass) {
    if (m_ReturnMetaVariable != nullptr) {
        m_ReturnMetaVariable->GetMetaDefineType()->SetMetaClass(metaClass);
    }
}

MetaDefineParam* MetaFunction::GetMetaDefineParamByName(const std::string& name) {
    if (m_MetaMemberParamCollection != nullptr) {
        return m_MetaMemberParamCollection->GetMetaDefineParamByName(name);
    }
    return nullptr;
}

bool MetaFunction::IsEqualMetaFunction(MetaFunction* mf) {
    if (mf == nullptr) {
        return false;
    }
    if (this->GetName() != mf->GetName()) {
        return false;
    }
    if (m_MetaMemberTemplateCollection != nullptr && mf->GetMetaMemberTemplateCollection() != nullptr) {
        if (!this->m_MetaMemberTemplateCollection->IsEqualMetaDefineTemplateCollection(mf->GetMetaMemberTemplateCollection())) {
            return false;
        }
    }
    if (m_MetaMemberParamCollection != nullptr && mf->GetMetaMemberParamCollection() != nullptr) {
        if (!this->m_MetaMemberParamCollection->IsEqualMetaDefineParamCollection(mf->GetMetaMemberParamCollection())) {
            return false;
        }
    }

    return true;
}

bool MetaFunction::IsEqualMetaInputParamCollection(MetaInputParamCollection* mpc) {
    if (m_MetaMemberParamCollection != nullptr && mpc != nullptr) {
        if (m_MetaMemberParamCollection->IsEqualMetaInputParamCollection(mpc)) {
            return true;
        }
    }
    return false;
}

bool MetaFunction::IsEqualMetaDefineParamCollection(MetaDefineParamCollection* mdpc) {
    if (m_MetaMemberParamCollection != nullptr && mdpc != nullptr) {
        if (m_MetaMemberParamCollection->IsEqualMetaDefineParamCollection(mdpc)) {
            return true;
        }
    }
    return false;
}

MetaTemplate* MetaFunction::GetMetaDefineTemplateByName(const std::string& name) {
    if (m_MetaMemberTemplateCollection != nullptr) {
        return m_MetaMemberTemplateCollection->GetMetaDefineTemplateByName(name);
    }
    return nullptr;
}

bool MetaFunction::IsEqualMetaTemplateCollectionAndMetaParamCollection(MetaInputTemplateCollection* mitc, MetaDefineParamCollection* mpc) {
    // if (m_MetaMemberParamCollection->IsEqualMetaTemplateAndParamCollection(mitc, mpc))
    // {
    //     return true;
    // }
    return false;
}

std::string MetaFunction::ToStatementString() {
    std::stringstream sb;

    sb << GetName();

    if (m_MetaMemberTemplateCollection != nullptr) {
        sb << m_MetaMemberTemplateCollection->ToFormatString();
    }
    // sb << "( ";
    if (m_MetaMemberParamCollection != nullptr) {
        sb << m_MetaMemberParamCollection->ToFormatString();
    }
    // sb << " )";

    return sb.str();
}

std::string MetaFunction::ToString() const {
    return GetFunctionAllName();
}

std::string MetaFunction::ToFormatString() const {
    return MetaBase::ToFormatString();
}

} // namespace Core
} // namespace SimpleLanguage
