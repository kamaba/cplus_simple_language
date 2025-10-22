//****************************************************************************
//  File:      MetaDefineVarStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "MetaStatements.h"
#include "../MetaVariable.h"
#include "../MetaType.h"
#include "../MetaClass.h"
#include "../BaseMetaClass/CoreMetaClassManager.h"
#include "../TypeManager.h"
#include "../ExpressManager.h"
#include "../ClassManager.h"
#include "../AllowUseSettings.h"
#include "../Compile/CoreFileMeta/FileMetaDefineVariableSyntax.h"
#include "../Compile/CoreFileMeta/FileMetaOpAssignSyntax.h"
#include "../Compile/CoreFileMeta/FileMetaCallSyntax.h"
#include "../Compile/Token.h"
#include "../Global.h"
#include <string>
#include <iostream>

namespace SimpleLanguage {
namespace Core {
namespace Statements {

class MetaDefineVarStatements : public MetaStatements {
private:
    FileMetaDefineVariableSyntax* m_FileMetaDefineVariableSyntax = nullptr;
    FileMetaOpAssignSyntax* m_FileMetaOpAssignSyntax = nullptr;
    FileMetaCallSyntax* m_FileMetaCallSyntax = nullptr;
    
    MetaVariable* m_DefineVarMetaVariable = nullptr;
    MetaExpressNode* m_ExpressNode = nullptr;
    bool m_IsNeedCastStatements = false;

public:
    MetaExpressNode* GetExpressNode() const { return m_ExpressNode; }
    MetaVariable* GetDefineVarMetaVariable() const { return m_DefineVarMetaVariable; }
    
    MetaDefineVarStatements(MetaBlockStatements* mbs) : MetaStatements(mbs) {
    }
    
    MetaDefineVarStatements(MetaBlockStatements* mbs, FileMetaDefineVariableSyntax* fmdvs) : MetaStatements(mbs) {
        m_FileMetaDefineVariableSyntax = fmdvs;
        m_Name = fmdvs->GetName();
        m_OwnerMetaBlockStatements->AddOnlyNameMetaVariable(m_Name);
        Parse();
    }
    
    MetaDefineVarStatements(MetaBlockStatements* mbs, FileMetaOpAssignSyntax* fmoas) : MetaStatements(mbs) {
        m_FileMetaOpAssignSyntax = fmoas;
        m_Name = m_FileMetaOpAssignSyntax->GetVariableRef()->GetName();
        m_OwnerMetaBlockStatements->AddOnlyNameMetaVariable(m_Name);
        Parse();
    }
    
