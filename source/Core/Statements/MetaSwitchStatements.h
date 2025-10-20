//****************************************************************************
//  File:      MetaSwitchStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description:  Metadata Switch statements 
//****************************************************************************

#pragma once

#include "MetaStatements.h"
#include "../MetaExpressNode/MetaExpressConst.h"
#include "../MetaCallLink.h"
#include "../AllowUseSettings.h"
#include "../Compile/CoreFileMeta/FileMetaKeySwitchSyntax.h"
#include "../Global.h"
#include <string>
#include <vector>

namespace SimpleLanguage {
namespace Core {
namespace Statements {

class MetaSwitchStatements : public MetaStatements {
public:
    enum class SwitchMatchType {
        None,
        ClassType,
        ClassValue,
        ConstValue
    };
    
    class MetaNextStatements : public MetaStatements {
    public:
        Token* token = nullptr;
        
        MetaNextStatements(Token* _token) {
            token = _token;
        }
        
        virtual std::string ToFormatString() override {
            std::string result = "next;";
            if (m_NextMetaStatements != nullptr) {
                result += m_NextMetaStatements->ToFormatString();
            }
            return result;
        }
    };
    
    class MetaCaseStatements : public MetaStatements {
    public:
        enum class SwitchCaseType {
            None,
            Const,
            ClassType
        };
        
        SwitchCaseType switchCaseType = SwitchCaseType::None;
        std::vector<MetaConstExpressNode*> constExpressList;
        MetaClass* matchTypeClass = nullptr;
        MetaVariable* matchMetaVariable = nullptr;
        MetaVariable* defineMetaVariable = nullptr;
        MetaBlockStatements* thenMetaStatements = nullptr;
        bool isContinueNext = false;

    private:
        FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax* m_FileMetaKeyCaseSyntax = nullptr;

    public:
        MetaCaseStatements(FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax* fmkcs, MetaBlockStatements* mbs) {
            m_FileMetaKeyCaseSyntax = fmkcs;
            m_OwnerMetaBlockStatements = mbs;
            thenMetaStatements = new MetaBlockStatements(mbs, fmkcs->GetExecuteBlockSyntax());
            if (fmkcs->IsContinueNextCastSyntax()) {
                isContinueNext = true;
            }
            
            Parse();
            MetaMemberFunction::CreateMetaSyntax(fmkcs->GetExecuteBlockSyntax(), thenMetaStatements);
        }
        
    private:
        void Parse() {
            if (m_FileMetaKeyCaseSyntax->GetDefineClassCallLink() != nullptr) {
                MetaCallLinkExpressNode* mcen = new MetaCallLinkExpressNode(m_FileMetaKeyCaseSyntax->GetDefineClassCallLink(), nullptr, nullptr, nullptr);
                mcen->Parse(new AllowUseSettings());
                mcen->CalcReturnType();
                
                matchTypeClass = mcen->GetMetaCallLink()->GetFinalCallNode()->GetCallMetaType()->GetMetaClass();
                
                if (matchTypeClass != nullptr) {
                    switchCaseType = SwitchCaseType::ClassType;
                }
                
                if (m_FileMetaKeyCaseSyntax->GetVariableToken() != nullptr) {
                    if (matchTypeClass == nullptr) {
                        std::cout << "Error 解析case中，前边的类型没有找到!" << m_FileMetaKeyCaseSyntax->GetVariableToken()->ToLexemeAllString() << std::endl;
                        return;
                    }
                    std::string token2name = m_FileMetaKeyCaseSyntax->GetVariableToken()->GetLexeme()->ToString();
                    if (thenMetaStatements->GetIsMetaVariable(token2name)) {
                        std::cout << "Error 已有定义变量名称!!" << m_FileMetaKeyCaseSyntax->GetVariableToken()->ToLexemeAllString() << std::endl;
                        return;
                    }
                    MetaType* mdt = new MetaType(matchTypeClass);
                    defineMetaVariable = new MetaVariable(token2name, MetaVariable::EVariableFrom::LocalStatement, 
                        m_OwnerMetaBlockStatements, m_OwnerMetaBlockStatements->GetOwnerMetaClass(), mdt);
                    thenMetaStatements->AddMetaVariable(defineMetaVariable);
                    
                    switchCaseType = SwitchCaseType::ClassType;
                }
            } else {
                auto list = m_FileMetaKeyCaseSyntax->GetConstValueTokenList();
                if (list.size() > 0) {
                    switchCaseType = SwitchCaseType::Const;
                    for (int i = 0; i < list.size(); i++) {
                        auto constExpress = new MetaConstExpressNode(list[i]);
                        constExpressList.push_back(constExpress);
                    }
                } else {
                    std::cout << "Error 解析case 中，内容为空!!" << std::endl;
                    return;
                }
            }
            
            if (switchCaseType == SwitchCaseType::None) {
                std::cout << "Error 解析Case失败!!" << std::endl;
                return;
            }
            thenMetaStatements->SetTRMetaVariable(m_TrMetaVariable);
            
            if (switchCaseType == SwitchCaseType::Const) {
                for (int i = 0; i < constExpressList.size(); i++) {
                    constExpressList[i]->CalcReturnType();
                }
            } else if (switchCaseType == SwitchCaseType::ClassType) {
                // 处理类型匹配
            }
        }

    public:
        virtual void SetDeep(int dp) override {
            m_Deep = dp;
            if (thenMetaStatements != nullptr) {
                thenMetaStatements->SetDeep(dp);
            }
        }
        
        void SetMatchMetaVariable(MetaVariable* matchMV) {
            matchMetaVariable = matchMV;
        }
        
