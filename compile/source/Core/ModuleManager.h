//****************************************************************************
//  File:      ModuleManager.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include <map>
#include <string>
#include "../Debug/Log.h"

namespace SimpleLanguage {
namespace Core {

class MetaModule;
class MetaNode;

class ModuleManager {
public:
    static ModuleManager* s_Instance;
    static ModuleManager* GetInstance();

    std::string moduleName = "S";
    static const std::string csharpModuleName;
    static const std::string coreModuleName;

    MetaModule* GetSelfModule() const { return m_SelfModule; }
    MetaModule* GetCoreModule() const { return m_CoreModule; }
    MetaModule* GetCSharpModule() const { return m_CSharpModule; }

private:
    std::map<std::string, MetaModule*> m_ImportMetaModuleDict;

public:
    std::map<std::string, MetaModule*> m_AllMetaModuleDict;

private:
    MetaModule* m_SelfModule;
    MetaModule* m_CoreModule;
    MetaModule* m_CSharpModule;

public:
    ModuleManager();
    ~ModuleManager();
    
    MetaModule* GetMetaModuleOrRetSelfModuleByName(const std::string& name);
    MetaModule* GetMetaModuleByName(const std::string& name);
    MetaNode* GetChildrenMetaNodeByName(const std::string& name);
    void AddMetaMdoule(MetaModule* mm);
    
    std::string ToFormatString();
};

} // namespace Core
} // namespace SimpleLanguage
