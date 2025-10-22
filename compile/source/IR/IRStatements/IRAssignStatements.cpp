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
            Log::AddGenIR(EError::None, "û�����ձ��ʽ��������");
            return;
        }
        
        if (ms->GetFinalMetaExpress() != nullptr) {
            m_IRExpress = new IRExpress(m_IrMethod, ms->GetFinalMetaExpress());
            m_IRStatements.push_back(m_IRExpress);

            if (finalMVN->GetVisitType() == MetaVisitNode::EVisitType::Variable) {
                // ������ Obja.Objb = new()�ķ�ʽ
                // ���������ֵ�߼�
            }
        }
    } else {
        // ����������ĸ�ֵ�߼�
        if (ms->GetFinalMetaExpress() != nullptr) {
            m_IRExpress = new IRExpress(m_IrMethod, ms->GetFinalMetaExpress());
            m_IRStatements.push_back(m_IRExpress);
        }
        
        // ����洢�����߼�
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
