//****************************************************************************
//  File:      MetaBase.cpp
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/5/17 12:00:00
//  Description:  Core MetaBase is a basement class, attribute value has name or tree's deepvalue or tree struct node!
//****************************************************************************

#include "MetaBase.h"
#include "MetaNode.h"

namespace SimpleLanguage {
namespace Core {

    MetaBase::MetaBase()
    {
        m_RefFromType = RefFromType::Local;
    }

    MetaBase::MetaBase(const MetaBase& mb)
        : m_Name(mb.m_Name)
        , m_AllName(mb.m_AllName)
        , m_RefFromType(mb.m_RefFromType)
        , m_Permission(mb.m_Permission)
        , m_MetaNode(mb.m_MetaNode)
    {
    }

    std::string MetaBase::getPathName() const
    {
        if (m_MetaNode) {
            return m_MetaNode->getAllName();
        }
        return "";
    }

} // namespace Core
} // namespace SimpleLanguage
