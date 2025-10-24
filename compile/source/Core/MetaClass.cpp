//****************************************************************************
//  File:      MetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta class's attribute
//****************************************************************************

#include "MetaClass.h"
#include "../Compile/FileMeta/FileMetaClass.h"
#include "MetaNode.h"
#include "MetaType.h"
#include "MetaMemberVariable.h"
#include "MetaMemberFunction.h"
#include "MetaExpressNode.h"
#include "MetaTemplate.h"
#include "MetaParam.h"
#include "MetaGenTemplateClass.h"
#include "ClassManager.h"
#include "TypeManager.h"
#include "MetaVariableManager.h"
#include "MethodManager.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "../Debug/Log.h"
#include <algorithm>
#include <sstream>

using namespace SimpleLanguage::Debug;
using namespace SimpleLanguage::Compile;

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
    m_AllName = name;
}

MetaClass::MetaClass(const std::string& name, EType type) : MetaBase() {
    m_Name = name;
    m_Type = type;
    m_AllName = name;
}

MetaClass::MetaClass(const MetaClass& mc) : MetaBase(mc) {
    m_Name = mc.m_Name;
    m_AllName = mc.m_AllName;
    m_Type = mc.m_Type;
    m_FileMetaClassDict = mc.m_FileMetaClassDict;
    m_ExtendClass = mc.m_ExtendClass;
    if (m_ExtendClass != nullptr) {
        m_ExtendLevel = m_ExtendClass->m_ExtendLevel;
    }
    m_InterfaceClass = mc.m_InterfaceClass;

    m_MetaMemberVariableDict = mc.m_MetaMemberVariableDict;
    m_FileCollectMetaMemberVariable = mc.m_FileCollectMetaMemberVariable;
    m_MetaExtendMemeberVariableDict = mc.m_MetaExtendMemeberVariableDict;

    m_MetaMemberFunctionTemplateNodeDict = mc.m_MetaMemberFunctionTemplateNodeDict;
    m_FileCollectMetaMemberFunctionList = mc.m_FileCollectMetaMemberFunctionList;
    m_NonStaticVirtualMetaMemberFunctionList = mc.m_NonStaticVirtualMetaMemberFunctionList;
    m_StaticMetaMemberFunctionList = mc.m_StaticMetaMemberFunctionList;
    m_DefaultExpressNode = mc.m_DefaultExpressNode;
}

void MetaClass::SetDeep(int deep) {
    this->m_Deep = deep;
    for (const auto& pair : m_MetaExtendMemeberVariableDict) {
        pair.second->SetDeep(deep + 1);
    }
    for (const auto& pair : m_MetaMemberVariableDict) {
        pair.second->SetDeep(deep + 1);
    }
    for (auto v : m_NonStaticVirtualMetaMemberFunctionList) {
        v->SetDeep(deep + 1);
    }
    for (auto v : m_StaticMetaMemberFunctionList) {
        v->SetDeep(deep + 1);
    }
}

const std::unordered_map<std::string, MetaMemberVariable*>& MetaClass::GetAllMetaMemberVariableDict() const {
    static std::unordered_map<std::string, MetaMemberVariable*> allMetaMemberVariableDict;
    allMetaMemberVariableDict = m_MetaMemberVariableDict;
    allMetaMemberVariableDict.insert(m_MetaExtendMemeberVariableDict.begin(), m_MetaExtendMemeberVariableDict.end());
    return allMetaMemberVariableDict;
}

std::vector<MetaMemberVariable*> MetaClass::GetAllMetaMemberVariableList() const {
    std::vector<MetaMemberVariable*> allMetaMemberVariableList;
    allMetaMemberVariableList.reserve(m_MetaMemberVariableDict.size() + m_MetaExtendMemeberVariableDict.size());

    for (const auto& pair : GetAllMetaMemberVariableDict()) {
        allMetaMemberVariableList.push_back(pair.second);
    }
    return allMetaMemberVariableList;
}

