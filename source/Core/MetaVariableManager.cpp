//****************************************************************************
//  File:      MetaVariableManager.cpp
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/1/17 12:00:00
//  Description: MetaVariableManager implementation
//****************************************************************************

#include "MetaVariableManager.h"
#include "MetaMemberVariable.h"
#include "MetaMemberData.h"
#include "MetaMemberEnum.h"

namespace SimpleLanguage {
namespace Core {

MetaVariableManager* MetaVariableManager::s_Instance = nullptr;

void MetaVariableManager::AddMetaMemberVariable(MetaMemberVariable* mv) {
    metaMemeberVariableList.push_back(mv);
}

void MetaVariableManager::AddMetaDataVariable(MetaMemberData* mv) {
    metaMemberDataVariableList.push_back(mv);
}

void MetaVariableManager::AddMetaEnumVariable(MetaMemberEnum* mv) {
    metaMemberEnumVariableList.push_back(mv);
}

void MetaVariableManager::ParseMetaClassMemberExpress() {
    for (auto v : metaMemeberVariableList) {
        v->CreateExpress();
    }
    for (auto v : metaMemeberVariableList) {
        v->CalcParseLevel();
    }
    std::sort(metaMemeberVariableList.begin(), metaMemeberVariableList.end(), 
              [](MetaMemberVariable* x, MetaMemberVariable* y) { return x->CompareTo(y); });

    for (auto v : metaMemeberVariableList) {
        v->ParseMetaExpress();
        v->CalcReturnType();
        v->ParseChildMemberData();
    }
}

void MetaVariableManager::ParseMetaDataMemberExpress() {
    for (auto v : metaMemberDataVariableList) {
        v->ParseMetaExpress();
    }
}

void MetaVariableManager::ParseMetaEnumMemberExpress() {
    for (auto v : metaMemberEnumVariableList) {
        v->ParseMetaExpress();
    }
}

} // namespace Core
} // namespace SimpleLanguage
