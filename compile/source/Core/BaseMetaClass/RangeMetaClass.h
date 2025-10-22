//****************************************************************************
//  File:      RangeMetaClass.h
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
#include <vector>

namespace SimpleLanguage {
namespace Core {

class RangeIteratorMetaClass : public MetaClass {
public:
    RangeIteratorMetaClass() : MetaClass(DefaultObject::Class.ToString()) {
        m_Type = EType::Class;
        m_ClassDefineType = EClassDefineType::InnerDefine;
    }
    
    virtual void ParseInnerVariable() override {
        MetaMemberVariable* index = new MetaMemberVariable(this, "index", CoreMetaClassManager::GetInstance().GetInt32MetaClass());
        AddMetaMemberVariable(index);

        //MetaMemberVariable tvalue = new MetaMemberVariable(this, "value", CoreMetaClassManager.templateMetaClass);
        //AddMetaMemberVariable(tvalue);
    }
    
    static MetaClass* CreateMetaClass() {
        RangeIteratorMetaClass* mc = new RangeIteratorMetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetSelfModule());
        return mc;
    }
};

class RangeMetaClass : public MetaClass {
public:
    RangeMetaClass() : MetaClass(DefaultObject::Range.ToString()) {
        m_Type = EType::Range;
        SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        m_ClassDefineType = EClassDefineType::InnerDefine;
        m_MetaTemplateList.push_back(std::make_unique<MetaTemplate>(this, "T"));
    }
    
    virtual void ParseInnerVariable() override {
        // MetaTemplate mt = m_MetaTemplateList[0];

        // MetaMemberVariable m_Start = new MetaMemberVariable(this, "m_Start", mt );
        // AddMetaMemberVariable(m_Start);
        // MetaMemberVariable m_End = new MetaMemberVariable(this, "m_End", mt );
        // AddMetaMemberVariable(m_End);
        // MetaMemberVariable m_Step = new MetaMemberVariable(this, "m_Step", mt );
        // AddMetaMemberVariable(m_Step);

        // MetaMemberVariable index = new MetaMemberVariable(this, "index", CoreMetaClassManager.int32MetaClass);
        // AddMetaMemberVariable(index);

        ////MetaMemberVariable tvalue = new MetaMemberVariable(this, "value", CoreMetaClassManager.templateMetaClass);
        ////AddMetaMemberVariable(tvalue);
    }
    
    virtual void ParseInnerFunction() override {
        MetaMemberFunction* _init_ = new MetaMemberFunction(this, "_init_");
        _init_->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetRangeMetaClass());

        // MetaTemplate mt = m_MetaTemplateList[0];

        // _init_.AddMetaDefineParam(new MetaDefineParam("_start", this, null, mt ));
        // _init_.AddMetaDefineParam(new MetaDefineParam("_end", this, null, mt ));
        // _init_.AddMetaDefineParam(new MetaDefineParam("_step", this, null, mt ));
        // AddInnerMetaMemberFunction(_init_);

        // MetaMemberFunction IsIn = new MetaMemberFunction(this, "IsIn");
        // IsIn.AddMetaDefineParam(new MetaDefineParam("name", this, null, mt ));
        // IsIn.SetMetaDefineType(new MetaType(CoreMetaClassManager.voidMetaClass));
        // AddInnerMetaMemberFunction(IsIn);
    }
    
    void Init_Call(const std::vector<int>& arrObj, int count) {
        // Implementation for Init_Call
    }
    
    static MetaClass* CreateMetaClass() {
        MetaClass* mc = new RangeMetaClass();
        ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
        return mc;
    }
};

} // namespace Core
} // namespace SimpleLanguage
