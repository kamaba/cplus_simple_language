//****************************************************************************
//  File:      MetaCallNode.cpp
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/5/17 12:00:00
//  Description:  this's a calllink's node handle
//****************************************************************************

#include "MetaCallNode.h"
#include "MetaType.h"
#include "MetaClass.h"
#include "MetaData.h"
#include "MetaEnum.h"
#include "MetaVariable.h"
#include "MetaMemberVariable.h"
#include "MetaMemberFunction.h"
#include "MetaMemberData.h"
#include "Statements/MetaBlockStatements.h"
#include "MetaInputParamCollection.h"
#include "MetaBraceOrBracketStatementsContent.h"
#include "MetaConstExpressNode.h"
#include "MetaVisitVariable.h"
#include "MetaMemberEnum.h"
#include "MetaGenTemplateClass.h"
#include "MetaGenTempalteFunction.h"
#include "MetaInputTemplateCollection.h"
#include "MetaCallLink.h"
#include "ExpressManager.h"
#include "TypeManager.h"
#include "ClassManager.h"
#include "ProjectManager.h"
#include "CoreMetaClassManager.h"
#include "../Compile/Token.h"
#include "../Compile/FileMeta/FileMetaBase.h"
#include "../Compile/FileMeta/FileMetaCommon.h"
#include "../Compile/FileMeta/FileMetaExpress.h"
#include "../Compile/FileMeta/FileMetaSyntax.h"
#include "../Debug/Log.h"
#include <sstream>

