//****************************************************************************
//  File:      MetaGenTemplateFunction.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/30 12:00:00
//  Description: Meta enum's attribute
//****************************************************************************

#include "MetaGenTemplateFunction.h"
#include "MetaMemberFunction.h"
#include "MetaGenTemplate.h"
#include "MetaClass.h"
#include "MetaBlockStatements.h"
#include "TypeManager.h"
#include "../Debug/Log.h"
#include <sstream>
#include <algorithm>

using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Core {

MetaGenTemplateFunction::MetaGenTemplateFunction(MetaMemberFunction* mmc, const std::vector<MetaGenTemplate*>& list) 
    : MetaMemberFunction(mmc->GetOwnerMetaClass()) {
    m_SourceTemplateFunctionMetaMemberFunction = mmc;
    UpdateGenMemberFunctionByTemplateClass(mmc);
    m_MetaGenTemplateList = list;
}

MetaGenTemplateFunction::MetaGenTemplateFunction(const MetaGenTemplateFunction& mgtf) 
    : MetaMemberFunction(mgtf) {
    m_SourceMetaMemberFunction = mgtf.m_SourceMetaMemberFunction;
    m_SourceTemplateFunctionMetaMemberFunction = mgtf.m_SourceTemplateFunctionMetaMemberFunction;
    m_MetaGenTemplateList = mgtf.m_MetaGenTemplateList;
}

MetaGenTemplateFunction::MetaGenTemplateFunction(MetaClass* mc, const std::string& _name) 
    : MetaMemberFunction(mc) {
    m_Name = _name;
    m_IsCanRewrite = true;
    m_MetaMemberParamCollection->Clear();

    m_MetaBlockStatements = new MetaBlockStatements(this, nullptr);
    m_MetaBlockStatements->SetIsOnFunction(true);

    Init();
}

bool MetaGenTemplateFunction::MatchInputTemplateInsance(const std::vector<MetaClass*>& instMcList) {
    if (m_MetaGenTemplateList.size() != instMcList.size()) {
        return false;
    }

    for (size_t i = 0; i < m_MetaGenTemplateList.size(); i++) {
        auto c1 = m_MetaGenTemplateList[i];
        auto c2 = instMcList[i];

        if (c1->GetMetaType()->GetMetaClass() != c2) {
            return false;
        }
    }
    return true;
}

void MetaGenTemplateFunction::UpdateGenMemberFunctionByTemplateClass(MetaMemberFunction* mmf) {
    m_MetaMemberParamCollection = mmf->GetMetaMemberParamCollection();
    for (size_t i = 0; i < m_MetaMemberParamCollection->GetMetaDefineParamList().size(); i++) {
        // 空循环，与C#版本保持一致
    }
    m_FileMetaMemberFunction = mmf->GetFileMetaMemberFunction();
    m_Name = mmf->GetName();

    m_IsStatic = mmf->IsStatic();
    m_IsGet = mmf->IsGet();
    m_IsSet = mmf->IsSet();
    m_IsFinal = mmf->IsFinal();
    m_MetaBlockStatements = mmf->GetMetaBlockStatements();
    m_ConstructInitFunction = mmf->IsConstructInitFunction();
    m_SourceMetaMemberFunction = mmf->GetSourceMetaMemberFunction();
    m_ReturnMetaVariable = new MetaVariable(*mmf->GetReturnMetaVariable());
}

MetaGenTemplate* MetaGenTemplateFunction::GetMetaGenTemplate(const std::string& name) {
    auto it = std::find_if(m_MetaGenTemplateList.begin(), m_MetaGenTemplateList.end(),
        [&name](const MetaGenTemplate* mgt) { return mgt->GetName() == name; });
    return (it != m_MetaGenTemplateList.end()) ? *it : nullptr;
}

void MetaGenTemplateFunction::ParseMetaMemberFunctionDefineMetaType() {
    if (m_ReturnMetaVariable != nullptr && m_ReturnMetaVariable->GetMetaDefineType() != nullptr) {
        if (!(m_ReturnMetaVariable->GetMetaDefineType()->GetEType() == EMetaTypeType::MetaClass
            && m_ReturnMetaVariable->GetMetaDefineType()->GetMetaClass()->IsTemplateClass() == false)) {
            TypeManager::GetInstance().UpdateMetaTypeByGenClassAndFunction(m_ReturnMetaVariable->GetMetaDefineType(), 
                dynamic_cast<MetaGenTemplateClass*>(m_OwnerMetaClass), this);
        }
    }
    for (size_t i = 0; i < m_MetaMemberParamCollection->GetMetaDefineParamList().size(); i++) {
        auto mdp = m_MetaMemberParamCollection->GetMetaDefineParamList()[i];
        if (!(mdp->GetMetaVariable()->GetMetaDefineType()->GetEType() == EMetaTypeType::MetaClass
            && mdp->GetMetaVariable()->GetMetaDefineType()->GetMetaClass()->IsTemplateClass() == false)) {
            TypeManager::GetInstance().UpdateMetaTypeByGenClassAndFunction(mdp->GetMetaVariable()->GetMetaDefineType(), 
                dynamic_cast<MetaGenTemplateClass*>(m_OwnerMetaClass), this);
        }
    }
}

void MetaGenTemplateFunction::UpdateRegsterGenMetaFunction() {
    // 这个过程是 绑定 原来注册过来的T的已有的类

    std::vector<MetaGenTemplate*> mgtList = m_MetaGenTemplateList;
    auto curfun = this->m_SourceMetaMemberFunction;
    while (true) {
        if (curfun->GetSourceMetaMemberFunction() == nullptr)
            break;
        curfun = curfun->GetSourceMetaMemberFunction();
    }

    for (size_t i = 0; i < curfun->GetBindStructTemplateFunctionMtList().size(); i++) {
        curfun->GetBindStructTemplateFunctionMtList()[i]->UpdateMetaGenTemplate(mgtList);
    }
}

void MetaGenTemplateFunction::UpdateRegsterGenMetaFunctionAndClass(const std::vector<MetaGenTemplate*>& classGtList) {
    // 这个过程是 绑定 原来注册过来的T的已有的类

    std::vector<MetaGenTemplate*> mgtList = m_MetaGenTemplateList;
    auto curfun = this->m_SourceMetaMemberFunction;
    while (true) {
        if (curfun->GetSourceMetaMemberFunction() == nullptr)
            break;
        curfun = curfun->GetSourceMetaMemberFunction();
    }
    if (curfun->GetBindStructTemplateFunctionAndClassMtList().size() == 0) {
        return;
    }

    mgtList.insert(mgtList.end(), classGtList.begin(), classGtList.end());

    for (size_t i = 0; i < curfun->GetBindStructTemplateFunctionAndClassMtList().size(); i++) {
        curfun->GetBindStructTemplateFunctionAndClassMtList()[i]->UpdateMetaGenTemplate(mgtList);
    }
}

bool MetaGenTemplateFunction::Parse() {
    UpdateRegsterGenMetaFunction();

    if (this->m_OwnerMetaClass->IsTemplateClass()) {
        for (size_t i = 0; i < this->m_OwnerMetaClass->GetMetaGenTemplateClassList().size(); i++) {
            auto mgtc = this->m_OwnerMetaClass->GetMetaGenTemplateClassList()[i];
            mgtc->UpdateRegisterTemplateFunction();
        }
    }
    ParseMetaMemberFunctionDefineMetaType();
    UpdateFunctionName();

    return true;
}

std::string MetaGenTemplateFunction::ToString() const {
    std::stringstream sb;
    if (m_ReturnMetaVariable != nullptr && m_ReturnMetaVariable->GetMetaDefineType() != nullptr) {
        sb << m_ReturnMetaVariable->GetMetaDefineType()->ToFormatString();
    }
    sb << " ";
    sb << GetName();
    sb << "<";
    for (size_t i = 0; i < m_MetaGenTemplateList.size(); i++) {
        auto mgt = m_MetaGenTemplateList[i];
        sb << mgt->ToString();
    }
    sb << ">";
    sb << "(");

    for (size_t i = 0; i < m_MetaMemberParamCollection->GetMetaDefineParamList().size(); i++) {
        auto mpl = m_MetaMemberParamCollection->GetMetaDefineParamList()[i];
        sb << mpl->ToString();
        if (i < m_MetaMemberParamCollection->GetMetaDefineParamList().size() - 1) {
            sb << ",";
        }
    }
    sb << ")";

    return sb.str();
}

} // namespace Core
} // namespace SimpleLanguage
