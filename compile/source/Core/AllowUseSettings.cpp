//****************************************************************************
//  File:      AllowUseSettings.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#include "AllowUseSettings.h"
#include <iostream>

namespace SimpleLanguage {
namespace Core {

// 默认构造函数
AllowUseSettings::AllowUseSettings() {
    // 使用成员初始化列表的默认值
}

// 拷贝构造函数
AllowUseSettings::AllowUseSettings(const AllowUseSettings& clone) {
    m_UseNotStatic = clone.m_UseNotStatic;
    m_UseNotConst = clone.m_UseNotConst;
    m_CallFunction = clone.m_CallFunction;
    m_CallConstructFunction = clone.m_CallConstructFunction;
    m_SetterFunction = clone.m_SetterFunction;
    m_GetterFunction = clone.m_GetterFunction;
    m_ParseFrom = clone.m_ParseFrom;
}

// 赋值操作符
AllowUseSettings& AllowUseSettings::operator=(const AllowUseSettings& other) {
    if (this != &other) {
        m_UseNotStatic = other.m_UseNotStatic;
        m_UseNotConst = other.m_UseNotConst;
        m_CallFunction = other.m_CallFunction;
        m_CallConstructFunction = other.m_CallConstructFunction;
        m_SetterFunction = other.m_SetterFunction;
        m_GetterFunction = other.m_GetterFunction;
        m_ParseFrom = other.m_ParseFrom;
    }
    return *this;
}

} // namespace Core
} // namespace SimpleLanguage
