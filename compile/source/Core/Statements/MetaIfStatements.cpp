//****************************************************************************
//  File:      MetaIfStatements.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#include "../../Compile/FileMeta/FileMetaSyntax.h"
#include "../../Compile/FileMeta/FileMetaExpress.h"
#include "MetaIfStatements.h"
#include "../MetaExpressNode/MetaExpressBase.h"
#include "../ExpressManager.h"
#include "../AllowUseSettings.h"
#include "../BaseMetaClass/CoreMetaClassManager.h"
#include "../MetaMemberFunction.h"
#include "../MetaType.h"
#include "../MetaVariable.h"
#include "../MetaClass.h"
#include "MetaBlockStatements.h"
#include <iostream>
#include <sstream>

using namespace SimpleLanguage::Compile;

namespace SimpleLanguage {
namespace Core {

// MetaElseIfStatements implementation
MetaBlockStatements* MetaIfStatements::MetaElseIfStatements::GetThenMetaStatements() const {
    return m_ThenMetaStatements;
}

MetaIfStatements::IfElseState MetaIfStatements::MetaElseIfStatements::GetIfElseState() const {
    return m_IfElseState;
}

Compile::FileMetaKeyOnlySyntax* MetaIfStatements::MetaElseIfStatements::GetElseKeySyntax() const {
    return m_ElseKeySyntax;
}

MetaExpressNode* MetaIfStatements::MetaElseIfStatements::GetFinalExpress() const {
    return m_FinalExpress;
}

MetaAssignManager* MetaIfStatements::MetaElseIfStatements::GetMetaAssignManager() const {
    return m_MetaAssignManager;
}

MetaVariable* MetaIfStatements::MetaElseIfStatements::GetBoolConditionVariable() const {
    return m_BoolConditionVariable;
}

MetaIfStatements::MetaElseIfStatements::MetaElseIfStatements(MetaBlockStatements* mbs, Compile::FileMetaConditionExpressSyntax* ifexpress, MetaExpressNode* conditionExpress) {
    m_IfOrElseIfKeySyntax = ifexpress;
    if (conditionExpress != nullptr) {
        m_Express = conditionExpress;
        m_FinalExpress = ExpressManager::CreateOptimizeAfterExpress(m_Express);
    }
    
    m_ThenMetaStatements = new MetaBlockStatements(mbs, ifexpress->GetExecuteBlockSyntax());
    MetaMemberFunction::CreateMetaSyntax(ifexpress->GetExecuteBlockSyntax(), m_ThenMetaStatements);
    Parse();
}

MetaIfStatements::MetaElseIfStatements::MetaElseIfStatements(MetaBlockStatements* mbs, Compile::FileMetaKeyOnlySyntax* elseKeySyntax) {
    m_ElseKeySyntax = elseKeySyntax;
    m_ThenMetaStatements = new MetaBlockStatements(mbs, elseKeySyntax->GetExecuteBlockSyntax());
    MetaMemberFunction::CreateMetaSyntax(elseKeySyntax->GetExecuteBlockSyntax(), m_ThenMetaStatements);
    Parse();
}

void MetaIfStatements::MetaElseIfStatements::ParseDefine(MetaVariable* mv) {
    m_ThenMetaStatements->SetTRMetaVariable(mv);
}

void MetaIfStatements::MetaElseIfStatements::SetIfElseState(IfElseState ieState) {
    m_IfElseState = ieState;
}

void MetaIfStatements::MetaElseIfStatements::SetDeep(int d) {
    m_Deep = d;
    m_ThenMetaStatements->SetDeep(m_Deep);
}

void MetaIfStatements::MetaElseIfStatements::Parse() {
    if (m_FinalExpress != nullptr) {
        m_FinalExpress->CalcReturnType();
        m_MetaAssignManager = new MetaAssignManager(m_FinalExpress, m_ThenMetaStatements, 
            new MetaType(CoreMetaClassManager::GetInstance().GetBooleanMetaClass()));
        m_BoolConditionVariable = m_MetaAssignManager->GetJudgmentValueMetaVariable();
        if (m_MetaAssignManager->IsNeedSetMetaVariable()) {
            m_ThenMetaStatements->UpdateMetaVariableDict(m_BoolConditionVariable);
        }
    }
}

std::string MetaIfStatements::MetaElseIfStatements::ToFormatString() {
    std::string result;
    
    if (m_IfElseState != IfElseState::Else) {
        for (int i = 0; i < m_Deep; i++) {
            result += Global::GetTabChar();
        }
        result += m_BoolConditionVariable->GetMetaDefineType()->GetMetaClass()->GetAllClassName();
        result += " ";
        result += m_BoolConditionVariable->GetName();
        result += " = ";
        result += m_Express->ToFormatString();
        result += "\n";
    }
    
    for (int i = 0; i < m_Deep; i++) {
        result += Global::GetTabChar();
    }
    
    if (m_IfElseState == IfElseState::If) {
        result += "if ";
        result += m_BoolConditionVariable->GetName();
    } else if (m_IfElseState == IfElseState::ElseIf) {
        result += "elif ";
        result += m_BoolConditionVariable->GetName();
    } else if (m_IfElseState == IfElseState::Else) {
        result += "else";
    }
    
    if (m_ThenMetaStatements != nullptr) {
        result += "\n";
        result += m_ThenMetaStatements->ToFormatString();
    }
    
    return result;
}

// MetaIfStatements implementation
std::vector<MetaIfStatements::MetaElseIfStatements*> MetaIfStatements::GetMetaElseIfStatements() const {
    return m_MetaElseIfStatements;
}

Compile::FileMetaKeyIfSyntax* MetaIfStatements::GetFileMetaKeyIfSyntax() const {
    return m_FileMetaKeyIfSyntax;
}

MetaIfStatements::MetaIfStatements(MetaBlockStatements* mbs, Compile::FileMetaKeyIfSyntax* fm, MetaVariable* mv) : MetaStatements(mbs) {
    m_FileMetaKeyIfSyntax = fm;
    m_MetaVariable = mv;
    Parse();
}

void MetaIfStatements::Parse() {
    if (m_FileMetaKeyIfSyntax->GetIfExpressSyntax() == nullptr) {
        std::cout << "Error 没有if语句!!" << std::endl;
    }
    
    MetaType* mdt = nullptr;
    if (m_MetaVariable == nullptr) {
        // 自动创建变量
    } else {
        mdt = m_MetaVariable->GetMetaDefineType();
    }
    
    CreateExpressParam* cep = new CreateExpressParam();
    cep->SetOwnerMBS(m_OwnerMetaBlockStatements);
    cep->SetOwnerMetaClass(m_OwnerMetaBlockStatements->GetOwnerMetaClass());
    cep->SetMetaType(mdt);
    cep->SetFme(m_FileMetaKeyIfSyntax->GetIfExpressSyntax()->GetConditionExpress());
    cep->SetIsStatic(false);
    cep->SetIsConst(false);
    cep->SetParseFrom(EParseFrom::StatementRightExpress);
    
    auto express = ExpressManager::CreateExpressNode(cep);
    express->Parse(new AllowUseSettings());
    
    MetaElseIfStatements* msis = new MetaElseIfStatements(m_OwnerMetaBlockStatements, 
        m_FileMetaKeyIfSyntax->GetIfExpressSyntax(), express);
    AddIfElseStateStatements(msis, IfElseState::If);
    
    if (m_MetaVariable == nullptr) {
        m_MetaVariable = msis->GetMetaAssignManager()->GetJudgmentValueMetaVariable();
    }
    
    if (msis->GetThenMetaStatements() != nullptr) {
        msis->GetThenMetaStatements()->SetTRMetaVariable(m_MetaVariable);
    }
    
    for (int i = 0; i < m_FileMetaKeyIfSyntax->GetElseIfExpressSyntax().size(); i++) {
        auto fmsthen = m_FileMetaKeyIfSyntax->GetElseIfExpressSyntax()[i];
        
        CreateExpressParam* cep2 = new CreateExpressParam();
        cep2->SetOwnerMBS(m_OwnerMetaBlockStatements);
        cep2->SetMetaType(mdt);
        cep2->SetFme(fmsthen->GetConditionExpress());
        cep2->SetIsStatic(false);
        cep2->SetIsConst(false);
        cep2->SetParseFrom(EParseFrom::StatementRightExpress);
        cep2->SetEqualMetaVariable(m_MetaVariable);
        
        auto express2 = ExpressManager::CreateExpressNode(cep2);
        MetaElseIfStatements* msis2 = new MetaElseIfStatements(m_OwnerMetaBlockStatements, fmsthen, express2);
        AddIfElseStateStatements(msis2, IfElseState::ElseIf);
        
        if (msis2->GetThenMetaStatements() != nullptr) {
            msis2->GetThenMetaStatements()->SetTRMetaVariable(m_MetaVariable);
        }
    }
    
    if (m_FileMetaKeyIfSyntax->GetElseExpressSyntax() != nullptr) {
        MetaElseIfStatements* msis3 = new MetaElseIfStatements(m_OwnerMetaBlockStatements, 
            m_FileMetaKeyIfSyntax->GetElseExpressSyntax());
        AddIfElseStateStatements(msis3, IfElseState::Else);
        
        if (msis3->GetThenMetaStatements() != nullptr) {
            msis3->GetThenMetaStatements()->SetTRMetaVariable(m_MetaVariable);
        }
    }
}

void MetaIfStatements::UpdateOwnerMetaClass(MetaClass* ownerclass) {
    MetaStatements::UpdateOwnerMetaClass(ownerclass);
}

void MetaIfStatements::SetDeep(int dp) {
    for (int i = 0; i < m_MetaElseIfStatements.size(); i++) {
        m_MetaElseIfStatements[i]->SetDeep(dp);
    }
    if (m_NextMetaStatements != nullptr) {
        m_NextMetaStatements->SetDeep(dp);
    }
}

void MetaIfStatements::AddIfElseStateStatements(MetaElseIfStatements* meis, IfElseState ife) {
    m_MetaElseIfStatements.push_back(meis);
    meis->SetIfElseState(ife);
}

void MetaIfStatements::SetTRMetaVariable(MetaVariable* mv) {
    for (int i = 0; i < m_MetaElseIfStatements.size(); i++) {
        if (m_MetaElseIfStatements[i]->GetThenMetaStatements() != nullptr) {
            m_MetaElseIfStatements[i]->GetThenMetaStatements()->SetTRMetaVariable(mv);
        }
    }
}

std::string MetaIfStatements::ToFormatString() const {
    std::string result;
    
    for (int i = 0; i < m_MetaElseIfStatements.size(); i++) {
        result += m_MetaElseIfStatements[i]->ToFormatString();
        result += "\n";
    }
    
    if (m_NextMetaStatements != nullptr) {
        result += m_NextMetaStatements->ToFormatString();
    }
    
    return result;
}

} // namespace Core
} // namespace SimpleLanguage