//****************************************************************************
//  File:      MetaData.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta data class
//****************************************************************************

#pragma once

#include "MetaClass.h"
#include "MetaMemberData.h"
#include "../Compile/FileMeta/FileMetaClass.h"
#include "../Compile/Token.h"
#include <unordered_map>
#include <vector>
#include <string>

namespace SimpleLanguage {
namespace Core {

    class MetaData : public MetaClass
    {
    public:
        // Properties
        bool IsConst() const { return m_IsConst; }
        bool IsStatic() const { return m_IsStatic; }
        bool IsDynamic() const { return m_IsDynamic; }
        const std::unordered_map<std::string, MetaMemberData*>& GetMetaMemberDataDict() const { return m_MetaMemberDataDict; }

        // Constructors
        MetaData(Compile::FileMetaClass* md);
        MetaData(const std::string& _name, bool constToken, bool staticToken, bool dynamic);

        // Methods
        void AddPingToken(Compile::Token* tok);
        MetaMemberData* GetMemberDataByName(const std::string& name);
        void AddMetaMemberData(MetaMemberData* mmd);
        std::vector<MetaMemberData*> GetMetaMemberDataList();
        virtual void ParseFileMetaDataMemeberData(Compile::FileMetaClass* fmc);
        void ParseDefineComplete() override;
        std::string ToFormatString() override;
        std::string ToString() override;

    protected:
        bool m_IsConst = false;
        bool m_IsStatic = false;
        bool m_IsDynamic = false;

        std::unordered_map<std::string, MetaMemberData*> m_MetaMemberDataDict;
        std::vector<Compile::Token*> m_PingTokensList;
    };

} // namespace Core
} // namespace SimpleLanguage