void MetaClass::UpdateClassAllName() {
    std::stringstream sb;
    sb << m_MetaNode->GetAllName();

    if (m_MetaTemplateList.size() > 0) {
        sb << "<";
        for (size_t i = 0; i < m_MetaTemplateList.size(); i++) {
            sb << m_MetaTemplateList[i]->GetName();
            if (m_MetaTemplateList[i]->GetExtendsMetaClass() != nullptr) {
                sb << ":";
                sb << m_MetaTemplateList[i]->GetExtendsMetaClass()->GetAllClassName();
            }
            if (i < m_MetaTemplateList.size() - 1) {
                sb << ",";
            }
        }
        sb << ">";
    }

    this->m_AllName = sb.str();
}

void MetaClass::Parse() {
    // 基础解析逻辑
}

void MetaClass::ParseInnerVariable() {
    // 解析内部变量
}

void MetaClass::ParseInnerFunction() {
    // 解析内部函数
}

void MetaClass::ParseInner() {
    ParseInnerVariable();
    ParseInnerFunction();
}

void MetaClass::ParseExtendsRelation() {
    if (this->GetClassDefineType() == EClassDefineType::InnerDefine) {
        return;
    }
    if (CoreMetaClassManager::IsIncludeMetaClass(this)) {
        return;
    }

    if (this->m_ExtendClassMetaType != nullptr) {
        Log::AddInStructMeta(EError::None, "已绑定过了继承类 : " + GetExtendClass()->GetName());
        return;
    }
    
    for (const auto& pair : m_FileMetaClassDict) {
        auto mc = pair.second;
        if (mc->GetFileMetaExtendClass() == nullptr) {
            continue;
        }
        if (this->m_ExtendClassMetaType != nullptr) {
            Log::AddInStructMeta(EError::None, "已绑定过了继承类 : " + mc->GetMetaClass()->GetExtendClass()->GetName());
            continue;
        }

        MetaType* getmt = TypeManager::GetInstance().GetMetaTemplateClassAndRegisterExptendTemplateClassInstance(this, mc->GetFileMetaExtendClass());
        if (getmt != nullptr) {
            this->m_ExtendClassMetaType = getmt;
        }
        else {
            Log::AddInStructMeta(EError::None, "没有发现继承类的类型!!! " + mc->GetMetaClass()->GetExtendClass()->GetName());
        }
    }

    if (m_ExtendClassMetaType == nullptr && this != CoreMetaClassManager::GetInstance().GetObjectMetaClass()) {
        m_ExtendClassMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    }

    if (!m_ExtendClassMetaType->IsIncludeTemplate()) {
        this->m_ExtendClass = this->m_ExtendClassMetaType->GetMetaClass();
    }
    else {
        this->m_ExtendClass = m_ExtendClassMetaType->GetMetaClass();
    }
}

void MetaClass::ParseInterfaceRelation() {
    m_InterfaceMetaType.clear();
    for (const auto& pair : m_FileMetaClassDict) {
        for (size_t i = 0; i < pair.second->GetInterfaceClassList().size(); i++) {
            auto icd = pair.second->GetInterfaceClassList()[i];

            MetaType* getmt = TypeManager::GetInstance().GetMetaTemplateClassAndRegisterExptendTemplateClassInstance(this, icd);
            if (getmt == nullptr) {
                Log::AddInStructMeta(EError::None, "没有找到接口相关的定义类!!");
                continue;
            }
            m_InterfaceMetaType.push_back(getmt);
        }
    }
    if (this->m_MetaTemplateList.size() == 0) {
        for (size_t i = 0; i < m_InterfaceMetaType.size(); i++) {
            AddInterfaceClass(m_InterfaceMetaType[i]->GetMetaClass());
        }
    }
}

