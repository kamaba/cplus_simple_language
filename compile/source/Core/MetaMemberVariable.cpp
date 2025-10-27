//****************************************************************************
//  File:      MetaMemberVariable.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/30 12:00:00
//  Description: class's memeber variable metadata and member 'data' metadata
//****************************************************************************

#include "MetaMemberVariable.h"
#include "MetaClass.h"
#include "MetaType.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "TypeManager.h"
#include "ExpressManager.h"
#include "ClassManager.h"
#include "MetaExpressNode/MetaExpressConst.h"
#include "MetaExpressNode/MetaExpressOperator.h"
#include "MetaExpressNode/MetaExpressCalllink.h"
#include "../Compile/FileMeta/FileMetaMemberVariable.h"
#include "../Compile/CompilerUtil.h"
#include "../Debug/Log.h"
#include <sstream>

using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Core {

// 静态成员初始化
int MetaMemberVariable::s_ConstLevel = 10000000;
int MetaMemberVariable::s_IsHaveRetStaticLevel = 100000000;
int MetaMemberVariable::s_NoHaveRetStaticLevel = 200000000;
int MetaMemberVariable::s_DefineMetaTypeLevel = 1000000000;
int MetaMemberVariable::s_ExpressLevel = 1500000000;

MetaMemberVariable::MetaMemberVariable() : MetaVariable() {
    m_FromType = EFromType::Code;
    m_VariableFrom = EVariableFrom::Member;
}

MetaMemberVariable::MetaMemberVariable(const MetaMemberVariable& mmv) : MetaVariable(mmv) {
    m_FromType = EFromType::Manual;
    m_IsInnerDefine = mmv.m_IsInnerDefine;
    m_Express = mmv.m_Express;
    m_VariableFrom = EVariableFrom::Member;
    m_FileMetaMemeberVariable = mmv.m_FileMetaMemeberVariable;
    m_Name = mmv.m_Name;
    m_PintTokenList = mmv.m_PintTokenList;
    m_Index = mmv.m_Index;
    m_FromType = mmv.m_FromType;
    m_IsStatic = mmv.m_IsStatic;
    m_Permission = mmv.m_Permission;
    m_SourceMetaMemberVariable = mmv.m_SourceMetaMemberVariable;
}

MetaMemberVariable::MetaMemberVariable(MetaClass* mc, const std::string& name) : MetaVariable() {
    m_Name = name;
    m_FromType = EFromType::Manual;
    m_DefineMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_IsInnerDefine = true;
    m_VariableFrom = EVariableFrom::Member;
    SetOwnerMetaClass(mc);
}

MetaMemberVariable::MetaMemberVariable(MetaClass* mc, const std::string& name, MetaClass* defineTypeClass, MetaExpressNode* men) : MetaVariable() {
    m_Name = name;
    m_IsInnerDefine = true;
    m_FromType = EFromType::Manual;
    m_DefineMetaType = new MetaType(defineTypeClass);
    m_DefineMetaType->SetMetaClass(defineTypeClass);
    m_Express = men;
    m_VariableFrom = EVariableFrom::Member;
    SetOwnerMetaClass(mc);
}

MetaMemberVariable::MetaMemberVariable(MetaClass* mc, void* fmmv) : MetaVariable() {
    m_FileMetaMemeberVariable = fmmv;
    // 这里需要根据实际的 FileMetaMemberVariable 结构来设置
    // m_Name = fmmv->name;
    // AddPingToken(fmmv->nameToken);
    m_Index = mc->GetMetaMemberVariableDict().size();
    m_FromType = EFromType::Code;
    m_DefineMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    // m_IsStatic = m_FileMetaMemeberVariable?.staticToken != null;
    m_VariableFrom = EVariableFrom::Member;
    // if (m_FileMetaMemeberVariable.permissionToken != null) {
    //     m_Permission = CompilerUtil::GetPerMissionByString(m_FileMetaMemeberVariable.permissionToken?.lexeme.ToString());
    // }
    SetOwnerMetaClass(mc);
}

void MetaMemberVariable::ParseDefineMetaType() {
    if (m_FileMetaMemeberVariable != nullptr) {
        // 这里需要根据实际的 FileMetaMemberVariable 结构来处理
        // m_DefineMetaType = TypeManager::GetInstance().GetMetaTemplateClassAndRegisterExptendTemplateClassInstance(m_OwnerMetaClass, m_FileMetaMemeberVariable->classDefineRef);
        // if (m_DefineMetaType->IsTemplate() || m_DefineMetaType->GetEType() == EMetaTypeType::TemplateClassWithTemplate) {
        //     m_RealMetaType = new MetaType(m_DefineMetaType);
        // }
    }
}

int MetaMemberVariable::CalcParseLevelBeCall(int level) {
    m_ParseLevel = level - 1;
    return m_ParseLevel;
}

void MetaMemberVariable::CalcParseLevel() {
    if (IsConst()) {
        m_ParseLevel = s_ConstLevel;
        s_ConstLevel = s_ConstLevel + 10000;
    }
    else if (IsStatic()) {
        if (m_ParseLevel == -1) {
            if (m_DefineMetaType != nullptr) {
                m_ParseLevel = s_IsHaveRetStaticLevel;
                s_IsHaveRetStaticLevel = s_IsHaveRetStaticLevel + 100000;
            }
            else {
                m_ParseLevel = s_NoHaveRetStaticLevel;
                s_NoHaveRetStaticLevel = s_NoHaveRetStaticLevel + 100000;
            }
        }
    }
    else {
        if (m_ParseLevel == -1) {
            if (m_DefineMetaType != nullptr) {
                m_ParseLevel = s_DefineMetaTypeLevel;
                s_DefineMetaTypeLevel = s_DefineMetaTypeLevel + 1000000;
            }
            else {
                m_ParseLevel = s_ExpressLevel;
                s_ExpressLevel = s_ExpressLevel + 1000000;
            }
        }
    }

    if (m_Express != nullptr) {
        ExpressManager::CalcParseLevel(m_ParseLevel, m_Express);
    }
}

void MetaMemberVariable::CreateExpress() {
    if (m_FileMetaMemeberVariable != nullptr) {
        // 这里需要根据实际的 FileMetaMemberVariable 结构来处理
        // if (this->m_FileMetaMemeberVariable->DataType == FileMetaMemberVariable::EMemberDataType::Array) {
        //     m_Express = CreateExpressNodeInClassMetaVariable();
        // }
        // else {
        //     m_Express = CreateExpressNodeInClassMetaVariable();
        // }
    }
    if (m_Express == nullptr) {
        auto ld = Log::AddInStructMeta(EError::MemberNeedExpress, 
            "Error [" + (GetOwnerMetaClass() ? GetOwnerMetaClass()->GetAllClassName() + "." + m_Name : m_Name) + "]配置成员变量时，必须需要有等号及后续的表达式!!");
        ld->demo = "T t";
        ld->advan = "T t = null";
    }
}

void MetaMemberVariable::CalcReturnType() {
    std::string defineName = m_Name;
    if (m_Express != nullptr) {
        m_Express->CalcReturnType();
        auto enode = SimulateExpressRun(m_Express);
        if (enode != nullptr) {
            m_Express = enode;
            m_Express->CalcReturnType();
        }
        CalcDefineClassType();
    }
    else {
        m_Express = m_DefineMetaType->GetDefaultExpressNode();
    }
    if (m_Express == nullptr && m_DefineMetaType == nullptr) {
        Log::AddInStructMeta(EError::None, "Error 表达式为空 或者 表达示必须有返回值");
    }
}

int MetaMemberVariable::CompareTo(const MetaMemberVariable* mmv) const {
    if (m_ParseLevel > mmv->m_ParseLevel)
        return 1;
    else
        return -1;
}

MetaExpressNode* MetaMemberVariable::SimulateExpressRun(MetaExpressNode* node) {
    MetaExpressNode* newnode = node;
    if (auto mcen = dynamic_cast<MetaCallLinkExpressNode*>(node)) {
        if (mcen != nullptr) {
            newnode = mcen->ConvertConstExpressNode();
        }
    }
    else if (auto moen = dynamic_cast<MetaOpExpressNode*>(node)) {
        auto left = SimulateExpressRun(moen->GetLeft());
        auto right = SimulateExpressRun(moen->GetRight());
        if (left != nullptr) {
            moen->SetLeft(left);
        }
        if (right != nullptr) {
            moen->SetRight(right);
        }
        newnode = node;
    }
    else if (auto muoen = dynamic_cast<MetaUnaryOpExpressNode*>(node)) {
        auto v = SimulateExpressRun(muoen->GetValue());
        if (v != nullptr) {
            muoen->SetValue(v);
        }
        newnode = node;
    }
    return newnode;
}

MetaExpressNode* MetaMemberVariable::CreateExpressNodeInClassMetaVariable() {
    // 这里需要根据实际的 FileMetaMemberVariable 结构来处理
    // auto express = this->m_FileMetaMemeberVariable?.express;
    // if (express == nullptr) return nullptr;
    
    // auto root = express->root;
    // if (root == nullptr) return nullptr;
    
    // 创建表达式参数的逻辑
    CreateExpressParam* cep = new CreateExpressParam();
    cep->SetOwnerMetaClass(GetOwnerMetaClass());
    cep->SetMetaType(m_DefineMetaType);
    cep->SetEqualMetaVariable(this);
    cep->SetParseFrom(EParseFrom::MemberVariableExpress);
    cep->SetIsConst(IsConst());
    cep->SetIsStatic(IsStatic());
    cep->SetAllowUseIfSyntax(false);
    cep->SetAllowUseSwitchSyntax(false);
    cep->SetAllowUseParSyntax(m_IsSupportConstructionFunctionOnlyParType);
    cep->SetAllowUseBraceSyntax(m_IsSupportConstructionFunctionOnlyBraceType);
    // cep->SetFme(root);

    MetaExpressNode* mn = ExpressManager::CreateExpressNode(*cep);
    return mn;
}

bool MetaMemberVariable::ParseMetaExpress() {
    if (m_Express != nullptr) {
        AllowUseSettings* auc = new AllowUseSettings();
        auc->SetParseFrom(EParseFrom::MemberVariableExpress);
        m_Express->Parse(auc);
    }
    return true;
}

void MetaMemberVariable::ParseChildMemberData() {
    if (m_FileMetaMemeberVariable == nullptr) {
        return;
    }
    
    // 这里需要根据实际的 FileMetaMemberVariable 结构来处理
    // int count = m_FileMetaMemeberVariable->fileMetaMemberVariable.size();
    // 处理子成员数据的逻辑
}

std::string MetaMemberVariable::ToFormatString() const {
    std::stringstream sb;

    for (int i = 0; i < GetRealDeep(); i++) {
        sb << Global::GetTabChar();
    }

    sb << (int)GetPermission() << " ";
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

std::string MetaMemberVariable::ToTokenString() const {
    std::stringstream sb;
    // 这里需要根据实际的 FileMetaMemberVariable 结构来处理
    // sb << m_FileMetaMemeberVariable->nameToken->GetSourceBeginLine() << " 与父类的Token位置: " << m_FileMetaMemeberVariable->nameToken->GetSourceBeginLine();
    return sb.str();
}

void MetaMemberVariable::CalcDefineClassType() {
    std::string defineName = m_Name;
    if (m_RealMetaType == nullptr) {
        if (m_Express != nullptr) {
            auto constExpressNode = dynamic_cast<MetaConstExpressNode*>(m_Express);
            bool isCheckReturnType = true;
            if (constExpressNode != nullptr) {
                if (constExpressNode->GetEType() == EType::Null) {
                    isCheckReturnType = false;
                }
            }
            if (isCheckReturnType) {
                auto dmct = m_Express->GetReturnMetaDefineType();
                if (dmct != nullptr) {
                    if (dmct->GetMetaClass() == GetOwnerMetaClass()) {
                        Log::AddInStructMeta(EError::None, "Error 自己类内部不允许包含 自己的实体，必须赋值为null");
                        return;
                    }
                    m_RealMetaType = dmct;
                }
            }
        }
    }
    else {
        if (m_Express != nullptr) {
            EClassRelation relation = EClassRelation::No;
            auto constExpressNode = dynamic_cast<MetaConstExpressNode*>(m_Express);
            MetaClass* curClass = m_DefineMetaType->GetMetaClass();

            MetaClass* compareClass = nullptr;
            MetaType* expressRetMetaDefineType = nullptr;
            if (constExpressNode != nullptr && constExpressNode->GetEType() == EType::Null) {
                relation = EClassRelation::Same;
            }
            else {
                expressRetMetaDefineType = m_Express->GetReturnMetaDefineType();
                if (expressRetMetaDefineType == nullptr) {
                    Log::AddInStructMeta(EError::None, "Error 表达式中返回定义类型为空 " + m_Express->ToTokenString());
                    return;
                }

                compareClass = expressRetMetaDefineType->GetMetaClass();
                relation = ClassManager::ValidateClassRelationByMetaClass(curClass, compareClass);
            }

            std::stringstream sb;
            if (curClass != nullptr) {
                sb << " 定义类 : " << curClass->GetAllClassName();
            }
            if (!defineName.empty()) {
                sb << " 名称为: " << defineName;
            }
            sb << "与后边赋值语句中 ";
            if (compareClass != nullptr) {
                sb << "表达式类为: " << compareClass->GetAllClassName();
            }
            
            if (relation == EClassRelation::No) {
                sb << "类型不相同，可能会有强转，强转后可能默认值为null";
                Log::AddInStructMeta(EError::None, sb.str());
            }
            else if (relation == EClassRelation::Same) {
                if (!(constExpressNode != nullptr && constExpressNode->GetEType() == EType::Null)) {
                    if (expressRetMetaDefineType->GetMetaClass() == GetOwnerMetaClass() && !IsStatic()) {
                        Log::AddInStructMeta(EError::None, "Error 自己类内部不允许包含 自己的实体，必须赋值为null");
                        return;
                    }
                    SetRealMetaType(expressRetMetaDefineType);
                }
            }
            else if (relation == EClassRelation::Parent) {
                sb << "类型不相同，可能会有强转， 返回值是父类型向子类型转换，存在错误转换!!";
                Log::AddInStructMeta(EError::None, sb.str());
            }
            else if (relation == EClassRelation::Child) {
                if (compareClass != nullptr) {
                    if (expressRetMetaDefineType->GetMetaClass() == GetOwnerMetaClass()) {
                        Log::AddInStructMeta(EError::None, "Error 自己类内部不允许包含 自己的实体，必须赋值为null");
                        return;
                    }
                    SetRealMetaType(expressRetMetaDefineType);
                }
            }
            else {
                sb << "表达式错误，或者是定义类型错误";
                Log::AddInStructMeta(EError::None, sb.str());
            }
        }
    }
}

} // namespace Core
} // namespace SimpleLanguage