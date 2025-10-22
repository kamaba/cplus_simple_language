//****************************************************************************
//  File:      MetaGenTemplateClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Generator Template Class's entity by Template Class
//****************************************************************************

#pragma once

#include "MetaClass.h"
#include "TypeManager.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace SimpleLanguage {
namespace Core {

class MetaGenTemplateClass : public MetaClass {
public:
    // 构造函数
    MetaGenTemplateClass(MetaClass* mtc, const std::vector<MetaGenTemplate*>& list);
    virtual ~MetaGenTemplateClass() = default;

    // 属性访问器
    MetaClass* GetMetaTemplateClass() const { return m_MetaTemplateClass; }
    const std::vector<MetaGenTemplate*>& GetMetaGenTemplateList() const { return m_MetaGenTemplateList; }
    virtual bool IsGenTemplate() const override { return true; }

    // 方法
    void UpdateRegsterGenMetaClass();
    virtual void SetDeep(int deep) override;
    MetaType* GetGenTemplateByIndex(int index);
    bool IsMatchByMetaTemplateClass(const std::vector<MetaGenTemplate*>& mgtList);
    void GetMetaTemplateMT(std::unordered_map<std::string, MetaType*>& mtdict);
    virtual MetaMemberVariable* GetMetaMemberVariableByName(const std::string& name) override;
    void AddMetaGenTemplate(MetaGenTemplate* mgt);
    MetaGenTemplate* GetMetaGenTemplate(const std::string& name);
    virtual void Parse() override;
    virtual void HandleExtendMemberVariable() override;
    virtual void HandleExtendMemberFunction() override;
    virtual void ParseMemberVariableDefineMetaType() override;
    MetaMemberVariable* ParseMetaMemberVariableDefineMetaType(MetaMemberVariable* mmv);
    virtual void ParseMemberFunctionDefineMetaType() override;
    void ParseMetaMemberFunctionDefineMetaType(MetaMemberFunction* mmf);
    void UpdateRegisterTemplateFunction();
    void UpdateRegisterTemplateFunctionSingle(MetaMemberFunction* mmf);
    virtual std::vector<MetaMemberVariable*> GetMetaMemberVariableListByFlag(bool isStatic) override;
    bool Adapter(MetaInputTemplateCollection* mitc);
    void UpdateTemplateInstanceStatement(MetaMemberFunction* mmf);
    virtual std::string ToString() const override;
    virtual std::string ToDefineTypeString() const override;
    virtual std::string ToFormatString() const override;

private:
    std::vector<MetaGenTemplate*> m_MetaGenTemplateList;
    MetaClass* m_MetaTemplateClass = nullptr;
};

} // namespace Core
} // namespace SimpleLanguage