        virtual std::string ToFormatString() override {
            std::string result;
            
            for (int i = 0; i < m_Deep; i++) {
                result += Global::GetTabChar();
            }
            result += "case ";
            
            if (switchCaseType == SwitchCaseType::Const) {
                for (int i = 0; i < constExpressList.size(); i++) {
                    result += constExpressList[i]->ToFormatString();
                    if (i < constExpressList.size() - 1) {
                        result += ",";
                    }
                }
            } else if (switchCaseType == SwitchCaseType::ClassType) {
                result += matchTypeClass ? matchTypeClass->GetAllClassName() : "";
                result += " ";
                if (defineMetaVariable != nullptr) {
                    result += defineMetaVariable->GetName();
                }
            }
            
            result += "\n";
            result += thenMetaStatements->ToFormatString();
            if (isContinueNext) {
                result += "next;";
            }
            
            return result;
        }
    };

private:
    SwitchMatchType m_MatchType;
    std::vector<MetaCaseStatements*> metaCaseStatements;
    MetaBlockStatements* defaultMetaStatements = nullptr;
    MetaVariable* matchMV = nullptr;
    MetaCallLink* m_MetaCallLink = nullptr;
    FileMetaKeySwitchSyntax* m_FileMetaKeySwitchSyntax = nullptr;

public:
    MetaSwitchStatements(MetaBlockStatements* mbs, FileMetaKeySwitchSyntax* fmkss, MetaVariable* retMv = nullptr) : MetaStatements(mbs) {
        m_FileMetaKeySwitchSyntax = fmkss;
        m_TrMetaVariable = retMv;
        Parse();
    }
    
private:
    void Parse() {
        if (m_FileMetaKeySwitchSyntax->GetFileMetaVariableRef() != nullptr) {
            m_MetaCallLink = new MetaCallLink(m_FileMetaKeySwitchSyntax->GetFileMetaVariableRef(), 
                GetOwnerMetaClass(), m_OwnerMetaBlockStatements, nullptr, nullptr);
        } else {
            m_MatchType = SwitchMatchType::ClassType;
            matchMV = nullptr;
        }
        
        for (int i = 0; i < m_FileMetaKeySwitchSyntax->GetFileMetaKeyCaseSyntaxList().size(); i++) {
            auto cmcs = m_FileMetaKeySwitchSyntax->GetFileMetaKeyCaseSyntaxList()[i];
            MetaCaseStatements* mcs = new MetaCaseStatements(cmcs, m_OwnerMetaBlockStatements);
            metaCaseStatements.push_back(mcs);
        }
        
        if (m_FileMetaKeySwitchSyntax->GetDefaultExecuteBlockSyntax() != nullptr) {
            defaultMetaStatements = new MetaBlockStatements(m_OwnerMetaBlockStatements, 
                m_FileMetaKeySwitchSyntax->GetDefaultExecuteBlockSyntax());
            MetaMemberFunction::CreateMetaSyntax(m_FileMetaKeySwitchSyntax->GetDefaultExecuteBlockSyntax(), defaultMetaStatements);
        }
        
        for (int i = 0; i < metaCaseStatements.size(); i++) {
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
        
        for (int i = 0; i < metaCaseStatements.size(); i++) {
            metaCaseStatements[i]->SetMatchMetaVariable(matchMV);
        }
    }

public:
    virtual void SetDeep(int dp) override {
        m_Deep = dp;
        for (int i = 0; i < metaCaseStatements.size(); i++) {
            metaCaseStatements[i]->SetDeep(dp + 1);
        }
        if (defaultMetaStatements != nullptr) {
            defaultMetaStatements->SetDeep(dp + 1);
        }
        if (m_NextMetaStatements != nullptr) {
            m_NextMetaStatements->SetDeep(dp);
        }
    }
    
    virtual void SetTRMetaVariable(MetaVariable* mv) override {
        for (int i = 0; i < metaCaseStatements.size(); i++) {
            metaCaseStatements[i]->SetTRMetaVariable(mv);
        }
    }
    
    virtual std::string ToFormatString() override {
        std::string result;
        
        for (int i = 0; i < m_Deep; i++) {
            result += Global::GetTabChar();
        }
        result += "switch ";
        
        if (m_MatchType == SwitchMatchType::None) {
            // 无匹配类型
        } else if (m_MatchType == SwitchMatchType::ConstValue) {
            result += matchMV ? matchMV->GetName() : "";
        } else if (m_MatchType == SwitchMatchType::ClassType) {
            result += matchMV ? matchMV->GetName() : "";
            result += " ";
            result += m_MetaCallLink ? m_MetaCallLink->ToFormatString() : "";
        }
        
        result += "\n";
        for (int i = 0; i < m_Deep; i++) {
            result += Global::GetTabChar();
        }
        result += "{";
        
        for (int i = 0; i < metaCaseStatements.size(); i++) {
            result += "\n";
            result += metaCaseStatements[i]->ToFormatString();
        }
        
        if (defaultMetaStatements != nullptr) {
            result += "\n";
            for (int i = 0; i < m_Deep + 1; i++) {
                result += Global::GetTabChar();
            }
            result += "default";
            result += "\n";
            result += defaultMetaStatements->ToFormatString();
        }
        
        result += "\n\n";
        for (int i = 0; i < m_Deep; i++) {
            result += Global::GetTabChar();
        }
        result += "}";
        result += "\n";
        
        if (m_NextMetaStatements != nullptr) {
            result += m_NextMetaStatements->ToFormatString();
        }
        
        return result;
    }
};

} // namespace Statements
} // namespace Core
} // namespace SimpleLanguage
