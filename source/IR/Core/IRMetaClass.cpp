//****************************************************************************
//  File:      IRMetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/11/15 12:00:00
//  Description: Meta class's ir attribute
//****************************************************************************

#include "IRMetaClass.h"
#include <sstream>

namespace SimpleLanguage {
namespace IR {

int IRMetaClass::s_TypeLength = 1000;

IRMetaClass::IRMetaClass(MetaClass* mc) {
    m_MetaClass = mc;
    id = mc->GetHashCode();
    m_IRName = IRManager::GetIRNameByMetaClass(mc);
}

IRMethod* IRMetaClass::GetIRNonStaticMethodByIndex(int index) {
    if (index >= static_cast<int>(m_IRNotStaticMethodList.size()) || index < 0) {
        Log::AddVM(EError::None, "GetIRMethodByIndex is null");
        return nullptr;
    }
    return m_IRNotStaticMethodList[index];
}

IRMethod* IRMetaClass::GetIRNonStaticMethodIndexByMethod(const std::string& name, int& index) {
    index = -1;
    for (size_t i = 0; i < m_IRNotStaticMethodList.size(); i++) {
        if (m_IRNotStaticMethodList[i]->GetVirtualFunctionName() == name) {
            index = static_cast<int>(i);
            return m_IRNotStaticMethodList[i];
        }
    }
    return nullptr;
}

int IRMetaClass::GetIRNonStaticMethodIndexByMethod(const std::string& name) {
    for (size_t i = 0; i < m_IRNotStaticMethodList.size(); i++) {
        if (m_IRNotStaticMethodList[i]->GetVirtualFunctionName() == name) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int IRMetaClass::GetMetaMemberVariableIndexByHashCode(int id) {
    auto it = m_MetaMemberVariableHashCodeDict.find(id);
    if (it != m_MetaMemberVariableHashCodeDict.end()) {
        return it->second;
    }
    return -1;
}

void IRMetaClass::AddMetaMemberVariableIndexBindHashCode(int id, int newid) {
    if (m_MetaMemberVariableHashCodeDict.find(id) == m_MetaMemberVariableHashCodeDict.end()) {
        m_MetaMemberVariableHashCodeDict[id] = newid;
    }
}

void IRMetaClass::CreateMemberData() {
    m_MetaTypeList.clear();

    if (auto me = dynamic_cast<MetaEnum*>(m_MetaClass)) {
        // Handle enum case
    }
    else if (auto md = dynamic_cast<MetaData*>(m_MetaClass)) {
        auto localMetaMemberDatas = md->GetMetaMemberDataList();
        // Handle data case
    }
    else {
        auto localMetaMemberVariables = m_MetaClass->GetMetaMemberVariableListByFlag(false);
        for (size_t i = 0; i < localMetaMemberVariables.size(); i++) {
            auto v = localMetaMemberVariables[i];
            IRMetaVariable* irmv = new IRMetaVariable(this, v, static_cast<int>(i));
            m_LocalIRMetaVariableList.push_back(irmv);
            AddMetaMemberVariableIndexBindHashCode(irmv->GetId(), static_cast<int>(i));
            if (v->GetIsInnerDefine() == false) {
                // Handle inner define case
            }
        }
    }
    
    int count = 0;
    int ssize = 0;
    for (size_t i = 0; i < m_MetaTypeList.size(); i++) {
        ssize = IRUtil::GetTypeSize(m_MetaTypeList[i]);
        count += ssize;
        m_ByteCount += ssize;
    }

    auto staticMetaMemberVariables = m_MetaClass->GetMetaMemberVariableListByFlag(true);
    for (size_t i = 0; i < staticMetaMemberVariables.size(); i++) {
        auto v = staticMetaMemberVariables[i];
        IRMetaVariable* irmv = new IRMetaVariable(this, v, static_cast<int>(i));
        if (v->GetMetaDefineType()->IsIncludeTemplate()) {
            m_StaticIRMetaVariableList.push_back(irmv);
            AddMetaMemberVariableIndexBindHashCode(v->GetHashCode(), static_cast<int>(i));
        } else {
            IRManager::GetInstance().AddGlobalMetaMemberVariable(irmv);
        }
    }
}

void IRMetaClass::CreateMemberMethod() {
    auto smflist = m_MetaClass->GetStaticMetaMemberFunctionList();
    for (size_t i = 0; i < smflist.size(); i++) {
        auto mf = smflist[i];
        mf->UpdateFunctionName();
        auto gmf = IRManager::GetInstance().TranslateIRByFunction(mf);
        
        IRManager::GetInstance().AddIRMethod(gmf);
    }

    auto nonsmflist = m_MetaClass->GetNonStaticVirtualMetaMemberFunctionList();
    for (size_t i = 0; i < nonsmflist.size(); i++) {
        auto mf = nonsmflist[i];
        mf->UpdateVritualFunctionName();
        auto gmf = IRManager::GetInstance().TranslateIRByFunction(mf);
        m_IRNotStaticMethodList.push_back(gmf);
        IRManager::GetInstance().AddIRMethod(gmf);
    }
}

std::vector<IRData*> IRMetaClass::CreateStaticMetaMetaVariableIRList() {
    std::vector<IRData*> list;

    for (auto v : m_LocalIRMetaVariableList) {
        auto irexp = new IRExpress(&IRManager::GetInstance(), v->GetExpress());

        v->SetIRDataList(irexp->GetIRDataList());

        IRData* irdata = new IRData();
        irdata->id = static_cast<int>(irexp->GetIRDataList().size());
        irdata->opValue = v->GetIrMetaType();
        irdata->opCode = EIROpCode::StoreNotStaticField1;
        irdata->index = v->GetIndex();

        std::vector<IRData*> list22 = irexp->GetIRDataList();
        list22.push_back(irdata);

        list.insert(list.end(), list22.begin(), list22.end());
    }

    return list;
}

std::string IRMetaClass::ToString() {
    std::stringstream sb;
    sb << m_IRName;
    return sb.str();
}

} // namespace IR
} // namespace SimpleLanguage
