//****************************************************************************
//  File:      IRData.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#include "IRData.h"
#include "../Compile/Token.h"
#include "../Core/MetaType.h"
#include "IRMethod.h"
#include <sstream>
#include <typeinfo>

namespace SimpleLanguage {
namespace IR {

IRData::IRData() {
}

void IRData::SetDebugInfoByValue(const DebugInfo& info) {
    debugInfo = info;
}

void IRData::SetDebugInfoByToken(Token* token) {
    if (token != nullptr) {
        debugInfo.path = token->path;
        debugInfo.name = token->lexeme ? token->lexeme->ToString() : "";
        debugInfo.beginLine = token->sourceBeginLine;
        debugInfo.beginChar = token->sourceBeginChar;
        debugInfo.endLine = token->sourceEndLine;
        debugInfo.endChar = token->sourceEndChar;
    }
}

std::string IRData::ToString() const {
    std::ostringstream m_StringBuilder;
    m_StringBuilder << id << "   [ " << debugInfo.path << ":" << debugInfo.beginLine << "]" 
                    << " [" << static_cast<int>(opCode) << "] index:[" << index << "]";
    
    if (opValue != nullptr) {
        // Try to cast to different types
        MetaType* mt = static_cast<MetaType*>(opValue);
        IRMethod* irm = static_cast<IRMethod*>(opValue);
        
        // Check if it's an int32
        if (typeid(*opValue) == typeid(int32_t)) {
            m_StringBuilder << " val: int32[" << *static_cast<int32_t*>(opValue) << "] ";
        } else {
            if (mt != nullptr) {
                m_StringBuilder << " val mt:[" << mt->name << "] ";
            } else if (irm != nullptr) {
                m_StringBuilder << " val irm:[" << irm->id << "] ";
            } else {
                m_StringBuilder << " val:[" << static_cast<void*>(opValue) << "] ";
            }
        }
    }
    return m_StringBuilder.str();
}

} // namespace IR
} // namespace SimpleLanguage