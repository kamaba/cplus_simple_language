//****************************************************************************
//  File:      StringMetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "StringMetaClass.h"
#include <typeinfo>

namespace SimpleLanguage {
namespace Core {
namespace SelfMeta {

StringMetaClass::StringMetaClass() : MetaClass(DefaultObject::String.ToString()) {
    std::type_info& type = typeid(std::string);
    m_ClassDefineType = EClassDefineType::InnerDefine;
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_Type = EType::String;
}

MetaClass* StringMetaClass::CreateMetaClass() {
    MetaClass* mc = new StringMetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

} // namespace SelfMeta
} // namespace Core
} // namespace SimpleLanguage