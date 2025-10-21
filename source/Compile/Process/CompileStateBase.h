//****************************************************************************
//  File:      CompileStateBase.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include <chrono>
#include <ctime>

namespace SimpleLanguage {
namespace Compile {

enum class ELevelInfo {
    None = 0,
    Info,
    Warning,
    Error
};

class CompileStateBase {
public:
    bool IsInterrupt() const { return m_IsInterrupt; }
    
    std::chrono::system_clock::time_point GetDateTime() const { return dateTime; }
    void SetDateTime(const std::chrono::system_clock::time_point& dt) { dateTime = dt; }

protected:
    bool m_IsInterrupt = false;
    std::chrono::system_clock::time_point dateTime = std::chrono::system_clock::now();
};

} // namespace Compile
} // namespace SimpleLanguage
