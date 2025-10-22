//****************************************************************************
//  File:      VoidMetaClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaClass.h"
#include "../MetaConstExpressNode.h"
#include "CoreMetaClassManager.h"
#include "ClassManager.h"
#include "ModuleManager.h"

namespace SimpleLanguage {
namespace Core {

class VoidMetaClass : public MetaClass {
public:
    VoidMetaClass() : MetaClass(DefaultObject::Void.ToString()) {
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_ClassDefineType = EClassDefineType::InnerDefine;
        MetaConstExpressNode* mcen = new MetaConstExpressNode(EType::Null, "null");
        SetDefaultExpressNode(mcen);
        m_Type = EType::Void;
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new VoidMetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

} // namespace Core
} // namespace SimpleLanguage
