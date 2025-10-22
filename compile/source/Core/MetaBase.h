//****************************************************************************
//  File:      MetaBase.h
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/5/17 12:00:00
//  Description:  Core MetaBase is a basement class, attribute value has name or tree's deepvalue or tree struct node!
//****************************************************************************

#pragma once

#include "../Define.h"
#include <string>
#include <memory>

namespace SimpleLanguage {
namespace Core {

    enum class RefFromType : uint8_t
    {
        Local = 0,
        CSharp = 1,
        Javascript = 2
    };

    // 前向声明
    class MetaNode;

    class MetaBase
    {
    public:
        MetaBase();
        MetaBase(const MetaBase& mb);
        virtual ~MetaBase() = default;

        // 属性访问器
        int GetDeep() const { return m_Deep; }
        int GetRealDeep() const { return m_Deep - m_AnchorDeep; }
        EPermission GetPermission() const { return m_Permission; }
        virtual std::string GetName() const { return m_Name; }
        RefFromType GetRefFromType() const { return m_RefFromType; }
        MetaNode* GetMetaNode() const { return m_MetaNode; }
        virtual ::std::string GetPathName() const { return m_MetaNode != nullptr ? m_MetaNode->GetAllName() : ""; }

        // 设置方法
        void SetRefFromType(RefFromType type) { m_RefFromType = type; }
        void SetName(const std::string& name) { m_Name = name; }
        virtual void SetAnchorDeep(int addep) { m_AnchorDeep = addep; }
        virtual void SetDeep(int deep) { m_Deep = deep; }
        void SetMetaNode(MetaNode* mn) { m_MetaNode = mn; }

        // 虚方法
        virtual std::string GetFormatString() const { return ""; }
        virtual std::string ToFormatString() const { return ""; }
        virtual std::string ToString() { return "";}

    protected:
        EPermission m_Permission = EPermission::Public;
        RefFromType m_RefFromType = RefFromType::Local;
        std::string m_Name = "";
        std::string m_AllName = "";
        MetaNode* m_MetaNode = nullptr;
        int m_Deep = 0;
        int m_AnchorDeep = 0;
    };

} // namespace Core
} // namespace SimpleLanguage
