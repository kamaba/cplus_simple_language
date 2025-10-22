//****************************************************************************
//  File:      MetaDefineParam.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta define param class
//****************************************************************************

#pragma once

#include "MetaBase.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

    class MetaMemberFunction;
    class MetaType;
    class MetaVariable;

    class MetaDefineParam : public MetaBase
    {
    public:
        MetaDefineParam();
        MetaDefineParam(MetaMemberFunction* ownerFunction, void* param); // 简化实现
        virtual ~MetaDefineParam() = default;

        // 属性访问器
        MetaMemberFunction* GetOwnerFunction() const { return m_OwnerFunction; }
        MetaType* GetMetaDefineType() const { return m_MetaDefineType; }
        MetaVariable* GetMetaVariable() const { return m_MetaVariable; }

        // 设置方法
        void SetOwnerFunction(MetaMemberFunction* ownerFunction) { m_OwnerFunction = ownerFunction; }
        void SetMetaDefineType(MetaType* metaDefineType) { m_MetaDefineType = metaDefineType; }
        void SetMetaVariable(MetaVariable* metaVariable) { m_MetaVariable = metaVariable; }

        // 方法
        void ParseMetaDefineType();
        void CreateExpress();
        void Parse();
        void CalcReturnType();

    private:
        MetaMemberFunction* m_OwnerFunction = nullptr;
        MetaType* m_MetaDefineType = nullptr;
        MetaVariable* m_MetaVariable = nullptr;
    };

} // namespace Core
} // namespace SimpleLanguage
