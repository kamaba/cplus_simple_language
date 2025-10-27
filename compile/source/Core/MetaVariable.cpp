//****************************************************************************
//  File:      MetaVariable.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description:  all variable 's define, if it's iterator style then use IteratorMetaVariable, other custom same style!
//****************************************************************************

#include "MetaVariable.h"
#include "MetaType.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "../Compile/CompilerUtil.h"
#include "../Compile/Token.h"
#include "../Debug/Log.h"
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
bool MetaVariable::IsArray() const { return m_DefineMetaType != nullptr ? m_DefineMetaType->IsArray() : false; }

void MetaVariable::SetOwnerMetaClass(MetaClass* ownerclass) {
    m_OwnerMetaClass = ownerclass;
}

void MetaVariable::SetDefineMetaClass(MetaClass* defineClass) {
    if (m_DefineMetaType != nullptr) {
        m_DefineMetaType->SetMetaClass(defineClass);
    }
}

MetaClass* MetaVariable::GetOwnerClassTemplateClass() const {
    // ������Ҫ����ʵ�ʵ� MetaGenTemplateClass �ṹ������
    // if (auto mgtc = dynamic_cast<MetaGenTemplateClass*>(m_OwnerMetaClass)) {
    //     return mgtc->GetMetaTemplateClass();
    // }
    return m_OwnerMetaClass;
}

void MetaVariable::AddPingToken(const std::string& path, int beginline, int beginpos, int endline, int endpos) {
    auto pingToken = new Compile::Token(path, ETokenType::None, "", beginline, beginpos);
    pingToken->SetSourceEnd(endline, endpos);

    auto find1 = std::find_if(m_PintTokenList.begin(), m_PintTokenList.end(), 
        [beginline, beginpos](Compile::Token* token) {
            return token->GetSourceBeginLine() == beginline && token->GetSourceBeginChar() == beginpos;
        });
    
    if (find1 == m_PintTokenList.end()) {
        m_PintTokenList.push_back(pingToken);
    }
}