void MetaClass::HandleExtendMemberVariable() {
    if (m_ExtendClass == nullptr) {
        for (auto v : this->m_FileCollectMetaMemberVariable) {
            m_MetaMemberVariableDict[v->GetName()] = v;
        }
        return;
    }
    else {
        for (const auto& pair : m_ExtendClass->m_MetaExtendMemeberVariableDict) {
            auto c = pair.second;
            if (this->m_MetaMemberVariableDict.find(c->GetName()) != this->m_MetaMemberVariableDict.end()) {
                auto ld = Log::AddInStructMeta(EError::None, "Error 继承的类123:" + m_AllName + " 在继承的父类" + (m_ExtendClass ? m_ExtendClass->m_AllName : "null") + " 中已包含:" + c->GetName() + " ");
                continue;
            }
            this->m_MetaExtendMemeberVariableDict[c->GetName()] = c;
        }
        for (const auto& pair : m_ExtendClass->m_MetaMemberVariableDict) {
            auto c = pair.second;
            if (this->m_MetaMemberVariableDict.find(c->GetName()) != this->m_MetaMemberVariableDict.end()) {
                auto ld = Log::AddInStructMeta(EError::None, "Error 继承的类123:" + m_AllName + " 在继承的父类" + (m_ExtendClass ? m_ExtendClass->m_AllName : "null") + " 中已包含:" + c->GetName() + " ");
                continue;
            }
            this->m_MetaExtendMemeberVariableDict[c->GetName()] = c;
        }
        for (auto c : this->m_FileCollectMetaMemberVariable) {
            if (this->m_MetaMemberVariableDict.find(c->GetName()) != this->m_MetaMemberVariableDict.end()) {
                Log::AddInStructMeta(EError::None, "Error 继承的类321:" + m_AllName + " 在继承的父类" + m_ExtendClass->m_AllName + " 中已包含:" + c->GetName() + " ");
                continue;
            }
            this->m_MetaMemberVariableDict[c->GetName()] = c;
        }
    }
}

void MetaClass::HandleExtendMemberFunction() {
    if (this->m_ExtendClass == nullptr) {
        for (auto v : m_FileCollectMetaMemberFunctionList) {
            if (v->IsStatic()) {
                m_StaticMetaMemberFunctionList.push_back(v);
            }
            else {
                if (v->IsWithInterface()) continue;
                m_NonStaticVirtualMetaMemberFunctionList.push_back(v);
            }
        }
    }
    else {
        bool canAdd = false;
        for (auto v : this->m_ExtendClass->m_NonStaticVirtualMetaMemberFunctionList) {
            canAdd = true;
            auto efun = v;

            for (auto v2 : m_FileCollectMetaMemberFunctionList) {
                if (efun->IsEqualMetaFunction(v2)) {
                    canAdd = false;
                    m_NonStaticVirtualMetaMemberFunctionList.push_back(v2);
                    continue;
                }
            }
            if (canAdd) {
                m_NonStaticVirtualMetaMemberFunctionList.push_back(efun);
            }
        }

        for (auto v2 : this->m_FileCollectMetaMemberFunctionList) {
            if (v2->IsStatic()) {
                auto find = std::find(m_StaticMetaMemberFunctionList.begin(), m_StaticMetaMemberFunctionList.end(), v2);
                if (find != m_StaticMetaMemberFunctionList.end()) continue;

                m_StaticMetaMemberFunctionList.push_back(v2);
            }
            else {
                auto find = std::find(m_NonStaticVirtualMetaMemberFunctionList.begin(), m_NonStaticVirtualMetaMemberFunctionList.end(), v2);
                if (find != m_NonStaticVirtualMetaMemberFunctionList.end()) continue;

                m_NonStaticVirtualMetaMemberFunctionList.push_back(v2);
            }
        }

        for (auto v2 : m_NonStaticVirtualMetaMemberFunctionList) {
            AddMetaMemberFunction(v2);
        }
        for (auto v2 : m_StaticMetaMemberFunctionList) {
            AddMetaMemberFunction(v2);
        }

        std::vector<MetaMemberFunction*> addList;
        for (size_t i = 0; i < this->m_TempInnerFunctionList.size(); i++) {
            MetaMemberFunction* mmf = m_TempInnerFunctionList[i];

            bool isAdd = true;
            if (m_MetaMemberFunctionTemplateNodeDict.find(mmf->GetName()) != m_MetaMemberFunctionTemplateNodeDict.end()) {
                auto list = m_MetaMemberFunctionTemplateNodeDict[mmf->GetName()];
                MetaMemberFunction* curFun = list->IsSameMetaMemeberFunction(mmf);
                if (curFun != nullptr) {
                    isAdd = false;
                    if (mmf->IsCanRewrite()) {
                        // 处理重写逻辑
                    }
                    else {
                        isAdd = true;
                        break;
                    }
                }
            }
            if (isAdd) {
                addList.push_back(mmf);
            }
        }
        for (size_t i = 0; i < addList.size(); i++) {
            auto v = addList[i];
            if (v->IsStatic()) {
                m_StaticMetaMemberFunctionList.push_back(v);
            }
            else {
                m_NonStaticVirtualMetaMemberFunctionList.push_back(v);
            }
        }
        m_TempInnerFunctionList.clear();
    }
}

