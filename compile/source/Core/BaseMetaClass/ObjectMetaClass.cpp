//****************************************************************************
//  File:      ObjectMetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "ObjectMetaClass.h"
#include "../MetaMemberFunction.h"
#include "../MetaMemberVariable.h"

namespace SimpleLanguage {
namespace Core {

ObjectMetaClass::ObjectMetaClass() : MetaClass(DefaultObject::Object.ToString()) {
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

MetaClass* ObjectMetaClass::Cast(MetaTemplate* mc) {
    return nullptr;
}

int ObjectMetaClass::ToInt32() {
    return 0;
}

void ObjectMetaClass::ParseInnerFunction() {
    AddCoreFunction();
}

void ObjectMetaClass::AddCoreFunction() {
    MetaMemberFunction* Cast = new MetaMemberFunction(this, "cast");
    //Cast.AddMetaDefineParam(new MetaDefineParam("t", this, null, CoreMetaClassManager.templateMetaClass, null));
    //Cast.SetDefineMetaClass(CoreMetaClassManager.int32MetaClass);
    //AddMetaMemberFunction(Cast);

    //MetaMemberVariable mmvobjectid = new MetaMemberVariable(this, "objectid", CoreMetaClassManager.int32MetaClass);
    //AddMetaMemberVariable(mmvobjectid);

    //MetaMemberVariable mmvname = new MetaMemberVariable(this, "name", CoreMetaClassManager.stringMetaClass);
    //AddMetaMemberVariable(mmvname);

    //MetaMemberFunction _init_ = new MetaMemberFunction(this, "_init_");
    //AddMetaMemberFunction(_init_);

    //MetaMemberFunction GetHashCode = new MetaMemberFunction(this, "hashCode");
    //GetHashCode.SetReturnMetaClass(CoreMetaClassManager.int32MetaClass);
    //AddInnerMetaMemberFunction(GetHashCode);
    //MetaMemberFunction GetType = new MetaMemberFunction(this, "getType");
    //GetType.SetReturnMetaClass(this);
    //AddInnerMetaMemberFunction(GetType);
    //MetaMemberFunction Clone = new MetaMemberFunction(this, "clone");
    //Clone.SetReturnMetaClass( this );
    //AddInnerMetaMemberFunction(Clone);
    //MetaMemberFunction ToString = new MetaMemberFunction(this, "toString");
    //ToString.SetReturnMetaClass(CoreMetaClassManager.stringMetaClass);
    //AddInnerMetaMemberFunction(ToString);
    //MetaMemberFunction ToShort = new MetaMemberFunction(this, "toShort");
    //ToShort.SetReturnMetaClass(CoreMetaClassManager.int16MetaClass);
    //AddInnerMetaMemberFunction(ToShort);
    //MetaMemberFunction ToInt = new MetaMemberFunction(this, "toInt");
    //ToInt.SetReturnMetaClass(CoreMetaClassManager.int32MetaClass);
    //AddInnerMetaMemberFunction(ToShort);
    //MetaMemberFunction ToLong = new MetaMemberFunction(this, "toLong");
    //ToLong.SetReturnMetaClass(CoreMetaClassManager.int64MetaClass);
    //AddInnerMetaMemberFunction(ToLong);
    //MetaMemberFunction ToFloat = new MetaMemberFunction(this, "toFloat");
    //ToFloat.SetReturnMetaClass(CoreMetaClassManager.float32MetaClass);
    //AddInnerMetaMemberFunction(ToFloat);
    //MetaMemberFunction ToDouble = new MetaMemberFunction(this, "toDouble");
    //ToDouble.SetReturnMetaClass(CoreMetaClassManager.float64MetaClass);
    //AddInnerMetaMemberFunction(ToLong);
}

MetaClass* ObjectMetaClass::CreateMetaClass() {
    MetaClass* mc = new ObjectMetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

} // namespace Core
} // namespace SimpleLanguage
