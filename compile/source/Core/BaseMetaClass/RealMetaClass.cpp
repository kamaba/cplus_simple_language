//****************************************************************************
//  File:      RealMetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "RealMetaClass.h"
#include "../MetaTemplate.h"
#include "CoreMetaClassManager.h"
#include "../ClassManager.h"
#include "../ModuleManager.h"
#include "../MetaExpressNode/MetaExpressConst.h"

namespace SimpleLanguage {
namespace Core {

Float32MetaClass::Float32MetaClass() : MetaClass("Float32") {
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_ClassDefineType = EClassDefineType::InnerDefine;
    MetaConstExpressNode* mcen = new MetaConstExpressNode(EType::Float32, 0.0f);
    SetDefaultExpressNode(mcen);
    m_Type = EType::Float32;
}
void Float32MetaClass::ParseInnerFunction() {
    AddCoreFunction();
}

void Float32MetaClass::AddCoreFunction() {
    //MetaMemberFunction Cast = new MetaMemberFunction(this, "Cast");
    //Cast.AddMetaDefineParam(new MetaDefineParam("Template", this, null, CoreMetaClassManager.templateMetaClass, null));
    //AddMetaMemberFunction(Cast);

    //MetaMemberFunction ToInt32 = new MetaMemberFunction(this, "toInt32");
    //ToInt32.isOverrideFunction = true;
    //AddInnerMetaMemberFunction(ToInt32);

    //MetaMemberFunction ToString = new MetaMemberFunction(this, "toString");
    //ToString.SetMetaDefineType(new MetaType(CoreMetaClassManager.stringMetaClass));
    //AddMetaMemberFunction(ToString);
    //ToString.AddCSharpMetaStatements("SimpleLanguage.VM.Int32Object", "FloatToString");
}
MetaClass* Float32MetaClass::CreateMetaClass() {
    MetaClass* mc = new Float32MetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

Float64MetaClass::Float64MetaClass() : MetaClass("Float64") {
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_ClassDefineType = EClassDefineType::InnerDefine;
    MetaConstExpressNode* mcen = new MetaConstExpressNode(EType::Float64, 0.0);
    SetDefaultExpressNode(mcen);
    m_Type = EType::Float64;
}
void Float64MetaClass::ParseInnerFunction() {
    AddCoreFunction();
}
void Float64MetaClass::AddCoreFunction() {
    //MetaMemberFunction toFloat = new MetaMemberFunction(this, "toFloat");
    //toFloat.SetMetaDefineType(new MetaType(CoreMetaClassManager.floatMetaClass));
    //AddMetaMemberFunction(toFloat);
    ////toFloat.AddCSharpMetaStatements("SimpleLanguage.VM.DoubleObject", "DoubleToFloat");

    //MetaMemberFunction ToString = new MetaMemberFunction(this, "toString");
    //ToString.SetMetaDefineType(new MetaType(CoreMetaClassManager.stringMetaClass));
    //AddMetaMemberFunction(ToString);
    //ToString.AddCSharpMetaStatements("SimpleLanguage.VM.DoubleObject", "DoubleToString");
}
MetaClass* Float64MetaClass::CreateMetaClass() {
    MetaClass* mc = new Float64MetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

} // namespace Core
} // namespace SimpleLanguage