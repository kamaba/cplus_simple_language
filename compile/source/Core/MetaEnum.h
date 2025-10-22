//****************************************************************************
//  File:      MetaEnum.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta enum class
//****************************************************************************

#pragma once

#include "MetaBase.h"

namespace SimpleLanguage {
namespace Core {

    class MetaNode;

    class MetaEnum : public MetaBase
    {
    public:
        MetaEnum(const std::string& name);
        virtual ~MetaEnum() = default;

        void setMetaNode(MetaNode* node) { m_MetaNode = node; }
        std::string ToFormatString() const override;
    };

} // namespace Core
} // namespace SimpleLanguage
