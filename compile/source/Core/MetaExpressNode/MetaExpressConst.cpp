//****************************************************************************
//  File:      MetaExpressConst.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2025/5/18 12:00:00
//  Description: 
//****************************************************************************

#include "MetaExpressConst.h"
#include "../MetaType.h"
#include "../MetaInputTemplateCollection.h"
#include "../BaseMetaClass/CoreMetaClassManager.h"
#include "../Compile/CoreFileMeta/FileMetaConstValueTerm.h"
#include <sstream>
#include <iostream>

namespace SimpleLanguage {
namespace Core {

// 构造函数实现
MetaConstExpressNode::MetaConstExpressNode(FileMetaConstValueTerm* fmct) {
    m_FileMetaConstValueTerm = fmct;
    m_EType = fmct->GetToken()->GetEType();
    Parse1(m_EType, fmct->GetToken()->GetLexeme());
}

MetaConstExpressNode::MetaConstExpressNode(EType eType, const MultiData& val) {
    m_EType = eType;
    Parse1(eType, val);
}

MetaConstExpressNode::MetaConstExpressNode(MetaType* mt, const MultiData& val) {
    Parse1(m_EType, val);
}

// 操作符重载实现
MetaConstExpressNode MetaConstExpressNode::operator+(const MetaConstExpressNode& left, const MetaConstExpressNode& right) {
    if (left.GetOpLevel() > right.GetOpLevel()) {
        MetaConstExpressNode result = left;
        result.ComputeAddRight(right);
        return result;
    } else {
        MetaConstExpressNode result = right;
        result.ComputeAddRight(left);
        return result;
    }
}

MetaConstExpressNode MetaConstExpressNode::operator-(const MetaConstExpressNode& left, const MetaConstExpressNode& right) {
    if (left.GetOpLevel() > right.GetOpLevel()) {
        MetaConstExpressNode result = left;
        result.ComputeMinusRight(right);
        return result;
    } else {
        MetaConstExpressNode result = right;
        result.ComputeMinusRight(left);
        return result;
    }
}

MetaConstExpressNode MetaConstExpressNode::operator*(const MetaConstExpressNode& left, const MetaConstExpressNode& right) {
    if (left.GetOpLevel() > right.GetOpLevel()) {
        MetaConstExpressNode result = left;
        result.ComputeMulRight(right);
        return result;
    } else {
        MetaConstExpressNode result = right;
        result.ComputeMulRight(left);
        return result;
    }
}

MetaConstExpressNode MetaConstExpressNode::operator/(const MetaConstExpressNode& left, const MetaConstExpressNode& right) {
    if (left.GetOpLevel() > right.GetOpLevel()) {
        MetaConstExpressNode result = left;
        result.ComputeDivRight(right);
        return result;
    } else {
        MetaConstExpressNode result = right;
        result.ComputeDivRight(left);
        return result;
    }
}

MetaConstExpressNode MetaConstExpressNode::operator%(const MetaConstExpressNode& left, const MetaConstExpressNode& right) {
    if (left.GetOpLevel() > right.GetOpLevel()) {
        MetaConstExpressNode result = left;
        result.ComputeModRight(right);
        return result;
    } else {
        MetaConstExpressNode result = right;
        result.ComputeModRight(left);
        return result;
    }
}

// Parse1方法实现
void MetaConstExpressNode::Parse1(EType eType, const MultiData& val) {
    m_EType = eType;
    switch (eType) {
        case EType::Boolean: {
            if (val.type == DataType::String) {
                std::string strVal = val.ToString();
                m_Value = MultiData(strVal == "true");
            } else {
                m_Value = val;
            }
            break;
        }
        default: {
            m_Value = val;
            break;
        }
    }
}

// CalcReturnType方法实现
void MetaConstExpressNode::CalcReturnType() {
    if (m_MetaDefineType != nullptr) {
        if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetNullMetaClass()) {
            m_EType = EType::Null;
            m_Value = MultiData("null");
        } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetBooleanMetaClass()) {
            m_EType = EType::Boolean;
            m_Value = ConvertToMultiData(EType::Boolean, m_Value);
        } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetByteMetaClass()) {
            m_EType = EType::Byte;
            m_Value = ConvertToMultiData(EType::Byte, m_Value);
        } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetSByteMetaClass()) {
            m_EType = EType::SByte;
            m_Value = ConvertToMultiData(EType::SByte, m_Value);
        } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetInt16MetaClass()) {
            m_EType = EType::Int16;
            m_Value = ConvertToMultiData(EType::Int16, m_Value);
        } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetUInt16MetaClass()) {
            m_EType = EType::UInt16;
            m_Value = ConvertToMultiData(EType::UInt16, m_Value);
        } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetInt32MetaClass()) {
            m_EType = EType::Int32;
            m_Value = ConvertToMultiData(EType::Int32, m_Value);
        } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetUInt32MetaClass()) {
            m_EType = EType::UInt32;
            m_Value = ConvertToMultiData(EType::UInt32, m_Value);
        } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetInt64MetaClass()) {
            m_EType = EType::Int64;
            m_Value = ConvertToMultiData(EType::Int64, m_Value);
        } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetUInt64MetaClass()) {
            m_EType = EType::UInt64;
            m_Value = ConvertToMultiData(EType::UInt64, m_Value);
        } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetFloat32MetaClass()) {
            m_EType = EType::Float32;
            m_Value = ConvertToMultiData(EType::Float32, m_Value);
        } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetFloat64MetaClass()) {
            m_EType = EType::Float64;
            m_Value = ConvertToMultiData(EType::Float64, m_Value);
        } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetStringMetaClass()) {
            m_EType = EType::String;
            m_Value = ConvertToMultiData(EType::String, m_Value);
        } else {
            m_EType = EType::Class;
        }
        return;
    }
    
    if (m_EType == EType::Null) {
        m_MetaDefineType = new MetaType(CoreMetaClassManager::GetInstance().GetNullMetaClass());
    } else {
        MetaClass* mc = CoreMetaClassManager::GetMetaClassByEType(m_EType);
        
        if (mc == nullptr) {
            m_MetaDefineType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        } else {
            MetaInputTemplateCollection* mitc = new MetaInputTemplateCollection();
            if (m_EType == EType::Array) {
                MetaType* mitp = new MetaType(CoreMetaClassManager::GetInstance().GetInt32MetaClass());
                mitc->AddMetaTemplateParamsList(mitp);
                m_MetaDefineType = new MetaType(mc, nullptr, mitc);
            } else {
                m_MetaDefineType = new MetaType(mc);
            }
        }
    }
}