using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Core {

MetaCallNode::MetaCallNode() : MetaBase() {
    m_CallNodeSign = ECallNodeSign::Null;
    m_CallNodeType = ECallNodeType::Null;
}

MetaCallNode::MetaCallNode(FileMetaCallNode* fmcn1, FileMetaCallNode* fmcn2, MetaClass* mc, MetaBlockStatements* mbs, MetaType* fdmt) : MetaBase() {
    m_FileMetaCallSign = fmcn1;
    m_FileMetaCallNode = fmcn2;
    m_Token = m_FileMetaCallNode->GetToken();
    m_OwnerMetaClass = mc;
    m_OwnerMetaFunctionBlock = mbs;
    m_FrontDefineMetaType = fdmt;
    m_IsFunction = m_FileMetaCallNode->IsCallFunction();
    m_IsArray = m_FileMetaCallNode->IsArray();

    for (size_t i = 0; i < m_FileMetaCallNode->GetArrayNodeList().size(); i++) {
        MetaCallLink* cmcl = new MetaCallLink(m_FileMetaCallNode->GetArrayNodeList()[i], mc, mbs, fdmt, m_DefineMetaVariable);
        m_MetaArrayCallNodeList.push_back(cmcl);
    }
}

void MetaCallNode::SetFrontCallNode(MetaCallNode* mcn) {
    m_FrontCallNode = mcn;
}

void MetaCallNode::SetDefineMetaVariable(MetaVariable* mv) {
    this->m_DefineMetaVariable = mv;
}

void MetaCallNode::SetStoreMetaVariable(MetaVariable* mv) {
    this->m_StoreMetaVariable = mv;
}

bool MetaCallNode::ParseNode(AllowUseSettings* _auc) {
    m_AllowUseSettings = _auc;
    if (m_FileMetaCallSign != nullptr) {
        if (m_FileMetaCallSign->GetToken()->GetType() == ETokenType::Period) {
            m_CallNodeSign = ECallNodeSign::Period;
        }
        else if (m_FileMetaCallSign->GetToken()->GetType() == ETokenType::And) {
            m_CallNodeSign = ECallNodeSign::Pointer;
            Log::AddInStructMeta(EError::None, "Error MetaStatements Parse  不允许使用其它连接符!!");
            return false;
        }
        else {
            Log::AddInStructMeta(EError::None, "Error MetaStatements Parse  不允许使用其它连接符!!");
            return false;
        }
    }
    if (m_FileMetaCallNode == nullptr) {
        Log::AddInStructMeta(EError::None, "Error 定义原数据为空!! " + m_Token->ToLexemeAllString());
    }
    if (m_FileMetaCallNode->GetFileMetaParTerm() != nullptr && !m_IsFunction) {
        auto firstNode = m_FileMetaCallNode->GetFileMetaParTerm()->GetFileMetaExpressList()[0];
        if (firstNode == nullptr) {
            Log::AddInStructMeta(EError::None, "Error 不能使用输入()中的内容 0号位的没有内容!!");
        }
        else {
            CreateExpressParam cep;
            cep.ownerMBS = m_OwnerMetaFunctionBlock;
            cep.metaType = nullptr;
            cep.fme = firstNode;
            m_ExpressNode = ExpressManager::CreateExpressNode(cep);
            m_ExpressNode->CalcReturnType();
            m_MetaClass = nullptr;
            m_CallNodeType = ECallNodeType::Express;
            return true;
        }
    }
    return CreateCallNode();
}

bool MetaCallNode::CreateCallNode() {
    int tokenLine = m_FileMetaCallNode->GetToken() != nullptr ? m_FileMetaCallNode->GetToken()->GetSourceBeginLine() : -1;
    m_Name = m_FileMetaCallNode->GetName();

    std::string fatherName = m_FrontCallNode != nullptr ? m_FrontCallNode->GetName() : "";
    bool isAt = m_FileMetaCallNode->GetAtToken() != nullptr;
    bool isFirst = m_FrontCallNode == nullptr;
    int templateCount = static_cast<int>(m_FileMetaCallNode->GetInputTemplateNodeList().size());

    ETokenType etype = m_Token->GetType();
    ECallNodeType frontCNT = ECallNodeType::Null;

    for (size_t i = 0; i < m_MetaArrayCallNodeList.size(); i++) {
        m_MetaArrayCallNodeList[i]->Parse(m_AllowUseSettings);
    }
    if (m_FrontCallNode != nullptr) {
        frontCNT = m_FrontCallNode->GetCallNodeType();
    }

    if (m_IsFunction) {
        m_MetaInputParamCollection = new MetaInputParamCollection(m_FileMetaCallNode->GetFileMetaParTerm(), m_OwnerMetaClass, m_OwnerMetaFunctionBlock);
        m_MetaInputParamCollection->CaleReturnType();
    }

    if (!isFirst && frontCNT == ECallNodeType::Null) {
        Log::AddInStructMeta(EError::None, "Error 前边节点没有发现MetaBase!!");
        return false;
    }

    if (etype == ETokenType::Number || etype == ETokenType::String || etype == ETokenType::Boolean) {
        bool isNotConstValue = false;
        if (frontCNT == ECallNodeType::FunctionInnerVariableName || frontCNT == ECallNodeType::MemberVariableName || frontCNT == ECallNodeType::VisitVariable) {
            MetaVariable* mv = m_FrontCallNode->GetMetaVariable();
            if (mv->IsArray()) {
                if (isAt) {
                    std::string inputMVName = m_Name;
                    m_MetaVariable = mv->GetMetaVaraible(inputMVName);
                    if (m_MetaVariable == nullptr) {
                        m_MetaVariable = new MetaVisitVariable(inputMVName, m_OwnerMetaClass, m_OwnerMetaFunctionBlock, mv, nullptr);
                        mv->AddMetaVariable(m_MetaVariable);
                    }
                    isNotConstValue = true;
                }
                else {
                    Log::AddInStructMeta(EError::None, "Error 在Array.后边如果使用变量或者是数字常量，必须使用Array.$方式!!");
                }
            }
        }
        else if (frontCNT == ECallNodeType::MemberDataName) {
            MetaMemberData* mmd = dynamic_cast<MetaMemberData*>(m_FrontCallNode->GetMetaVariable());
            if (mmd != nullptr) {
                if (mmd->GetMemberDataType() == EMemberDataType::MemberArray) {
                    if (isAt) {
                        std::string inputMVName = m_Name;
                        m_MetaVariable = mmd->GetMemberDataByName(inputMVName);
                        if (m_MetaVariable == nullptr) {
                            m_MetaVariable = new MetaVisitVariable(inputMVName, m_OwnerMetaClass, m_OwnerMetaFunctionBlock, mmd, nullptr);
                            mmd->AddMetaVariable(m_MetaVariable);
                        }
                        isNotConstValue = true;
                    }
                    else {
                        Log::AddInStructMeta(EError::None, "Error 在Array.后边如果使用变量或者是数字常量，必须使用Array.$方式!!");
                    }
                }
            }
        }
        if (!isNotConstValue) {
            m_CallNodeType = ECallNodeType::ConstValue;
            m_ExpressNode = new MetaConstExpressNode(m_Token->GetEType(), m_Token->GetLexeme());
            m_MetaClass = CoreMetaClassManager::GetInstance().GetMetaClassByEType(m_Token->GetEType());
            m_MetaVariable = m_OwnerMetaFunctionBlock->GetMetaVariable(m_Token->GetHashCode().ToString());
            if (m_MetaVariable == nullptr) {
                m_MetaVariable = new MetaVariable(m_Token->GetHashCode().ToString(),
                    MetaVariable::EVariableFrom::LocalStatement, m_OwnerMetaFunctionBlock, m_OwnerMetaClass,
                    new MetaType(m_MetaClass));
                m_OwnerMetaFunctionBlock->AddMetaVariable(m_MetaVariable);
            }
        }
    }
    else if (etype == ETokenType::Global) {
        if (isFirst) {
            if (m_IsFunction) {
                Log::AddInStructMeta(EError::None, "Error 不允许global的函数形式!!");
            }
            else {
                m_MetaData = ProjectManager::GetInstance().GetGlobalData();
                m_CallNodeType = ECallNodeType::Global;
            }
        }
        else {
            Log::AddInStructMeta(EError::None, "Error  不允许 使用global 只有第一位置可以使用This关键字" + m_Token->ToLexemeAllString());
        }
    }
    else if (etype == ETokenType::New) {
        if (isFirst) {
            if (!m_IsFunction) {
                Log::AddInStructMeta(EError::None, "Error 不允许new是非函数形式出现!!" + m_Token->ToLexemeAllString());
            }
            else {
                if (m_FrontDefineMetaType == nullptr) {
                    Log::AddInStructMeta(EError::None, "Error 没有前置定义类型!!" + m_Token->ToLexemeAllString());
                    return false;
                }
                m_MetaType = m_FrontDefineMetaType;
                m_StoreMetaVariable = m_DefineMetaVariable;
                if (m_FrontDefineMetaType->GetEType() == EMetaTypeType::Template) {
                    m_MetaTemplate = m_FrontDefineMetaType->GetMetaTemplate();
                    m_MetaType = new MetaType(m_MetaTemplate);
                    m_CallNodeType = ECallNodeType::NewTemplate;
                    m_CallMetaType = new MetaType(m_MetaTemplate);
                    MetaMemberFunction* mmf = m_FrontDefineMetaType->GetMetaClass()->GetMetaMemberFunctionByNameAndInputTemplateInputParamCount("_init_", 0, m_MetaInputParamCollection);
                    if (mmf == nullptr) {
                        Log::AddInStructMeta(EError::None, "Error 没有找到 关于类中" + m_FrontDefineMetaType->GetMetaClass()->GetAllClassName() + "的_init_方法!)", m_Token);
                        return false;
                    }
                    this->m_MetaFunction = mmf;
                }
                else if (m_FrontDefineMetaType->GetEType() == EMetaTypeType::MetaClass) {
                    m_MetaClass = m_FrontDefineMetaType->GetMetaClass();
                    m_CallNodeType = ECallNodeType::NewClass;
                }
                else {
                    m_CallNodeType = ECallNodeType::NewTemplate;
                    m_MetaClass = m_FrontDefineMetaType->GetMetaClass();
                }
            }
        }
        else {
            Log::AddInStructMeta(EError::None, "Error 只有第一位置可以使用new关键字" + m_Token->ToLexemeAllString());
        }
    }
    else if (etype == ETokenType::This) {
        if (this->m_AllowUseSettings->parseFrom == EParseFrom::MemberVariableExpress) {
            Log::AddInStructMeta(EError::None, "Error 不允许在成员变量中使用this关键字" + m_Token->ToLexemeAllString());
        }
        if (this->m_AllowUseSettings->parseFrom == EParseFrom::InputParamExpress) {
            Log::AddInStructMeta(EError::None, "Error 不允许在输入变量中中使用this关键字" + m_Token->ToLexemeAllString());
        }
        if (isFirst) {
            if (m_IsFunction) {
                Log::AddInStructMeta(EError::None, "Error 不允许this的函数形式!!" + m_Token->ToLexemeAllString());
            }
            else {
                m_MetaClass = m_OwnerMetaClass;
                m_MetaVariable = dynamic_cast<MetaMemberFunction*>(m_OwnerMetaFunctionBlock->GetOwnerMetaFunction())->GetThisMetaVariable();
                m_CallNodeType = ECallNodeType::This;
            }
        }
        else {
            Log::AddInStructMeta(EError::None, "Error 只有第一位置可以使用This关键字" + m_Token->ToLexemeAllString());
        }
    }
    else if (etype == ETokenType::Base) {
        if (this->m_AllowUseSettings->parseFrom == EParseFrom::MemberVariableExpress) {
            Log::AddInStructMeta(EError::None, "Error 不允许在成员变量中使用base关键字" + m_Token->ToLexemeAllString());
        }
        if (this->m_AllowUseSettings->parseFrom == EParseFrom::InputParamExpress) {
            Log::AddInStructMeta(EError::None, "Error 不允许在输入变量中中使用base关键字" + m_Token->ToLexemeAllString());
        }

        MetaClass* parentClass = m_OwnerMetaClass->GetMetaNode()->GetParentNode()->GetMetaClassByTemplateCount(0);
        if (parentClass == nullptr) {
            Log::AddInStructMeta(EError::None, "Error 使用base没有找到父节点!!");
            return false;
        }

        if (isFirst) {
            if (m_IsFunction) {
                Log::AddInStructMeta(EError::None, "Error 不允许base的函数形式!!");
            }
            else {
                m_MetaClass = parentClass;
                m_CallNodeType = ECallNodeType::Base;
            }
        }
        else {
            Log::AddInStructMeta(EError::None, "Error 只有第一位置可以使用base关键字" + m_Token->ToLexemeAllString());
        }
    }
    else if (etype == ETokenType::Type) {
        auto selfClass = CoreMetaClassManager::GetInstance().GetCoreMetaClass(m_Name);
        if (selfClass != nullptr) {
            m_MetaClass = selfClass->GetMetaClassByTemplateCount(0);
            m_CallNodeType = ECallNodeType::TypeName;
        }
    }
    else if (etype == ETokenType::Identifier) {
        if (isFirst) {
            if (GetFirstNode(m_Name, m_OwnerMetaClass, static_cast<int>(m_FileMetaCallNode->GetInputTemplateNodeList().size())) == false) {
                return false;
            }
        }
        else {
            // 处理非第一个标识符的情况
            if (frontCNT == ECallNodeType::MetaNode) {
                MetaNode* mn = nullptr;
                if (m_FrontCallNode->GetMetaNode()->IsMetaNamespace()) {
                    if (m_FrontCallNode->GetMetaNode()->GetMetaNamespace()->GetRefFromType() == RefFromType::CSharp) {
                        mn = SimpleLanguage::CSharp::CSharpManager::FindAndCreateMetaNode(m_FrontCallNode->GetMetaNode(), m_Name);
                        if (mn->IsMetaClass()) {
                            m_MetaClass = mn->GetMetaClassByTemplateCount(0);
                            m_CallNodeType = ECallNodeType::ClassName;
                        }
                        else if (mn->IsMetaNamespace()) {
                            m_MetaNode = mn;
                            m_CallNodeType = ECallNodeType::MetaNode;
                        }
                    }
                }

                if (mn == nullptr) {
                    mn = m_FrontCallNode->GetMetaNode()->GetChildrenMetaNodeByName(m_Name);
                    if (mn != nullptr) {
                        if (mn->IsMetaNamespace() || mn->IsMetaModule()) {
                            m_MetaNode = mn;
                            m_CallNodeType = ECallNodeType::MetaNode;
                        }
                        else if (mn->IsMetaData()) {
                            m_MetaData = mn->GetMetaData();
                            m_CallNodeType = ECallNodeType::DataName;
                        }
                        else if (mn->IsMetaEnum()) {
                            m_MetaEnum = mn->GetMetaEnum();
                            m_MetaVariable = m_MetaEnum->GetMetaVariable();
                            m_CallNodeType = ECallNodeType::EnumName;
                        }
                        else if (mn->IsMetaClass()) {
                            m_MetaClass = mn->GetMetaClassByTemplateCount(static_cast<int>(m_FileMetaCallNode->GetInputTemplateNodeList().size()));
                            m_CallNodeType = ECallNodeType::ClassName;
                        }
                        else {
                            Log::AddInStructMeta(EError::None, "Error 没有发该RetMC的类别MetaCommon");
                        }
                    }
                }
            }
            else if (frontCNT == ECallNodeType::ClassName || frontCNT == ECallNodeType::MetaType) {
                if (GetFunctionOrVariableByOwnerClass(m_FrontCallNode->GetMetaClass(), m_Name) == false) {
                    return false;
                }
            }
            else if (frontCNT == ECallNodeType::Global || frontCNT == ECallNodeType::DataName) {
                m_MetaVariable = GetDataValueByMetaData(m_FrontCallNode->GetMetaData(), m_Name);
                m_CallNodeType = ECallNodeType::MemberDataName;
            }
            else if (frontCNT == ECallNodeType::MemberDataName) {
                auto md = dynamic_cast<MetaMemberData*>(m_FrontCallNode->GetMetaVariable());
                MetaMemberData* findMd = nullptr;
                if (md != nullptr) {
                    findMd = md->GetMemberDataByName(m_Name);
                }
                if (findMd == nullptr) {
                    Log::AddInStructMeta(EError::None, "Error 没有找到" + m_Name + " 的MetaData数据!");
                    return false;
                }
                if (findMd->GetMemberDataType() == EMemberDataType::MemberClass) {
                    m_MetaClass = m_MetaVariable->GetRealMetaType()->GetMetaClass();
                    m_CallNodeType = ECallNodeType::MemberVariableName;
                }
                else if (findMd->GetMemberDataType() == EMemberDataType::ConstValue) {
                    m_CallNodeType = ECallNodeType::ConstValue;
                }
                else if (findMd->GetMemberDataType() == EMemberDataType::MemberArray) {
                    m_MetaVariable = findMd;
                    m_CallNodeType = ECallNodeType::MemberDataName;
                }
                else {
                    m_CallNodeType = ECallNodeType::MemberDataName;
                }
            }
            else if (frontCNT == ECallNodeType::EnumName) {
                if (m_Name == "values") {
                    m_MetaVariable = m_FrontCallNode->GetMetaEnum()->GetMetaVariable();
                    if (m_MetaVariable == nullptr) {
                        m_FrontCallNode->GetMetaEnum()->CreateValues();
                        m_MetaVariable = m_FrontCallNode->GetMetaEnum()->GetMetaVariable();
                        if (m_MetaVariable == nullptr) {
                            return false;
                        }
                    }
                    m_CallNodeType = ECallNodeType::EnumValueArray;
                }
                else {
                    MetaMemberEnum* mme = m_FrontCallNode->GetMetaEnum()->GetMemberEnumByName(m_Name);
                    if (mme != nullptr) {
                        if (m_IsFunction) {
                            Log::AddInStructMeta(EError::None, "不能使用Enum.metaVariable(2) 这样的格式!");
                            return false;
                        }
                        else {
                            m_MetaVariable = mme;
                            m_CallNodeType = ECallNodeType::EnumDefaultValue;
                        }
                    }
                    else {
                        Log::AddInStructMeta(EError::None, "Error 不能使用Enum.xxxx未发现后续!");
                    }
                }
            }
            else if (frontCNT == ECallNodeType::FunctionInnerVariableName || frontCNT == ECallNodeType::MemberVariableName || frontCNT == ECallNodeType::VisitVariable) {
                MetaBase* tempMetaBase2 = nullptr;
                auto mv = m_FrontCallNode->GetMetaVariable();
                if (frontCNT == ECallNodeType::VisitVariable) {
                    mv = dynamic_cast<MetaVisitVariable*>(mv)->GetTargetMetaVariable();
                }
                MetaVariable* getmv2 = nullptr;
                if (mv->IsArray()) {
                    if (isAt) {
                        getmv2 = m_OwnerMetaFunctionBlock->GetMetaVariableByName(m_Name);
                        if (getmv2 != nullptr) {
                            std::string inputMVName = "Visit_" + m_Name;
                            m_MetaVariable = mv->GetMetaVaraible(inputMVName);
                            if (m_MetaVariable == nullptr) {
                                m_MetaVariable = new MetaVisitVariable(inputMVName, m_OwnerMetaClass, m_OwnerMetaFunctionBlock, mv, getmv2);
                                mv->AddMetaVariable(m_MetaVariable);
                            }
                            tempMetaBase2 = m_MetaVariable;
                            m_CallNodeType = ECallNodeType::VisitVariable;
                        }
                    }
                }

                if (tempMetaBase2 == nullptr) {
                    MetaClass* mc = mv->GetRealMetaType()->GetMetaClass();
                    if (dynamic_cast<MetaData*>(mc) != nullptr) {
                        MetaData* md = dynamic_cast<MetaData*>(mc);
                        auto retmmd = GetDataValueByMetaData(md, m_Name);
                        m_MetaVariable = retmmd;
                        if (retmmd == nullptr) {
                            Log::AddInStructMeta(EError::None, "Error 没有找到" + m_Name + " 的MetaData数据!");
                            return false;
                        }
                        if (retmmd->GetMemberDataType() == EMemberDataType::MemberClass) {
                            m_MetaClass = m_MetaVariable->GetRealMetaType()->GetMetaClass();
                            m_MetaVariable = retmmd;
                            m_CallNodeType = ECallNodeType::MemberVariableName;
                        }
                        else if (retmmd->GetMemberDataType() == EMemberDataType::ConstValue) {
                            m_CallNodeType = ECallNodeType::ConstValue;
                        }
                        else if (retmmd->GetMemberDataType() == EMemberDataType::MemberArray) {
                            m_CallNodeType = ECallNodeType::MemberDataName;
                        }
                        else {
                            m_CallNodeType = ECallNodeType::MemberDataName;
                        }
                    }
                    else if (dynamic_cast<MetaEnum*>(mc) != nullptr) {
                        MetaEnum* me = dynamic_cast<MetaEnum*>(mc);
                        m_MetaVariable = me->GetMemberVariableByName(m_Name);
                        m_CallNodeType = ECallNodeType::MemberVariableName;
                        m_FrontCallNode->SetStoreMetaVariable(m_MetaVariable);
                    }
                    else {
                        MetaClass* tmc = mv->GetRealMetaType()->GetEType() == EMetaTypeType::TemplateClassWithTemplate ? mv->GetRealMetaType()->GetMetaClass() : mv->GetRealMetaType()->GetMetaClass();
                        if (GetFunctionOrVariableByOwnerClass(tmc, m_Name) == false) {
                            return false;
                        }
                    }
                }
            }
            else if (frontCNT == ECallNodeType::This || frontCNT == ECallNodeType::Base || frontCNT == ECallNodeType::ConstValue) {
                if (GetFunctionOrVariableByOwnerClass(m_FrontCallNode->GetMetaClass(), m_Name) == false) {
                    return false;
                }
            }
            else if (frontCNT == ECallNodeType::Express) {
                if (GetFunctionOrVariableByOwnerClass(m_FrontCallNode->GetMetaClass(), m_Name) == false) {
                    return false;
                }
            }
            else if (frontCNT == ECallNodeType::MemberFunctionName) {
                MetaFunction* mf = m_FrontCallNode->GetMetaFunction();
                MetaType* retMT = mf->GetReturnMetaVariable()->GetRealMetaType();
                if (retMT != nullptr && retMT->GetMetaClass() != nullptr) {
                    if (GetFunctionOrVariableByOwnerClass(retMT->GetMetaClass(), m_Name) == false) {
                        return false;
                    }
                }
                else {
                    Log::AddInStructMeta(EError::None, "Error 函数没有返回类型");
                }
            }
            else if (frontCNT == ECallNodeType::TemplateName) {
                auto mt = m_FrontCallNode->GetMetaTemplate();
                if (mt != nullptr) {
                    if (mt->GetExtendsMetaClass() != nullptr) {
                        GetFunctionOrVariableByOwnerClass(mt->GetExtendsMetaClass(), m_Name);
                    }
                    else {
                        if (m_Name == "instance") {
                            m_MetaVariable = new MetaVariable("instance", MetaVariable::EVariableFrom::LocalStatement, m_OwnerMetaFunctionBlock, nullptr, nullptr);
                            m_CallNodeType = ECallNodeType::MemberVariableName;
                        }
                        else {
                            GetFunctionOrVariableByOwnerClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass(), m_Name);
                        }
                    }
                }
            }
            else {
                Log::AddInStructMeta(EError::None, "Error 暂不支持上节点的类型: " + std::to_string(static_cast<int>(frontCNT)));
            }
        }
    }

    if (m_CallNodeType == ECallNodeType::TemplateName) {
        // 处理模板名称的情况
    }

    if (m_FileMetaCallNode->GetInputTemplateNodeList().size() > 0) {
        MetaMemberFunction* tmf = dynamic_cast<MetaMemberFunction*>(m_OwnerMetaFunctionBlock->GetOwnerMetaFunction());
        if (m_MetaClass != nullptr || tmf != nullptr) {
            CreateMetaTemplateParams(m_MetaClass, tmf);
        }
    }

    if (m_CallNodeType == ECallNodeType::ClassName) {
        if (m_MetaTemplateParamsList.size() > 0) {
            auto ngmc = m_MetaClass->AddMetaTemplateClassByMetaClassAndMetaTemplateMetaTypeList(m_MetaTemplateParamsList);
            if (dynamic_cast<MetaGenTemplateClass*>(ngmc) != nullptr) {
                dynamic_cast<MetaGenTemplateClass*>(ngmc)->Parse();
                m_MetaClass = ngmc;
            }
            m_MetaType = new MetaType(m_MetaClass, m_MetaTemplateParamsList);
        }
    }
    else if (m_CallNodeType == ECallNodeType::MemberFunctionName) {
        if (dynamic_cast<MetaMemberFunction*>(m_MetaFunction) != nullptr) {
            auto mmf = dynamic_cast<MetaMemberFunction*>(m_MetaFunction);
            if (mmf->IsTemplateFunction()) {
                MetaClass* mcagm = m_MetaClass;
                if (m_FrontCallNode != nullptr) {
                    if (m_FrontCallNode->GetMetaClass() != nullptr) {
                        mcagm = m_FrontCallNode->GetMetaClass();
                    }
                    else if (m_FrontCallNode->GetMetaVariable() != nullptr) {
                        mcagm = m_FrontCallNode->GetMetaVariable()->GetRealMetaType()->GetMetaClass();
                    }
                }
                MetaGenTempalteFunction* mgtfind = mmf->AddGenTemplateMemberFunctionByMetaTypeList(mcagm, m_MetaTemplateParamsList);
                if (mgtfind != nullptr) {
                    m_MetaFunction = mgtfind;
                }
            }
        }
    }

    if (m_IsFunction) {
        if (m_CallNodeType == ECallNodeType::MetaNode) {
            Log::AddInStructMeta(EError::None, "Error 函数调用与命名空间冲突!!");
            return false;
        }
        else if (m_CallNodeType == ECallNodeType::MemberFunctionName) {
            if (this->m_DefineMetaVariable != nullptr && m_MetaFunction->GetReturnMetaVariable()->GetRealMetaType()->GetMetaClass() != CoreMetaClassManager::GetInstance().GetVoidMetaClass()) {
                this->m_StoreMetaVariable = this->m_DefineMetaVariable;
            }
            return true;
        }
        else if (m_MetaTemplate != nullptr) {
            m_CallNodeType = ECallNodeType::NewTemplate;
        }
        else if (m_MetaClass != nullptr) {
            MetaClass* curmc = m_MetaClass;
            if (this->m_IsArray) {
                curmc = CoreMetaClassManager::GetInstance().GetArrayMetaClass();
            }
            MetaMemberFunction* mmf = curmc->GetMetaMemberFunctionByNameAndInputTemplateInputParamCount("_init_", 0, m_MetaInputParamCollection);
            if (mmf == nullptr) {
                Log::AddInStructMeta(EError::None, "Error 没有找到 关于类中" + curmc->GetAllClassName() + "的_init_方法!)", m_Token);
                return false;
            }
            MetaType* retMt = m_MetaType;
            if (retMt != nullptr) {
                retMt->SetTemplateMetaClass(m_MetaClass);
            }
            if (m_DefineMetaVariable == nullptr) {
                m_MetaVariable = new MetaVariable("new_" + curmc->GetAllClassName() + "_" + std::to_string(curmc->GetHashCode()), MetaVariable::EVariableFrom::LocalStatement, m_OwnerMetaFunctionBlock, m_OwnerMetaClass, retMt);
                m_OwnerMetaFunctionBlock->AddMetaVariable(m_MetaVariable);
            }
            else {
                m_MetaVariable = m_DefineMetaVariable;
            }
            this->m_MetaClass = curmc;
            m_MetaFunction = mmf;
            if ((m_CallNodeType != ECallNodeType::NewTemplate) && (m_CallNodeType != ECallNodeType::NewClass)) {
                m_CallNodeType = ECallNodeType::NewClass;
            }

            if (m_FileMetaCallNode->GetFileMetaBraceTerm() != nullptr) {
                if (m_AllowUseSettings->parseFrom == EParseFrom::InputParamExpress) {
                    Log::AddInStructMeta(EError::None, "Error 在InputParam 里边，构建函数，只允许 使用ClassName() 的方式, 不允许使用 ClassName(){}的方式" + m_FileMetaCallNode->GetFileMetaBraceTerm()->ToTokenString());
                    return false;
                }
                m_MetaBraceStatementsContent = new MetaBraceOrBracketStatementsContent(m_FileMetaCallNode->GetFileMetaBraceTerm(), m_OwnerMetaFunctionBlock, m_OwnerMetaClass);
                m_MetaBraceStatementsContent->SetMetaType(new MetaType(curmc));
                m_MetaBraceStatementsContent->Parse();
            }
        }
        else if (m_MetaData != nullptr) {
            m_CallNodeType = ECallNodeType::NewData;
            if (m_FileMetaCallNode->GetFileMetaBraceTerm() != nullptr) {
                if (m_AllowUseSettings->parseFrom == EParseFrom::InputParamExpress) {
                    Log::AddInStructMeta(EError::None, "Error 在InputParam 里边，构建函数，只允许 使用ClassName() 的方式, 不允许使用 ClassName(){}的方式" + m_FileMetaCallNode->GetFileMetaBraceTerm()->ToTokenString());
                    return false;
                }
                m_MetaBraceStatementsContent = new MetaBraceOrBracketStatementsContent(m_FileMetaCallNode->GetFileMetaBraceTerm(), m_OwnerMetaFunctionBlock, m_OwnerMetaClass);
                m_MetaBraceStatementsContent->SetMetaType(new MetaType(m_MetaData));
                m_MetaBraceStatementsContent->Parse();
            }
        }
        else if (m_MetaEnum != nullptr) {
            // 处理枚举的情况
        }
        else if (m_MetaFunction != nullptr) {
            // 处理函数的情况
        }
        else {
            Log::AddInStructMeta(EError::None, "Error 使用函数调用与当前节点不吻合!!");
            return false;
        }
    }
    else {
        if (m_MetaVariable != nullptr) {
            auto tmv = m_MetaVariable;
            if (m_AllowUseSettings->useNotStatic == false && m_MetaVariable->IsStatic()) {
                if (frontCNT == ECallNodeType::FunctionInnerVariableName || frontCNT == ECallNodeType::MemberVariableName || frontCNT == ECallNodeType::This || frontCNT == ECallNodeType::Base) {
                    Log::AddInStructMeta(EError::None, "Error 1 静态调用，不能调用非静态字段!!");
                    return false;
                }
            }
            if (m_MetaVariable->IsArray()) {
                if (m_MetaArrayCallNodeList.size() == 1 && this->m_IsArray) {
                    MetaCallNode* fmcn1 = nullptr;
                    if (fmcn1 != nullptr && fmcn1->GetCallNodeType() == ECallNodeType::ConstValue) {
                        std::string tname = dynamic_cast<MetaConstExpressNode*>(fmcn1->GetMetaExpressValue())->GetValue().ToString();
                        m_MetaVariable = m_MetaVariable->GetMetaVaraible(tname);
                        if (m_MetaVariable == nullptr) {
                            m_MetaVariable = new MetaVisitVariable(tname, m_OwnerMetaClass, m_OwnerMetaFunctionBlock, m_MetaVariable, nullptr);
                            tmv->AddMetaVariable(m_MetaVariable);
                        }
                    }
                    else if (fmcn1 != nullptr && fmcn1->GetCallNodeType() == ECallNodeType::FunctionInnerVariableName) {
                        auto gmv = fmcn1->GetMetaVariable();
                        std::string tname = "VarName_" + gmv->GetName() + "_VarHashCode_" + std::to_string(gmv->GetHashCode());
                        m_MetaVariable = tmv->GetMetaVaraible(tname);
                        if (m_MetaVariable == nullptr) {
                            m_MetaVariable = new MetaVisitVariable(tname, m_OwnerMetaClass, m_OwnerMetaFunctionBlock, tmv, gmv);
                            tmv->AddMetaVariable(m_MetaVariable);
                        }
                    }
                    m_CallNodeType = ECallNodeType::VisitVariable;
                }
            }
        }
        else if (dynamic_cast<MetaClass*>(m_MetaClass) != nullptr) {
            if (this->m_IsArray) {
                if (m_MetaArrayCallNodeList.size() > 0) {
                    if (m_MetaArrayCallNodeList.size() > 3) {
                        Log::AddInStructMeta(EError::None, "Error 数组不能超过三维!!");
                    }
                    m_MetaInputParamCollection = new MetaInputParamCollection(m_OwnerMetaClass, m_OwnerMetaFunctionBlock);
                    // 处理数组维度的逻辑
                }
            }
        }
        else if (m_MetaData != nullptr) {
            // 处理数据的情况
        }
        else if (m_MetaFunction != nullptr) {
            // 处理函数的情况
        }
        else if (m_MetaEnum != nullptr) {
            // 处理枚举的情况
        }
        else if (m_MetaNode != nullptr) {
            // 处理节点的情况
        }
        else if (m_MetaTemplate != nullptr) {
            // 处理模板的情况
        }
        else {
            Log::AddInStructMeta(EError::None, "Error !! 非函数类型!!" + m_FileMetaCallNode->GetToken()->ToLexemeAllString());
        }
    }
    return true;
}

