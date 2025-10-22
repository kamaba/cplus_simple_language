//****************************************************************************
//  File:      MetaBreakContinueGoStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "MetaStatements.h"
#include "../MetaForStatements.h"
#include "../MetaWhileDoWhileStatements.h"
#include "../MetaFunction.h"
#include "../Compile/CoreFileMeta/FileMetaKeyOnlySyntax.h"
#include "../Compile/CoreFileMeta/FileMetaKeyGotoLabelSyntax.h"
#include "../Compile/Token.h"
#include "../Global.h"
#include <string>

namespace SimpleLanguage {
namespace Core {
namespace Statements {

class MetaBreakStatements : public MetaStatements {
private:
    FileMetaKeyOnlySyntax* m_FileMetaKeyOnlySyntax = nullptr;
    MetaForStatements* m_ForStatements = nullptr;
    MetaWhileDoWhileStatements* m_WhileStatements = nullptr;

public:
    MetaBreakStatements(MetaBlockStatements* mbs, FileMetaKeyOnlySyntax* fmkos) : MetaStatements(mbs) {
        m_FileMetaKeyOnlySyntax = fmkos;
        
        auto fwd = mbs->FindNearestMetaForStatementsOrMetaWhileOrDoWhileStatements();
        if (auto forStmt = dynamic_cast<MetaForStatements*>(fwd)) {
            m_ForStatements = forStmt;
        } else if (auto whileStmt = dynamic_cast<MetaWhileDoWhileStatements*>(fwd)) {
            m_WhileStatements = whileStmt;
        }
    }
    
    virtual std::string ToFormatString() override {
        std::string result;
        for (int i = 0; i < m_Deep; i++) {
            result += Global::GetTabChar();
        }
        result += "break;";
        return result;
    }
};

class MetaContinueStatements : public MetaStatements {
private:
    FileMetaKeyOnlySyntax* m_FileMetaKeyOnlySyntax = nullptr;
    MetaForStatements* m_ForStatements = nullptr;
    MetaWhileDoWhileStatements* m_WhileStatements = nullptr;

public:
    MetaContinueStatements(MetaBlockStatements* mbs, FileMetaKeyOnlySyntax* fmkos) : MetaStatements(mbs) {
        m_FileMetaKeyOnlySyntax = fmkos;
        
        auto fwd = mbs->FindNearestMetaForStatementsOrMetaWhileOrDoWhileStatements();
        if (auto forStmt = dynamic_cast<MetaForStatements*>(fwd)) {
            m_ForStatements = forStmt;
        } else if (auto whileStmt = dynamic_cast<MetaWhileDoWhileStatements*>(fwd)) {
            m_WhileStatements = whileStmt;
        }
    }
    
    virtual std::string ToFormatString() override {
        std::string result;
        for (int i = 0; i < m_Deep; i++) {
            result += Global::GetTabChar();
        }
        result += "continue;";
        return result;
    }
};

class MetaGotoLabelStatements : public MetaStatements {
public:
    bool isLabel = true;
    LabelData* labelData = nullptr;
    Token* labelToken = nullptr;
    FileMetaKeyGotoLabelSyntax* m_FileMetaKeyGotoLabelSyntax = nullptr;

public:
    MetaGotoLabelStatements(MetaBlockStatements* mbs, FileMetaKeyGotoLabelSyntax* labelSyntax) : MetaStatements(mbs) {
        m_FileMetaKeyGotoLabelSyntax = labelSyntax;
        labelToken = labelSyntax->GetLabelToken();
        
        if (m_FileMetaKeyGotoLabelSyntax->GetToken()->GetType() == ETokenType::Goto) {
            isLabel = false;
        }
        
        MetaFunction* mf = m_OwnerMetaBlockStatements->GetOwnerMetaFunction();
        if (mf != nullptr) {
            std::string labelName = labelToken->GetLexeme()->ToString();
            labelData = mf->GetLabelDataById(labelName);
            if (labelData == nullptr) {
                if (isLabel) {
                    labelData = mf->AddLabelData(labelName, m_NextMetaStatements);
                } else {
                    std::cout << "Error 使用goto跳转，必须在本函数中已有定义!!" << std::endl;
                    return;
                }
            }
        }
    }
    
    virtual void SetDeep(int dp) override {
        m_Deep = dp;
        if (m_NextMetaStatements != nullptr) {
            m_NextMetaStatements->SetDeep(dp);
        }
    }
    
    virtual std::string ToFormatString() override {
        std::string result;
        for (int i = 0; i < m_Deep; i++) {
            result += Global::GetTabChar();
        }
        result += isLabel ? "label " : "goto ";
        if (labelData != nullptr) {
            result += labelData->GetLabel()->ToString() + ";";
        }
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
