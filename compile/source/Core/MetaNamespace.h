//****************************************************************************
//  File:      MetaNamespace.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta namespace class
//****************************************************************************

#pragma once

#include "MetaBase.h"

namespace SimpleLanguage {
namespace Core {

    class MetaNode;

    class MetaNamespace : public MetaBase
    {
    public:
        MetaNamespace(const std::string& name);
        virtual ~MetaNamespace() = default;

        bool GetIsNowAllowCreateName() { return m_IsNotAllowCreateName; }
        void SetIsNowAllowCreateName(bool isAllow) { m_IsNotAllowCreateName = isAllow; }  

        void SetMetaNode(MetaNode* node) { m_MetaNode = node; }
        std::string ToFormatString() const override;

    private:
        bool m_IsNotAllowCreateName = false;
    };

} // namespace Core
} // namespace SimpleLanguage
