//****************************************************************************
//  File:      MetaGenTemplateClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Generator Template Class's entity by Template Class
//****************************************************************************

#include "MetaGenTemplateClass.h"
#include "MetaClass.h"
#include "MetaGenTemplate.h"
#include "MetaType.h"
#include "MetaMemberVariable.h"
#include "MetaMemberFunction.h"
#include "MetaInputTemplateCollection.h"
#include "TypeManager.h"
#include "../Debug/Log.h"
#include <sstream>
#include <algorithm>

using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Core {

MetaGenTemplateClass::MetaGenTemplateClass(MetaClass* mtc, const std::vector<MetaGenTemplate*>& list) : MetaClass(mtc->GetName()) {
    m_MetaTemplateClass = mtc;
    m_MetaGenTemplateList = list;
    m_MetaNode = mtc->GetMetaNode();
    m_ExtendClassMetaType = mtc->GetExtendClassMetaType();

    std::stringstream sb;
    sb << m_MetaTemplateClass->GetPathName();
    sb << "<";
    for (size_t i = 0; i < m_MetaGenTemplateList.size(); i++) {
        auto v = m_MetaGenTemplateList[i];
        sb << v->ToDefineTypeString();
        if (i < m_MetaGenTemplateList.size() - 1) {
            sb << ",";
        }
    }
    sb << ">";
    this->m_AllName = sb.str();
}

void MetaGenTemplateClass::UpdateRegsterGenMetaClass() {
    // 这个过程是 绑定 原来注册过来的T的已有的类
    for (size_t i = 0; i < this->m_MetaTemplateClass->GetBindStructTemplateMetaClassList().size(); i++) {
        m_MetaTemplateClass->GetBindStructTemplateMetaClassList()[i]->UpdateMetaGenTemplate(m_MetaGenTemplateList);
    }
}

void MetaGenTemplateClass::SetDeep(int deep) {
    m_Deep = deep;
    for (const auto& v : m_MetaMemberVariableDict) {
        v.second->SetDeep(m_Deep + 1);
    }
    for (const auto& v : m_MetaMemberFunctionTemplateNodeDict) {
        v.second->SetDeep(m_Deep + 1);
    }
}

MetaType* MetaGenTemplateClass::GetGenTemplateByIndex(int index) {
    if (index < static_cast<int>(m_MetaGenTemplateList.size()) && index >= 0) {
        return m_MetaGenTemplateList[index]->GetMetaType();
    }
    return nullptr;
}

bool MetaGenTemplateClass::IsMatchByMetaTemplateClass(const std::vector<MetaGenTemplate*>& mgtList) {
    if (mgtList.empty()) return false;
    if (mgtList.size() != m_MetaGenTemplateList.size()) return false;
    bool flag = true;
    for (size_t i = 0; i < mgtList.size(); i++) {
        auto c1 = mgtList[i];
        auto c2 = m_MetaGenTemplateList[i];
        if (c1->GetMetaType()->GetMetaClass() != c2->GetMetaType()->GetMetaClass()) {
            flag = false;
            break;
        }
    }
    return flag;
}

void MetaGenTemplateClass::GetMetaTemplateMT(std::unordered_map<std::string, MetaType*>& mtdict) {
    for (auto v : m_MetaGenTemplateList) {
        auto cmg = v;
        if (mtdict.find(cmg->GetName()) != mtdict.end()) {
            continue;
        }
        mtdict[cmg->GetName()] = cmg->GetMetaType();
    }
}

MetaMemberVariable* MetaGenTemplateClass::GetMetaMemberVariableByName(const std::string& name) {
    if (m_MetaMemberVariableDict.find(name) != m_MetaMemberVariableDict.end()) {
        return m_MetaMemberVariableDict[name];
    }
    if (m_MetaExtendMemeberVariableDict.find(name) != m_MetaExtendMemeberVariableDict.end()) {
        return m_MetaExtendMemeberVariableDict[name];
    }
    return nullptr;
}

void MetaGenTemplateClass::AddMetaGenTemplate(MetaGenTemplate* mgt) {
    m_MetaGenTemplateList.push_back(mgt);
}

MetaGenTemplate* MetaGenTemplateClass::GetMetaGenTemplate(const std::string& name) {
    auto it = std::find_if(m_MetaGenTemplateList.begin(), m_MetaGenTemplateList.end(),
        [&name](const MetaGenTemplate* mgt) { return mgt->GetName() == name; });
    return (it != m_MetaGenTemplateList.end()) ? *it : nullptr;
}

void MetaGenTemplateClass::Parse() {
    m_MetaMemberVariableDict.clear();
    m_MetaMemberFunctionTemplateNodeDict.clear();
    m_MetaExtendMemeberVariableDict.clear();

    ParseMemberVariableDefineMetaType();
    ParseMemberFunctionDefineMetaType();

    m_ExtendClassMetaType = this->m_MetaTemplateClass->GetExtendClassMetaType();

    TypeManager::GetInstance().UpdateMetaTypeByGenClassAndFunction(m_ExtendClassMetaType, this, nullptr);
    m_ExtendClass = m_ExtendClassMetaType->GetMetaClass();

    HandleExtendMemberVariable();
    HandleExtendMemberFunction();
}

void MetaGenTemplateClass::HandleExtendMemberVariable() {
    MetaClass::HandleExtendMemberVariable();
}

void MetaGenTemplateClass::HandleExtendMemberFunction() {
    this->m_NonStaticVirtualMetaMemberFunctionList = m_ExtendClass->GetNonStaticVirtualMetaMemberFunctionList();
    this->m_StaticMetaMemberFunctionList = m_ExtendClass->GetStaticMetaMemberFunctionList();
}

void MetaGenTemplateClass::ParseMemberVariableDefineMetaType() {
    for (const auto& it : this->m_MetaTemplateClass->GetMetaExtendMemeberVariableDict()) {
        auto mmv = ParseMetaMemberVariableDefineMetaType(it.second);
        m_MetaExtendMemeberVariableDict[mmv->GetName()] = mmv;
    }
    for (const auto& it : this->m_MetaTemplateClass->GetMetaMemberVariableDict()) {
        auto mmv = ParseMetaMemberVariableDefineMetaType(it.second);
        m_MetaMemberVariableDict[mmv->GetName()] = mmv;
    }
}

MetaMemberVariable* MetaGenTemplateClass::ParseMetaMemberVariableDefineMetaType(MetaMemberVariable* mmv) {
    MetaMemberVariable* mgmv = new MetaMemberVariable(*mmv);
    mgmv->SetOwnerMetaClass(this);
    TypeManager::GetInstance().UpdateMetaTypeByGenClassAndFunction(mgmv->GetRealMetaType(), this, nullptr);
    return mgmv;
}

void MetaGenTemplateClass::ParseMemberFunctionDefineMetaType() {
    for (const auto& it : this->m_MetaTemplateClass->GetNonStaticVirtualMetaMemberFunctionList()) {
        ParseMetaMemberFunctionDefineMetaType(it);
    }
    for (const auto& it : this->m_MetaTemplateClass->GetStaticMetaMemberFunctionList()) {
        ParseMetaMemberFunctionDefineMetaType(it);
    }
}

void MetaGenTemplateClass::ParseMetaMemberFunctionDefineMetaType(MetaMemberFunction* mmf) {
    MetaMemberFunction* mgmf = new MetaMemberFunction(*mmf);
    mgmf->SetSourceMetaMemberFunction(mmf);

    if (mmf->IsTemplateFunction() == false) {
        if (mgmf->GetReturnMetaVariable() != nullptr && mgmf->GetReturnMetaVariable()->GetMetaDefineType() != nullptr) {
            if (!(mgmf->GetReturnMetaVariable()->GetMetaDefineType()->GetEType() == EMetaTypeType::MetaClass
                && mgmf->GetReturnMetaVariable()->GetMetaDefineType()->GetMetaClass()->IsTemplateClass() == false)) {
                TypeManager::GetInstance().UpdateMetaTypeByGenClassAndFunction(mgmf->GetReturnMetaVariable()->GetRealMetaType(), this, nullptr);
            }
        }
        for (size_t i = 0; i < mgmf->GetMetaMemberParamCollection()->GetMetaDefineParamList().size(); i++) {
            auto mdp = mgmf->GetMetaMemberParamCollection()->GetMetaDefineParamList()[i];
            if (!(mgmf->GetReturnMetaVariable()->GetMetaDefineType()->GetEType() == EMetaTypeType::MetaClass
                && mgmf->GetReturnMetaVariable()->GetMetaDefineType()->GetMetaClass()->IsTemplateClass() == false)) {
                TypeManager::GetInstance().UpdateMetaTypeByGenClassAndFunction(mdp->GetMetaVariable()->GetRealMetaType(), this, nullptr);
            }
        }
    } else {
        for (size_t i = 0; i < mmf->GetGenTempalteFunctionList().size(); i++) {
            auto v = mmf->GetGenTempalteFunctionList()[i];
            v->UpdateRegsterGenMetaFunctionAndClass(m_MetaGenTemplateList);
        }
    }
    mgmf->UpdateFunctionName();
    AddMetaMemberFunction(mgmf);
}

void MetaGenTemplateClass::UpdateRegisterTemplateFunction() {
    for (const auto& it : this->m_MetaTemplateClass->GetNonStaticVirtualMetaMemberFunctionList()) {
        if (it->IsTemplateFunction()) {
            UpdateRegisterTemplateFunctionSingle(it);
        }
    }
    for (const auto& it : this->m_MetaTemplateClass->GetStaticMetaMemberFunctionList()) {
        if (it->IsTemplateFunction()) {
            UpdateRegisterTemplateFunctionSingle(it);
        }
    }
}

void MetaGenTemplateClass::UpdateRegisterTemplateFunctionSingle(MetaMemberFunction* mmf) {
    for (size_t i = 0; i < mmf->GetGenTempalteFunctionList().size(); i++) {
        auto v = mmf->GetGenTempalteFunctionList()[i];
        v->UpdateRegsterGenMetaFunctionAndClass(m_MetaGenTemplateList);
    }
}

std::vector<MetaMemberVariable*> MetaGenTemplateClass::GetMetaMemberVariableListByFlag(bool isStatic) {
    std::vector<MetaMemberVariable*> mmvList;
    MetaMemberVariable* tempMmv = nullptr;
    for (const auto& v : m_MetaMemberVariableDict) {
        tempMmv = v.second;
        if (isStatic) {
            if (tempMmv->IsStatic() == isStatic || tempMmv->IsConst() == isStatic) {
                mmvList.push_back(tempMmv);
            }
        } else {
            if (tempMmv->IsStatic() == false && tempMmv->IsConst() == false) {
                mmvList.push_back(tempMmv);
            }
        }
    }
    for (const auto& v : m_MetaExtendMemeberVariableDict) {
        tempMmv = v.second;
        if (isStatic) {
            if (tempMmv->IsStatic() == isStatic || tempMmv->IsConst() == isStatic) {
                mmvList.push_back(tempMmv);
            }
        } else {
            if (tempMmv->IsStatic() == false && tempMmv->IsConst() == false) {
                mmvList.push_back(tempMmv);
            }
        }
    }
    return mmvList;
}

bool MetaGenTemplateClass::Adapter(MetaInputTemplateCollection* mitc) {
    if (mitc->GetMetaTemplateParamsList().size() == m_MetaGenTemplateList.size()) {
        size_t i = 0;
        for (auto v : m_MetaGenTemplateList) {
            auto mtpl = mitc->GetMetaTemplateParamsList()[i++];
            auto mgtl = v;
            if (v->GetMetaType()->GetMetaClass() != mtpl->GetMetaClass()) {
                return false;
            }
        }
        return true;
    }

    return false;
}

void MetaGenTemplateClass::UpdateTemplateInstanceStatement(MetaMemberFunction* mmf) {
    for (size_t i = 0; i < mmf->GetMetaMemberParamCollection()->GetMetaDefineParamList().size(); i++) {
        auto mdp = mmf->GetMetaMemberParamCollection()->GetMetaDefineParamList()[i];
        // TypeManager::GetInstance().UpdateMetaTypeByGenClassAndFunction(mdp->GetMetaVariable()->GetMetaDefineType(), this, mmf as MetaGenTempalteFunction);
    }

    auto list = mmf->GetCalcMetaVariableList();

    for (size_t i = 0; i < list.size(); i++) {
        // TypeManager::GetInstance().UpdateMetaTypeByGenClassAndFunction(list[i]->GetMetaDefineType(), this, mmf as MetaGenTempalteFunction);
    }
}

std::string MetaGenTemplateClass::ToString() const {
    return this->ToDefineTypeString();
}

std::string MetaGenTemplateClass::ToDefineTypeString() const {
    std::stringstream sb;

    sb << " [Gen] ";
    sb << GetName();
    if (m_MetaGenTemplateList.size() > 0) {
        sb << "<";
        for (size_t i = 0; i < m_MetaGenTemplateList.size(); i++) {
            auto v = m_MetaGenTemplateList[i];
            sb << v->ToDefineTypeString();
            if (i < m_MetaGenTemplateList.size() - 1) {
                sb << ",";
            }
        }
        sb << ">";
    }

    return sb.str();
}

std::string MetaGenTemplateClass::ToFormatString() const {
    std::stringstream stringBuilder;

    stringBuilder.str(""); // Clear the stringstream

    for (const auto& v : m_MetaMemberVariableDict) {
        stringBuilder << v.second->ToFormatString();
        stringBuilder << std::endl;
    }

    for (const auto& v : m_MetaMemberFunctionTemplateNodeDict) {
        // 这里可以添加函数模板节点的格式化逻辑
    }

    stringBuilder << std::endl;
    stringBuilder << "}" << std::endl;

    return stringBuilder.str();
}

} // namespace Core
} // namespace SimpleLanguage
