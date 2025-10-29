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

namespace SimpleLanguage {
namespace Compile {

class FileMeta;
class LexerParse;
class TokenParse;
class StructParse;
class FileMetaCompileState;

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
    size_t fileSize;
    std::string content;
    
    std::function<void()> structParseComplete;
    std::function<void()> buildParseComplete;
    std::function<void()> grammerParseComplete;

    FileParse(const std::string& path, const ParseFileParam& param);
    
    bool IsExists();
    bool LoadFile();
    void HandleStructParse();
    void CreateNamespace();
    void CombineFileMeta();
    std::string ToFormatString();
    void PrintFormatString();

private:
    LexerParse* lexerParse;
    TokenParse* tokenParse;
    StructParse* structBuild;
    FileMeta* m_File;
    FileMetaCompileState* m_FileCompileState;
};

} // namespace Compile
} // namespace SimpleLanguage
