//****************************************************************************
//  File:      IRBlockStatements.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/12 12:00:00
//  Description: 
//****************************************************************************

#include "IRBlockStatements.h"
#include "../IRNop.h"
#include "IRDefineVarStatements.h"
#include "IRAssignStatements.h"
#include "IRBreakStatements.h"
#include "IRContinueStatements.h"
#include "IRGotoLabelStatements.h"
#include "IRIfStatements.h"
#include "IRReturnStatements.h"
#include "IRSwitchStatements.h"
#include "IRForStatements.h"
#include "IRWhileDoWhileStatements.h"
#include "IRCallStatements.h"
#include "../Core/Statements/MetaBlockStatements.h"
#include "../Core/Statements/MetaDefineVarStatements.h"
#include "../Core/Statements/MetaAssignStatements.h"
#include "../Core/Statements/MetaBreakStatements.h"
#include "../Core/Statements/MetaContinueStatements.h"
#include "../Core/Statements/MetaGotoLabelStatements.h"
#include "../Core/Statements/MetaIfStatements.h"
#include "../Core/Statements/MetaReturnStatements.h"
#include "../Core/Statements/MetaSwitchStatements.h"
#include "../Core/Statements/MetaForStatements.h"
#include "../Core/Statements/MetaWhileDoWhileStatements.h"
#include "../Core/Statements/MetaCallStatements.h"
#include "../Core/Statements/MetaOtherPlatformStatements.h"
#include <iostream>

namespace SimpleLanguage {
namespace IR {

IRBlockStatements::IRBlockStatements(IRMethod* irmthod) {
    irMethod = irmthod;
    m_BlockStart = new IRNop(this->irMethod);
    m_IRStatements.push_back(m_BlockStart);
}

void IRBlockStatements::ParseAllIRStatements(MetaBlockStatements* ms) {
    MetaStatements* nextmbs = ms->nextMetaStatements;
    while (nextmbs != nullptr) {
        if (auto mbs = dynamic_cast<MetaBlockStatements*>(nextmbs)) {
            IRBlockStatements* ibs = new IRBlockStatements(irMethod);
            ibs->ParseAllIRStatements(mbs);
            m_IRStatements.insert(m_IRStatements.end(), ibs->m_IRStatements.begin(), ibs->m_IRStatements.end());
        } else if (auto mns = dynamic_cast<MetaDefineVarStatements*>(nextmbs)) {
            IRDefineVarStatements* mirns = new IRDefineVarStatements(irMethod);
            mirns->ParseIRStatements(mns);
            m_IRStatements.insert(m_IRStatements.end(), mirns->GetIRStatements().begin(), mirns->GetIRStatements().end());
        } else if (auto mas = dynamic_cast<MetaAssignStatements*>(nextmbs)) {
            IRAssignStatements* miras = new IRAssignStatements(irMethod);
            miras->ParseIRStatements(mas);
            m_IRStatements.insert(m_IRStatements.end(), miras->GetIRStatements().begin(), miras->GetIRStatements().end());
        } else if (auto mbreaks = dynamic_cast<MetaBreakStatements*>(nextmbs)) {
            IRBreakStatements* mirbs = new IRBreakStatements(irMethod);
            mirbs->ParseIRStatements(mbreaks);
            m_IRStatements.insert(m_IRStatements.end(), mirbs->GetIRStatements().begin(), mirbs->GetIRStatements().end());
        } else if (auto mcs = dynamic_cast<MetaContinueStatements*>(nextmbs)) {
            IRContinueStatements* mircs = new IRContinueStatements(irMethod);
            mircs->ParseIRStatements(mcs);
            m_IRStatements.insert(m_IRStatements.end(), mircs->GetIRStatements().begin(), mircs->GetIRStatements().end());
        } else if (auto mgls = dynamic_cast<MetaGotoLabelStatements*>(nextmbs)) {
            IRGotoLabelStatements* mirgls = new IRGotoLabelStatements(irMethod);
            mirgls->ParseIRStatements(mgls);
            m_IRStatements.insert(m_IRStatements.end(), mirgls->GetIRStatements().begin(), mirgls->GetIRStatements().end());
        } else if (auto mif = dynamic_cast<MetaIfStatements*>(nextmbs)) {
            IRIfStatements* mirif = new IRIfStatements(irMethod);
            mirif->ParseIRStatements(mif);
            m_IRStatements.insert(m_IRStatements.end(), mirif->GetIRStatements().begin(), mirif->GetIRStatements().end());
        } else if (auto mirrs = dynamic_cast<MetaReturnStatements*>(nextmbs)) {
            IRReturnStatements* mirrss = new IRReturnStatements(irMethod);
            mirrss->ParseIRStatements(mirrs);
            m_IRStatements.insert(m_IRStatements.end(), mirrss->GetIRStatements().begin(), mirrss->GetIRStatements().end());
        } else if (auto mswitchs = dynamic_cast<MetaSwitchStatements*>(nextmbs)) {
            IRSwitchStatements* mirss = new IRSwitchStatements(irMethod);
            mirss->ParseIRStatements(mswitchs);
            m_IRStatements.insert(m_IRStatements.end(), mirss->GetIRStatements().begin(), mirss->GetIRStatements().end());
        } else if (auto mfors = dynamic_cast<MetaForStatements*>(nextmbs)) {
            IRForStatements* mirfors = new IRForStatements(irMethod);
            mirfors->ParseIRStatements(mfors);
            m_IRStatements.insert(m_IRStatements.end(), mirfors->GetIRStatements().begin(), mirfors->GetIRStatements().end());
        } else if (auto mwdws = dynamic_cast<MetaWhileDoWhileStatements*>(nextmbs)) {
            IRWhileDoWhileStatements* mirwdws = new IRWhileDoWhileStatements(irMethod);
            mirwdws->ParseIRStatements(mwdws);
            m_IRStatements.insert(m_IRStatements.end(), mirwdws->GetIRStatements().begin(), mirwdws->GetIRStatements().end());
        } else if (auto mcs2 = dynamic_cast<MetaCallStatements*>(nextmbs)) {
            IRCallStatements* ircs = new IRCallStatements(irMethod);
            ircs->ParseIRStatements(mcs2);
            m_IRStatements.insert(m_IRStatements.end(), ircs->GetIRStatements().begin(), ircs->GetIRStatements().end());
        } else if (auto mops = dynamic_cast<MetaOtherPlatformStatements*>(nextmbs)) {
            std::cout << "------------------没有解析IR的语句类型------------" << std::endl;
        } else {
            std::cout << "------------------没有解析IR的语句类型------------" << std::endl;
        }
        nextmbs = nextmbs->nextMetaStatements;
    }
}

} // namespace IR
} // namespace SimpleLanguage