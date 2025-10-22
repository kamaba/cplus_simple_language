//****************************************************************************
//  File:      IRMetaCallLink.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#include "IRMetaCallLink.h"
#include <sstream>

namespace SimpleLanguage {
namespace Core {
namespace IR {

void IRMetaCallLink::ParseToIRDataList(IRMethod* irMethod, const std::vector<MetaVisitNode*>& cnlist) {
    m_IRMethod = irMethod;

    for (size_t i = 0; i < cnlist.size(); i++) {
        auto cnode = cnlist[i];
        auto result = ExecOnceCnode(irMethod, cnode);
        irList.insert(irList.end(), result.begin(), result.end());
    }
}

std::vector<IRBase*> IRMetaCallLink::ExecOnceCnode(IRMethod* irMethod, MetaVisitNode* cnode) {
    std::vector<IRBase*> irList;
    
    if (cnode->GetVisitType() == MetaVisitNode::EVisitType::ConstValue) {
        IRExpress* ire = new IRExpress(irMethod, cnode->GetConstValueExpress());
        irList.push_back(ire);
    }
    else if (cnode->GetVisitType() == MetaVisitNode::EVisitType::Variable) {
        MetaVariable* mv = cnode->GetOrgTemplateMetaVariable();

        IRMetaType* irmt = nullptr;
        IRMetaClass* irmc = nullptr;
        
        if (mv->IsStatic()) {
            if (cnode->GetCallMetaType() != nullptr) {
                irmt = new IRMetaType(cnode->GetCallMetaType());
            } else {
                irmt = new IRMetaType(mv->GetMetaDefineType());
            }
            irmc = IRManager::GetInstance().GetIRMetaClassById(mv->GetOwnerClassTemplateClass()->GetHashCode());
        } else {
            irmc = IRManager::GetInstance().GetIRMetaClassById(mv->GetOwnerClassTemplateClass()->GetHashCode());
        }
        
        IRLoadVariable* irVar = IRLoadVariable::CreateLoadVariable(irmt, irmc, irMethod, mv);
        irList.push_back(irVar);
    }
    else if (cnode->GetVisitType() == MetaVisitNode::EVisitType::MethodCall) {
        auto mfc = cnode->GetMethodCall();
        IRCallFunction* irCallFun = new IRCallFunction(irMethod);
        irCallFun->Parse(mfc);
        irList.push_back(irCallFun);
    }
    else if (cnode->GetVisitType() == MetaVisitNode::EVisitType::New) {
        ParseNew(cnode, irMethod, irList);
    }

    return irList;
}

void IRMetaCallLink::ParseNew(MetaVisitNode* cnode, IRMethod* irMethod, std::vector<IRBase*>& irList) {
    IRMetaClass* irmc = IRManager::GetInstance().GetIRMetaClassById(cnode->GetCallMetaType()->GetTemplateMetaClass()->GetHashCode());
    
    if (cnode->GetCallMetaType()->GetEType() == EMetaTypeType::TemplateClassWithTemplate ||
        cnode->GetCallMetaType()->GetEType() == EMetaTypeType::Template) {
        auto irnew = new IRNew(irMethod, new IRMetaType(cnode->GetCallMetaType()));
        irList.push_back(irnew);
    }
    else if (cnode->GetCallMetaType()->GetEType() == EMetaTypeType::MetaClass) {
        IRNew* irnew = new IRNew(irMethod, irmc);
        irList.push_back(irnew);

        if (irmc->GetNeedCallInitMethod() == false) {
            if (irmc->GetLocalIRMetaVariableList().size() > 0) {
                bool isUseAssign = false;
                for (size_t x = 0; x < irmc->GetLocalIRMetaVariableList().size(); x++) {
                    auto lirmv = irmc->GetLocalIRMetaVariableList()[x];
                    if (cnode->GetMetaBraceStatementsContent() && 
                        cnode->GetMetaBraceStatementsContent()->GetAssignStatementsList().size() > 0) {
                        
                        IRLoadVariable* irlv = IRLoadVariable::CreateLoadVariable(nullptr, irmc, irMethod, cnode->GetOrgTemplateMetaVariable());
                        irList.push_back(irlv);
                        
                        for (size_t y = 0; y < cnode->GetMetaBraceStatementsContent()->GetAssignStatementsList().size(); y++) {
                            auto asl = cnode->GetMetaBraceStatementsContent()->GetAssignStatementsList()[y];

                            if (asl->GetMetaMemberVariable()->GetName() == lirmv->GetName()) {
                                IRExpress* irexp = new IRExpress(irMethod, asl->GetExpressNode());
                                irList.push_back(irexp);

                                IRStoreVariable* irStoreNodeVar3 = new IRStoreVariable(nullptr, irMethod, lirmv->GetIndex(), IRMetaVariableFrom::Member);
                                irList.push_back(irStoreNodeVar3);
                                isUseAssign = true;
                                break;
                            }
                        }

                        if (isUseAssign == false) {
                            IRExpress* irexp = new IRExpress(irMethod, lirmv->GetExpress());
                            irList.push_back(irexp);

                            IRStoreVariable* irStoreVar2 = new IRStoreVariable(nullptr, irMethod, lirmv->GetIndex(), IRMetaVariableFrom::Member);
                            irList.push_back(irStoreVar2);
                        }
                    }
                }
            }
        } else {
            if (cnode->GetMetaBraceStatementsContent() != nullptr && 
                cnode->GetMetaBraceStatementsContent()->GetAssignStatementsList().size() > 0) {
                
                IRLoadVariable* irlv = IRLoadVariable::CreateLoadVariable(nullptr, irmc, irMethod, cnode->GetOrgTemplateMetaVariable());
                irList.push_back(irlv);
                
                for (size_t y = 0; y < cnode->GetMetaBraceStatementsContent()->GetAssignStatementsList().size(); y++) {
                    auto asl = cnode->GetMetaBraceStatementsContent()->GetAssignStatementsList()[y];

                    IRExpress* irexp = new IRExpress(irMethod, asl->GetExpressNode());
                    irList.push_back(irexp);

                    IRStoreVariable* irStoreNodeVar3 = new IRStoreVariable(nullptr, irMethod, cnode->GetOrgTemplateMetaVariable()->GetHashCode(), IRMetaVariableFrom::LocalStatement);
                    irList.push_back(irStoreNodeVar3);
                }
            }
        }
    }

    if (cnode->GetMethodCall() != nullptr) {
        IRDup* irdup = new IRDup(irMethod);
        irList.push_back(irdup);

        IRCallFunction* ircf = new IRCallFunction(irMethod);
        ircf->Parse(cnode->GetMethodCall());
        irList.push_back(ircf);
    }
}

void IRMetaCallLink::ParseToIRDataListByIRManager(IRManager* irManager, const std::vector<MetaVisitNode*>& cnlist) {
    for (size_t i = 0; i < cnlist.size(); i++) {
        auto cnode = cnlist[i];
        if (cnode->GetVisitType() == MetaVisitNode::EVisitType::ConstValue) {
            IRExpress* ire = new IRExpress(irManager, cnode->GetConstValueExpress());
            irList.push_back(ire);
        }
        else if (cnode->GetVisitType() == MetaVisitNode::EVisitType::Variable) {
            MetaVariable* mv = cnode->GetOrgTemplateMetaVariable();
            if (mv->GetVariableFrom() == MetaVariable::EVariableFrom::Static ||
                mv->GetVariableFrom() == MetaVariable::EVariableFrom::Global) {
                
                auto irmt = new IRMetaType(mv->GetMetaDefineType());
                IRMetaClass* irmc = IRManager::GetInstance().GetIRMetaClassById(mv->GetOwnerMetaClass()->GetHashCode());
                IRLoadVariable* irVar = IRLoadVariable::CreateLoadVariable(irmt, irmc, m_IRMethod, mv);
                irList.push_back(irVar);
            } else {
                Log::AddGenIR(EError::None, "Error VM IRMetaCall 该位置不应该有非静态变量");
            }
        }
        else if (cnode->GetVisitType() == MetaVisitNode::EVisitType::MethodCall) {
            auto mfc = cnode->GetMethodCall();
            IRCallFunction* irCallFun = new IRCallFunction(m_IRMethod);
            irCallFun->Parse(cnode->GetMethodCall());
            irList.push_back(irCallFun);
        }
        else if (cnode->GetVisitType() == MetaVisitNode::EVisitType::New) {
            ParseNew(cnode, nullptr, irList);
        }
    }
}

std::string IRMetaCallLink::ToIRString() {
    std::stringstream sb;

    sb << "#Call#" << std::endl;
    for (size_t i = 0; i < irList.size(); i++) {
        sb << irList[i]->ToIRString() << std::endl;
    }
    return sb.str();
}

} // namespace IR
} // namespace Core
} // namespace SimpleLanguage
