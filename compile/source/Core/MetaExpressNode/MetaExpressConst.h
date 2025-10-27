//****************************************************************************
//  File:      MetaExpressConst.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2025/5/18 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "MetaExpressBase.h"
#include "../MetaType.h"
#include "../../Compile/Token.h"
#include <string>
#include <sstream>
#include <iostream>

namespace SimpleLanguage {
    namespace Compile
    {
        class FileMetaConstValueTerm;
        struct MultiData;
    }
namespace Core {

class MetaConstExpressNode : public MetaExpressNode {
private:
    Compile::FileMetaConstValueTerm* m_FileMetaConstValueTerm = nullptr;
    Compile::MultiData m_Value;
    EType m_EType = EType::None;

public:
    // 构造函数
    MetaConstExpressNode(Compile::FileMetaConstValueTerm* fmct);
    MetaConstExpressNode(EType eType, const Compile::MultiData& val);
    MetaConstExpressNode(MetaType* mt, const Compile::MultiData& val);
    
    // 操作符重载
    friend MetaConstExpressNode operator+(const MetaConstExpressNode& left, const MetaConstExpressNode& right);
    friend MetaConstExpressNode operator-(const MetaConstExpressNode& left, const MetaConstExpressNode& right);
    friend MetaConstExpressNode operator*(const MetaConstExpressNode& left, const MetaConstExpressNode& right);
    friend MetaConstExpressNode operator/(const MetaConstExpressNode& left, const MetaConstExpressNode& right);
    friend MetaConstExpressNode operator%(const MetaConstExpressNode& left, const MetaConstExpressNode& right);
    
    // 属性访问器
    Compile::MultiData GetValue() const { return m_Value; }
    EType GetEType() const { return m_EType; }
    std::string GetETypeString() { std::stringstream ss; ss << (int)m_EType; return ss.str(); }
    
    // 重写的方法
    virtual void CalcReturnType() override;
    virtual std::string ToFormatString() override;
    virtual std::string ToTokenString() override;

public:
    void Parse(EType eType, const Compile::MultiData& val);
    void ComputeAddRight(const MetaConstExpressNode& right);
    void ComputeMinusRight(const MetaConstExpressNode& right);
    void ComputeMulRight(const MetaConstExpressNode& right);
    void ComputeDivRight(const MetaConstExpressNode& right);
    void ComputeModRight(const MetaConstExpressNode& right);
    void ComputeEqualComputeRight(const MetaConstExpressNode& right, ELeftRightOpSign opSign);
    
    // 辅助方法
    Compile::MultiData ConvertToMultiData(EType targetType, const Compile::MultiData& value);
    std::string GetTypeSuffix(EType type) const;
};

} // namespace Core
} // namespace SimpleLanguage