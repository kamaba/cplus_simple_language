//****************************************************************************
//  File:      DynamicMetaData.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaClass.h"
#include "../MetaTemplate.h"
#include "../MetaMemberFunction.h"
#include "../MetaMemberVariable.h"
#include "CoreMetaClassManager.h"
#include "ClassManager.h"
#include "ModuleManager.h"

namespace SimpleLanguage {
namespace Core {
namespace SelfMeta {

class DynamicMetaData : public MetaClass {
public:
    DynamicMetaData() : MetaClass(DefaultObject::Data.ToString()) {
        m_ClassDefineType = EClassDefineType::InnerDefine;
    }
    
    MetaClass* Cast(MetaTemplate* mc) {
        return nullptr;
    }
    
    virtual void ParseInnerFunction() override {
        AddCoreFunction();
    }
    
    void AddCoreFunction() {
        MetaMemberFunction* Cast = new MetaMemberFunction(this, "cast");
        //Cast.AddMetaDefineParam(new MetaDefineParam("t", this, null, CoreMetaClassManager.templateMetaClass, null));
        //Cast.SetDefineMetaClass(CoreMetaClassManager.int32MetaClass);
        //AddMetaMemberFunction(Cast);

        //MetaMemberVariable mmvobjectid = new MetaMemberVariable(this, "objectid", CoreMetaClassManager.int32MetaClass);
        //AddMetaMemberVariable(mmvobjectid);

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
        
        MetaMemberFunction* ToString = new MetaMemberFunction(this, "toString");
        ToString->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetStringMetaClass());
        AddInnerMetaMemberFunction(ToString);
        
        MetaMemberFunction* ToShort = new MetaMemberFunction(this, "toShort");
        ToShort->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetInt16MetaClass());
        AddInnerMetaMemberFunction(ToShort);
        
        MetaMemberFunction* ToInt = new MetaMemberFunction(this, "toInt");
        ToInt->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetInt32MetaClass());
        AddInnerMetaMemberFunction(ToInt);
        
        MetaMemberFunction* ToLong = new MetaMemberFunction(this, "toLong");
        ToLong->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetInt64MetaClass());
        AddInnerMetaMemberFunction(ToLong);
        
        MetaMemberFunction* ToFloat = new MetaMemberFunction(this, "toFloat32");
        ToFloat->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetFloat32MetaClass());
        AddInnerMetaMemberFunction(ToFloat);
        
        MetaMemberFunction* ToDouble = new MetaMemberFunction(this, "toFloat64");
        ToDouble->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetFloat64MetaClass());
        AddInnerMetaMemberFunction(ToDouble);
    }

    static MetaClass* CreateMetaClass() {
        DynamicMetaData* mc = new DynamicMetaData();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

} // namespace SelfMeta
} // namespace Core
} // namespace SimpleLanguage
