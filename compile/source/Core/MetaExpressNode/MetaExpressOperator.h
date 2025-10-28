//****************************************************************************
//  File:      MetaExpressOperator.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "MetaExpressBase.h"
#include "../../Define.h"
#include <string>

namespace SimpleLanguage {
    namespace Compile {
        class Token;
        class FileMetaSymbolTerm;
    }
namespace Core {

    // 前向声明
    class MetaExpressNode;
    class MetaType;
    class AllowUseSettings;
    class ExpressOptimizeConfig;

class ConvertType {
public:
    EType oriType;
    EType targetType;
};

class MetaUnaryOpExpressNode : public MetaExpressNode {
private:
    ESingleOpSign m_OpSign = ESingleOpSign::None;
    MetaExpressNode* m_Value = nullptr;
    Compile::Token* m_TokeType = nullptr;

public:
    Compile::Token* GetTokeType() const;
    ESingleOpSign GetOpSign() const;
    MetaExpressNode* GetValue() const;
    
    MetaUnaryOpExpressNode(Compile::FileMetaSymbolTerm* fme, MetaExpressNode* _value);
    
    void SetValue(MetaExpressNode* _value);
    
    virtual void Parse(AllowUseSettings* auc) override;
    virtual int CalcParseLevel(int level) override;
    virtual void CalcReturnType() override;
    
    MetaExpressNode* SimulateCompute();
    
    virtual std::string ToFormatString() override;
    virtual std::string ToTokenString() override;
};

class MetaOpExpressNode : public MetaExpressNode {
private:
    MetaExpressNode* m_Left = nullptr;
    MetaExpressNode* m_Right = nullptr;
    ConvertType* m_LeftConvert = nullptr;
    ConvertType* m_RightConvert = nullptr;
    ELeftRightOpSign m_OpLevelSign;
    Compile::Token* m_SignToken = nullptr;
    Compile::FileMetaSymbolTerm* m_FileMetaBaseTerm = nullptr;

public:
    bool isEqualType = false;
    MetaExpressNode* GetLeft() const;
    MetaExpressNode* GetRight() const;
    ELeftRightOpSign GetOpSign() const;
    ConvertType* GetLeftConvert() const;
    ConvertType* GetRightConvert() const;
    
    MetaOpExpressNode(Compile::FileMetaSymbolTerm* fme, MetaType* mt, MetaExpressNode* _left, MetaExpressNode* _right);
    MetaOpExpressNode(MetaExpressNode* _left, MetaExpressNode* _right, ELeftRightOpSign _opSign);
    
    void ComputeIsComputeType();
    void SetLeft(MetaExpressNode* _left);
    void SetRight(MetaExpressNode* _right);
    
    virtual void Parse(AllowUseSettings* auc) override;
    virtual int CalcParseLevel(int level) override;
    virtual void CalcReturnType() override;
    
    void ParseCompute();
    void ParseAddOrMinus();
    void ParseDivide();
    void ParseMultiplyOrModulo();
    
    MetaExpressNode* SimulateCompute(ExpressOptimizeConfig* config);
    
    static std::string GetSignString(ELeftRightOpSign opSign);
    
    virtual std::string ToFormatString() override;
    virtual std::string ToTokenString() override;
};

} // namespace Core
} // namespace SimpleLanguage