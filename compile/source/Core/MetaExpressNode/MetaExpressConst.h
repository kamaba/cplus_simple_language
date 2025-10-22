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
#include "../MetaInputTemplateCollection.h"
#include "../BaseMetaClass/CoreMetaClassManager.h"
#include "../Compile/CoreFileMeta/FileMetaConstValueTerm.h"
#include "../Compile/Token.h"
#include <string>
#include <sstream>
#include <iostream>

namespace SimpleLanguage {
namespace Core {

class MetaConstExpressNode : public MetaExpressNode {
private:
    FileMetaConstValueTerm* m_FileMetaConstValueTerm = nullptr;
    MultiData m_Value;
    EType m_EType = EType::None;

public:
    // 构造函数
    MetaConstExpressNode(FileMetaConstValueTerm* fmct);
    MetaConstExpressNode(EType eType, const MultiData& val);
    MetaConstExpressNode(MetaType* mt, const MultiData& val);
    
    // 操作符重载
    static MetaConstExpressNode operator+(const MetaConstExpressNode& left, const MetaConstExpressNode& right);
    static MetaConstExpressNode operator-(const MetaConstExpressNode& left, const MetaConstExpressNode& right);
    static MetaConstExpressNode operator*(const MetaConstExpressNode& left, const MetaConstExpressNode& right);
    static MetaConstExpressNode operator/(const MetaConstExpressNode& left, const MetaConstExpressNode& right);
    static MetaConstExpressNode operator%(const MetaConstExpressNode& left, const MetaConstExpressNode& right);
    
    // 属性访问器
    MultiData GetValue() const { return m_Value; }
    EType GetEType() const { return m_EType; }
    
    // 重写的方法
    virtual void CalcReturnType() override;
    virtual std::string ToFormatString() override;
    virtual std::string ToTokenString() override;

private:
    void Parse1(EType eType, const MultiData& val);
    void ComputeAddRight(const MetaConstExpressNode& right);
    void ComputeMinusRight(const MetaConstExpressNode& right);
    void ComputeMulRight(const MetaConstExpressNode& right);
    void ComputeDivRight(const MetaConstExpressNode& right);
    void ComputeModRight(const MetaConstExpressNode& right);
    void ComputeEqualComputeRight(const MetaConstExpressNode& right, ELeftRightOpSign opSign);
    
    // 辅助方法
    MultiData ConvertToMultiData(EType targetType, const MultiData& value);
    std::string GetTypeSuffix(EType type) const;
};

} // namespace Core
} // namespace SimpleLanguage