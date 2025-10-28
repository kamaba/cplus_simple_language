//****************************************************************************
//  File:      MetaDefineVarStatements.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#include "MetaDefineVarStatements.h"
#include "../MetaExpressNode/MetaExpressBase.h"
#include "../MetaExpressNode/MetaExpressConst.h"
#include "../MetaExpressNode/MetaExpressCalllink.h"
#include "../MetaExpressNode/MetaExecuteStatementsNode.h"
#include "../MetaGenTemplateClass.h"
#include "../MetaVariable.h"
#include "../MetaType.h"
#include "../MetaClass.h"
#include "../MetaCallLink.h"
#include "../MetaVisitCall.h"
#include "../MetaMemberFunction.h"
#include "../ClassManager.h"
#include "../TypeManager.h"
#include "../ExpressManager.h"
#include "../AllowUseSettings.h"
#include "../BaseMetaClass/CoreMetaClassManager.h"
#include "../../Compile/FileMeta/FileMetaSyntax.h"
#include "../../Compile/FileMeta/FileMetaCommon.h"
#include "../../Compile/FileMeta/FileMetaBase.h"
#include "../../Compile/Token.h"
#include "../../Define.h"
#include "../../Debug/Log.h"
#include <iostream>
#include <string>

using namespace SimpleLanguage::Debug;
using namespace SimpleLanguage::Compile;

namespace SimpleLanguage {
namespace Core {

// 属性访问器实现
MetaExpressNode* MetaDefineVarStatements::GetExpressNode() const {
    return m_ExpressNode;
}

MetaVariable* MetaDefineVarStatements::GetDefineVarMetaVariable() const {
    return m_DefineVarMetaVariable;
}

// 构造函数实现
MetaDefineVarStatements::MetaDefineVarStatements(MetaBlockStatements* mbs) : MetaStatements(mbs) {
}

MetaDefineVarStatements::MetaDefineVarStatements(MetaBlockStatements* mbs, Compile::FileMetaDefineVariableSyntax* fmdvs) : MetaStatements(mbs) {
    m_FileMetaDefineVariableSyntax = fmdvs;
    m_Name = fmdvs->GetName();
    m_OwnerMetaBlockStatements->AddOnlyNameMetaVariable(m_Name);
    Parse();
}

MetaDefineVarStatements::MetaDefineVarStatements(MetaBlockStatements* mbs, Compile::FileMetaOpAssignSyntax* fmoas) : MetaStatements(mbs) {
    m_FileMetaOpAssignSyntax = fmoas;
    m_Name = m_FileMetaOpAssignSyntax->GetVariableRef()->GetName();
    m_OwnerMetaBlockStatements->AddOnlyNameMetaVariable(m_Name);
    Parse();
}

MetaDefineVarStatements::MetaDefineVarStatements(MetaBlockStatements* mbs, Compile::FileMetaCallSyntax* callSyntax) : MetaStatements(mbs) {
    m_FileMetaCallSyntax = callSyntax;
    m_Name = callSyntax->GetVariableRef()->GetName();
    m_OwnerMetaBlockStatements->AddOnlyNameMetaVariable(m_Name);
    Parse();
}

// Parse方法实现
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
        
