//****************************************************************************
//  File:      CompileManager.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "CompileManager.h"
#include <string>

namespace SimpleLanguage {
namespace Compile {

    CompileManager* CompileManager::s_Instance = nullptr;

    CompileManager& CompileManager::GetInstance()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = new CompileManager();
        }
        return *s_Instance;
    }

    void CompileManager::AddCompileError(const std::string& str)
    {
        // ʵ�ֱ��������
    }

    void CompileManager::AddProjectCompileState(int state, int info, int error, const std::string& str)
    {
        // ʵ����Ŀ����״̬����
    }

} // namespace Compile
} // namespace SimpleLanguage
