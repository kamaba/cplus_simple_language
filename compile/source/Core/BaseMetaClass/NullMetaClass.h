//****************************************************************************
//  File:      NullMetaClass.h
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

class NullMetaClass : public MetaClass {
public:
    NullMetaClass() : MetaClass(DefaultObject::Null.ToString()) {
        m_ClassDefineType = EClassDefineType::InnerDefine;
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new NullMetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

} // namespace Core
} // namespace SimpleLanguage
