//****************************************************************************
//  File:      MetaTemplate.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta template class
//****************************************************************************

#include "MetaTemplate.h"
#include "MetaClass.h"

namespace SimpleLanguage {
namespace Core {

    MetaTemplate::MetaTemplate()
    {
    }

    MetaTemplate::MetaTemplate(MetaClass* ownerClass, const std::string& name, int index)
        : m_OwnerClass(ownerClass)
        , m_Name(name)
        , m_Index(index)
    {
    }

    std::string MetaTemplate::ToFormatString() const
    {
        return m_Name;
    }

} // namespace Core
} // namespace SimpleLanguage
