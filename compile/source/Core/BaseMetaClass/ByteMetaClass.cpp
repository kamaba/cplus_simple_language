//****************************************************************************
//  File:      ByteMetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "ByteMetaClass.h"

namespace SimpleLanguage {
namespace Core {

ByteMetaClass::ByteMetaClass() : MetaClass(DefaultObject::Byte.ToString()) {
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_Type = EType::Byte;
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

MetaClass* ByteMetaClass::CreateMetaClass() {
    MetaClass* mc = new ByteMetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

SByteMetaClass::SByteMetaClass() : MetaClass(DefaultObject::SByte.ToString()) {
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_Type = EType::SByte;
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

MetaClass* SByteMetaClass::CreateMetaClass() {
    MetaClass* mc = new SByteMetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

} // namespace Core
} // namespace SimpleLanguage
