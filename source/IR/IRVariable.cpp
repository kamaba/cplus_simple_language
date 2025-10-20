//****************************************************************************
//  File:      IRVariable.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/30 12:00:00
//  Description: 
//****************************************************************************

#include "IRVariable.h"
#include "IRMetaType.h"
#include "IRMetaClass.h"
#include "IRMethod.h"
#include "IRManager.h"
#include "../Core/MetaVariable.h"
#include "../Core/MetaType.h"
#include "../Debug/Log.h"
#include <sstream>
#include <cassert>

namespace SimpleLanguage {
namespace IR {

IRLoadVariable* IRLoadVariable::CreateLoadVariable(IRMetaType* irmt, IRMetaClass* irmc, IRMethod* _irMethod, MetaVariable* mv) {
    IRMetaVariable* irmv = nullptr;
    if (mv->variableFrom == MetaVariable::EVariableFrom::Global) {
        IRLoadVariable* irVar = new IRLoadVariable(irmt, _irMethod, mv->GetHashCode(), IRMetaVariableFrom::Global);
        return irVar;
    } else if (mv->variableFrom == MetaVariable::EVariableFrom::Argument) {
        int id = mv->GetHashCode();
        irmv = _irMethod->GetIRArgumentById(id);
        assert(irmv != nullptr);
        IRLoadVariable* irVar = new IRLoadVariable(irmt, _irMethod, irmv->index, IRMetaVariableFrom::Argument);
        return irVar;
    } else if (mv->variableFrom == MetaVariable::EVariableFrom::Member) {
        int index = -1;
        if (irmc != nullptr) {
            index = irmc->GetMetaMemberVariableIndexByHashCode(mv->GetHashCode());
        }
        if (index == -1) {
            Log::AddGenIR(EError::None, "没有找到对应成员变量的Index");
            return nullptr;
        }
        if (mv->isStatic) {
            if (mv->metaDefineType->IsIncludeTemplate()) {
                IRLoadVariable* irVar = new IRLoadVariable(irmt, _irMethod, index, IRMetaVariableFrom::Static);
                return irVar;
            } else {
                IRLoadVariable* irVar = new IRLoadVariable(irmt, _irMethod, mv->GetHashCode(), IRMetaVariableFrom::Global);
                return irVar;
            }
        } else {
            IRLoadVariable* irVar = new IRLoadVariable(irmt, _irMethod, index, IRMetaVariableFrom::Member);
            return irVar;
        }
    } else {
        irmv = _irMethod->GetIRLocalVariableById(mv->GetHashCode());
        assert(irmv != nullptr);
        IRLoadVariable* irVar = new IRLoadVariable(irmt, _irMethod, irmv->index, IRMetaVariableFrom::LocalStatement);
        return irVar;
    }
}

IRLoadVariable::IRLoadVariable(IRMetaType* irmt, IRMethod* _irMethod, int id, IRMetaVariableFrom irmvf) : IRBase(_irMethod) {
    m_Data = new IRData();
    
    if (irmvf == IRMetaVariableFrom::Global) {
        m_Data->opCode = EIROpCode::LocalGlobal;
        m_Data->index = id;
        m_IRDataList.push_back(m_Data);
    } else if (irmvf == IRMetaVariableFrom::Argument) {
        m_Data->opCode = EIROpCode::LoadArgument;
        m_Data->index = id;
        m_IRDataList.push_back(m_Data);
    } else if (irmvf == IRMetaVariableFrom::Member) {
        m_Data->index = id;
        m_Data->opCode = EIROpCode::LoadNotStaticField;
        m_IRDataList.push_back(m_Data);
    } else if (irmvf == IRMetaVariableFrom::LocalStatement) {
        m_Data->opCode = EIROpCode::LoadLocal;
        m_Data->index = id;
        m_IRDataList.push_back(m_Data);
    } else if (irmvf == IRMetaVariableFrom::Static) {
        m_Data->opValue = irmt;
        m_Data->opCode = EIROpCode::LoadStaticField;
        m_Data->index = id;
        m_IRDataList.push_back(m_Data);
    } else {
        Log::AddVM(EError::None, "SVM Error 没有找到加载变量的来源类型！");
    }
}

std::string IRLoadVariable::ToIRString() {
    std::ostringstream sb;
    sb << "#LoadVariable#" << std::endl;
    for (size_t i = 0; i < m_IRDataList.size(); i++) {
        sb << m_IRDataList[i]->ToString() << std::endl;
    }
    return sb.str();
}

IRStoreVariable* IRStoreVariable::CreateIRStoreVariable(IRMetaType* irmt, IRMetaClass* irmc, IRMethod* _irMethod, MetaVariable* mv) {
    IRMetaVariable* irmv = nullptr;
    if (mv->variableFrom == MetaVariable::EVariableFrom::Argument) {
        irmv = _irMethod->GetIRArgumentById(mv->GetHashCode());
        IRStoreVariable* irsv = new IRStoreVariable(irmt, _irMethod, irmv->index, IRMetaVariableFrom::Argument);
        return irsv;
    } else if (mv->variableFrom == MetaVariable::EVariableFrom::LocalStatement) {
        irmv = _irMethod->GetIRLocalVariableById(mv->GetHashCode());
        IRStoreVariable* irsv = new IRStoreVariable(irmt, _irMethod, irmv->index, IRMetaVariableFrom::LocalStatement);
        return irsv;
    } else if (mv->variableFrom == MetaVariable::EVariableFrom::Member) {
        int index = -1;
        auto cirmc = irmc == nullptr ? irmt->irMetaClass : irmc;
        if (cirmc != nullptr) {
            index = cirmc->GetMetaMemberVariableIndexByHashCode(mv->GetHashCode());
        }
        if (mv->isStatic) {
            if (mv->metaDefineType->IsIncludeTemplate()) {
                IRStoreVariable* irsv = new IRStoreVariable(irmt, _irMethod, index, IRMetaVariableFrom::Static);
                return irsv;
            } else {
                IRStoreVariable* irsv = new IRStoreVariable(irmt, _irMethod, mv->GetHashCode(), IRMetaVariableFrom::Global);
                return irsv;
            }
        } else {
            IRStoreVariable* irsv = new IRStoreVariable(irmt, _irMethod, index, IRMetaVariableFrom::Member);
            return irsv;
        }
    } else if (mv->variableFrom == MetaVariable::EVariableFrom::Global) {
        IRStoreVariable* irsv = new IRStoreVariable(irmt, _irMethod, mv->GetHashCode(), IRMetaVariableFrom::Global);
        return irsv;
    } else {
        assert(false);
    }
    return nullptr;
}

IRStoreVariable::IRStoreVariable(IRMetaType* irmt, IRMethod* _irMethod, int id, IRMetaVariableFrom irmvf) : IRBase(_irMethod) {
    m_Data = new IRData();
    
    if (irmvf == IRMetaVariableFrom::Global) {
        m_Data->index = id;
        m_Data->opCode = EIROpCode::StoreGlobal;
        m_IRDataList.push_back(m_Data);
    } else if (irmvf == IRMetaVariableFrom::Static) {
        m_Data->opValue = irmt;
        m_Data->opCode = EIROpCode::StoreStaticField;
        m_Data->index = id;
        AddIRData(m_Data);
    } else if (irmvf == IRMetaVariableFrom::Member) {
        m_Data->index = id;
        m_Data->opCode = EIROpCode::StoreNotStaticField2;
        m_IRDataList.push_back(m_Data);
    } else if (irmvf == IRMetaVariableFrom::Argument) {
        m_Data->opCode = EIROpCode::StoreLocal;
        m_Data->index = id;
        m_IRDataList.push_back(m_Data);
    } else if (irmvf == IRMetaVariableFrom::LocalStatement) {
        m_Data->opCode = EIROpCode::StoreLocal;
        m_Data->index = id;
        m_IRDataList.push_back(m_Data);
    } else {
        Log::AddVM(EError::None, "SVM Error 没有找到加载变量的来源类型！");
    }
}

IRStoreVariable::IRStoreVariable() {
}

IRStoreVariable* IRStoreVariable::CreateStaticReturnIRSV() {
    IRStoreVariable* irsv = new IRStoreVariable();
    IRData* storeNode = new IRData();
    storeNode->opCode = EIROpCode::StoreReturn;
    storeNode->index = 0;
    irsv->m_IRDataList.push_back(storeNode);
    return irsv;
}

std::string IRStoreVariable::ToIRString() {
    std::ostringstream sb;
    sb << "#StoreVariable#" << std::endl;
    for (size_t i = 0; i < m_IRDataList.size(); i++) {
        sb << m_IRDataList[i]->ToString() << std::endl;
    }
    return sb.str();
}

} // namespace IR
} // namespace SimpleLanguage