// ComputeAddRight方法实现
void MetaConstExpressNode::ComputeAddRight(const MetaConstExpressNode& right) {
    switch (right.m_EType) {
        case EType::Byte:
            m_Value = MultiData(static_cast<uint8_t>(m_Value.data.byte_val + right.m_Value.data.byte_val));
            break;
        case EType::Int16:
            m_Value = MultiData(static_cast<int16_t>(m_Value.data.short_val + right.m_Value.data.short_val));
            break;
        case EType::UInt16:
            m_Value = MultiData(static_cast<uint16_t>(m_Value.data.short_val + right.m_Value.data.short_val));
            break;
        case EType::Int32:
            m_Value = MultiData(m_Value.data.int_val + right.m_Value.data.int_val);
            break;
        case EType::UInt32:
            m_Value = MultiData(static_cast<uint32_t>(m_Value.data.int_val + right.m_Value.data.int_val));
            break;
        case EType::Int64:
            m_Value = MultiData(m_Value.data.long_val + right.m_Value.data.long_val);
            break;
        case EType::UInt64:
            m_Value = MultiData(static_cast<uint64_t>(m_Value.data.long_val + right.m_Value.data.long_val));
            break;
        case EType::String: {
            std::string leftStr = m_Value.ToString();
            std::string rightStr = right.m_Value.ToString();
            m_Value = MultiData(leftStr + rightStr);
            break;
        }
    }
}

// ComputeMinusRight方法实现
void MetaConstExpressNode::ComputeMinusRight(const MetaConstExpressNode& right) {
    switch (right.m_EType) {
        case EType::Byte:
            m_Value = MultiData(static_cast<uint8_t>(m_Value.data.byte_val - right.m_Value.data.byte_val));
            break;
        case EType::Int16:
            m_Value = MultiData(static_cast<int16_t>(m_Value.data.short_val - right.m_Value.data.short_val));
            break;
        case EType::UInt16:
            m_Value = MultiData(static_cast<uint16_t>(m_Value.data.short_val - right.m_Value.data.short_val));
            break;
        case EType::Int32:
            m_Value = MultiData(m_Value.data.int_val - right.m_Value.data.int_val);
            break;
        case EType::UInt32:
            m_Value = MultiData(static_cast<uint32_t>(m_Value.data.int_val - right.m_Value.data.int_val));
            break;
        case EType::Int64:
            m_Value = MultiData(m_Value.data.long_val - right.m_Value.data.long_val);
            break;
        case EType::UInt64:
            m_Value = MultiData(static_cast<uint64_t>(m_Value.data.long_val - right.m_Value.data.long_val));
            break;
    }
}

