//****************************************************************************
//  File:      StringMetaClass.h
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
#include <typeinfo>

namespace SimpleLanguage {
namespace Core {
namespace SelfMeta {

class StringMetaClass : public MetaClass {
public:
    StringMetaClass() : MetaClass(DefaultObject::String.ToString()) {
        std::type_info& type = typeid(std::string);
        m_ClassDefineType = EClassDefineType::InnerDefine;
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_Type = EType::String;
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new StringMetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

} // namespace SelfMeta
} // namespace Core
} // namespace SimpleLanguage
