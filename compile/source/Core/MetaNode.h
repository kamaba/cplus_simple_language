//****************************************************************************
//  File:      MetaNode.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta class's attribute
//****************************************************************************

#pragma once

#include "MetaBase.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace SimpleLanguage {
namespace Core {

    enum class EStructNodeType : uint8_t
    {
        Module = 0,
        Namespace = 1,
        Class = 2,
        Data = 3,
        Enum = 4
    };

    // 前向声明
    class MetaModule;
    class MetaNamespace;
    class MetaData;
    class MetaEnum;
    class MetaClass;

    class MetaNode
    {
    public:
        MetaNode();
        MetaNode(MetaModule* mm);
        MetaNode(MetaNamespace* mn);
        MetaNode(MetaData* md);
        MetaNode(MetaEnum* me);
        MetaNode(MetaClass* mc);
        virtual ~MetaNode() = default;

        // 属性访问器
        std::string GetName() const { return m_Name; }
        int GetDeep() const { return m_Deep; }
        int GetRealDeep() const { return m_Deep - m_AnchorDeep; }
        bool IsMetaModule() const { return m_MetaModule != nullptr; }
        bool IsMetaNamespace() const { return m_MetaNamespace != nullptr; }
        bool IsMetaData() const { return m_MetaData != nullptr; }
        bool IsMetaEnum() const { return m_MetaEnum != nullptr; }
        
        MetaNode* GetParentNode() const { return m_ParentNode; }
        MetaModule* GetMetaModule() const { return m_MetaModule; }
        MetaNamespace* GetMetaNamespace() const { return m_MetaNamespace; }
        MetaData* GetMetaData() const { return m_MetaData; }
        MetaEnum* GetMetaEnum() const { return m_MetaEnum; }
        int GetAnchorDeep() const { return m_AnchorDeep; }

        std::vector<MetaClass*> GetMetaClassList() const;
        virtual std::string GetAllName() const;
        std::string GetAllNameIncludeModule() const;
        
        const std::unordered_map<int, MetaClass*>& GetMetaTemplateClassDict() const { return m_MetaTemplateClassDict; }
        const std::unordered_map<std::string, MetaNode*>& GetChildrenMetaNodeDict() const { return m_ChildrenMetaNodeDict; }

        // 方法
        std::string GetAllName() const;
        MetaNode* AddMetaNamespace(MetaNamespace* namespaceName);
        MetaNode* AddMetaEnum(MetaEnum* me);
        MetaNode* AddMetaData(MetaData* me);
        MetaNode* AddMetaClass(MetaClass* mc);
        bool AddMetaNode(MetaNode* mn);
        bool IsMetaClass() const;
        bool IsIncludeMetaNode(const std::string& name) const;
        virtual MetaNode* GetChildrenMetaNodeByName(const std::string& name) const;
        MetaNamespace* GetMetaNamespaceByName(const std::string& name) const;
        MetaClass* GetMetaClassByTemplateCount(int count) const;
        void SetDeep(int deep);
        virtual void SetAnchorDeep(int addep);
        virtual MetaNode* GetMetaBaseInParentByName(const std::string& inputname, bool isInclude = true) const;
        bool RemoveMetaChildNode(MetaNode* mb);
        std::string ToFormatString() const;
        std::string ToString() const;

    protected:
        int m_Deep = 0;
        int m_AnchorDeep = 0;
        MetaNode* m_ParentNode = nullptr;

        EStructNodeType m_EStructNodeType = EStructNodeType::Namespace;
        MetaModule* m_MetaModule = nullptr;
        MetaNamespace* m_MetaNamespace = nullptr;
        // 模板个数类
        std::unordered_map<int, MetaClass*> m_MetaTemplateClassDict;
        MetaData* m_MetaData = nullptr;
        MetaEnum* m_MetaEnum = nullptr;
        std::string m_Name = "";
        std::string m_AllName = "";
        // 子节点
        std::unordered_map<std::string, MetaNode*> m_ChildrenMetaNodeDict;
    };

} // namespace Core
} // namespace SimpleLanguage
