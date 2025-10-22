//****************************************************************************
//  File:      MetaBreakContinueGoStatements.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#include "MetaBreakContinueGoStatements.h"
#include "../MetaForStatements.h"
#include "../MetaWhileDoWhileStatements.h"
#include "../MetaFunction.h"
#include "../Compile/CoreFileMeta/FileMetaKeyOnlySyntax.h"
#include "../Compile/CoreFileMeta/FileMetaKeyGotoLabelSyntax.h"
#include "../Compile/Token.h"
#include "../Global.h"
#include <iostream>

namespace SimpleLanguage {
namespace Core {
namespace Statements {

MetaBreakStatements::MetaBreakStatements(MetaBlockStatements* mbs, FileMetaKeyOnlySyntax* fmkos) : MetaStatements(mbs) {
    m_FileMetaKeyOnlySyntax = fmkos;
    
    auto fwd = mbs->FindNearestMetaForStatementsOrMetaWhileOrDoWhileStatements();
    if (auto forStmt = dynamic_cast<MetaForStatements*>(fwd)) {
        m_ForStatements = forStmt;
    } else if (auto whileStmt = dynamic_cast<MetaWhileDoWhileStatements*>(fwd)) {
        m_WhileStatements = whileStmt;
    }
}

std::string MetaBreakStatements::ToFormatString() {
    std::string result;
    for (int i = 0; i < m_Deep; i++) {
        result += Global::GetTabChar();
    }
    result += "break;";
    return result;
}

MetaContinueStatements::MetaContinueStatements(MetaBlockStatements* mbs, FileMetaKeyOnlySyntax* fmkos) : MetaStatements(mbs) {
    m_FileMetaKeyOnlySyntax = fmkos;
    
    auto fwd = mbs->FindNearestMetaForStatementsOrMetaWhileOrDoWhileStatements();
    if (auto forStmt = dynamic_cast<MetaForStatements*>(fwd)) {
        m_ForStatements = forStmt;
    } else if (auto whileStmt = dynamic_cast<MetaWhileDoWhileStatements*>(fwd)) {
        m_WhileStatements = whileStmt;
    }
}

std::string MetaContinueStatements::ToFormatString() {
    std::string result;
    for (int i = 0; i < m_Deep; i++) {
        result += Global::GetTabChar();
    }
    result += "continue;";
    return result;
}

MetaGotoLabelStatements::MetaGotoLabelStatements(MetaBlockStatements* mbs, FileMetaKeyGotoLabelSyntax* labelSyntax) : MetaStatements(mbs) {
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

void MetaGotoLabelStatements::SetDeep(int dp) {
    m_Deep = dp;
    if (m_NextMetaStatements != nullptr) {
        m_NextMetaStatements->SetDeep(dp);
    }
}

std::string MetaGotoLabelStatements::ToFormatString() {
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

} // namespace Statements
} // namespace Core
} // namespace SimpleLanguage
