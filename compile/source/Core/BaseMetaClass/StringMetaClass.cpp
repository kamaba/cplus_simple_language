//****************************************************************************
//  File:      StringMetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "StringMetaClass.h"
#include "CoreMetaClassManager.h"
#include "../ClassManager.h"
#include "../ModuleManager.h"
#include "../MetaExpressNode/MetaExpressConst.h"

namespace SimpleLanguage {
namespace Core {

StringMetaClass::StringMetaClass() : MetaClass("String") {    
    m_ClassDefineType = EClassDefineType::InnerDefine;
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_Type = EType::String;
}
MetaClass* StringMetaClass::CreateMetaClass() {
    MetaClass* mc = new StringMetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

} // namespace Core
} // namespace SimpleLanguage