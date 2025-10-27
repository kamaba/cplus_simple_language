//****************************************************************************
//  File:      MetaEnum.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta enum class
//****************************************************************************

#include "MetaEnum.h"
#include "MetaNode.h"
#include "MetaVariable.h"
#include "MetaMemberEnum.h"
#include "MetaMemberVariable.h"
#include "MetaParam.h"
#include "MetaType.h"
#include "MetaExpressNode/MetaExpressBase.h"
#include "MetaExpressNode/MetaExpressConst.h"
#include "MetaExpressNode/MetaExpressNewObject.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "../Compile/FileMeta/FileMetaClass.h"
#include "../Compile/FileMeta/FileMetaMemberVariable.h"
#include "../Compile/Token.h"
#include "../Debug/Log.h"
#include <sstream>
#include <stdexcept>

using namespace SimpleLanguage::Debug;
using namespace SimpleLanguage::Compile;

namespace SimpleLanguage {
namespace Core {

MetaEnum::MetaEnum(const std::string& name) : MetaClass(name) {
    m_Type = EType::Enum;
}

MetaMemberVariable* MetaEnum::GetMemberVariableByName(const std::string& name) {
    auto it = m_MetaMemberVariableDict.find(name);
    if (it != m_MetaMemberVariableDict.end()) {
        return it->second;
    }
    return nullptr;
}

void MetaEnum::CreateValues() {
    if (m_MetaVariable == nullptr) {
        MetaInputTemplateCollection* mitc = new MetaInputTemplateCollection();
        mitc->AddMetaTemplateParamsList(new MetaType(GetExtendClass()));
        auto mt = new MetaType(CoreMetaClassManager::GetInstance().GetArrayMetaClass(), nullptr, mitc);
        m_MetaVariable = new MetaVariable("values", EVariableFrom::Static, nullptr, this, mt);
        m_MetaVariable->SetIsStatic(true);
        for (const auto& pair : m_MetaMemberEnumDict) {
            m_MetaVariable->AddMetaVariable(pair.second);
        }
    }
}

MetaMemberEnum* MetaEnum::GetMemberEnumByName(const std::string& name) {
    auto it = m_MetaMemberEnumDict.find(name);
    if (it != m_MetaMemberEnumDict.end()) {
        return it->second;
    }
    return nullptr;
}

void MetaEnum::AddMetaMemberEnum(MetaMemberEnum* mmd) {
    if (m_MetaMemberEnumDict.find(mmd->GetName()) != m_MetaMemberEnumDict.end()) {
        return;
    }
    m_MetaMemberEnumDict[mmd->GetName()] = mmd;
}

void MetaEnum::ParseFileMetaEnumMemeberEnum(Compile::FileMetaClass* fmc) {
    if (fmc->GetMemberFunctionList().size() > 0) {
        Log::AddInStructMeta(EError::None, "Error Enum中不允许有Function!!");
    }
    if (fmc->GetTemplateDefineList().size() > 0) {
        Log::AddInStructMeta(EError::None, "Error 在Enum定义中，不允许使用Template模板的形式!");
    }

    bool isHave = false;
    for (auto v : fmc->GetMemberVariableList()) {
        MetaBase* mb = GetMemberVariableByName(v->GetName());
        if (mb != nullptr) {
            //Log::AddInStructMeta(EError::None, "Error Enum MetaMemberData已有定义类: " + m_AllName + "中 已有: " + (v->GetToken() != nullptr ? v->GetToken()->ToLexemeAllString() : "") + "的元素!!");
            isHave = true;
        }
        else {
            isHave = false;
        }
        MetaMemberEnum* mmv = new MetaMemberEnum(this, v);
        if (isHave) {
            mmv->SetName(mmv->GetName() + "__repeat__");
        }
        AddMetaMemberEnum(mmv);
    }
}

void MetaEnum::HandleExtendMemberVariable() {
    if (m_ExtendClass == nullptr) {
        return;
    }

    if (m_ExtendClass == CoreMetaClassManager::GetInstance().GetByteMetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetSByteMetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetInt16MetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetUInt16MetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetInt32MetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetUInt32MetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetInt64MetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetUInt64MetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetStringMetaClass()) {
        auto mt = new MetaType(m_ExtendClass);
        for (const auto& pair : m_MetaMemberEnumDict) {
            pair.second->SetMetaDefineType(mt);
        }
    }
    else if (m_ExtendClass == CoreMetaClassManager::GetInstance().GetDynamicMetaData()) {
        // 仅限data数据类型
        auto mt = new MetaType(m_ExtendClass);
        for (const auto& pair : m_MetaMemberEnumDict) {
            pair.second->SetMetaDefineType(mt);
        }
    }
}

void MetaEnum::ParseDefineComplete() {
    MetaClass::ParseDefineComplete();
}

void MetaEnum::ParseMemberMetaEnumExpress() {
    if (m_MetaMemberEnumDict.size() == 0) {
        //Log::AddInStructMeta(EError::None, "Warning 在enum : " + GetName() + " 没有发现有任何成员");
        return;
    }

    if (m_ExtendClass == CoreMetaClassManager::GetInstance().GetByteMetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetSByteMetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetInt16MetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetUInt16MetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetInt32MetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetUInt32MetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetInt64MetaClass() ||
        m_ExtendClass == CoreMetaClassManager::GetInstance().GetUInt64MetaClass()) {

        int i = 0;
        long long indexdynamic = 0;
        for (const auto& pair : m_MetaMemberEnumDict) {
            pair.second->ParseDefineMetaType();

            if (i++ == 0) {
                if (pair.second->GetExpress() == nullptr) {
                    //Log::AddInStructMeta(EError::None, "Warning Enum Member Enum 成员第一位必须有=号");
                    continue;
                }
            }
            if (pair.second->GetExpress() != nullptr) {
                if (pair.second->GetConstExpressNode() == nullptr) {
                    //Log::AddInStructMeta(EError::None, "Error Enum Member Enum 内允许使用const值类变量");
                    continue;
                }
                else if (m_ExtendClass == CoreMetaClassManager::GetInstance().GetByteMetaClass()) {
                    try {
                        indexdynamic = static_cast<long long>(std::stoi(pair.second->GetConstExpressNode()->GetValue().data.string_val ));
                    }
                    catch (const std::exception& ex) {
                        //Log::AddInStructMeta(EError::None, "Error Enum Member Enum 内部int转byte出错");
                        continue;
                    }
                }
                else if (m_ExtendClass == CoreMetaClassManager::GetInstance().GetSByteMetaClass()) {
                    try {
                        indexdynamic = static_cast<signed char>(std::stoi(pair.second->GetConstExpressNode()->GetValue().data.string_val ));
                    }
                    catch (const std::exception& ex) {
                        //Log::AddInStructMeta(EError::None, "Error Enum Member Enum 内部int转byte出错");
                        continue;
                    }
                }
                else if (m_ExtendClass == CoreMetaClassManager::GetInstance().GetUInt16MetaClass()) {
                    try {
                        indexdynamic = static_cast<unsigned short>(std::stoi(pair.second->GetConstExpressNode()->GetValue().data.string_val ));
                    }
                    catch (const std::exception& ex) {
                        //Log::AddInStructMeta(EError::None, "Error Enum Member Enum 内部int转byte出错");
                        continue;
                    }
                }
                else if (m_ExtendClass == CoreMetaClassManager::GetInstance().GetInt16MetaClass()) {
                    try {
                        indexdynamic = static_cast<short>(std::stoi(pair.second->GetConstExpressNode()->GetValue().data.string_val ));
                    }
                    catch (const std::exception& ex) {
                        //Log::AddInStructMeta(EError::None, "Error Enum Member Enum 内部int转byte出错");
                        continue;
                    }
                }
                else if (m_ExtendClass == CoreMetaClassManager::GetInstance().GetInt32MetaClass()) {
                    try {
                        indexdynamic = std::stoi(pair.second->GetConstExpressNode()->GetValue().data.string_val);
                    }
                    catch (const std::exception& ex) {
                        //Log::AddInStructMeta(EError::None, "Error Enum Member Enum 内部int转byte出错");
                        continue;
                    }
                }
                else if (m_ExtendClass == CoreMetaClassManager::GetInstance().GetUInt32MetaClass()) {
                    try {
                        indexdynamic = static_cast<unsigned int>(std::stoi(pair.second->GetConstExpressNode()->GetValue().data.string_val ));
                    }
                    catch (const std::exception& ex) {
                        //Log::AddInStructMeta(EError::None, "Error Enum Member Enum 内部int转byte出错");
                        continue;
                    }
                }
                else if (m_ExtendClass == CoreMetaClassManager::GetInstance().GetInt64MetaClass()) {
                    indexdynamic = std::stoll(pair.second->GetConstExpressNode()->GetValue().data.string_val );
                }
                else if (m_ExtendClass == CoreMetaClassManager::GetInstance().GetUInt64MetaClass()) {
                    indexdynamic = static_cast<unsigned long long>(std::stoull(pair.second->GetConstExpressNode()->GetValue().data.string_val ));
                }
            }
            else {
                // 创建常量表达式节点
                MultiData value(indexdynamic++);
                pair.second->SetExpress(new MetaConstExpressNode(m_ExtendClass->GetEType(), value));
            }
        }
    }
    else if (m_ExtendClass == CoreMetaClassManager::GetInstance().GetStringMetaClass()) {
        for (const auto& pair : m_MetaMemberEnumDict) {
            pair.second->ParseDefineMetaType();
            if (pair.second->GetExpress() == nullptr) {
                //Log::AddInStructMeta(EError::None, "Error Enum Member Enum String成员必须有=号" + pair.first);
                continue;
            }
            if (pair.second->GetConstExpressNode() == nullptr) {
                //Log::AddInStructMeta(EError::None, "Error Enum Member Enum 内允许使用const值类变量");
                continue;
            }
            if (pair.second->GetConstExpressNode()->GetEType() != EType::String) {
                //Log::AddInStructMeta(EError::None, "Error Enum Member Enum 内允许使用string值类变量");
                continue;
            }
        }
    }
    else if (m_ExtendClass == CoreMetaClassManager::GetInstance().GetDynamicMetaData()) {
        for (const auto& pair : m_MetaMemberEnumDict) {
            pair.second->ParseDefineMetaType();
            if (pair.second->GetExpress() == nullptr) {
                //Log::AddInStructMeta(EError::None, "Error Enum Member Enum 动态成员第一位必须有=号");
                continue;
            }
            pair.second->ParseMetaExpress();
            if (auto mnoe = dynamic_cast<MetaNewObjectExpressNode*>(pair.second->GetExpress())) {
                if (mnoe->GetReturnMetaDefineType()->IsData()) {
                    // 处理数据类型的逻辑
                }
                else {
                    //Log::AddInStructMeta(EError::None, "Error Enum Member Enum 内允许使用data值类变量, 不允许其它类型");
                }
            }
            else {
                //Log::AddInStructMeta(EError::None, "Error Enum Member Enum 内允许使用data new 值类变量");
            }
        }
    }
    else {
        for (const auto& pair : m_MetaMemberEnumDict) {
            pair.second->ParseDefineMetaType();
            if (pair.second->GetExpress() == nullptr) {
                //Log::AddInStructMeta(EError::None, "Error Enum Member Enum 成员第一位必须有=号");
                continue;
            }
            pair.second->ParseMetaExpress();

            if (auto mnoe = dynamic_cast<MetaNewObjectExpressNode*>(pair.second->GetExpress())) {
                if (mnoe->GetReturnMetaDefineType()->IsData()) {
                    // 处理数据类型的逻辑
                }
                else {
                    Log::AddInStructMeta(EError::None, "Error Enum Member Enum 内允许使用data值类变量, 不允许其它类型");
                }
            }
            else if (pair.second->GetConstExpressNode() != nullptr) {
                // 处理常量表达式节点
            }
            else {
                Log::AddInStructMeta(EError::None, "Error Enum Member Enum 内允许使用data new 值类变量");
            }
        }
    }
}

std::string MetaEnum::ToFormatString() const {
    std::stringstream stringBuilder;
    stringBuilder.clear();
    for (int i = 0; i < GetRealDeep(); i++)
        stringBuilder << "\t";
    stringBuilder << (int)GetPermission();
    stringBuilder << " ";
    stringBuilder << "enum ";
    stringBuilder << GetName();

    stringBuilder << "\n";
    for (int i = 0; i < GetRealDeep(); i++)
        stringBuilder << "\t";
    stringBuilder << "{\n";

    for (const auto& pair : m_MetaMemberEnumDict) {
        MetaMemberEnum* mmv = pair.second;
        if (mmv->GetFromType() == EFromType::Code) {
            stringBuilder << mmv->ToFormatString();
            stringBuilder << "\n";
        }
        else {
            stringBuilder << "Errrrrroooorrr ---" + mmv->ToFormatString();
            stringBuilder << "\n";
        }
    }

    for (int i = 0; i < GetRealDeep(); i++)
        stringBuilder << "\t";
    stringBuilder << "}\n";

    return stringBuilder.str();
    }

} // namespace Core
} // namespace SimpleLanguage