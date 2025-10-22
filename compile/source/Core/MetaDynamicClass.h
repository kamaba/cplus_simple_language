//****************************************************************************
//  File:      MetaDynamicClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2025/5/30 12:00:00
//  Description: meta dynamic class
//****************************************************************************

#pragma once

#include "MetaClass.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

class MetaDynamicClass : public MetaClass {
public:
    // ���캯��
    MetaDynamicClass(const std::string& _name);
    virtual ~MetaDynamicClass() = default;

    // ��д�ķ���
    virtual void ParseDefineComplete() override;
    virtual std::string ToFormatString() const override;
    virtual std::string ToString() const override;
};

} // namespace Core
} // namespace SimpleLanguage
