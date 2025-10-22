//****************************************************************************
//  File:      MetaAssignStatements.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#include "MetaAssignStatements.h"

namespace SimpleLanguage {
namespace Core {

// MetaAssignManager implementation
MetaVariable* MetaAssignManager::GetJudgmentValueMetaVariable() const {
    return m_JudgmentValueMetaVariable;
}

MetaExpressNode* MetaAssignManager::GetExpressNode() const {
    return m_ExpressNode;
}

bool MetaAssignManager::IsNeedSetMetaVariable() const {
    return m_IsNeedSetMetaVariable;
}

MetaAssignManager::MetaAssignManager(MetaExpressNode* expressNode, MetaBlockStatements* mbs, MetaType* defaultMdt) {
    m_ExpressNode = expressNode;
    m_MetaBlockStatements = mbs;
    m_MetaDefineType = defaultMdt;
    CreateMetaVariable();
}

void MetaAssignManager::CreateMetaVariable() {
    if (auto mcen = dynamic_cast<MetaCallLinkExpressNode*>(m_ExpressNode)) {
        auto retMc = mcen->GetReturnMetaClass();
        if (retMc == CoreMetaClassManager::GetInstance().GetBooleanMetaClass()) {
            m_JudgmentValueMetaVariable = mcen->GetMetaVariable();
        } else {
            Log::AddInStructMeta(EError::None, "Error 返回的判断语句: " + mcen->ToTokenString() + "   并非是boolean类型!");
        }
    } else if (auto mconen = dynamic_cast<MetaConstExpressNode*>(m_ExpressNode)) {
        Log::AddInStructMeta(EError::None, "Error -------------------------------------------");
    } else if (auto moen = dynamic_cast<MetaOpExpressNode*>(m_ExpressNode)) {
        if (moen->isEqualType) {
            // m_JudgmentValueMetaVariable = CreateOptimizeAfterExpress;
        } else {
            // Log::AddInStructMeta(EError::None, "Error 返回的判断语句: " + mcen->ToTokenString() + "   并非是boolean类型!");
        }
    } else {
        Log::AddInStructMeta(EError::None, "Error -------------------------------------------");
    }
    
    if (m_JudgmentValueMetaVariable == nullptr) {
        m_IsNeedSetMetaVariable = true;
        std::string name = "autocreate_" + std::to_string(reinterpret_cast<uintptr_t>(this));
        m_JudgmentValueMetaVariable = new MetaVariable(name, MetaVariable::EVariableFrom::LocalStatement, m_MetaBlockStatements, m_MetaBlockStatements->GetOwnerMetaClass(), m_MetaDefineType);
    }
}

// MetaAssignStatements implementation
MetaExpressNode* MetaAssignStatements::GetFinalMetaExpress() const {
    return m_FinalMetaExpress;
}

MetaVariable* MetaAssignStatements::GetMetaVariable() const {
    return m_MetaVariable;
}

MetaExpressNode* MetaAssignStatements::GetExpressNode() const {
    return m_ExpressNode;
}

MetaCallLinkExpressNode* MetaAssignStatements::GetLeftMetaExpress() const {
    return m_LeftMetaExpress;
}

bool MetaAssignStatements::IsNewStatements() const {
    return false;
}

MetaAssignStatements::MetaAssignStatements(MetaBlockStatements* mbs) : MetaStatements(mbs) {
}

MetaAssignStatements::MetaAssignStatements(MetaBlockStatements* mbs, FileMetaOpAssignSyntax* fmos) : MetaStatements(mbs) {
    m_FileMetaOpAssignSyntax = fmos;
    Parse();
}

MetaAssignStatements::MetaAssignStatements(MetaBlockStatements* mbs, FileMetaDefineVariableSyntax* fmos) : MetaStatements(mbs) {
    m_FileMetaDefineVariableSyntax = fmos;
    m_MetaVariable = mbs->GetOwnerMetaClass()->GetMetaMemberVariableByName(m_FileMetaDefineVariableSyntax->GetName());
    Parse();
}

void MetaAssignStatements::Parse() {
    MetaCallLink* metaCallLink = nullptr;
    
    if (m_FileMetaOpAssignSyntax != nullptr) {
        metaCallLink = new MetaCallLink(m_FileMetaOpAssignSyntax->GetVariableRef(),
            m_OwnerMetaBlockStatements ? m_OwnerMetaBlockStatements->GetOwnerMetaClass() : nullptr, 
            m_OwnerMetaBlockStatements, nullptr, nullptr);
        m_SignToken = m_FileMetaOpAssignSyntax->GetAssignToken();
    } else if (m_FileMetaDefineVariableSyntax != nullptr) {
        m_SignToken = m_FileMetaDefineVariableSyntax->GetAssignToken();
    }
    
    if (metaCallLink == nullptr) {
        Log::AddInStructMeta(EError::None, "Error MetaAssignStatements ParseDefine!!!" + (m_FileMetaOpAssignSyntax ? m_FileMetaOpAssignSyntax->GetVariableRef()->ToTokenString() : ""));
        return;
    }
    
    m_LeftMetaExpress = new MetaCallLinkExpressNode(metaCallLink);
    
    ETokenType ett = m_SignToken->GetType();
    bool isCanNew = false;
    
    switch (ett) {
        case ETokenType::Assign:
            isCanNew = true;
            break;
        case ETokenType::PlusAssign:
            m_IsAssign = true;
            m_AutoAddExpressOpSign = ELeftRightOpSign::Add;
            break;
        case ETokenType::MinusAssign:
            m_IsAssign = true;
            m_AutoAddExpressOpSign = ELeftRightOpSign::Minus;
            break;
        case ETokenType::DivideAssign:
            m_IsAssign = true;
            m_AutoAddExpressOpSign = ELeftRightOpSign::Divide;
            break;
        case ETokenType::MultiplyAssign:
            m_IsAssign = true;
            m_AutoAddExpressOpSign = ELeftRightOpSign::Multiply;
            break;
        case ETokenType::InclusiveOrAssign:
            m_IsAssign = true;
            m_AutoAddExpressOpSign = ELeftRightOpSign::InclusiveOr;
            break;
        case ETokenType::CombineAssign:
            m_IsAssign = true;
            m_AutoAddExpressOpSign = ELeftRightOpSign::Combine;
            break;
        case ETokenType::XORAssign:
            m_IsAssign = true;
            m_AutoAddExpressOpSign = ELeftRightOpSign::XOR;
            break;
        case ETokenType::DoublePlus:
            m_IsAssign = true;
            m_AutoAddExpressOpSign = ELeftRightOpSign::Add;
            break;
        case ETokenType::DoubleMinus:
            m_IsAssign = true;
            m_AutoAddExpressOpSign = ELeftRightOpSign::Minus;
            break;
        default:
            Log::AddInStructMeta(EError::None, "Error 赋值语句解析符号暂不支持: " + std::to_string(static_cast<int>(ett)));
            break;
    }
    
    AllowUseSettings* auc = new AllowUseSettings();
    auc->SetUseNotStatic(m_FileMetaOpAssignSyntax ? (m_FileMetaOpAssignSyntax->GetStaticToken() != nullptr ? false : true) : true);
    auc->SetUseNotConst(m_FileMetaOpAssignSyntax ? (m_FileMetaOpAssignSyntax->GetConstToken() == nullptr ? false : true) : true);
    auc->SetSetterFunction(true);
    auc->SetGetterFunction(false);
    m_LeftMetaExpress->Parse(auc);
    m_LeftMetaExpress->CalcReturnType();
    
    if (m_LeftMetaExpress->GetMetaCallLink()->GetFinalCallNode()->GetVisitType() == MetaVisitNode::EVisitType::MethodCall) {
        auto fun = m_LeftMetaExpress->GetMetaCallLink()->GetFinalCallNode()->GetMethodCall()->GetFunction();
        if (auto mmf = dynamic_cast<MetaMemberFunction*>(fun)) {
            if (mmf->IsSet()) {
                m_IsSetStatements = true;
            }
        }
    }
    
    MetaType* expressMdt = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    if (!m_IsSetStatements) {
        m_MetaVariable = m_LeftMetaExpress->GetMetaVariable();
        if (m_MetaVariable == nullptr) {
            Log::AddInStructMeta(EError::None, "Error 变量没有发现" + m_LeftMetaExpress->ToTokenString());
            return;
        }
        if (m_MetaVariable->IsConst()) {
            Log::AddInStructMeta(EError::None, "Error 类型为Const类型，不允许使用赋值!!");
        }
        
        m_Name = m_MetaVariable->GetName();
        if (m_MetaVariable->IsGlobal()) {
            if (GetOwnerMetaClass()->GetName() == "Project") {
                // 允许全局变量赋值
            } else {
                Log::AddInStructMeta(EError::None, "Error 只能在Project工程下的函数中，给全局变量赋值!!");
                return;
            }
        }
        expressMdt = m_MetaVariable->GetMetaDefineType();
    }
    
    if (m_FileMetaOpAssignSyntax && m_FileMetaOpAssignSyntax->GetExpress() != nullptr) {
        CreateExpressParam* cep = new CreateExpressParam();
        cep->SetOwnerMBS(m_OwnerMetaBlockStatements);
        cep->SetMetaType(expressMdt);
        cep->SetOwnerMetaClass(GetOwnerMetaClass());
        cep->SetFme(m_FileMetaOpAssignSyntax->GetExpress());
        cep->SetIsStatic(false);
        cep->SetIsConst(false);
        cep->SetParseFrom(EParseFrom::StatementRightExpress);
        cep->SetEqualMetaVariable(m_MetaVariable);
        m_ExpressNode = ExpressManager::CreateExpressNodeByCEP(cep);
        
        if (m_ExpressNode == nullptr) {
            Log::AddInStructMeta(EError::None, "Error 解析新建变量语句时，表达式解析为空!!");
            return;
        }
    } else {
        if (m_SignToken != nullptr && m_ExpressNode == nullptr) {
            if (m_SignToken->GetType() == ETokenType::DoublePlus ||
                m_SignToken->GetType() == ETokenType::DoubleMinus) {
                m_ExpressNode = new MetaConstExpressNode(EType::Int32, 1);
            }
        }
    }
    
    if (m_IsAssign) {
        m_FinalMetaExpress = new MetaOpExpressNode(m_LeftMetaExpress, m_ExpressNode, m_AutoAddExpressOpSign);
    } else {
        m_FinalMetaExpress = m_ExpressNode;
    }
    
    if (m_FinalMetaExpress == nullptr) {
        Log::AddInStructMeta(EError::None, "Error 类: " + (GetOwnerMetaClass() ? GetOwnerMetaClass()->GetAllClassName() : "") + "没有找到变量:[" + (m_FileMetaOpAssignSyntax ? m_FileMetaOpAssignSyntax->GetExpress()->ToFormatString() : "") + "]的定义!!! 69 ");
        return;
    }
    
    AllowUseSettings* auc2 = new AllowUseSettings();
    m_FinalMetaExpress->Parse(auc2);
    m_FinalMetaExpress->CalcReturnType();
    
    MetaType* expressRetMetaDefineType = m_FinalMetaExpress->GetReturnMetaDefineType();
    if (expressRetMetaDefineType == nullptr) {
        Log::AddInStructMeta(EError::None, "Error 解析新建变量语句时，表达式返回类型为空!!__3");
        return;
    }
    
    if (m_IsSetStatements == false) {
        MetaType* mdt = m_MetaVariable->GetRealMetaType();
        
        if (expressRetMetaDefineType->GetMetaClass() == CoreMetaClassManager::GetInstance().GetNullMetaClass()) {
            // 允许null赋值
        } else {
            ClassManager::EClassRelation relation = ClassManager::EClassRelation::No;
            MetaClass* curClass = mdt->GetMetaClass();
            
            MetaClass* compareClass = nullptr;
            auto constExpressNode = dynamic_cast<MetaConstExpressNode*>(m_ExpressNode);
            if (constExpressNode != nullptr && constExpressNode->GetEType() == EType::Null) {
                relation = ClassManager::EClassRelation::Same;
            } else {
                compareClass = expressRetMetaDefineType->GetMetaClass();
                if (mdt->IsTemplate()) {
                    if (curClass == compareClass) {
                        relation = ClassManager::EClassRelation::Same;
                    }
                } else {
                    relation = ClassManager::ValidateClassRelationByMetaClass(curClass, compareClass);
                }
            }
            
            std::stringstream sb;
            sb << "Warning 在类: " << (m_OwnerMetaBlockStatements ? m_OwnerMetaBlockStatements->GetOwnerMetaClass()->GetAllClassName() : "") 
               << " 函数: " << (m_OwnerMetaBlockStatements ? m_OwnerMetaBlockStatements->GetOwnerMetaFunction()->GetName() : "") << "中  ";
            if (curClass != nullptr) {
                sb << " 定义类 : " << curClass->GetAllClassName();
            }
            sb << " 名称为: " << (m_Name ? m_Name->ToString() : "");
            sb << "与后边赋值语句中 ";
            if (compareClass != nullptr) {
                sb << "表达式类为: " << compareClass->GetAllClassName();
            }
            
            if (relation == ClassManager::EClassRelation::No) {
                sb << "类型不相同，可能会有强转，强转后可能默认值为null";
                Log::AddInStructMeta(EError::None, sb.str());
                m_IsNeedCastState = true;
            } else if (relation == ClassManager::EClassRelation::Similar) {
                sb << "数字类型相似，可能会有强转会有精度的丢失!";
                Log::AddInStructMeta(EError::None, sb.str());
                m_IsNeedCastState = true;
            } else if (relation == ClassManager::EClassRelation::Same) {
                // 类型相同，无需转换
            } else if (relation == ClassManager::EClassRelation::Parent) {
                sb << "类型不相同，可能会有强转， 返回值是父类型向子类型转换，存在错误转换!!";
                Log::AddInStructMeta(EError::None, sb.str());
                m_IsNeedCastState = true;
            } else if (relation == ClassManager::EClassRelation::Child) {
                if (compareClass != nullptr) {
                    m_MetaVariable->SetMetaDefineType(expressRetMetaDefineType);
                }
            } else {
                sb << "表达式错误，或者是定义类型错误";
                Log::AddInStructMeta(EError::None, sb.str());
            }
        }
    } else {
        MetaInputParam* mip = new MetaInputParam(m_ExpressNode);
        // 处理setter函数调用
    }
}

void MetaAssignStatements::UpdateOwnerMetaClass(MetaClass* ownerclass) {
    if (m_MetaVariable != nullptr) {
        m_MetaVariable->SetOwnerMetaClass(ownerclass);
    }
    MetaStatements::UpdateOwnerMetaClass(ownerclass);
}

std::string MetaAssignStatements::ToFormatString() {
    std::string result = GetFormatString();
    result += ";";
    result += "\n";
    if (m_NextMetaStatements != nullptr) {
        result += m_NextMetaStatements->ToFormatString();
    }
    return result;
}

std::string MetaAssignStatements::GetFormatString() {
    std::string result;
    for (int i = 0; i < m_RealDeep; i++) {
        result += Global::GetTabChar();
    }
    
    if (m_IsSetStatements) {
        result += m_LeftMetaExpress->GetMetaCallLink()->ToFormatString();
    } else {
        if (m_MetaVariable != nullptr) {
            result += m_LeftMetaExpress->GetMetaCallLink()->ToFormatString();
        } else {
            result += "NotFind[ " + m_LeftMetaExpress->GetMetaCallLink()->ToFormatString();
        }
        result += " = ";
        
        if (m_FinalMetaExpress != nullptr) {
            result += m_FinalMetaExpress->ToFormatString();
        }
        if (m_IsNeedCastState) {
            result += ".Cast<";
            result += m_MetaVariable->GetMetaDefineType()->ToFormatString();
            result += ">()";
        }
    }
    return result;
}

} // namespace Core
} // namespace SimpleLanguage
