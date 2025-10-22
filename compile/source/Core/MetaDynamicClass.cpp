//****************************************************************************
//  File:      MetaDynamicClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2025/5/30 12:00:00
//  Description: meta dynamic class
//****************************************************************************

#include "MetaDynamicClass.h"
#include "MetaClass.h"
#include <sstream>

namespace SimpleLanguage {
namespace Core {

MetaDynamicClass::MetaDynamicClass(const std::string& _name) : MetaClass(_name) {
    m_Type = EType::Class;
}

void MetaDynamicClass::ParseDefineComplete() {
    MetaClass::ParseDefineComplete();
}

std::string MetaDynamicClass::ToFormatString() const {
    std::stringstream stringBuilder;
    stringBuilder.str(""); // Clear the stringstream
    
    for (int i = 0; i < GetRealDeep(); i++) {
        stringBuilder << Global::tabChar;
    }
    
    // if (topLevelMetaNamespace != null)
    // {
    //     stringBuilder << topLevelMetaNamespace->GetAllName() << ".";
    // }
    stringBuilder << GetName() << " ";

    stringBuilder << std::endl;
    for (int i = 0; i < GetRealDeep(); i++) {
        stringBuilder << Global::tabChar;
    }
    stringBuilder << "{" << std::endl;

    // foreach (var v in m_ChildrenNameNodeDict )
    // {
    //     MetaBase* mb = v.second;
    //     if (dynamic_cast<MetaMemberVariable*>(mb) != nullptr)
    //     {
    //         stringBuilder << (dynamic_cast<MetaMemberVariable*>(mb))->ToFormatString();
    //         stringBuilder << std::endl;
    //     }
    // }

    for (int i = 0; i < GetRealDeep(); i++) {
        stringBuilder << Global::tabChar;
    }
    stringBuilder << "}" << std::endl;

    return stringBuilder.str();
}

std::string MetaDynamicClass::ToString() const {
    return MetaClass::ToString();
}

} // namespace Core
} // namespace SimpleLanguage
