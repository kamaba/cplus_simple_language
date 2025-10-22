//****************************************************************************
//  File:      MetaType.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta type class
//****************************************************************************

#pragma once

#include "MetaBase.h"
#include "../Define.h"

namespace SimpleLanguage {
namespace Core {

    enum class EMetaTypeType : uint8_t
    {
        MetaClass = 0,
        MetaGenClass = 1,
        TemplateClassWithTemplate = 2
    };

    class MetaClass;
    class MetaTemplate;

    class MetaType
    {
    public:
        MetaType();
        MetaType(MetaClass* metaClass);
        MetaType(const MetaType& mt);
        virtual ~MetaType() = default;

        // 属性访问器
        EMetaTypeType GetEType() const { return m_EType; }
        MetaClass* GetMetaClass() const { return m_MetaClass; }
        const std::vector<MetaType*>& GetTemplateMetaTypeList() const { return m_TemplateMetaTypeList; }
        bool IsTemplate() const { return m_IsTemplate; }
        bool IsIncludeTemplate() const { return m_IsIncludeTemplate; }

        // 设置方法
        void SetMetaClass(MetaClass* metaClass) { m_MetaClass = metaClass; }
        void SetEType(EMetaTypeType eType) { m_EType = eType; }
        void SetTemplate(bool isTemplate) { m_IsTemplate = isTemplate; }
        void SetIncludeTemplate(bool include) { m_IsIncludeTemplate = include; }

        // 方法
        std::string ToFormatString() const;
        bool IsIncludeClassTemplate(MetaClass* metaClass) const;
        static bool EqualMetaDefineType(const MetaType* v, const MetaType* mt);

    private:
        EMetaTypeType m_EType = EMetaTypeType::MetaClass;
        MetaClass* m_MetaClass = nullptr;
        std::vector<MetaType*> m_TemplateMetaTypeList;
        bool m_IsTemplate = false;
        bool m_IsIncludeTemplate = false;
    };

} // namespace Core
} // namespace SimpleLanguage
