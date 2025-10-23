//****************************************************************************
//  File:      MetaEnum.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta enum class
//****************************************************************************

#pragma once

#include "MetaClass.h"
#include <string>
#include <unordered_map>

namespace SimpleLanguage {
    namespace Compile
    {
        class FileMetaClass;;
    }
namespace Core {

    // 前向声明
    class MetaNode;
    class MetaVariable;
    class MetaMemberEnum;
    class MetaMemberVariable;
    class MetaInputTemplateCollection;
    class MetaType;
    class MetaConstExpressNode;
    class MetaNewObjectExpressNode;

    class MetaEnum : public MetaClass
    {
    public:
        MetaEnum(const std::string& name);
        virtual ~MetaEnum() = default;

        // 属性访问器
        MetaVariable* GetMetaVariable() const { return m_MetaVariable; }
        const std::unordered_map<std::string, MetaMemberEnum*>& GetMetaMemberEnumDict() const { return m_MetaMemberEnumDict; }

        // 成员变量管理
        MetaMemberVariable* GetMemberVariableByName(const std::string& name);
        void CreateValues();
        MetaMemberEnum* GetMemberEnumByName(const std::string& name);
        void AddMetaMemberEnum(MetaMemberEnum* mmd);

        // 解析方法
        void ParseFileMetaEnumMemeberEnum(Compile::FileMetaClass* fmc);
        void ParseMemberMetaEnumExpress();

        // 重写父类方法
        void HandleExtendMemberVariable() override;
        void ParseDefineComplete() override;

        // 格式化方法
        std::string ToFormatString() const override;

    protected:
        MetaVariable* m_MetaVariable = nullptr;
        std::unordered_map<std::string, MetaMemberEnum*> m_MetaMemberEnumDict;
    };

} // namespace Core
} // namespace SimpleLanguage