//****************************************************************************
//  File:      DynamicMetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "DynamicMetaClass.h"
#include "CoreMetaClassManager.h"
#include "../ClassManager.h"
#include "../ModuleManager.h"
#include "../MetaExpressNode/MetaExpressConst.h"
#include <sstream>

namespace SimpleLanguage {
namespace Core {

DynamicMetaClass::DynamicMetaClass() : MetaClass("Dynamic") {
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

DynamicMetaClass::DynamicMetaClass(const std::string& _name) : MetaClass(_name) {
    m_Type = EType::Class;
}

void DynamicMetaClass::ParseInnerFunction() {
    AddCoreFunction();
}

void DynamicMetaClass::ParseDefineComplete() {
    MetaClass::ParseDefineComplete();
}

std::string DynamicMetaClass::ToFormatString() const {
    std::stringstream stringBuilder;
    for (int i = 0; i < GetRealDeep(); i++)
        stringBuilder << Global::tabChar;
    
    stringBuilder << GetName() << " ";
    stringBuilder << std::endl;
    
    for (int i = 0; i < GetRealDeep(); i++)
        stringBuilder << Global::tabChar;
    stringBuilder << "{" << std::endl;

    for (int i = 0; i < GetRealDeep(); i++)
        stringBuilder << Global::tabChar;
    stringBuilder << "}" << std::endl;

    return stringBuilder.str();
}

std::string DynamicMetaClass::ToString() const {
    return MetaClass::ToString();
}

void DynamicMetaClass::AddCoreFunction() {
    MetaMemberFunction* Cast = new MetaMemberFunction(this, "cast");
    //Cast.AddMetaDefineParam(new MetaDefineParam("t", this, null, CoreMetaClassManager.templateMetaClass, null));
    //Cast.SetDefineMetaClass(CoreMetaClassManager.int32MetaClass);
    //AddMetaMemberFunction(Cast);

    MetaConstExpressNode* ecen = new MetaConstExpressNode(EType::Int32, 0);
    MetaMemberVariable* mmvobjectid = new MetaMemberVariable(this, "objectid", CoreMetaClassManager::GetInstance().GetInt32MetaClass(), ecen);
    AddMetaMemberVariable(mmvobjectid);

    //MetaMemberVariable mmvname = new MetaMemberVariable(this, "name", CoreMetaClassManager.stringMetaClass);
    //AddMetaMemberVariable(mmvname);

    MetaMemberFunction* _init_ = new MetaMemberFunction(this, "_init_");
    AddMetaMemberFunction(_init_);

    MetaMemberFunction* GetHashCode = new MetaMemberFunction(this, "GetHashCode");
    GetHashCode->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetInt32MetaClass());
    AddInnerMetaMemberFunction(GetHashCode);
    
    MetaMemberFunction* GetType = new MetaMemberFunction(this, "GetType");
    GetType->SetReturnMetaClass(this);
    AddInnerMetaMemberFunction(GetType);
    
    MetaMemberFunction* Clone = new MetaMemberFunction(this, "Clone");
    Clone->SetReturnMetaClass(this);
    AddInnerMetaMemberFunction(Clone);
    
    MetaMemberFunction* ToString = new MetaMemberFunction(this, "ToString");
    ToString->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetStringMetaClass());
    AddInnerMetaMemberFunction(ToString);
    
    MetaMemberFunction* ToShort = new MetaMemberFunction(this, "ToShort");
    ToShort->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetInt16MetaClass());
    AddInnerMetaMemberFunction(ToShort);
    
    MetaMemberFunction* ToInt = new MetaMemberFunction(this, "ToInt");
    ToInt->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetInt32MetaClass());
    AddInnerMetaMemberFunction(ToInt);
    
    MetaMemberFunction* ToLong = new MetaMemberFunction(this, "ToLong");
    ToLong->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetInt64MetaClass());
    AddInnerMetaMemberFunction(ToLong);
    
    MetaMemberFunction* ToFloat = new MetaMemberFunction(this, "ToFloat");
    ToFloat->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetFloat32MetaClass());
    AddInnerMetaMemberFunction(ToFloat);
    
    MetaMemberFunction* ToDouble = new MetaMemberFunction(this, "ToDouble");
    ToDouble->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetFloat64MetaClass());
    AddInnerMetaMemberFunction(ToDouble);
}

MetaClass* DynamicMetaClass::CreateMetaClass() {
    MetaClass* mc = new DynamicMetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

} // namespace Core
} // namespace SimpleLanguage
