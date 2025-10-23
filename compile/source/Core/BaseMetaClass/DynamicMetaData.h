//****************************************************************************
//  File:      DynamicMetaData.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaClass.h"

namespace SimpleLanguage {
namespace Core {

class DynamicMetaData : public MetaClass {
public:
    DynamicMetaData();
    virtual void ParseInnerFunction() override;    
    void AddCoreFunction();
    static MetaClass* CreateMetaClass();
};

} // namespace Core
} // namespace SimpleLanguage
