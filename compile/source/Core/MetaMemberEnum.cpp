//****************************************************************************
//  File:      MetaMemberEnum.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2025/1/17 12:00:00
//  Description: enum's memeber variable metadata and member 'data' metadata implementation
//****************************************************************************

#include "MetaMemberEnum.h"
#include "MetaVariable.h"
#include "MetaExpressNode/MetaExpressBase.h"
#include "MetaExpressNode/MetaExpressConst.h"
#include "MetaExpressNode/MetaExpressCallLink.h"
#include "MetaExpressNode/MetaExpressNewObject.h"
#include "MetaType.h"
#include "MetaClass.h"
#include "../Compile/FileMeta/FileMetaMemberData.h"
#include "../Compile/FileMeta/FileMetaMemberVariable.h"
#include "../Debug/Log.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "ExpressManager.h"
#include "AllowUseSettings.h"
#include "MetaParam.h"
#include "MetaTemplate.h"
#include "MetaMemberFunction.h"
#include <vector>
#include <string>
#include <sstream>

using namespace SimpleLanguage::Compile;
using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Core {

// Static member initialization
int MetaMemberEnum::s_ConstLevel = 10000000;
int MetaMemberEnum::s_IsHaveRetStaticLevel = 100000000;
int MetaMemberEnum::s_NoHaveRetStaticLevel = 200000000;
int MetaMemberEnum::s_DefineMetaTypeLevel = 1000000000;
int MetaMemberEnum::s_ExpressLevel = 1500000000;

MetaMemberEnum::MetaMemberEnum(MetaClass* mc, Compile::FileMetaMemberVariable* fmmv) {
    m_FileMetaMemeberVariable = fmmv;
    m_Name = fmmv->GetName();
    AddPingToken(fmmv->NameToken());
    m_Index = mc->GetMetaMemberVariableDict().size();
    m_FromType = EFromType::Code;
    m_DefineMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_IsStatic = true; // enum ��Աȫ��Ϊstatic
    m_VariableFrom = EVariableFrom::Static;
    if (fmmv->StaticToken() != nullptr) {
        Log::AddInStructMeta(EError::None, "Error ENum�У��������о�̬�ؼ��֣�����ȫ���Ǿ�̬�ؼ���!!");
    }
    if (m_FileMetaMemeberVariable->PermissionToken() != nullptr) {
        Log::AddInStructMeta(EError::None, "Error Enum�У�������ʹ��public/private��Ȩ�޹ؼ���!!");
        m_Permission = CompilerUtil::GetPerMissionByString(m_FileMetaMemeberVariable->PermissionToken()->GetLexeme().ToString());
    }

    SetOwnerMetaClass(mc);
}

void MetaMemberEnum::ParseDefineMetaType() {
    if (m_FileMetaMemeberVariable != nullptr) {
        if (m_FileMetaMemeberVariable->GetExpress() != nullptr) {
            CreateExpressParam cep;
            cep.fme = m_FileMetaMemeberVariable->GetExpress();
            cep.metaType = m_DefineMetaType;
            cep.ownerMetaClass = m_OwnerMetaClass;
            cep.equalMetaVariable = this;
            cep.ownerMBS = m_OwnerMetaBlockStatements;
            cep.SetParseFrom( EParseFrom::MemberVariableExpress );
            m_Express = ExpressManager::CreateExpressNodeByCEP(cep);

            if (m_Express == nullptr) {
                Log::AddInStructMeta(EError::None, "Error û�н�����Express������ ��MetaMemberData ��� 372");
            }
        }
    }
}

bool MetaMemberEnum::ParseMetaExpress() {
    if (m_Express != nullptr) {
        AllowUseSettings auc;
        auc.SetParseFrom( EParseFrom::MemberVariableExpress );
        m_Express->Parse(&auc);
    }
    return true;
}
MetaConstExpressNode* MetaMemberEnum::GetConstExpressNode() const { return dynamic_cast<MetaConstExpressNode*>(m_Express); }

void MetaMemberEnum::SetExpress(MetaConstExpressNode* mcen) {
    m_Express = mcen;
}

std::string MetaMemberEnum::ToFormatString() const {
    std::stringstream sb;

    for (int i = 0; i < GetRealDeep(); i++)
        sb << Global::GetTabChar();

    sb << (int)m_Permission << " ";
    if (IsConst()) {
        sb << "const ";
    }
    if (IsStatic()) {
        sb << "static ";
    }
    sb << MetaVariable::ToFormatString();
    if (m_Express != nullptr) {
        sb << " = ";
        sb << m_Express->ToFormatString();
    }
    sb << ";";

    return sb.str();
}

std::string MetaMemberEnum::ToTokenString() {
    std::stringstream sb;

    sb << m_FileMetaMemeberVariable->NameToken()->GetSourceBeginLine() << " �븸���Tokenλ��: "
       << m_FileMetaMemeberVariable->NameToken()->GetSourceBeginLine();

    return sb.str();
}

MetaExpressNode* MetaMemberEnum::CreateExpressNodeInClassMetaVariable() {
    auto express = m_FileMetaMemeberVariable->GetExpress();
    if (express == nullptr) return nullptr;

    auto root = express->GetRoot();
    if (root == nullptr)
        return nullptr;
    if (root->GetLeft() == nullptr && root->GetRight() == nullptr) {
        auto fmpt = dynamic_cast<FileMetaParTerm*>(root);
        auto fmct = dynamic_cast<FileMetaCallTerm*>(root);
        auto fmbt = dynamic_cast<FileMetaBraceTerm*>(root);
        if (m_DefineMetaType != nullptr) {
            if (fmpt != nullptr) {
                if (m_IsSupportConstructionFunctionOnlyParType) {
                    MetaInputParamCollection mpc(fmpt, GetOwnerMetaClass(), nullptr);

                    MetaMemberFunction* mmf = m_DefineMetaType->GetMetaMemberConstructFunction(&mpc);

                    if (mmf == nullptr) return nullptr;

                    MetaNewObjectExpressNode* mnoen = new MetaNewObjectExpressNode(fmpt, m_DefineMetaType, GetOwnerMetaClass(), mmf->GetMetaBlockStatements());
                    if (mnoen != nullptr) {
                        return mnoen;
                    }
                } else {
                    Log::AddInStructMeta(EError::None, "Error ���������У���֧�ֳ�Ա������ʹ�����()���췽ʽ!!");
                }
            } else if (fmbt != nullptr) {
                MetaNewObjectExpressNode* mnoen = new MetaNewObjectExpressNode(fmbt, m_DefineMetaType, GetOwnerMetaClass(), nullptr, nullptr);
                return mnoen;
            } else if (fmct != nullptr) {
                if (fmct->GetCallLink()->GetCallNodeList().size() > 0) {
                    auto finalNode = fmct->GetCallLink()->GetCallNodeList()[fmct->GetCallLink()->GetCallNodeList().size() - 1];
                    if (finalNode->GetFileMetaBraceTerm() != nullptr && !m_IsSupportConstructionFunctionConnectBraceType) {
                        Log::AddInStructMeta(EError::None, "Error ��������У������� ʹ��Class()���{}�ĸ�ֵ��ʽ!!" + fmbt->GetToken()->ToLexemeAllString());
                        return nullptr;
                    }
                }
                AllowUseSettings auc;
                auc.SetUseNotConst( false );
                auc.SetUseNotStatic( true );
            }
        } else {
            if (fmpt != nullptr) {
                Log::AddInStructMeta(EError::None, "Error ����û�ж���ı����У������� ʹ��()�ĸ�ֵ��ʽ!!" + fmbt->GetToken()->ToLexemeAllString());
                return nullptr;
            } else if (fmbt != nullptr) {
                Log::AddInStructMeta(EError::None, "Error ����û�ж���ı����У������� ʹ��{}�ĸ�ֵ��ʽ!!" + fmbt->GetToken()->ToLexemeAllString());
                return nullptr;
            } else if (fmct != nullptr) {
                if (fmct->GetCallLink()->GetCallNodeList().size() > 0) {
                    auto finalNode = fmct->GetCallLink()->GetCallNodeList()[fmct->GetCallLink()->GetCallNodeList().size() - 1];
                    if (finalNode->GetFileMetaBraceTerm() != nullptr && !m_IsSupportConstructionFunctionConnectBraceType) {
                        Log::AddInStructMeta(EError::None, "Error ��������У������� ʹ��Class()���{}�ĸ�ֵ��ʽ!!" + fmbt->GetToken()->ToLexemeAllString());
                        return nullptr;
                    }
                }
                AllowUseSettings auc;
                auc.SetUseNotConst( false );
                auc.SetUseNotStatic( true );
            }
        }
        CreateExpressParam cep;
        cep.ownerMetaClass = m_OwnerMetaClass;
        cep.metaType = m_DefineMetaType;
        cep.fme = root;
        return ExpressManager::CreateExpressNode(cep);
    }

    return nullptr;
}

} // namespace Core
} // namespace SimpleLanguage
