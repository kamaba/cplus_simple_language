//****************************************************************************
//  File:      CompileManager.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "CompileManager.h"

namespace SimpleLanguage {
namespace Compile {

    CompileManager* CompileManager::s_Instance = nullptr;

    CompileManager& CompileManager::getInstance()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = new CompileManager();
        }
        return *s_Instance;
    }

    void CompileManager::addCompileError(const std::string& str)
    {
        // 实现编译错误处理
    }

    void CompileManager::addProjectCompileState(int state, int info, int error, const std::string& str)
    {
        // 实现项目编译状态处理
    }

} // namespace Compile
} // namespace SimpleLanguage
