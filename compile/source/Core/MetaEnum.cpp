//****************************************************************************
//  File:      MetaEnum.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta enum class
//****************************************************************************

#include "MetaEnum.h"
#include "MetaNode.h"

namespace SimpleLanguage {
namespace Core {

    MetaEnum::MetaEnum(const std::string& name)
    {
        m_Name = name;
    }

    std::string MetaEnum::ToFormatString() const
    {
        return "enum " + m_Name;
    }

} // namespace Core
} // namespace SimpleLanguage
