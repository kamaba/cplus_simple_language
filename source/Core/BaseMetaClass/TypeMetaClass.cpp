//****************************************************************************
//  File:      TypeMetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/3/3 12:00:00
//  Description: 
//****************************************************************************

#include "TypeMetaClass.h"

namespace SimpleLanguage {
namespace Core {
namespace SelfMeta {

TypeMetaClass::TypeMetaClass() : MetaClass(DefaultObject::Type.ToString()) {
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_Type = EType::Class;
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

MetaClass* TypeMetaClass::CreateMetaClass() {
    MetaClass* mc = new TypeMetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

} // namespace SelfMeta
} // namespace Core
} // namespace SimpleLanguage