void MetaClass::ParseMemberVariableDefineMetaType() {
    for (auto it : this->m_FileCollectMetaMemberVariable) {
        it->ParseDefineMetaType();
    }
}

void MetaClass::ParseMemberFunctionDefineMetaType() {
    for (auto it : m_FileCollectMetaMemberFunctionList) {
        it->ParseDefineMetaType();
    }
}

bool MetaClass::CheckInterface() {
    return true;
}

void MetaClass::ParseDefineComplete() {
    if (m_IsInterfaceClass) {
        return;
    }
    AddDefineConstructFunction();
    if (m_DefaultExpressNode == nullptr) {
        MetaType* mdt = new MetaType(this);
        if (GetEType() == EType::Data) {
            return;
        }
        auto defaultFunction = GetMetaMemberConstructDefaultFunction();
        if (defaultFunction == nullptr) {
            Log::AddInStructMeta(EError::None, "没有找发现默认构造函数");
            return;
        }
        // m_DefaultExpressNode = new MetaNewObjectExpressNode(mdt, this, defaultFunction->GetMetaBlockStatements());
    }
}

void MetaClass::ParseGenTemplateClassMetaType() {
    // 解析生成模板类元类型
}

void MetaClass::ParseMetaInConstraint() {
    // 解析元约束
}

void MetaClass::BindFileMetaClass(Compile::FileMetaClass* fmc) {
    if (m_FileMetaClassDict.find(fmc->GetToken()) != m_FileMetaClassDict.end()) {
        return;
    }
    fmc->SetMetaClass(this);
    m_FileMetaClassDict[fmc->GetToken()] = fmc;

    if (m_IsInterfaceClass == false) {
        m_IsInterfaceClass = fmc->GetPreInterfaceToken() != nullptr;
    }
}

void MetaClass::ParseFileMetaClassTemplate(Compile::FileMetaClass* fmc) {
    // 解析文件元类模板
    // 这里需要实现模板解析逻辑
}

void MetaClass::ParseFileMetaClassMemeberVarAndFunc(Compile::FileMetaClass* fmc) {
    bool isHave = false;
    for (auto v2 : fmc->GetMemberVariableList()) {
        auto mn = m_MetaNode->GetChildrenMetaNodeByName(v2->GetName());
        if (mn != nullptr) {
            Log::AddInStructMeta(EError::None, "Error MetaClass MemberVarAndFunc已有定义类: " + m_AllName + "中 已有: " + (v2->GetToken() ? v2->GetToken()->ToLexemeAllString() : "") + "的元素!!");
            continue;
        }

        MetaMemberVariable* cmmv = GetMetaMemberVariableByName(v2->GetName());
        if (cmmv != nullptr) {
            if (cmmv != nullptr && cmmv->IsInnerDefine()) {
                break;
            }
            else {
                Log::AddInStructMeta(EError::None, "Error MetaClass MemberVarAndFunc已有定义类: " + m_AllName + "中 已有: " + (v2->GetToken() ? v2->GetToken()->ToLexemeAllString() : "") + "的元素!!");
            }
            isHave = true;
        }
        else {
            isHave = false;
        }
        MetaMemberVariable* mmv = new MetaMemberVariable(this, v2);
        if (isHave) {
            mmv->SetName(mmv->GetName() + "__repeat__");
        }
        m_FileCollectMetaMemberVariable.push_back(mmv);
        MetaVariableManager::GetInstance().AddMetaMemberVariable(mmv);
    }
    for (auto v2 : fmc->GetMemberFunctionList()) {
        auto mn = this->m_MetaNode->GetChildrenMetaNodeByName(v2->GetName());
        if (mn != nullptr) {
            Log::AddInStructMeta(EError::None, "Error MetaClass MemberVarAndFunc已有定义类: " + m_AllName + "中 已有: " + (v2->GetToken() ? v2->GetToken()->ToLexemeAllString() : "") + "的元素!!");
            continue;
        }

        MetaMemberFunction* mmf = new MetaMemberFunction(this, v2);
        m_FileCollectMetaMemberFunctionList.push_back(mmf);
        MethodManager::GetInstance().AddOriginalMemeberFunction(mmf);
    }
}

