//****************************************************************************
//  File:      VoidMetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "VoidMetaClass.h"
#include "CoreMetaClassManager.h"
#include "../ClassManager.h"
#include "../ModuleManager.h"
#include "../MetaExpressNode/MetaExpressConst.h"

namespace SimpleLanguage {
namespace Core {

VoidMetaClass::VoidMetaClass() : MetaClass("Void") {
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_ClassDefineType = EClassDefineType::InnerDefine;
    MetaConstExpressNode* mcen = new MetaConstExpressNode(EType::Null, "null");
    SetDefaultExpressNode(mcen);
    m_Type = EType::Void;
}

MetaClass* VoidMetaClass::CreateMetaClass() {
    MetaClass* mc = new VoidMetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

} // namespace Core
} // namespace SimpleLanguage
