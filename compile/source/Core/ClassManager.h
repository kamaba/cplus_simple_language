//****************************************************************************
//  File:      ClassManager.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/30 12:00:00
//  Description: Meta Class's manager center 
//****************************************************************************

#pragma once

#include "MetaClass.h"
#include "MetaData.h"
#include "MetaDynamicClass.h"
#include "MetaGenTemplateClass.h"
#include "MetaEnum.h"
#include "../Compile/CoreFileMeta/FileMetaClass.h"
#include "../Compile/CoreFileMeta/FileMetaClassDefine.h"
#include "../Compile/CoreFileMeta/FileMeta.h"
#include "../Compile/CoreFileMeta/FileInputTemplateNode.h"
#include "../BaseMetaClass/CoreMetaClassManager.h"
#include "ModuleManager.h"
#include "NamespaceManager.h"
#include "MetaVariableManager.h"
#include "../Debug/Log.h"
#include "../Project/ProjectManager.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace SimpleLanguage {
namespace Core {

enum class EClassRelation {
    None = 0,
    CurClassError = 1,
    CompareClassError = 2,
    No = 3,
    Same = 4,
    Child = 5,
    Parent = 6,
    Similar = 7,
    SameClassNotSameInputTemplate = 8,
    SameClassAndSameInputTemplate = 9
};

class ClassManager {
private:
    static ClassManager* s_Instance;
    
    std::vector<MetaClass*> m_RuntimeClassList;
    std::unordered_map<std::string, MetaClass*> m_AllClassDict;
    std::vector<MetaDynamicClass*> m_DynamicClassList;
    std::unordered_map<std::string, MetaData*> m_AllDataDict;
    std::vector<MetaGenTemplateClass*> m_GenTemplateMetaClassList;
    std::vector<MetaClass*> m_InitHandleMetaClassList;

public:
    static ClassManager& GetInstance();
    static void DestroyInstance();
    
    // 属性访问器
    const std::unordered_map<std::string, MetaClass*>& GetAllClassDict() const { return m_AllClassDict; }
    const std::unordered_map<std::string, MetaData*>& GetAllDataDict() const { return m_AllDataDict; }
    const std::vector<MetaDynamicClass*>& GetDynamicClassList() const { return m_DynamicClassList; }
    const std::vector<MetaGenTemplateClass*>& GetGenTemplateMetaClassList() const { return m_GenTemplateMetaClassList; }
    const std::vector<MetaClass*>& GetRuntimeClassList() const { return m_RuntimeClassList; }
    
    // 类管理方法
    MetaClass* GetClassByName(const std::string& name, int templateCount = 0);
    MetaClass* GetMetaClassByCSharpType(const std::string& typeName);
    bool AddMetaClass(MetaClass* mc, MetaModule* mm = nullptr);
    void AddGenTemplateClass(MetaGenTemplateClass* mc);
    MetaDynamicClass* FindDynamicClass(MetaClass* dc);
    bool AddDynamicClass(MetaDynamicClass* dc);
    void AddInitHandleMetaClassList(MetaClass* mc);
    
    // 数据管理方法
    MetaData* FindMetaData(MetaData* md);
    MetaData* FindMetaDataByName(const std::string& name);
    bool AddMetaData(MetaData* dc);
    
    // 比较方法
    bool CompareMetaClassMemberVariable(MetaClass* curClass, MetaClass* cpClass);
    bool CompareMetaDataMember(MetaData* curClass, MetaData* cpClass);
    
    // 类添加和处理方法
    MetaClass* AddClass(FileMetaClass* fmc);
    void AddRuntimeMetaClass(MetaClass* mc);
    void HandleExtendMember();
    void ParseInitMetaClassList();
    void UpdateMetaGenTemplateClassHandle();
    void CheckInterfaces();
    void ParseDefineComplete();
    void ParseMemberEnumExpress();
    
    // 静态工具方法
    static bool IsNumberClass(MetaClass* curClass);
    static EClassRelation ValidateClassRelationByMetaClass(MetaClass* curClass, MetaClass* compareClass);
    
    // 类查找方法
    MetaNode* GetMetaClassByRef(MetaClass* mc, FileMetaClassDefine* fmcv);
    MetaNode* GetMetaClassByClassDefine(MetaClass* ownerClass, FileMetaClassDefine* fmcd);
    MetaNode* GetMetaClassByNameAndFileMeta(MetaClass* ownerClass, FileMeta* fm, const std::vector<std::string>& stringList);
    MetaNode* GetMetaClassByClassDefineAndFileMeta(MetaClass* ownerClass, FileMetaClassDefine* fmcd);
    MetaClass* GetMetaClassByInputTemplateAndFileMeta(MetaClass* ownerClass, FileInputTemplateNode* fitn);
    MetaClass* GetMetaClassAndRegisterExptendTemplateClassInstance(MetaClass* curMc, FileMetaClassDefine* fmcd);
    
    // 调试方法
    void PrintAlllClassContent();
    
    // 接口处理方法
    void HandleInterface(FileMetaClass* mc);

private:
    ClassManager();
    ~ClassManager();
    ClassManager(const ClassManager&) = delete;
    ClassManager& operator=(const ClassManager&) = delete;
    
    void AddDictMetaClass(MetaClass* mc);
    MetaNode* GetMetaNodeByListString(MetaClass* ownerClass, const std::vector<std::string>& stringList);
};

} // namespace Core
} // namespace SimpleLanguage
