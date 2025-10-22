//****************************************************************************
//  File:      NullMetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "NullMetaClass.h"

namespace SimpleLanguage {
namespace Core {

NullMetaClass::NullMetaClass() : MetaClass(DefaultObject::Null.ToString()) {
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

MetaClass* NullMetaClass::CreateMetaClass() {
    MetaClass* mc = new NullMetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

} // namespace Core
} // namespace SimpleLanguage
