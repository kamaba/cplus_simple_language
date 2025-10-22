//****************************************************************************
//  File:      MetaExpressBase.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once


#include <string>

namespace SimpleLanguage {
namespace Core {
    class AllowUseSettings;
enum class ELeftRightOpSign {
    Add,
    Minus,
    Multiply,
    Divide,
    Modulo,
    InclusiveOr,
    Combine,
    XOR,
    Shi,
    Shr,
    Equal,
    NotEqual,
    Greater,
    GreaterOrEqual,
    Less,
    LessOrEqual,
    And,
    Or
};

enum class ESingleOpSign {
    None,
    Neg,
    Not,
    Xor
};

class MetaExpressNode {
protected:
    MetaClass* m_OwnerMetaClass = nullptr;
    MetaBlockStatements* m_OwnerMetaBlockStatements = nullptr;
    MetaType* m_MetaDefineType = nullptr;

public:
    virtual ~MetaExpressNode() = default;
    
    virtual int GetOpLevel() const {
        return MetaTypeFactory::GetOpLevelByMetaType(m_MetaDefineType);
    }
    
    MetaType* GetMetaDefineType() const { return m_MetaDefineType; }
    
    virtual int CalcParseLevel(int level) { return level; }
    virtual void CalcReturnType() { }
    virtual void Parse(AllowUseSettings* auc) { }
    
    MetaClass* GetReturnMetaClass() {
        if (m_MetaDefineType == nullptr) {
            GetReturnMetaDefineType();
        }
        return m_MetaDefineType ? m_MetaDefineType->GetMetaClass() : nullptr;
    }
    
    virtual void SetMetaType(MetaType* mt) {
        m_MetaDefineType = mt;
    }
    
    virtual MetaType* GetReturnMetaDefineType() {
        return m_MetaDefineType;
    }
    
    virtual std::string ToFormatString() {
        return "";
    }
    
    virtual std::string ToTokenString() {
        return "";
    }
};

} // namespace Core
} // namespace SimpleLanguage
