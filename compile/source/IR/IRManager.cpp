//****************************************************************************
//  File:      IRManager.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#include "IRManager.h"
#include <iostream>
#include <sstream>

namespace SimpleLanguage {
namespace IR {

IRManager* IRManager::s_Instance = nullptr;

IRManager* IRManager::GetInstance() {
    if (s_Instance == nullptr) {
        s_Instance = new IRManager();
    }
    return s_Instance;
}

EIROpCode IRManager::GetConstIROpCode(EType etype) {
    switch (etype) {
        case EType::Byte: return EIROpCode::LoadConstByte;
        case EType::SByte: return EIROpCode::LoadConstSByte;
        case EType::Boolean: return EIROpCode::LoadConstBoolean;
        case EType::Int16: return EIROpCode::LoadConstInt16;
        case EType::UInt16: return EIROpCode::LoadConstUInt16;
        case EType::Int32: return EIROpCode::LoadConstInt32;
        case EType::UInt32: return EIROpCode::LoadConstUInt32;
        case EType::Int64: return EIROpCode::LoadConstInt64;
        case EType::UInt64: return EIROpCode::LoadConstUInt64;
        case EType::Float32: return EIROpCode::LoadConstFloat;
        case EType::Float64: return EIROpCode::LoadConstDouble;
        case EType::String: return EIROpCode::LoadConstString;
        case EType::Null: return EIROpCode::LoadConstNull;
        default:
            std::cout << "Error GetConstIROpCode!!" << std::endl;
            break;
    }
    return EIROpCode::Nop;
}

void IRManager::TranslateIR() {
    ParseClass();

    // 动态解析出来的函数
    auto dynamicMmfDict4 = MethodManager::GetInstance().GetMetaDynamicFunctionList();
    for (auto v : dynamicMmfDict4) {
        IRMethod* irm = TranslateIRByFunction(v);
        AddIRMethod(irm);
    }

    ParseIRMethod();
}

IRMetaClass* IRManager::GetIRMetaClassById(int id) {
    for (auto a : m_IRMetaClassList) {
        if (a->id == id) {
            return a;
        }
    }
    return nullptr;
}

IRMetaClass* IRManager::GetIRMetaClassByName(const std::string& allname) {
    for (auto a : m_IRMetaClassList) {
        if (a->GetIrName() == allname) {
            return a;
        }
    }
    return nullptr;
}

std::string IRManager::GetIRNameByMetaClass(MetaClass* mc) {
    return mc->GetAllClassName();
}

std::string IRManager::GetIRNameByMetaType(MetaType* mt) {
    return mt->GetName();
}

void IRManager::AddIRMethod(IRMethod* irm) {
    IRMethodDict[irm->GetId()] = irm;
}

void IRManager::AddGlobalMetaMemberVariable(IRMetaVariable* irmv) {
    m_GlobalStaticVariableList.push_back(irmv);
}

IRMethod* IRManager::TranslateIRByFunction(MetaFunction* mf) {
    return new IRMethod(this, mf);
}

void IRManager::ParseClass() {
    // 解析类的实现
}

void IRManager::ParseIRMethod() {
    // 解析IR方法的实现
}

void IRManager::AddIRData(IRData* irdata) {
    if (irdata != nullptr) {
        m_IRDataList.push_back(irdata);
    }
}

void IRManager::AddIRDataList(const std::vector<IRData*>& list) {
    for (auto data : list) {
        if (data != nullptr) {
            m_IRDataList.push_back(data);
        }
    }
}

void IRManager::PrintIR() {
    std::cout << "----------IR 开始---------------------" << std::endl;
    for (auto ld : m_IRDataList) {
        std::cout << ld->ToString() << std::endl;
    }
    std::cout << "----------IR 结束---------------------" << std::endl;
}

} // namespace IR
} // namespace SimpleLanguage
