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
    // ������Ҫ����ʵ�ʵ� param ����������
}
bool MetaDefineParam::IsExtendParams() const { return m_FileMetaParamter != nullptr && m_FileMetaParamter->GetParamsToken() != nullptr; }

void MetaDefineParam::ParseMetaDefineType() {
    // ����Ԫ�������͵��߼�
}

void MetaDefineParam::CreateExpress() {
    // �������ʽ���߼�
}

void MetaDefineParam::Parse() {
    // �������߼�
}

void MetaDefineParam::CalcReturnType() {
    // ���㷵�����͵��߼�
}

} // namespace Core
} // namespace SimpleLanguage
