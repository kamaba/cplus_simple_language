//****************************************************************************
//  File:      MetaTemplate.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta template class
//****************************************************************************

#pragma once

#include "MetaBase.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

    class MetaClass;

    class MetaTemplate
    {
    public:
        MetaTemplate();
        MetaTemplate(MetaClass* ownerClass, const std::string& name, int index);
        virtual ~MetaTemplate() = default;

        // 属性访问器
        std::string GetName() const { return m_Name; }
        int GetIndex() const { return m_Index; }
        MetaClass* GetOwnerClass() const { return m_OwnerClass; }
        MetaClass* GetInConstraintMetaClass() const { return m_InConstraintMetaClass; }

        // 设置方法
        void SetName(const std::string& name) { m_Name = name; }
        void SetIndex(int index) { m_Index = index; }
        void SetOwnerClass(MetaClass* ownerClass) { m_OwnerClass = ownerClass; }
        void SetInConstraintMetaClass(MetaClass* metaClass) { m_InConstraintMetaClass = metaClass; }

        // 方法
        std::string ToFormatString() const;

    private:
        std::string m_Name;
        int m_Index = 0;
        MetaClass* m_OwnerClass = nullptr;
        MetaClass* m_InConstraintMetaClass = nullptr;
    };

} // namespace Core
} // namespace SimpleLanguage
