//****************************************************************************
//  File:      MetaMethod.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/30 12:00:00
//  Description: Meta method's attribute
//****************************************************************************

#pragma once

#include "MetaBase.h"
#include "../Compile/Token.h"
#include <string>
#include <vector>
#include <memory>

namespace SimpleLanguage {
namespace Core {
    class MetaVariable;
    class MetaStatements;
    class MetaClass;
    class MetaType;
    class MetaDefineParam;
    class MetaDefineParamCollection;
    class MetaBlockStatements;
    class MetaDefineTemplateCollection;
    class MetaTemplate;
    class MetaInputTemplateCollection;
    class MetaInputParamCollection;
enum class EMethodCallType : uint8_t {
    Local = 0,
    CSharp = 1,
    CPlus = 2
};

class LabelData {
public:
    std::string label;
    MetaStatements* frontStatements = nullptr;
    MetaStatements* nextStatements = nullptr;

    std::string GetLabel() { return label; }
    
    LabelData() = default;
    LabelData(const std::string& _label, MetaStatements* _nextStatements = nullptr) 
        : label(_label), nextStatements(_nextStatements) {}
};

class MetaFunction : public MetaBase {
public:
    // 构造函数
    MetaFunction(MetaClass* mc);
    MetaFunction(const MetaFunction& mf);
    virtual ~MetaFunction() = default;

    virtual void SetDeep(int deep);

    // 属性访问器
    MetaType* GetMetaDefineType();
    Compile::Token* GetPingToken() const;
    virtual bool IsStatic() const { return m_IsStatic; }
    virtual bool IsParsed() const { return m_IsParsed; }
    virtual std::string GetFunctionAllName() const;
    virtual std::string GetVirtualFunctionName() const { return m_VirtualFunctionName; }
    MetaVariable* GetThisMetaVariable() const { return m_ThisMetaVariable; }
    MetaVariable* GetReturnMetaVariable() const { return m_ReturnMetaVariable; }
    EMethodCallType GetMethodCallType() const { return m_MethodCallType; }
    MetaClass* GetOwnerMetaClass() const { return m_OwnerMetaClass; }
    MetaDefineParamCollection* GetMetaMemberParamCollection() const { return m_MetaMemberParamCollection; }
    MetaBlockStatements* GetMetaBlockStatements() const { return m_MetaBlockStatements; }
    MetaDefineTemplateCollection* GetMetaMemberTemplateCollection() const { return m_MetaMemberTemplateCollection; }

    // 设置方法
    virtual void SetOwnerMetaClass(MetaClass* ownerclass);
    void SetReturnMetaClass(MetaClass* metaClass);
    void SetMethodCallType(EMethodCallType type) { m_MethodCallType = type; }
    void SetVirtualFunctionName(const std::string& name) { m_VirtualFunctionName = name; }

    // 方法
    void AddFrontMetaStatements(MetaStatements* state);
    std::vector<MetaVariable*> GetCalcMetaVariableList(bool isIncludeArgument = false);
    LabelData* GetLabelDataById(const std::string& label);
    LabelData* AddLabelData(const std::string& label, MetaStatements* nextState = nullptr);
    void UpdateLabelData(LabelData* newld);
    void UpdateFunctionName();
    virtual bool Parse();
    MetaDefineParam* GetMetaDefineParamByName(const std::string& name);
    bool IsEqualMetaFunction(MetaFunction* mf);
    virtual bool IsEqualMetaInputParamCollection(MetaInputParamCollection* mpc);
    virtual bool IsEqualMetaDefineParamCollection(MetaDefineParamCollection* mdpc);
    MetaTemplate* GetMetaDefineTemplateByName(const std::string& name);
    virtual bool IsEqualMetaTemplateCollectionAndMetaParamCollection(MetaInputTemplateCollection* mitc, MetaDefineParamCollection* mpc);
    virtual std::string ToStatementString();

    // 格式化方法
    virtual std::string ToString() const;
    virtual std::string ToFormatString() const override;

protected:
    // 属性
    MetaClass* m_OwnerMetaClass = nullptr;
    MetaBlockStatements* m_MetaBlockStatements = nullptr;
    MetaVariable* m_ThisMetaVariable = nullptr;
    MetaVariable* m_ReturnMetaVariable = nullptr;
    MetaDefineParamCollection* m_MetaMemberParamCollection = nullptr;
    MetaDefineTemplateCollection* m_MetaMemberTemplateCollection = nullptr;
    EMethodCallType m_MethodCallType = EMethodCallType::Local;
    std::vector<LabelData*> m_LabelDataList;
    bool m_IsStatic = false;

    // Compile or Debug
    bool m_IsParsed = false;
    mutable std::string m_FunctionAllName;
    std::string m_VirtualFunctionName;
    std::vector<Compile::Token*> m_PintTokenList;
};

} // namespace Core
} // namespace SimpleLanguage
