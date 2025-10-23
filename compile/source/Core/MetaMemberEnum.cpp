//****************************************************************************
//  File:      MetaMemberEnum.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2025/1/17 12:00:00
//  Description: enum's memeber variable metadata and member 'data' metadata implementation
//****************************************************************************

#include "MetaMemberEnum.h"
#include "MetaVariable.h"
#include "MetaExpressNode.h"
#include "MetaType.h"
#include "MetaClass.h"
#include "FileMetaMemberVariable.h"
#include "MetaConstExpressNode.h"
#include "MetaCallLinkExpressNode.h"
#include "MetaNewObjectExpressNode.h"
#include "Log.h"
#include "CoreMetaClassManager.h"
#include "ExpressManager.h"
#include "AllowUseSettings.h"
#include "CreateExpressParam.h"
#include "Global.h"
#include "CompilerUtil.h"
#include "MetaInputParamCollection.h"
#include "MetaMemberFunction.h"
#include "MetaGenTemplate.h"
#include <vector>
#include <string>
#include <sstream>

namespace SimpleLanguage {
namespace Core {

// Static member initialization
int MetaMemberEnum::s_ConstLevel = 10000000;
int MetaMemberEnum::s_IsHaveRetStaticLevel = 100000000;
int MetaMemberEnum::s_NoHaveRetStaticLevel = 200000000;
int MetaMemberEnum::s_DefineMetaTypeLevel = 1000000000;
int MetaMemberEnum::s_ExpressLevel = 1500000000;

MetaMemberEnum::MetaMemberEnum(MetaClass* mc, FileMetaMemberVariable* fmmv) {
    m_FileMetaMemeberVariable = fmmv;
    m_Name = fmmv->Name();
    AddPingToken(fmmv->GetNameToken());
    m_Index = mc->GetMetaMemberVariableDict().size();
    m_FromType = EFromType::Code;
    m_DefineMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_IsStatic = true; // enum 成员全部为static
    m_VariableFrom = EVariableFrom::Static;
    if (fmmv->GetStaticToken() != nullptr) {
        Log::AddInStructMeta(EError::None, "Error ENum中，不允许有静态关键字，而是全部是静态关键字!!");
    }
    if (m_FileMetaMemeberVariable->GetPermissionToken() != nullptr) {
        Log::AddInStructMeta(EError::None, "Error Enum中，不允许使用public/private等权限关键字!!");
        m_Permission = CompilerUtil::GetPerMissionByString(m_FileMetaMemeberVariable->GetPermissionToken()->GetLexeme().ToString());
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
            cep.parsefrom = EParseFrom::MemberVariableExpress;
            m_Express = ExpressManager::CreateExpressNodeByCEP(cep);

            if (m_Express == nullptr) {
                Log::AddInStructMeta(EError::None, "Error 没有解析到Express的内容 在MetaMemberData 里边 372");
            }
        }
    }
}

bool MetaMemberEnum::ParseMetaExpress() {
    if (m_Express != nullptr) {
        AllowUseSettings auc;
        auc.parseFrom = EParseFrom::MemberVariableExpress;
        m_Express->Parse(auc);
    }
    return true;
}
MetaConstExpressNode* MetaMemberEnum::GetConstExpressNode() const { return dynamic_cast<MetaConstExpressNode*>(m_Express); }

void MetaMemberEnum::SetExpress(MetaConstExpressNode* mcen) {
    m_Express = mcen;
}

std::string MetaMemberEnum::ToFormatString() const {
    std::stringstream sb;

    for (int i = 0; i < realDeep; i++)
        sb << Global::GetTabChar();

    sb << permission.ToFormatString() << " ";
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

    sb << m_FileMetaMemeberVariable->GetNameToken()->GetSourceBeginLine() << " 与父类的Token位置: "
       << m_FileMetaMemeberVariable->GetNameToken()->GetSourceBeginLine();

    return sb.str();
}

MetaExpressNode* MetaMemberEnum::CreateExpressNodeInClassMetaVariable() {
    auto express = m_FileMetaMemeberVariable->GetExpress();
    if (express == nullptr) return nullptr;

    auto root = express->Root();
    if (root == nullptr)
        return nullptr;
    if (root->GetLeft() == nullptr && root->GetRight() == nullptr) {
        auto fmpt = dynamic_cast<FileMetaParTerm*>(root);
        auto fmct = dynamic_cast<FileMetaCallTerm*>(root);
        auto fmbt = dynamic_cast<FileMetaBraceTerm*>(root);
        if (m_DefineMetaType != nullptr) {
            if (fmpt != nullptr) {
                if (m_IsSupportConstructionFunctionOnlyParType) {
                    MetaInputParamCollection mpc(fmpt, ownerMetaClass, nullptr);

                    MetaMemberFunction* mmf = m_DefineMetaType->GetMetaMemberConstructFunction(&mpc);

                    if (mmf == nullptr) return nullptr;

                    MetaNewObjectExpressNode* mnoen = new MetaNewObjectExpressNode(fmpt, m_DefineMetaType, ownerMetaClass, mmf->GetMetaBlockStatements());
                    if (mnoen != nullptr) {
                        return mnoen;
                    }
                } else {
                    Log::AddInStructMeta(EError::None, "Error 现在配置中，不支持成员变量中使用类的()构造方式!!");
                }
            } else if (fmbt != nullptr) {
                MetaNewObjectExpressNode* mnoen = new MetaNewObjectExpressNode(fmbt, m_DefineMetaType, ownerMetaClass, nullptr, nullptr);
                return mnoen;
            } else if (fmct != nullptr) {
                if (fmct->GetCallLink()->GetCallNodeList().size() > 0) {
                    auto finalNode = fmct->GetCallLink()->GetCallNodeList()[fmct->GetCallLink()->GetCallNodeList().size() - 1];
                    if (finalNode->GetFileMetaBraceTerm() != nullptr && !m_IsSupportConstructionFunctionConnectBraceType) {
                        Log::AddInStructMeta(EError::None, "Error 在类变量中，不允许 使用Class()后带{}的赋值方式!!" + fmbt->GetToken()->ToLexemeAllString());
                        return nullptr;
                    }
                }
                AllowUseSettings auc;
                auc.useNotConst = false;
                auc.useNotStatic = true;
            }
        } else {
            if (fmpt != nullptr) {
                Log::AddInStructMeta(EError::None, "Error 在类没有定义的变量中，不允许 使用()的赋值方式!!" + fmbt->GetToken()->ToLexemeAllString());
                return nullptr;
            } else if (fmbt != nullptr) {
                Log::AddInStructMeta(EError::None, "Error 在类没有定义的变量中，不允许 使用{}的赋值方式!!" + fmbt->GetToken()->ToLexemeAllString());
                return nullptr;
            } else if (fmct != nullptr) {
                if (fmct->GetCallLink()->GetCallNodeList().size() > 0) {
                    auto finalNode = fmct->GetCallLink()->GetCallNodeList()[fmct->GetCallLink()->GetCallNodeList().size() - 1];
                    if (finalNode->GetFileMetaBraceTerm() != nullptr && !m_IsSupportConstructionFunctionConnectBraceType) {
                        Log::AddInStructMeta(EError::None, "Error 在类变量中，不允许 使用Class()后带{}的赋值方式!!" + fmbt->GetToken()->ToLexemeAllString());
                        return nullptr;
                    }
                }
                AllowUseSettings auc;
                auc.useNotConst = false;
                auc.useNotStatic = true;
            }
        }
        CreateExpressParam cep;
        cep.ownerMetaClass = ownerMetaClass;
        cep.metaType = m_DefineMetaType;
        cep.fme = root;
        return ExpressManager::CreateExpressNode(cep);
    }

    return nullptr;
}

} // namespace Core
} // namespace SimpleLanguage
