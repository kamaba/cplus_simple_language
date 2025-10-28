//****************************************************************************
//  File:      MetaParam.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/30 12:00:00
//  Description: Meta params about info class!
//****************************************************************************

#include "../Compile/FileMeta/FileMetaBase.h"
#include "../Compile/FileMeta/FileMetaSyntax.h"
#include "../Compile/FileMeta/FileMetaExpress.h"
#include "../Compile/FileMeta/FileMetaCommon.h"
#include "../Compile/FileMeta/FileMetaMemberFunction.h"

#include "MetaParam.h"
#include "MetaType.h"
#include "MetaVariable.h"
#include "MetaClass.h"
#include "MetaMemberFunction.h"
#include "MetaExpressNode/MetaExpressBase.h"
#include "MetaFunction.h"
#include "MetaExpressNode/MetaExpressBase.h"
#include "Statements/MetaBlockStatements.h"
#include "../Debug/Log.h"
#include "ClassManager.h"
#include "AllowUseSettings.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "TypeManager.h"
#include "ExpressManager.h"
#include <sstream>
#include <algorithm>

using namespace SimpleLanguage::Debug;
using namespace SimpleLanguage::Compile;

namespace SimpleLanguage {
namespace Core {

// MetaInputParam implementation
MetaInputParam::MetaInputParam(Compile::FileInputParamNode* fipn, MetaClass* mc, MetaBlockStatements* mbs) {
    m_FileInputParamNode = fipn;
    m_OwnerMetaBlockStatements = mbs;
    m_OwnerMetaClass = mc;

    CreateExpressParam cep;
    cep.ownerMBS = m_OwnerMetaBlockStatements;
    cep.ownerMetaClass = m_OwnerMetaClass;
    cep.metaType = new MetaType(CoreMetaClassManager::objectMetaClass);
    cep.fme = m_FileInputParamNode->Express();
    cep.isStatic = false;
    cep.isConst = false;
    cep.parseFrom = EParseFrom::InputParamExpress;
    m_Express = ExpressManager::CreateExpressNode(cep);
}

MetaInputParam::MetaInputParam(MetaExpressNode* inputExpress) {
    m_Express = inputExpress;
}

void MetaInputParam::Parse() {
    if (m_Express != nullptr) {
        AllowUseSettings auc;
        auc.SetParseFrom( EParseFrom::InputParamExpress );
        m_Express->Parse(&auc);
    }
}

void MetaInputParam::CalcReturnType() {
    if (m_Express != nullptr) {
        m_Express->CalcReturnType();
    }
}

MetaClass* MetaInputParam::GetRetMetaClass() {
    if (m_Express != nullptr) {
        return m_Express->GetReturnMetaClass();
    }
    return CoreMetaClassManager::objectMetaClass;
}

std::string MetaInputParam::ToFormatString() {
    if (m_Express != nullptr) {
        return m_Express->ToFormatString();
    }
    return "";
}

std::string MetaInputParam::ToStatementString() {
    std::stringstream sb;
    if (m_Express != nullptr) {
        sb << m_Express->ToFormatString();
    }
    return sb.str();
}

// MetaDefineParam implementation
MetaDefineParam::MetaDefineParam(const std::string& _name, MetaFunction* mf) {
    m_Name = _name;
    m_OwnerMetaFunction = mf;
    m_MetaVariable = new MetaVariable(m_Name, EVariableFrom::Argument,
        nullptr, m_OwnerMetaFunction->GetOwnerMetaClass(), nullptr);
}

MetaDefineParam::MetaDefineParam(const MetaDefineParam& mdp) {
    m_Name = mdp.m_Name;
    m_IsFunctionTemplate = mdp.m_IsFunctionTemplate;
    m_FileMetaParamter = mdp.m_FileMetaParamter;
    m_MetaExpressNode = mdp.m_MetaExpressNode;
    m_OwnerMetaFunction = mdp.m_OwnerMetaFunction;
    m_MetaVariable = new MetaVariable(*mdp.m_MetaVariable);
}

MetaDefineParam::MetaDefineParam(MetaFunction* mf, Compile::FileMetaParamterDefine* fmp) {
    m_OwnerMetaFunction = mf;
    m_FileMetaParamter = fmp;
    m_Name = m_FileMetaParamter->GetName();

    m_MetaVariable = new MetaVariable(m_Name, EVariableFrom::Argument,
        nullptr, m_OwnerMetaFunction->GetOwnerMetaClass(), nullptr);
}

void MetaDefineParam::ParseMetaDefineType() {
    MetaType* mdt = new MetaType(CoreMetaClassManager::objectMetaClass);
    if (this->m_FileMetaParamter != nullptr && this->m_FileMetaParamter->GetClassDefineRef() != nullptr) {
        mdt = TypeManager::GetInstance().GetMetaTypeByTemplateFunction(m_OwnerMetaFunction->GetOwnerMetaClass(), 
            dynamic_cast<MetaMemberFunction*>(m_OwnerMetaFunction), m_FileMetaParamter->GetClassDefineRef());
    }
    m_MetaVariable->SetMetaDefineType(mdt);
    if (m_FileMetaParamter != nullptr) {
        m_MetaVariable->AddPingToken(m_FileMetaParamter->GetToken());
    }
}

void MetaDefineParam::CreateExpress() {
    if (m_FileMetaParamter != nullptr && m_FileMetaParamter->GetExpress() != nullptr) {
        CreateExpressParam cep;
        cep.ownerMBS = nullptr;
        cep.metaType = new MetaType(CoreMetaClassManager::objectMetaClass);
        cep.fme = m_FileMetaParamter->GetExpress();
        cep.isStatic = false;
        cep.isConst = false;
        cep.SetParseFrom( EParseFrom::InputParamExpress );
        m_MetaExpressNode = ExpressManager::CreateExpressNode(cep);
    }
}

void MetaDefineParam::Parse() {
    if (m_MetaExpressNode != nullptr) {
        AllowUseSettings auc;
        auc.SetUseNotConst( false );
        auc.SetUseNotStatic(false);
        auc.SetCallConstructFunction(true);
        auc.SetCallFunction(true);
        m_MetaExpressNode->Parse(&auc);
    }
}

bool MetaDefineParam::EqualDefineMetaParam(MetaDefineParam* param) {
    if (param != nullptr) {
        MetaType* md = param->GetMetaVariable()->GetMetaDefineType();
        if (!MetaType::EqualMetaDefineType(md, m_MetaVariable->GetMetaDefineType())) {
            return false;
        }
        return true;
    }
    return false;
}

bool MetaDefineParam::EqualsInputMetaParam(MetaInputParam* mip) {
    if (m_MetaVariable == nullptr) {
        return true;
    }
    if (mip != nullptr) {
        auto retMC = mip->GetRetMetaClass();
        auto relation = ClassManager::ValidateClassRelationByMetaClass(m_MetaVariable->GetMetaDefineType()->GetMetaClass(), retMC);

        if (relation == EClassRelation::Same || relation == EClassRelation::Child) {
            return true;
        }
    }

    return false;
}

bool MetaDefineParam::EqualsName(const std::string& name) {
    return m_MetaVariable->GetName() == name;
}

void MetaDefineParam::SetMetaType(MetaType* mt) {
    m_MetaVariable->SetMetaDefineType(mt);
}

void MetaDefineParam::CalcReturnType() {
    if (m_MetaExpressNode != nullptr) {
        m_MetaExpressNode->CalcReturnType();
    }
}

std::string MetaDefineParam::ToFormatString() {
    std::stringstream sb;
    if (m_MetaVariable != nullptr) {
        sb << m_MetaVariable->ToFormatString();
    }
    return sb.str();
}

std::string MetaDefineParam::ToString() {
    std::stringstream sb;

    if (m_MetaVariable != nullptr) {
        sb << m_MetaVariable->GetMetaDefineType()->ToFormatString();
        sb << " ";
        sb << m_Name;
    }
    if (m_MetaExpressNode != nullptr) {
        sb << " = ";
        sb << m_MetaExpressNode->ToFormatString();
    }

    return sb.str();
}

// MetaDefineParamCollection implementation
MetaDefineParamCollection::MetaDefineParamCollection() {
}

MetaDefineParamCollection::MetaDefineParamCollection(const MetaDefineParamCollection& mdpc) {
    m_IsCanCallFunction = mdpc.m_IsCanCallFunction;
    m_IsExtendParams = mdpc.m_IsExtendParams;
    m_MinParamCount = mdpc.m_MinParamCount;
    m_IsAllConst = mdpc.m_IsAllConst;
    m_IsHaveDefaultParamExpress = mdpc.m_IsHaveDefaultParamExpress;

    for (size_t i = 0; i < mdpc.m_MetaDefineParamList.size(); i++) {
        auto mdp = new MetaDefineParam(*mdpc.m_MetaDefineParamList[i]);
        m_MetaDefineParamList.push_back(mdp);
    }
}

MetaDefineParamCollection::MetaDefineParamCollection(bool _isAllConst, bool _isCanCallFunction) {
    m_IsAllConst = _isAllConst;
    m_IsCanCallFunction = _isCanCallFunction;
}

void MetaDefineParamCollection::Clear() {
    m_MetaDefineParamList.clear();
}

void MetaDefineParamCollection::SetOwnerMetaClass(MetaClass* ownerclass) {
    for (size_t i = 0; i < m_MetaDefineParamList.size(); i++) {
        auto dParam = m_MetaDefineParamList[i];
        // ���������������ӵ����Ԫ����߼�
    }
}

MetaDefineParam* MetaDefineParamCollection::GetMetaDefineParamByName(const std::string& name) {
    for (size_t i = 0; i < m_MetaDefineParamList.size(); i++) {
        auto dParam = m_MetaDefineParamList[i];
        if (dParam->EqualsName(name)) {
            return dParam;
        }
    }
    return nullptr;
}

bool MetaDefineParamCollection::CheckDefineMetaParam(MetaDefineParam* a, MetaDefineParam* b) {
    if (a->EqualDefineMetaParam(b)) {
        return true;
    }
    return a == b;
}

void MetaDefineParamCollection::AddMetaDefineParam(MetaDefineParam* metaMemberParam) {
    if (m_IsExtendParams) {
        Log::AddInStructMeta(EError::None, "Error Params mode: only one parameter allowed, otherwise invalid mode");
        return;
    }

    m_MetaDefineParamList.push_back(metaMemberParam);
    if (metaMemberParam->IsExtendParams()) {
        m_IsExtendParams = true;
    }

    if (IsHaveDefaultParamExpress()) {
        if (metaMemberParam->GetExpressNode() == nullptr) {
            Log::AddInStructMeta(EError::None, "Error AddMetaDefineParam ����ǰ���Ѷ������ʽ����߱������Ĭ��ֵ����ʽ!!");
        }
    } else {
        if (metaMemberParam->GetExpressNode() != nullptr) {
            m_IsHaveDefaultParamExpress = true;
        } else {
            m_MinParamCount++;
        }
    }
}

bool MetaDefineParamCollection::IsEqualMetaInputParamCollection(MetaInputParamCollection* mpc) {
    int inputCount = 0;
    if (mpc != nullptr) {
        inputCount = mpc->GetCount();
    }
    if (m_IsExtendParams) {
        // ����ֵ �������붨��ֵ��ͬ����Ϊʹ��params �ķ�ʽ ���һ���һ���������飬���������������������
        if (m_MetaDefineParamList.size() == 0) {
            return false;
        }
        auto mdp = m_MetaDefineParamList[m_MetaDefineParamList.size() - 1];
        if (mdp->IsExtendParams() && mdp->GetMetaVariable()->IsArray()) {
            auto mdt = mdp->GetMetaVariable()->GetMetaDefineType();
            for (size_t i = 0; i < mpc->GetMetaInputParamList().size(); i++) {
                auto mip = mpc->GetMetaInputParamList()[i];
                if (mip->GetRetMetaClass() != mdt->GetMetaClass()) {
                    return false;
                }
            }
            return true;
        }

        return false;
    } else {
        if (m_MetaDefineParamList.size() >= static_cast<size_t>(inputCount)) {
            for (size_t i = 0; i < m_MetaDefineParamList.size(); i++) {
                MetaDefineParam* a = m_MetaDefineParamList[i];
                if (a == nullptr)
                    return false;
                MetaInputParam* b = nullptr;
                if (mpc != nullptr && i < static_cast<size_t>(inputCount)) {
                    b = mpc->GetMetaInputParamList()[i];
                }
                if (!MetaInputParamCollection::CheckInputMetaParam(a, b))
                    return false;
            }
            return true;
        }
        return false;
    }
}

bool MetaDefineParamCollection::IsEqualMetaDefineParamCollection(MetaDefineParamCollection* mdpc) {
    if (mdpc == nullptr) {
        return GetMinParamCount() == 0;
    }

    if (m_MetaDefineParamList.size() == mdpc->m_MetaDefineParamList.size()) {
        if (m_MetaDefineParamList.size() == 0) {
            return true;
        }

        for (size_t i = 0; i < m_MetaDefineParamList.size(); i++) {
            auto a = m_MetaDefineParamList[i];
            auto b = mdpc->m_MetaDefineParamList[i];
            if (!CheckDefineMetaParam(a, b))
                return false;
        }
        return true;
    }
    return false;
}

std::string MetaDefineParamCollection::ToParamTypeName() {
    std::stringstream sb;
    for (size_t i = 0; i < m_MetaDefineParamList.size(); i++) {
        sb << m_MetaDefineParamList[i]->GetName();
        if (i < m_MetaDefineParamList.size() - 1)
            sb << "_";
    }
    return sb.str();
}

std::string MetaDefineParamCollection::ToFormatString() {
    std::stringstream sb;

    sb << "(";
    for (size_t i = 0; i < m_MetaDefineParamList.size(); i++) {
        sb << m_MetaDefineParamList[i]->ToFormatString();
        if (i < m_MetaDefineParamList.size() - 1) {
            sb << ",";
        }
    }
    sb << ")";
    return sb.str();
}

// MetaInputParamCollection implementation
MetaInputParamCollection::MetaInputParamCollection(MetaClass* mc, MetaBlockStatements* mbs) {
    m_OwnerMetaClass = mc;
    m_MetaBlockStatements = mbs;
}

MetaInputParamCollection::MetaInputParamCollection(Compile::FileMetaParTerm* fmpt, MetaClass* mc, MetaBlockStatements* mbs) {
    m_OwnerMetaClass = mc;
    m_MetaBlockStatements = mbs;
    auto splitList = fmpt->SplitParamList();
    std::vector<Compile::FileInputParamNode*> list;
    for (size_t i = 0; i < splitList.size(); i++) {
        Compile::FileInputParamNode* fnpn = new Compile::FileInputParamNode(splitList[i]);
        list.push_back(fnpn);
    }
    ParseList(list);
}

void MetaInputParamCollection::Clear() {
    m_MetaInputParamList.clear();
}

bool MetaInputParamCollection::CheckInputMetaParam(MetaDefineParam* a, MetaInputParam* b) {
    if (b == nullptr) {
        return !a->IsMust();      // ���봫�Σ���û�в���
    }
    if (a->EqualsInputMetaParam(b))
        return true;
    return false;
}

void MetaInputParamCollection::ParseList(const std::vector<Compile::FileInputParamNode*>& splitList) {
    for (size_t i = 0; i < splitList.size(); i++) {
        MetaInputParam* mp = new MetaInputParam(splitList[i], m_OwnerMetaClass, m_MetaBlockStatements);
        AddMetaInputParam(mp);
    }
}

void MetaInputParamCollection::AddMetaInputParam(MetaInputParam* mip) {
    m_MetaInputParamList.push_back(mip);
}

void MetaInputParamCollection::CaleReturnType() {
    for (size_t i = 0; i < m_MetaInputParamList.size(); i++) {
        m_MetaInputParamList[i]->Parse();
        m_MetaInputParamList[i]->CalcReturnType();
    }
}

MetaClass* MetaInputParamCollection::GetMaxLevelMetaClassType() {
    MetaClass* mc = CoreMetaClassManager::objectMetaClass;
    bool isAllSame = true;
    for (size_t i = 0; i < m_MetaInputParamList.size() - 1; i++) {
        MetaInputParam* cmc = m_MetaInputParamList[i];
        MetaInputParam* nmc = m_MetaInputParamList[i + 1];
        if (mc == nullptr || nmc == nullptr) continue;
        if (cmc->GetExpress()->GetOpLevel() == nmc->GetExpress()->GetOpLevel()) {
            if (cmc->GetExpress()->GetOpLevel() == 10) {
                auto cur = cmc->GetRetMetaClass();
                auto next = nmc->GetRetMetaClass();
                auto relation = ClassManager::ValidateClassRelationByMetaClass(cur, next);
                if (relation == EClassRelation::Same
                    || relation == EClassRelation::Child) {
                    mc = next;
                } else if (relation == EClassRelation::Parent) {
                    mc = cur;
                } else {
                    isAllSame = false;
                    break;
                }
            } else {
                mc = cmc->GetRetMetaClass();
                isAllSame = true;
            }
        } else {
            if (cmc->GetExpress()->GetOpLevel() > nmc->GetExpress()->GetOpLevel()) {
                mc = cmc->GetRetMetaClass();
            } else {
                mc = nmc->GetRetMetaClass();
            }
        }
    }
    if (isAllSame) {
        Log::AddInStructMeta(EError::None, "ȫ������");
    }
    return mc;
}

std::string MetaInputParamCollection::ToFormatString() {
    std::stringstream sb;

    for (size_t i = 0; i < m_MetaInputParamList.size(); i++) {
        sb << m_MetaInputParamList[i]->ToFormatString();
        if (i < m_MetaInputParamList.size() - 1) {
            sb << ",";
        }
    }
    return sb.str();
}

// MetaInputTemplateCollection implementation
MetaInputTemplateCollection::MetaInputTemplateCollection() {
}

std::vector<MetaClass*> MetaInputTemplateCollection::GetMetaClassList(bool& isAllMetaClass) {
    isAllMetaClass = false;
    std::vector<MetaClass*> mcList;
    for (size_t i = 0; i < m_MetaTemplateParamsList.size(); i++) {
        if (m_MetaTemplateParamsList[i]->GetMetaClass() != nullptr) {
            mcList.push_back(m_MetaTemplateParamsList[i]->GetMetaClass());
        }
    }
    if (mcList.size() > 0 && mcList.size() == m_MetaTemplateParamsList.size()) {
        isAllMetaClass = true;
    }
    return mcList;
}

void MetaInputTemplateCollection::AddMetaTemplateParamsList(MetaType* mp) {
    m_MetaTemplateParamsList.push_back(mp);
}

std::vector<MetaClass*> MetaInputTemplateCollection::GetMetaClassParamsList() {
    std::vector<MetaClass*> list;

    for (auto v : m_MetaTemplateParamsList) {
        if (v->GetMetaClass() == nullptr)
            return std::vector<MetaClass*>();
        list.push_back(v->GetMetaClass());
    }

    return list;
}

MetaClass* MetaInputTemplateCollection::GetMaxLevelMetaClassType() {
    MetaClass* mc = CoreMetaClassManager::objectMetaClass;
    bool isAllSame = true;
    for (size_t i = 0; i < m_MetaTemplateParamsList.size() - 1; i++) {
        MetaType* cmdt = m_MetaTemplateParamsList[i];
        MetaType* nmdt = m_MetaTemplateParamsList[i + 1];
        if (cmdt == nmdt) {
            isAllSame = true;
        } else {
            // ����������Ӹ����ӵ��߼�
        }
    }
    if (isAllSame) {
        mc = m_MetaTemplateParamsList[0]->GetMetaClass();
    }
    return mc;
}

std::string MetaInputTemplateCollection::ToFormatString() {
    std::stringstream sb;
    sb << "<";
    for (size_t i = 0; i < GetMetaTemplateParamsList().size(); i++) {
        sb << GetMetaTemplateParamsList()[i]->ToFormatString();
        if (i < GetMetaTemplateParamsList().size() - 1) {
            sb << ",";
        }
    }
    sb << ">";
    return sb.str();
}

// MetaInputArrayCollection implementation
MetaInputArrayCollection::MetaInputArrayCollection(Compile::FileMetaBracketTerm* fmbt) {
    // ʵ���߼�
}

bool MetaDefineParam::IsExtendParams() const {
    return false; // 默认实现，可以根据需要修改
}

} // namespace Core
} // namespace SimpleLanguage
