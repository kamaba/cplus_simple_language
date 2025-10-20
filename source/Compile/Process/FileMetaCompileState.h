//****************************************************************************
//  File:      FileMetaCompileState.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "CompileStateBase.h"

namespace SimpleLanguage {
namespace Compile {
namespace Process {

class FileMetaCompileState : public CompileStateBase {
public:
    enum class ELoadState {
        None,
        LoadStart,
        Loading,
        LoadEnd,
        LexerParse,
        TokenParse,
        StructParseBegin,
        StructParseImport,
        StructParseClass,
        StructParseData,
        StructParseEnum,
        StructParseEnd
    };

    FileMetaCompileState() = default;
    virtual ~FileMetaCompileState() = default;
    
    bool IsInterrupt() const { return m_IsInterrupt; }
    ELoadState GetLoadState() const { return m_LoadState; }
    void SetLoadState(ELoadState loadState) { m_LoadState = loadState; }

private:
    ELoadState m_LoadState = ELoadState::None;
};

} // namespace Process
} // namespace Compile
} // namespace SimpleLanguage
