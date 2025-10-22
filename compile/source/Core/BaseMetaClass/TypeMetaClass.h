//****************************************************************************
//  File:      TypeMetaClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/3/3 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaClass.h"
#include "CoreMetaClassManager.h"
#include "ClassManager.h"
#include "ModuleManager.h"

namespace SimpleLanguage {
namespace Core {
namespace SelfMeta {

class TypeMetaClass : public MetaClass {
public:
    TypeMetaClass() : MetaClass(DefaultObject::Type.ToString()) {
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_Type = EType::Class;
        m_ClassDefineType = EClassDefineType::InnerDefine;
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new TypeMetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

} // namespace SelfMeta
} // namespace Core
} // namespace SimpleLanguage
