//****************************************************************************
//  File:      IRMetaVariable.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/11/15 12:00:00
//  Description: Meta class's ir attribute
//****************************************************************************

#pragma once

#include "../IRMetaType.h"
#include "../IRData.h"
#include "../../Core/MetaVariable.h"
#include "../../Core/MetaMemberEnum.h"
#include "../../Core/MetaMemberData.h"
#include "../../Core/MetaMemberVariable.h"
#include "../../Core/MetaExpressNode.h"
#include "../../Debug/Log.h"
#include <vector>
#include <string>

namespace SimpleLanguage {
namespace IR {

enum class IRMetaVariableFrom {
    None,
    Argument,
    LocalStatement,
    Member,
    Static,
    Global,
    Return
};

class IRMetaVariable {
private:
    MetaExpressNode* m_ExpressNode = nullptr;
    std::vector<IRData*> m_IRDataList;
    IRMetaType* m_IRMetaType = nullptr;
    IRMetaVariableFrom m_IRMetaVariableFrom = IRMetaVariableFrom::None;
    bool m_IsTemplate = false;
    int m_Id = -1;
    int m_Index = -1;
    std::string m_Name = "";
    MetaVariable* m_MetaVariable = nullptr;

public:
    MetaExpressNode* GetExpress() const { return m_ExpressNode; }
    IRMetaType* GetIrMetaType() const { return m_IRMetaType; }
    IRMetaVariableFrom GetIrMetaVariableFrom() const { return m_IRMetaVariableFrom; }
    int GetId() const { return m_Id; }
    std::string GetName() const { return m_Name; }
    int GetIndex() const { return m_Index; }
    std::vector<IRData*>& GetIrDataList() { return m_IRDataList; }
    
    IRMetaVariable(MetaVariable* mv, int index = -1);
    IRMetaVariable(IRMetaClass* irmc, MetaMemberEnum* mme);
    IRMetaVariable(IRMetaClass* irmc, MetaMemberData* mmd);
    IRMetaVariable(IRMetaClass* irmc, MetaMemberVariable* mmv, int index = -1);
    
    void SetExpress(MetaExpressNode* men);
    void SetIRDataList(const std::vector<IRData*>& list);
    
    std::string ToString();
};

} // namespace IR
} // namespace SimpleLanguage
