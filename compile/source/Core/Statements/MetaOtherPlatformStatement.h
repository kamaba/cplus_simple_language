//****************************************************************************
//  File:      MetaOtherPlatformStatement.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "MetaStatements.h"
#include "../Global.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

class MetaOtherPlatformStatements : public MetaStatements {
public:
    MetaOtherPlatformStatements(MetaBlockStatements* mbs) : MetaStatements(mbs) {
    }
    
    virtual std::string ToFormatString() override {
        std::string result;
        for (int i = 0; i < m_RealDeep; i++) {
            result += Global::GetTabChar();
        }
        return result;
    }
    
    std::string ToTokenString() {
        std::string result;
        // result += m_FileMetaCallLink->ToTokenString();
        return result;
    }
};

} // namespace Core
} // namespace SimpleLanguage