void MetaClass::CalcExtendLevel() {
    if (this->m_ExtendClass == nullptr) {
        m_ExtendLevel = 0;
    }
    else {
        MetaClass* mc = m_ExtendClass;
        int level = 0;
        while (mc != nullptr) {
            level++;
            mc = mc->GetExtendClass();
        }
        m_ExtendLevel = level;
    }
}

bool MetaClass::IsParseMetaClass(MetaClass* parentClass, bool isIncludeSelf) {
    MetaClass* mc = isIncludeSelf ? this : this->m_ExtendClass;
    while (mc != nullptr) {
        if (mc == CoreMetaClassManager::GetInstance().GetObjectMetaClass())
            break;
        
        if (mc == parentClass) {
            return true;
        }
        mc = mc->m_ExtendClass;
    }
    return false;
}

void MetaClass::AddInterfaceClass(MetaClass* aic) {
    if (std::find(m_InterfaceClass.begin(), m_InterfaceClass.end(), aic) == m_InterfaceClass.end()) {
        m_InterfaceClass.push_back(aic);
    }
    else {
        Log::AddInStructMeta(EError::None, "重复添加接口");
    }
}

void MetaClass::AddMetaMemberVariable(MetaMemberVariable* mmv, bool isAddManager) {
    if (m_MetaMemberVariableDict.find(mmv->GetName()) != m_MetaMemberVariableDict.end()) {
        return;
    }
    m_MetaMemberVariableDict[mmv->GetName()] = mmv;
    if (isAddManager) {
        MetaVariableManager::GetInstance().AddMetaMemberVariable(mmv);
    }
}

void MetaClass::AddInnerMetaMemberFunction(MetaMemberFunction* mmf) {
    m_TempInnerFunctionList.push_back(mmf);
}

bool MetaClass::AddMetaMemberFunction(MetaMemberFunction* mmf) {
    MetaMemberFunctionTemplateNode* find = nullptr;
    if (this->m_MetaMemberFunctionTemplateNodeDict.find(mmf->GetName()) != this->m_MetaMemberFunctionTemplateNodeDict.end()) {
        find = m_MetaMemberFunctionTemplateNodeDict[mmf->GetName()];
    }
    else {
        find = new MetaMemberFunctionTemplateNode();
        m_MetaMemberFunctionTemplateNodeDict[mmf->GetName()] = find;
    }
    if (find->AddMetaMemberFunction(mmf)) {
        return true;
    }
    return false;
}

void MetaClass::AddDefineConstructFunction() {
    MetaMemberFunction* mmf = GetMetaMemberConstructDefaultFunction();
    if (mmf == nullptr) {
        mmf = new MetaMemberFunction(this, "_init_");
        mmf->SetReturnMetaClass(CoreMetaClassManager::GetInstance().GetVoidMetaClass());
        mmf->Parse();
        AddMetaMemberFunction(mmf);
        MethodManager::GetInstance().AddOriginalMemeberFunction(mmf);
    }
}

void MetaClass::AddDefineInstanceValue() {
    MetaMemberVariable* mmv = this->GetMetaMemberVariableByName("instance");
    if (mmv == nullptr) {
        mmv = new MetaMemberVariable(this, "instance");
        mmv->SetDefineMetaClass(this);
        AddMetaMemberVariable(mmv);
    }
}

void MetaClass::HandleExtendClassVariable() {
    m_IsHandleExtendVariableDirty = true;
    if (m_ExtendClass != nullptr) {
        for (const auto& pair : m_ExtendClass->m_MetaMemberVariableDict) {
            m_MetaExtendMemeberVariableDict[pair.first] = pair.second;
        }
    }
}

