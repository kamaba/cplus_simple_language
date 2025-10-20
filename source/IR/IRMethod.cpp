//****************************************************************************
//  File:      IRMethod.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#include "IRMethod.h"
#include "IRManager.h"
#include "IRMetaVariable.h"
#include "IRBlockStatements.h"
#include "../Core/MetaFunction.h"
#include "../Core/MetaVariable.h"
#include <algorithm>
#include <sstream>
#include <iostream>

namespace SimpleLanguage {
namespace IR {

IRMethod::IRMethod(IRManager* irma, MetaFunction* func) : IRBase() {
    irManager = irma;
    m_BindMetaFunction = func;
    this->id = func->functionAllName;
    this->virtualFunctionName = func->virtualFunctionName;
}

void IRMethod::Parse() {
    auto mf = m_BindMetaFunction;
    auto id2 = this->id;
    auto vfn = mf->virtualFunctionName;

    if (mf->thisMetaVariable != nullptr) {
        IRMetaVariable* imp = new IRMetaVariable(mf->thisMetaVariable, 0);
        m_MethodArgumentList.push_back(imp);
    }
    if (mf->returnMetaVariable != nullptr) {
        IRMetaVariable* imp = new IRMetaVariable(mf->returnMetaVariable, 0);
        m_MethodReturnList.push_back(imp);
    }
    
    auto list2 = mf->metaMemberParamCollection->metaDefineParamList;
    for (size_t i = 0; i < list2.size(); i++) {
        MetaDefineParam* mdp = list2[i];
        if (mdp == nullptr) continue;
        auto tmv = mdp->metaVariable;
        IRMetaVariable* imp = new IRMetaVariable(tmv, static_cast<int>(m_MethodArgumentList.size()));
        m_MethodArgumentList.push_back(imp);
    }

    auto list = mf->GetCalcMetaVariableList();
    for (size_t i = 0; i < list.size(); i++) {
        auto irsd = new IRMetaVariable(list[i], static_cast<int>(m_MethodLocalVariableList.size()));
        m_MethodLocalVariableList.push_back(irsd);
    }

    auto mmf = mf;
    MetaBlockStatements* mbs = mmf->metaBlockStatements;
    if (mbs == nullptr) {
        std::cout << "----------------  Info 空函数!! --------------------" << std::endl;
        return;
    }
    
    IRBlockStatements* irbs = new IRBlockStatements(this);
    irbs->ParseAllIRStatements(mbs);
    
    for (size_t i = 0; i < irbs->irStatements.size(); i++) {
        for (size_t j = 0; j < irbs->irStatements[i]->GetIRDataList().size(); j++) {
            auto addIR = irbs->irStatements[i]->GetIRDataList()[j];
            addIR->id = static_cast<int>(m_IRDataList.size());
            AddLabelDict(addIR);
            m_IRDataList.push_back(addIR);
        }
    }

    for (size_t i = 0; i < m_LabelList.size(); i++) {
        auto defLabel = m_LabelList[i];
        switch (defLabel->opCode) {
            case EIROpCode::BrLabel: {
                auto findLabel = std::find_if(m_LabelList.begin(), m_LabelList.end(), 
                    [defLabel](IRData* a) { return a->opValue == defLabel->opValue; });
                if (findLabel != m_LabelList.end()) {
                    defLabel->opValue = *findLabel;
                }
                break;
            }
            case EIROpCode::Br: {
                auto findex = std::find(m_IRDataList.begin(), m_IRDataList.end(), defLabel->opValue);
                if (findex != m_IRDataList.end()) {
                    defLabel->index = static_cast<int>(std::distance(m_IRDataList.begin(), findex));
                }
                break;
            }
            case EIROpCode::BrFalse: {
                auto findex = std::find(m_IRDataList.begin(), m_IRDataList.end(), defLabel->opValue);
                if (findex != m_IRDataList.end()) {
                    defLabel->index = static_cast<int>(std::distance(m_IRDataList.begin(), findex));
                }
                break;
            }
            case EIROpCode::BrTrue: {
                auto findex = std::find(m_IRDataList.begin(), m_IRDataList.end(), defLabel->opValue);
                if (findex != m_IRDataList.end()) {
                    defLabel->index = static_cast<int>(std::distance(m_IRDataList.begin(), findex));
                }
                break;
            }
        }
    }
}

void IRMethod::AddLabelDict(IRData* irdata) {
    if (irdata->opCode == EIROpCode::Label) {
        auto findlabel = std::find_if(m_LabelList.begin(), m_LabelList.end(),
            [irdata](IRData* a) { return a->opValue == irdata->opValue; });
        if (findlabel == m_LabelList.end()) {
            m_LabelList.push_back(irdata);
        }
    } else if (irdata->opCode == EIROpCode::Br ||
               irdata->opCode == EIROpCode::BrFalse ||
               irdata->opCode == EIROpCode::BrTrue) {
        m_LabelList.push_back(irdata);
    }
}

IRMetaVariable* IRMethod::GetIRLocalVariableById(int id) {
    auto it = std::find_if(m_MethodLocalVariableList.begin(), m_MethodLocalVariableList.end(),
        [id](IRMetaVariable* a) { return a->id == id; });
    return (it != m_MethodLocalVariableList.end()) ? *it : nullptr;
}

IRMetaVariable* IRMethod::GetIRArgumentById(int id) {
    auto it = std::find_if(m_MethodArgumentList.begin(), m_MethodArgumentList.end(),
        [id](IRMetaVariable* a) { return a->id == id; });
    return (it != m_MethodArgumentList.end()) ? *it : nullptr;
}

IRMetaVariable* IRMethod::GetReturnVariableById(int id) {
    auto it = std::find_if(m_MethodReturnList.begin(), m_MethodReturnList.end(),
        [id](IRMetaVariable* a) { return a->id == id; });
    return (it != m_MethodReturnList.end()) ? *it : nullptr;
}

std::string IRMethod::ToIRString() {
    std::ostringstream sb;

    for (size_t i = 0; i < m_IRDataList.size(); i++) {
        sb << i << " ";
        sb << m_IRDataList[i]->ToString();
        sb << std::endl;
    }

    return sb.str();
}

std::string IRMethod::ToString() const {
    return this->id;
}

} // namespace IR
} // namespace SimpleLanguage