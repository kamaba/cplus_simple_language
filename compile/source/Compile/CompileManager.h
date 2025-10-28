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
#include <string>
#include "Process/CompileStateBase.h"

namespace SimpleLanguage {
namespace Compile {

    // ǰ������
    class CompileStateBase;

    class CompileManager
    {
    public:
        static bool IsInterrupt() { return false; }
        
        static CompileManager& GetInstance();
        
        void AddCompileError(const std::string& str);
        void AddProjectCompileState(int state, int info, int error, const std::string& str);

    private:
        CompileManager() = default;
        ~CompileManager() = default;
        CompileManager(const CompileManager&) = delete;
        CompileManager& operator=(const CompileManager&) = delete;

        static CompileManager* s_Instance;
        std::vector<CompileStateBase> m_CompileStateList;
    };

} // namespace Compile
} // namespace SimpleLanguage
