//****************************************************************************
//  File:      MetaMemberData.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2025/1/17 12:00:00
//  Description: class's memeber variable metadata and member 'data' metadata implementation
//****************************************************************************

#include "MetaMemberData.h"
#include "MetaVariable.h"
#include "MetaExpressNode.h"
#include "MetaType.h"
#include "MetaData.h"
#include "FileMetaMemberData.h"
#include "FileMetaOpAssignSyntax.h"
#include "MetaConstExpressNode.h"
#include "MetaCallLinkExpressNode.h"
#include "MetaNewObjectExpressNode.h"
#include "MetaVariableManager.h"
#include "Log.h"
#include "CoreMetaClassManager.h"
#include "ExpressManager.h"
#include "AllowUseSettings.h"
#include "CreateExpressParam.h"
#include "Global.h"
#include <unordered_map>
#include <string>
#include <sstream>

namespace SimpleLanguage {
namespace Core {

MetaMemberData::MetaMemberData(MetaData* mc, FileMetaOpAssignSyntax* fmoa) {
    m_FileMetaOpAssignSyntax = fmoa;
    m_DefineMetaType = new MetaType(mc);
    SetOwnerMetaClass(mc);
    m_IsConst = mc->IsConst();
    ParseName();
}

MetaMemberData::MetaMemberData(MetaData* mc, FileMetaMemberData* fmmd, int index, bool isStatic) {
    m_FileMetaMemeberData = fmmd;
    fmmd->SetMetaMemberData(this);
    m_Name = fmmd->Name();
    m_Index = index;
    m_IsStatic = isStatic;
    m_IsWithName = m_FileMetaMemeberData->IsWithName();
    m_DefineMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    SetOwnerMetaClass(mc);
    m_IsConst = mc->IsConst();
}

MetaMemberData::MetaMemberData(MetaMemberData* parentNode, FileMetaMemberData* fmmd, int index, bool isEnd) {
    m_Index = index;
    m_End = isEnd;
    m_FileMetaMemeberData = fmmd;
    fmmd->SetMetaMemberData(this);
    m_DefineMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    SetOwnerMetaClass(parentNode->GetOwnerMetaClass());
    m_IsConst = parentNode->IsConst();

    ParseName();
}

MetaMemberData::MetaMemberData(MetaMemberData* parentNode, const std::string& name, int index, MetaExpressNode* men) {
    m_Name = name;
    m_Index = index;
    SetOwnerMetaClass(parentNode->GetOwnerMetaClass());
    m_IsConst = parentNode->IsConst();
    m_Express = men;
}

void MetaMemberData::SetIndex(int index) {
    m_Index = index;
}

std::string MetaMemberData::GetString(const std::string& name, bool isInChildren) {
    auto constExpress = dynamic_cast<MetaConstExpressNode*>(m_Express);
    if (constExpress != nullptr) {
        return constExpress->GetValue().ToString();
    } else {
        if (isInChildren) {
            auto it = m_MetaMemberDataDict.find(name);
            if (it != m_MetaMemberDataDict.end()) {
                return it->second->GetString(name);
            }
        }
    }
    return "";
}

int MetaMemberData::GetInt(const std::string& name, int defaultValue) {
    auto constExpress = dynamic_cast<MetaConstExpressNode*>(m_Express);
    if (constExpress != nullptr) {
        if (constExpress->GetEType() == EType::Int16 ||
            constExpress->GetEType() == EType::UInt16 ||
            constExpress->GetEType() == EType::Int32 ||
            constExpress->GetEType() == EType::UInt32 ||
            constExpress->GetEType() == EType::Int64 ||
            constExpress->GetEType() == EType::UInt64) {
            return std::stoi(constExpress->GetValue().ToString());
        }
    }
    return defaultValue;
}

MetaMemberData* MetaMemberData::GetMemberDataByName(const std::string& name) {
    auto it = m_MetaMemberDataDict.find(name);
    if (it != m_MetaMemberDataDict.end()) {
        return it->second;
    }
    return nullptr;
}

bool MetaMemberData::AddMetaMemberData(MetaMemberData* mmd) {
    if (m_MetaMemberDataDict.find(mmd->GetName()) != m_MetaMemberDataDict.end()) {
        return false;
    }
    m_MetaMemberDataDict[mmd->GetName()] = mmd;

    MetaVariableManager::Instance()->AddMetaDataVariable(mmd);

    return true;
}

void MetaMemberData::ParseName() {
    if (m_FileMetaMemeberData != nullptr) {
        m_IsWithName = m_FileMetaMemeberData->IsWithName();
        if (m_IsWithName) {
            m_Name = m_FileMetaMemeberData->Name();
        } else {
            m_Name = std::to_string(m_Index);
        }
    } else if (m_FileMetaOpAssignSyntax != nullptr) {
        m_Name = m_FileMetaOpAssignSyntax->GetVariableRef()->GetName();
    }
}

void MetaMemberData::ParseDefineMetaType() {
    if (m_FileMetaMemeberData != nullptr) {
        switch (m_FileMetaMemeberData->GetDataType()) {
            case FileMetaMemberData::EMemberDataType::Data: {
                m_MemberDataType = EMemberDataType::MemberData;
            }
            break;
            case FileMetaMemberData::EMemberDataType::Class: {
                m_MemberDataType = EMemberDataType::MemberClass;
                m_Express = new MetaCallLinkExpressNode(m_FileMetaMemeberData->GetFileMetaCallTermValue()->GetCallLink(), nullptr, nullptr, nullptr);
            }
            break;
            case FileMetaMemberData::EMemberDataType::Array: {
                m_MemberDataType = EMemberDataType::MemberArray;
            }
            break;
            case FileMetaMemberData::EMemberDataType::ConstValue: {
                m_MemberDataType = EMemberDataType::ConstValue;
                if (m_FileMetaMemeberData->GetFileMetaConstValue() != nullptr) {
                    m_Express = new MetaConstExpressNode(m_FileMetaMemeberData->GetFileMetaConstValue());
                }
            }
            break;
        }
    } else if (m_FileMetaOpAssignSyntax != nullptr) {
        if (m_FileMetaOpAssignSyntax->GetVariableRef() != nullptr) {
            if (m_FileMetaOpAssignSyntax->GetVariableRef()->IsOnlyName()) {
                m_DefineMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
            }

            FileMetaBaseTerm* curFMBT = m_FileMetaOpAssignSyntax->GetExpress();
            auto fme = m_FileMetaOpAssignSyntax->GetExpress();

            CreateExpressParam cep;
            cep.fme = curFMBT;
            cep.metaType = m_DefineMetaType;
            cep.equalMetaVariable = this;
            cep.ownerMBS = m_OwnerMetaBlockStatements;
            cep.parsefrom = EParseFrom::StatementRightExpress;
            m_Express = ExpressManager::CreateExpressNode(cep);
            if (m_Express == nullptr) {
                Log::AddInStructMeta(EError::None, "Error 没有解析到Express的内容 在MetaMemberData 里边 372");
            }
        }
    }
}

bool MetaMemberData::ParseMetaExpress() {
    if (m_Express != nullptr) {
        AllowUseSettings auc;
        auc.parseFrom = EParseFrom::MemberVariableExpress;
        m_Express->Parse(auc);
        m_Express->CalcReturnType();
        m_DefineMetaType = m_Express->GetReturnMetaDefineType();
        if (m_DefineMetaType == nullptr) {
            Log::AddInStructMeta(EError::None, "Error 在生成Data时，没有找到." + m_FileMetaMemeberData->GetFileMetaCallTermValue()->ToTokenString());
            return false;
        }
        if (m_DefineMetaType->IsData()) {
            m_MemberDataType = EMemberDataType::MemberData;
        } else if (m_DefineMetaType->IsArray()) {
            m_MemberDataType = EMemberDataType::MemberArray;
        } else if (dynamic_cast<MetaConstExpressNode*>(m_Express) != nullptr) {
            m_MemberDataType = EMemberDataType::ConstValue;
        } else {
            m_MemberDataType = EMemberDataType::MemberClass;

            auto mcen = dynamic_cast<MetaCallLinkExpressNode*>(m_Express);
            if (mcen != nullptr) {
                m_MetaVariable = mcen->GetMetaVariable();
            }
        }
    }
    return true;
}

MetaMemberData* MetaMemberData::Copy() {
    return nullptr;
}

void MetaMemberData::CopyByMetaData(MetaData* md) {
    MetaData* curMD = dynamic_cast<MetaData*>(m_OwnerMetaClass);
    for (const auto& v : md->GetMetaMemberDataDict()) {
        if (v.second->IsIncludeMetaData(curMD)) {
            Log::AddInStructMeta(EError::None, "Error 当前有循环引用数量现象，请查正!!" + md->GetAllClassName());
            continue;
        }
        auto newMMD = v.second->Copy();
        this->AddMetaMemberData(newMMD);
    }
}

bool MetaMemberData::IsIncludeMetaData(MetaData* md) {
    if (md == nullptr) return false;

    MetaData* belongMD = dynamic_cast<MetaData*>(m_OwnerMetaClass);
    if (belongMD != nullptr) {
        if (belongMD == md) {
            return true;
        }
    }

    return false;
}

void MetaMemberData::ParseChildMemberData() {
    if (m_FileMetaMemeberData != nullptr) {
        int count = m_FileMetaMemeberData->GetFileMetaMemberData().size();
        for (int i = 0; i < count; i++) {
            MetaMemberData* mmd = new MetaMemberData(this, m_FileMetaMemeberData->GetFileMetaMemberData()[i], i, i == count - 1);

            mmd->ParseName();
            mmd->ParseDefineMetaType();
            mmd->ParseMetaExpress();

            if (AddMetaMemberData(mmd)) {
                mmd->ParseChildMemberData();
            } else {
                Log::AddInStructMeta(EError::None, "Error ParseChildMemberData 命名有重名!!" + mmd->GetName());
            }
        }
    } else if (m_Express != nullptr) {
        auto mne = dynamic_cast<MetaNewObjectExpressNode*>(m_Express);
        auto cne = dynamic_cast<MetaCallLinkExpressNode*>(m_Express);
        if (mne != nullptr) {
            for (int i = 0; i < mne->GetMetaBraceOrBracketStatementsContent()->GetAssignStatementsList().size(); i++) {
                auto asl = mne->GetMetaBraceOrBracketStatementsContent()->GetAssignStatementsList()[i];

                if (asl == nullptr) continue;

                MetaMemberData* addMmd = nullptr;
                if (m_MemberDataType == EMemberDataType::MemberArray) {
                    auto mcen = dynamic_cast<MetaConstExpressNode*>(asl->GetExpressNode());
                    auto mnoe = dynamic_cast<MetaNewObjectExpressNode*>(asl->GetExpressNode());
                    if (mcen != nullptr) {
                        addMmd = new MetaMemberData(this, std::to_string(i), i, mcen);
                        addMmd->ParseMetaExpress();
                    }
                    if (mnoe != nullptr) {
                        addMmd = new MetaMemberData(this, std::to_string(i), i, mnoe);
                        addMmd->ParseMetaExpress();
                    }
                } else if (m_MemberDataType == EMemberDataType::MemberData) {
                    addMmd = asl->GetMetaMemberData();
                }

                if (addMmd == nullptr) continue;

                if (m_MetaMemberDataDict.find(addMmd->GetName()) != m_MetaMemberDataDict.end()) {
                    Log::AddInStructMeta(EError::None, "Error 重复的MetaMemberData的名称 在484");
                    continue;
                }
                m_MetaMemberDataDict[addMmd->GetName()] = addMmd;
            }
        } else if (cne != nullptr) {
            MetaMemberData* addMmd = new MetaMemberData(this, m_Name, 0, cne);
            if (m_MetaMemberDataDict.find(addMmd->GetName()) != m_MetaMemberDataDict.end()) {
                Log::AddInStructMeta(EError::None, "Error 重复的MetaMemberData的名称 在410");
            }
            m_MetaMemberDataDict[addMmd->GetName()] = addMmd;
        }
    }
}

std::string MetaMemberData::ToFormatString2(bool isDynamic) {
    std::stringstream sb;
    switch (this->m_MemberDataType) {
        case EMemberDataType::MemberData: {
            if (isDynamic) {
                sb << m_Name;
                sb << "{";
                for (const auto& v : m_MetaMemberDataDict) {
                    sb << v.second->ToFormatString2(isDynamic);
                }
                sb << "}";
            } else {
                for (int i = 0; i < realDeep; i++)
                    sb << Global::GetTabChar();
                sb << m_Name << std::endl;
                for (int i = 0; i < realDeep; i++)
                    sb << Global::GetTabChar();
                sb << "{" << std::endl;
                for (const auto& v : m_MetaMemberDataDict) {
                    sb << v.second->ToFormatString() << std::endl;
                }
                for (int i = 0; i < realDeep; i++)
                    sb << Global::GetTabChar();
                sb << "}";
            }
        }
        break;
        case EMemberDataType::MemberClass: {
            if (isDynamic) {
                sb << m_Name;
                sb << " = ";
                sb << m_Express->ToFormatString();
            } else {
                for (int i = 0; i < realDeep; i++)
                    sb << Global::GetTabChar();
                sb << m_Name + " = ";
                sb << m_Express->ToFormatString();
                for (int i = 0; i < realDeep; i++)
                    sb << Global::GetTabChar();
            }
        }
        break;
        case EMemberDataType::MemberArray: {
            if (isDynamic) {
                sb << m_Name;
                sb << "[";
                for (const auto& v : m_MetaMemberDataDict) {
                    sb << v.second->ToFormatString2(isDynamic);
                }
                sb << "]";
            } else {
                int i = 0;
                for (i = 0; i < realDeep; i++)
                    sb << Global::GetTabChar();
                sb << m_Name + " = [";
                i = 0;
                for (const auto& v : m_MetaMemberDataDict) {
                    sb << v.second->ToFormatString();
                    if (i < m_MetaMemberDataDict.size() - 1)
                        sb << ",";
                    i++;
                }
                sb << "]";
            }
        }
        break;
        case EMemberDataType::ConstValue: {
            if (isDynamic) {
                sb << m_Express->ToFormatString();
            } else {
                for (int i = 0; i < realDeep; i++)
                    sb << Global::GetTabChar();
                sb << m_Express->ToFormatString();
            }
        }
        break;
        default: {
            Log::AddInStructMeta(EError::None, "error 暂不支持其它类型 1");
        }
        break;
    }
    return sb.str();
}

std::string MetaMemberData::ToString() {
    std::stringstream sb;
    if (m_IsWithName) {
        sb << m_Name;
        sb << " = ";
    }
    switch (m_MemberDataType) {
        case EMemberDataType::MemberData: {
            sb << "{" << std::endl;
            for (const auto& v : m_MetaMemberDataDict) {
                sb << v.second->ToFormatString() << std::endl;
            }
            sb << "}" << std::endl;
        }
        break;
        case EMemberDataType::MemberClass: {
            sb << m_Express->ToFormatString();
        }
        break;
        case EMemberDataType::MemberArray: {
            int i = 0;
            sb << "[";
            i = 0;
            for (const auto& v : m_MetaMemberDataDict) {
                sb << v.second->ToFormatString();
                if (i < m_MetaMemberDataDict.size() - 1)
                    sb << ",";
                i++;
            }
            sb << "]";
        }
        break;
        case EMemberDataType::ConstValue: {
            sb << m_Express->ToFormatString();
        }
        break;
        default: {
            Log::AddInStructMeta(EError::None, "error 暂不支持其它类型 1");
        }
        break;
    }
    return sb.str();
}

} // namespace Core
} // namespace SimpleLanguage
