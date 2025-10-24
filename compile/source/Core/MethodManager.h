//****************************************************************************
//  File:      MethodManager.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/30 12:00:00
//  Description: Meta enum's attribute
//****************************************************************************

#pragma once

#include "MetaBase.h"
#include "../Define.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace SimpleLanguage {
namespace Compile {
    class FileMetaCallLink;
}

namespace Core {

// Forward declarations
class MetaClass;
class MetaMemberFunction;
class MetaFunction;
class MetaVariable;

class MethodManager {
public:
    // Singleton pattern
    static MethodManager& GetInstance();
    
    // Properties
    const std::vector<MetaMemberFunction*>& GetMetaOriginalFunctionList() const { return m_MetaOriginalFunctionList; }
    const std::vector<MetaMemberFunction*>& GetMetaDynamicFunctionList() const { return m_MetaDynamicFunctionList; }

    // Static methods
    static MetaVariable* GetMetaVariableInMetaClass(MetaClass* mc, Compile::FileMetaCallLink* fmcl);

    // Instance methods
    void AddMetaAllFunction(MetaFunction* mf);
    void AddOriginalMemeberFunction(MetaMemberFunction* mmf);
    void AddDynamicMemeberFunction(MetaMemberFunction* mmf);
    void ParseStatements();

private:
    // Private constructor for singleton
    MethodManager() = default;
    
    // Disable copy constructor and assignment operator
    MethodManager(const MethodManager&) = delete;
    MethodManager& operator=(const MethodManager&) = delete;

    // Member variables
    std::unordered_map<std::string, MetaFunction*> m_MetaAllFunctionDict;
    std::vector<MetaMemberFunction*> m_MetaOriginalFunctionList;
    std::vector<MetaMemberFunction*> m_MetaDynamicFunctionList;
};

} // namespace Core
} // namespace SimpleLanguage
