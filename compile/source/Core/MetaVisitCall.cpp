//****************************************************************************
//  File:      MetaVisitCall.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description:  create visit variable or method call!
//****************************************************************************

#include "MetaVisitCall.h"
#include "MetaExpressNode/MetaExpressConst.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "MetaParam.h"
#include "MetaFunction.h"
#include "MetaMemberFunction.h"
#include "MetaClass.h"
#include "MetaType.h"
#include "MetaVariable.h"
#include "../Debug/Log.h"
#include <sstream>

using namespace SimpleLanguage::Debug;
namespace SimpleLanguage {
namespace Core {

MetaMethodCall::MetaMethodCall(MetaClass* staticMc, const std::vector<MetaType*>& staticMmitList, MetaFunction* fun, const std::vector<MetaType*>& mpipList, MetaInputParamCollection* paramCollection, MetaVariable* loadMv, MetaVariable* storeMv) {
    m_StaticCallerMetaClass = staticMc;
    if (!staticMmitList.empty()) {
        m_StaticMetaClassInputTemplateList = staticMmitList;
    }
    m_VMCallMetaFunction = fun;
    if (!mpipList.empty()) {
        m_MetaFunctionInputTemplateList = mpipList;
    }

    std::vector<MetaDefineParam*> mpList;
    if (m_VMCallMetaFunction != nullptr && m_VMCallMetaFunction->GetMetaMemberParamCollection() != nullptr) {
        mpList = m_VMCallMetaFunction->GetMetaMemberParamCollection()->GetMetaDefineParamList();
    }
    int defineCount = m_VMCallMetaFunction != nullptr ? m_VMCallMetaFunction->GetMetaMemberParamCollection()->GetMaxParamCount() : 0;
    int inputCount = paramCollection != nullptr ? paramCollection->metaInputParamList().size() : 0;
    for (int i = 0; i < defineCount; i++) {
        if (i < inputCount) {
            MetaInputParam* mip = paramCollection->metaInputParamList()[i];
            m_MetaInputParamList.push_back(mip->express);
        } else {
            MetaDefineParam* mdp = mpList[i];
            if (mdp != nullptr) {
                m_MetaInputParamList.push_back(mdp->expressNode);
            }
        }
    }
    m_LoadMetaVariable = loadMv;
    m_StoreMetaVariable = storeMv;
}

void MetaMethodCall::SetStoreMetaVariable(MetaVariable* mv) {
    m_StoreMetaVariable = mv;
}

MetaType* MetaMethodCall::GeMetaDefineType() {
    return m_VMCallMetaFunction->metaDefineType;
}

MetaFunction* MetaMethodCall::GetTemplateMemberFunction() {
    if (dynamic_cast<MetaGenTempalteFunction*>(m_VMCallMetaFunction) != nullptr) {
        MetaGenTempalteFunction* mgtf = dynamic_cast<MetaGenTempalteFunction*>(m_VMCallMetaFunction);
        return mgtf->sourceTemplateFunctionMetaMemberFunction;
    }
    return m_VMCallMetaFunction;
}

std::string MetaMethodCall::ToCommonString() const {
    std::ostringstream sb;
    if (m_VMCallMetaFunction != nullptr) {
        sb << m_VMCallMetaFunction->GetName() << "(";
        int inputCount = m_MetaInputParamList.size();
        for (int i = 0; i < inputCount; i++) {
            sb << m_MetaInputParamList[i]->ToFormatString();
            if (i < inputCount - 1) {
                sb << ",";
            }
        }
        sb << ")";
    }
    return sb.str();
}

std::string MetaMethodCall::ToFormatString() const {
    std::ostringstream sb;

    if (loadMetaVariable() != nullptr) {
        sb << "[";
        sb << m_VMCallMetaFunction->GetOwnerMetaClass()->allClassName;
        sb << "]";

        sb << loadMetaVariable()->name;
        sb << ".";
    } else {
        sb << m_VMCallMetaFunction->GetOwnerMetaClass()->allClassName;
        sb << ".";
    }
    if (m_VMCallMetaFunction != nullptr) {
        sb << m_VMCallMetaFunction->GetName() << "(";
        int inputCount = m_MetaInputParamList.size();
        for (int i = 0; i < inputCount; i++) {
            sb << m_MetaInputParamList[i]->ToFormatString();
            if (i < inputCount - 1) {
                sb << ",";
            }
        }
        sb << ")";
    }

    return sb.str();
}

std::string MetaMethodCall::ToString() const {
    return ToFormatString();
}

MetaVisitNode::MetaVisitNode() {
    m_VisitType = EVisitType::Variable;
}

MetaVisitNode* MetaVisitNode::CreateByNewTemplate(MetaType* mt, MetaFunction* mf, MetaVariable* mv) {
    MetaVisitNode* vn = new MetaVisitNode();

    vn->m_CallMetaType = mt;
    vn->m_VisitType = EVisitType::New;
    vn->m_Variable = mv;
    vn->m_MethodCall = new MetaMethodCall(mt->GetMetaClass(), {}, mf, {}, nullptr, nullptr, mv);
    return vn;
}

MetaVisitNode* MetaVisitNode::CraeteByNewClass(MetaType* mt, MetaBraceOrBracketStatementsContent* mb, MetaVariable* mv) {
    MetaVisitNode* vn = new MetaVisitNode();

    vn->m_CallMetaType = mt;
    vn->m_MetaBraceStatementsContent = mb;
    vn->m_VisitType = EVisitType::New;
    vn->m_Variable = mv;
    if (dynamic_cast<MetaGenTemplateClass*>(mt->GetMetaClass()) != nullptr) {
        MetaGenTemplateClass* mgtc = dynamic_cast<MetaGenTemplateClass*>(mt->GetMetaClass());
        vn->m_ReturnMetaType = new MetaType(mgtc);
    }

    return vn;
}

MetaVisitNode* MetaVisitNode::CraeteByNewData(MetaType* mt, MetaBraceOrBracketStatementsContent* mb) {
    MetaVisitNode* vn = new MetaVisitNode();

    vn->m_CallMetaType = mt;
    vn->m_MetaBraceStatementsContent = mb;
    vn->m_VisitType = EVisitType::New;

    return vn;
}

MetaVisitNode* MetaVisitNode::CreateByConstExpress(MetaConstExpressNode* constExpress, MetaVariable* variable) {
    MetaVisitNode* vn = new MetaVisitNode();

    vn->m_ConstValueExpress = constExpress;
    vn->m_Variable = variable;
    vn->m_VisitType = EVisitType::ConstValue;

    return vn;
}

MetaVisitNode* MetaVisitNode::CreateByEnumDefaultValue(MetaType* mt, MetaVariable* variable) {
    MetaVisitNode* vn = new MetaVisitNode();
    vn->m_Variable = variable;
    vn->m_VisitType = EVisitType::Variable;

    return vn;
}

MetaVisitNode* MetaVisitNode::CreateByMethodCall(MetaMethodCall* methodCall) {
    MetaVisitNode* vn = new MetaVisitNode();

    vn->m_VisitType = EVisitType::MethodCall;
    vn->m_MethodCall = methodCall;

    return vn;
}

MetaVisitNode* MetaVisitNode::CreateByVisitVariable(MetaVisitVariable* variable) {
    MetaVisitNode* vn = new MetaVisitNode();

    vn->m_VisitType = EVisitType::VisitVariable;
    vn->m_VisitVariable = variable;

    return vn;
}

MetaVisitNode* MetaVisitNode::CreateByVariable(MetaVariable* variable, MetaType* callerMt) {
    MetaVisitNode* vn = new MetaVisitNode();

    vn->m_VisitType = EVisitType::Variable;
    vn->m_Variable = variable;
    vn->m_CallMetaType = callerMt;

    return vn;
}

MetaVisitNode* MetaVisitNode::CreateByThis(MetaVariable* variable) {
    MetaVisitNode* vn = new MetaVisitNode();

    vn->m_VisitType = EVisitType::Variable;
    vn->m_Variable = variable;
    vn->m_CallMetaType = nullptr;

    return vn;
}

MetaVisitNode* MetaVisitNode::CreateByBase(MetaVariable* variable) {
    MetaVisitNode* vn = new MetaVisitNode();

    vn->m_VisitType = EVisitType::Variable;
    vn->m_Variable = variable;
    vn->m_CallMetaType = nullptr;

    return vn;
}

void MetaVisitNode::SetMethodCall(MetaMethodCall* methodCall) {
    m_MethodCall = methodCall;
}

MetaVariable* MetaVisitNode::GetOrgTemplateMetaVariable() {
    if (m_Variable == nullptr) { return nullptr; }

    MetaVariable* t = m_Variable;
    while (t != nullptr) {
        if (t->GetSourceMetaVariable() == nullptr) {
            break;
        }
        t = t->GetSourceMetaVariable();
    }
    return t;
}

MetaType* MetaVisitNode::GetMetaDefineType() {
    if (m_ReturnMetaType != nullptr) {
        return m_ReturnMetaType;
    }
    switch (m_VisitType) {
        case EVisitType::MethodCall: {
            if (m_MethodCall->metaMemberFunction() != nullptr) {
                return m_MethodCall->metaMemberFunction()->GetReturnMetaVariable()->GetMetaDefineType();
            }
            return m_MethodCall->function()->GetReturnMetaVariable()->GetMetaDefineType();
        }
        case EVisitType::VisitVariable: {
            return m_VisitVariable->GetMetaDefineType();
        }
        case EVisitType::Variable: {
            return m_Variable->GetMetaDefineType();
        }
        case EVisitType::New: {
            return m_CallMetaType;
        }
        default: {
            Log::AddInStructMeta(EError::None, "Error ---------" + std::to_string(static_cast<int>(m_VisitType)));
        }
        break;
    }
    return new MetaType(CoreMetaClassManager::GetInstance().objectMetaClass);
}

MetaClass* MetaVisitNode::GetMetaClass() {
    MetaType* mt = GetMetaDefineType();
    if (mt == nullptr) {
        Log::AddInStructMeta(EError::None, "Error");
        return nullptr;
    }
    return mt->metaClass;
}

MetaVariable* MetaVisitNode::GetRetMetaVariable() {
    switch (m_VisitType) {
        case EVisitType::Variable: {
            return m_Variable;
        }
        case EVisitType::MethodCall: {
            return m_MethodCall->function()->returnMetaVariable;
        }
        case EVisitType::New: {
            return m_Variable;
        }
        case EVisitType::Enum: {
            return m_Variable;
        }
        default: {
            Log::AddInStructMeta(EError::None, "Error MetaVisiCall IsNull!");
        }
        break;
    }
    return nullptr;
}

int MetaVisitNode::CalcParseLevel(int level) {
    return level;
}

void MetaVisitNode::CalcReturnType() {
    GetMetaDefineType();
}

std::string MetaVisitNode::ToFormatString() const {
    std::ostringstream sb;

    switch (m_VisitType) {
        case EVisitType::MethodCall: {
            sb << m_MethodCall->ToFormatString();
        }
        break;
        case EVisitType::VisitVariable: {
            sb << m_VisitVariable->ToFormatString();
        }
        break;
        case EVisitType::ConstValue: {
            sb << m_ConstValueExpress->value;
        }
        break;
        case EVisitType::Variable: {
            sb << m_Variable->ToFormatString();
        }
        break;
        case EVisitType::New: {
            sb << m_CallMetaType->ToString();
        }
        break;
        default: {
            sb << "Error MetaVisitCall Default Parse!";
        }
        break;
    }

    return sb.str();
}

} // namespace Core
} // namespace SimpleLanguage