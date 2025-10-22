//****************************************************************************
//  File:      EnumMetaClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaClass.h"
#include "../MetaMemberFunction.h"
#include "CoreMetaClassManager.h"
#include "ClassManager.h"
#include "ModuleManager.h"

namespace SimpleLanguage {
namespace Core {

class EnumMetaClass : public MetaClass {
public:
    EnumMetaClass() : MetaClass(DefaultObject::Enum.ToString()) {
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_Type = EType::Enum;
        m_ClassDefineType = EClassDefineType::InnerDefine;
    }
    
    virtual void ParseInnerFunction() override {
        AddCoreFunction();
    }
    
    void AddCoreFunction() {
        MetaMemberFunction* values = new MetaMemberFunction(this, "_values_");
        values->SetIsGet(true);
        values->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetArrayMetaClass());
        AddMetaMemberFunction(values);
    }
    
    static MetaClass* CreateMetaClass() {
        EnumMetaClass* mc = new EnumMetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

} // namespace Core
} // namespace SimpleLanguage
