//****************************************************************************
//  File:      MetaExpressOperator.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "MetaExpressBase.h"
#include "../MetaType.h"
#include "../BaseMetaClass/CoreMetaClassManager.h"
#include "../Compile/CoreFileMeta/FileMetaSymbolTerm.h"
#include "../Compile/Token.h"
#include "../VM/ExpressOptimizeConfig.h"
#include <string>
#include <iostream>

namespace SimpleLanguage {
namespace Core {

class ConvertType {
public:
    EType oriType;
    EType targetType;
};

class MetaUnaryOpExpressNode : public MetaExpressNode {
private:
    ESingleOpSign m_OpSign = ESingleOpSign::None;
    MetaExpressNode* m_Value = nullptr;
    Token* m_TokeType = nullptr;

public:
    Token* GetTokeType() const { return m_TokeType; }
    ESingleOpSign GetOpSign() const { return m_OpSign; }
    MetaExpressNode* GetValue() const { return m_Value; }
    
    MetaUnaryOpExpressNode(FileMetaSymbolTerm* fme, MetaExpressNode* _value) {
        m_Value = _value;
        m_TokeType = fme->GetToken();
        if (fme->GetSymbolType() == ETokenType::Minus) {
            m_OpSign = ESingleOpSign::Neg;
        } else if (fme->GetSymbolType() == ETokenType::Not) {
            m_OpSign = ESingleOpSign::Not;
        }
    }
    
    void SetValue(MetaExpressNode* _value) {
        m_Value = _value;
    }
    
    virtual void Parse(AllowUseSettings* auc) override {
        m_Value->Parse(auc);
    }
    
    virtual int CalcParseLevel(int level) override {
        return m_Value->CalcParseLevel(level);
    }
    
    virtual void CalcReturnType() override {
        if (m_OpSign == ESingleOpSign::Not) {
            m_MetaDefineType->SetMetaClass(CoreMetaClassManager::GetInstance().GetBooleanMetaClass());
        } else {
            m_Value->CalcReturnType();
            m_MetaDefineType = m_Value->GetMetaDefineType();
        }
    }
    
    MetaExpressNode* SimulateCompute() {
        auto mcen = dynamic_cast<MetaConstExpressNode*>(m_Value);
        if (mcen != nullptr) {
            auto eType = mcen->GetEType();
            switch (m_OpSign) {
                case ESingleOpSign::Neg: {
                    switch (eType) {
                        case EType::Int16: {
                            mcen->SetValue(-std::any_cast<int16_t>(mcen->GetValue()));
                            return mcen;
                        }
                        case EType::UInt16: {
                            mcen->SetValue(-std::any_cast<uint16_t>(mcen->GetValue()));
                            return mcen;
                        }
                        case EType::Int32: {
                            mcen->SetValue(-std::any_cast<int32_t>(mcen->GetValue()));
                            return mcen;
                        }
                        case EType::UInt32: {
                            mcen->SetValue(-std::any_cast<uint32_t>(mcen->GetValue()));
                            return mcen;
                        }
                        case EType::Int64: {
                            mcen->SetValue(-std::any_cast<int64_t>(mcen->GetValue()));
                            return mcen;
                        }
                    }
                    break;
                }
                case ESingleOpSign::Not: {
                    switch (eType) {
                        case EType::Byte: {
                            mcen->SetValue(std::any_cast<uint8_t>(mcen->GetValue()) != 0);
                            return mcen;
                        }
                        case EType::Int16: {
                            mcen->SetValue(std::any_cast<int16_t>(mcen->GetValue()) != 0);
                            return mcen;
                        }
                        case EType::UInt16: {
                            mcen->SetValue(std::any_cast<uint16_t>(mcen->GetValue()) != 0);
                            return mcen;
                        }
                        case EType::Int32: {
                            mcen->SetValue(std::any_cast<int32_t>(mcen->GetValue()) != 0);
                            return mcen;
                        }
                        case EType::UInt32: {
                            mcen->SetValue(std::any_cast<uint32_t>(mcen->GetValue()) != 0);
                            return mcen;
                        }
                        case EType::Int64: {
                            mcen->SetValue(std::any_cast<int64_t>(mcen->GetValue()) != 0);
                            return mcen;
                        }
                        case EType::UInt64: {
                            mcen->SetValue(std::any_cast<uint64_t>(mcen->GetValue()) != 0);
                            return mcen;
                        }
                        case EType::String: {
                            std::string str = std::any_cast<std::string>(mcen->GetValue());
                            mcen->SetValue(str.empty());
                            return mcen;
                        }
                    }
                    break;
                }
                case ESingleOpSign::Xor: {
                    switch (eType) {
                        case EType::Byte: {
                            mcen->SetValue(~std::any_cast<uint8_t>(mcen->GetValue()));
                            return mcen;
                        }
                        case EType::SByte: {
                            mcen->SetValue(~std::any_cast<int8_t>(mcen->GetValue()));
                            return mcen;
                        }
                        case EType::Int16: {
                            mcen->SetValue(~std::any_cast<int16_t>(mcen->GetValue()));
                            return mcen;
                        }
                        case EType::UInt16: {
                            mcen->SetValue(~std::any_cast<uint16_t>(mcen->GetValue()));
                            return mcen;
                        }
                        case EType::Int32: {
                            mcen->SetValue(~std::any_cast<int32_t>(mcen->GetValue()));
                            return mcen;
                        }
                    }
                    break;
                }
            }
        }
        return this;
    }
    
