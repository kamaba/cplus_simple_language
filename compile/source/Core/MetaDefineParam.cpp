//****************************************************************************
//  File:      MetaDefineParam.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta define param class
//****************************************************************************

#include "MetaDefineParam.h"
#include "MetaMemberFunction.h"
#include "MetaType.h"
#include "MetaVariable.h"

namespace SimpleLanguage {
namespace Core {

MetaDefineParam::MetaDefineParam() : MetaBase() {
    m_OwnerFunction = nullptr;
    m_MetaDefineType = nullptr;
    m_MetaVariable = nullptr;
}

MetaDefineParam::MetaDefineParam(MetaMemberFunction* ownerFunction, void* param) : MetaBase() {
    m_OwnerFunction = ownerFunction;
    m_MetaDefineType = nullptr;
    m_MetaVariable = nullptr;
    // 这里需要根据实际的 param 类型来处理
}
bool MetaDefineParam::IsExtendParams() const { return m_FileMetaParamter != nullptr && m_FileMetaParamter->GetParamsToken() != nullptr; }

void MetaDefineParam::ParseMetaDefineType() {
    // 解析元定义类型的逻辑
}

void MetaDefineParam::CreateExpress() {
    // 创建表达式的逻辑
}

void MetaDefineParam::Parse() {
    // 解析的逻辑
}

void MetaDefineParam::CalcReturnType() {
    // 计算返回类型的逻辑
}

} // namespace Core
} // namespace SimpleLanguage