    MetaDefineVarStatements(MetaBlockStatements* mbs, FileMetaCallSyntax* callSyntax) : MetaStatements(mbs) {
        m_FileMetaCallSyntax = callSyntax;
        m_Name = callSyntax->GetVariableRef()->GetName();
        m_OwnerMetaBlockStatements->AddOnlyNameMetaVariable(m_Name);
        Parse();
    }
    
private:
    void Parse() {
        std::string defineName = m_Name;
        MetaType* mdt = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        auto metaFunction = m_OwnerMetaBlockStatements ? m_OwnerMetaBlockStatements->GetOwnerMetaFunction() : nullptr;
        
        bool isDynamicClass = false;
        FileMetaBaseTerm* fileExpress = nullptr;
        
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
            CreateExpressParam* cep = new CreateExpressParam();
            cep->SetFme(fileExpress);
            cep->SetEqualMetaVariable(m_DefineVarMetaVariable);
            cep->SetMetaType(mdt);
            cep->SetOwnerMBS(m_OwnerMetaBlockStatements);
            cep->SetOwnerMetaClass(m_OwnerMetaBlockStatements->GetOwnerMetaClass());
            
            m_ExpressNode = ExpressManager::CreateExpressNodeByCEP(cep);
            if (m_ExpressNode == nullptr) {
                std::cout << "Error 解析新建变量语句时，表达式解析为空!!__1" << std::endl;
                return;
            }
            
            AllowUseSettings* auc = new AllowUseSettings();
            auc->SetParseFrom(EParseFrom::StatementRightExpress);
            m_ExpressNode->Parse(auc);
            m_ExpressNode->CalcReturnType();
            expressRetMetaDefineType = m_ExpressNode->GetReturnMetaDefineType();
            
            if (expressRetMetaDefineType == nullptr) {
                std::cout << "Error 解析新建变量语句时，表达式返回类型为空!!__2 " << defineName << std::endl;
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
                            std::cout << "Error Enum模式，只允许是调用模式[CallLinkExpress]" << std::endl;
                        } else {
                            auto varableEnum = expressMDT->GetMetaCallLink()->GetFinalCallNode()->GetVariable()->GetOwnerMetaClass();
                            if (mdt->GetMetaClass() != varableEnum) {
                                std::cout << "Error Enum与值不相等!!" << std::endl;
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
                    
                    std::string sb = "Warning 在类: " + (metaFunction ? metaFunction->GetOwnerMetaClass()->GetAllClassName() : "") + 
                                    " 函数: " + (metaFunction ? metaFunction->GetName() : "") + "中  ";
                    if (curClass != nullptr) {
                        sb += " 定义类 : " + curClass->GetAllClassName();
                    }
                    if (defineName != nullptr) {
                        sb += " 名称为: " + defineName;
                    }
                    sb += "与后边赋值语句中 ";
                    if (compareClass != nullptr) {
                        sb += "表达式类为: " + compareClass->GetAllClassName();
                    }
                    
                    if (relation == ClassManager::EClassRelation::No) {
                        sb += "类型不相同，可能会有强转，强转后可能默认值为null";
                        std::cout << sb << std::endl;
                        m_IsNeedCastState = true;
                    } else if (relation == ClassManager::EClassRelation::Same) {
                        m_DefineVarMetaVariable->SetRealMetaType(expressRetMetaDefineType);
                    } else if (relation == ClassManager::EClassRelation::Parent) {
                        sb += "类型不相同，可能会有强转， 返回值是父类型向子类型转换，存在错误转换!!";
                        std::cout << sb << std::endl;
                        m_IsNeedCastState = true;
                    } else if (relation == ClassManager::EClassRelation::Child) {
                        if (compareClass != nullptr) {
                            m_DefineVarMetaVariable->SetRealMetaType(expressRetMetaDefineType);
                        }
                    } else {
                        sb += "表达式错误，或者是定义类型错误";
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

public:
    virtual void SetTRMetaVariable(MetaVariable* mv) override {
        if (m_ExpressNode != nullptr && dynamic_cast<MetaExecuteStatementsNode*>(m_ExpressNode)) {
            dynamic_cast<MetaExecuteStatementsNode*>(m_ExpressNode)->UpdateTrMetaVariable(mv);
        }
        if (m_NextMetaStatements != nullptr) {
            m_NextMetaStatements->SetTRMetaVariable(mv);
        }
    }
    
    virtual void SetDeep(int dp) override {
        MetaStatements::SetDeep(dp);
        if (auto mesn = dynamic_cast<MetaExecuteStatementsNode*>(m_ExpressNode)) {
            mesn->SetDeep(dp);
        }
    }
    
    virtual std::string ToFormatString() override {
        std::string result;
        
        for (int i = 0; i < m_RealDeep; i++) {
            result += Global::GetTabChar();
        }
        result += m_DefineVarMetaVariable->ToFormatString();
        result += " = ";
        
        if (m_DefineVarMetaVariable->GetMetaDefineType()->IsData()) {
            result += m_ExpressNode->ToFormatString();
        } else if (m_DefineVarMetaVariable->GetMetaDefineType()->IsEnum()) {
            // Enum处理
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
};

} // namespace Statements
} // namespace Core
} // namespace SimpleLanguage
