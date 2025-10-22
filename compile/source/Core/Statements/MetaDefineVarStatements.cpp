//****************************************************************************
//  File:      MetaDefineVarStatements.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#include "MetaDefineVarStatements.h"
#include "../MetaExpressNode/MetaExpressBase.h"
#include "../MetaExpressNode/MetaConstExpressNode.h"
#include "../MetaExpressNode/MetaCallLinkExpressNode.h"
#include "../MetaExpressNode/MetaExecuteStatementsNode.h"
#include "../MetaGenTemplateClass.h"
#include "../Debug/Log.h"
#include <iostream>

using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Core {

// ���캯��ʵ��
MetaDefineVarStatements::MetaDefineVarStatements(MetaBlockStatements* mbs) : MetaStatements(mbs) {
}

MetaDefineVarStatements::MetaDefineVarStatements(MetaBlockStatements* mbs, FileMetaDefineVariableSyntax* fmdvs) : MetaStatements(mbs) {
    m_FileMetaDefineVariableSyntax = fmdvs;
    m_Name = fmdvs->GetName();
    m_OwnerMetaBlockStatements->AddOnlyNameMetaVariable(m_Name);
    Parse();
}

MetaDefineVarStatements::MetaDefineVarStatements(MetaBlockStatements* mbs, FileMetaOpAssignSyntax* fmoas) : MetaStatements(mbs) {
    m_FileMetaOpAssignSyntax = fmoas;
    m_Name = m_FileMetaOpAssignSyntax->GetVariableRef()->GetName();
    m_OwnerMetaBlockStatements->AddOnlyNameMetaVariable(m_Name);
    Parse();
}

MetaDefineVarStatements::MetaDefineVarStatements(MetaBlockStatements* mbs, FileMetaCallSyntax* callSyntax) : MetaStatements(mbs) {
    m_FileMetaCallSyntax = callSyntax;
    m_Name = callSyntax->GetVariableRef()->GetName();
    m_OwnerMetaBlockStatements->AddOnlyNameMetaVariable(m_Name);
    Parse();
}

// Parse����ʵ��
void MetaDefineVarStatements::Parse() {
    std::string defineName = m_Name;
    MetaType* mdt = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    auto metaFunction = m_OwnerMetaBlockStatements ? m_OwnerMetaBlockStatements->GetOwnerMetaFunction() : nullptr;
    
    bool isDynamicClass = false;
    Compile::FileMetaBaseTerm* fileExpress = nullptr;
    
    if (m_FileMetaDefineVariableSyntax != nullptr) {
        auto fmcd = m_FileMetaDefineVariableSyntax->GetFileMetaClassDefine();
        mdt = TypeManager::GetInstance().GetMetaTypeByTemplateFunction(GetOwnerMetaClass(), 
            dynamic_cast<MetaMemberFunction*>(m_OwnerMetaBlockStatements->GetOwnerMetaFunction()), fmcd);
        
        if (auto mgtc = dynamic_cast<MetaGenTemplateClass*>(mdt->GetMetaClass())) {
            mgtc->Parse();
        }
        
        m_DefineVarMetaVariable = new MetaVariable(m_Name, MetaVariable::EVariableFrom::LocalStatement, 
            m_OwnerMetaBlockStatements, m_OwnerMetaBlockStatements->GetOwnerMetaClass(), mdt);
        m_DefineVarMetaVariable->AddPingToken(m_FileMetaDefineVariableSyntax->GetToken());
        fileExpress = m_FileMetaDefineVariableSyntax->GetExpress();
    } else if (m_FileMetaOpAssignSyntax != nullptr) {
        m_DefineVarMetaVariable = new MetaVariable(m_Name, MetaVariable::EVariableFrom::LocalStatement, 
            m_OwnerMetaBlockStatements, m_OwnerMetaBlockStatements->GetOwnerMetaClass(), mdt);
        Token* token = m_FileMetaOpAssignSyntax->GetAssignToken();
        if (m_FileMetaOpAssignSyntax->GetDynamicToken() != nullptr) {
            isDynamicClass = true;
        }
        if (m_FileMetaOpAssignSyntax->GetVariableRef() != nullptr) {
            for (auto& v : m_FileMetaOpAssignSyntax->GetVariableRef()->GetCallNodeList()) {
                m_DefineVarMetaVariable->AddPingToken(v->GetToken());
            }
        }
        m_DefineVarMetaVariable->AddPingToken(token);
        fileExpress = m_FileMetaOpAssignSyntax->GetExpress();
    } else if (m_FileMetaCallSyntax != nullptr) {
        m_DefineVarMetaVariable = new MetaVariable(m_Name, MetaVariable::EVariableFrom::LocalStatement, 
            m_OwnerMetaBlockStatements, m_OwnerMetaBlockStatements->GetOwnerMetaClass(), mdt);
        m_DefineVarMetaVariable->AddPingToken(m_FileMetaCallSyntax->GetToken());
    }
    
    if (m_DefineVarMetaVariable == nullptr) {
        std::cout << "Error " << defineName << " MetaVariable is Null" << std::endl;
        return;
    }
    m_OwnerMetaBlockStatements->UpdateMetaVariableDict(m_DefineVarMetaVariable);
    
    MetaType* expressRetMetaDefineType = nullptr;
    if (fileExpress != nullptr) {
        CreateExpressParam cep;
        cep.fme = fileExpress;
        cep.equalMetaVariable = m_DefineVarMetaVariable;
        cep.metaType = mdt;
        cep.ownerMBS = m_OwnerMetaBlockStatements;
        cep.ownerMetaClass = m_OwnerMetaBlockStatements->GetOwnerMetaClass();
        
        m_ExpressNode = ExpressManager::CreateExpressNodeByCEP(cep);
        if (m_ExpressNode == nullptr) {
            std::cout << "Error �����½��������ʱ�����ʽ����Ϊ��!!__1" << std::endl;
            return;
        }
        
        AllowUseSettings auc;
        auc.parseFrom = EParseFrom::StatementRightExpress;
        m_ExpressNode->Parse(auc);
        m_ExpressNode->CalcReturnType();
        expressRetMetaDefineType = m_ExpressNode->GetReturnMetaDefineType();
        
        if (expressRetMetaDefineType == nullptr) {
            std::cout << "Error �����½��������ʱ�����ʽ��������Ϊ��!!__2 " << defineName << std::endl;
            return;
        }
    }
    
    if (!mdt->IsDefineMetaClass()) {
        auto constExpressNode = dynamic_cast<MetaConstExpressNode*>(m_ExpressNode);
        bool isCheckReturnType = true;
        if (constExpressNode != nullptr) {
            if (constExpressNode->GetEType() == EType::Null) {
                isCheckReturnType = false;
            }
        }
        if (isCheckReturnType) {
            m_DefineVarMetaVariable->SetRealMetaType(expressRetMetaDefineType);
        }
    } else {
        if (m_ExpressNode != nullptr) {
            ClassManager::EClassRelation relation = ClassManager::EClassRelation::No;
            auto constExpressNode = dynamic_cast<MetaConstExpressNode*>(m_ExpressNode);
            MetaClass* curClass = mdt->GetMetaClass();
            
            if (mdt->IsEnum()) {
                if (m_ExpressNode != nullptr) {
                    auto expressMDT = dynamic_cast<MetaCallLinkExpressNode*>(m_ExpressNode);
                    if (expressMDT == nullptr) {
                        std::cout << "Error Enumģʽ��ֻ�����ǵ���ģʽ[CallLinkExpress]" << std::endl;
                    } else {
                        auto varableEnum = expressMDT->GetMetaCallLink()->GetFinalCallNode()->GetVariable()->GetOwnerMetaClass();
                        if (mdt->GetMetaClass() != varableEnum) {
                            std::cout << "Error Enum��ֵ�����!!" << std::endl;
                        } else {
                            m_IsNeedCastState = false;
                        }
                    }
                }
            } else {
                MetaClass* compareClass = nullptr;
                if (constExpressNode != nullptr && constExpressNode->GetEType() == EType::Null) {
                    relation = ClassManager::EClassRelation::Same;
                } else {
                    compareClass = expressRetMetaDefineType->GetMetaClass();
                    relation = ClassManager::ValidateClassRelationByMetaClass(curClass, compareClass);
                }
                
                std::string sb = "Warning ����: " + (metaFunction ? metaFunction->GetOwnerMetaClass()->GetAllClassName() : "") + 
                                " ����: " + (metaFunction ? metaFunction->GetName() : "") + "��  ";
                if (curClass != nullptr) {
                    sb += " ������ : " + curClass->GetAllClassName();
                }
                if (defineName != nullptr) {
                    sb += " ����Ϊ: " + defineName;
                }
                sb += "���߸�ֵ����� ";
                if (compareClass != nullptr) {
                    sb += "���ʽ��Ϊ: " + compareClass->GetAllClassName();
                }
                
                if (relation == ClassManager::EClassRelation::No) {
                    sb += "���Ͳ���ͬ�����ܻ���ǿת��ǿת�����Ĭ��ֵΪnull";
                    std::cout << sb << std::endl;
                    m_IsNeedCastState = true;
                } else if (relation == ClassManager::EClassRelation::Same) {
                    m_DefineVarMetaVariable->SetRealMetaType(expressRetMetaDefineType);
                } else if (relation == ClassManager::EClassRelation::Parent) {
                    sb += "���Ͳ���ͬ�����ܻ���ǿת�� ����ֵ�Ǹ�������������ת�������ڴ���ת��!!";
                    std::cout << sb << std::endl;
                    m_IsNeedCastState = true;
                } else if (relation == ClassManager::EClassRelation::Child) {
                    if (compareClass != nullptr) {
                        m_DefineVarMetaVariable->SetRealMetaType(expressRetMetaDefineType);
                    }
                } else {
                    sb += "���ʽ���󣬻����Ƕ������ʹ���";
                    std::cout << sb << std::endl;
                }
            }
        }
    }
    
    if (m_ExpressNode == nullptr) {
        m_ExpressNode = m_DefineVarMetaVariable->GetMetaDefineType()->GetDefaultExpressNode();
    }
    SetTRMetaVariable(m_DefineVarMetaVariable);
}

// SetTRMetaVariable����ʵ��
void MetaDefineVarStatements::SetTRMetaVariable(MetaVariable* mv) {
    if (m_ExpressNode != nullptr && dynamic_cast<MetaExecuteStatementsNode*>(m_ExpressNode)) {
        dynamic_cast<MetaExecuteStatementsNode*>(m_ExpressNode)->UpdateTrMetaVariable(mv);
    }
    if (m_NextMetaStatements != nullptr) {
        m_NextMetaStatements->SetTRMetaVariable(mv);
    }
}

// SetDeep����ʵ��
void MetaDefineVarStatements::SetDeep(int dp) {
    MetaStatements::SetDeep(dp);
    if (auto mesn = dynamic_cast<MetaExecuteStatementsNode*>(m_ExpressNode)) {
        mesn->SetDeep(dp);
    }
}

// ToFormatString����ʵ��
std::string MetaDefineVarStatements::ToFormatString() {
    std::string result;
    
    for (int i = 0; i < GetRealDeep(); i++) {
        result += Global::GetTabChar();
    }
    result += m_DefineVarMetaVariable->ToFormatString();
    result += " = ";
    
    if (m_DefineVarMetaVariable->GetMetaDefineType()->IsData()) {
        result += m_ExpressNode->ToFormatString();
    } else if (m_DefineVarMetaVariable->GetMetaDefineType()->IsEnum()) {
        // Enum����
    } else {
        if (m_IsNeedCastState) {
            result += "(";
        }
        result += m_ExpressNode->ToFormatString();
        if (m_IsNeedCastState) {
            result += ").cast<" + m_DefineVarMetaVariable->GetMetaDefineType()->GetMetaClass()->GetAllClassName() + ">()";
        }
        result += ";";
    }
    
    if (m_NextMetaStatements != nullptr) {
        result += "\n";
        result += m_NextMetaStatements->ToFormatString();
    }
    
    return result;
}

} // namespace Core
} // namespace SimpleLanguage