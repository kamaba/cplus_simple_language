//****************************************************************************
//  File:      RangeMetaClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaClass.h"
#include <vector>

namespace SimpleLanguage {
namespace Core {

class RangeIteratorMetaClass : public MetaClass {
public:
    RangeIteratorMetaClass();
    
    virtual void ParseInnerVariable() override;    
    static MetaClass* CreateMetaClass();
};

class RangeMetaClass : public MetaClass {
public:
    RangeMetaClass();    
    virtual void ParseInnerVariable() override;    
    virtual void ParseInnerFunction() override;
    
    static MetaClass* CreateMetaClass();
};

} // namespace Core
} // namespace SimpleLanguage
