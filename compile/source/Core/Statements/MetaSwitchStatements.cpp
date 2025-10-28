//****************************************************************************
//  File:      MetaSwitchStatements.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description:  Metadata Switch statements 
//****************************************************************************

#include "MetaSwitchStatements.h"
#include "../MetaVariable.h"
#include "../MetaType.h"
#include "../MetaClass.h"
#include "../MetaCallLink.h"
#include "../MetaCallNode.h"
#include "../MetaVisitCall.h"
#include "../MetaExpressNode/MetaExpressConst.h"
#include "../MetaExpressNode/MetaExpressCallLink.h"
#include "../MetaMemberFunction.h"
#include "MetaBlockStatements.h"
#include "../AllowUseSettings.h"
#include "../../Compile/FileMeta/FileMetaSyntax.h"
#include "../../Compile/Token.h"
#include "../../Define.h"
#include "../../Debug/Log.h"
#include <iostream>
#include <sstream>

using namespace SimpleLanguage::Debug;
using namespace SimpleLanguage::Compile;

namespace SimpleLanguage {
namespace Core {

// MetaNextStatements 实现
MetaSwitchStatements::MetaNextStatements::MetaNextStatements(Compile::Token* _token) {
    token = _token;
}

std::string MetaSwitchStatements::MetaNextStatements::ToFormatString() const {
    std::string result = "next;";
    if (m_NextMetaStatements != nullptr) {
        result += m_NextMetaStatements->ToFormatString();
    }
    return result;
}

// MetaCaseStatements 实现
MetaSwitchStatements::MetaCaseStatements::MetaCaseStatements(Compile::FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax* fmkcs, MetaBlockStatements* mbs) {
    m_FileMetaKeyCaseSyntax = fmkcs;
    m_OwnerMetaBlockStatements = mbs;
    thenMetaStatements = new MetaBlockStatements(mbs, fmkcs->GetExecuteBlockSyntax());
    if (fmkcs->IsContinueNextCastSyntax()) {
        isContinueNext = true;
    }
    
    Parse();
    MetaMemberFunction::CreateMetaSyntax(fmkcs->GetExecuteBlockSyntax(), thenMetaStatements);
}

void MetaSwitchStatements::MetaCaseStatements::Parse() {
    if (m_FileMetaKeyCaseSyntax->GetDefineClassCallLink() != nullptr) {
        MetaCallLinkExpressNode* mcen = new MetaCallLinkExpressNode(m_FileMetaKeyCaseSyntax->GetDefineClassCallLink(), nullptr, nullptr, nullptr);
        AllowUseSettings auc;
        mcen->Parse(&auc);
        mcen->CalcReturnType();
        
        matchTypeClass = mcen->GetMetaCallLink()->GetFinalCallNode()->callMetaType()->GetMetaClass();
        
        if (matchTypeClass != nullptr) {
            switchCaseType = SwitchCaseType::ClassType;
        }
        
        if (m_FileMetaKeyCaseSyntax->GetVariableToken() != nullptr) {
            if (matchTypeClass == nullptr) {
                //Log::AddInStructMeta(EError::None, "Error 解析case中，前边的类型没有找到!" + m_FileMetaKeyCaseSyntax->GetVariableToken()->ToLexemeAllString());
                return;
            }
            std::string token2name = m_FileMetaKeyCaseSyntax->GetVariableToken()->ToLexemeAllString();
            if (thenMetaStatements->GetIsMetaVariable(token2name)) {
                //Log::AddInStructMeta(EError::None, "Error 已有定义变量名称!!" + m_FileMetaKeyCaseSyntax->GetVariableToken()->ToLexemeAllString());
                return;
            }
            MetaType* mdt = new MetaType(matchTypeClass);
            defineMetaVariable = new MetaVariable(token2name, EVariableFrom::LocalStatement, 
                m_OwnerMetaBlockStatements, m_OwnerMetaBlockStatements->GetOwnerMetaClass(), mdt);
            thenMetaStatements->AddMetaVariable(defineMetaVariable);
            
            switchCaseType = SwitchCaseType::ClassType;
        }
    } else {
        auto list = m_FileMetaKeyCaseSyntax->GetConstValueTokenList();
        if (list.size() > 0) {
            switchCaseType = SwitchCaseType::Const;
            for (size_t i = 0; i < list.size(); i++) {
                auto constExpress = new MetaConstExpressNode(list[i]);
                constExpressList.push_back(constExpress);
            }
        } else {
            Log::AddInStructMeta(EError::None, "Error 解析case 中，内容为空!!");
            return;
        }
    }
    
    if (switchCaseType == SwitchCaseType::None) {
        Log::AddInStructMeta(EError::None, "Error 解析Case失败!!");
        return;
    }
    thenMetaStatements->SetTRMetaVariable(m_TrMetaVariable);
    
    if (switchCaseType == SwitchCaseType::Const) {
        for (size_t i = 0; i < constExpressList.size(); i++) {
            constExpressList[i]->CalcReturnType();
        }
    } else if (switchCaseType == SwitchCaseType::ClassType) {
        // 处理类型匹配
    }
}

void MetaSwitchStatements::MetaCaseStatements::SetDeep(int dp) {
    m_Deep = dp;
    if (thenMetaStatements != nullptr) {
        thenMetaStatements->SetDeep(dp);
    }
}

void MetaSwitchStatements::MetaCaseStatements::SetMatchMetaVariable(MetaVariable* matchMV) {
    matchMetaVariable = matchMV;
}

std::string MetaSwitchStatements::MetaCaseStatements::ToFormatString() const {
    std::stringstream sb;
    
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "case ";
    
    if (switchCaseType == SwitchCaseType::Const) {
        for (size_t i = 0; i < constExpressList.size(); i++) {
            sb << constExpressList[i]->ToFormatString();
            if (i < constExpressList.size() - 1) {
                sb << ",";
            }
        }
    } else if (switchCaseType == SwitchCaseType::ClassType) {
        sb << (matchTypeClass ? matchTypeClass->GetAllClassName() : "");
        sb << " ";
        if (defineMetaVariable != nullptr) {
            sb << defineMetaVariable->GetName();
        }
    }
    
    sb << "\n";
    sb << thenMetaStatements->ToFormatString();
    if (isContinueNext) {
        sb << "next;";
    }
    
    return sb.str();
}

// MetaSwitchStatements 实现
MetaSwitchStatements::MetaSwitchStatements(MetaBlockStatements* mbs, Compile::FileMetaKeySwitchSyntax* fmkss, MetaVariable* retMv) : MetaStatements(mbs) {
    m_FileMetaKeySwitchSyntax = fmkss;
    m_TrMetaVariable = retMv;
    Parse();
}

void MetaSwitchStatements::Parse() {
    if (m_FileMetaKeySwitchSyntax->GetFileMetaVariableRef() != nullptr) {
        m_MetaCallLink = new MetaCallLink(m_FileMetaKeySwitchSyntax->GetFileMetaVariableRef(), 
            GetOwnerMetaClass(), m_OwnerMetaBlockStatements, nullptr, nullptr);
    } else {
        m_MatchType = SwitchMatchType::ClassType;
        matchMV = nullptr;
    }
    
    for (size_t i = 0; i < m_FileMetaKeySwitchSyntax->GetFileMetaKeyCaseSyntaxList().size(); i++) {
        FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax* cmcs = m_FileMetaKeySwitchSyntax->GetFileMetaKeyCaseSyntaxList()[i];
        MetaCaseStatements* mcs = new MetaCaseStatements(cmcs, m_OwnerMetaBlockStatements);
        metaCaseStatements.push_back(mcs);
    }
    
    if (m_FileMetaKeySwitchSyntax->GetDefaultExecuteBlockSyntax() != nullptr) {
        defaultMetaStatements = new MetaBlockStatements(m_OwnerMetaBlockStatements, 
            m_FileMetaKeySwitchSyntax->GetDefaultExecuteBlockSyntax());
        MetaMemberFunction::CreateMetaSyntax(m_FileMetaKeySwitchSyntax->GetDefaultExecuteBlockSyntax(), defaultMetaStatements);
    }
    
    for (size_t i = 0; i < metaCaseStatements.size(); i++) {
        metaCaseStatements[i]->SetTRMetaVariable(m_TrMetaVariable);
    }
    
    if (defaultMetaStatements != nullptr) {
        defaultMetaStatements->SetTRMetaVariable(m_TrMetaVariable);
    }
    
    if (m_MetaCallLink != nullptr) {
        AllowUseSettings* auc = new AllowUseSettings();
        auc->SetCallConstructFunction(false);
        m_MetaCallLink->Parse(auc);
        m_MetaCallLink->CalcReturnType();
        matchMV = m_MetaCallLink->ExecuteGetMetaVariable();
        auto mv = m_OwnerMetaBlockStatements->GetMetaVariableByName(matchMV->GetName());
        if (mv == matchMV) { // 如果直接调用其它地方的metavariable，需要生成一个临时的metavariable
            m_MatchType = SwitchMatchType::ConstValue;
        }
    }
    
    for (size_t i = 0; i < metaCaseStatements.size(); i++) {
        metaCaseStatements[i]->SetMatchMetaVariable(matchMV);
    }
}

void MetaSwitchStatements::SetDeep(int dp) {
    m_Deep = dp;
    for (size_t i = 0; i < metaCaseStatements.size(); i++) {
        metaCaseStatements[i]->SetDeep(dp + 1);
    }
    if (defaultMetaStatements != nullptr) {
        defaultMetaStatements->SetDeep(dp + 1);
    }
    if (m_NextMetaStatements != nullptr) {
        m_NextMetaStatements->SetDeep(dp);
    }
}

void MetaSwitchStatements::SetTRMetaVariable(MetaVariable* mv) {
    for (size_t i = 0; i < metaCaseStatements.size(); i++) {
        metaCaseStatements[i]->SetTRMetaVariable(mv);
    }
}

std::string MetaSwitchStatements::ToFormatString() const {
    std::stringstream sb;
    
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "switch ";
    
    if (m_MatchType == SwitchMatchType::None) {
        // 无匹配类型
    } else if (m_MatchType == SwitchMatchType::ConstValue) {
        sb << (matchMV ? matchMV->GetName() : "");
    } else if (m_MatchType == SwitchMatchType::ClassType) {
        sb << (matchMV ? matchMV->GetName() : "");
        sb << " ";
        sb << (m_MetaCallLink ? m_MetaCallLink->ToFormatString() : "");
    }
    
    sb << "\n";
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "{";
    
    for (size_t i = 0; i < metaCaseStatements.size(); i++) {
        sb << "\n";
        sb << metaCaseStatements[i]->ToFormatString();
    }
    
    if (defaultMetaStatements != nullptr) {
        sb << "\n";
        for (int i = 0; i < m_Deep + 1; i++) {
            sb << Global::GetTabChar();
        }
        sb << "default";
        sb << "\n";
        sb << defaultMetaStatements->ToFormatString();
    }
    
    sb << "\n\n";
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "}";
    sb << "\n";
    
    if (m_NextMetaStatements != nullptr) {
        sb << m_NextMetaStatements->ToFormatString();
    }
    
    return sb.str();
}

} // namespace Core
} // namespace SimpleLanguage