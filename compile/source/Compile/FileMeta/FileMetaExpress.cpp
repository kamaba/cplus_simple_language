//****************************************************************************
//  File:      FileMetaExpress.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "FileMetaExpress.h"
#include "FileMeta.h"
#include "FileMetaCommon.h"
#include "FileMetaSyntax.h"
#include "FileMetatUtil.h"
#include "../../Debug/Log.h"
#include "../../Define.h"
#include "../Parse/StructParse.h"
#include <algorithm>
#include <limits>
#include <sstream>

using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Compile {

// FileMetaBaseTerm implementation
FileMetaBaseTerm::FileMetaBaseTerm(FileMeta* fm) 
{
    m_FileMeta = fm;
    m_Root = this;
}

bool FileMetaBaseTerm::IsOnlyOne() const {
    return GetLeft() == nullptr && GetRight() == nullptr;
}

void FileMetaBaseTerm::SetLeft(FileMetaBaseTerm* value) {
    m_Left = value;
    m_IsDirty = true;
}

void FileMetaBaseTerm::SetRight(FileMetaBaseTerm* value) {
    m_Right = value;
    m_IsDirty = true;
}

std::vector<FileMetaBaseTerm*> FileMetaBaseTerm::SplitParamList() {
    std::vector<FileMetaBaseTerm*> paramFileMetaTermList;
    
    std::vector<std::vector<FileMetaBaseTerm*>> fmbtListList;
    std::vector<FileMetaBaseTerm*> fmbtList;
    
    bool isComma = false;
    for (size_t i = 0; i < m_FileMetaExpressList.size(); i++) {
        auto fmen = m_FileMetaExpressList[i];
        auto fmst = dynamic_cast<FileMetaSymbolTerm*>(fmen);
        if (fmst != nullptr && fmst->GetToken() != nullptr && 
            fmst->GetToken()->GetType() == ETokenType::Comma) {
            if (isComma) {
                Log::AddInStructFileMeta(EError::None, "Error 多重逗号，导致解析无法解析!!");
                break;
            }
            if (fmbtList.empty()) {
                Log::AddInStructFileMeta(EError::None, "Error 首符号不能为逗号");
                break;
            }
            isComma = true;
            fmbtListList.push_back(fmbtList);
            fmbtList.clear();
        } else {
            isComma = false;
            fmbtList.push_back(fmen);
        }
    }
    if (fmbtList.empty()) {
        return paramFileMetaTermList;
    }
    fmbtListList.push_back(fmbtList);
    
    for (size_t i = 0; i < fmbtListList.size(); i++) {
        auto fmbt2 = fmbtListList[i];
        
        if (fmbt2.size() == 1) {
            paramFileMetaTermList.push_back(fmbt2[0]);
        } else {
            paramFileMetaTermList.insert(paramFileMetaTermList.end(), fmbt2.begin(), fmbt2.end());
        }
    }
    return paramFileMetaTermList;
}

void FileMetaBaseTerm::ClearDirty() {
    m_IsDirty = false;
    for (size_t i = 0; i < m_FileMetaExpressList.size(); i++) {
        auto fme = m_FileMetaExpressList[i];
        fme->ClearDirty();
    }
}

void FileMetaBaseTerm::AddFileMetaTerm(FileMetaBaseTerm* fmn) {
    if (fmn != nullptr) {
        fmn->SetFileMeta(m_FileMeta);
        m_FileMetaExpressList.push_back(fmn);
    }
}

void FileMetaBaseTerm::AddRangeFileMetaTerm(const std::vector<FileMetaBaseTerm*>& fmn) {
    for (size_t i = 0; i < fmn.size(); i++) {
        if (fmn[i] != nullptr) {
            fmn[i]->SetFileMeta(m_FileMeta);
        }
    }
    m_FileMetaExpressList.insert(m_FileMetaExpressList.end(), fmn.begin(), fmn.end());
}

std::string FileMetaBaseTerm::ToFormatString() const {
    if (m_Token != nullptr) {
        return m_Token->GetLexemeString();
    }
    return "";
}

std::string FileMetaBaseTerm::ToTokenString() const {
    std::ostringstream sb;
    
    if (m_Token != nullptr) {
        sb << m_Token->ToLexemeAllString();
    }
    
    if (GetLeft() != nullptr) {
        // Handle left side
    }
    if (GetRight() != nullptr) {
        // Handle right side
    }
    
    for (size_t i = 0; i < m_FileMetaExpressList.size(); i++) {
        auto fme = m_FileMetaExpressList[i];
        sb << " " << fme->ToTokenString();
    }
    return sb.str();
}

} // namespace Compile
} // namespace SimpleLanguage
