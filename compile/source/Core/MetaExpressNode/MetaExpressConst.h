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
#include <string>
#include <sstream>
#include <iostream>

namespace SimpleLanguage {
namespace Core {

class MetaConstExpressNode : public MetaExpressNode {
private:
    FileMetaConstValueTerm* m_FileMetaConstValueTerm = nullptr;
    std::any value;
    EType eType = EType::None;

public:
    MetaConstExpressNode(FileMetaConstValueTerm* fmct) {
        m_FileMetaConstValueTerm = fmct;
        eType = fmct->GetToken()->GetEType();
        Parse1(eType, fmct->GetToken()->GetLexeme());
    }
    
    MetaConstExpressNode(EType _eType, const std::any& val) {
        eType = _eType;
        Parse1(_eType, val);
    }
    
    MetaConstExpressNode(MetaType* mt, const std::any& val) {
        Parse1(eType, val);
    }
    
    // Operator overloads
    static MetaConstExpressNode operator+(const MetaConstExpressNode& left, const MetaConstExpressNode& right) {
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
    
    static MetaConstExpressNode operator-(const MetaConstExpressNode& left, const MetaConstExpressNode& right) {
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
    
    static MetaConstExpressNode operator*(const MetaConstExpressNode& left, const MetaConstExpressNode& right) {
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
    
    static MetaConstExpressNode operator/(const MetaConstExpressNode& left, const MetaConstExpressNode& right) {
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
    
    static MetaConstExpressNode operator%(const MetaConstExpressNode& left, const MetaConstExpressNode& right) {
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
    
    std::any GetValue() const { return value; }
    EType GetEType() const { return eType; }
    
private:
    void Parse1(EType _etype, const std::any& val) {
        eType = _etype;
        switch (eType) {
            case EType::Boolean: {
                std::string strVal = std::any_cast<std::string>(val);
                value = (strVal == "true");
                break;
            }
            default: {
                value = val;
                break;
            }
        }
    }

public:
    virtual void CalcReturnType() override {
        if (m_MetaDefineType != nullptr) {
            if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetNullMetaClass()) {
                eType = EType::Null;
                value = std::string("null");
            } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetBooleanMetaClass()) {
                eType = EType::Boolean;
                value = std::any_cast<bool>(value);
            } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetByteMetaClass()) {
                eType = EType::Byte;
                value = static_cast<uint8_t>(std::any_cast<int>(value));
            } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetSByteMetaClass()) {
                eType = EType::SByte;
                value = static_cast<int8_t>(std::any_cast<int>(value));
            } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetInt16MetaClass()) {
                eType = EType::Int16;
                value = static_cast<int16_t>(std::any_cast<int>(value));
            } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetUInt16MetaClass()) {
                eType = EType::UInt16;
                value = static_cast<uint16_t>(std::any_cast<int>(value));
            } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetInt32MetaClass()) {
                eType = EType::Int32;
                value = static_cast<int32_t>(std::any_cast<int>(value));
            } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetUInt32MetaClass()) {
                eType = EType::UInt32;
                value = static_cast<uint32_t>(std::any_cast<int>(value));
            } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetInt64MetaClass()) {
                eType = EType::Int64;
                value = static_cast<int64_t>(std::any_cast<int>(value));
            } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetUInt64MetaClass()) {
                eType = EType::UInt64;
                value = static_cast<uint64_t>(std::any_cast<int>(value));
            } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetFloat32MetaClass()) {
                eType = EType::Float32;
                value = static_cast<float>(std::any_cast<double>(value));
            } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetFloat64MetaClass()) {
                eType = EType::Float64;
                value = std::any_cast<double>(value);
            } else if (m_MetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetStringMetaClass()) {
                eType = EType::String;
                value = std::any_cast<std::string>(value);
            } else {
                eType = EType::Class;
            }
            return;
        }
        
        if (eType == EType::Null) {
            m_MetaDefineType = new MetaType(CoreMetaClassManager::GetInstance().GetNullMetaClass());
        } else {
            MetaClass* mc = CoreMetaClassManager::GetMetaClassByEType(eType);
            
            if (mc == nullptr) {
                m_MetaDefineType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
            } else {
                MetaInputTemplateCollection* mitc = new MetaInputTemplateCollection();
                if (eType == EType::Array) {
                    MetaType* mitp = new MetaType(CoreMetaClassManager::GetInstance().GetInt32MetaClass());
                    mitc->AddMetaTemplateParamsList(mitp);
                    m_MetaDefineType = new MetaType(mc, nullptr, mitc);
                } else {
                    m_MetaDefineType = new MetaType(mc);
                }
            }
        }
    }
    
    void ComputeAddRight(const MetaConstExpressNode& right) {
        switch (right.eType) {
            case EType::Byte:
                value = std::any_cast<uint8_t>(value) + std::any_cast<uint8_t>(right.value);
                break;
            case EType::Int16:
                value = std::any_cast<int16_t>(value) + std::any_cast<int16_t>(right.value);
                break;
            case EType::UInt16:
                value = std::any_cast<uint16_t>(value) + std::any_cast<uint16_t>(right.value);
                break;
            case EType::Int32:
                value = std::any_cast<int32_t>(value) + std::any_cast<int32_t>(right.value);
                break;
            case EType::UInt32:
                value = std::any_cast<uint32_t>(value) + std::any_cast<uint32_t>(right.value);
                break;
            case EType::Int64:
                value = std::any_cast<int64_t>(value) + std::any_cast<int64_t>(right.value);
                break;
            case EType::UInt64:
                value = std::any_cast<uint64_t>(value) + std::any_cast<uint64_t>(right.value);
                break;
            case EType::String: {
                std::string leftStr = std::any_cast<std::string>(value);
                std::string rightStr = std::any_cast<std::string>(right.value);
                value = leftStr + rightStr;
                break;
            }
        }
    }
    
    void ComputeMinusRight(const MetaConstExpressNode& right) {
        switch (right.eType) {
            case EType::Byte:
                value = std::any_cast<uint8_t>(value) - std::any_cast<uint8_t>(right.value);
                break;
            case EType::Int16:
                value = std::any_cast<int16_t>(value) - std::any_cast<int16_t>(right.value);
                break;
            case EType::UInt16:
                value = std::any_cast<uint16_t>(value) - std::any_cast<uint16_t>(right.value);
                break;
            case EType::Int32:
                value = std::any_cast<int32_t>(value) - std::any_cast<int32_t>(right.value);
                break;
            case EType::UInt32:
                value = std::any_cast<uint32_t>(value) - std::any_cast<uint32_t>(right.value);
                break;
            case EType::Int64:
                value = std::any_cast<int64_t>(value) - std::any_cast<int64_t>(right.value);
                break;
            case EType::UInt64:
                value = std::any_cast<uint64_t>(value) - std::any_cast<uint64_t>(right.value);
                break;
        }
    }
    
    void ComputeMulRight(const MetaConstExpressNode& right) {
        switch (right.eType) {
            case EType::Byte:
                value = std::any_cast<uint8_t>(value) * std::any_cast<uint8_t>(right.value);
                break;
            case EType::Int16:
                value = std::any_cast<int16_t>(value) * std::any_cast<int16_t>(right.value);
                break;
            case EType::UInt16:
                value = std::any_cast<uint16_t>(value) * std::any_cast<uint16_t>(right.value);
                break;
            case EType::Int32:
                value = std::any_cast<int32_t>(value) * std::any_cast<int32_t>(right.value);
                break;
            case EType::UInt32:
                value = std::any_cast<uint32_t>(value) * std::any_cast<uint32_t>(right.value);
                break;
            case EType::Int64:
                value = std::any_cast<int64_t>(value) * std::any_cast<int64_t>(right.value);
                break;
            case EType::UInt64:
                value = std::any_cast<uint64_t>(value) * std::any_cast<uint64_t>(right.value);
                break;
        }
    }
    
    void ComputeDivRight(const MetaConstExpressNode& right) {
        switch (right.eType) {
            case EType::Byte:
                value = std::any_cast<uint8_t>(value) / std::any_cast<uint8_t>(right.value);
                break;
            case EType::Int16:
                value = std::any_cast<int16_t>(value) / std::any_cast<int16_t>(right.value);
                break;
            case EType::UInt16:
                value = std::any_cast<uint16_t>(value) / std::any_cast<uint16_t>(right.value);
                break;
            case EType::Int32:
                value = std::any_cast<int32_t>(value) / std::any_cast<int32_t>(right.value);
                break;
            case EType::UInt32:
                value = std::any_cast<uint32_t>(value) / std::any_cast<uint32_t>(right.value);
                break;
            case EType::Int64:
                value = std::any_cast<int64_t>(value) / std::any_cast<int64_t>(right.value);
                break;
            case EType::UInt64:
                value = std::any_cast<uint64_t>(value) / std::any_cast<uint64_t>(right.value);
                break;
        }
    }
    
    void ComputeModRight(const MetaConstExpressNode& right) {
        switch (right.eType) {
            case EType::Byte:
                value = std::any_cast<uint8_t>(value) % std::any_cast<uint8_t>(right.value);
                break;
            case EType::Int16:
                value = std::any_cast<int16_t>(value) % std::any_cast<int16_t>(right.value);
                break;
            case EType::UInt16:
                value = std::any_cast<uint16_t>(value) % std::any_cast<uint16_t>(right.value);
                break;
            case EType::Int32:
                value = std::any_cast<int32_t>(value) % std::any_cast<int32_t>(right.value);
                break;
            case EType::UInt32:
                value = std::any_cast<uint32_t>(value) % std::any_cast<uint32_t>(right.value);
                break;
            case EType::Int64:
                value = std::any_cast<int64_t>(value) % std::any_cast<int64_t>(right.value);
                break;
            case EType::UInt64:
                value = std::any_cast<uint64_t>(value) % std::any_cast<uint64_t>(right.value);
                break;
            case EType::String: {
                std::string leftStr = std::any_cast<std::string>(value);
                std::string rightStr = std::any_cast<std::string>(right.value);
                value = leftStr + rightStr;
                break;
            }
        }
    }
    
    void ComputeEqualComputeRight(const MetaConstExpressNode& right, ELeftRightOpSign opSign) {
        switch (right.eType) {
            case EType::Byte:
                switch (opSign) {
                    case ELeftRightOpSign::Equal:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint8_t>(value) == std::any_cast<uint8_t>(right.value));
                        break;
                    case ELeftRightOpSign::NotEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint8_t>(value) != std::any_cast<uint8_t>(right.value));
                        break;
                    case ELeftRightOpSign::Greater:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint8_t>(value) > std::any_cast<uint8_t>(right.value));
                        break;
                    case ELeftRightOpSign::GreaterOrEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint8_t>(value) >= std::any_cast<uint8_t>(right.value));
                        break;
                    case ELeftRightOpSign::Less:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint8_t>(value) < std::any_cast<uint8_t>(right.value));
                        break;
                    case ELeftRightOpSign::LessOrEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint8_t>(value) <= std::any_cast<uint8_t>(right.value));
                        break;
                }
                break;
            case EType::Int16:
                switch (opSign) {
                    case ELeftRightOpSign::Equal:
                        eType = EType::Boolean;
                        value = (std::any_cast<int16_t>(value) == std::any_cast<int16_t>(right.value));
                        break;
                    case ELeftRightOpSign::NotEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<int16_t>(value) != std::any_cast<int16_t>(right.value));
                        break;
                    case ELeftRightOpSign::Greater:
                        eType = EType::Boolean;
                        value = (std::any_cast<int16_t>(value) > std::any_cast<int16_t>(right.value));
                        break;
                    case ELeftRightOpSign::GreaterOrEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<int16_t>(value) >= std::any_cast<int16_t>(right.value));
                        break;
                    case ELeftRightOpSign::Less:
                        eType = EType::Boolean;
                        value = (std::any_cast<int16_t>(value) < std::any_cast<int16_t>(right.value));
                        break;
                    case ELeftRightOpSign::LessOrEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<int16_t>(value) <= std::any_cast<int16_t>(right.value));
                        break;
                }
                break;
            case EType::UInt16:
                switch (opSign) {
                    case ELeftRightOpSign::Equal:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint16_t>(value) == std::any_cast<uint16_t>(right.value));
                        break;
                    case ELeftRightOpSign::NotEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint16_t>(value) != std::any_cast<uint16_t>(right.value));
                        break;
                    case ELeftRightOpSign::Greater:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint16_t>(value) > std::any_cast<uint16_t>(right.value));
                        break;
                    case ELeftRightOpSign::GreaterOrEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint16_t>(value) >= std::any_cast<uint16_t>(right.value));
                        break;
                    case ELeftRightOpSign::Less:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint16_t>(value) < std::any_cast<uint16_t>(right.value));
                        break;
                    case ELeftRightOpSign::LessOrEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint16_t>(value) <= std::any_cast<uint16_t>(right.value));
                        break;
                }
                break;
            case EType::Int32:
                switch (opSign) {
                    case ELeftRightOpSign::Equal:
                        eType = EType::Boolean;
                        value = (std::any_cast<int32_t>(value) == std::any_cast<int32_t>(right.value));
                        break;
                    case ELeftRightOpSign::NotEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<int32_t>(value) != std::any_cast<int32_t>(right.value));
                        break;
                    case ELeftRightOpSign::Greater:
                        eType = EType::Boolean;
                        value = (std::any_cast<int32_t>(value) > std::any_cast<int32_t>(right.value));
                        break;
                    case ELeftRightOpSign::GreaterOrEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<int32_t>(value) >= std::any_cast<int32_t>(right.value));
                        break;
                    case ELeftRightOpSign::Less:
                        eType = EType::Boolean;
                        value = (std::any_cast<int32_t>(value) < std::any_cast<int32_t>(right.value));
                        break;
                    case ELeftRightOpSign::LessOrEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<int32_t>(value) <= std::any_cast<int32_t>(right.value));
                        break;
                }
                break;
            case EType::UInt32:
                switch (opSign) {
                    case ELeftRightOpSign::Equal:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint32_t>(value) == std::any_cast<uint32_t>(right.value));
                        break;
                    case ELeftRightOpSign::NotEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint32_t>(value) != std::any_cast<uint32_t>(right.value));
                        break;
                    case ELeftRightOpSign::Greater:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint32_t>(value) > std::any_cast<uint32_t>(right.value));
                        break;
                    case ELeftRightOpSign::GreaterOrEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint32_t>(value) >= std::any_cast<uint32_t>(right.value));
                        break;
                    case ELeftRightOpSign::Less:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint32_t>(value) < std::any_cast<uint32_t>(right.value));
                        break;
                    case ELeftRightOpSign::LessOrEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint32_t>(value) <= std::any_cast<uint32_t>(right.value));
                        break;
                }
                break;
            case EType::Int64:
                switch (opSign) {
                    case ELeftRightOpSign::Equal:
                        eType = EType::Boolean;
                        value = (std::any_cast<int64_t>(value) == std::any_cast<int64_t>(right.value));
                        break;
                    case ELeftRightOpSign::NotEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<int64_t>(value) != std::any_cast<int64_t>(right.value));
                        break;
                    case ELeftRightOpSign::Greater:
                        eType = EType::Boolean;
                        value = (std::any_cast<int64_t>(value) > std::any_cast<int64_t>(right.value));
                        break;
                    case ELeftRightOpSign::GreaterOrEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<int64_t>(value) >= std::any_cast<int64_t>(right.value));
                        break;
                    case ELeftRightOpSign::Less:
                        eType = EType::Boolean;
                        value = (std::any_cast<int64_t>(value) < std::any_cast<int64_t>(right.value));
                        break;
                    case ELeftRightOpSign::LessOrEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<int64_t>(value) <= std::any_cast<int64_t>(right.value));
                        break;
                }
                break;
            case EType::UInt64:
                switch (opSign) {
                    case ELeftRightOpSign::Equal:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint64_t>(value) == std::any_cast<uint64_t>(right.value));
                        break;
                    case ELeftRightOpSign::NotEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint64_t>(value) != std::any_cast<uint64_t>(right.value));
                        break;
                    case ELeftRightOpSign::Greater:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint64_t>(value) > std::any_cast<uint64_t>(right.value));
                        break;
                    case ELeftRightOpSign::GreaterOrEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint64_t>(value) >= std::any_cast<uint64_t>(right.value));
                        break;
                    case ELeftRightOpSign::Less:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint64_t>(value) < std::any_cast<uint64_t>(right.value));
                        break;
                    case ELeftRightOpSign::LessOrEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<uint64_t>(value) <= std::any_cast<uint64_t>(right.value));
                        break;
                }
                break;
            case EType::String:
                switch (opSign) {
                    case ELeftRightOpSign::Equal:
                        eType = EType::Boolean;
                        value = (std::any_cast<std::string>(value) == std::any_cast<std::string>(right.value));
                        break;
                    case ELeftRightOpSign::NotEqual:
                        eType = EType::Boolean;
                        value = (std::any_cast<std::string>(value) != std::any_cast<std::string>(right.value));
                        break;
                    default:
                        std::cout << "Error Not Support string < <= > >=sign operator!!" << std::endl;
                        break;
                }
                break;
        }
    }
    
    virtual std::string ToFormatString() override {
        std::string signEn = "";
        std::string str;
        
        switch (eType) {
            case EType::Null:
                str = "null";
                break;
            case EType::String: {
                std::string val = std::any_cast<std::string>(value);
                str = "\"" + val + "\"";
                break;
            }
            case EType::Int16:
                signEn = "s";
                str = std::to_string(std::any_cast<int16_t>(value));
                break;
            case EType::UInt16:
                signEn = "us";
                str = std::to_string(std::any_cast<uint16_t>(value));
                break;
            case EType::Int32:
                signEn = "i";
                str = std::to_string(std::any_cast<int32_t>(value));
                break;
            case EType::UInt32:
                signEn = "ui";
                str = std::to_string(std::any_cast<uint32_t>(value));
                break;
            case EType::Int64:
                signEn = "L";
                str = std::to_string(std::any_cast<int64_t>(value));
                break;
            case EType::UInt64:
                signEn = "uL";
                str = std::to_string(std::any_cast<uint64_t>(value));
                break;
            case EType::Float32:
                signEn = "f";
                str = std::to_string(std::any_cast<float>(value));
                break;
            case EType::Float64:
                signEn = "d";
                str = std::to_string(std::any_cast<double>(value));
                break;
            default:
                str = std::to_string(std::any_cast<int>(value));
                break;
        }
        return str + signEn;
    }
    
    virtual std::string ToTokenString() override {
        std::string result;
        if (m_FileMetaConstValueTerm != nullptr) {
            result = m_FileMetaConstValueTerm->ToTokenString();
        }
        return result;
    }
};

} // namespace Core
} // namespace SimpleLanguage
