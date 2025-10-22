//****************************************************************************
//  File:      MetaReturnStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "MetaStatements.h"
#include "../MetaExpressNode/MetaExpressBase.h"
#include "../BaseMetaClass/CoreMetaClassManager.h"
#include "../ExpressManager.h"
#include "../AllowUseSettings.h"
#include "../Compile/CoreFileMeta/FileMetaKeyReturnSyntax.h"
#include "../Global.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

class MetaReturnStatements : public MetaStatements {
private:
    FileMetaKeyReturnSyntax* m_FileMetaReturnSyntax = nullptr;
    MetaType* m_ReturnMetaDefineType = nullptr;
    MetaExpressNode* m_Express = nullptr;

public:
    MetaExpressNode* GetExpress() const { return m_Express; }
    
    MetaReturnStatements(MetaBlockStatements* mbs, FileMetaKeyReturnSyntax* fmrs) : MetaStatements(mbs) {
        m_FileMetaReturnSyntax = fmrs;
        
        MetaType* mdt = mbs->GetOwnerMetaFunction()->GetMetaDefineType();
        
        CreateExpressParam* cep2 = new CreateExpressParam();
        cep2->SetOwnerMetaClass(m_OwnerMetaBlockStatements->GetOwnerMetaClass());
        cep2->SetOwnerMBS(m_OwnerMetaBlockStatements);
        cep2->SetMetaType(mdt);
        cep2->SetFme(m_FileMetaReturnSyntax->GetReturnExpress());
        cep2->SetIsStatic(false);
        cep2->SetIsConst(false);
        cep2->SetParseFrom(EParseFrom::StatementRightExpress);
        cep2->SetEqualMetaVariable(mbs->GetOwnerMetaFunction()->GetReturnMetaVariable());
        
        m_Express = ExpressManager::CreateExpressNode(cep2);
        if (m_Express != nullptr) {
            AllowUseSettings* auc = new AllowUseSettings();
            auc->SetParseFrom(EParseFrom::StatementRightExpress);
            m_Express->Parse(auc);
            m_Express->CalcReturnType();
            m_ReturnMetaDefineType = m_Express->GetReturnMetaDefineType();
        } else {
            m_ReturnMetaDefineType = new MetaType(CoreMetaClassManager::GetInstance().GetVoidMetaClass());
        }
    }
    
    virtual std::string ToFormatString() override {
        std::string result;
        for (int i = 0; i < m_RealDeep; i++) {
            result += Global::GetTabChar();
        }
        result += "return ";
        result += m_Express ? m_Express->ToFormatString() : "";
        result += ";";
        return result;
    }
};

class MetaTRStatements : public MetaStatements {
public:
    MetaClass* returnMetaClass = nullptr;
    MetaExpressNode* m_Express = nullptr;

private:
    FileMetaKeyReturnSyntax* m_FileMetaReturnSyntax = nullptr;

public:
    MetaTRStatements(MetaBlockStatements* mbs, FileMetaKeyReturnSyntax* fmrs) : MetaStatements(mbs) {
        m_FileMetaReturnSyntax = fmrs;
        
        if (m_FileMetaReturnSyntax && m_FileMetaReturnSyntax->GetReturnExpress() != nullptr) {
            MetaType* mdt = new MetaType(returnMetaClass);
            
            CreateExpressParam* cep2 = new CreateExpressParam();
            cep2->SetOwnerMBS(m_OwnerMetaBlockStatements);
            cep2->SetMetaType(mdt);
            cep2->SetFme(m_FileMetaReturnSyntax->GetReturnExpress());
            cep2->SetIsStatic(false);
            cep2->SetIsConst(false);
            cep2->SetParseFrom(EParseFrom::StatementRightExpress);
            
            m_Express = ExpressManager::CreateExpressNode(cep2);
        }
        
        if (m_Express != nullptr) {
            m_Express->CalcReturnType();
            // returnMetaClass = ClassManager::GetInstance().GetClassByMetaType(m_Express->GetReturnType());
        } else {
            returnMetaClass = CoreMetaClassManager::GetInstance().GetVoidMetaClass();
        }
    }
    
    virtual std::string ToFormatString() override {
        std::string result;
        for (int i = 0; i < m_RealDeep; i++) {
            result += Global::GetTabChar();
        }
        if (m_TrMetaVariable != nullptr) {
            result += m_TrMetaVariable->GetName();
            result += " = ";
        }
        result += m_Express ? m_Express->ToFormatString() : "";
        result += ";";
        return result;
    }
};

} // namespace Core
} // namespace SimpleLanguage
