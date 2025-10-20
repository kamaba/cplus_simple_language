//****************************************************************************
//  File:      IntMetaClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaClass.h"
#include "CoreMetaClassManager.h"
#include "ClassManager.h"
#include "ModuleManager.h"
#include <string>

namespace SimpleLanguage {
namespace Core {
namespace SelfMeta {

class Int16MetaClass : public MetaClass {
public:
    Int16MetaClass() : MetaClass(DefaultObject::Int16.ToString()) {
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_Type = EType::Int16;
        m_ClassDefineType = EClassDefineType::InnerDefine;
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new Int16MetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

class UInt16MetaClass : public MetaClass {
public:
    UInt16MetaClass() : MetaClass(DefaultObject::UInt16.ToString()) {
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_Type = EType::UInt16;
        m_ClassDefineType = EClassDefineType::InnerDefine;
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new UInt16MetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

class Int32MetaClass : public MetaClass {
public:
    Int32MetaClass() : MetaClass(DefaultObject::Int32.ToString()) {
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_Type = EType::Int32;
        m_ClassDefineType = EClassDefineType::InnerDefine;
    }
    
    virtual void ParseInnerFunction() override {
        AddCoreFunction();
    }
    
    void AddCoreFunction() {
        //MetaMemberFunction ToString = new MetaMemberFunction( this, "toString" );              
        //ToString.SetMetaDefineType(new MetaType(CoreMetaClassManager.stringMetaClass));
        //AddMetaMemberFunction(ToString);
        //ToString.AddCSharpMetaStatements("SimpleLanguage.VM.Int32Object", "Int32ToString" );
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new Int32MetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
    
    static std::string MetaToString(int32_t v) {
        return std::to_string(v);
    }
};

class UInt32MetaClass : public MetaClass {
public:
    UInt32MetaClass() : MetaClass(DefaultObject::UInt32.ToString()) {
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_Type = EType::UInt32;
        m_ClassDefineType = EClassDefineType::InnerDefine;
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new UInt32MetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

class Int64MetaClass : public MetaClass {
public:
    Int64MetaClass() : MetaClass(DefaultObject::Int64.ToString()) {
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_Type = EType::Int64;
        m_ClassDefineType = EClassDefineType::InnerDefine;
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new Int64MetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

class UInt64MetaClass : public MetaClass {
public:
    UInt64MetaClass() : MetaClass(DefaultObject::UInt64.ToString()) {
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_Type = EType::UInt64;
        m_ClassDefineType = EClassDefineType::InnerDefine;
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new UInt64MetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

} // namespace SelfMeta
} // namespace Core
} // namespace SimpleLanguage
