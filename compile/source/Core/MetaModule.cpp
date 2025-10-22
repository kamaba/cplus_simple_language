//****************************************************************************
//  File:      MetaModule.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta module class
//****************************************************************************

#include "MetaModule.h"
#include "MetaNode.h"

namespace SimpleLanguage {
namespace Core {

    MetaModule::MetaModule(const std::string& name)
    {
        m_Name = name;
    }

    std::string MetaModule::ToFormatString() const
    {
        return "module " + m_Name;
    }

} // namespace Core
} // namespace SimpleLanguage