    virtual std::string ToFormatString() override {
        std::string result = m_TokeType->GetLexeme().ToString();
        result += m_Value->ToFormatString();
        return result;
    }
    
    virtual std::string ToTokenString() override {
        std::string result;
        if (m_TokeType != nullptr) {
            result = m_TokeType->ToLexemeAllString();
        }
        if (m_Value != nullptr) {
            result += m_Value->ToTokenString();
        }
        return result;
    }
};

class MetaOpExpressNode : public MetaExpressNode {
private:
    MetaExpressNode* m_Left = nullptr;
    MetaExpressNode* m_Right = nullptr;
    ConvertType* m_LeftConvert = nullptr;
    ConvertType* m_RightConvert = nullptr;
    ELeftRightOpSign m_OpLevelSign;
    Token* m_SignToken = nullptr;
    FileMetaSymbolTerm* m_FileMetaBaseTerm = nullptr;

public:
    bool isEqualType = false;
    MetaExpressNode* GetLeft() const { return m_Left; }
    MetaExpressNode* GetRight() const { return m_Right; }
    ELeftRightOpSign GetOpSign() const { return m_OpLevelSign; }
    ConvertType* GetLeftConvert() const { return m_LeftConvert; }
    ConvertType* GetRightConvert() const { return m_RightConvert; }
    
    MetaOpExpressNode(FileMetaSymbolTerm* fme, MetaType* mt, MetaExpressNode* _left, MetaExpressNode* _right) {
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
                std::cout << "Error 没有适合的符号!!!" << static_cast<int>(ett) << std::endl;
                break;
        }
        ComputeIsComputeType();
    }
    
    MetaOpExpressNode(MetaExpressNode* _left, MetaExpressNode* _right, ELeftRightOpSign _opSign) {
        m_Left = _left;
        m_Right = _right;
        m_OpLevelSign = _opSign;
        ComputeIsComputeType();
    }
    
    void ComputeIsComputeType() {
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
    
    void SetLeft(MetaExpressNode* _left) {
        m_Left = _left;
    }
    
    void SetRight(MetaExpressNode* _right) {
        m_Right = _right;
    }
    
    virtual void Parse(AllowUseSettings* auc) override {
        m_Left->Parse(auc);
        m_Right->Parse(auc);
    }
    
    virtual int CalcParseLevel(int level) override {
        int level1 = m_Left->CalcParseLevel(level);
        int level2 = m_Right->CalcParseLevel(level1);
        return level2;
    }
    
    virtual void CalcReturnType() override {
        if (m_Left != nullptr) {
            m_Left->CalcReturnType();
        }
        if (m_Right != nullptr) {
            m_Right->CalcReturnType();
        }
        ParseCompute();
        GetReturnMetaDefineType();
    }
    
    void ParseCompute() {
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
            std::cout << "Error 错误，左右值不对!!" << std::endl;
        }
    }
    
    void ParseAddOrMinus() {
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
    
    void ParseDivide() {
        if (m_Left->GetOpLevel() < m_Right->GetOpLevel()) {
            m_MetaDefineType = m_Right->GetMetaDefineType();
        } else if (m_Left->GetOpLevel() > m_Right->GetOpLevel()) {
            m_MetaDefineType = m_Left->GetMetaDefineType();
        } else {
            m_MetaDefineType = m_Left->GetMetaDefineType();
        }
    }
    
    void ParseMultiplyOrModulo() {
        if (m_Left->GetOpLevel() < m_Right->GetOpLevel()) {
            m_MetaDefineType = m_Left->GetMetaDefineType();
        } else if (m_Left->GetOpLevel() > m_Right->GetOpLevel()) {
            m_MetaDefineType = m_Left->GetMetaDefineType();
        } else {
            m_MetaDefineType = m_Left->GetMetaDefineType();
        }
    }
    
    MetaExpressNode* SimulateCompute(ExpressOptimizeConfig* config) {
        if (config->GetGreaterOrEqualConvertGeraterAndEqual() && m_OpLevelSign == ELeftRightOpSign::GreaterOrEqual) {
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
        if (config->GetLessOrEqualConvertLessAndEqual() && m_OpLevelSign == ELeftRightOpSign::LessOrEqual) {
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
        if (config->GetIfLeftAndRightIsConstThenCompute()) {
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
    
    static std::string GetSignString(ELeftRightOpSign opSign) {
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
    
    virtual std::string ToFormatString() override {
        std::string result = "(" + m_Left->ToFormatString();
        result += " " + GetSignString(m_OpLevelSign);
        result += " " + m_Right->ToFormatString() + ")";
        return result;
    }
    
    virtual std::string ToTokenString() override {
        std::string result = m_Left ? m_Left->ToTokenString() : "";
        if (m_SignToken != nullptr) {
            result += m_SignToken->GetLexeme().ToString();
        } else {
            result += GetSignString(m_OpLevelSign);
        }
        result += m_Right ? m_Right->ToTokenString() : "";
        return result;
    }
};

} // namespace Core
} // namespace SimpleLanguage
