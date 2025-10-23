//****************************************************************************
//  File:      BooleanMetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "BooleanMetaClass.h"
#include "CoreMetaClassManager.h"
#include "../ClassManager.h"
#include "../ModuleManager.h"
#include "../MetaExpressNode/MetaExpressConst.h"

namespace SimpleLanguage {
namespace Core {

BooleanMetaClass::BooleanMetaClass() : MetaClass("Boolean") {
    m_Type = EType::Boolean;
    m_ClassDefineType = EClassDefineType::InnerDefine;
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
}

MetaClass* BooleanMetaClass::CreateMetaClass() {
    MetaClass* mc = new BooleanMetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    MetaConstExpressNode* mcen = new MetaConstExpressNode(EType::Boolean, false);
    mc->SetDefaultExpressNode(mcen);
    return mc;
}

} // namespace Core
} // namespace SimpleLanguage