bool MetaCallNode::GetFirstNode(const std::string& inputname, MetaClass* mc, int count) {
    if (m_AllowUseSettings->parseFrom == EParseFrom::MemberVariableExpress) {
        // 处理成员变量表达式的情况
    }
    MetaNode* retMC = nullptr;
    
    if (m_Token->GetExtend() != nullptr) {
        EType etype = EType::None;
        // 尝试解析扩展类型
        auto retMC2 = CoreMetaClassManager::GetInstance().GetMetaClassByEType(etype);
        if (retMC2 != nullptr) {
            retMC = retMC2->GetMetaNode();
        }
    }
    
    if (retMC == nullptr) {
        auto t = mc->GetMetaTemplateByName(inputname);
        if (t != nullptr) {
            m_MetaTemplate = t;
            m_CallNodeType = ECallNodeType::TemplateName;
            return true;
        }
    }
    
    if (retMC == nullptr) {
        retMC = ClassManager::GetInstance().GetMetaClassByNameAndFileMeta(m_OwnerMetaClass, m_FileMetaCallNode->GetFileMeta(), {inputname});
    }
    
    if (retMC == nullptr) {
        retMC = mc->GetMetaNode()->GetChildrenMetaNodeByName(inputname);
    }
    
    if (retMC != nullptr) {
        if (retMC->IsMetaModule() || retMC->IsMetaNamespace()) {
            m_MetaNode = retMC;
            m_CallNodeType = ECallNodeType::MetaNode;
        }
        else if (retMC->IsMetaData()) {
            m_MetaData = retMC->GetMetaData();
            m_CallNodeType = ECallNodeType::DataName;
        }
        else if (retMC->IsMetaEnum()) {
            m_MetaEnum = retMC->GetMetaEnum();
            m_MetaVariable = m_MetaEnum->GetMetaVariable();
            m_CallNodeType = ECallNodeType::EnumName;
        }
        else if (retMC->IsMetaClass()) {
            m_MetaClass = retMC->GetMetaClassByTemplateCount(count);
            m_CallNodeType = ECallNodeType::ClassName;
            if (m_MetaClass == nullptr) {
                Log::AddInStructMeta(EError::None, "找到" + retMC->GetAllName() + " 里边模板数据为" + std::to_string(count) + " 没有找到相关的类!");
                return false;
            }
            m_MetaType = new MetaType(m_MetaClass);
        }
        else {
            Log::AddInStructMeta(EError::None, "Error 没有发该RetMC的类别MetaCommon");
        }
    }
    else {
        auto mmv = mc->GetMetaMemberVariableByName(inputname);
        if (mmv != nullptr) {
            if (mmv->IsStatic()) {
                m_MetaVariable = mmv;
                m_MetaClass = mc;
                m_MetaType = mmv->GetRealMetaType();
                std::vector<MetaType*> mtList;
                for (size_t i = 0; i < mmv->GetOwnerMetaClass()->GetMetaTemplateList().size(); i++) {
                    mtList.push_back(new MetaType(mmv->GetOwnerMetaClass()->GetMetaTemplateList()[i]));
                }
                m_CallMetaType = new MetaType(mmv->GetOwnerMetaClass(), mtList);
                m_CallNodeType = ECallNodeType::MemberVariableName;
                return true;
            }
            else {
                Log::AddInStructMeta(EError::None, "第一位的成员变量名称必须是个静态变量才可以哇!");
                return false;
            }
        }
        auto mmf = mc->GetFirstMetaMemberFunctionByName(inputname);
        if (mmf != nullptr) {
            if (mmf->IsStatic()) {
                m_MetaFunction = mmf;
                m_MetaClass = mc;
                std::vector<MetaType*> mtList;
                for (size_t i = 0; i < mmv->GetOwnerMetaClass()->GetMetaTemplateList().size(); i++) {
                    mtList.push_back(new MetaType(mmv->GetOwnerMetaClass()->GetMetaTemplateList()[i]));
                }
                m_CallMetaType = new MetaType(mmv->GetOwnerMetaClass(), mtList);
                m_CallNodeType = ECallNodeType::MemberFunctionName;
                return true;
            }
            else {
                Log::AddInStructMeta(EError::None, "第一位的成员函数名称必须是个静态函数才可以哇!");
                return false;
            }
        }
    }

    if (retMC == nullptr) {
        MetaVariable* mv = m_OwnerMetaFunctionBlock != nullptr ? m_OwnerMetaFunctionBlock->GetMetaVariableByName(inputname) : nullptr;
        if (mv != nullptr) {
            m_MetaVariable = mv;
            m_CallNodeType = ECallNodeType::FunctionInnerVariableName;
        }
        auto ownerFun = m_OwnerMetaFunctionBlock != nullptr ? m_OwnerMetaFunctionBlock->GetOwnerMetaFunction() : nullptr;
        if (ownerFun != nullptr) {
            auto metaDefineParam = ownerFun->GetMetaDefineParamByName(inputname);
            if (metaDefineParam != nullptr) {
                if (metaDefineParam->GetMetaVariable() != nullptr) {
                    m_MetaVariable = mv;
                    m_CallNodeType = ECallNodeType::FunctionInnerVariableName;
                }
            }
        }
    }
    return true;
}

