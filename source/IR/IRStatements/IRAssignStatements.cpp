//****************************************************************************
//  File:      IRAssignStatements.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/11 12:00:00
//  Description:  handle assign statements syntax to instruction r!
//****************************************************************************

#include "IRAssignStatements.h"

namespace SimpleLanguage {
namespace IR {

IRAssignStatements::IRAssignStatements(IRMethod* method) : IRStatements(method) {
}

void IRAssignStatements::ParseIRStatements(MetaAssignStatements* ms) {
    auto clist = ms->GetLeftMetaExpress()->GetMetaCallLink()->GetCallNodeList();
    if (ms->IsNewStatements()) {
        MetaVisitNode* finalMVN = nullptr;
        for (size_t i = 0; i < clist.size(); i++) {
            finalMVN = clist[i];
            auto list = IRMetaCallLink::ExecOnceCnode(m_IrMethod, finalMVN);
            m_IRStatements.insert(m_IRStatements.end(), list.begin(), list.end());
        }

        if (finalMVN == nullptr) {
            Log::AddGenIR(EError::None, "没有最终表达式，错误处理");
            return;
        }
        
        if (ms->GetFinalMetaExpress() != nullptr) {
            m_IRExpress = new IRExpress(m_IrMethod, ms->GetFinalMetaExpress());
            m_IRStatements.push_back(m_IRExpress);

            if (finalMVN->GetVisitType() == MetaVisitNode::EVisitType::Variable) {
                // 这种是 Obja.Objb = new()的方式
                // 处理变量赋值逻辑
            }
        }
    } else {
        // 处理非新语句的赋值逻辑
        if (ms->GetFinalMetaExpress() != nullptr) {
            m_IRExpress = new IRExpress(m_IrMethod, ms->GetFinalMetaExpress());
            m_IRStatements.push_back(m_IRExpress);
        }
        
        // 处理存储变量逻辑
        if (ms->GetMetaVariable() != nullptr) {
            m_StoreVariable = IRStoreVariable::CreateIRStoreVariable(
                new IRMetaType(ms->GetMetaVariable()->GetMetaDefineType()),
                nullptr,
                m_IrMethod,
                ms->GetMetaVariable()
            );
            m_IRStatements.push_back(m_StoreVariable);
        }
    }
}

} // namespace IR
} // namespace SimpleLanguage
