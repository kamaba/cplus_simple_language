//****************************************************************************
//  File:      IntMetaClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaClass.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

class Int16MetaClass : public MetaClass {
public:
    Int16MetaClass();
    static MetaClass* CreateMetaClass();
};

class UInt16MetaClass : public MetaClass {
public:
    UInt16MetaClass();    
    static MetaClass* CreateMetaClass();
};

class Int32MetaClass : public MetaClass {
public:
    Int32MetaClass();    
    virtual void ParseInnerFunction() override;    
    void AddCoreFunction();    
    static MetaClass* CreateMetaClass();
};

class UInt32MetaClass : public MetaClass {
public:
    UInt32MetaClass();    
    static MetaClass* CreateMetaClass();
};

class Int64MetaClass : public MetaClass {
public:
    Int64MetaClass();    
    static MetaClass* CreateMetaClass();
};

class UInt64MetaClass : public MetaClass {
public:
    UInt64MetaClass();    
    static MetaClass* CreateMetaClass();
};

} // namespace Core
} // namespace SimpleLanguage
