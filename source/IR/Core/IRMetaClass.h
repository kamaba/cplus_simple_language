//****************************************************************************
//  File:      IRMetaClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/11/15 12:00:00
//  Description: Meta class's ir attribute
//****************************************************************************

#pragma once

#include "../IRManager.h"
#include "../IRMethod.h"
#include "../IRMetaVariable.h"
#include "../IRData.h"
#include "../IRExpress.h"
#include "../../Core/MetaClass.h"
#include "../../Core/MetaEnum.h"
#include "../../Core/MetaData.h"
#include "../../Core/MetaMemberVariable.h"
#include "../../Core/MetaMemberFunction.h"
#include "../../Debug/Log.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace SimpleLanguage {
namespace IR {

class IRMetaClass {
private:
    std::unordered_map<int, int> m_MetaMemberVariableHashCodeDict;
    std::vector<IRMetaVariable*> m_LocalIRMetaVariableList;
    std::vector<IRMetaVariable*> m_StaticIRMetaVariableList;
    std::vector<IRMethod*> m_IRNotStaticMethodList;
    std::string m_IRName;
    MetaClass* m_MetaClass = nullptr;

    int allocSize = 0;
    std::vector<EType> m_MetaTypeList;
    int m_ByteCount = 0;
    bool m_NeedCallInitMethod = false;

    static int s_TypeLength;

public:
    int id = 0;
    
    std::string GetIrName() const { return m_IRName; }
    int GetByteCount() const { return m_ByteCount; }
    bool GetNeedCallInitMethod() const { return m_NeedCallInitMethod; }
    std::vector<IRMetaVariable*>& GetLocalIRMetaVariableList() { return m_LocalIRMetaVariableList; }
    std::vector<IRMetaVariable*>& GetStaticIRMetaVariableList() { return m_StaticIRMetaVariableList; }
    
    IRMetaClass(MetaClass* mc);
    
    IRMethod* GetIRNonStaticMethodByIndex(int index);
    IRMethod* GetIRNonStaticMethodIndexByMethod(const std::string& name, int& index);
    int GetIRNonStaticMethodIndexByMethod(const std::string& name);
    int GetMetaMemberVariableIndexByHashCode(int id);
    void AddMetaMemberVariableIndexBindHashCode(int id, int newid);
    
    void CreateMemberData();
    void CreateMemberMethod();
    std::vector<IRData*> CreateStaticMetaMetaVariableIRList();
    
    std::string ToString();
};

} // namespace IR
} // namespace SimpleLanguage
