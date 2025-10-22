//****************************************************************************
//  File:      RealMetaClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaClass.h"
#include "../MetaTemplate.h"
#include "../MetaConstExpressNode.h"
#include "CoreMetaClassManager.h"
#include "ClassManager.h"
#include "ModuleManager.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

class Float32MetaClass : public MetaClass {
public:
    Float32MetaClass() : MetaClass(DefaultObject::Float32.ToString()) {
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_ClassDefineType = EClassDefineType::InnerDefine;
        MetaConstExpressNode* mcen = new MetaConstExpressNode(EType::Float32, 0.0f);
        SetDefaultExpressNode(mcen);
        m_Type = EType::Float32;
    }
    
    virtual void ParseInnerFunction() override {
        AddCoreFunction();
    }
    
    void AddCoreFunction() {
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
    
    MetaClass* Cast(MetaTemplate* mt) {
        if (mt->GetName() == "Int32") {
            // Implementation for cast to Int32
        }
        return nullptr;
    }
    
    static std::string MetaToString(float v) {
        return std::to_string(v);
    }

    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new Float32MetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

class Float64MetaClass : public MetaClass {
public:
    Float64MetaClass() : MetaClass(DefaultObject::Float64.ToString()) {
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_ClassDefineType = EClassDefineType::InnerDefine;
        MetaConstExpressNode* mcen = new MetaConstExpressNode(EType::Float64, 0.0);
        SetDefaultExpressNode(mcen);
        m_Type = EType::Float64;
    }
    
    MetaClass* Cast(MetaTemplate* mc) {
        if (mc->GetName() == "Int32") {
            // Implementation for cast to Int32
        }
        return nullptr;
    }
    
    virtual void ParseInnerFunction() override {
        AddCoreFunction();
    }
    
    void AddCoreFunction() {
        //MetaMemberFunction toFloat = new MetaMemberFunction(this, "toFloat");
        //toFloat.SetMetaDefineType(new MetaType(CoreMetaClassManager.floatMetaClass));
        //AddMetaMemberFunction(toFloat);
        ////toFloat.AddCSharpMetaStatements("SimpleLanguage.VM.DoubleObject", "DoubleToFloat");

        //MetaMemberFunction ToString = new MetaMemberFunction(this, "toString");
        //ToString.SetMetaDefineType(new MetaType(CoreMetaClassManager.stringMetaClass));
        //AddMetaMemberFunction(ToString);
        //ToString.AddCSharpMetaStatements("SimpleLanguage.VM.DoubleObject", "DoubleToString");
    }
    
    static std::string MetaToString(double v) {
        return std::to_string(v);
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new Float64MetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

} // namespace Core
} // namespace SimpleLanguage
