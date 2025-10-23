//****************************************************************************
//  File:      ArrayMetaClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaClass.h"
#include "../MetaTemplate.h"
#include "../MetaMemberVariable.h"
#include "../MetaMemberFunction.h"
#include "../MetaExpressNode/MetaExpressConst.h"
#include "CoreMetaClassManager.h"
#include "../ClassManager.h"
#include "../ModuleManager.h"
#include <cstdint>

namespace SimpleLanguage {
namespace Core {

class IEnumerableMetaClass : public MetaClass {
public:
    IEnumerableMetaClass();
};

class ArrayIteratorMetaClass : public MetaClass {
public:
    ArrayIteratorMetaClass();

    virtual void ParseInnerVariable() override;    
    static MetaClass* CreateMetaClass();
};

class ArrayMetaClass : public MetaClass {
public:
    ArrayMetaClass();
    
    virtual void ParseInnerVariable() override;    
    virtual void ParseInnerFunction() override;    
    void AddCoreFunction();
    
    static MetaClass* CreateMetaClass();    
    static int SetArrayLength(uint32_t v);
};

} // namespace Core
} // namespace SimpleLanguage
