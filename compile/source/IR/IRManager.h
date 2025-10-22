//****************************************************************************
//  File:      IRManager.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "IRData.h"
#include "IRMethod.h"
#include "Core/IRMetaClass.h"
#include "Core/IRMetaVariable.h"
#include "../Core/MetaClass.h"
#include "../Core/MetaFunction.h"
#include "../VM/SValue.h"
#include "../Compile/Token.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace SimpleLanguage {
namespace IR {

class IRManager {
private:
    static IRManager* s_Instance;
    std::vector<IRMetaClass*> m_IRMetaClassList;
    std::vector<IRData*> m_IRDataList;
    std::vector<IRMetaVariable*> m_GlobalStaticVariableList;
    std::unordered_map<int, IRMetaVariable*> m_AllVariableDict;

public:
    static IRManager* GetInstance();
    
    std::vector<IRData*>& GetIrDataList() { return m_IRDataList; }
    std::vector<IRMetaClass*>& GetIrMetaClassList() { return m_IRMetaClassList; }
    std::unordered_map<std::string, IRMethod*>& GetIRMethodDict() { return IRMethodDict; }
    std::unordered_map<int, std::string>& GetIRStringDict() { return IRStringDict; }
    std::unordered_map<int, SValue*>& GetIRConstDict() { return IRConstDict; }
    std::vector<IRMetaVariable*>& GetGlobalStaticVariableList() { return m_GlobalStaticVariableList; }
    
    std::unordered_map<std::string, IRMethod*> IRMethodDict;
    std::unordered_map<int, std::string> IRStringDict;
    std::unordered_map<int, SValue*> IRConstDict;
    
    static EIROpCode GetConstIROpCode(EType etype);
    void TranslateIR();
    IRMetaClass* GetIRMetaClassById(int id);
    IRMetaClass* GetIRMetaClassByName(const std::string& allname);
    static std::string GetIRNameByMetaClass(MetaClass* mc);
    static std::string GetIRNameByMetaType(MetaType* mt);
    void AddIRMethod(IRMethod* irm);
    void AddGlobalMetaMemberVariable(IRMetaVariable* irmv);
    IRMethod* TranslateIRByFunction(MetaFunction* mf);
    void ParseClass();
    void ParseIRMethod();
    void AddIRData(IRData* irdata);
    void AddIRDataList(const std::vector<IRData*>& list);
    void PrintIR();
};

} // namespace IR
} // namespace SimpleLanguage
