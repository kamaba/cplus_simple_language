//****************************************************************************
//  File:      IRMetaCallLink.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../IRMethod.h"
#include "../IRBase.h"
#include "../IRExpress.h"
#include "../IRLoadVariable.h"
#include "../IRNew.h"
#include "../IRDup.h"
#include "../IRCallFunction.h"
#include "../IRStoreVariable.h"
#include "../IRManager.h"
#include "../../Core/MetaVariable.h"
#include "../../Core/MetaType.h"
#include "../../Core/MetaClass.h"
#include "../../Parse/MetaVisitNode.h"
#include "../../Debug/Log.h"
#include <vector>
#include <string>

namespace SimpleLanguage {
namespace Core {
namespace IR {

class IRMetaCallLink {
private:
    IRMethod* m_IRMethod = nullptr;
    std::vector<IRBase*> irList;

public:
    std::vector<IRBase*>& GetIrList() { return irList; }
    
    void ParseToIRDataList(IRMethod* irMethod, const std::vector<MetaVisitNode*>& cnlist);
    static std::vector<IRBase*> ExecOnceCnode(IRMethod* irMethod, MetaVisitNode* cnode);
    static void ParseNew(MetaVisitNode* cnode, IRMethod* irMethod, std::vector<IRBase*>& irList);
    void ParseToIRDataListByIRManager(IRManager* irManager, const std::vector<MetaVisitNode*>& cnlist);
    
    std::string ToIRString();
};

} // namespace IR
} // namespace Core
} // namespace SimpleLanguage
