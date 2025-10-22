//****************************************************************************
//  File:      MetaData.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta data class
//****************************************************************************

#pragma once

#include "MetaBase.h"

namespace SimpleLanguage {
namespace Core {

    class MetaNode;

    class MetaData : public MetaBase
    {
    public:
        MetaData(const std::string& name);
        virtual ~MetaData() = default;

        void SetMetaNode(MetaNode* node) { m_MetaNode = node; }
        std::string ToFormatString() const override;
    };

} // namespace Core
} // namespace SimpleLanguage
