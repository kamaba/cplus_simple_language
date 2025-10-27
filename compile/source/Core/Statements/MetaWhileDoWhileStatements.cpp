//****************************************************************************
//  File:      MetaWhileDoWhileStatements.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#include "MetaWhileDoWhileStatements.h"
#include "../MetaExpressNode/MetaExpressBase.h"
#include "../MetaExpressNode/MetaExpressOperator.h"
#include "../MetaExpressNode/MetaExpressCalllink.h"
#include "../MetaExpressNode/MetaExpressNewObject.h"
#include "../MetaVariable.h"
#include "../ExpressManager.h"
#include "../AllowUseSettings.h"
#include "../../Compile/FileMeta/FileMetaSyntax.h"
#include "../../Compile/FileMeta/FileMetaCommon.h"
#include "../../Compile/Token.h"
#include "../../Debug/Log.h"
#include "../../Define.h"
#include "../MetaType.h"
#include "MetaDefineVarStatements.h"
#include "MetaAssignStatements.h"
#include "MetaBlockStatements.h"
#include "../MetaMemberFunction.h"
#include <string>
#include <sstream>

using namespace SimpleLanguage::Debug;
using namespace SimpleLanguage::Compile;

namespace SimpleLanguage {
namespace Core {

// MetaForStatements implementation
MetaForStatements::MetaForStatements(MetaBlockStatements* mbs, Compile::FileMetaKeyForSyntax* fmkfs) : MetaStatements(mbs) {
    m_FileMetaKeyForSyntax = fmkfs;
    Parse();
}

void MetaForStatements::Parse() {
    m_IsForIn = m_FileMetaKeyForSyntax->IsInFor();
    
    m_ThenMetaStatements = new MetaBlockStatements(m_OwnerMetaBlockStatements, m_FileMetaKeyForSyntax->GetExecuteBlockSyntax());
    m_ThenMetaStatements->SetOwnerMetaStatements(this);
    
    if (m_IsForIn) {
        if (m_FileMetaKeyForSyntax->GetConditionExpress() == nullptr) {
            Log::AddInStructMeta(EError::None, "Error for in express没有表达式!!");
        }
        
        m_ForInContent = nullptr;
        if (m_FileMetaKeyForSyntax->GetConditionExpress() != nullptr) {
            CreateExpressParam* cep2 = new CreateExpressParam();
            cep2->SetOwnerMBS(m_OwnerMetaBlockStatements);
            cep2->SetMetaType(nullptr);
            cep2->SetFme(m_FileMetaKeyForSyntax->GetConditionExpress());
            cep2->SetIsStatic(false);
            cep2->SetIsConst(false);
            cep2->SetParseFrom(EParseFrom::StatementRightExpress);
            
            m_ConditionExpress = ExpressManager::CreateExpressNode(*cep2);
            m_ConditionExpress->CalcReturnType();
        }
        
        auto mcallEn = dynamic_cast<MetaCallLinkExpressNode*>(m_ConditionExpress);
        auto mnoen = dynamic_cast<MetaNewObjectExpressNode*>(m_ConditionExpress);
        if (mcallEn == nullptr && mnoen == nullptr) {
            Log::AddInStructMeta(EError::None, "Error For in 表达式应该是迭代表达式");
            return;
        }
        
        if (mcallEn != nullptr) {
            m_ForInContent = mcallEn->GetMetaVariable();
        } else {
            m_ForInContent = new MetaVariable("forcontent_" + std::to_string(reinterpret_cast<uintptr_t>(this)), 
                EVariableFrom::LocalStatement, m_OwnerMetaBlockStatements, GetOwnerMetaClass(), 
                mnoen->GetReturnMetaDefineType());
            m_ThenMetaStatements->UpdateMetaVariableDict(m_ForInContent);
        }
        
        MetaType* mdt = m_ForInContent->GetMetaDefineType();
        if (!mdt->IsCanForIn()) {
            Log::AddInStructMeta(EError::None, "Error For in 表达式应该是迭代表达式!");
            return;
        }
        
        auto forMVMC = mdt->GetMetaInputTemplateByIndex();
        if (forMVMC == nullptr) {
            forMVMC = m_ForInContent->GetMetaDefineType();
        }
        
        auto fmcd = dynamic_cast<Compile::FileMetaCallSyntax*>(m_FileMetaKeyForSyntax->GetFileMetaClassDefine());
        if (fmcd == nullptr) {
            Log::AddInStructMeta(EError::None, "Error For x in X语法错误!!");
            return;
        }
        
        std::string dname = fmcd->GetVariableRef()->GetName();
        auto dmv = m_ThenMetaStatements->GetMetaVariableByName(dname);
        if (dmv != nullptr) {
            Log::AddInStructMeta(EError::None, "Error 在 for .. in 中，变量名for 左边不能重复定义!!");
            return;
        } else {
            m_ForMetaVariable = new MetaIteratorVariable(dname, GetOwnerMetaClass(), m_OwnerMetaBlockStatements, 
                m_ForInContent, forMVMC);
        }
        
        m_ThenMetaStatements->UpdateMetaVariableDict(m_ForMetaVariable);
    } else {
        auto fmcd = m_FileMetaKeyForSyntax->GetFileMetaClassDefine();
        if (auto fmcd1 = dynamic_cast<Compile::FileMetaDefineVariableSyntax*>(fmcd)) {
            m_DefineVarStatements = new MetaDefineVarStatements(m_ThenMetaStatements, fmcd1);
        } else if (auto fmoas = dynamic_cast<Compile::FileMetaOpAssignSyntax*>(fmcd)) {
            std::string sname = fmoas->GetVariableRef() ? fmoas->GetVariableRef()->GetName() : "";
            
            if (m_ThenMetaStatements->GetIsMetaVariable(sname)) {
                m_AssignStatements = new MetaAssignStatements(m_ThenMetaStatements, fmoas);
            } else {
                m_ThenMetaStatements->AddOnlyNameMetaVariable(sname);
                m_DefineVarStatements = new MetaDefineVarStatements(m_ThenMetaStatements, fmoas);
            }
        } else if (auto fmcs = dynamic_cast<Compile::FileMetaCallSyntax*>(fmcd)) {
            std::string sname = fmcs->GetVariableRef() ? fmcs->GetVariableRef()->GetName() : "";
            
            if (m_ThenMetaStatements->GetIsMetaVariable(sname)) {
                m_AssignStatements = new MetaAssignStatements(m_ThenMetaStatements);
            } else {
                m_ThenMetaStatements->AddOnlyNameMetaVariable(sname);
                m_DefineVarStatements = new MetaDefineVarStatements(m_ThenMetaStatements, fmcs);
            }
        }
        
        if (m_FileMetaKeyForSyntax->GetStepFileMetaOpAssignSyntax() != nullptr) {
            m_StepStatements = new MetaAssignStatements(m_ThenMetaStatements, m_FileMetaKeyForSyntax->GetStepFileMetaOpAssignSyntax());
        }
        
        if (m_DefineVarStatements != nullptr) {
            m_ForMetaVariable = m_DefineVarStatements->GetDefineVarMetaVariable();
        } else if (m_AssignStatements != nullptr) {
            m_ForMetaVariable = m_AssignStatements->GetMetaVariable();
        }
        
        if (m_ForMetaVariable == nullptr) {
            Log::AddInStructMeta(EError::None, "Error 没有找到对应的变量!!");
        }
        m_ThenMetaStatements->UpdateMetaVariableDict(m_ForMetaVariable);
        
        if (m_FileMetaKeyForSyntax->GetConditionExpress() != nullptr) {
            CreateExpressParam* cep2 = new CreateExpressParam();
            cep2->SetOwnerMBS(m_ThenMetaStatements);
            cep2->SetMetaType(m_ForMetaVariable->GetMetaDefineType());
            cep2->SetFme(m_FileMetaKeyForSyntax->GetConditionExpress());
            cep2->SetIsStatic(false);
            cep2->SetIsConst(false);
            cep2->SetParseFrom(EParseFrom::StatementRightExpress);
            
            m_ConditionExpress = ExpressManager::CreateExpressNode(*cep2);
            m_ConditionExpress->CalcReturnType();
        }
    }
    
    // 解析块的语法为前序列表需要处理
    MetaMemberFunction::CreateMetaSyntax(m_FileMetaKeyForSyntax->GetExecuteBlockSyntax(), m_ThenMetaStatements);
}

void MetaForStatements::SetDeep(int dp) {
    if (m_ThenMetaStatements != nullptr) {
        m_ThenMetaStatements->SetDeep(dp);
    }
    if (m_NextMetaStatements != nullptr) {
        m_NextMetaStatements->SetDeep(dp);
    }
}

std::string MetaForStatements::ToFormatString() const {
    std::string result;
    
    for (int i = 0; i < m_Deep; i++) {
        result += Global::GetTabChar();
    }
    result += "for ";
    
    if (m_IsForIn) {
        result += m_ForMetaVariable->GetName();
        result += " in ";
        result += m_ForInContent->GetName();
    }
    
    result += "\n";
    for (int i = 0; i < m_Deep; i++) {
        result += Global::GetTabChar();
    }
    result += "{";
    result += "\n";
    
    if (!m_IsForIn) {
        for (int i = 0; i < m_Deep + 1; i++) {
            result += Global::GetTabChar();
        }
        if (m_DefineVarStatements != nullptr) {
            result += m_DefineVarStatements->ToFormatString();
        }
        if (m_AssignStatements != nullptr) {
            result += m_AssignStatements->ToFormatString();
        }
        if (m_StepStatements != nullptr) {
            for (int i = 0; i < m_Deep + 1; i++) {
                result += Global::GetTabChar();
            }
            result += m_StepStatements->ToFormatString();
        }
        
        if (m_ConditionExpress != nullptr) {
            result += "\n";
            for (int i = 0; i < m_Deep + 1; i++) {
                result += Global::GetTabChar();
            }
            result += "if ";
            result += m_ConditionExpress->ToFormatString();
            result += "{break;}";
            result += "\n";
        }
        result += m_ThenMetaStatements ? m_ThenMetaStatements->GetNextMetaStatements()->ToFormatString() : "";
    } else {
        result += m_ThenMetaStatements ? m_ThenMetaStatements->GetNextMetaStatements()->ToFormatString() : "";
    }
    
    result += "\n";
    for (int i = 0; i < m_Deep; i++) {
        result += Global::GetTabChar();
    }
    result += "}";
    result += "\n";
    
    if (m_NextMetaStatements != nullptr) {
        result += m_NextMetaStatements->ToFormatString();
    }
    result += "\n";
    
    return result;
}

// MetaWhileDoWhileStatements implementation
MetaWhileDoWhileStatements::MetaWhileDoWhileStatements(MetaBlockStatements* mbs, Compile::FileMetaConditionExpressSyntax* whileStatements) : MetaStatements(mbs) {
    m_FileMetaKeyWhileSyntax = whileStatements;
    
    if (m_FileMetaKeyWhileSyntax->GetToken() && m_FileMetaKeyWhileSyntax->GetToken()->GetType() == ETokenType::DoWhile) {
        m_IsWhile = false;
    } else {
        m_IsWhile = true;
    }
    
    Parse();
}

void MetaWhileDoWhileStatements::Parse() {
    m_ThenMetaStatements = new MetaBlockStatements(m_OwnerMetaBlockStatements, m_FileMetaKeyWhileSyntax->GetExecuteBlockSyntax());
    m_ThenMetaStatements->SetOwnerMetaStatements(this);
    
    if (m_FileMetaKeyWhileSyntax->GetConditionExpress() != nullptr) {
        MetaType* mdt = new MetaType(m_OwnerMetaBlockStatements->GetOwnerMetaClass());
        
        CreateExpressParam* cep2 = new CreateExpressParam();
        cep2->SetOwnerMBS(m_OwnerMetaBlockStatements);
        cep2->SetMetaType(mdt);
        cep2->SetFme(m_FileMetaKeyWhileSyntax->GetConditionExpress());
        cep2->SetIsStatic(false);
        cep2->SetIsConst(false);
        cep2->SetParseFrom(EParseFrom::StatementRightExpress);
        
        m_ConditionExpress = ExpressManager::CreateExpressNode(*cep2);
    }
    
    MetaMemberFunction::CreateMetaSyntax(m_FileMetaKeyWhileSyntax->GetExecuteBlockSyntax(), m_ThenMetaStatements);
    
    if (m_ConditionExpress != nullptr) {
        AllowUseSettings* auc = new AllowUseSettings();
        m_ConditionExpress->Parse(auc);
        m_ConditionExpress->CalcReturnType();
    }
}

void MetaWhileDoWhileStatements::SetDeep(int dp) {
    if (m_ThenMetaStatements != nullptr) {
        m_ThenMetaStatements->SetDeep(dp);
    }
    if (m_NextMetaStatements != nullptr) {
        m_NextMetaStatements->SetDeep(dp);
    }
}

std::string MetaWhileDoWhileStatements::ToFormatString() const {
    std::string result;
    
    std::string sb2;
    // 处理条件表达式的格式化逻辑
    
    result += "{";
    result += "\n";
    
    if (m_IsWhile) {
        if (!sb2.empty()) {
            result += sb2;
        }
    }
    
    if (m_ThenMetaStatements && m_ThenMetaStatements->GetNextMetaStatements() != nullptr) {
        result += m_ThenMetaStatements->GetNextMetaStatements()->ToFormatString();
        result += "\n";
    }
    
    if (!m_IsWhile) {
        if (!sb2.empty()) {
            result += sb2;
            result += "\n";
        }
    }
    
    result += "}";
    result += "\n";
    
    if (m_NextMetaStatements != nullptr) {
        result += m_NextMetaStatements->ToFormatString();
    }
    result += "\n";
    
    return result;
}

} // namespace Core
} // namespace SimpleLanguage