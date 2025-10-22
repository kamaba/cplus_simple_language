//****************************************************************************
//  File:      IRIfStatements.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/13 12:00:00
//  Description: 
//****************************************************************************

#include "IRIfStatements.h"
#include "../IRExpress.h"
#include "../IRBranch.h"
#include "../IRNop.h"
#include "../IRStoreVariable.h"
#include "../IRLoadVariable.h"
#include "../IRMetaType.h"
#include "../IRMetaClass.h"
#include "../IRBlockStatements.h"
#include "../Core/Statements/MetaIfStatements.h"
#include <sstream>

namespace SimpleLanguage {
namespace IR {

IRIfStatements::IRIfStatements(IRMethod* method) {
    this->irMethod = method;
}

void IRIfStatements::MetaIRElseIfStatements::ParseIRStatements(IRMethod* _irMethod, MetaIfStatements::MetaElseIfStatements* mires) {
    startNop = new IRNop(_irMethod);
    conditionStatList.push_back(startNop);

    if (mires->ifElseState == MetaIfStatements::IfElseState::If || mires->ifElseState == MetaIfStatements::IfElseState::ElseIf) {
        m_IrExpress = new IRExpress(_irMethod, mires->finalExpress);
        conditionStatList.push_back(m_IrExpress);

        if (mires->metaAssignManager != nullptr && mires->metaAssignManager->isNeedSetMetaVariable) {
            IRMetaClass* irmc = _irMethod->irManager->GetIRMetaClassByName("S.Core.Bool");
            IRStoreVariable* storeLocal = IRStoreVariable::CreateIRStoreVariable(new IRMetaType(irmc, nullptr), irmc, _irMethod, mires->boolConditionVariable);
            conditionStatList.push_back(storeLocal);

            IRMetaType* irmt = new IRMetaType(irmc, nullptr);
            IRLoadVariable* loadLocal = IRLoadVariable::CreateLoadVariable(irmt, irmc, _irMethod, mires->boolConditionVariable);
            conditionStatList.push_back(loadLocal);
        }

        ifFalseBreach = new IRBranch(_irMethod, EIROpCode::BrFalse, nullptr);
        conditionStatList.push_back(ifFalseBreach);
    }

    IRBlockStatements* irbs = new IRBlockStatements(_irMethod);
    irbs->ParseAllIRStatements(mires->thenMetaStatements);
    thenStatList.insert(thenStatList.end(), irbs->GetIRStatements().begin(), irbs->GetIRStatements().end());

    // {}代码执行结束后的位置
    ifEndBrach = new IRBranch(_irMethod, EIROpCode::Br, nullptr);
    thenStatList.push_back(ifEndBrach);
}

std::string IRIfStatements::MetaIRElseIfStatements::ToIRString() {
    std::ostringstream sb;
    sb << "#if ";
    return sb.str();
}

void IRIfStatements::ParseIRStatements(MetaIfStatements* ifstatements) {
    IRNop* ifEndIRNop = new IRNop(irMethod);
    std::vector<MetaIRElseIfStatements*> mirList;
    
    for (size_t i = 0; i < ifstatements->metaElseIfStatements.size(); i++) {
        auto meis = ifstatements->metaElseIfStatements[i];

        MetaIRElseIfStatements* mire = new MetaIRElseIfStatements();
        mirList.push_back(mire);

        mire->ParseIRStatements(irMethod, meis);
        m_IRStatements.insert(m_IRStatements.end(), mire->conditionStatList.begin(), mire->conditionStatList.end());
        m_IRStatements.insert(m_IRStatements.end(), mire->thenStatList.begin(), mire->thenStatList.end());
        mire->ifEndBrach->data->opValue = ifEndIRNop->data;
    }
    m_IRStatements.push_back(ifEndIRNop);

    std::vector<IRData*> irdataList;
    for (size_t i = 0; i < m_IRStatements.size(); i++) {
        for (size_t j = 0; j < m_IRStatements[i]->GetIRDataList().size(); j++) {
            auto addIR = m_IRStatements[i]->GetIRDataList()[j];
            irdataList.push_back(addIR);
        }
    }

    for (size_t i = 0; i < mirList.size(); i++) {
        auto mire = mirList[i];
        if (mire->ifFalseBreach != nullptr) {
            if (i < mirList.size() - 1) {
                mire->ifFalseBreach->data->opValue = mirList[i + 1]->startNop->data;
            } else if (i == mirList.size() - 1) {
                mire->ifFalseBreach->data->opValue = ifEndIRNop->data;
            }
        }
    }
}

} // namespace IR
} // namespace SimpleLanguage