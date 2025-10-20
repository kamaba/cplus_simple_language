//****************************************************************************
//  File:      MetaVariable.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description:  all variable 's define, if it's iterator style then use IteratorMetaVariable, other custom same style!
//****************************************************************************

#include "MetaVariable.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "../Compile/CompilerUtil.h"
#include <algorithm>

namespace SimpleLanguage {
namespace Core {

MetaVariable::MetaVariable() : MetaBase() {
    m_VariableFrom = EVariableFrom::None;
}

MetaVariable::MetaVariable(const MetaVariable& mv) : MetaBase(mv) {
    m_OwnerMetaClass = mv.m_OwnerMetaClass;
    m_DefineMetaType = new MetaType(*mv.m_DefineMetaType);
    if (mv.m_RealMetaType != nullptr) {
        m_RealMetaType = new MetaType(*mv.m_RealMetaType);
    }
    m_VariableFrom = mv.m_VariableFrom;
    m_PintTokenList = mv.m_PintTokenList;
    m_IsStatic = mv.m_IsStatic;
    m_IsConst = mv.m_IsConst;
    m_IsParsed = mv.m_IsParsed;
    m_SourceMetaVariable = mv.m_SourceMetaVariable;

    for (const auto& v : mv.m_MetaVariableDict) {
        m_MetaVariableDict[v.first] = new MetaVariable(*v.second);
    }
    m_OwnerMetaBlockStatements = mv.m_OwnerMetaBlockStatements;
}

MetaVariable::MetaVariable(const std::string& name, EVariableFrom from, MetaBlockStatements* mbs, MetaClass* ownerClass, MetaType* mdt) : MetaBase() {
    m_Name = name;
    m_VariableFrom = from;
    m_OwnerMetaBlockStatements = mbs;
    m_OwnerMetaClass = ownerClass;
    m_DefineMetaType = mdt;
    if (m_DefineMetaType == nullptr) {
        m_DefineMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    }
    m_RealMetaType = new MetaType(*m_DefineMetaType);
}

void MetaVariable::SetOwnerMetaClass(MetaClass* ownerclass) {
    m_OwnerMetaClass = ownerclass;
}

MetaClass* MetaVariable::GetOwnerClassTemplateClass() const {
    // 这里需要根据实际的 MetaGenTemplateClass 结构来处理
    // if (auto mgtc = dynamic_cast<MetaGenTemplateClass*>(m_OwnerMetaClass)) {
    //     return mgtc->GetMetaTemplateClass();
    // }
    return m_OwnerMetaClass;
}

void MetaVariable::AddPingToken(const std::string& path, int beginline, int beginpos, int endline, int endpos) {
    auto pingToken = new Token(path, ETokenType::None, "", beginline, beginpos);
    pingToken->SetSourceEnd(endline, endpos);

    auto find1 = std::find_if(m_PintTokenList.begin(), m_PintTokenList.end(), 
        [beginline, beginpos](Token* token) {
            return token->GetSourceBeginLine() == beginline && token->GetSourceBeginChar() == beginpos;
        });
    
    if (find1 == m_PintTokenList.end()) {
        m_PintTokenList.push_back(pingToken);
    }
}

void MetaVariable::AddPingToken(Token* token) {
    if (token != nullptr) {
        m_PintTokenList.push_back(token);
    }
}

void MetaVariable::AddMetaVariable(const std::string& name, MetaVariable* mv) {
    if (mv != nullptr) {
        m_MetaVariableDict[name] = mv;
    }
}

MetaVariable* MetaVariable::GetMetaVariable(const std::string& name) const {
    auto it = m_MetaVariableDict.find(name);
    if (it != m_MetaVariableDict.end()) {
        return it->second;
    }
    return nullptr;
}

bool MetaVariable::AddMetaVariable(MetaVariable* mv) {
    if (mv != nullptr && !mv->GetName().empty()) {
        auto it = m_MetaVariableDict.find(mv->GetName());
        if (it == m_MetaVariableDict.end()) {
            m_MetaVariableDict[mv->GetName()] = mv;
            return true;
        }
    }
    return false;
}

void MetaVariable::RemoveMetaVariable(const std::string& name) {
    auto it = m_MetaVariableDict.find(name);
    if (it != m_MetaVariableDict.end()) {
        delete it->second;
        m_MetaVariableDict.erase(it);
    }
}

void MetaVariable::ClearMetaVariable() {
    for (auto& pair : m_MetaVariableDict) {
        delete pair.second;
    }
    m_MetaVariableDict.clear();
}

std::vector<MetaVariable*> MetaVariable::GetAllMetaVariableList() const {
    std::vector<MetaVariable*> result;
    for (const auto& pair : m_MetaVariableDict) {
        result.push_back(pair.second);
    }
    return result;
}

std::string MetaVariable::ToFormatString() const {
    std::string result = m_Name;
    if (m_DefineMetaType != nullptr) {
        result += " : " + m_DefineMetaType->ToFormatString();
    }
    return result;
}

} // namespace Core
} // namespace SimpleLanguage
