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
namespace Core {

    // 前向声明
    class MetaClass;
    class MetaType;
    class MetaMemberFunction;
    class MetaMemberFunctionTemplateNode;
    class MetaMemberFunctionNode;
    class MetaDefineParam;
    class MetaDefineParamCollection;
    class MetaTemplateCollection;
    class MetaBlockStatements;
    class MetaStatements;

    class MetaMemberFunctionTemplateNode
    {
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

    class MetaMemberFunctionNode
    {
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

    class MetaMemberFunction : public MetaFunction
    {
    public:
        MetaMemberFunction();
        MetaMemberFunction(MetaClass* mc);
        MetaMemberFunction(MetaClass* mc, void* fmmf); // 简化实现
        MetaMemberFunction(MetaClass* mc, const std::string& name);
        MetaMemberFunction(const MetaMemberFunction& mmf);
        virtual ~MetaMemberFunction() = default;

        // 属性访问器
        virtual std::string GetFunctionAllName() const;
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
        void* GetFileMetaMemberFunction() const { return m_FileMetaMemberFunction; }
        MetaMemberFunction* GetSourceMetaMemberFunction() const { return m_SourceMetaMemberFunction; }
        const std::vector<MetaType*>& GetBindStructTemplateFunctionMtList() const { return m_BindStructTemplateFunctionMtList; }
        const std::vector<MetaType*>& GetBindStructTemplateFunctionAndClassMtList() const { return m_BindStructTemplateFunctionAndClassMtList; }

        // 设置方法
        void SetSourceMetaMemberFunction(MetaMemberFunction* mmf) { m_SourceMetaMemberFunction = mmf; }
        void SetIsGet(bool isGet) { m_IsGet = isGet; }
        void SetIsSet(bool isSet) { m_IsSet = isSet; }
        void SetIsOverrideFunction(bool flag) { m_IsOverrideFunction = flag; }

        // 方法
        void* GetToken() const;
        bool IsEqualWithMMFByNameAndParam(MetaMemberFunction* mmf);
        void AddMetaDefineParam(MetaDefineParam* mdp);
        void AddMetaDefineTemplate(MetaTemplate* mt);
        void* AddGenTemplateMemberFunctionByMetaTypeList(MetaClass* mc, const std::vector<MetaType*>& list);
        void* AddGenTemplateMemberFunctionBySelf(MetaClass* mc, const std::vector<MetaClass*>& list);
        void* GetGenTemplateFunction(const std::vector<MetaClass*>& mcList);
        virtual bool Parse();
        virtual void ParseDefineMetaType();
        virtual void CreateMetaExpress();
        virtual bool ParseMetaExpress();
        void ParseStatements();
        void UpdateVritualFunctionName();
        MetaType* AddMetaPreTemplateFunction(MetaType* mt, bool& isGenMetaClass);
        MetaType* FindBindStructTemplateFunctionMtList(MetaType* mt);
        MetaType* FindBindStructTemplateFunctionAndClassMtList(MetaType* mt);

        // 静态方法
        static MetaStatements* CreateMetaSyntax(void* rootMs, MetaBlockStatements* currentBlockStatements);
        static MetaStatements* HandleMetaSyntax(MetaBlockStatements* currentBlockStatements, 
                                               MetaStatements*& beforeStatements,
                                               void* childFms);

        // 格式化方法
        std::string ToString() const;
        std::string ToFormatString() const override;

    protected:
        void init();

    private:
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
        void* m_FileMetaMemberFunction = nullptr; // 简化实现

        // 绑定构建元类型
        std::vector<MetaType*> m_BindStructTemplateFunctionMtList;
        std::vector<MetaType*> m_BindStructTemplateFunctionAndClassMtList;

        // 模板生成函数
        std::vector<void*> m_GenTempalteFunctionList; // 简化实现

        // 其他成员
        MetaDefineParamCollection* m_MetaMemberParamCollection = nullptr;
        MetaTemplateCollection* m_MetaMemberTemplateCollection = nullptr;
        MetaBlockStatements* m_MetaBlockStatements = nullptr;
        MetaClass* m_OwnerMetaClass = nullptr;
        MetaVariable* m_ThisMetaVariable = nullptr;
        MetaVariable* m_ReturnMetaVariable = nullptr;
        std::string m_VirtualFunctionName = "";
        std::string m_FunctionAllName = "";
    };

} // namespace Core
} // namespace SimpleLanguage
