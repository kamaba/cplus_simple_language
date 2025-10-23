//****************************************************************************
//  File:      FileMeta.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include <string>

namespace SimpleLanguage {
namespace Compile {
// Forward declarations
class FileMeta;
class Token;

class FileMetaBase {
public:
    FileMetaBase();
    virtual ~FileMetaBase() = default;

    // Properties
    Token* GetToken() const { return m_Token; }
    int Deep() const { return m_Deep; }
    virtual std::string GetName() const;
    FileMeta* GetFileMeta() const { return m_FileMeta; }

    // Methods
    void SetFileMeta(FileMeta* fm);
    virtual void SetDeep(int _deep);
    virtual std::string ToFormatString() const;
    virtual std::string ToString() const { return ""; }

protected:
    int m_Id = 0;
    int m_Deep = 0;
    FileMeta* m_FileMeta = nullptr;
    Token* m_Token = nullptr;
    static int s_IdCount;
};

} // namespace Compile
} // namespace SimpleLanguage

