//****************************************************************************
//  File:      MetaMemberFunction.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/30 12:00:00
//  Description: Meta enum's attribute
//****************************************************************************

#include "MetaMemberFunction.h"
#include "MetaMemberVariable.h"
#include "MetaClass.h"
#include "MetaFunction.h"
#include "MetaType.h"
#include "MetaTemplate.h"
#include "MetaParam.h"
#include "Statements/MetaBlockStatements.h"
#include "Statements/MetaStatements.h"
#include "MetaVariable.h"
#include "MetaGenTemplateFunction.h"
#include "../Compile/FileMeta/FileMetaCommon.h"
#include "../Compile/FileMeta/FileMetaSyntax.h"
#include "../Compile/FileMeta/FileMetaMemberFunction.h"
#include "../Compile/Token.h"
#include "../Debug/Log.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "TypeManager.h"
#include "ClassManager.h"
#include "Statements/MetaDefineVarStatements.h"
#include "Statements/MetaAssignStatements.h"
#include "Statements/MetaCallStatements.h"
#include "Statements/MetaReturnStatements.h"
#include "Statements/MetaIfStatements.h"
#include "Statements/MetaSwitchStatements.h"
#include "Statements/MetaWhileDoWhileStatements.h"
#include "Statements/MetaBreakContinueGoStatements.h"
#include <sstream>
#include <algorithm>

using namespace SimpleLanguage::Compile;

