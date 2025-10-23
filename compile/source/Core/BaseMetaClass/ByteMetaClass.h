//****************************************************************************
//  File:      ByteMetaClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaClass.h"

namespace SimpleLanguage {
namespace Core {

class ByteMetaClass : public MetaClass {
public:
    ByteMetaClass();    
    static MetaClass* CreateMetaClass();
};

class SByteMetaClass : public MetaClass {
public:
    SByteMetaClass();    
    static MetaClass* CreateMetaClass();
};

} // namespace Core
} // namespace SimpleLanguage
