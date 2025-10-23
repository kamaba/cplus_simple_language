//****************************************************************************
//  File:      EnumMetaClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaClass.h"

namespace SimpleLanguage {
namespace Core {

class EnumMetaClass : public MetaClass {
public:
    EnumMetaClass();    
    virtual void ParseInnerFunction() override;    
    void AddCoreFunction();    
    static MetaClass* CreateMetaClass();
};

} // namespace Core
} // namespace SimpleLanguage
