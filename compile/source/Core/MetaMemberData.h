//****************************************************************************
//  File:      MetaMemberData.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2025/1/17 12:00:00
//  Description: class's memeber variable metadata and member 'data' metadata
//****************************************************************************

#pragma once

#include "MetaVariable.h"

#include <unordered_map>
#include <string>
#include <sstream>

namespace SimpleLanguage {
    namespace Compile
    {
        class FileMetaOpAssignSyntax;
        class FileMetaMemberData;
    }
namespace Core {

enum class EMemberDataType {
    None,
    ConstValue,
    MemberData,
    MemberArray,
    MemberClass
};


class MetaData;
class MetaVariable;
class MetaExpressNode;

class MetaMemberData : public MetaVariable {
public:
    // Properties
    bool IsConst() const override { return m_IsConst; }
    EMemberDataType GetMemberDataType() const { return m_MemberDataType; }
    MetaVariable* GetMetaVariable() const { return m_MetaVariable; }
    MetaExpressNode* GetExpressNode() const { return m_Express; }
    const std::unordered_map<std::string, MetaMemberData*>& GetMetaMemberDataDict() const { return m_MetaMemberDataDict; }

    // Constructors
    MetaMemberData(MetaData* mc, Compile::FileMetaOpAssignSyntax* fmoa);
    MetaMemberData(MetaData* mc, Compile::FileMetaMemberData* fmmd, int index, bool isStatic);
    MetaMemberData(MetaMemberData* parentNode, Compile::FileMetaMemberData* fmmd, int index, bool isEnd = false);
    MetaMemberData(MetaMemberData* parentNode, const std::string& name, int index, MetaExpressNode* men);

    // Methods
    void SetIndex(int index);
    std::string GetString(const std::string& name, bool isInChildren = true);
    int GetInt(const std::string& name, int defaultValue = 0);
    MetaMemberData* GetMemberDataByName(const std::string& name);
    bool AddMetaMemberData(MetaMemberData* mmd);
    void ParseDefineMetaType() override;
    bool ParseMetaExpress() override;
    MetaMemberData* Copy();
    void CopyByMetaData(MetaData* md);
    bool IsIncludeMetaData(MetaData* md);
    void ParseChildMemberData();
    std::string ToFormatString2(bool isDynamic);
    std::string ToString() const override;

private:
    void ParseName();

    MetaExpressNode* m_Express = nullptr;
    MetaVariable* m_MetaVariable = nullptr;
    EMemberDataType m_MemberDataType = EMemberDataType::None;
    int m_Index = -1;
    bool m_End = false;
    bool m_IsWithName = false;

    std::unordered_map<std::string, MetaMemberData*> m_MetaMemberDataDict;

    Compile::FileMetaMemberData* m_FileMetaMemeberData = nullptr;
    Compile::FileMetaOpAssignSyntax* m_FileMetaOpAssignSyntax = nullptr;
};

} // namespace Core
} // namespace SimpleLanguage
