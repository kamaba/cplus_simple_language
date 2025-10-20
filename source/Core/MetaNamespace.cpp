//****************************************************************************
//  File:      MetaNamespace.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta namespace class
//****************************************************************************

#include "MetaNamespace.h"
#include "MetaNode.h"

namespace SimpleLanguage {
namespace Core {

    MetaNamespace::MetaNamespace(const std::string& name)
    {
        m_Name = name;
    }

    std::string MetaNamespace::ToFormatString() const
    {
        return "namespace " + m_Name;
    }

} // namespace Core
} // namespace SimpleLanguage
