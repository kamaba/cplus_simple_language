//****************************************************************************
//  File:      BooleanMetaClass.h
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
namespace SelfMeta {

class BooleanMetaClass : public MetaClass {
public:
    BooleanMetaClass() : MetaClass(DefaultObject::Boolean.ToString()) {
        m_Type = EType::Boolean;
        m_ClassDefineType = EClassDefineType::InnerDefine;
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new BooleanMetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        MetaConstExpressNode* mcen = new MetaConstExpressNode(EType::Boolean, false);
        mc->SetDefaultExpressNode(mcen);
        return mc;
    }
};

} // namespace SelfMeta
} // namespace Core
} // namespace SimpleLanguage