MetaMemberVariable* MetaClass::GetMetaMemberVariableByName(const std::string& name) {
    auto it = m_MetaMemberVariableDict.find(name);
    if (it != m_MetaMemberVariableDict.end()) {
        return it->second;
    }
    auto it2 = m_MetaExtendMemeberVariableDict.find(name);
    if (it2 != m_MetaExtendMemeberVariableDict.end()) {
        return it2->second;
    }
    return nullptr;
}

std::vector<MetaMemberVariable*> MetaClass::GetMetaMemberVariableListByFlag(bool isStatic) {
    std::vector<MetaMemberVariable*> mmvList;
    MetaMemberVariable* tempMmv = nullptr;
    for (const auto& pair : m_MetaMemberVariableDict) {
        tempMmv = pair.second;
        if (isStatic) {
            if (tempMmv->IsStatic() == isStatic || tempMmv->IsConst() == isStatic) {
                mmvList.push_back(tempMmv);
            }
        }
        else {
            if (tempMmv->IsStatic() == false && tempMmv->IsConst() == false) {
                mmvList.push_back(tempMmv);
            }
        }
    }
    for (const auto& pair : m_MetaExtendMemeberVariableDict) {
        tempMmv = pair.second;
        if (isStatic) {
            if (tempMmv->IsStatic() == isStatic || tempMmv->IsConst() == isStatic) {
                mmvList.push_back(tempMmv);
            }
        }
        else {
            if (tempMmv->IsStatic() == false && tempMmv->IsConst() == false) {
                mmvList.push_back(tempMmv);
            }
        }
    }
    return mmvList;
}

MetaMemberFunction* MetaClass::GetMetaDefineGetSetMemberFunctionByName(const std::string& name, MetaInputParamCollection* inputParam, bool isGet, bool isSet) {
    auto it = m_MetaMemberFunctionTemplateNodeDict.find(name);
    if (it == m_MetaMemberFunctionTemplateNodeDict.end()) {
        return nullptr;
    }
    auto tnode = it->second;

    auto it2 = tnode->GetMetaTemplateFunctionNodeDict().find(0);
    if (it2 == tnode->GetMetaTemplateFunctionNodeDict().end()) {
        return nullptr;
    }
    auto tfunctionNode = it2->second;

    auto list = tfunctionNode->GetMetaMemberFunctionListByParamCount(inputParam != nullptr ? inputParam->GetCount() : 0);
    if (list == nullptr) return nullptr;

    for (size_t i = 0; i < list->size(); i++) {
        auto fun = (*list)[i];
        if (fun->IsTemplateFunction()) {
            return fun;
        }
        else {
            if (fun->IsEqualMetaInputParamCollection(inputParam))
                return fun;
        }
    }
    return nullptr;
}

MetaMemberFunction* MetaClass::GetMetaMemberFunctionByNameAndInputTemplateInputParamCount(const std::string& name, int templateParamCount, MetaInputParamCollection* inputParam, bool isIncludeExtendClass) {
    auto it = this->m_MetaMemberFunctionTemplateNodeDict.find(name);
    if (it == this->m_MetaMemberFunctionTemplateNodeDict.end()) {
        return nullptr;
    }
    auto tnode = it->second;
    auto it2 = tnode->GetMetaTemplateFunctionNodeDict().find(templateParamCount);
    if (it2 == tnode->GetMetaTemplateFunctionNodeDict().end()) {
        return nullptr;
    }
    auto tfunctionNode = it2->second;

    auto list = tfunctionNode->GetMetaMemberFunctionListByParamCount(inputParam != nullptr ? inputParam->GetCount() : 0);
    if (list == nullptr) return nullptr;

    for (size_t i = 0; i < list->size(); i++) {
        auto fun = (*list)[i];
        if (fun->IsTemplateFunction()) {
            return fun;
        }
        else {
            if (fun->IsEqualMetaInputParamCollection(inputParam))
                return fun;
        }
    }
    return nullptr;
}

