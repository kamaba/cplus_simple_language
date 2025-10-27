//****************************************************************************
//  File:      MetaData.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta data class
//****************************************************************************

#include "MetaData.h"
#include "MetaMemberData.h"
#include "MetaNode.h"
#include "../Compile/FileMeta/FileMetaClass.h"
#include "../Compile/FileMeta/FileMetaCommon.h"
#include "../Compile/FileMeta/FileMetaMemberData.h"
#include "../Compile/Token.h"
#include "../Debug/Log.h"
#include <sstream>

using namespace SimpleLanguage::Debug;
using namespace SimpleLanguage::Compile;

namespace SimpleLanguage {
namespace Core {

    MetaData::MetaData(Compile::FileMetaClass* md) {
        m_Name = md->GetName();
        m_AllName = md->GetName();
        m_Type = EType::Data;
        m_IsConst = md->IsConst();
        m_IsStatic = md->IsStatic();
        m_IsDynamic = false;
    }

    MetaData::MetaData(const std::string& _name, bool constToken, bool staticToken, bool dynamic) 
        : MetaClass(_name) {
        m_Name = _name;
        m_AllName = _name;
        m_Type = EType::Data;
        m_IsConst = constToken;
        m_IsStatic = staticToken;
        m_IsDynamic = dynamic;
    }

    void MetaData::AddPingToken(Compile::Token* tok) {
        m_PingTokensList.push_back(tok);
    }

    MetaMemberData* MetaData::GetMemberDataByName(const std::string& name) {
        auto it = m_MetaMemberDataDict.find(name);
        if (it != m_MetaMemberDataDict.end()) {
            return it->second;
        }
        return nullptr;
    }

    void MetaData::AddMetaMemberData(MetaMemberData* mmd) {
        if (m_MetaMemberDataDict.find(mmd->GetName()) != m_MetaMemberDataDict.end()) {
            return;
        }
        m_MetaMemberDataDict[mmd->GetName()] = mmd;
    }

    std::vector<MetaMemberData*> MetaData::GetMetaMemberDataList() {
        std::vector<MetaMemberData*> list;
        for (const auto& pair : m_MetaMemberDataDict) {
            list.push_back(pair.second);
        }
        return list;
    }

    void MetaData::ParseFileMetaDataMemeberData(Compile::FileMetaClass* fmc) {
        bool isHave = false;
        for (size_t i = 0; i < fmc->GetMemberDataList().size(); i++) {
            auto v = fmc->GetMemberDataList()[i];
            MetaNode* mb = m_MetaNode->GetChildrenMetaNodeByName(v->GetName());
            if (mb != nullptr) {
                Debug::Log::AddInStructMeta(EError::None, "Error MetaData MetaDataMember已有定义类: " + m_AllName + "中 已有: " + (v->GetToken() != nullptr ? v->GetToken()->ToLexemeAllString() : "null") + "的元素!!");
                isHave = true;
            } else {
                isHave = false;
            }
            MetaMemberData* mmv = new MetaMemberData(this, v, static_cast<int>(i), IsStatic());
            if (isHave) {
                mmv->SetName(mmv->GetName() + "__repeat__");
            }
            mmv->ParseDefineMetaType();
            AddMetaMemberData(mmv);

            mmv->ParseChildMemberData();
        }

        if (fmc->GetMemberVariableList().size() > 0 || fmc->GetMemberFunctionList().size() > 0) {
            Debug::Log::AddInStructMeta(EError::None, "Error Data中不允许有Variable 和 Function!!");
        }
    }

    void MetaData::ParseDefineComplete() {
        MetaClass::ParseDefineComplete();
    }

    std::string MetaData::ToFormatString() const {
        std::stringstream stringBuilder;

        if (m_IsDynamic) {
            if (IsConst()) {
                stringBuilder << "const ";
            }
            stringBuilder << "}";
        } else {
            stringBuilder << "}" << std::endl;
        }

        return stringBuilder.str();
    }

    std::string MetaData::ToString() const {
        return MetaClass::ToString();
    }

} // namespace Core
} // namespace SimpleLanguage