//****************************************************************************
//  File:      IRMethod.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "IRBase.h"
#include "IRData.h"
#include "../Core/MetaFunction.h"
#include <vector>
#include <string>

namespace SimpleLanguage {
namespace IR {

class IRManager;
class IRMetaVariable;
class IRBlockStatements;

class IRMethod : public IRBase {
public:
    std::string id;
    std::string virtualFunctionName;
    IRManager* irManager = nullptr;
    
    std::vector<IRMetaVariable*>& GetMethodArgumentList() { return m_MethodArgumentList; }
    const std::vector<IRMetaVariable*>& GetMethodArgumentList() const { return m_MethodArgumentList; }
    
    std::vector<IRMetaVariable*>& GetMethodLocalVariableList() { return m_MethodLocalVariableList; }
    const std::vector<IRMetaVariable*>& GetMethodLocalVariableList() const { return m_MethodLocalVariableList; }
    
    std::vector<IRMetaVariable*>& GetMethodReturnVariableList() { return m_MethodReturnList; }
    const std::vector<IRMetaVariable*>& GetMethodReturnVariableList() const { return m_MethodReturnList; }

private:
    std::vector<IRMetaVariable*> m_MethodArgumentList;
    std::vector<IRMetaVariable*> m_MethodLocalVariableList;
    std::vector<IRMetaVariable*> m_MethodReturnList;
    std::vector<IRData*> m_LabelList;
    std::vector<IRData*> m_IRDataList;
    MetaFunction* m_BindMetaFunction = nullptr;

public:
    IRMethod(IRManager* irma, MetaFunction* func);
    virtual ~IRMethod() = default;

    void Parse();
    void AddLabelDict(IRData* irdata);
    IRMetaVariable* GetIRLocalVariableById(int id);
    IRMetaVariable* GetIRArgumentById(int id);
    IRMetaVariable* GetReturnVariableById(int id);
    std::string ToIRString() override;
    std::string ToString() const;
};

} // namespace IR
} // namespace SimpleLanguage