//****************************************************************************
//  File:      MetaBlockStatements.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description:  this's a statement in function! same link table model!
//****************************************************************************

#include "MetaBlockStatements.h"
#include <iostream>

namespace SimpleLanguage {
namespace Core {

// MetaBlockStatements implementation
MetaFunction* MetaBlockStatements::GetOwnerMetaFunction() const {
    return m_OwnerMetaFunction;
}

MetaStatements* MetaBlockStatements::GetOwnerMetaStatements() const {
    return m_OwnerMetaStatements;
}

FileMetaBlockSyntax* MetaBlockStatements::GetFileMetaBlockSyntax() const {
    return m_FileMetaBlockSyntax;
}

MetaBlockStatements::MetaBlockStatements(MetaBlockStatements* mbs) {
    m_OwnerMetaFunction = mbs->GetOwnerMetaFunction();
    m_OwnerMetaBlockStatements = mbs->m_OwnerMetaBlockStatements;
}

MetaBlockStatements::MetaBlockStatements(MetaFunction* mf) {
    m_OwnerMetaBlockStatements = nullptr;
    m_OwnerMetaFunction = mf;
}

MetaBlockStatements::MetaBlockStatements(MetaFunction* mf, FileMetaBlockSyntax* fmbs) {
    m_OwnerMetaBlockStatements = nullptr;
    m_OwnerMetaFunction = mf;
    m_FileMetaBlockSyntax = fmbs;
}

MetaBlockStatements::MetaBlockStatements(MetaBlockStatements* mbs, FileMetaBlockSyntax* fmbs) : MetaStatements(mbs) {
    mbs->m_ChildrenMetaBlockStatementsList.push_back(this);
    m_OwnerMetaFunction = mbs->GetOwnerMetaFunction();
    m_FileMetaBlockSyntax = fmbs;
}

void MetaBlockStatements::SetFileMetaBlockSyntax(FileMetaBlockSyntax* blockSyntax) {
    m_FileMetaBlockSyntax = blockSyntax;
}

void MetaBlockStatements::SetOwnerMetaStatements(MetaStatements* ms) {
    m_OwnerMetaStatements = ms;
}

void MetaBlockStatements::SetNextStatements(MetaStatements* ms) {
    m_NextMetaStatements = ms;
}

MetaStatements* MetaBlockStatements::FindNearestMetaForStatementsOrMetaWhileOrDoWhileStatements() {
    if (m_OwnerMetaStatements != nullptr) {
        // 检查是否为for或while语句
        if (auto forStmt = dynamic_cast<MetaForStatements*>(m_OwnerMetaStatements)) {
            return m_OwnerMetaStatements;
        }
        if (auto whileStmt = dynamic_cast<MetaWhileDoWhileStatements*>(m_OwnerMetaStatements)) {
            return m_OwnerMetaStatements;
        }
    }
    
    auto nextStatements = m_NextMetaStatements;
    while (nextStatements != nullptr) {
        if (auto forStmt = dynamic_cast<MetaForStatements*>(nextStatements)) {
            return nextStatements;
        } else if (auto whileStmt = dynamic_cast<MetaWhileDoWhileStatements*>(nextStatements)) {
            return nextStatements;
        }
        nextStatements = nextStatements->GetNextMetaStatements();
    }
    
    if (m_OwnerMetaBlockStatements != nullptr) {
        return m_OwnerMetaBlockStatements->FindNearestMetaForStatementsOrMetaWhileOrDoWhileStatements();
    }
    return nullptr;
}

void MetaBlockStatements::SetDeep(int dp) {
    m_Deep = dp;
    if (m_NextMetaStatements != nullptr) {
        m_NextMetaStatements->SetDeep(m_Deep + 1);
    }
}

MetaVariable* MetaBlockStatements::GetMetaVariable(const std::string& name) {
    auto it = m_MetaVariableDict.find(name);
    if (it != m_MetaVariableDict.end()) {
        return it->second;
    }
    return nullptr;
}

bool MetaBlockStatements::AddMetaVariable(MetaVariable* mv) {
    if (m_MetaVariableDict.find(mv->GetName()) != m_MetaVariableDict.end()) {
        Token* token = m_FileMetaBlockSyntax ? m_FileMetaBlockSyntax->GetToken() : nullptr;
        std::cout << "error Class: [" << (GetOwnerMetaClass() ? GetOwnerMetaClass()->GetAllClassName() : "") 
                  << "] Method: [" << (m_OwnerMetaFunction ? m_OwnerMetaFunction->GetFunctionAllName() : "") << "]"
                  << "已定义过了变量名称!!! MBS:" << (token ? token->ToLexemeAllString() : "") 
                  << " var:" << mv->ToFormatString() << std::endl;
        return false;
    }
    m_MetaVariableDict[mv->GetName()] = mv;
    return true;
}

void MetaBlockStatements::AddFrontStatements(MetaStatements* ms) {
    auto t = m_NextMetaStatements;
    m_NextMetaStatements = ms;
    ms->SetNextStatements(t);
}

void MetaBlockStatements::AddFrontToEndStatements(MetaStatements* ms) {
    auto t = m_NextMetaStatements;
    m_NextMetaStatements = ms;
    
    auto tms = ms;
    while (true) {
        if (tms->GetNextMetaStatements() != nullptr) {
            tms = tms->GetNextMetaStatements();
        } else {
            break;
        }
    }
    tms->SetNextStatements(t);
}

bool MetaBlockStatements::UpdateMetaVariableDict(MetaVariable* mv) {
    if (m_MetaVariableDict.find(mv->GetName()) != m_MetaVariableDict.end()) {
        m_MetaVariableDict[mv->GetName()] = mv;
        return true;
    }
    m_MetaVariableDict[mv->GetName()] = mv;
    return true;
}

void MetaBlockStatements::GetCalcMetaVariableList(std::vector<MetaVariable*>& list) {
    for (auto& v : m_MetaVariableDict) {
        if (!v.second->IsArgument()) {
            list.push_back(v.second);
        }
    }
    
    for (auto& t : m_ChildrenMetaBlockStatementsList) {
        t->GetCalcMetaVariableList(list);
    }
}

bool MetaBlockStatements::GetIsMetaVariable(const std::string& name, bool isFromParent) {
    if (m_MetaVariableDict.find(name) != m_MetaVariableDict.end()) {
        return true;
    }
    if (m_ParentBlockStatements != nullptr && isFromParent) {
        return m_ParentBlockStatements->GetIsMetaVariable(name);
    }
    return false;
}

bool MetaBlockStatements::AddOnlyNameMetaVariable(const std::string& name) {
    if (m_MetaVariableDict.find(name) != m_MetaVariableDict.end()) {
        return false;
    }
    m_MetaVariableDict[name] = nullptr;
    return true;
}

MetaVariable* MetaBlockStatements::GetMetaVariableByName(const std::string& name, bool isFromParent) {
    auto it = m_MetaVariableDict.find(name);
    if (it != m_MetaVariableDict.end()) {
        return it->second;
    }
    
    if (m_ParentBlockStatements != nullptr && isFromParent) {
        return m_ParentBlockStatements->GetMetaVariableByName(name, isFromParent);
    }
    return nullptr;
}

void MetaBlockStatements::SetMetaMemberParamCollection(MetaDefineParamCollection* mmpc) {
    auto list = mmpc->GetMetaDefineParamList();
    for (int i = 0; i < list.size(); i++) {
        auto mmpcp = list[i];
        AddMetaVariable(mmpcp->GetMetaVariable());
    }
}

std::string MetaBlockStatements::ToFormatString() {
    std::string result;
    for (int i = 0; i < m_RealDeep; i++) {
        result += Global::GetTabChar();
    }
    result += "{\n";
    
    if (m_NextMetaStatements != nullptr) {
        result += m_NextMetaStatements->ToFormatString() + "\n";
    }
    
    for (int i = 0; i < m_RealDeep; i++) {
        result += Global::GetTabChar();
    }
    result += "}";
    
    return result;
}

} // namespace Core
} // namespace SimpleLanguage
