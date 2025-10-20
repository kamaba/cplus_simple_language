//****************************************************************************
//  File:      IRVariable.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/30 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "IRBase.h"
#include "IRData.h"
#include "../Core/MetaVariable.h"
#include "../Core/MetaType.h"
#include <string>

namespace SimpleLanguage {
namespace IR {

class IRMetaType;
class IRMetaClass;
class IRMethod;

enum class IRMetaVariableFrom {
    Global,
    Argument,
    Member,
    LocalStatement,
    Static
};

class IRLoadVariable : public IRBase {
private:
    IRData* m_Data = nullptr;

public:
    static IRLoadVariable* CreateLoadVariable(IRMetaType* irmt, IRMetaClass* irmc, IRMethod* _irMethod, MetaVariable* mv);

protected:
    IRLoadVariable(IRMetaType* irmt, IRMethod* _irMethod, int id, IRMetaVariableFrom irmvf);

public:
    virtual ~IRLoadVariable() = default;
    std::string ToIRString() override;
};

class IRStoreVariable : public IRBase {
private:
    IRData* m_Data = nullptr;

public:
    static IRStoreVariable* CreateIRStoreVariable(IRMetaType* irmt, IRMetaClass* irmc, IRMethod* _irMethod, MetaVariable* mv);
    static IRStoreVariable* CreateStaticReturnIRSV();

    IRStoreVariable(IRMetaType* irmt, IRMethod* _irMethod, int id, IRMetaVariableFrom irmvf);
    
protected:
    IRStoreVariable();

public:
    virtual ~IRStoreVariable() = default;
    std::string ToIRString() override;
};

} // namespace IR
} // namespace SimpleLanguage
