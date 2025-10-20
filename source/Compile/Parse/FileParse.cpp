//****************************************************************************
//  File:      FileParse.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "FileParse.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace SimpleLanguage {
namespace Compile {

FileParse::FileParse(const std::string& path, const ParseFileParam& param)
    : filePath(path), fileSize(0) {
    m_File = std::make_unique<FileMeta>(filePath);
}

bool FileParse::IsExists() {
    std::string realpath = std::filesystem::path(ProjectManager::GetInstance().GetProjectPath()) / filePath;
    return std::filesystem::exists(realpath);
}

bool FileParse::LoadFile() {
    m_FileCompileState.SetLoadState(FileCompileState::ELoadState::LoadStart);
    
    std::string realpath = std::filesystem::path(ProjectManager::GetInstance().GetProjectPath()) / filePath;
    
    std::ifstream file(realpath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    m_FileCompileState.SetLoadState(FileCompileState::ELoadState::Loading);
    
    // 获取文件大小
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // 读取文件内容
    content.resize(fileSize);
    file.read(&content[0], fileSize);
    file.close();
    
    m_FileCompileState.SetLoadState(FileCompileState::ELoadState::LoadEnd);
    return true;
}

void FileParse::StructParse() {
    if (LoadFile()) {
        lexerParse = std::make_unique<LexerParse>(filePath, content);
        content.clear();
        
        lexerParse->ParseToTokenList();
        
        tokenParse = std::make_unique<TokenParse>(*m_File, lexerParse->GetListTokensWidthEnd());
        
        tokenParse->BuildStruct();
        
        structBuild = std::make_unique<StructParse>(*m_File, tokenParse->GetRootNode());
        
        structBuild->ParseRootNodeToFileMeta();
        
        m_File->SetDeep(0);
        
        if (structParseComplete) {
            structParseComplete();
        }
    } else {
        Log::GetInstance().AddInStructFileMeta(EError::None, "读取文件出错 FileParse Parse LoadFile !!!");
    }
}

void FileParse::CreateNamespace() {
    m_File->CreateNamespace();
}

void FileParse::CombineFileMeta() {
    m_File->CombineFileMeta();
}

std::string FileParse::ToFormatString() {
    return m_File->ToFormatString();
}

void FileParse::PrintFormatString() {
    Log::GetInstance().AddInStructFileMeta(EError::None, m_File->ToFormatString());
}

} // namespace Compile
} // namespace SimpleLanguage
