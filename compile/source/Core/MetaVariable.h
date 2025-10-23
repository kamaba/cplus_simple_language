//****************************************************************************
//  File:      MetaVariable.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description:  all variable 's define, if it's iterator style then use IteratorMetaVariable, other custom same style!
//****************************************************************************

#pragma once

#include "MetaBase.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace SimpleLanguage {
    namespace Compile
    {
        class Token;
    }
namespace Core {
    class MetaClass;
    class MetaType;
    class MetaBlockStatements;
enum class EVariableFrom {
    None = 0,
    Static = 1,
    Global = 2,
    Argument = 3,
    LocalStatement = 4,
    Member = 5,
    ArrayInner = 6
};

class MetaVariable : public MetaBase {
protected:
    MetaClass* m_OwnerMetaClass = nullptr;
    MetaType* m_DefineMetaType = nullptr;
    MetaType* m_RealMetaType = nullptr;
    EVariableFrom m_VariableFrom;
    std::vector<Compile::Token*> m_PintTokenList;
    bool m_IsParsed = false;
    bool m_IsStatic = false;
    bool m_IsConst = false;
    // 用来存放扩展包含变量
    std::unordered_map<std::string, MetaVariable*> m_MetaVariableDict;
    MetaBlockStatements* m_OwnerMetaBlockStatements = nullptr;
    MetaVariable* m_SourceMetaVariable = nullptr;

public:
    MetaVariable();
    MetaVariable(const MetaVariable& mv);
    MetaVariable(const std::string& name, EVariableFrom from, MetaBlockStatements* mbs, MetaClass* ownerClass, MetaType* mdt);
    virtual ~MetaVariable() = default;

    // 属性访问器
    virtual bool IsStatic() const { return m_IsStatic; }
    virtual bool IsConst() const { return m_IsConst; }
    virtual bool IsParsed() const { return m_IsParsed; }
    bool IsArgument() const { return m_VariableFrom == EVariableFrom::Argument; }
    bool IsGlobal() const { return m_VariableFrom == EVariableFrom::Global; }
    bool IsArray() const;

    MetaBlockStatements* GetOwnerMetaBlockStatements() const { return m_OwnerMetaBlockStatements; }
    EVariableFrom GetVariableFrom() const { return m_VariableFrom; }
    MetaType* GetMetaDefineType() const { return m_DefineMetaType; }
    MetaType* GetRealMetaType() const { return m_RealMetaType; }
    MetaClass* GetOwnerMetaClass() const { return m_OwnerMetaClass; }
    MetaVariable* GetSourceMetaVariable() const { return m_SourceMetaVariable; }
    Compile::Token* GetPingToken() const { return m_PintTokenList.size() > 0 ? m_PintTokenList[0] : nullptr; }

    // 设置方法
    virtual void SetOwnerMetaClass(MetaClass* ownerclass);
    void SetIsStatic(bool iss) { m_IsStatic = iss; }
    void SetIsConst(bool isc) { m_IsConst = isc; }
    void SetIsParsed(bool isp) { m_IsParsed = isp; }
    void SetRealMetaType(MetaType* realMt) { m_RealMetaType = realMt; }
    void SetVariableFrom(EVariableFrom from) { m_VariableFrom = from; }
    void SetOwnerMetaBlockStatements(MetaBlockStatements* mbs) { m_OwnerMetaBlockStatements = mbs; }
    void SetSourceMetaVariable(MetaVariable* mv) { m_SourceMetaVariable = mv; }

    // 方法
    MetaClass* GetOwnerClassTemplateClass() const;
    void AddPingToken(const std::string& path, int beginline, int beginpos, int endline, int endpos);
    void AddPingToken(Compile::Token* token);
    void AddMetaVariable(const std::string& name, MetaVariable* mv);
    MetaVariable* GetMetaVariable(const std::string& name) const;
    bool AddMetaVariable(MetaVariable* mv);
    void RemoveMetaVariable(const std::string& name);
    void ClearMetaVariable();
    std::vector<MetaVariable*> GetAllMetaVariableList() const;
    const std::unordered_map<std::string, MetaVariable*>& GetMetaVariableDict() const { return m_MetaVariableDict; }

    // 虚方法
    virtual void ParseDefineMetaType() {}
    virtual bool ParseMetaExpress() { return true; }
    virtual std::string ToFormatString() const;
    virtual std::string ToString() const;
};

} // namespace Core
} // namespace SimpleLanguage
