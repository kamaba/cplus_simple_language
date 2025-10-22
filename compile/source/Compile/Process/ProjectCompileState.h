//****************************************************************************
//  File:      ProjectCompileState.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "CompileStateBase.h"

namespace SimpleLanguage {
namespace Compile {

class ProjectCompileState : public CompileStateBase {
public:
    enum class ELoadState {
        None,
        LoadProjectFile,
        LexerParse,
        TokenParse,
        StructParse,
        ProjectParse,
        CompileClassParse,
        CompileBeforeExec,
        CoreMetaClassInit,
        CompileConfigFile,
        CompileAfterExec,
        RuntimeExec
    };

    enum class EError {
        None,
        ParseError,
        CompileError,
        RuntimeError
    };

    ProjectCompileState() = default;
    virtual ~ProjectCompileState() = default;
    
    bool IsInterrupt() const { return m_IsInterrupt; }
    ELoadState GetLoadState() const { return m_LoadState; }
    void SetLoadState(ELoadState loadState) { m_LoadState = loadState; }
    
    EError GetError() const { return m_Error; }
    void SetError(EError error) { m_Error = error; }

private:
    ELoadState m_LoadState = ELoadState::None;
    EError m_Error = EError::None;
};

} // namespace Compile
} // namespace SimpleLanguage
