//****************************************************************************
//  File:      EnumMetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "EnumMetaClass.h"
#include "../MetaMemberFunction.h"

namespace SimpleLanguage {
namespace Core {
namespace SelfMeta {

EnumMetaClass::EnumMetaClass() : MetaClass(DefaultObject::Enum.ToString()) {
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_Type = EType::Enum;
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

void EnumMetaClass::ParseInnerFunction() {
    AddCoreFunction();
}

void EnumMetaClass::AddCoreFunction() {
    MetaMemberFunction* values = new MetaMemberFunction(this, "_values_");
    values->SetIsGet(true);
    values->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetArrayMetaClass());
    AddMetaMemberFunction(values);
}

MetaClass* EnumMetaClass::CreateMetaClass() {
    EnumMetaClass* mc = new EnumMetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

} // namespace SelfMeta
} // namespace Core
} // namespace SimpleLanguage