        m_DefineVarMetaVariable = new MetaVariable(m_Name, EVariableFrom::LocalStatement, 
            m_OwnerMetaBlockStatements, m_OwnerMetaBlockStatements->GetOwnerMetaClass(), mdt);
        m_DefineVarMetaVariable->AddPingToken(m_FileMetaDefineVariableSyntax->GetToken());
        fileExpress = m_FileMetaDefineVariableSyntax->GetExpress();
    } else if (m_FileMetaOpAssignSyntax != nullptr) {
        m_DefineVarMetaVariable = new MetaVariable(m_Name, EVariableFrom::LocalStatement, 
            m_OwnerMetaBlockStatements, m_OwnerMetaBlockStatements->GetOwnerMetaClass(), mdt);
        Compile::Token* token = m_FileMetaOpAssignSyntax->GetAssignToken();
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
        m_DefineVarMetaVariable = new MetaVariable(m_Name, EVariableFrom::LocalStatement, 
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
        cep.SetFme(fileExpress);
        cep.SetEqualMetaVariable(m_DefineVarMetaVariable);
        cep.SetMetaType(mdt);
        cep.SetOwnerMBS(m_OwnerMetaBlockStatements);
        cep.SetOwnerMetaClass(m_OwnerMetaBlockStatements->GetOwnerMetaClass());
        
        m_ExpressNode = ExpressManager::CreateExpressNodeByCEP(cep);
        if (m_ExpressNode == nullptr) {
            std::cout << "Error 解析新建变量语句时，表达式解析为空!!__1" << std::endl;
            return;
        }
        
        AllowUseSettings auc;
        auc.SetParseFrom( EParseFrom::StatementRightExpress );
        m_ExpressNode->Parse(&auc);
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
            EClassRelation relation = EClassRelation::No;
            auto constExpressNode = dynamic_cast<MetaConstExpressNode*>(m_ExpressNode);
            MetaClass* curClass = mdt->GetMetaClass();
            
            if (mdt->IsEnum()) {
                if (m_ExpressNode != nullptr) {
                    auto expressMDT = dynamic_cast<MetaCallLinkExpressNode*>(m_ExpressNode);
                    if (expressMDT == nullptr) {
                        std::cout << "Error Enum模式，只允许是调用模式[CallLinkExpress]" << std::endl;
                    } else {
                        auto varableEnum = expressMDT->GetMetaCallLink()->GetFinalCallNode()->variable()->GetOwnerMetaClass();
                        if (mdt->GetMetaClass() != varableEnum) {
                            std::cout << "Error Enum与值不相等!!" << std::endl;
                        } else {
                            m_IsNeedCastStatements = false;
                        }
                    }
                }
            } else {
                MetaClass* compareClass = nullptr;
                if (constExpressNode != nullptr && constExpressNode->GetEType() == EType::Null) {
                    relation = EClassRelation::Same;
                } else {
                    compareClass = expressRetMetaDefineType->GetMetaClass();
                    relation = ClassManager::ValidateClassRelationByMetaClass(curClass, compareClass);
                }
                
                std::string sb = "Warning 在类: " + (metaFunction ? metaFunction->GetOwnerMetaClass()->GetAllClassName() : "") + 
                                " 函数: " + (metaFunction ? metaFunction->GetName() : "") + "中  ";
                if (curClass != nullptr) {
                    sb += " 定义类 : " + curClass->GetAllClassName();
                }
                if (!defineName.empty()) {
                    sb += " 名称为: " + defineName;
                }
                sb += "与后边赋值语句中 ";
                if (compareClass != nullptr) {
                    sb += "表达式类为: " + compareClass->GetAllClassName();
                }
                
                if (relation == EClassRelation::No) {
                    sb += "类型不相同，可能会有强转，强转后可能默认值为null";
                    std::cout << sb << std::endl;
                    m_IsNeedCastStatements = true;
                } else if (relation == EClassRelation::Same) {
                    m_DefineVarMetaVariable->SetRealMetaType(expressRetMetaDefineType);
                } else if (relation == EClassRelation::Parent) {
                    sb += "类型不相同，可能会有强转， 返回值是父类型向子类型转换，存在错误转换!!";
                    std::cout << sb << std::endl;
                    m_IsNeedCastStatements = true;
                } else if (relation == EClassRelation::Child) {
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

// SetTRMetaVariable方法实现
void MetaDefineVarStatements::SetTRMetaVariable(MetaVariable* mv) {
    if (m_ExpressNode != nullptr && dynamic_cast<MetaExecuteStatementsNode*>(m_ExpressNode)) {
        dynamic_cast<MetaExecuteStatementsNode*>(m_ExpressNode)->UpdateTrMetaVariable(mv);
    }
    if (m_NextMetaStatements != nullptr) {
        m_NextMetaStatements->SetTRMetaVariable(mv);
    }
}

// SetDeep方法实现
void MetaDefineVarStatements::SetDeep(int dp) {
    MetaStatements::SetDeep(dp);
    if (auto mesn = dynamic_cast<MetaExecuteStatementsNode*>(m_ExpressNode)) {
        mesn->SetDeep(dp);
    }
}

// ToFormatString方法实现
std::string MetaDefineVarStatements::ToFormatString() const {
    std::string result;
    
    for (int i = 0; i < GetRealDeep(); i++) {
        result += Global::GetTabChar();
    }
    result += m_DefineVarMetaVariable->ToFormatString();
    result += " = ";
    
    if (m_DefineVarMetaVariable->GetMetaDefineType()->IsData()) {
        result += m_ExpressNode->ToFormatString();
    } else if (m_DefineVarMetaVariable->GetMetaDefineType()->IsEnum()) {
        // Enum处理
    } else {
        if (m_IsNeedCastStatements) {
            result += "(";
        }
        result += m_ExpressNode->ToFormatString();
        if (m_IsNeedCastStatements) {
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