MetaMemberFunction* MetaClass::GetMetaMemberFunctionByNameAndInputTemplateInputParam(const std::string& name, const std::vector<MetaClass*>& mtList, MetaInputParamCollection* inputParam, bool isIncludeExtendClass) {
    auto it = this->m_MetaMemberFunctionTemplateNodeDict.find(name);
    if (it == this->m_MetaMemberFunctionTemplateNodeDict.end()) {
        return nullptr;
    }
    int templateParamCount = 0;
    if (mtList.size() > 0) {
        templateParamCount = mtList.size();
    }
    auto tnode = it->second;
    auto it2 = tnode->GetMetaTemplateFunctionNodeDict().find(templateParamCount);
    if (it2 == tnode->GetMetaTemplateFunctionNodeDict().end()) {
        return nullptr;
    }
    auto tfunctionNode = it2->second;

    auto list = tfunctionNode->GetMetaMemberFunctionListByParamCount(inputParam != nullptr ? inputParam->GetCount() : 0);
    if (list == nullptr) return nullptr;

    for (size_t i = 0; i < list->size(); i++) {
        auto fun = (*list)[i];
        if (fun->IsTemplateFunction()) {
            auto gfun = fun->GetGenTemplateFunction(mtList);
            if (gfun != nullptr) {
                return gfun;
            }
            return fun;
        }
        else {
            if (fun->IsEqualMetaInputParamCollection(inputParam))
                return fun;
        }
    }
    return nullptr;
}

MetaMemberFunction* MetaClass::GetMetaMemberConstructDefaultFunction() {
    return GetMetaMemberConstructFunction(nullptr);
}

MetaMemberFunction* MetaClass::GetMetaMemberConstructFunction(MetaInputParamCollection* mmpc) {
    return GetMetaMemberFunctionByNameAndInputTemplateInputParamCount("_init_", 0, mmpc, false);
}

MetaMemberFunction* MetaClass::GetFirstMetaMemberFunctionByName(const std::string& name) {
    return GetMetaMemberFunctionByNameAndInputTemplateInputParamCount(name, 0, nullptr);
}

std::vector<MetaMemberFunction*> MetaClass::GetMemberInterfaceFunction() {
    std::vector<MetaMemberFunction*> mmf;
    return mmf;
}

bool MetaClass::GetMemberInterfaceFunctionByFunc(MetaMemberFunction* func) {
    return true;
}

MetaType* MetaClass::AddMetaPreTemplateClass(MetaType* mt, bool isParse, bool& isGenMetaClass) {
    isGenMetaClass = false;
    if (mt->GetMetaClass() == nullptr) {
        return nullptr;
    }
    MetaGenTemplateClass* mgtc = mt->GetMetaClass()->AddMetaTemplateClassByMetaClassAndMetaTemplateMetaTypeList(mt->GetTemplateMetaTypeList());

    if (mgtc != nullptr) {
        isGenMetaClass = true;
        if (isParse) {
            mgtc->Parse();
        }
        return new MetaType(mgtc, mt->GetTemplateMetaTypeList());
    }

    auto find = BindStructTemplateMetaClassList(mt);
    if (find == nullptr) {
        this->m_BindStructTemplateMetaClassList.push_back(new MetaType(*mt));
    }
    return mt;
}

MetaGenTemplateClass* MetaClass::AddMetaTemplateClassByMetaClassAndMetaTemplateMetaTypeList(const std::vector<MetaType*>& templateMetaTypeList) {
    std::vector<MetaClass*> mcList;
    for (size_t i = 0; i < templateMetaTypeList.size(); i++) {
        auto mtc = templateMetaTypeList[i];
        if (mtc->GetEType() == EMetaTypeType::MetaClass || mtc->GetEType() == EMetaTypeType::MetaGenClass) {
            mcList.push_back(mtc->GetMetaClass());
        }
    }
    if (mcList.size() == templateMetaTypeList.size()) {
        MetaGenTemplateClass* mgtc = AddInstanceMetaClass(mcList);
        return mgtc;
    }
    return nullptr;
}

MetaType* MetaClass::BindStructTemplateMetaClassList(MetaType* mt) {
    for (auto v : m_BindStructTemplateMetaClassList) {
        if (MetaType::EqualMetaDefineType(v, mt)) {
            return v;
        }
    }
    return nullptr;
}

