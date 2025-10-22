//****************************************************************************
//  File:      MetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta class's attribute
//****************************************************************************

#include "MetaClass.h"
#include "MetaNode.h"
#include "MetaType.h"
#include "MetaMemberVariable.h"
#include "MetaMemberFunction.h"
#include "MetaMemberFunctionTemplateNode.h"
#include "MetaExpressNode.h"
#include "MetaTemplate.h"
#include "../Debug/Log.h"
#include "../Compile/CoreFileMeta/FileMetaClass.h"
#include <algorithm>
#include <sstream>

namespace SimpleLanguage {
namespace Core {

MetaClass::MetaClass() : MetaBase() {
    m_Type = EType::Class;
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

MetaClass::MetaClass(const std::string& name, EClassDefineType ecdt) : MetaBase() {
    m_Name = name;
    m_Type = EType::Class;
    m_ClassDefineType = ecdt;
}

MetaClass::MetaClass(const std::string& name, EType type) : MetaBase() {
    m_Name = name;
    m_Type = type;
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

MetaClass::MetaClass(const MetaClass& mc) : MetaBase(mc) {
    m_ExtendLevel = mc.m_ExtendLevel;
    m_Type = mc.m_Type;
    m_FileMetaClassDict = mc.m_FileMetaClassDict;
    m_ExtendClass = mc.m_ExtendClass;
    m_ExtendClassMetaType = mc.m_ExtendClassMetaType;
    m_BindStructTemplateMetaClassList = mc.m_BindStructTemplateMetaClassList;
    m_InterfaceClass = mc.m_InterfaceClass;
    m_InterfaceMetaType = mc.m_InterfaceMetaType;
    m_MetaMemberVariableDict = mc.m_MetaMemberVariableDict;
    m_FileCollectMetaMemberVariable = mc.m_FileCollectMetaMemberVariable;
    m_MetaExtendMemeberVariableDict = mc.m_MetaExtendMemeberVariableDict;
    m_MetaMemberFunctionTemplateNodeDict = mc.m_MetaMemberFunctionTemplateNodeDict;
    m_FileCollectMetaMemberFunctionList = mc.m_FileCollectMetaMemberFunctionList;
    m_NonStaticVirtualMetaMemberFunctionList = mc.m_NonStaticVirtualMetaMemberFunctionList;
    m_StaticMetaMemberFunctionList = mc.m_StaticMetaMemberFunctionList;
    m_TempInnerFunctionList = mc.m_TempInnerFunctionList;
    m_DefaultExpressNode = mc.m_DefaultExpressNode;
    m_ClassDefineType = mc.m_ClassDefineType;
    m_IsInterfaceClass = mc.m_IsInterfaceClass;
    m_IsHandleExtendVariableDirty = mc.m_IsHandleExtendVariableDirty;
    m_MetaTemplateList = mc.m_MetaTemplateList;
    m_IsTemplateClass = mc.m_IsTemplateClass;
    m_IsGenTemplate = mc.m_IsGenTemplate;
}

const std::unordered_map<std::string, MetaMemberVariable*>& MetaClass::GetAllMetaMemberVariableDict() const {
    return m_MetaMemberVariableDict;
}

std::vector<MetaMemberVariable*> MetaClass::GetAllMetaMemberVariableList() const {
    std::vector<MetaMemberVariable*> result;
    for (const auto& pair : m_MetaMemberVariableDict) {
        result.push_back(pair.second);
    }
    return result;
}

void MetaClass::Parse() {
    // 解析类的逻辑
}

void MetaClass::ParseInnerVariable() {
    // 解析内部变量的逻辑
}

void MetaClass::ParseInnerFunction() {
    // 解析内部函数的逻辑
}

void MetaClass::ParseInner() {
    ParseInnerVariable();
    ParseInnerFunction();
}

void MetaClass::ParseExtendsRelation() {
    // 解析继承关系的逻辑
}

void MetaClass::ParseInterfaceRelation() {
    // 解析接口关系的逻辑
}

void MetaClass::HandleExtendMemberVariable() {
    // 处理扩展成员变量的逻辑
}

void MetaClass::HandleExtendMemberFunction() {
    // 处理扩展成员函数的逻辑
}

void MetaClass::ParseMemberVariableDefineMetaType() {
    // 解析成员变量定义元类型的逻辑
}

void MetaClass::ParseMemberFunctionDefineMetaType() {
    // 解析成员函数定义元类型的逻辑
}

bool MetaClass::CheckInterface() {
    // 检查接口的逻辑
    return true;
}

void MetaClass::ParseDefineComplete() {
    // 解析定义完成的逻辑
}

MetaMemberVariable* MetaClass::GetMetaMemberVariableByName(const std::string& name) {
    auto it = m_MetaMemberVariableDict.find(name);
    if (it != m_MetaMemberVariableDict.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<MetaMemberVariable*> MetaClass::GetMetaMemberVariableListByFlag(bool isStatic) {
    std::vector<MetaMemberVariable*> result;
    for (const auto& pair : m_MetaMemberVariableDict) {
        if (pair.second->IsStatic() == isStatic) {
            result.push_back(pair.second);
        }
    }
    return result;
}

MetaMemberFunction* MetaClass::GetMetaDefineGetSetMemberFunctionByName(const std::string& name, void* inputParam, bool isGet, bool isSet) {
    // 获取 Get/Set 成员函数的逻辑
    return nullptr;
}

MetaMemberFunction* MetaClass::GetMetaMemberFunctionByNameAndInputTemplateInputParamCount(const std::string& name, int templateParamCount, void* inputParam, bool isIncludeExtendClass) {
    // 根据名称和模板参数数量获取成员函数的逻辑
    return nullptr;
}

MetaMemberFunction* MetaClass::GetMetaMemberFunctionByNameAndInputTemplateInputParam(const std::string& name, const std::vector<MetaClass*>& mtList, void* inputParam, bool isIncludeExtendClass) {
    // 根据名称和模板参数获取成员函数的逻辑
    return nullptr;
}

MetaMemberFunction* MetaClass::GetMetaMemberConstructDefaultFunction() {
    // 获取默认构造函数的逻辑
    return nullptr;
}

MetaMemberFunction* MetaClass::GetMetaMemberConstructFunction(void* mmpc) {
    // 获取构造函数的逻辑
    return nullptr;
}

MetaMemberFunction* MetaClass::GetFirstMetaMemberFunctionByName(const std::string& name) {
    // 获取第一个指定名称的成员函数的逻辑
    return nullptr;
}

std::vector<MetaMemberFunction*> MetaClass::GetMemberInterfaceFunction() {
    // 获取成员接口函数的逻辑
    return std::vector<MetaMemberFunction*>();
}

bool MetaClass::GetMemberInterfaceFunctionByFunc(MetaMemberFunction* func) {
    // 根据函数获取成员接口函数的逻辑
    return false;
}

std::string MetaClass::ToDefineTypeString() {
    // 转换为定义类型字符串的逻辑
    return m_Name;
}

std::string MetaClass::GetFormatString(bool isShowNamespace) const {
    std::stringstream ss;
    if (isShowNamespace) {
        ss << m_AllName;
    } else {
        ss << m_Name;
    }
    return ss.str();
}

void MetaClass::UpdateClassAllName() {
    // 更新类全名的逻辑
    m_AllName = m_Name;
}

void MetaClass::CalcExtendLevel() {
    if (m_ExtendClass == nullptr) {
        m_ExtendLevel = 0;
    } else {
        MetaClass* mc = m_ExtendClass;
        int level = 0;
        while (mc != nullptr) {
            level++;
            mc = mc->m_ExtendClass;
        }
        m_ExtendLevel = level;
    }
}

bool MetaClass::IsParseMetaClass(MetaClass* parentClass, bool isIncludeSelf) {
    MetaClass* mc = isIncludeSelf ? this : m_ExtendClass;
    while (mc != nullptr) {
        if (mc == parentClass) {
            return true;
        }
        mc = mc->m_ExtendClass;
    }
    return false;
}

void MetaClass::AddInterfaceClass(MetaClass* aic) {
    auto it = std::find(m_InterfaceClass.begin(), m_InterfaceClass.end(), aic);
    if (it == m_InterfaceClass.end()) {
        m_InterfaceClass.push_back(aic);
    } else {
        Log::AddInStructMeta(EError::None, "重复添加接口");
    }
}

void MetaClass::AddMetaMemberVariable(MetaMemberVariable* mmv, bool isAddManager) {
    if (m_MetaMemberVariableDict.find(mmv->GetName()) != m_MetaMemberVariableDict.end()) {
        return;
    }
    m_MetaMemberVariableDict[mmv->GetName()] = mmv;
    if (isAddManager) {
        // MetaVariableManager::GetInstance().AddMetaMemberVariable(mmv);
    }
}

void MetaClass::AddInnerMetaMemberFunction(MetaMemberFunction* mmf) {
    // 添加内部成员函数的逻辑
}

bool MetaClass::AddMetaMemberFunction(MetaMemberFunction* mmf) {
    // 添加成员函数的逻辑
    return true;
}

void MetaClass::AddDefineConstructFunction() {
    // 添加定义构造函数的逻辑
}

void MetaClass::AddDefineInstanceValue() {
    // 添加定义实例值的逻辑
}

void MetaClass::HandleExtendClassVariable() {
    // 处理扩展类变量的逻辑
}

MetaType* MetaClass::AddMetaPreTemplateClass(MetaType* mt, bool isParse, bool& isGenMetaClass) {
    // 添加元预模板类的逻辑
    return nullptr;
}

MetaClass* MetaClass::AddMetaTemplateClassByMetaClassAndMetaTemplateMetaTypeList(const std::vector<MetaType*>& templateMetaTypeList) {
    // 根据元类和元模板元类型列表添加元模板类的逻辑
    return nullptr;
}

MetaType* MetaClass::BindStructTemplateMetaClassList(MetaType* mt) {
    // 绑定结构模板元类列表的逻辑
    return nullptr;
}

std::string MetaClass::ToString() const {
    return m_Name;
}

std::string MetaClass::ToFormatString() const {
    return getFormatString(false);
}

} // namespace Core
} // namespace SimpleLanguage
