//****************************************************************************
//  File:      ModuleManager.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "ModuleManager.h"
#include "MetaModule.h"
#include "MetaNode.h"
#include "../Debug/Log.h"
#include <iostream>

using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Core {

// Static member initialization
ModuleManager* ModuleManager::s_Instance = nullptr;
const std::string ModuleManager::csharpModuleName = "CSharp";
const std::string ModuleManager::coreModuleName = "Core";

ModuleManager* ModuleManager::GetInstance() {
    if (s_Instance == nullptr) {
        s_Instance = new ModuleManager();
    }
    return s_Instance;
}

ModuleManager::ModuleManager() {
    m_SelfModule = new MetaModule(moduleName);
    m_CoreModule = new MetaModule(coreModuleName);
    m_CSharpModule = new MetaModule(csharpModuleName);
    m_CSharpModule->SetRefFromType(RefFromType::CSharp);
    m_AllMetaModuleDict[moduleName] = m_SelfModule;
    m_AllMetaModuleDict[coreModuleName] = m_CoreModule;
    m_AllMetaModuleDict[csharpModuleName] = m_CSharpModule;
    m_SelfModule->SetDeep(0);
    m_CoreModule->SetDeep(0);
}

ModuleManager::~ModuleManager() {
    // Clean up modules
    delete m_SelfModule;
    delete m_CoreModule;
    delete m_CSharpModule;
    
    // Clean up imported modules
    for (auto& pair : m_ImportMetaModuleDict) {
        delete pair.second;
    }
    m_ImportMetaModuleDict.clear();
    
    // Clean up all modules (excluding the ones already deleted above)
    for (auto& pair : m_AllMetaModuleDict) {
        if (pair.second != m_SelfModule && 
            pair.second != m_CoreModule && 
            pair.second != m_CSharpModule) {
            delete pair.second;
        }
    }
    m_AllMetaModuleDict.clear();
}

MetaModule* ModuleManager::GetMetaModuleOrRetSelfModuleByName(const std::string& name) {
    MetaModule* mm = GetMetaModuleByName(name);
    if (mm == nullptr) return m_SelfModule;
    else return mm;
}

MetaModule* ModuleManager::GetMetaModuleByName(const std::string& name) {
    if (name.empty()) {
        Log::AddInStructMeta(EError::None, "Error 严重错误，获取模式不传名称!!");
        return nullptr;
    }
    if (m_AllMetaModuleDict.find(name) != m_AllMetaModuleDict.end()) {
        return m_AllMetaModuleDict[name];
    }
    return nullptr;
}

MetaNode* ModuleManager::GetChildrenMetaNodeByName(const std::string& name) {
    if (name == "Core") {
        return m_CoreModule->GetMetaNode();
    }
    MetaNode* m2 = m_SelfModule->GetMetaNode()->GetChildrenMetaNodeByName(name);
    if (m2 != nullptr) {
        return m2;
    }
    for (auto& v : m_ImportMetaModuleDict) {
        m2 = v.second->GetMetaNode()->GetChildrenMetaNodeByName(name);
        if (m2 != nullptr) {
            return m2;
        }
    }
    return nullptr;
}

void ModuleManager::AddMetaMdoule(MetaModule* mm) {
    if (m_ImportMetaModuleDict.find(mm->GetName()) != m_ImportMetaModuleDict.end()) {
        return;
    }
    m_ImportMetaModuleDict[mm->GetName()] = mm;
    if (m_AllMetaModuleDict.find(mm->GetName()) != m_AllMetaModuleDict.end()) {
        Log::AddInStructMeta(EError::None, "Error 严重错误，模块有重名!!!");
        return;
    }
    m_AllMetaModuleDict[mm->GetName()] = mm;
}

std::string ModuleManager::ToFormatString() {
    return m_SelfModule->ToFormatString();
}

} // namespace Core
} // namespace SimpleLanguage
