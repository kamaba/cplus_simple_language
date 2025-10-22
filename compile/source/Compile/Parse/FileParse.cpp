//****************************************************************************
//  File:      FileParse.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "FileParse.h"
#include "LexerParse.h"
#include "TokenParse.h"
#include "StructParse.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include "../FileMeta/FileMeta.h"
#include "../../Project/ProjectManager.h"
#include "../Process/FileMetaCompileState.h"
#include "../../Debug/Log.h"
#include <stdio.h>
#include <fstream>

bool FileExists(const char* path) {
    if (path == NULL) return false;  // ·��Ϊ�գ�ֱ�ӷ��� false
    // ��ֻ����ʽ���ļ����ɹ������
    std::ifstream file(path);
    if (file.bad() == false) {
        return true;
    }
    return false;  // ��ʧ�ܣ��ļ������ڻ���Ȩ�ޣ�
}
using namespace SimpleLanguage::Project;
using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Compile {

FileParse::FileParse(const std::string& path, const ParseFileParam& param)
{
    filePath = path;
    fileSize = 0;
    m_File = new FileMeta(filePath);
    m_FileCompileState = new FileMetaCompileState();
}

bool FileParse::IsExists() 
{
    std::string addsign = "\\";
#ifdef _WIN32
    addsign = "\\";
#else
    addsign = "/"; 
#endif
    ::std::string realpath = ProjectManager::projectPath + addsign + filePath;
    return FileExists(realpath.c_str());
}
static std::string AddPath(std::string path1, std::string path2)
{

    std::string addsign = "\\";
#ifdef _WIN32
    addsign = "\\";
#else
    addsign = "/";
#endif
    ::std::string realpath = path1 + addsign + path2;
}

bool FileParse::LoadFile() 
{
    m_FileCompileState->SetLoadState(FileMetaCompileState::ELoadState::LoadStart);
    
    std::string realpath = AddPath(ProjectManager::projectPath, filePath );
    
    std::ifstream file(realpath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    m_FileCompileState->SetLoadState(FileMetaCompileState::ELoadState::Loading);
    
    // ��ȡ�ļ���С
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // ��ȡ�ļ�����
    content.resize(fileSize);
    file.read(&content[0], fileSize);
    file.close();
    
    m_FileCompileState->SetLoadState(FileMetaCompileState::ELoadState::LoadEnd);
    return true;
}

void FileParse::HandleStructParse() 
{
    if (LoadFile())
    {
        lexerParse = new LexerParse(filePath, content);
        content.clear();
        
        lexerParse->ParseToTokenList();
        
        tokenParse = new TokenParse( m_File, lexerParse->GetListTokensWidthEnd());
        
        tokenParse->BuildStruct();
        
        structBuild = new StructParse(m_File, tokenParse->GetRootNode());
        
        structBuild->ParseRootNodeToFileMeta();
        
        m_File->SetDeep(0);
        
        if (structParseComplete) {
            structParseComplete();
        }
    } else {
        Log::AddInStructFileMeta(EError::None, "��ȡ�ļ����� FileParse Parse LoadFile !!!");
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
    Log::AddInStructFileMeta(EError::None, m_File->ToFormatString());
}

} // namespace Compile
} // namespace SimpleLanguage
