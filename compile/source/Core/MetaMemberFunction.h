//****************************************************************************
//  File:      MetaMemberFunction.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/30 12:00:00
//  Description: Meta enum's attribute
//****************************************************************************

#pragma once

#include "MetaFunction.h"
#include "../Define.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace SimpleLanguage {
    namespace Compile {
        class FileMetaMemberFunction;
        class Token;
        class FileMetaSyntax;
    }
namespace Core {

// Forward declarations
class MetaClass;
class MetaType;
class MetaMemberFunction;
class MetaDefineParam;
class MetaDefineParamCollection;
class MetaTemplateCollection;
class MetaBlockStatements;
class MetaStatements;
class MetaGenTemplateFunction;
class MetaMemberFunctionNode;

class MetaMemberFunctionTemplateNode {
public:
    MetaMemberFunctionTemplateNode();
    virtual ~MetaMemberFunctionTemplateNode() = default;

    const std::unordered_map<int, MetaMemberFunctionNode*>& GetMetaTemplateFunctionNodeDict() const { return m_MetaTemplateFunctionNodeDict; }

    void SetDeep(int deep);
    MetaMemberFunction* IsSameMetaMemeberFunction(MetaMemberFunction* mmf);
    bool AddMetaMemberFunction(MetaMemberFunction* mmf);
    void ParseMemberFunctionDefineMetaType();

private:
    std::unordered_map<int, MetaMemberFunctionNode*> m_MetaTemplateFunctionNodeDict;
};

class MetaMemberFunctionNode {
public:
    MetaMemberFunctionNode();
    virtual ~MetaMemberFunctionNode() = default;

    const std::unordered_map<int, std::vector<MetaMemberFunction*>>& GetMetaParamFunctionDict() const { return m_MetaParamFunctionDict; }

    void SetDeep(int deep);
    MetaMemberFunction* IsSameMetaMemeberFunction(MetaMemberFunction* mmf);
    bool AddMetaMemberFunction(MetaMemberFunction* mmf);
    void ParseMemberFunctionDefineMetaType();
    std::vector<MetaMemberFunction*> GetMetaMemberFunctionListByParamCount(int count);

private:
    std::unordered_map<int, std::vector<MetaMemberFunction*>> m_MetaParamFunctionDict;
};

class MetaMemberFunction : public MetaFunction {
public:
    MetaMemberFunction(MetaClass* mc = nullptr);
    MetaMemberFunction(MetaClass* mc, Compile::FileMetaMemberFunction* fmmf);
    MetaMemberFunction(MetaClass* mc, const std::string& name);
    MetaMemberFunction(const MetaMemberFunction& mmf);
    virtual ~MetaMemberFunction() = default;

    // Properties
    virtual std::string GetFunctionAllName() const override;
    int GetParseLevel() const;
    bool IsTemplateFunction() const { return m_IsTemplateFunction; }
    bool IsWithInterface() const { return m_IsWithInterface; }
    bool IsOverrideFunction() const { return m_IsOverrideFunction; }
    bool IsConstructInitFunction() const { return m_ConstructInitFunction; }
    bool IsGet() const { return m_IsGet; }
    bool IsSet() const { return m_IsSet; }
    bool IsFinal() const { return m_IsFinal; }
    bool IsCanRewrite() const { return m_IsCanRewrite; }
    bool IsTemplateInParam() const { return m_IsTemplateInParam; }
    Compile::FileMetaMemberFunction* GetFileMetaMemberFunction() const { return m_FileMetaMemberFunction; }
    MetaMemberFunction* GetSourceMetaMemberFunction() const { return m_SourceMetaMemberFunction; }
    const std::vector<MetaType*>& GetBindStructTemplateFunctionMtList() const { return m_BindStructTemplateFunctionMtList; }
    const std::vector<MetaType*>& GetBindStructTemplateFunctionAndClassMtList() const { return m_BindStructTemplateFunctionAndClassMtList; }
    const std::vector<MetaGenTemplateFunction*>& GetGenTempalteFunctionList() const { return m_GenTempalteFunctionList; }

    // Setter methods
    void SetSourceMetaMemberFunction(MetaMemberFunction* mmf);
    void SetIsGet(bool isGet);
    void SetIsSet(bool isSet);
    void SetIsOverrideFunction(bool flag);

    // Methods
    Compile::Token* GetToken() const;
    bool IsEqualWithMMFByNameAndParam(MetaMemberFunction* mmf);
    void AddMetaDefineParam(MetaDefineParam* mdp);
    void AddMetaDefineTemplate(MetaTemplate* mt);
    MetaGenTemplateFunction* AddGenTemplateMemberFunctionByMetaTypeList(MetaClass* mc, const std::vector<MetaType*>& list);
    MetaGenTemplateFunction* AddGenTemplateMemberFunctionBySelf(MetaClass* mc, const std::vector<MetaClass*>& list);
    MetaGenTemplateFunction* GetGenTemplateFunction(const std::vector<MetaClass*>& mcList);
    virtual bool Parse() override;
    virtual void ParseDefineMetaType();
    virtual void CreateMetaExpress();
    virtual bool ParseMetaExpress();
    void ParseStatements();
    void UpdateVritualFunctionName();
    MetaType* AddMetaPreTemplateFunction(MetaType* mt, bool& isGenMetaClass);
    MetaType* FindBindStructTemplateFunctionMtList(MetaType* mt);
    MetaType* FindBindStructTemplateFunctionAndClassMtList(MetaType* mt);

    // Static methods
    static MetaStatements* CreateMetaSyntax(Compile::FileMetaSyntax* rootMs, MetaBlockStatements* currentBlockStatements);
    static MetaStatements* HandleMetaSyntax(MetaBlockStatements* currentBlockStatements, 
                                           MetaStatements*& beforeStatements,
        Compile::FileMetaSyntax* childFms);

    // Override methods
    virtual bool Equals(MetaBase* obj) const;
    virtual std::string ToString() const override;
    virtual std::string ToFormatString() const override;
    virtual int GetHashCode() const;

protected:
    void Init();

protected:
    bool m_IsTemplateFunction = false;
    bool m_IsOverrideFunction = false;
    bool m_IsGet = false;
    bool m_IsSet = false;
    bool m_IsFinal = false;
    bool m_IsCanRewrite = false;
    bool m_IsTemplateInParam = false;
    bool m_ConstructInitFunction = false;
    bool m_IsWithInterface = false;
    MetaMemberFunction* m_SourceMetaMemberFunction = nullptr;
    Compile::FileMetaMemberFunction* m_FileMetaMemberFunction = nullptr;

    // 绑定构建元类型
    std::vector<MetaType*> m_BindStructTemplateFunctionMtList;
    std::vector<MetaType*> m_BindStructTemplateFunctionAndClassMtList;

    // 模板生成函数
    std::vector<MetaGenTemplateFunction*> m_GenTempalteFunctionList;

    // 其他成员
    std::string m_VirtualFunctionName = "";
    std::string m_FunctionAllName = "";
};

} // namespace Core
} // namespace SimpleLanguage