void MetaVariable::AddPingToken(Compile::Token* token) {
    if (token == nullptr) {
        return;
    }
    
    auto find1 = std::find_if(m_PintTokenList.begin(), m_PintTokenList.end(),
        [token](Compile::Token* t) {
            return t->GetSourceBeginLine() == token->GetSourceBeginLine()
                && t->GetSourceBeginChar() == token->GetSourceBeginChar()
                && t->GetSourceEndLine() == token->GetSourceEndLine()
                && t->GetSourceEndChar() == token->GetSourceEndChar()
                && t->GetPath() == token->GetPath();
        });
    
    if (find1 == m_PintTokenList.end()) {
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
    std::ostringstream sb;
    sb << "[" << m_DefineMetaType->ToFormatString() << "]";
    sb << m_Name;
    return sb.str();
}

std::string MetaVariable::ToString() const {
    return m_Name;
}

// MetaVisitVariable implementation
MetaVisitVariable::MetaVisitVariable(MetaVariable* source, MetaVariable* target) : MetaVariable() {
    m_VisitType = EVisitType::Link;
    m_SourceMetaVariable = source;
    m_TargetMetaVariable = target;
    m_DefineMetaType = new MetaType(*target->GetMetaDefineType());
}

MetaVisitVariable::MetaVisitVariable(const std::string& name, MetaClass* mc, MetaBlockStatements* mbs, MetaVariable* lmv, MetaVariable* vmv) : MetaVariable() {
    m_Name = name;
    m_AtName = name;
    m_OwnerMetaClass = mc;
    m_SourceMetaVariable = lmv;
    if (lmv->IsArray()) {
        if (vmv == nullptr && m_AtName.empty()) {
            //Log::AddInStructMeta(EError::None, "Error VisitMetaVariable访问变量访问位置不能同时为空!!");
            return;
        }
        m_TargetMetaVariable = vmv;

        MetaType* gmit = m_SourceMetaVariable->GetMetaDefineType()->GetMetaInputTemplateByIndex();
        if (gmit == nullptr) {
            //Log::AddInStructMeta(EError::None, "Error 访问的Array中，没有找到模版 名称!!");
            return;
        }
        m_DefineMetaType = new MetaType(*gmit);
    }
}

int MetaVisitVariable::GetIRMemberIndex() {
    // 使用前向声明，避免循环依赖
    // MetaMemberVariable* mmv = dynamic_cast<MetaMemberVariable*>(m_SourceMetaVariable);
    // if (mmv != nullptr) {
    //     // return mmv->GetOwnerMetaClass()->GetLocalMemberVariableIndex(mmv);
    // }
    return -1;
}

std::string MetaVisitVariable::ToFormatString() const {
    std::ostringstream sb;

    if (m_VisitType == EVisitType::Link) {
        if (m_SourceMetaVariable != nullptr) {
            sb << "[" << m_SourceMetaVariable->GetMetaDefineType()->GetName() << "]";
            sb << m_SourceMetaVariable->GetName();
            sb << ".";
        }
        sb << "[" << m_TargetMetaVariable->GetMetaDefineType()->GetName() << "]";
        sb << m_TargetMetaVariable->GetName();
    } else {
        sb << m_SourceMetaVariable->GetName();
        if (m_SourceMetaVariable->IsArray()) {
            sb << "[";
            sb << m_Name;
            sb << "]";
        } else {
            sb << m_TargetMetaVariable->GetName();
        }
    }

    return sb.str();
}

// MetaIteratorVariable implementation
MetaIteratorVariable::MetaIteratorVariable(const std::string& name, MetaClass* mc, MetaBlockStatements* mbs, MetaVariable* lmv, MetaType* orgMC) : MetaVariable() {
    m_Name = name;
    m_OwnerMetaClass = mc;
    m_OwnerMetaBlockStatements = mbs;
    m_LocalMetaVariable = lmv;
    m_OrgMetaDefineType = orgMC;
    m_IndexMetaVariable = new MetaVariable("index", EVariableFrom::ArrayInner, mbs, mc, new MetaType(CoreMetaClassManager::GetInstance().GetInt32MetaClass()));
    m_ValueMetaVariable = new MetaVariable("value", EVariableFrom::ArrayInner, mbs, mc, new MetaType(orgMC->GetMetaClass()));
    m_IndexMetaVariable->AddPingToken(lmv->GetPingToken());
    m_ValueMetaVariable->AddPingToken(lmv->GetPingToken());
    if (lmv->IsArray()) {
        MetaType* gmit = m_LocalMetaVariable->GetMetaDefineType()->GetMetaInputTemplateByIndex();
        if (gmit == nullptr) {
            //Log::AddInStructMeta(EError::None, "Error 访问的Array中，没有找到模版 名称!!");
            return;
        }
        m_DefineMetaType = new MetaType(*gmit);
    } else {
        m_DefineMetaType = new MetaType(*lmv->GetMetaDefineType());
    }
}
MetaClass* MetaIteratorVariable::GetIteratorMetaClass() const { return m_OrgMetaDefineType->GetMetaClass(); }


MetaVariable* MetaIteratorVariable::GetMetaVariable(const std::string& name) const {
    if (name == "index") {
        return m_IndexMetaVariable;
    } else if (name == "value") {
        return m_ValueMetaVariable;
    }
    auto it = m_MetaVariableDict.find(name);
    if (it != m_MetaVariableDict.end()) {
        return it->second;
    }
    return m_OrgMetaDefineType->GetMetaClass()->GetMetaMemberVariableByName(name);
}

std::string MetaIteratorVariable::ToFormatString() const {
    std::ostringstream sb;

    sb << m_LocalMetaVariable->GetName();
    if (m_LocalMetaVariable->IsArray()) {
        sb << "[";
        sb << m_Name;
        sb << "]";
    }

    return sb.str();
}

std::string MetaIteratorVariable::ToString() const {
    return m_Name;
}

} // namespace Core
} // namespace SimpleLanguage
