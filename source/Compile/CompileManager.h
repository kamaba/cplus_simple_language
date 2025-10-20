//****************************************************************************
//  File:      CompileManager.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include <vector>
#include <memory>

namespace SimpleLanguage {
namespace Compile {

    // 前向声明
    class CompileStateBase;

    class CompileManager
    {
    public:
        static bool isInterrupt() { return false; }
        
        static CompileManager& getInstance();
        
        void addCompileError(const std::string& str);
        void addProjectCompileState(int state, int info, int error, const std::string& str);

    private:
        CompileManager() = default;
        ~CompileManager() = default;
        CompileManager(const CompileManager&) = delete;
        CompileManager& operator=(const CompileManager&) = delete;

        static CompileManager* s_Instance;
        std::vector<std::unique_ptr<CompileStateBase>> m_CompileStateList;
    };

} // namespace Compile
} // namespace SimpleLanguage
