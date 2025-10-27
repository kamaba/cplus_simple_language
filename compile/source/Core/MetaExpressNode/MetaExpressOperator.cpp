//****************************************************************************
//  File:      MetaExpressOperator.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "MetaExpressOperator.h"
#include "../MetaType.h"
#include "../ExpressManager.h"
#include "../BaseMetaClass/CoreMetaClassManager.h"
#include "../../Compile/FileMeta/FileMetaSyntax.h"
#include "../../Compile/FileMeta/FileMetaExpress.h"
#include "../../Compile/Token.h"
#include "../MetaExpressNode/MetaExpressConst.h"
#include "../MetaExpressNode/MetaExpressBase.h"
#include "../AllowUseSettings.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace SimpleLanguage::Compile;

namespace SimpleLanguage {
namespace Core {

// MetaUnaryOpExpressNode implementation
Compile::Token* MetaUnaryOpExpressNode::GetTokeType() const {
    return m_TokeType;
}

ESingleOpSign MetaUnaryOpExpressNode::GetOpSign() const {
    return m_OpSign;
}

MetaExpressNode* MetaUnaryOpExpressNode::GetValue() const {
    return m_Value;
}

MetaUnaryOpExpressNode::MetaUnaryOpExpressNode(Compile::FileMetaSymbolTerm* fme, MetaExpressNode* _value) {
    m_Value = _value;
    m_TokeType = fme->GetToken();
    if (fme->GetSymbolType() == ETokenType::Minus) {
        m_OpSign = ESingleOpSign::Neg;
    } else if (fme->GetSymbolType() == ETokenType::Not) {
        m_OpSign = ESingleOpSign::Not;
    }
}

void MetaUnaryOpExpressNode::SetValue(MetaExpressNode* _value) {
    m_Value = _value;
}

void MetaUnaryOpExpressNode::Parse(AllowUseSettings* auc) {
    m_Value->Parse(auc);
}

int MetaUnaryOpExpressNode::CalcParseLevel(int level) {
    return m_Value->CalcParseLevel(level);
}

void MetaUnaryOpExpressNode::CalcReturnType() {
    if (m_OpSign == ESingleOpSign::Not) {
        m_MetaDefineType->SetMetaClass(CoreMetaClassManager::GetInstance().GetBooleanMetaClass());
    } else {
        m_Value->CalcReturnType();
        m_MetaDefineType = m_Value->GetMetaDefineType();
    }
}

MetaExpressNode* MetaUnaryOpExpressNode::SimulateCompute() {
    auto mcen = dynamic_cast<MetaConstExpressNode*>(m_Value);
    if (mcen != nullptr) {
        auto eType = mcen->GetEType();
        switch (m_OpSign) {
            case ESingleOpSign::Neg: {
                switch (eType) {
                    case EType::Int16: {
                       // mcen->SetValue(-mcen->GetValue().ToShort());
                        return mcen;
                    }
                    case EType::UInt16: {
                      //  mcen->SetValue(-mcen->GetValue().ToUShort());
                        return mcen;
                    }
                    case EType::Int32: {
                     //   mcen->SetValue(-mcen->GetValue().ToInt());
                        return mcen;
                    }
                    case EType::UInt32: {
                     //   mcen->SetValue(-mcen->GetValue().ToUInt());
                        return mcen;
                    }
                    case EType::Int64: {
                     //   mcen->SetValue(-mcen->GetValue().ToLong());
                        return mcen;
                    }
                }
                break;
            }
            case ESingleOpSign::Not: {
                switch (eType) {
                    case EType::Byte: {
                     //   mcen->SetValue(mcen->GetValue().ToByte() != 0);
                        return mcen;
                    }
                    case EType::Int16: {
                     //   mcen->SetValue(mcen->GetValue().ToShort() != 0);
                        return mcen;
                    }
                    case EType::UInt16: {
                    //    mcen->SetValue(mcen->GetValue().ToUShort() != 0);
                        return mcen;
                    }
                    case EType::Int32: {
                    //    mcen->SetValue(mcen->GetValue().ToInt() != 0);
                        return mcen;
                    }
                    case EType::UInt32: {
                    //    mcen->SetValue(mcen->GetValue().ToUInt() != 0);
                        return mcen;
                    }
                    case EType::Int64: {
                     //   mcen->SetValue(mcen->GetValue().ToLong() != 0);
                        return mcen;
                    }
                    case EType::UInt64: {
                    //    mcen->SetValue(mcen->GetValue().ToULong() != 0);
                        return mcen;
                    }
                    case EType::String: {
                        std::string str = mcen->GetValue().ToString();
                    //    mcen->SetValue(str.empty());
                        return mcen;
                    }
                }
                break;
            }
            case ESingleOpSign::Xor: {
                switch (eType) {
                    case EType::Byte: {
                     //   mcen->SetValue(~mcen->GetValue().data.byte_val );
                        return mcen;
                    }
                    case EType::SByte: {
                    //    mcen->SetValue(~mcen->GetValue().ToSByte());
                        return mcen;
                    }
                    case EType::Int16: {
                     //   mcen->SetValue(~mcen->GetValue().ToShort());
                        return mcen;
                    }
                    case EType::UInt16: {
                    //    mcen->SetValue(~mcen->GetValue().ToUShort());
                        return mcen;
                    }
                    case EType::Int32: {
                    //    mcen->SetValue(~mcen->GetValue().ToInt());
                        return mcen;
                    }
                }
                break;
            }
        }
    }
    return this;
}

std::string MetaUnaryOpExpressNode::ToFormatString() {
    std::string result = m_TokeType->GetLexeme().ToString();
    result += m_Value->ToFormatString();
    return result;
}

std::string MetaUnaryOpExpressNode::ToTokenString() {
    std::string result;
    if (m_TokeType != nullptr) {
        result = m_TokeType->ToLexemeAllString();
    }
    if (m_Value != nullptr) {
        result += m_Value->ToTokenString();
    }
    return result;
}

// MetaOpExpressNode implementation
MetaExpressNode* MetaOpExpressNode::GetLeft() const {
    return m_Left;
}

MetaExpressNode* MetaOpExpressNode::GetRight() const {
    return m_Right;
}

ELeftRightOpSign MetaOpExpressNode::GetOpSign() const {
    return m_OpLevelSign;
}

ConvertType* MetaOpExpressNode::GetLeftConvert() const {
    return m_LeftConvert;
}

ConvertType* MetaOpExpressNode::GetRightConvert() const {
    return m_RightConvert;
}

MetaOpExpressNode::MetaOpExpressNode(Compile::FileMetaSymbolTerm* fme, MetaType* mt, MetaExpressNode* _left, MetaExpressNode* _right) {
    m_Left = _left;
    m_Right = _right;
    m_FileMetaBaseTerm = fme;
    m_MetaDefineType = mt;
    
    ETokenType ett = fme->GetToken()->GetType();
    m_SignToken = fme->GetToken();
    switch (ett) {
        case ETokenType::Plus:
            m_OpLevelSign = ELeftRightOpSign::Add;
            break;
        case ETokenType::Minus:
            m_OpLevelSign = ELeftRightOpSign::Minus;
            break;
        case ETokenType::Multiply:
            m_OpLevelSign = ELeftRightOpSign::Multiply;
            break;
        case ETokenType::Divide:
            m_OpLevelSign = ELeftRightOpSign::Divide;
            break;
        case ETokenType::Modulo:
            m_OpLevelSign = ELeftRightOpSign::Modulo;
            break;
        case ETokenType::GreaterOrEqual:
            m_OpLevelSign = ELeftRightOpSign::GreaterOrEqual;
            break;
        case ETokenType::Greater:
            m_OpLevelSign = ELeftRightOpSign::Greater;
            break;
        case ETokenType::LessOrEqual:
            m_OpLevelSign = ELeftRightOpSign::LessOrEqual;
            break;
        case ETokenType::Less:
            m_OpLevelSign = ELeftRightOpSign::Less;
            break;
        case ETokenType::Equal:
            m_OpLevelSign = ELeftRightOpSign::Equal;
            break;
        case ETokenType::NotEqual:
            m_OpLevelSign = ELeftRightOpSign::NotEqual;
            break;
        case ETokenType::And:
            m_OpLevelSign = ELeftRightOpSign::And;
            break;
        case ETokenType::Or:
            m_OpLevelSign = ELeftRightOpSign::Or;
            break;
        default:
            std::cout << "Error 没有合适的符号!!!" << static_cast<int>(ett) << std::endl;
            break;
    }
    ComputeIsComputeType();
}

MetaOpExpressNode::MetaOpExpressNode(MetaExpressNode* _left, MetaExpressNode* _right, ELeftRightOpSign _opSign) {
    m_Left = _left;
    m_Right = _right;
    m_OpLevelSign = _opSign;
    ComputeIsComputeType();
}

void MetaOpExpressNode::ComputeIsComputeType() {
    if (m_OpLevelSign == ELeftRightOpSign::Equal ||
        m_OpLevelSign == ELeftRightOpSign::NotEqual ||
        m_OpLevelSign == ELeftRightOpSign::Greater ||
        m_OpLevelSign == ELeftRightOpSign::GreaterOrEqual ||
        m_OpLevelSign == ELeftRightOpSign::Less ||
        m_OpLevelSign == ELeftRightOpSign::LessOrEqual ||
        m_OpLevelSign == ELeftRightOpSign::And ||
        m_OpLevelSign == ELeftRightOpSign::Or) {
        isEqualType = true;
    } else {
        isEqualType = false;
    }
}

void MetaOpExpressNode::SetLeft(MetaExpressNode* _left) {
    m_Left = _left;
}

void MetaOpExpressNode::SetRight(MetaExpressNode* _right) {
    m_Right = _right;
}

void MetaOpExpressNode::Parse(AllowUseSettings* auc) {
    m_Left->Parse(auc);
    m_Right->Parse(auc);
}

int MetaOpExpressNode::CalcParseLevel(int level) {
    int level1 = m_Left->CalcParseLevel(level);
    int level2 = m_Right->CalcParseLevel(level1);
    return level2;
}

void MetaOpExpressNode::CalcReturnType() {
    if (m_Left != nullptr) {
        m_Left->CalcReturnType();
    }
    if (m_Right != nullptr) {
        m_Right->CalcReturnType();
    }
    ParseCompute();
    GetReturnMetaDefineType();
}

void MetaOpExpressNode::ParseCompute() {
    if (m_Left != nullptr && m_Right != nullptr) {
        switch (m_OpLevelSign) {
            case ELeftRightOpSign::Add:
            case ELeftRightOpSign::Minus: {
                ParseAddOrMinus();
                break;
            }
            case ELeftRightOpSign::Multiply: {
                ParseMultiplyOrModulo();
                break;
            }
            case ELeftRightOpSign::Divide: {
                ParseDivide();
                break;
            }
            case ELeftRightOpSign::Modulo: {
                ParseMultiplyOrModulo();
                break;
            }
            case ELeftRightOpSign::Equal:
            case ELeftRightOpSign::NotEqual:
            case ELeftRightOpSign::Greater:
            case ELeftRightOpSign::GreaterOrEqual:
            case ELeftRightOpSign::Less:
            case ELeftRightOpSign::LessOrEqual:
            case ELeftRightOpSign::Or:
            case ELeftRightOpSign::And: {
                if (m_MetaDefineType != nullptr) {
                    m_MetaDefineType->SetMetaClass(CoreMetaClassManager::GetInstance().GetBooleanMetaClass());
                } else {
                    m_MetaDefineType = new MetaType(CoreMetaClassManager::GetInstance().GetBooleanMetaClass());
                }
                break;
            }
        }
    } else {
        std::cout << "Error 左右值不能为空!!" << std::endl;
    }
}

void MetaOpExpressNode::ParseAddOrMinus() {
    if (m_Left->GetOpLevel() < m_Right->GetOpLevel()) {
        m_MetaDefineType = m_Right->GetMetaDefineType();
        if (m_Right->GetOpLevel() < 10) {
            m_LeftConvert = new ConvertType();
            m_LeftConvert->oriType = m_Left->GetMetaDefineType()->GetMetaClass()->GetEType();
            m_LeftConvert->targetType = m_MetaDefineType->GetMetaClass()->GetEType();
        }
    } else if (m_Left->GetOpLevel() > m_Right->GetOpLevel()) {
        m_MetaDefineType = m_Left->GetMetaDefineType();
        if (m_Left->GetOpLevel() < 10) {
            m_RightConvert = new ConvertType();
            m_RightConvert->oriType = m_Right->GetMetaDefineType()->GetMetaClass()->GetEType();
            m_RightConvert->targetType = m_MetaDefineType->GetMetaClass()->GetEType();
        }
    } else {
        m_MetaDefineType = m_Left->GetMetaDefineType();
    }
}

void MetaOpExpressNode::ParseDivide() {
    if (m_Left->GetOpLevel() < m_Right->GetOpLevel()) {
        m_MetaDefineType = m_Right->GetMetaDefineType();
    } else if (m_Left->GetOpLevel() > m_Right->GetOpLevel()) {
        m_MetaDefineType = m_Left->GetMetaDefineType();
    } else {
        m_MetaDefineType = m_Left->GetMetaDefineType();
    }
}

void MetaOpExpressNode::ParseMultiplyOrModulo() {
    if (m_Left->GetOpLevel() < m_Right->GetOpLevel()) {
        m_MetaDefineType = m_Left->GetMetaDefineType();
    } else if (m_Left->GetOpLevel() > m_Right->GetOpLevel()) {
        m_MetaDefineType = m_Left->GetMetaDefineType();
    } else {
        m_MetaDefineType = m_Left->GetMetaDefineType();
    }
}

MetaExpressNode* MetaOpExpressNode::SimulateCompute(ExpressOptimizeConfig* config) {
    if (config->greaterOrEqualConvertGeraterAndEqual && m_OpLevelSign == ELeftRightOpSign::GreaterOrEqual) {
        auto constLeft = dynamic_cast<MetaConstExpressNode*>(m_Left);
        auto constRight = dynamic_cast<MetaConstExpressNode*>(m_Right);
        if (constLeft == nullptr || constRight == nullptr) {
            MetaExpressNode* left1 = m_Left;
            MetaExpressNode* right1 = m_Right;
            MetaExpressNode* left2 = m_Left;
            MetaExpressNode* right2 = m_Right;
            m_Left = new MetaOpExpressNode(left1, right1, ELeftRightOpSign::Greater);
            m_Right = new MetaOpExpressNode(left2, right2, ELeftRightOpSign::Equal);
            m_OpLevelSign = ELeftRightOpSign::Or;
        }
    }
    if (config->lessOrEqualConvertLessAndEqual && m_OpLevelSign == ELeftRightOpSign::LessOrEqual) {
        auto constLeft = dynamic_cast<MetaConstExpressNode*>(m_Left);
        auto constRight = dynamic_cast<MetaConstExpressNode*>(m_Right);
        if (constLeft == nullptr || constRight == nullptr) {
            MetaExpressNode* left1 = m_Left;
            MetaExpressNode* right1 = m_Right;
            MetaExpressNode* left2 = m_Left;
            MetaExpressNode* right2 = m_Right;
            m_Left = new MetaOpExpressNode(left1, right1, ELeftRightOpSign::Less);
            m_Right = new MetaOpExpressNode(left2, right2, ELeftRightOpSign::Equal);
            m_OpLevelSign = ELeftRightOpSign::Or;
        }
    }
    if (config->ifLeftAndRightIsConstThenCompute ) {
        auto constLeft = dynamic_cast<MetaConstExpressNode*>(m_Left);
        auto constRight = dynamic_cast<MetaConstExpressNode*>(m_Right);
        if (constLeft != nullptr && constRight != nullptr) {
            switch (m_OpLevelSign) {
                case ELeftRightOpSign::Add: {
                    *constLeft = *constLeft + *constRight;
                    return constLeft;
                }
                case ELeftRightOpSign::Minus: {
                    *constLeft = *constLeft - *constRight;
                    return constLeft;
                }
                case ELeftRightOpSign::Multiply: {
                    *constLeft = *constLeft * *constRight;
                    return constLeft;
                }
                case ELeftRightOpSign::Divide: {
                    *constLeft = *constLeft / *constRight;
                    return constLeft;
                }
                case ELeftRightOpSign::Modulo: {
                    *constLeft = *constLeft % *constRight;
                    return constLeft;
                }
                case ELeftRightOpSign::Equal:
                case ELeftRightOpSign::NotEqual:
                case ELeftRightOpSign::Greater:
                case ELeftRightOpSign::GreaterOrEqual:
                case ELeftRightOpSign::Less:
                case ELeftRightOpSign::LessOrEqual:
                case ELeftRightOpSign::And:
                case ELeftRightOpSign::Or: {
                    constLeft->ComputeEqualComputeRight(*constRight, m_OpLevelSign);
                    return constLeft;
                }
            }
        }
    }
    return this;
}

std::string MetaOpExpressNode::GetSignString(ELeftRightOpSign opSign) {
    switch (opSign) {
        case ELeftRightOpSign::Add: return "+";
        case ELeftRightOpSign::Minus: return "-";
        case ELeftRightOpSign::Multiply: return "*";
        case ELeftRightOpSign::Divide: return "/";
        case ELeftRightOpSign::Modulo: return "%";
        case ELeftRightOpSign::Shi: return ">>";
        case ELeftRightOpSign::Shr: return "<<";
        case ELeftRightOpSign::Equal: return "==";
        case ELeftRightOpSign::NotEqual: return "!=";
        case ELeftRightOpSign::Greater: return ">";
        case ELeftRightOpSign::GreaterOrEqual: return ">=";
        case ELeftRightOpSign::Less: return "<";
        case ELeftRightOpSign::LessOrEqual: return "<=";
        case ELeftRightOpSign::Or: return "||";
        case ELeftRightOpSign::And: return "&&";
    }
    return "NoSign";
}

std::string MetaOpExpressNode::ToFormatString() {
    std::string result = "(" + m_Left->ToFormatString();
    result += " " + GetSignString(m_OpLevelSign);
    result += " " + m_Right->ToFormatString() + ")";
    return result;
}

std::string MetaOpExpressNode::ToTokenString() {
    std::string result = m_Left ? m_Left->ToTokenString() : "";
    if (m_SignToken != nullptr) {
        result += m_SignToken->GetLexeme().ToString();
    } else {
        result += GetSignString(m_OpLevelSign);
    }
    result += m_Right ? m_Right->ToTokenString() : "";
    return result;
}

} // namespace Core
} // namespace SimpleLanguage