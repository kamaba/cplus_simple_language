//****************************************************************************
//  File:      ClassManager.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/30 12:00:00
//  Description: Meta Class's manager center 
//****************************************************************************

#include "MetaType.h"
#include "ClassManager.h"
#include "ModuleManager.h"
#include "MetaModule.h"
#include "MetaClass.h"
#include "MetaData.h"
#include "MetaEnum.h"
#include "MetaDynamicClass.h"
#include "NamespaceManager.h"
#include "MetaVariableManager.h"
#include "MetaGenTemplateClass.h"
#include "../Debug/Log.h"
#include "../Project/ProjectManager.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "../Compile/FileMeta/FileMetaClass.h"
#include "../Compile/FileMeta/FileMeta.h"
#include <algorithm>
#include <iostream>

using namespace SimpleLanguage::Compile;
using namespace SimpleLanguage::Debug;
using namespace SimpleLanguage::Project;

namespace SimpleLanguage {
namespace Core {

ClassManager* ClassManager::s_Instance = nullptr;

ClassManager& ClassManager::GetInstance() {
    if (s_Instance == nullptr) {
        s_Instance = new ClassManager();
    }
    return *s_Instance;
}

void ClassManager::DestroyInstance() {
    if (s_Instance != nullptr) {
        delete s_Instance;
        s_Instance = nullptr;
    }
}

ClassManager::ClassManager() {
}

ClassManager::~ClassManager() {
    // 清理资源
    for (auto& pair : m_AllClassDict) {
        delete pair.second;
    }
    for (auto& pair : m_AllDataDict) {
        delete pair.second;
    }
    for (auto* dc : m_DynamicClassList) {
        delete dc;
    }
    for (auto* gtc : m_GenTemplateMetaClassList) {
        delete gtc;
    }
}

MetaClass* ClassManager::GetClassByName(const std::string& name, int templateCount) {
    std::string nname = name + "_" + std::to_string(templateCount);
    auto it = m_AllClassDict.find(nname);
    if (it != m_AllClassDict.end()) {
        return it->second;
    }
    return nullptr;
}

MetaClass* ClassManager::GetMetaClassByCSharpType(const std::string& typeName) {
    if (typeName == "Byte") {
        return CoreMetaClassManager::GetInstance().GetByteMetaClass();
    }
    else if (typeName == "SByte") {
        return CoreMetaClassManager::GetInstance().GetSByteMetaClass();
    }
    else if (typeName == "Int16") {
        return CoreMetaClassManager::GetInstance().GetInt16MetaClass();
    }
    else if (typeName == "UInt16") {
        return CoreMetaClassManager::GetInstance().GetUInt16MetaClass();
    }
    else if (typeName == "Int32") {
        return CoreMetaClassManager::GetInstance().GetInt32MetaClass();
    }
    else if (typeName == "UInt32") {
        return CoreMetaClassManager::GetInstance().GetUInt32MetaClass();
    }
    else if (typeName == "Int64") {
        return CoreMetaClassManager::GetInstance().GetInt64MetaClass();
    }
    else if (typeName == "UInt64") {
        return CoreMetaClassManager::GetInstance().GetUInt64MetaClass();
    }
    else if (typeName == "Float32") {
        return CoreMetaClassManager::GetInstance().GetFloat32MetaClass();
    }
    else if (typeName == "Float64") {
        return CoreMetaClassManager::GetInstance().GetFloat64MetaClass();
    }
    else if (typeName == "String") {
        return CoreMetaClassManager::GetInstance().GetStringMetaClass();
    }
    else if (typeName == "Object") {
        return CoreMetaClassManager::GetInstance().GetObjectMetaClass();
    }
    else if (typeName == "Void") {
        return CoreMetaClassManager::GetInstance().GetVoidMetaClass();
    }
    return nullptr;
}

bool ClassManager::AddMetaClass(MetaClass* mc, MetaModule* mm) {
    MetaNode* topLevelNamespace = mm ? mm->GetMetaNode() : nullptr;
    if (topLevelNamespace == nullptr) {
        topLevelNamespace = ModuleManager::GetInstance().GetSelfModule()->GetMetaNode();
    }
    topLevelNamespace->AddMetaClass(mc);
    return true;
}

void ClassManager::AddGenTemplateClass(MetaGenTemplateClass* mc) {
    auto it = std::find(m_GenTemplateMetaClassList.begin(), m_GenTemplateMetaClassList.end(), mc);
    if (it == m_GenTemplateMetaClassList.end()) {
        m_GenTemplateMetaClassList.push_back(mc);
    }
    if (m_AllClassDict.find(mc->GetAllClassName()) == m_AllClassDict.end()) {
        m_AllClassDict[mc->GetAllClassName()] = mc;
    }
}

MetaDynamicClass* ClassManager::FindDynamicClass(MetaClass* dc) {
    for (auto* v : m_DynamicClassList) {
        if (CompareMetaClassMemberVariable(dc, v)) {
            return v;
        }
    }
    return nullptr;
}

bool ClassManager::AddDynamicClass(MetaDynamicClass* dc) {
    m_DynamicClassList.push_back(dc);
    m_AllClassDict[dc->GetAllClassName()] = dc;
    return true;
}

void ClassManager::AddInitHandleMetaClassList(MetaClass* mc) {
    auto it = std::find(m_InitHandleMetaClassList.begin(), m_InitHandleMetaClassList.end(), mc);
    if (it == m_InitHandleMetaClassList.end()) {
        m_InitHandleMetaClassList.push_back(mc);
    }
}

MetaData* ClassManager::FindMetaData(MetaData* md) {
    for (const auto& v : m_AllDataDict) {
        if (CompareMetaDataMember(v.second, md)) {
            return v.second;
        }
    }
    return nullptr;
}

MetaData* ClassManager::FindMetaDataByName(const std::string& name) {
    auto it = m_AllDataDict.find(name);
    if (it != m_AllDataDict.end()) {
        return it->second;
    }
    return nullptr;
}

bool ClassManager::AddMetaData(MetaData* dc) {
    m_AllDataDict[dc->GetName()] = dc;
    return true;
}

bool ClassManager::CompareMetaClassMemberVariable(MetaClass* curClass, MetaClass* cpClass) {
    auto curClassList = curClass->GetAllMetaMemberVariableList();
    auto cpClassList = cpClass->GetAllMetaMemberVariableList();

    if (curClassList.size() == cpClassList.size()) {
        for (size_t i = 0; i < curClassList.size(); i++) {
            auto curMV = curClassList[i];
            auto cpMV = cpClassList[i];
            if (curMV->IsConst() == cpMV->IsConst() &&
                curMV->IsStatic() == cpMV->IsStatic() &&
                curMV->GetName() == cpMV->GetName() &&
                curMV->GetMetaDefineType() == cpMV->GetMetaDefineType()) {
                // 继续比较
            }
            else {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool ClassManager::CompareMetaDataMember(MetaData* curClass, MetaData* cpClass) {
    auto curClassList = curClass->GetMetaMemberDataDict();
    auto cpClassList = cpClass->GetMetaMemberDataDict();

    if (curClassList.size() != cpClassList.size()) {
        return false;
    }
    
    for (const auto& v : curClassList) {
        auto it = cpClassList.find(v.first);
        if (it == cpClassList.end()) {
            return false;
        }
        auto vval = v.second;
        auto val2 = it->second;

        if (vval->GetMetaDefineType()->GetMetaClass() != val2->GetMetaDefineType()->GetMetaClass()) {
            return false;
        }
    }

    return true;
}

MetaClass* ClassManager::AddClass(FileMetaClass* fmc) {
    bool isCanAddBind = false;
    MetaNode* finalTopMetaNode = ModuleManager::GetInstance().GetSelfModule()->GetMetaNode();
    FileMetaClass* topLevelClass = fmc->GetTopLevelFileMetaClass();
    
    if (topLevelClass != nullptr) {
        if (topLevelClass->GetMetaClass() && topLevelClass->GetMetaClass()->GetMetaNode() == nullptr) {
            Log::AddInStructMeta(EError::None, "Error 上级类中的MetaClass没有绑!!");
            return nullptr;
        }

        auto findmc = topLevelClass->GetMetaClass()->GetMetaNode()->GetChildrenMetaNodeByName(fmc->GetName());
        if (findmc != nullptr) {
            if (findmc->IsMetaNamespace() || findmc->IsMetaData() || findmc->IsMetaEnum()) {
                Log::AddInStructMeta(EError::None, "找到同名空间或数据节点，不允许重复名称的节点再次创建");
                return nullptr;
            }

            MetaClass* findmc2 = findmc->GetMetaClassByTemplateCount(fmc->GetTemplateDefineList().size());
            if (findmc2 != nullptr) {
                if (findmc2->GetClassDefineType() == EClassDefineType::StructDefine) {
                    findmc2->BindFileMetaClass(fmc);
                    findmc2->SetClassDefineType(EClassDefineType::CodeDefine);
                    findmc2->ParseFileMetaClassTemplate(fmc);
                    findmc2->ParseFileMetaClassMemeberVarAndFunc(fmc);
                    return findmc2;
                }
                else {
                    Log::AddInStructMeta(EError::None, "Error 找到内部类内部类，不允许相同成员");
                }
            }
            else {
                Log::AddInStructMeta(EError::None, "Error 找到内部类内部类，不允许相同成员");
                return nullptr;
            }
        }
        else {
            finalTopMetaNode = topLevelClass->GetMetaClass()->GetMetaNode();
            isCanAddBind = true;
        }
    }
    else {
        // 处理命名空间逻辑
        if (fmc->GetTopLevelFileMetaNamespace() != nullptr) {
            finalTopMetaNode = ModuleManager::GetInstance().GetChildrenMetaNodeByName(fmc->GetTopLevelFileMetaNamespace()->GetName());
            // 处理命名空间处理逻辑...
        }
    }

    if (isCanAddBind) {
        if (ProjectManager::useDefineNamespaceType == EUseDefineType::LimitUseProjectConfigNamespaceAndClass) {
            Log::AddInStructMeta(EError::None, "Error 使用的强制定义节点的方式中，没有找到相关的类，不能创建新的类，请在工程中定义");
        }
        
        if (fmc->IsEnum()) {
            MetaEnum* newme = new MetaEnum(fmc->GetName());
            finalTopMetaNode->AddMetaEnum(newme);
            newme->BindFileMetaClass(fmc);
            newme->ParseFileMetaEnumMemeberEnum(fmc);
            AddInitHandleMetaClassList(newme);
            return newme;
        }
        else if (fmc->IsData()) {
            MetaData* newmd = new MetaData(fmc);
            finalTopMetaNode->AddMetaData(newmd);
            AddInitHandleMetaClassList(newmd);
            newmd->BindFileMetaClass(fmc);
            newmd->ParseFileMetaDataMemeberData(fmc);
            return newmd;
        }
        else {
            if (fmc->IsConst()) {
                Log::AddInStructMeta(EError::None, "Class 中，使用关键字，不允许使用Const");
                return nullptr;
            }
            MetaClass* newmc = new MetaClass(fmc->GetName());
            newmc->BindFileMetaClass(fmc);
            newmc->SetClassDefineType(EClassDefineType::CodeDefine);
            newmc->ParseFileMetaClassTemplate(fmc);
            finalTopMetaNode->AddMetaClass(newmc);
            newmc->UpdateClassAllName();
            newmc->ParseFileMetaClassMemeberVarAndFunc(fmc);
            AddInitHandleMetaClassList(newmc);
            return newmc;
        }
    }
    else {
        return nullptr;
    }
}

void ClassManager::AddRuntimeMetaClass(MetaClass* mc) {
    auto find1 = std::find(m_RuntimeClassList.begin(), m_RuntimeClassList.end(), mc);
    if (find1 == m_RuntimeClassList.end()) {
        m_RuntimeClassList.push_back(mc);
        AddDictMetaClass(mc);
    }
}

void ClassManager::AddDictMetaClass(MetaClass* mc) {
    std::string acn = mc->GetAllClassName() + "_" + std::to_string(mc->GetMetaTemplateList().size());
    for (const auto& v : m_AllClassDict) {
        if (v.second == mc) {
            return;
        }
    }
    m_AllClassDict[acn] = mc;
}

void ClassManager::HandleExtendMember() {
    for (auto* it : m_InitHandleMetaClassList) {
        it->CalcExtendLevel();
    }

    std::sort(m_InitHandleMetaClassList.begin(), m_InitHandleMetaClassList.end(),
        [](MetaClass* x, MetaClass* y) { return x->GetExtendLevel() - y->GetExtendLevel(); });
    
    for (auto* it : m_InitHandleMetaClassList) {
        it->HandleExtendMemberVariable();
        it->HandleExtendMemberFunction();
    }
    for (auto* it : m_InitHandleMetaClassList) {
        it->ParseGenTemplateClassMetaType();
    }
}

void ClassManager::ParseInitMetaClassList() {
    for (auto* it : m_InitHandleMetaClassList) {
        it->ParseMetaInConstraint();
        it->ParseExtendsRelation();
        it->ParseInterfaceRelation();
        AddRuntimeMetaClass(it);
    }
    for (auto* it : m_InitHandleMetaClassList) {
        it->ParseMemberVariableDefineMetaType();
        it->ParseMemberFunctionDefineMetaType();
    }
}

void ClassManager::UpdateMetaGenTemplateClassHandle() {
    auto list = std::vector<MetaGenTemplateClass*>(m_GenTemplateMetaClassList);
    for (auto* v : list) {
        v->UpdateRegsterGenMetaClass();
    }
}

void ClassManager::CheckInterfaces() {
    for (auto* it : m_InitHandleMetaClassList) {
        it->CheckInterface();
    }
}

void ClassManager::ParseDefineComplete() {
    for (auto* it : m_InitHandleMetaClassList) {
        it->ParseDefineComplete();
    }
}

void ClassManager::ParseMemberEnumExpress() {
    for (auto* it : m_InitHandleMetaClassList) {
        if (auto* me = dynamic_cast<MetaEnum*>(it)) {
            me->ParseMemberMetaEnumExpress();
        }
    }
}

bool ClassManager::IsNumberClass(MetaClass* curClass) {
    if (curClass == CoreMetaClassManager::GetInstance().GetByteMetaClass() ||
        curClass == CoreMetaClassManager::GetInstance().GetSByteMetaClass() ||
        curClass == CoreMetaClassManager::GetInstance().GetInt16MetaClass() ||
        curClass == CoreMetaClassManager::GetInstance().GetUInt16MetaClass() ||
        curClass == CoreMetaClassManager::GetInstance().GetInt32MetaClass() ||
        curClass == CoreMetaClassManager::GetInstance().GetUInt32MetaClass() ||
        curClass == CoreMetaClassManager::GetInstance().GetInt64MetaClass() ||
        curClass == CoreMetaClassManager::GetInstance().GetUInt64MetaClass()) {
        return true;
    }
    return false;
}

EClassRelation ClassManager::ValidateClassRelationByMetaClass(MetaClass* curClass, MetaClass* compareClass) {
    if (curClass == CoreMetaClassManager::GetInstance().GetObjectMetaClass()) {
        if (curClass == compareClass) {
            return EClassRelation::Same;
        }
        return EClassRelation::Child;
    }
    if (curClass == compareClass) {
        return EClassRelation::Same;
    }
    else {
        if (IsNumberClass(curClass) && IsNumberClass(compareClass)) {
            return EClassRelation::Similar;
        }
        else {
            if (curClass->IsParseMetaClass(compareClass)) {
                return EClassRelation::Parent;
            }
            if (compareClass->IsParseMetaClass(curClass)) {
                return EClassRelation::Child;
            }
            return EClassRelation::No;
        }
    }
}

void ClassManager::HandleInterface(FileMetaClass* mc) {
    // 接口处理逻辑
}

MetaNode* ClassManager::GetMetaClassByRef(MetaClass* mc, FileMetaClassDefine* fmcv) {
    if (fmcv == nullptr) return nullptr;

    MetaNode* mb = GetMetaClassByClassDefine(mc, fmcv);
    if (mb != nullptr) {
        return mb;
    }

    auto mb2 = fmcv->GetFileMeta()->GetMetaBaseByFileMetaClassRef(fmcv);
    return mb2;
}

MetaNode* ClassManager::GetMetaClassByClassDefine(MetaClass* ownerClass, FileMetaClassDefine* fmcd) {
    return GetMetaClassByNameAndFileMeta(ownerClass, fmcd->GetFileMeta(), fmcd->GetStringList());
}

MetaNode* ClassManager::GetMetaClassByNameAndFileMeta(MetaClass* ownerClass, FileMeta* fm, const std::vector<std::string>& stringList) {
    MetaNode* mn = GetMetaNodeByListString(ownerClass, stringList);
    if (mn == nullptr) {
        auto mb = fm->GetMetaNodeFileMetaClass(stringList);
        if (mb != nullptr) {
            return mb;
        }
    }
    return mn;
}

MetaNode* ClassManager::GetMetaNodeByListString(MetaClass* ownerClass, const std::vector<std::string>& stringList) {
    if (stringList.empty()) {
        return nullptr;
    }

    std::string firstName = "";
    if (stringList.size() == 1) {
        firstName = stringList[0];
    }
    MetaNode* findMB = CoreMetaClassManager::GetCoreMetaClass(firstName);
    if (findMB && findMB->IsMetaClass()) {
        return findMB;
    }
    findMB = nullptr;

    MetaNode* mb = ModuleManager::GetInstance().GetSelfModule()->GetMetaNode();
    if (ownerClass != nullptr) {
        mb = ownerClass->GetMetaNode();
    }
    
    while (true) {
        MetaNode* parentMB = mb;
        for (size_t i = 0; i < stringList.size(); i++) {
            std::string name = stringList[i];
            if (parentMB != nullptr) {
                if (findMB == nullptr) {
                    findMB = parentMB->GetChildrenMetaNodeByName(name);
                    if (findMB == nullptr) {
                        parentMB = nullptr;
                        break;
                    }
                    parentMB = findMB;
                }
                else {
                    parentMB = parentMB->GetChildrenMetaNodeByName(name);
                }
            }
        }
        if (parentMB != nullptr) {
            if (parentMB->IsMetaClass()) {
                return parentMB;
            }
        }
        mb = mb->GetParentNode();
        if (mb == nullptr) {
            break;
        }
    }
    return nullptr;
}

MetaNode* ClassManager::GetMetaClassByClassDefineAndFileMeta(MetaClass* ownerClass, FileMetaClassDefine* fmcd) {
    FileMeta* fm = fmcd->GetFileMeta();
    MetaNode* mc = GetMetaClassByClassDefine(ownerClass, fmcd);
    if (mc == nullptr) {
        auto mb = fm->GetMetaBaseByFileMetaClassRef(fmcd);
        if (mb != nullptr) {
            if (mb->IsMetaNamespace()) {
                Log::AddInStructMeta(EError::None, "找到的是命名空间节点，需要继承的类!!");
                return nullptr;
            }
            else if (mb->IsMetaClass()) {
                return mb;
            }
        }
    }
    return mc;
}

MetaClass* ClassManager::GetMetaClassByInputTemplateAndFileMeta(MetaClass* ownerClass, FileInputTemplateNode* fitn) {
    auto nlist = fitn->GetNameList();
    FileMeta* fm = fitn->GetFileMeta();
    MetaNode* mn = GetMetaClassByNameAndFileMeta(ownerClass, fitn->GetFileMeta(), nlist);
    if (mn == nullptr) {
        auto mb = mn->GetMetaClassByTemplateCount(fitn->GetInputTemplateCount());
        if (mb != nullptr) {
            return mb;
        }
    }
    return nullptr;
}

MetaClass* ClassManager::GetMetaClassAndRegisterExptendTemplateClassInstance(MetaClass* curMc, FileMetaClassDefine* fmcd) {
    if (fmcd == nullptr) return nullptr;

    MetaNode* getmc = GetMetaClassByRef(curMc, fmcd);
    if (getmc == nullptr) {
        Log::AddInStructMeta(EError::StructMetaStart, " CheckExtendAndInterface 判断继承的时候，父没有:");// +fmcd->GetAllName() + "  类");
    }
    else {
        // 模板处理逻辑
    }
    return nullptr;
}

void ClassManager::PrintAlllClassContent() {
    for (const auto& v : m_AllClassDict) {
        // 打印所有类
    }

    for (auto* v : m_RuntimeClassList) {
        // 打印运行时类
    }
}

} // namespace Core
} // namespace SimpleLanguage