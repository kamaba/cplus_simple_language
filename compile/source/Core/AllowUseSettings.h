//****************************************************************************
//  File:      AllowUseSettings.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include <string>

namespace SimpleLanguage {
namespace Core {

// 解析来源枚举
enum class EParseFrom {
    None,
    MemberVariableExpress,
    InputParamExpress,
    StatementLeftExpress,
    StatementRightExpress
};

// 允许使用设置类
class AllowUseSettings {
private:
    bool m_UseNotStatic = false;
    bool m_UseNotConst = true;
    bool m_CallFunction = true;
    bool m_CallConstructFunction = true;
    bool m_SetterFunction = false;
    bool m_GetterFunction = true;
    EParseFrom m_ParseFrom = EParseFrom::None;

public:
    // 构造函数
    AllowUseSettings();
    AllowUseSettings(const AllowUseSettings& clone);
    
    // 析构函数
    ~AllowUseSettings() = default;
    
    // 赋值操作符
    AllowUseSettings& operator=(const AllowUseSettings& other);
    
    // Getter方法
    bool GetUseNotStatic() const { return m_UseNotStatic; }
    bool GetUseNotConst() const { return m_UseNotConst; }
    bool GetCallFunction() const { return m_CallFunction; }
    bool GetCallConstructFunction() const { return m_CallConstructFunction; }
    bool GetSetterFunction() const { return m_SetterFunction; }
    bool GetGetterFunction() const { return m_GetterFunction; }
    EParseFrom GetParseFrom() const { return m_ParseFrom; }
    
    // Setter方法
    void SetUseNotStatic(bool value) { m_UseNotStatic = value; }
    void SetUseNotConst(bool value) { m_UseNotConst = value; }
    void SetCallFunction(bool value) { m_CallFunction = value; }
    void SetCallConstructFunction(bool value) { m_CallConstructFunction = value; }
    void SetSetterFunction(bool value) { m_SetterFunction = value; }
    void SetGetterFunction(bool value) { m_GetterFunction = value; }
    void SetParseFrom(EParseFrom value) { m_ParseFrom = value; }
};

} // namespace Core
} // namespace SimpleLanguage
