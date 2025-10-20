//****************************************************************************
//  File:      FileParse.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include <string>
#include <functional>
#include <memory>
#include "../FileMeta/FileMeta.h"
#include "LexerParse.h"
#include "TokenParse.h"
#include "StructParse.h"
#include "../Process/FileCompileState.h"
#include "ProjectManager.h"
#include "../../Debug/Log.h"

namespace SimpleLanguage {
namespace Compile {

    class FileMeta;
    class LexerParse;
    class TokenParse;
    class StructParse;
    class FileCompileState;

enum class ECodeFileParseState {
    Null,
    Init,
    LoadBegin,
    LoadEnd
};

struct ParseFileParam {
    // 空的参数结构
};

class FileParse {
public:
    std::string filePath;
    long fileSize;
    std::string content;
    
    std::function<void()> structParseComplete;
    std::function<void()> buildParseComplete;
    std::function<void()> grammerParseComplete;

    FileParse(const std::string& path, const ParseFileParam& param);
    
    bool IsExists();
    bool LoadFile();
    void StructParse();
    void CreateNamespace();
    void CombineFileMeta();
    std::string ToFormatString();
    void PrintFormatString();

private:
    std::unique_ptr<LexerParse> lexerParse;
    std::unique_ptr<TokenParse> tokenParse;
    std::unique_ptr<StructParse> structBuild;
    std::unique_ptr<FileMeta> m_File;
    FileCompileState m_FileCompileState;
};

} // namespace Compile
} // namespace SimpleLanguage
