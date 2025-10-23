//****************************************************************************
//  File:      IntMetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "IntMetaClass.h"
#include "CoreMetaClassManager.h"
#include "../ClassManager.h"
#include "../ModuleManager.h"
#include "../MetaExpressNode/MetaExpressConst.h"

namespace SimpleLanguage {
namespace Core {

Int16MetaClass::Int16MetaClass() : MetaClass("Int16") {
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_Type = EType::Int16;
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

MetaClass* Int16MetaClass::CreateMetaClass() {
    MetaClass* mc = new Int16MetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

UInt16MetaClass::UInt16MetaClass() : MetaClass("Int16") {
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_Type = EType::UInt16;
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

MetaClass* UInt16MetaClass::CreateMetaClass() {
    MetaClass* mc = new UInt16MetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

Int32MetaClass::Int32MetaClass() : MetaClass("Int32") {
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_Type = EType::Int32;
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

void Int32MetaClass::ParseInnerFunction() {
    AddCoreFunction();
}

void Int32MetaClass::AddCoreFunction() {
    //MetaMemberFunction ToString = new MetaMemberFunction( this, "toString" );              
    //ToString.SetMetaDefineType(new MetaType(CoreMetaClassManager.stringMetaClass));
    //AddMetaMemberFunction(ToString);
    //ToString.AddCSharpMetaStatements("SimpleLanguage.VM.Int32Object", "Int32ToString" );
}

MetaClass* Int32MetaClass::CreateMetaClass() {
    MetaClass* mc = new Int32MetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}


UInt32MetaClass::UInt32MetaClass() : MetaClass("UInt32") {
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_Type = EType::UInt32;
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

MetaClass* UInt32MetaClass::CreateMetaClass() {
    MetaClass* mc = new UInt32MetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

Int64MetaClass::Int64MetaClass() : MetaClass("Int64") {
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_Type = EType::Int64;
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

MetaClass* Int64MetaClass::CreateMetaClass() {
    MetaClass* mc = new Int64MetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

UInt64MetaClass::UInt64MetaClass() : MetaClass("UInt64") {
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_Type = EType::UInt64;
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

MetaClass* UInt64MetaClass::CreateMetaClass() {
    MetaClass* mc = new UInt64MetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

} // namespace Core
} // namespace SimpleLanguage
