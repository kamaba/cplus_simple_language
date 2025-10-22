//****************************************************************************
//  File:      IRExpress.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description:  express convert ir code!
//****************************************************************************

#include "IRExpress.h"
#include "IRManager.h"
#include "IRMetaCallLink.h"
#include "IRNew.h"
#include "IRConvert.h"
#include "../Core/MetaExpressNode.h"
#include "../Core/MetaExpressNode/MetaExpressBase.h"
#include "../Core/MetaExpressNode/MetaExpressConst.h"
#include "../Core/MetaExpressNode/MetaExpressOperator.h"
#include "../Core/MetaExpressNode/MetaExpressCalllink.h"
#include "../Core/MetaExpressNode/MetaExpressNewObject.h"
#include "../Debug/Log.h"
#include <iostream>
#include <sstream>

namespace SimpleLanguage {
namespace IR {

IRExpress::IRExpress(IRMethod* irMethod, MetaExpressNode* node) : IRBase(irMethod) {
    CreateIRDataOne(node);
}

IRExpress::IRExpress(IRManager* _irManager, MetaExpressNode* node) : IRBase() {
    m_IRManager = _irManager;
    CreateIRDataOne(node);
}

void IRExpress::CreateIRDataOne(MetaExpressNode* node) {
    // Use dynamic_cast for type checking
    if (auto mcn = dynamic_cast<MetaConstExpressNode*>(node)) {
        IRData* irdata = new IRData();
        irdata->opCode = IRManager::GetConstIROpCode(mcn->GetEType());
        irdata->opValue = &mcn->GetValue();
        AddIRData(irdata);
    } else if (auto muoen = dynamic_cast<MetaUnaryOpExpressNode*>(node)) {
        MetaExpressNode* valNode = muoen->GetValue();
        CreateIRDataOne(valNode);
        auto signData = CreateOneSignIRData(muoen->GetOpSign());
        AddIRData(signData);
    } else if (auto moen = dynamic_cast<MetaOpExpressNode*>(node)) {
        MetaExpressNode* leftNode = moen->GetLeft();
        MetaExpressNode* rightNode = moen->GetRight();
        CreateIRDataOne(leftNode);
        if (moen->GetLeftConvert() != nullptr) {
            IRConvert* ircovn = new IRConvert(m_IRMethod, moen->GetLeftConvert()->oriType, moen->GetLeftConvert()->targetType);
            AddIRData(ircovn->data);
        }
        CreateIRDataOne(rightNode);
        if (moen->GetRightConvert() != nullptr) {
            IRConvert* ircovn = new IRConvert(m_IRMethod, moen->GetRightConvert()->oriType, moen->GetRightConvert()->targetType);
            AddIRData(ircovn->data);
        }
        auto signData = CreateLeftAndRightIRData(moen->GetOpSign());
        AddIRData(signData);
    } else if (auto mcn = dynamic_cast<MetaCallLinkExpressNode*>(node)) {
        IRMetaCallLink* irmc = new IRMetaCallLink();
        if (m_IRManager != nullptr) {
            irmc->ParseToIRDataListByIRManager(m_IRManager, mcn->GetMetaCallLink()->callNodeList);
        } else {
            irmc->ParseToIRDataList(m_IRMethod, mcn->GetMetaCallLink()->callNodeList);
        }
        for (size_t i = 0; i < irmc->irList.size(); i++) {
            m_IRDataList.insert(m_IRDataList.end(), irmc->irList[i]->GetIRDataList().begin(), irmc->irList[i]->GetIRDataList().end());
        }
    } else if (auto mnoe = dynamic_cast<MetaNewObjectExpressNode*>(node)) {
        IRMetaClass* irmc = nullptr;
        if (m_IRManager != nullptr) {
            irmc = m_IRManager->GetIRMetaClassByName(mnoe->GetReturnMetaDefineType()->name);
        } else {
            irmc = m_IRMethod->irManager->GetIRMetaClassByName(mnoe->GetReturnMetaDefineType()->name);
        }
        IRNew* irnew = new IRNew(m_IRMethod, irmc);
        m_IRDataList.insert(m_IRDataList.end(), irnew->GetIRDataList().begin(), irnew->GetIRDataList().end());
    } else {
        std::cout << "Error IR表达式错误!!" << std::endl;
    }
}

IRData* IRExpress::CreateOneSignIRData(ESingleOpSign opSign) {
    IRData* data = new IRData();
    switch (opSign) {
        case ESingleOpSign::Neg:
            data->opCode = EIROpCode::Neg;
            break;
        case ESingleOpSign::Not:
            data->opCode = EIROpCode::Not;
            break;
    }
    return data;
}

IRData* IRExpress::CreateLeftAndRightIRData(ELeftRightOpSign opSign) {
    IRData* data = new IRData();
    switch (opSign) {
        case ELeftRightOpSign::Add:
            data->opCode = EIROpCode::Add;
            break;
        case ELeftRightOpSign::Minus:
            data->opCode = EIROpCode::Minus;
            break;
        case ELeftRightOpSign::Multiply:
            data->opCode = EIROpCode::Multiply;
            break;
        case ELeftRightOpSign::Divide:
            data->opCode = EIROpCode::Divide;
            break;
        case ELeftRightOpSign::Modulo:
            data->opCode = EIROpCode::Modulo;
            break;
        case ELeftRightOpSign::InclusiveOr:
            data->opCode = EIROpCode::InclusiveOr;
            break;
        case ELeftRightOpSign::Combine:
            data->opCode = EIROpCode::Combine;
            break;
        case ELeftRightOpSign::XOR:
            data->opCode = EIROpCode::XOR;
            break;
        case ELeftRightOpSign::Shi:
            data->opCode = EIROpCode::Shi;
            break;
        case ELeftRightOpSign::Shr:
            data->opCode = EIROpCode::Shr;
            break;
        case ELeftRightOpSign::Equal:
            data->opCode = EIROpCode::Ceq;
            break;
        case ELeftRightOpSign::Greater:
            data->opCode = EIROpCode::Cgt;
            break;
        case ELeftRightOpSign::GreaterOrEqual:
            data->opCode = EIROpCode::Cge;
            break;
        case ELeftRightOpSign::Less:
            data->opCode = EIROpCode::Clt;
            break;
        case ELeftRightOpSign::LessOrEqual:
            data->opCode = EIROpCode::Cle;
            break;
        case ELeftRightOpSign::Or:
            data->opCode = EIROpCode::Or;
            break;
        case ELeftRightOpSign::And:
            data->opCode = EIROpCode::And;
            break;
        default:
            std::cout << "Error 未支持表达式中的IR代码" << static_cast<int>(opSign) << std::endl;
            break;
    }
    return data;
}

std::string IRExpress::ToIRString() {
    std::ostringstream sb;
    sb << "#LoadConst#" << std::endl;
    sb << IRBase::ToIRString();
    return sb.str();
}

} // namespace IR
} // namespace SimpleLanguage
