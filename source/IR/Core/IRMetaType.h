//****************************************************************************
//  File:      IRMetaType.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2025/9/5 12:00:00
//  Description: Meta class's ir attribute
//****************************************************************************

#pragma once

#include "../IRMetaClass.h"
#include "../../Core/MetaType.h"
#include "../../Core/MetaClass.h"
#include <vector>
#include <string>

namespace SimpleLanguage {
namespace IR {

class IRMetaType {
private:
    IRMetaClass* m_IRMetaClass = nullptr;
    std::vector<IRMetaType*> m_IRMetaTypeList;
    int m_TemplateIndex = -1;

public:
    IRMetaClass* GetIrMetaClass() const { return m_IRMetaClass; }
    std::vector<IRMetaType*>& GetIrMetaTypeList() { return m_IRMetaTypeList; }
    int GetTemplateIndex() const { return m_TemplateIndex; }
    
    IRMetaType(MetaType* type);
    IRMetaType(IRMetaClass* irmc, const std::vector<IRMetaType*>& irlist);
    
    std::string ToString();
};

} // namespace IR
} // namespace SimpleLanguage
