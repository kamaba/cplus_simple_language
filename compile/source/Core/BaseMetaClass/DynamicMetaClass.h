//****************************************************************************
//  File:      DynamicMetaClass.h
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
#include "../MetaExpressNode/MetaExpressConst.h"
#include "CoreMetaClassManager.h"
#include "../ClassManager.h"
#include "../ModuleManager.h"

namespace SimpleLanguage {
namespace Core {

class DynamicMetaClass : public MetaClass {
public:
    DynamicMetaClass();    
    DynamicMetaClass(const std::string& _name);
    
    virtual void ParseInnerFunction() override;    
    virtual void ParseDefineComplete() override;    
    virtual std::string ToFormatString() const override;    
    virtual std::string ToString() const override;    
    void AddCoreFunction();

    static MetaClass* CreateMetaClass();
};

} // namespace Core
} // namespace SimpleLanguage