MetaBase* MetaCallNode::HandleCastFunction(MetaClass* mc) {
    return nullptr;
}

MetaMemberData* MetaCallNode::GetDataValueByMetaData(MetaData* md, const std::string& inputName) {
    return md->GetMemberDataByName(inputName);
}

MetaMemberData* MetaCallNode::GetDataValueByMetaMemberData(MetaMemberData* md, const std::string& inputName) {
    return md->GetMemberDataByName(inputName);
}

bool MetaCallNode::CreateMetaTemplateParams(MetaClass* mc, MetaMemberFunction* mmf) {
    for (size_t i = 0; i < m_FileMetaCallNode->GetInputTemplateNodeList().size(); i++) {
        auto itnlc = m_FileMetaCallNode->GetInputTemplateNodeList()[i];
        auto ct = TypeManager::GetInstance().RegisterTemplateDefineMetaTemplateFunction(m_OwnerMetaClass, mc, mmf, itnlc, true);
        if (ct != nullptr) {
            m_MetaTemplateParamsList.push_back(ct);
        }
        else {
            Log::AddInStructMeta(EError::None, "没有发现实体的模板类!!" + (m_MetaClass != nullptr ? m_MetaClass->GetName() : ""));
            return false;
        }
    }
    return true;
}

bool MetaCallNode::GetFunctionOrVariableByOwnerClass(MetaClass* mc, const std::string& inputname) {
    MetaMemberVariable* mmv = nullptr;
    MetaMemberFunction* mmf = nullptr;
    if (m_IsFunction) {
        if (inputname == "cast") {
            HandleCastFunction(mc);
        }
        mmf = mc->GetMetaMemberFunctionByNameAndInputTemplateInputParamCount(inputname, static_cast<int>(m_FileMetaCallNode->GetInputTemplateNodeList().size()), m_MetaInputParamCollection, true);
    }
    else {
        mmv = mc->GetMetaMemberVariableByName(inputname);
        if (mmv == nullptr) {
            mmf = mc->GetMetaDefineGetSetMemberFunctionByName(inputname, m_MetaInputParamCollection,
                m_AllowUseSettings->getterFunction,
                m_AllowUseSettings->setterFunction);
        }
    }

    if (mmv == nullptr && mmf == nullptr) {
        Log::AddInStructMeta(EError::None, "Error 设置notStatic时，没有找到相应的变量!" + (m_Token != nullptr ? m_Token->ToLexemeAllString() : ""));
        return false;
    }

    if (mmv != nullptr) {
        m_MetaVariable = mmv;
        m_MetaType = mmv->GetRealMetaType();
        m_CallMetaType = new MetaType(mmv->GetOwnerMetaClass());
        m_CallNodeType = ECallNodeType::MemberVariableName;
    }
    else if (mmf != nullptr) {
        m_MetaFunction = mmf;
        m_MetaType = mmf->GetReturnMetaVariable()->GetRealMetaType();
        m_CallMetaType = new MetaType(mmf->GetOwnerMetaClass());
        m_CallNodeType = ECallNodeType::MemberFunctionName;
    }
    return true;
}