// ComputeMulRight方法实现
void MetaConstExpressNode::ComputeMulRight(const MetaConstExpressNode& right) {
    switch (right.m_EType) {
        case EType::Byte:
            m_Value = MultiData(static_cast<uint8_t>(m_Value.data.byte_val * right.m_Value.data.byte_val));
            break;
        case EType::Int16:
            m_Value = MultiData(static_cast<int16_t>(m_Value.data.short_val * right.m_Value.data.short_val));
            break;
        case EType::UInt16:
            m_Value = MultiData(static_cast<uint16_t>(m_Value.data.short_val * right.m_Value.data.short_val));
            break;
        case EType::Int32:
            m_Value = MultiData(m_Value.data.int_val * right.m_Value.data.int_val);
            break;
        case EType::UInt32:
            m_Value = MultiData(static_cast<uint32_t>(m_Value.data.int_val * right.m_Value.data.int_val));
            break;
        case EType::Int64:
            m_Value = MultiData(m_Value.data.long_val * right.m_Value.data.long_val);
            break;
        case EType::UInt64:
            m_Value = MultiData(static_cast<uint64_t>(m_Value.data.long_val * right.m_Value.data.long_val));
            break;
    }
}

// ComputeDivRight方法实现
void MetaConstExpressNode::ComputeDivRight(const MetaConstExpressNode& right) {
    switch (right.m_EType) {
        case EType::Byte:
            m_Value = MultiData(static_cast<uint8_t>(m_Value.data.byte_val / right.m_Value.data.byte_val));
            break;
        case EType::Int16:
            m_Value = MultiData(static_cast<int16_t>(m_Value.data.short_val / right.m_Value.data.short_val));
            break;
        case EType::UInt16:
            m_Value = MultiData(static_cast<uint16_t>(m_Value.data.short_val / right.m_Value.data.short_val));
            break;
        case EType::Int32:
            m_Value = MultiData(m_Value.data.int_val / right.m_Value.data.int_val);
            break;
        case EType::UInt32:
            m_Value = MultiData(static_cast<uint32_t>(m_Value.data.int_val / right.m_Value.data.int_val));
            break;
        case EType::Int64:
            m_Value = MultiData(m_Value.data.long_val / right.m_Value.data.long_val);
            break;
        case EType::UInt64:
            m_Value = MultiData(static_cast<uint64_t>(m_Value.data.long_val / right.m_Value.data.long_val));
            break;
    }
}

// ComputeModRight方法实现
void MetaConstExpressNode::ComputeModRight(const MetaConstExpressNode& right) {
    switch (right.m_EType) {
        case EType::Byte:
            m_Value = MultiData(static_cast<uint8_t>(m_Value.data.byte_val % right.m_Value.data.byte_val));
            break;
        case EType::Int16:
            m_Value = MultiData(static_cast<int16_t>(m_Value.data.short_val % right.m_Value.data.short_val));
            break;
        case EType::UInt16:
            m_Value = MultiData(static_cast<uint16_t>(m_Value.data.short_val % right.m_Value.data.short_val));
            break;
        case EType::Int32:
            m_Value = MultiData(m_Value.data.int_val % right.m_Value.data.int_val);
            break;
        case EType::UInt32:
            m_Value = MultiData(static_cast<uint32_t>(m_Value.data.int_val % right.m_Value.data.int_val));
            break;
        case EType::Int64:
            m_Value = MultiData(m_Value.data.long_val % right.m_Value.data.long_val);
            break;
        case EType::UInt64:
            m_Value = MultiData(static_cast<uint64_t>(m_Value.data.long_val % right.m_Value.data.long_val));
            break;
        case EType::String: {
            std::string leftStr = m_Value.ToString();
            std::string rightStr = right.m_Value.ToString();
            m_Value = MultiData(leftStr + rightStr);
            break;
        }
    }
}

