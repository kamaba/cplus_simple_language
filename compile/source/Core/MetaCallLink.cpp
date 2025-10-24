//****************************************************************************
//  File:      MetaCallLink.cpp
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/7/29 12:00:00
//  Description:  this's a common node handles
//****************************************************************************

#include "MetaCallLink.h"
#include "MetaCallNode.h"
#include "MetaClass.h"
#include "Statements/MetaBlockStatements.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "MetaType.h"
#include "MetaVariable.h"
#include "AllowUseSettings.h"
#include "../Compile/Token.h"
#include "MetaExpressNode/MetaExpressBase.h"
#include "MetaExpressNode/MetaExpressConst.h"
#include "../Compile/FileMeta/FileMetaCommon.h"
#include "../Compile/Token.h"
#include "../Debug/Log.h"
#include <sstream>
#include "MetaVisitCall.h"

using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Core {

MetaCallLink::MetaCallLink(SimpleLanguage::Compile::FileMetaCallLink* fmcl, MetaClass* metaClass, MetaBlockStatements* mbs, MetaType* frontDefineMt, MetaVariable* mv) {
    m_FileMetaCallLink = fmcl;
    m_OwnerMetaClass = metaClass;
    m_OwnerMetaBlockStatements = mbs;
    CreateCallLinkNode(frontDefineMt, mv);
}

void MetaCallLink::CreateCallLinkNode(MetaType* frontDefineMt, MetaVariable* mv) {
    MetaCallNode* frontMetaNode = nullptr;
    if (m_FileMetaCallLink->GetCallNodeList().size() > 0) {
        Compile::FileMetaCallNode* fmcn = m_FileMetaCallLink->GetCallNodeList()[0];
        auto firstNode = new MetaCallNode(nullptr, fmcn, m_OwnerMetaClass, m_OwnerMetaBlockStatements, frontDefineMt);
        frontMetaNode = firstNode;
        m_CallNodeList.push_back(firstNode);
    }
    
    for (size_t i = 1; i < m_FileMetaCallLink->GetCallNodeList().size(); i += 2) {
        auto cn1 = m_FileMetaCallLink->GetCallNodeList()[i];
        auto cn2 = m_FileMetaCallLink->GetCallNodeList()[i + 1];
        auto fmn = new MetaCallNode(cn1, cn2, m_OwnerMetaClass, m_OwnerMetaBlockStatements, frontDefineMt);
        fmn->SetFrontCallNode(frontMetaNode);
        m_CallNodeList.push_back(fmn);
        frontMetaNode = fmn;
    }
    
    auto m_FinalMetaCallNode = frontMetaNode;
    if (m_FinalMetaCallNode == nullptr) {
        Log::AddInStructMeta(EError::None, "Error 连接串没有找到合适的节点  360!!!");
    }
    m_FinalMetaCallNode->SetDefineMetaVariable(mv);
}

Token* MetaCallLink::GetToken() {
    return nullptr;
}

bool MetaCallLink::Parse(AllowUseSettings* _useConst) {
    m_AllowUseSettings = new AllowUseSettings(*_useConst);
    m_AllowUseSettings->SetSetterFunction(false) ;
    m_AllowUseSettings->SetGetterFunction(true);
    bool flag = true;
    
    for (size_t i = 0; i < m_CallNodeList.size(); i++) {
        if (flag) {
            if (i == m_CallNodeList.size() - 1) {
                m_AllowUseSettings->SetSetterFunction(_useConst->GetSetterFunction() );
                m_AllowUseSettings->SetGetterFunction( _useConst->GetGetterFunction() );
            }
            flag = m_CallNodeList[i]->ParseNode(m_AllowUseSettings);

            if (m_CallNodeList[i]->GetCallNodeType() == ECallNodeType::NewClass 
                || m_CallNodeList[i]->GetCallNodeType() == ECallNodeType::NewData) {
                if (i < m_CallNodeList.size() - 1) {
                    flag = false;
                    Log::AddInStructMeta(EError::None, "Parse Statement Error 在使用NewClassName的方式，后边不允许有其它的调用!");
                }
            }
        }
    }
    
    if (flag) {
        size_t i = 0;
        MetaCallNode* frontNode = nullptr;
        while (true) {
            if (i >= m_CallNodeList.size()) {
                break;
            }
            MetaCallNode* mcn = m_CallNodeList[i++];
            if (mcn == nullptr) {
                break;
            }
            
            if (mcn->GetCallNodeType() == ECallNodeType::This) {
                MetaVisitNode* mvn = MetaVisitNode::CreateByThis(mcn->GetMetaVariable());
                m_VisitNodeList.push_back(mvn);
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::Base) {
                MetaVisitNode* mvn = MetaVisitNode::CreateByBase(mcn->GetMetaVariable());
                m_VisitNodeList.push_back(mvn);
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::FunctionInnerVariableName) {
                MetaVisitNode* mvn = MetaVisitNode::CreateByVariable(mcn->GetMetaVariable(), mcn->GetMetaType());
                m_VisitNodeList.push_back(mvn);
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::MemberVariableName) {
                MetaVisitNode* mvn = MetaVisitNode::CreateByVariable(mcn->GetMetaVariable(), mcn->GetCallMetaType());
                m_VisitNodeList.push_back(mvn);
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::MemberFunctionName) {
                MetaVariable* newmv = nullptr;
                MetaMethodCall* mmc = nullptr;
                if (frontNode != nullptr && frontNode->GetCallNodeType() == ECallNodeType::ConstValue) {
                    MetaVisitNode* fvn = m_VisitNodeList[m_VisitNodeList.size() - 1];
                    m_VisitNodeList.pop_back();

                    std::string name = "auto_constvalue_" + fvn->constValueExpress()->GetETypeString() + "_" + fvn->constValueExpress()->GetValue().ToString();
                    newmv = frontNode->GetOwnerMetaFunctionBlock()->GetMetaVariable(name);
                    if (newmv == nullptr) {
                        auto mccm = CoreMetaClassManager::GetInstance().GetMetaClassByEType(fvn->constValueExpress()->GetEType());
                        newmv = new MetaVariable(name, EVariableFrom::LocalStatement,
                            frontNode->GetOwnerMetaFunctionBlock(), frontNode->GetMetaType()->GetMetaClass(), new MetaType(mccm));

                        frontNode->GetOwnerMetaFunctionBlock()->AddMetaVariable(newmv);
                    }

                    MetaVisitNode* mvn1 = MetaVisitNode::CraeteByNewClass(frontNode->GetMetaType(), nullptr, newmv);
                    m_VisitNodeList.push_back(mvn1);

                    mmc = new MetaMethodCall(mcn->GetCallMetaType()->GetMetaClass(), mcn->GetCallMetaType()->GetTemplateMetaTypeList(),
                        (MetaFunction*)mcn->GetMetaFunction(), mcn->GetMetaTemplateParamsList(), mcn->GetMetaInputParamCollection(), newmv, mcn->GetStoreMetaVariable());
                }
                else {
                    auto retmv = frontNode != nullptr ? frontNode->GetMetaVariable() : nullptr;
                    if (m_VisitNodeList.size() > 0 && retmv != nullptr) {
                        m_VisitNodeList.pop_back();
                    }
                    mmc = new MetaMethodCall(mcn->GetCallMetaType()->GetMetaClass(), mcn->GetCallMetaType()->GetTemplateMetaTypeList(),
                        (MetaFunction*)mcn->GetMetaFunction(), mcn->GetMetaTemplateParamsList(), mcn->GetMetaInputParamCollection(), retmv, mcn->GetStoreMetaVariable());
                }

                MetaVisitNode* mvn2 = MetaVisitNode::CreateByMethodCall(mmc);
                m_VisitNodeList.push_back(mvn2);
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::ConstValue) {
                MetaVisitNode* mvn = MetaVisitNode::CreateByConstExpress(dynamic_cast<MetaConstExpressNode*>(mcn->GetMetaExpressValue()), mcn->GetMetaVariable());
                m_VisitNodeList.push_back(mvn);
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::NewClass) {
                if (i == m_CallNodeList.size()) {
                    MetaVisitNode* mvn = MetaVisitNode::CraeteByNewClass(mcn->GetMetaType(), mcn->GetMetaBraceStatementsContent(), mcn->GetMetaVariable());
                    m_VisitNodeList.push_back(mvn);

                    if (mcn->GetMetaFunction() != nullptr) {
                        MetaFunction* mf = (MetaFunction*)(mcn->GetMetaFunction());
                        std::vector<MetaType*> svmeVec;
                        MetaMethodCall* mmc = new MetaMethodCall(mcn->GetMetaType()->GetMetaClass(), svmeVec, mf,
                            svmeVec, mcn->GetMetaInputParamCollection(), nullptr, mcn->GetStoreMetaVariable());
                        mvn->SetMethodCall(mmc);
                    }
                }
                else {
                    Log::AddInStructMeta(EError::None, "Error 使用NewClass方式，后边不允许跟其它变量相关内容!");
                }
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::NewTemplate) {
                MetaFunction* mf = (MetaFunction*)mcn->GetMetaFunction();
                MetaVisitNode* mvn = MetaVisitNode::CreateByNewTemplate(mcn->GetMetaType(), mf, mcn->GetStoreMetaVariable());
                MetaClass* cmc = mcn->GetMetaType()->GetMetaClass();
                std::vector<MetaType*> svmeVec;
                MetaMethodCall* mmc = new MetaMethodCall(mcn->GetMetaType()->GetMetaClass(), svmeVec, mf,
                    svmeVec, mcn->GetMetaInputParamCollection(), nullptr, mcn->GetStoreMetaVariable());
                mvn->SetMethodCall(mmc);
                m_VisitNodeList.push_back(mvn);
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::NewData) {
                MetaVisitNode* mvn = MetaVisitNode::CraeteByNewData(mcn->GetMetaType(), mcn->GetMetaBraceStatementsContent());
                m_VisitNodeList.push_back(mvn);
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::EnumName) {
                // Debug::Write("Meta Common Parse IteratorVariable----------------------------------------------------");
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::EnumValueArray) {
                MetaVisitNode* mvn = MetaVisitNode::CreateByEnumDefaultValue(mcn->GetMetaType(), mcn->GetMetaVariable());
                m_VisitNodeList.push_back(mvn);
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::VisitVariable) {
                MetaVisitNode* mvn = MetaVisitNode::CreateByVisitVariable(dynamic_cast<MetaVisitVariable*>(mcn->GetMetaVariable()));
                m_VisitNodeList.push_back(mvn);
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::IteratorVariable) {
                Log::AddInStructMeta(EError::None, "Meta Common Parse IteratorVariable----------------------------------------------------");
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::DataName) {
                MetaVisitNode* mvn = MetaVisitNode::CreateByVariable(mcn->GetMetaVariable());
                m_VisitNodeList.push_back(mvn);
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::EnumDefaultValue) {
                MetaVisitNode* mvn = MetaVisitNode::CreateByEnumDefaultValue(mcn->GetMetaType(), mcn->GetMetaVariable());
                m_VisitNodeList.push_back(mvn);
            }
            else if (mcn->GetCallNodeType() == ECallNodeType::MemberDataName) {
                MetaVisitNode* mvn = MetaVisitNode::CreateByVariable(mcn->GetMetaVariable());
                m_VisitNodeList.push_back(mvn);
                // Debug::Write("Meta Common Parse MemberDataName----------------------------------------------------");
            }
            frontNode = mcn;
        }
    }
    
    if (m_VisitNodeList.size() > 0) {
        m_FinalCallNode = m_VisitNodeList[m_VisitNodeList.size() - 1];
    }
    else {
        //Log::AddInStructMeta(EError::None, "Error 解析执行链出错");
        flag = false;
    }

    return flag;
}

int MetaCallLink::CalcParseLevel(int level) {
    for (size_t i = 0; i < m_VisitNodeList.size(); i++) {
        level = m_VisitNodeList[i]->CalcParseLevel(level);
    }
    return level;
}

void MetaCallLink::CalcReturnType() {
    for (size_t i = 0; i < m_VisitNodeList.size(); i++) {
        m_VisitNodeList[i]->CalcReturnType();
    }
}

MetaVariable* MetaCallLink::ExecuteGetMetaVariable() {
    return m_FinalCallNode != nullptr ? m_FinalCallNode->GetRetMetaVariable() : nullptr;
}

MetaClass* MetaCallLink::ExecuteGetMetaClass() {
    return m_FinalCallNode != nullptr ? m_FinalCallNode->GetMetaClass() : nullptr;
}

MetaExpressNode* MetaCallLink::GetMetaExpressNode() {
    // if (m_FinalMetaCallNode->GetCallNodeType() == ECallNodeType::ConstValue) {
    //     return new MetaConstExpressNode(EType::Int32, m_FinalMetaCallNode->GetConstValue());
    // }
    return nullptr;
}

MetaType* MetaCallLink::GetMetaDeineType() {
    MetaType* mt = nullptr;
    for (size_t i = 0; i < m_VisitNodeList.size(); i++) {
        mt = m_VisitNodeList[i]->GetMetaDefineType();
    }
    return mt;
}

std::string MetaCallLink::ToFormatString() const {
    std::stringstream sb;
    for (size_t i = 0; i < m_VisitNodeList.size(); i++) {
        sb << m_VisitNodeList[i]->ToFormatString();
        if (i < m_VisitNodeList.size() - 1) {
            sb << "  ->  ";
        }
    }
    return sb.str();
}

std::string MetaCallLink::ToTokenString() const {
    std::stringstream sb;
    sb << m_FileMetaCallLink->ToTokenString();
    return sb.str();
}

} // namespace Core
} // namespace SimpleLanguage