// CRITICAL MISSING METHOD: AddInstanceMetaClass
MetaGenTemplateClass* MetaClass::AddInstanceMetaClass(const std::vector<MetaClass*>& mcList) {
    // 这个方法在C#源码中非常重要，用于创建模板类的实例
    // 在C#中，这个方法会创建一个MetaGenTemplateClass实例
    // 这里需要实现具体的逻辑来创建和管理模板类实例
    
    // 创建新的MetaGenTemplateClass实例
    MetaGenTemplateClass* mgtc = new MetaGenTemplateClass();
    
    // 设置模板参数
    for (size_t i = 0; i < mcList.size(); i++) {
        mgtc->AddTemplateParameter(mcList[i]);
    }
    
    // 注册到ClassManager中
    ClassManager::GetInstance().AddMetaGenTemplateClass(mgtc);
    
    return mgtc;
}

std::string MetaClass::ToString() const {
    std::stringstream stringBuilder;

    if (this->IsGenTemplate()) {
        stringBuilder << " [Gen] ";
    }
    else {
        if (this->IsTemplateClass()) {
            stringBuilder << " [Template] ";
        }
    }

    stringBuilder << GetAllClassName();

    return stringBuilder.str();
}

std::string MetaClass::ToFormatString() const {
    return GetFormatString(false);
}

std::string MetaClass::ToDefineTypeString() const {
    std::stringstream stringBuilder;
    stringBuilder << GetAllClassName();
    return stringBuilder.str();
}

std::string MetaClass::GetFormatString(bool isShowNamespace) const {
    std::stringstream stringBuilder;
    stringBuilder.clear();
    for (int i = 0; i < GetRealDeep(); i++)
        stringBuilder << "\t";
    stringBuilder << GetPermission().ToFormatString();
    stringBuilder << " ";
    if (isShowNamespace) {
        stringBuilder << "class ";
        stringBuilder << GetName();
    }
    else {
        stringBuilder << "class " + GetName();
    }
    if (m_MetaTemplateList.size() > 0) {
        stringBuilder << "<";
        for (size_t i = 0; i < m_MetaTemplateList.size(); i++) {
            stringBuilder << m_MetaTemplateList[i]->ToFormatString();
            if (i < m_MetaTemplateList.size() - 1) {
                stringBuilder << ",";
            }
        }
        stringBuilder << ">";
    }
    if (m_ExtendClass != nullptr) {
        stringBuilder << " extends ";
        stringBuilder << m_ExtendClass->GetAllClassName();
        auto mtl = m_ExtendClass->GetMetaTemplateList();
        if (mtl.size() > 0) {
            stringBuilder << "<";
            for (size_t i = 0; i < mtl.size(); i++) {
                stringBuilder << mtl[i]->ToFormatString();
                if (i < mtl.size() - 1) {
                    stringBuilder << ",";
                }
            }
            stringBuilder << ">";
        }
    }
    if (m_InterfaceMetaType.size() > 0) {
        stringBuilder << " interface ";
    }
    for (size_t i = 0; i < m_InterfaceMetaType.size(); i++) {
        stringBuilder << m_InterfaceMetaType[i]->ToFormatString();
        if (i != m_InterfaceClass.size() - 1)
            stringBuilder << ",";
    }
    stringBuilder << "\n";

    for (int i = 0; i < GetRealDeep(); i++)
        stringBuilder << "\t";
    stringBuilder << "{\n";

    for (const auto& pair : m_MetaNode->GetChildrenMetaNodeDict()) {
        stringBuilder << pair.second->ToFormatString();
    }

    for (const auto& pair : m_MetaMemberVariableDict) {
        stringBuilder << pair.second->ToFormatString() << "\n";
    }

    for (auto v : m_StaticMetaMemberFunctionList) {
        stringBuilder << v->ToFormatString();
        stringBuilder << "\n";
    }

    for (auto v : m_NonStaticVirtualMetaMemberFunctionList) {
        stringBuilder << v->ToFormatString();
        stringBuilder << "\n";
    }

    for (int i = 0; i < GetRealDeep(); i++)
        stringBuilder << "\t";
    stringBuilder << "}\n";

    return stringBuilder.str();
}

} // namespace Core
} // namespace SimpleLanguage