// ComputeEqualComputeRight方法实现
void MetaConstExpressNode::ComputeEqualComputeRight(const MetaConstExpressNode& right, ELeftRightOpSign opSign) {
    switch (right.m_EType) {
        case EType::Byte:
            switch (opSign) {
                case ELeftRightOpSign::Equal:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.byte_val == right.m_Value.data.byte_val);
                    break;
                case ELeftRightOpSign::NotEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.byte_val != right.m_Value.data.byte_val);
                    break;
                case ELeftRightOpSign::Greater:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.byte_val > right.m_Value.data.byte_val);
                    break;
                case ELeftRightOpSign::GreaterOrEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.byte_val >= right.m_Value.data.byte_val);
                    break;
                case ELeftRightOpSign::Less:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.byte_val < right.m_Value.data.byte_val);
                    break;
                case ELeftRightOpSign::LessOrEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.byte_val <= right.m_Value.data.byte_val);
                    break;
            }
            break;
        case EType::Int16:
            switch (opSign) {
                case ELeftRightOpSign::Equal:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.short_val == right.m_Value.data.short_val);
                    break;
                case ELeftRightOpSign::NotEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.short_val != right.m_Value.data.short_val);
                    break;
                case ELeftRightOpSign::Greater:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.short_val > right.m_Value.data.short_val);
                    break;
                case ELeftRightOpSign::GreaterOrEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.short_val >= right.m_Value.data.short_val);
                    break;
                case ELeftRightOpSign::Less:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.short_val < right.m_Value.data.short_val);
                    break;
                case ELeftRightOpSign::LessOrEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.short_val <= right.m_Value.data.short_val);
                    break;
            }
            break;
        case EType::UInt16:
            switch (opSign) {
                case ELeftRightOpSign::Equal:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.short_val == right.m_Value.data.short_val);
                    break;
                case ELeftRightOpSign::NotEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.short_val != right.m_Value.data.short_val);
                    break;
                case ELeftRightOpSign::Greater:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.short_val > right.m_Value.data.short_val);
                    break;
                case ELeftRightOpSign::GreaterOrEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.short_val >= right.m_Value.data.short_val);
                    break;
                case ELeftRightOpSign::Less:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.short_val < right.m_Value.data.short_val);
                    break;
                case ELeftRightOpSign::LessOrEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.short_val <= right.m_Value.data.short_val);
                    break;
            }
            break;
        case EType::Int32:
            switch (opSign) {
                case ELeftRightOpSign::Equal:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.int_val == right.m_Value.data.int_val);
                    break;
                case ELeftRightOpSign::NotEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.int_val != right.m_Value.data.int_val);
                    break;
                case ELeftRightOpSign::Greater:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.int_val > right.m_Value.data.int_val);
                    break;
                case ELeftRightOpSign::GreaterOrEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.int_val >= right.m_Value.data.int_val);
                    break;
                case ELeftRightOpSign::Less:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.int_val < right.m_Value.data.int_val);
                    break;
                case ELeftRightOpSign::LessOrEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.int_val <= right.m_Value.data.int_val);
                    break;
            }
            break;
        case EType::UInt32:
            switch (opSign) {
                case ELeftRightOpSign::Equal:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.int_val == right.m_Value.data.int_val);
                    break;
                case ELeftRightOpSign::NotEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.int_val != right.m_Value.data.int_val);
                    break;
                case ELeftRightOpSign::Greater:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.int_val > right.m_Value.data.int_val);
                    break;
                case ELeftRightOpSign::GreaterOrEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.int_val >= right.m_Value.data.int_val);
                    break;
                case ELeftRightOpSign::Less:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.int_val < right.m_Value.data.int_val);
                    break;
                case ELeftRightOpSign::LessOrEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.int_val <= right.m_Value.data.int_val);
                    break;
            }
            break;
        case EType::Int64:
            switch (opSign) {
                case ELeftRightOpSign::Equal:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.long_val == right.m_Value.data.long_val);
                    break;
                case ELeftRightOpSign::NotEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.long_val != right.m_Value.data.long_val);
                    break;
                case ELeftRightOpSign::Greater:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.long_val > right.m_Value.data.long_val);
                    break;
                case ELeftRightOpSign::GreaterOrEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.long_val >= right.m_Value.data.long_val);
                    break;
                case ELeftRightOpSign::Less:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.long_val < right.m_Value.data.long_val);
                    break;
                case ELeftRightOpSign::LessOrEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.long_val <= right.m_Value.data.long_val);
                    break;
            }
            break;
        case EType::UInt64:
            switch (opSign) {
                case ELeftRightOpSign::Equal:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.long_val == right.m_Value.data.long_val);
                    break;
                case ELeftRightOpSign::NotEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.long_val != right.m_Value.data.long_val);
                    break;
                case ELeftRightOpSign::Greater:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.long_val > right.m_Value.data.long_val);
                    break;
                case ELeftRightOpSign::GreaterOrEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.long_val >= right.m_Value.data.long_val);
                    break;
                case ELeftRightOpSign::Less:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.long_val < right.m_Value.data.long_val);
                    break;
                case ELeftRightOpSign::LessOrEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.data.long_val <= right.m_Value.data.long_val);
                    break;
            }
            break;
        case EType::String:
            switch (opSign) {
                case ELeftRightOpSign::Equal:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.ToString() == right.m_Value.ToString());
                    break;
                case ELeftRightOpSign::NotEqual:
                    m_EType = EType::Boolean;
                    m_Value = MultiData(m_Value.ToString() != right.m_Value.ToString());
                    break;
                default:
                    std::cout << "Error Not Support string < <= > >=sign operator!!" << std::endl;
                    break;
            }
            break;
    }
}

