//****************************************************************************
//  File:      StringMetaClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaClass.h"

namespace SimpleLanguage {
namespace Core {

class StringMetaClass : public MetaClass {
public:
    StringMetaClass();    
    static MetaClass* CreateMetaClass();
};

} // namespace Core
} // namespace SimpleLanguage
