//****************************************************************************
//  File:      MetaExpressNode.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta express node class
//****************************************************************************

#pragma once

#include "MetaBase.h"
#include "../Define.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

    class MetaType;

    class MetaExpressNode : public MetaBase
    {
    public:
        MetaExpressNode();
        virtual ~MetaExpressNode() = default;

        // Ðé·½·¨
        virtual void CalcReturnType() {}
        virtual MetaType* GetReturnMetaDefineType() const { return nullptr; }
        virtual std::string ToFormatString() const override { return ""; }
    };

} // namespace Core
} // namespace SimpleLanguage