namespace SimpleLanguage {
namespace Core {

// MetaMemberFunctionTemplateNode implementation
MetaMemberFunctionTemplateNode::MetaMemberFunctionTemplateNode() {
}

void MetaMemberFunctionTemplateNode::SetDeep(int deep) {
    // Implementation
}

MetaMemberFunction* MetaMemberFunctionTemplateNode::IsSameMetaMemeberFunction(MetaMemberFunction* mmf) {
    MetaMemberFunctionNode* find = nullptr;
    auto it = m_MetaTemplateFunctionNodeDict.find(mmf->GetMetaMemberTemplateCollection()->GetCount());
    if (it != m_MetaTemplateFunctionNodeDict.end()) {
        find = it->second;
    } else {
        find = new MetaMemberFunctionNode();
    }
    return find->IsSameMetaMemeberFunction(mmf);
}

bool MetaMemberFunctionTemplateNode::AddMetaMemberFunction(MetaMemberFunction* mmf) {
    MetaMemberFunctionNode* find = nullptr;
    auto it = m_MetaTemplateFunctionNodeDict.find(mmf->GetMetaMemberTemplateCollection()->GetCount());
    if (it != m_MetaTemplateFunctionNodeDict.end()) {
        find = it->second;
    } else {
        find = new MetaMemberFunctionNode();
        m_MetaTemplateFunctionNodeDict[mmf->GetMetaMemberTemplateCollection()->GetCount()] = find;
    }
    return find->AddMetaMemberFunction(mmf);
}

void MetaMemberFunctionTemplateNode::ParseMemberFunctionDefineMetaType() {
    for (auto& v : m_MetaTemplateFunctionNodeDict) {
        v.second->ParseMemberFunctionDefineMetaType();
    }
}

// MetaMemberFunctionNode implementation
MetaMemberFunctionNode::MetaMemberFunctionNode() {
}

void MetaMemberFunctionNode::SetDeep(int deep) {
    // Implementation
}

MetaMemberFunction* MetaMemberFunctionNode::IsSameMetaMemeberFunction(MetaMemberFunction* mmf) {
    std::vector<MetaMemberFunction*> list;
    auto it = m_MetaParamFunctionDict.find(mmf->GetMetaMemberParamCollection()->GetMetaDefineParamList().size());
    if (it != m_MetaParamFunctionDict.end()) {
        list = it->second;
    } else {
        list = std::vector<MetaMemberFunction*>();
    }

    MetaMemberFunction* find2 = nullptr;
    for (size_t i = 0; i < list.size(); i++) {
        MetaMemberFunction* curFun = list[i];
        if (curFun->GetMetaMemberParamCollection()->IsEqualMetaDefineParamCollection(mmf->GetMetaMemberParamCollection())) {
            find2 = curFun;
            break;
        }
    }
    if (find2 == nullptr) {
        list.push_back(mmf);
        return nullptr;
    } else {
        return find2;
    }
}

bool MetaMemberFunctionNode::AddMetaMemberFunction(MetaMemberFunction* mmf) {
    std::vector<MetaMemberFunction*> list;
    auto it = m_MetaParamFunctionDict.find(mmf->GetMetaMemberParamCollection()->GetMetaDefineParamList().size());
    if (it != m_MetaParamFunctionDict.end()) {
        list = it->second;
    } else {
        list = std::vector<MetaMemberFunction*>();
        m_MetaParamFunctionDict[mmf->GetMetaMemberParamCollection()->GetMetaDefineParamList().size()] = list;
    }

    MetaMemberFunction* find2 = nullptr;
    for (size_t i = 0; i < list.size(); i++) {
        MetaMemberFunction* curFun = list[i];
        if (curFun->GetMetaMemberParamCollection()->IsEqualMetaDefineParamCollection(mmf->GetMetaMemberParamCollection())) {
            find2 = curFun;
            break;
        }
    }
    if (find2 == nullptr) {
        list.push_back(mmf);
        return true;
    } else {
        //Log::AddInStructMeta(EError::None, "发现已经定义过某某类2" + mmf->GetFunctionAllName());
    }
    return false;
}

void MetaMemberFunctionNode::ParseMemberFunctionDefineMetaType() {
    for (auto& v : m_MetaParamFunctionDict) {
        for (auto& v2 : v.second) {
            v2->ParseDefineMetaType();
        }
    }
}

std::vector<MetaMemberFunction*> MetaMemberFunctionNode::GetMetaMemberFunctionListByParamCount(int count) {
    auto it = m_MetaParamFunctionDict.find(count);
    if (it != m_MetaParamFunctionDict.end()) {
        return it->second;
    }
    return std::vector<MetaMemberFunction*>();
}

// MetaMemberFunction implementation
MetaMemberFunction::MetaMemberFunction() : MetaFunction() {
}

MetaMemberFunction::MetaMemberFunction(MetaClass* mc) : MetaFunction(mc) {
}

MetaMemberFunction::MetaMemberFunction(MetaClass* mc, Compile::FileMetaMemberFunction* fmmf) : MetaFunction(mc) {
    m_MetaMemberParamCollection = new MetaDefineParamCollection(true, false);
    m_FileMetaMemberFunction = fmmf;
    m_Name = fmmf->GetName();

    m_IsStatic = fmmf->GetStaticToken() != nullptr;
    m_IsGet = fmmf->GetGetToken() != nullptr;
    m_IsSet = fmmf->GetSetToken() != nullptr;
    m_IsFinal = fmmf->GetFinalToken() != nullptr;
    if (fmmf->GetOverrideToken() != nullptr) {
        if (fmmf->GetOverrideToken()->GetType() == ETokenType::Override) {
            m_IsOverrideFunction = true;
        }
    }
    if (fmmf->GetInterfaceToken() != nullptr) {
        m_IsWithInterface = true;
    }

    int paramCount = fmmf->GetMetaParamtersList().size();
    for (int i = 0; i < paramCount; i++) {
        auto param = fmmf->GetMetaParamtersList()[i];
        MetaDefineParam* mmp = new MetaDefineParam(this, param);
        AddMetaDefineParam(mmp);
    }

    int templateCount = fmmf->GetMetaTemplatesList().size();
    for (int i = 0; i < templateCount; i++) {
        m_IsTemplateFunction = true;

        auto templateParam = fmmf->GetMetaTemplatesList()[i];

        MetaTemplate* mdt = new MetaTemplate( m_OwnerMetaClass, templateParam, mc->GetMetaTemplateList().size() + i);
        AddMetaDefineTemplate(mdt);

        if (templateParam->InClassNameTemplateNode() != nullptr) {
            auto inClassToken = templateParam->InClassNameTemplateNode();
            MetaNode* mn = ClassManager::GetInstance().GetMetaClassByNameAndFileMeta(m_OwnerMetaClass, inClassToken->GetFileMeta(), inClassToken->GetNameList() );
            if (mn == nullptr) {
                continue;
            }
            MetaClass* gmc = mn->GetMetaClassByTemplateCount(0);
            if (gmc == nullptr) {
                //Log::AddInStructMeta(EError::None, "Error 没有查找到inClass的类名, " + inClassToken->ToFormatString());
                continue;
            }
            mdt->SetInConstraintMetaClass(gmc);
        } else {
            mdt->SetInConstraintMetaClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        }
    }
    m_MetaBlockStatements = new MetaBlockStatements(this, nullptr);
    m_MetaBlockStatements->SetIsOnFunction(true);

    Init();
}

MetaMemberFunction::MetaMemberFunction(MetaClass* mc, const std::string& name) : MetaFunction(mc) {
    m_Name = name;
    m_IsCanRewrite = true;
    m_MetaMemberParamCollection->Clear();

    m_MetaBlockStatements = new MetaBlockStatements(this, nullptr);
    m_MetaBlockStatements->SetIsOnFunction(true);

    Init();
}

MetaMemberFunction::MetaMemberFunction(const MetaMemberFunction& mmf) : MetaFunction(mmf) {
    m_IsTemplateFunction = mmf.m_IsTemplateFunction;
    m_ConstructInitFunction = mmf.m_ConstructInitFunction;
    m_IsWithInterface = mmf.m_IsWithInterface;
    m_FileMetaMemberFunction = mmf.m_FileMetaMemberFunction;
    m_GenTempalteFunctionList = mmf.m_GenTempalteFunctionList;
    m_SourceMetaMemberFunction = mmf.m_SourceMetaMemberFunction;
}

std::string MetaMemberFunction::GetFunctionAllName() const {
    if (!m_FunctionAllName.empty()) {
        return m_FunctionAllName;
    }
    return MetaBase::GetName();
}

int MetaMemberFunction::GetParseLevel() const {
    if (m_IsTemplateFunction) {
        return 0;
    } else if (m_OwnerMetaClass && m_OwnerMetaClass->IsTemplateClass()) {
        return 1;
    } else {
        return 2;
    }
}

Compile::Token* MetaMemberFunction::GetToken() const {
    if (m_FileMetaMemberFunction != nullptr && m_FileMetaMemberFunction->GetFinalToken() != nullptr) {
        return m_FileMetaMemberFunction->GetFinalToken();
    }
    return this->GetPingToken();
}

bool MetaMemberFunction::IsEqualWithMMFByNameAndParam(MetaMemberFunction* mmf) {
    if (mmf->GetName() != m_Name) return false;

    if (!m_MetaMemberParamCollection->IsEqualMetaDefineParamCollection(mmf->GetMetaMemberParamCollection())) {
        return false;
    }

    return true;
}

void MetaMemberFunction::AddMetaDefineParam(MetaDefineParam* mdp) {
    m_MetaMemberParamCollection->AddMetaDefineParam(mdp);
}

void MetaMemberFunction::AddMetaDefineTemplate(MetaTemplate* mt) {
    m_MetaMemberTemplateCollection->AddMetaDefineTemplate(mt);
}

MetaGenTemplateFunction* MetaMemberFunction::AddGenTemplateMemberFunctionByMetaTypeList(MetaClass* mc, const std::vector<MetaType*>& list) {
    if (mc->IsTemplateClass()) {
        return nullptr;
    }

    std::vector<MetaClass*> mcList;

    for (auto& v : list) {
        if (v->GetEType() == EMetaTypeType::MetaClass || v->GetEType() == EMetaTypeType::MetaGenClass) {
            mcList.push_back(v->GetMetaClass());
        }
    }
    if (mcList.size() == list.size()) {
        return AddGenTemplateMemberFunctionBySelf(mc, mcList);
    }
    return nullptr;
}

MetaGenTemplateFunction* MetaMemberFunction::AddGenTemplateMemberFunctionBySelf(MetaClass* mc, const std::vector<MetaClass*>& list) {
    MetaGenTemplateFunction* mgtf = GetGenTemplateFunction(list);
    if (mgtf == nullptr) {
        std::vector<MetaGenTemplate*> mgtList;
        for (size_t i = 0; i < list.size(); i++) {
            auto l1 = this->m_MetaMemberTemplateCollection->GetMetaTemplateList()[i];
            MetaGenTemplate* mgt = new MetaGenTemplate(l1, new MetaType(list[i]));
            mgtList.push_back(mgt);
        }
        mgtf = new MetaGenTemplateFunction(this, mgtList);
        mgtf->SetOwnerMetaClass(mc);

        this->m_GenTempalteFunctionList.push_back(mgtf);

        mgtf->Parse();
    }
    return mgtf;
}

MetaGenTemplateFunction* MetaMemberFunction::GetGenTemplateFunction(const std::vector<MetaClass*>& mcList) {
    if (mcList.size() == m_GenTempalteFunctionList.size()) {
        for (size_t i = 0; i < m_GenTempalteFunctionList.size(); i++) {
            auto c = m_GenTempalteFunctionList[i];

            if (c->MatchInputTemplateInsance(mcList)) {
                return c;
            }
        }
    }
    return nullptr;
}

bool MetaMemberFunction::Parse() {
    bool flag = MetaFunction::Parse();

    UpdateVritualFunctionName();

    return flag;
}

void MetaMemberFunction::ParseDefineMetaType() {
    if (this->m_FileMetaMemberFunction != nullptr) {
        if (m_FileMetaMemberFunction->GetDefineMetaClass() != nullptr) {
            FileMetaClassDefine* cmr = m_FileMetaMemberFunction->GetDefineMetaClass();
            MetaType* defineMetaType = TypeManager::GetInstance().GetMetaTypeByTemplateFunction(m_OwnerMetaClass, this, cmr);

            if (m_ConstructInitFunction && defineMetaType->GetMetaClass() != CoreMetaClassManager::GetInstance().GetVoidMetaClass()) {
                //Log::AddInStructMeta(EError::None, "Error 当前类:" + m_AllName + " 是构建Init类，不允许有返回类型 ");
            } else {
                m_ReturnMetaVariable->SetMetaDefineType(defineMetaType);
                m_ReturnMetaVariable->SetRealMetaType(defineMetaType);
            }
        }
    }
    for (size_t i = 0; i < m_MetaMemberParamCollection->GetMetaDefineParamList().size(); i++) {
        MetaDefineParam* mpl = m_MetaMemberParamCollection->GetMetaDefineParamList()[i];
        mpl->ParseMetaDefineType();
    }
    UpdateVritualFunctionName();
}

void MetaMemberFunction::CreateMetaExpress() {
    for (size_t i = 0; i < m_MetaMemberParamCollection->GetMetaDefineParamList().size(); i++) {
        MetaDefineParam* mpl = m_MetaMemberParamCollection->GetMetaDefineParamList()[i];
        mpl->CreateExpress();
    }
}

bool MetaMemberFunction::ParseMetaExpress() {
    for (size_t i = 0; i < m_MetaMemberParamCollection->GetMetaDefineParamList().size(); i++) {
        MetaDefineParam* mpl = m_MetaMemberParamCollection->GetMetaDefineParamList()[i];
        mpl->Parse();
        mpl->CalcReturnType();
    }
    return true;
}

void MetaMemberFunction::ParseStatements() {
    bool nohasContent = false;
    if (this->m_FileMetaMemberFunction != nullptr) {
        if (m_ThisMetaVariable != nullptr) {
            m_ThisMetaVariable->AddPingToken(m_FileMetaMemberFunction->GetToken());
        }
        if (m_FileMetaMemberFunction->GetFileMetaBlockSyntax() != nullptr) {
            Compile::Token* beginToken = m_FileMetaMemberFunction->GetFileMetaBlockSyntax()->GetBeginBlock();
            Compile::Token* endToken = m_FileMetaMemberFunction->GetFileMetaBlockSyntax()->GetEndBlock();
            m_MetaBlockStatements->SetFileMetaBlockSyntax(m_FileMetaMemberFunction->GetFileMetaBlockSyntax());
            m_MetaBlockStatements->SetMetaMemberParamCollection(m_MetaMemberParamCollection);
            CreateMetaSyntax(m_FileMetaMemberFunction->GetFileMetaBlockSyntax(), m_MetaBlockStatements);
        } else {
            nohasContent = true;
        }
    }
    if (!m_IsWithInterface || this->m_OwnerMetaClass->IsInterfaceClass()) {
    } else {
        if (nohasContent) {
            //Log::AddInStructMeta(EError::None, "Error 类[" + this->m_OwnerMetaClass->GetAllClassName() + "] 该函数[" + this->GetFunctionAllName() + "] 没有定义函数内容！！");
        }
    }
}

void MetaMemberFunction::UpdateVritualFunctionName() {
    std::ostringstream sb;
    sb << m_Name;
    sb << "_";
    if (m_ReturnMetaVariable != nullptr) {
        sb << m_ReturnMetaVariable->GetMetaDefineType()->ToFormatString();
    }
    sb << "_";
    if (m_MetaMemberParamCollection != nullptr) {
        sb << m_MetaMemberParamCollection->GetMaxParamCount();
    }
    if (m_MetaMemberParamCollection != nullptr && m_MetaMemberParamCollection->GetMaxParamCount() > 0) {
        sb << "_";
        for (int i = 0; i < m_MetaMemberParamCollection->GetMaxParamCount(); i++) {
            auto mdp = m_MetaMemberParamCollection->GetMetaDefineParamList()[i];
            sb << mdp->GetMetaVariable()->GetMetaDefineType()->ToString();
            if (i < m_MetaMemberParamCollection->GetMaxParamCount() - 1) {
                sb << "_";
            }
        }
    }
    m_VirtualFunctionName = sb.str();
}

MetaType* MetaMemberFunction::AddMetaPreTemplateFunction(MetaType* mt, bool& isGenMetaClass) {
    isGenMetaClass = false;
    if (mt->GetMetaClass() == nullptr) {
        return nullptr;
    }
    bool isIncludeTemplateClass = mt->IsIncludeClassTemplate(m_OwnerMetaClass);
    std::vector<MetaClass*> mcList;
    for (size_t i = 0; i < mt->GetTemplateMetaTypeList().size(); i++) {
        auto mtc = mt->GetTemplateMetaTypeList()[i];
        if (mtc->GetEType() == EMetaTypeType::MetaClass) {
            mcList.push_back(mtc->GetMetaClass());
        }
    }
    if (mcList.size() == mt->GetTemplateMetaTypeList().size()) {
        MetaGenTemplateClass* mgtc = mt->GetMetaClass()->AddInstanceMetaClass(mcList);
        isGenMetaClass = true;
        std::vector<MetaType*> mtlist;
        return new MetaType(mgtc, mtlist);
    }
    if (isIncludeTemplateClass) {
        auto find = FindBindStructTemplateFunctionAndClassMtList(mt);
        if (find == nullptr) {
            this->m_BindStructTemplateFunctionAndClassMtList.push_back(new MetaType(*mt));
        }
    } else {
        auto find = FindBindStructTemplateFunctionMtList(mt);
        if (find == nullptr) {
            this->m_BindStructTemplateFunctionMtList.push_back(new MetaType(*mt));
        }
    }
    return mt;
}

MetaType* MetaMemberFunction::FindBindStructTemplateFunctionMtList(MetaType* mt) {
    for (auto& v : m_BindStructTemplateFunctionMtList) {
        if (MetaType::EqualMetaDefineType(v, mt)) {
            return v;
        }
    }
    return nullptr;
}

MetaType* MetaMemberFunction::FindBindStructTemplateFunctionAndClassMtList(MetaType* mt) {
    for (auto& v : m_BindStructTemplateFunctionAndClassMtList) {
        if (MetaType::EqualMetaDefineType(v, mt)) {
            return v;
        }
    }
    return nullptr;
}

MetaStatements* MetaMemberFunction::CreateMetaSyntax(Compile::FileMetaSyntax* rootMs, MetaBlockStatements* currentBlockStatements) {
    MetaStatements* beforeStatements = currentBlockStatements;
    while (rootMs->IsNotEnd()) {
        auto childFms = rootMs->GetCurrentSyntaxAndMove();
        HandleMetaSyntax(currentBlockStatements, beforeStatements, childFms);
    }
    return beforeStatements;
}

MetaStatements* MetaMemberFunction::HandleMetaSyntax(MetaBlockStatements* currentBlockStatements, 
                                                   MetaStatements*& beforeStatements,
                                                   Compile::FileMetaSyntax* childFms) {
    // 根据C#源码的switch语句实现
    if (auto fmbs1 = dynamic_cast<FileMetaBlockSyntax*>(childFms)) {
        auto createBlockStatements = new MetaBlockStatements(currentBlockStatements, fmbs1);
        createBlockStatements->SetParent(currentBlockStatements);
        auto cms = CreateMetaSyntax(fmbs1, createBlockStatements);
        beforeStatements->SetNextStatements(createBlockStatements);
        beforeStatements = createBlockStatements;
    }
    else if (auto fmkis = dynamic_cast<FileMetaKeyIfSyntax*>(childFms)) {
        auto metaIfStatements = new MetaIfStatements(currentBlockStatements, fmkis);
        beforeStatements->SetNextStatements(metaIfStatements);
        beforeStatements = metaIfStatements;
    }
    else if (auto fmkss = dynamic_cast<FileMetaKeySwitchSyntax*>(childFms)) {
        auto metaSwitchStatements = new MetaSwitchStatements(currentBlockStatements, fmkss);
        beforeStatements->SetNextStatements(metaSwitchStatements);
        beforeStatements = metaSwitchStatements;
    }
    else if (auto fmkfs = dynamic_cast<FileMetaKeyForSyntax*>(childFms)) {
        auto metaForStatements = new MetaForStatements(currentBlockStatements, fmkfs);
        beforeStatements->SetNextStatements(metaForStatements);
        beforeStatements = metaForStatements;
    }
    else if (auto fmkes = dynamic_cast<FileMetaConditionExpressSyntax*>(childFms)) {
        // dowhile/while conditionvariable
        if (fmkes->GetToken()->GetType() == ETokenType::While || fmkes->GetToken()->GetType() == ETokenType::DoWhile) {
            auto metaWhileStatements = new MetaWhileDoWhileStatements(currentBlockStatements, fmkes);
            beforeStatements->SetNextStatements(metaWhileStatements);
            beforeStatements = metaWhileStatements;
        } else {
            //Log::AddInStructMeta(EError::None, "Error FileMetaConditionExpressSyntax: 暂不支持该类型的解析!!");
        }
    }
    else if (auto fmoks = dynamic_cast<FileMetaKeyOnlySyntax*>(childFms)) {
        if (fmoks->GetToken()->GetType() == ETokenType::Break) {
            auto metaBreakStatements = new MetaBreakStatements(currentBlockStatements, fmoks);
            beforeStatements->SetNextStatements(metaBreakStatements);
            beforeStatements = metaBreakStatements;
        } else if (fmoks->GetToken()->GetType() == ETokenType::Continue) {
            auto metaContinueStatements = new MetaContinueStatements(currentBlockStatements, fmoks);
            beforeStatements->SetNextStatements(metaContinueStatements);
            beforeStatements = metaContinueStatements;
        }
    }
    else if (auto fmos = dynamic_cast<FileMetaOpAssignSyntax*>(childFms)) {
        bool isDefineVarStatements = false;
        if (fmos->GetVariableRef()->IsOnlyName() ) {
            std::string name1 = fmos->GetVariableRef()->GetName();
            if (fmos->HasDefine() ) {
                if (currentBlockStatements->GetIsMetaVariable(name1)) {
                    //Log::AddInStructMeta(EError::None, "Error 如果使用了var/data/dynamic/int 等前缀，有重复定义的行为" + fmos->variableRef->ToTokenString());
                    isDefineVarStatements = false;
                } else {
                    isDefineVarStatements = true;
                }
            } else {
                if (!currentBlockStatements->GetIsMetaVariable(name1)) {
                    auto ownerclass = currentBlockStatements->GetOwnerMetaClass();
                    MetaBase* mb = ownerclass->GetMetaMemberVariableByName(name1);
                    if (mb == nullptr) {
                        isDefineVarStatements = true;
                    }
                }
            }
        }
        if (isDefineVarStatements) {
            auto mnvs11 = new MetaDefineVarStatements(currentBlockStatements, fmos);
            beforeStatements->SetNextStatements(mnvs11);
            beforeStatements = mnvs11;
        } else {
            auto mas = new MetaAssignStatements(currentBlockStatements, fmos);
            beforeStatements->SetNextStatements(mas);
            beforeStatements = mas;
        }
    }
    else if (auto fmvs = dynamic_cast<FileMetaDefineVariableSyntax*>(childFms)) {
        // x = 2;
        bool isDefineVarStatements = false;
        std::string name1 = fmvs->GetName();
        if (currentBlockStatements->GetIsMetaVariable(name1)) {
            isDefineVarStatements = true;
            //Log::AddInStructMeta(EError::None, "Error 定义变量名称与类函数临时名称一样!!" + (fmvs->token ? fmvs->token->ToLexemeAllString() : ""));
            return nullptr;
        } else {
            auto mv = currentBlockStatements->GetOwnerMetaClass()->GetMetaMemberVariableByName(name1);
            if (mv == nullptr) {
                isDefineVarStatements = true;
            } else {
                if (!mv->IsStatic()) {
                    //Log::AddInStructMeta(EError::None, "Error 定义变量名称与类定义名称一样 如果调用成员变量，需要在前边使用this.!!" + (fmvs->token ? fmvs->token->ToLexemeAllString() : ""));
                    return nullptr;
                }
            }
        }
        if (isDefineVarStatements) {
            auto mnvs11 = new MetaDefineVarStatements(currentBlockStatements, fmvs);
            beforeStatements->SetNextStatements(mnvs11);
            beforeStatements = mnvs11;
        } else {
            auto mas = new MetaAssignStatements(currentBlockStatements, fmvs);
            beforeStatements->SetNextStatements(mas);
            beforeStatements = mas;
        }
    }
    else if (auto fmcs = dynamic_cast<FileMetaCallSyntax*>(childFms)) {
        // a.value.SetH(100);
        auto mcs = new MetaCallStatements(currentBlockStatements, fmcs);
        beforeStatements->SetNextStatements(mcs);
        beforeStatements = mcs;
        return mcs;
    }
    else if (auto fmrs = dynamic_cast<FileMetaKeyReturnSyntax*>(childFms)) {
        // ret 100
        if (fmrs->GetToken() != nullptr  && fmrs->GetToken()->GetType() == ETokenType::Return) {
            auto mrs = new MetaReturnStatements(currentBlockStatements, fmrs);
            beforeStatements->SetNextStatements(mrs);
            beforeStatements = mrs;
            return mrs;
        } else if (fmrs->GetToken() != nullptr && fmrs->GetToken()->GetType() == ETokenType::Transience) {
            auto mtrs = new MetaTRStatements(currentBlockStatements, fmrs);
            beforeStatements->SetNextStatements(mtrs);
            beforeStatements = mtrs;
            return mtrs;
        } else {
            //Log::AddInStructMeta(EError::None, "Error 生成MetaStatements出错KeyReturnSyntax类型错误!!");
        }
    }
    else if (auto fmkgls = dynamic_cast<FileMetaKeyGotoLabelSyntax*>(childFms)) {
        // goto 1// label 1
        auto metaGotoStatements = new MetaGotoLabelStatements(currentBlockStatements, fmkgls);
        beforeStatements->SetNextStatements(metaGotoStatements);
        beforeStatements = metaGotoStatements;
        return metaGotoStatements;
    }
    else {
        //Log::AddInStructMeta(EError::None, "Waning 还有没有解析的语句!! MetaMemberFunction 314");
    }
    
    return nullptr;
}

bool MetaMemberFunction::Equals(MetaBase* obj) const {
    if (obj == nullptr) return false;

    MetaMemberFunction* rec = dynamic_cast<MetaMemberFunction*>(obj);
    if (rec == nullptr) return false;

    if (rec->GetName() == GetName() && rec->GetMetaMemberParamCollection()->IsEqualMetaDefineParamCollection(GetMetaMemberParamCollection())) {
        return true;
    }
    
    return false;
}

std::string MetaMemberFunction::ToString() const {
    std::ostringstream sb;
    if (m_ReturnMetaVariable != nullptr) {
        sb << m_ReturnMetaVariable->GetMetaDefineType()->ToFormatString();
    }
    sb << " ";
    
    if (m_OwnerMetaClass != nullptr) {
        sb << m_OwnerMetaClass->GetAllClassName();
        sb << ".";
    }
    sb << m_Name;
    if (m_MetaMemberTemplateCollection != nullptr && m_MetaMemberTemplateCollection->GetMetaTemplateList().size() > 0) {
        sb << "<";
        for (size_t i = 0; i < m_MetaMemberTemplateCollection->GetMetaTemplateList().size(); i++) {
            auto mtl = m_MetaMemberTemplateCollection->GetMetaTemplateList()[i];
            sb << mtl->GetName();
            if (i < m_MetaMemberTemplateCollection->GetMetaTemplateList().size() - 1) {
                sb << ",";
            }
        }
        sb << ">";
    }
    sb << "(";

    for (size_t i = 0; i < m_MetaMemberParamCollection->GetMetaDefineParamList().size(); i++) {
        MetaDefineParam* mpl = m_MetaMemberParamCollection->GetMetaDefineParamList()[i];
        sb << mpl->ToString();
        if (i < m_MetaMemberParamCollection->GetMetaDefineParamList().size() - 1) {
            sb << ",";
        }
    }
    sb << ")";

    return sb.str();
}

std::string MetaMemberFunction::ToFormatString() const {
    std::ostringstream sb;

    for (int i = 0; i < GetRealDeep(); i++)
        sb << Global::tabChar;

    sb << (int)GetPermission() << " ";
    if (IsStatic()) {
        sb << " static";
    }
    if (IsOverrideFunction()) {
        sb << " override";
    }
    if (IsGet()) {
        sb << " get";
    }
    if (IsSet()) {
        sb << " set";
    }
    if (IsWithInterface()) {
        sb << " interface";
    }
    sb << " ";
    if (m_ReturnMetaVariable != nullptr) {
        sb << m_ReturnMetaVariable->GetMetaDefineType()->ToFormatString();
    }
    sb << " " << GetName();
    sb << m_MetaMemberParamCollection->ToFormatString();
    sb << "\n";

    if (m_MetaBlockStatements != nullptr)
        sb << m_MetaBlockStatements->ToFormatString();

    sb << "\n";

    return sb.str();
}

int MetaMemberFunction::GetHashCode() const{
    return MetaBase::GetHashCode();
}

void MetaMemberFunction::Init() {
    m_ConstructInitFunction = (m_Name == "_init_");

    MetaType* defineMetaType = nullptr;
    if (m_ConstructInitFunction) {
        defineMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetVoidMetaClass());
    } else {
        defineMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    }
    if (IsSet() && !IsGet()) {
        defineMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetVoidMetaClass());
    }
    if (!IsStatic()) {
        MetaType* mt = new MetaType(m_OwnerMetaClass);
        if (m_OwnerMetaClass->IsTemplateClass()) {
            auto tt = new MetaTemplate(m_OwnerMetaClass, "this");
            tt->SetIndex(0);
            tt->SetInConstraintMetaClass(m_OwnerMetaClass);
            mt = new MetaType(tt);
        }
        m_ThisMetaVariable = new MetaVariable(m_OwnerMetaClass->GetAllClassName() + "." + m_Name + ".this", EVariableFrom::Argument, nullptr, m_OwnerMetaClass, mt);
    }
    m_ReturnMetaVariable = new MetaVariable(m_OwnerMetaClass->GetAllClassName() + "." + m_Name + ".define", EVariableFrom::Argument, nullptr, m_OwnerMetaClass, defineMetaType);
}

} // namespace Core
} // namespace SimpleLanguage