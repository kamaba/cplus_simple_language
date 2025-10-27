//****************************************************************************
//  File:      MethodManager.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/30 12:00:00
//  Description: Meta enum's attribute
//****************************************************************************

#include "MethodManager.h"
#include "MetaMemberFunction.h"
#include "MetaFunction.h"
#include "MetaVariable.h"
#include "MetaClass.h"
#include "MetaType.h"
#include "MetaMemberVariable.h"
#include "../Compile/FileMeta/FileMetaBase.h"
#include "../Compile/FileMeta/FileMetaCommon.h"
#include "../Debug/Log.h"
#include <algorithm>

namespace SimpleLanguage {
namespace Core {

// Singleton instance
MethodManager& MethodManager::GetInstance() {
    static MethodManager instance;
    return instance;
}

MetaVariable* MethodManager::GetMetaVariableInMetaClass(MetaClass* mc, Compile::FileMetaCallLink* fmcl) {
    MetaClass* mb = mc;
    MetaVariable* mv = nullptr;
    
    for (size_t i = 0; i < fmcl->GetCallNodeList().size(); i++) {
        auto cnl = fmcl->GetCallNodeList()[i];

        mv = mb->GetMetaMemberVariableByName(cnl->Name());
        if (mv == nullptr) {
            return nullptr;
        }

        mb = mv->GetMetaDefineType()->GetMetaClass();

        if (mb == nullptr)
            return nullptr;
    }
    return mv;
}

void MethodManager::AddMetaAllFunction(MetaFunction* mf) {
    if (m_MetaAllFunctionDict.find(mf->GetFunctionAllName()) != m_MetaAllFunctionDict.end()) {
        return;
    }
    m_MetaAllFunctionDict[mf->GetFunctionAllName()] = mf;
}

void MethodManager::AddOriginalMemeberFunction(MetaMemberFunction* mmf) {
    auto it = std::find(m_MetaOriginalFunctionList.begin(), m_MetaOriginalFunctionList.end(), mmf);
    if (it == m_MetaOriginalFunctionList.end()) {
        m_MetaOriginalFunctionList.push_back(mmf);
        AddMetaAllFunction(mmf);
    }
}

void MethodManager::AddDynamicMemeberFunction(MetaMemberFunction* mmf) {
    auto it = std::find(m_MetaDynamicFunctionList.begin(), m_MetaDynamicFunctionList.end(), mmf);
    if (it == m_MetaDynamicFunctionList.end()) {
        m_MetaDynamicFunctionList.push_back(mmf);
        AddMetaAllFunction(mmf);
    }
}

void MethodManager::ParseStatements() {
    auto list = std::vector<MetaMemberFunction*>(m_MetaOriginalFunctionList);

    // Sort by parse level (equivalent to C# list.Sort((a, b) => a.parseLevel - b.parseLevel))
    std::sort(list.begin(), list.end(), 
        [](MetaMemberFunction* a, MetaMemberFunction* b) {
            return a->GetParseLevel() < b->GetParseLevel();
        });

    for (auto v : list) {
        v->CreateMetaExpress();
        v->ParseMetaExpress();
        v->ParseStatements();
    }
}

} // namespace Core
} // namespace SimpleLanguage
