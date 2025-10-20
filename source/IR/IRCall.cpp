//****************************************************************************
//  File:      IRCall.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#include "IRCall.h"
#include "IRMethod.h"
#include "IRMetaType.h"
#include "IRMetaClass.h"
#include "IRMethodCall.h"
#include "IRLoadVariable.h"
#include "IRExpress.h"
#include "IRPop.h"
#include "IRManager.h"
#include "../Core/MetaMethodCall.h"
#include "../Core/MetaMemberFunctionCSharp.h"
#include "../Core/MetaFunction.h"
#include "../Core/MetaGenTemplateClass.h"
#include "../Core/SelfMeta/CoreMetaClassManager.h"
#include "../Debug/Log.h"
#include <iostream>
#include <vector>

namespace SimpleLanguage {
namespace IR {

IRCallFunction::IRCallFunction(IRMethod* _irMethod) : IRBase(_irMethod) {
}

void IRCallFunction::Parse(MetaMethodCall* mfc) {
    IRMetaType* irmt = nullptr;
    IRMetaClass* irmc = nullptr;
    
    if (mfc->loadMetaVariable != nullptr) {
        irmt = new IRMetaType(mfc->loadMetaVariable->metaDefineType);
        irmc = IRManager::instance->GetIRMetaClassById(mfc->loadMetaVariable->ownerMetaClass->GetHashCode());
        IRLoadVariable* irload = IRLoadVariable::CreateLoadVariable(irmt, irmc, m_IRMethod, mfc->loadMetaVariable);
        AddIRRangeData(irload->GetIRDataList());
    }
    
    paramCount = static_cast<int>(mfc->metaInputParamList.size());
    for (int j = 0; j < paramCount; j++) {
        IRExpress* irexpress = new IRExpress(m_IRMethod, mfc->metaInputParamList[j]);
        AddIRRangeData(irexpress->GetIRDataList());
    }
    
    MetaFunction* mf = mfc->GetTemplateMemberFunction();
    MetaMemberFunctionCSharp* mmf = dynamic_cast<MetaMemberFunctionCSharp*>(mf);
    if (mmf != nullptr) {
        m_MethodInfo = mmf->methodInfo;
        IRData* data = new IRData();
        data->opCode = EIROpCode::CallCSharpMethod;
        data->opValue = this;
        data->SetDebugInfoByToken(mmf->GetToken());
        AddIRData(data);
        return;
    }

    int callMethodIndex = -1;

    if (mf->isStatic) {
        auto scmc = mfc->staticCallerMetaClass;
        if (scmc != nullptr && dynamic_cast<MetaGenTemplateClass*>(scmc) != nullptr) {
            auto mgtc = dynamic_cast<MetaGenTemplateClass*>(scmc);
            scmc = mgtc->metaTemplateClass;
        }
        irmc = IRManager::instance->GetIRMetaClassById(scmc->GetHashCode());
    } else {
        auto irname = IRManager::GetIRNameByMetaClass(mf->ownerMetaClass);
        irmc = IRManager::instance->GetIRMetaClassByName(irname);
    }

    if (!mf->isStatic) {
        m_IRRuntimeMethod = irmc->GetIRNonStaticMethodIndexByMethod(mf->virtualFunctionName, &callMethodIndex);
    } else {
        m_IRRuntimeMethod = m_IRMethod->irManager->GetIRMethod(mf->functionAllName);
    }
    
    std::vector<IRMetaType*> types;
    for (size_t i = 0; i < mfc->staticMetaClassInputTemplateList.size(); i++) {
        types.push_back(new IRMetaType(mfc->staticMetaClassInputTemplateList[i]));
    }
    irmt = new IRMetaType(irmc, types);
    
    std::vector<IRMetaType*> functionMtList;
    for (size_t i = 0; i < mfc->metaFunctionInputTemplateList.size(); i++) {
        functionMtList.push_back(new IRMetaType(mfc->metaFunctionInputTemplateList[i]));
    }
    
    auto irmethodcall = new IRMethodCall(irmt, functionMtList, m_IRRuntimeMethod, paramCount);
    
    if (callMethodIndex == -1) {
        if (m_IRRuntimeMethod == nullptr) {
            Log::AddVM(EError::None, "------------没有找到调用的方法体!!");
            return;
        }

        if (mf->isStatic) {
            IRData* datacall = new IRData();
            datacall->opCode = EIROpCode::CallStatic;
            datacall->opValue = irmethodcall;
            datacall->index = 0;
            datacall->SetDebugInfoByToken(mf->pingToken);
            AddIRData(datacall);
        } else {
            IRData* datacall = new IRData();
            datacall->opCode = EIROpCode::CallDynamic;
            datacall->opValue = irmethodcall;
            datacall->index = paramCount + 1;
            datacall->SetDebugInfoByToken(mf->pingToken);
            AddIRData(datacall);
        }
    } else {
        IRData* datacall = new IRData();
        datacall->opCode = EIROpCode::CallVirt;
        datacall->index = callMethodIndex;
        datacall->opValue = irmethodcall;
        datacall->SetDebugInfoByToken(mf->pingToken);
        AddIRData(datacall);
    }

    if (mfc->storeMetaVariable == nullptr) {
        std::string voidn = IRManager::GetIRNameByMetaClass(CoreMetaClassManager::voidMetaClass);
        for (size_t i = 0; i < m_IRRuntimeMethod->GetMethodReturnVariableList().size(); i++) {
            auto mrv = m_IRRuntimeMethod->GetMethodReturnVariableList()[i];
            if (mrv->irMetaType != nullptr) {
                if (mrv->irMetaType->templateIndex > -1) {
                    // Handle template case
                } else {
                    if (mrv->irMetaType->irMetaClass->irName != "Void") {
                        IRPop* irpop = new IRPop(m_IRMethod);
                        AddIRData(irpop->data);
                    }
                }
            }
        }
    }
}

void* IRCallFunction::InvokeCSharp(void* target, void** csParamObjs) {
    if (m_MethodInfo == nullptr) {
        std::cout << "error 执行时发现系统空函数" << std::endl;
        return nullptr;
    }
    // Note: This would need reflection implementation in C++
    // For now, return nullptr as placeholder
    return nullptr;
}

std::string IRCallFunction::ToIRString() {
    return IRBase::ToIRString();
}

} // namespace IR
} // namespace SimpleLanguage
