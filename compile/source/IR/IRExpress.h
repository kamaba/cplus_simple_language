//****************************************************************************
//  File:      IRExpress.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/2 12:00:00
//  Description:  express convert ir code!
//****************************************************************************

#pragma once

#include "IRBase.h"
#include "IRData.h"
#include "../Core/MetaExpressNode.h"
#include "../Core/MetaExpressNode/MetaExpressBase.h"
#include <string>

namespace SimpleLanguage {
namespace IR {

class IRManager;
class IRMethod;
class IRMetaCallLink;
class IRMetaClass;
class IRNew;

class IRExpress : public IRBase {
private:
    IRManager* m_IRManager = nullptr;

public:
    IRExpress(IRMethod* irMethod, MetaExpressNode* node);
    IRExpress(IRManager* _irManager, MetaExpressNode* node);
    virtual ~IRExpress() = default;

    void CreateIRDataOne(MetaExpressNode* node);
    IRData* CreateOneSignIRData(ESingleOpSign opSign);
    IRData* CreateLeftAndRightIRData(ELeftRightOpSign opSign);
    std::string ToIRString() override;
};

} // namespace IR
} // namespace SimpleLanguage