// ToFormatString方法实现
std::string MetaConstExpressNode::ToFormatString() {
    std::string signEn = GetTypeSuffix(m_EType);
    std::string str;
    
    switch (m_EType) {
        case EType::Null:
            str = "null";
            break;
        case EType::String: {
            std::string val = m_Value.ToString();
            str = "\"" + val + "\"";
            break;
        }
        case EType::Int16:
            str = std::to_string(m_Value.data.short_val);
            break;
        case EType::UInt16:
            str = std::to_string(static_cast<uint16_t>(m_Value.data.short_val));
            break;
        case EType::Int32:
            str = std::to_string(m_Value.data.int_val);
            break;
        case EType::UInt32:
            str = std::to_string(static_cast<uint32_t>(m_Value.data.int_val));
            break;
        case EType::Int64:
            str = std::to_string(m_Value.data.long_val);
            break;
        case EType::UInt64:
            str = std::to_string(static_cast<uint64_t>(m_Value.data.long_val));
            break;
        case EType::Float32:
            str = std::to_string(m_Value.data.float_val);
            break;
        case EType::Float64:
            str = std::to_string(m_Value.data.double_val);
            break;
        default:
            str = std::to_string(m_Value.data.int_val);
            break;
    }
    return str + signEn;
}

// ToTokenString方法实现
std::string MetaConstExpressNode::ToTokenString() {
    std::string result;
    if (m_FileMetaConstValueTerm != nullptr) {
        result = m_FileMetaConstValueTerm->ToTokenString();
    }
    return result;
}

// 辅助方法实现
MultiData MetaConstExpressNode::ConvertToMultiData(EType targetType, const MultiData& value) {
    switch (targetType) {
        case EType::Boolean:
            return MultiData(value.data.int_val != 0);
        case EType::Byte:
            return MultiData(static_cast<uint8_t>(value.data.int_val));
        case EType::SByte:
            return MultiData(static_cast<int8_t>(value.data.int_val));
        case EType::Int16:
            return MultiData(static_cast<int16_t>(value.data.int_val));
        case EType::UInt16:
            return MultiData(static_cast<uint16_t>(value.data.int_val));
        case EType::Int32:
            return MultiData(value.data.int_val);
        case EType::UInt32:
            return MultiData(static_cast<uint32_t>(value.data.int_val));
        case EType::Int64:
            return MultiData(static_cast<int64_t>(value.data.int_val));
        case EType::UInt64:
            return MultiData(static_cast<uint64_t>(value.data.int_val));
        case EType::Float32:
            return MultiData(static_cast<float>(value.data.double_val));
        case EType::Float64:
            return MultiData(value.data.double_val);
        case EType::String:
            return MultiData(value.ToString());
        default:
            return value;
    }
}

std::string MetaConstExpressNode::GetTypeSuffix(EType type) const {
    switch (type) {
        case EType::Int16:
            return "s";
        case EType::UInt16:
            return "us";
        case EType::Int32:
            return "i";
        case EType::UInt32:
            return "ui";
        case EType::Int64:
            return "L";
        case EType::UInt64:
            return "uL";
        case EType::Float32:
            return "f";
        case EType::Float64:
            return "d";
        default:
            return "";
    }
}

} // namespace Core
} // namespace SimpleLanguage