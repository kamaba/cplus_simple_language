//****************************************************************************
//  File:      ProjectManager.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/30 12:00:00
//  Description:  manager project enter and compile 
//****************************************************************************

#pragma once

#include <string>
#include <vector>

namespace SimpleLanguage {
    namespace Core {
        class MetaData;;
    }
namespace Project {

// Forward declarations
class ProjectData;

class CommandInputArgs {
public:
    CommandInputArgs(const std::vector<std::string>& args);
    virtual ~CommandInputArgs() = default;

    // Properties
    bool GetIsTest() const { return m_IsTest; }
    void SetIsTest(bool value) { m_IsTest = value; }
    
    bool GetIsPrintToken() const { return m_IsPrintToken; }
    void SetIsPrintToken(bool value) { m_IsPrintToken = value; }

private:
    bool m_IsTest = false;
    bool m_IsPrintToken = false;
};

enum class EUseDefineType {
    NoUseProjectConfigNamespace = 0,        // 不使用项目内部配置
    LimitUseProjectConfigNamespace = 1,     // 限制使用配置后的命名空间 类自由创作
    LimitUseProjectConfigNamespaceAndClass = 2, // 限制使用配置后的命名空间与类
};

class ProjectManager {
public:
    // Static properties
    static std::string GetProjectPath() { return projectPath; }
    static void SetProjectPath(const std::string& path) { projectPath = path; }
    static ProjectData* GetData();
    static EUseDefineType GetUseDefineNamespaceType() { return useDefineNamespaceType; }
    static void SetUseDefineNamespaceType(EUseDefineType type) { useDefineNamespaceType = type; }
    
    static bool GetUseGenMetaClass() { return useGenMetaClass; }
    static void SetUseGenMetaClass(bool value) { useGenMetaClass = value; }
    static bool GetCompileUseTemplateClassGenClassFunction() { return compileUseTemplateClassGenClassFunction; }
    static void SetCompileUseTemplateClassGenClassFunction(bool value) { compileUseTemplateClassGenClassFunction = value; }
    static bool GetIsUseNamespaceSearch() { return isUseNamespaceSearch; }
    static void SetIsUseNamespaceSearch(bool value) { isUseNamespaceSearch = value; }
    static bool GetIsUseForceSemiColonInLineEnd() { return isUseForceSemiColonInLineEnd; }
    static void SetIsUseForceSemiColonInLineEnd(bool value) { isUseForceSemiColonInLineEnd = value; }
    static bool GetIsFirstPosMustUseThisBaseOrStaticClassName() { return isFirstPosMustUseThisBaseOrStaticClassName; }
    static void SetIsFirstPosMustUseThisBaseOrStaticClassName(bool value) { isFirstPosMustUseThisBaseOrStaticClassName = value; }
    
    static std::string GetRootPath() { return rootPath; }
    static void SetRootPath(const std::string& path) { rootPath = path; }
    static Core::MetaData* GetGlobalData() { return globalData; }
    static void SetGlobalData(Core::MetaData* data) { globalData = data; }
    
    // Static methods
    static void Run(const std::string& path, CommandInputArgs* cinputArgs);

private:
    static std::string projectPath;
    static ProjectData* m_Data;
    static EUseDefineType useDefineNamespaceType;
    static bool useGenMetaClass;
    static bool compileUseTemplateClassGenClassFunction;
    static bool isUseNamespaceSearch;
    static bool isUseForceSemiColonInLineEnd;
    static bool isFirstPosMustUseThisBaseOrStaticClassName;
    static std::string rootPath;
    static Core::MetaData* globalData;
};

} // namespace Parse
} // namespace SimpleLanguage