std::string MetaCallNode::ToString() const {
    return ToFormatString();
}

std::string MetaCallNode::ToFormatString() const {
    std::stringstream sb;

    if (m_CallNodeSign == ECallNodeSign::Period) {
        sb << ".";
    }
    if (m_CallNodeType == ECallNodeType::Express) {
        sb << m_ExpressNode->ToFormatString();
    }
    else {
        if (m_CallNodeType == ECallNodeType::ClassName) {
            if (m_CallNodeSign == ECallNodeSign::Null) {
                sb << (m_MetaClass != nullptr ? m_MetaClass->GetAllClassName() : "");
            }
            else {
                sb << (m_MetaClass != nullptr ? m_MetaClass->GetName() : "");
            }
        }
        else if (m_CallNodeType == ECallNodeType::EnumName) {
            sb << (m_MetaEnum != nullptr ? m_MetaEnum->GetAllClassName() : "");
        }
        else if (m_CallNodeType == ECallNodeType::EnumDefaultValue) {
            sb << (m_MetaVariable != nullptr ? m_MetaVariable->GetName() : "");
        }
        else if (m_CallNodeType == ECallNodeType::DataName) {
            sb << (m_MetaData != nullptr ? m_MetaData->GetAllClassName() : "");
        }
        else if (m_CallNodeType == ECallNodeType::MemberDataName) {
            sb << (m_MetaVariable != nullptr ? m_MetaVariable->GetName() : "");
        }
        else if (m_CallNodeType == ECallNodeType::NewClass) {
            sb << (m_MetaClass != nullptr ? m_MetaClass->ToFormatString() : "");
        }
        else if (m_CallNodeType == ECallNodeType::MetaNode) {
            sb << (m_MetaNode != nullptr ? m_MetaNode->GetName() : "");
        }
        else if (m_CallNodeType == ECallNodeType::MemberFunctionName) {
            sb << (m_MetaFunction != nullptr ? m_MetaFunction->ToString() : "");
        }
        else if (m_CallNodeType == ECallNodeType::FunctionInnerVariableName) {
            sb << (m_MetaVariable != nullptr ? m_MetaVariable->GetName() : "");
        }
        else if (m_CallNodeType == ECallNodeType::VisitVariable) {
            sb << (m_MetaVariable != nullptr ? m_MetaVariable->ToString() : "");
        }
        else if (m_CallNodeType == ECallNodeType::MemberVariableName) {
            sb << (m_MetaVariable != nullptr ? m_MetaVariable->GetName() : "");
        }
        else if (m_CallNodeType == ECallNodeType::This) {
            sb << "this";
        }
        else if (m_CallNodeType == ECallNodeType::Base) {
            sb << "base";
        }
        else if (m_CallNodeType == ECallNodeType::Global) {
            sb << "global";
        }
        else if (m_CallNodeType == ECallNodeType::MetaType) {
            sb << (m_MetaType != nullptr ? m_MetaType->ToString() : "");
        }
        else if (m_CallNodeType == ECallNodeType::ConstValue) {
            sb << (m_ExpressNode != nullptr ? m_ExpressNode->ToString() : "");
        }
        else {
            sb << (m_Token != nullptr ? m_Token->GetLexeme().ToString() : "") + "Error(CurrentMetaBase is Null!)";
        }
    }
    return sb.str();
}

} // namespace Core
} // namespace SimpleLanguage