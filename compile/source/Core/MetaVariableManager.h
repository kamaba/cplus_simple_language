//****************************************************************************
//  File:      MetaVariableManager.h
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/1/17 12:00:00
//  Description: MetaVariableManager header file
//****************************************************************************

#pragma once

#include <vector>
#include <algorithm>
#include "MetaMemberVariable.h"
#include "MetaMemberData.h"
#include "MetaMemberEnum.h"

namespace SimpleLanguage {
namespace Core {

class MetaVariableManager {
public:
    static MetaVariableManager* s_Instance;
    
    static MetaVariableManager Instance() {
        if (s_Instance == nullptr) {
            s_Instance = new MetaVariableManager();
        }
        return *s_Instance;
    }
    
    std::vector<MetaMemberVariable*> metaMemeberVariableList;
    std::vector<MetaMemberData*> metaMemberDataVariableList;
    std::vector<MetaMemberEnum*> metaMemberEnumVariableList;

    void AddMetaMemberVariable(MetaMemberVariable* mv);
    void AddMetaDataVariable(MetaMemberData* mv);
    void AddMetaEnumVariable(MetaMemberEnum* mv);
    void ParseMetaClassMemberExpress();
    void ParseMetaDataMemberExpress();
    void ParseMetaEnumMemberExpress();
};

} // namespace Core
} // namespace SimpleLanguage
