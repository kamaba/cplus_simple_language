//****************************************************************************
//  File:      MetaData.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta data class
//****************************************************************************

#include "MetaData.h"
#include "MetaNode.h"

namespace SimpleLanguage {
namespace Core {

    MetaData::MetaData(const std::string& name)
    {
        m_Name = name;
    }

    std::string MetaData::ToFormatString() const
    {
        return "data " + m_Name;
    }

} // namespace Core
} // namespace SimpleLanguage
