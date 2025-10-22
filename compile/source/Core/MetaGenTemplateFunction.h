//****************************************************************************
//  File:      MetaGenTemplateFunction.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/30 12:00:00
//  Description: Meta enum's attribute
//****************************************************************************

#pragma once

#include "MetaMemberFunction.h"
#include <string>
#include <vector>

namespace SimpleLanguage {
namespace Core {

    class MetaGenTemplate;
    class MetaClass;

class MetaGenTemplateFunction : public MetaMemberFunction {
public:
    // 构造函数
    MetaGenTemplateFunction(MetaMemberFunction* mmc, const std::vector<MetaGenTemplate*>& list);
    MetaGenTemplateFunction(const MetaGenTemplateFunction& mgtf);
    MetaGenTemplateFunction(MetaClass* mc, const std::string& _name);
    virtual ~MetaGenTemplateFunction() = default;

    // 属性访问器
    MetaMemberFunction* GetSourceTemplateFunctionMetaMemberFunction() const { return m_SourceTemplateFunctionMetaMemberFunction; }
    const std::vector<MetaGenTemplate*>& GetMetaGenTemplateList() const { return m_MetaGenTemplateList; }

    // 方法
    bool MatchInputTemplateInsance(const std::vector<MetaClass*>& instMcList);
    void UpdateGenMemberFunctionByTemplateClass(MetaMemberFunction* mmf);
    MetaGenTemplate* GetMetaGenTemplate(const std::string& name);
    void ParseMetaMemberFunctionDefineMetaType();
    void UpdateRegsterGenMetaFunction();
    void UpdateRegsterGenMetaFunctionAndClass(const std::vector<MetaGenTemplate*>& classGtList);
    virtual bool Parse() override;
    virtual std::string ToString() const override;

protected:
    MetaMemberFunction* m_SourceTemplateFunctionMetaMemberFunction = nullptr;
    std::vector<MetaGenTemplate*> m_MetaGenTemplateList;
};

} // namespace Core
} // namespace SimpleLanguage
