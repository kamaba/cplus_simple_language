#include "InnerCLRRegisterClass.h"
#include "ClassObject.h"
#include "../IR/IRBase.h"
#include <iostream>

namespace SimpleLanguage {
namespace VM {

// RuntimeClass implementation
RuntimeClass::RuntimeClass(IRMetaClass* irmc) {
    id = 0;
    irMetaClass = irmc;
    m_StaticMetaMemberVariableArray = nullptr;
    
    // irmc.CreateStaticMetaMetaVariableIRList
}

RuntimeClass::~RuntimeClass() {
    if (m_StaticMetaMemberVariableArray != nullptr) {
        delete[] m_StaticMetaMemberVariableArray;
    }
}

ClassObject* RuntimeClass::GetStaticMetaMemberVaraible(int index) {
    if (index < 0 || index >= static_cast<int>(irMetaClass->staticIRMetaVariableList.size())) {
        return nullptr;
    }
    return m_StaticMetaMemberVariableArray[index];
}

// InnerCLRRegisterClass implementation
InnerCLRRegisterClass* InnerCLRRegisterClass::s_Instance = nullptr;

InnerCLRRegisterClass* InnerCLRRegisterClass::GetInstance() {
    if (s_Instance == nullptr) {
        s_Instance = new InnerCLRRegisterClass();
    }
    return s_Instance;
}

InnerCLRRegisterClass::InnerCLRRegisterClass() {
    // Constructor
}

InnerCLRRegisterClass::~InnerCLRRegisterClass() {
    // Clean up m_RuntimeClassDict
    for (auto& pair : m_RuntimeClassDict) {
        delete pair.second;
    }
    m_RuntimeClassDict.clear();
}

void InnerCLRRegisterClass::RegisterDymnicClass() {
    // Implementation for RegisterDymnicClass
}

ClassObject* InnerCLRRegisterClass::GetStaticMetaMemberVariable(int classid, int index) {
    if (m_RuntimeClassDict.find(classid) != m_RuntimeClassDict.end()) {
        return m_RuntimeClassDict[classid]->GetStaticMetaMemberVaraible(index);
    }

    return nullptr;
}

} // namespace VM
} // namespace SimpleLanguage
