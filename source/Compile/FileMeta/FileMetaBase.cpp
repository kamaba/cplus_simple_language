//****************************************************************************
//  File:      FileMeta.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "FileMetaBase.h"
#include "FileMeta.h"

namespace SimpleLanguage {
namespace Compile {

int FileMetaBase::s_IdCount = 0;

FileMetaBase::FileMetaBase() {
    m_Id = ++s_IdCount;
}

std::string FileMetaBase::Name() const {
    if (m_Token != nullptr) {
        return m_Token->GetLexeme();
    }
    return "";
}

void FileMetaBase::SetFileMeta(FileMeta* fm) {
    m_FileMeta = fm;
}

void FileMetaBase::SetDeep(int _deep) {
    m_Deep = _deep;
}

std::string FileMetaBase::ToFormatString() const {
    return "";
}

} // namespace Compile
} // namespace SimpleLanguage

