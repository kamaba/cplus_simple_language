//****************************************************************************
//  File:      ByteMetaClass.h
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

namespace SimpleLanguage {
namespace Core {

class ByteMetaClass : public MetaClass {
public:
    ByteMetaClass() : MetaClass(DefaultObject::Byte.ToString()) {
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_Type = EType::Byte;
        m_ClassDefineType = EClassDefineType::InnerDefine;
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new ByteMetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

class SByteMetaClass : public MetaClass {
public:
    SByteMetaClass() : MetaClass(DefaultObject::SByte.ToString()) {
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_Type = EType::SByte;
        m_ClassDefineType = EClassDefineType::InnerDefine;
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new SByteMetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

} // namespace Core
} // namespace SimpleLanguage
