#pragma once

#include "../Core/MetaData.h"
#include "../Define.h"
#include <string>
#include <vector>

namespace SimpleLanguage {
namespace Parse {

class CommandInputArgs {
public:
    CommandInputArgs(const char* args);
    CommandInputArgs(const std::vector<std::string>& args);
    
    bool isTest() const { return m_IsTest; }
    void setTest(bool value) { m_IsTest = value; }
    
    bool isPrintToken() const { return m_IsPrintToken; }
    void setPrintToken(bool value) { m_IsPrintToken = value; }

private:
    bool m_IsTest = false;
    bool m_IsPrintToken = false;
};

enum class EUseDefineType {
    NoUseProjectConfigNamespace,        // 不使用项目内部配置
    LimitUseProjectConfigNamespace,     // 限制使用配置后的命名空间 类自由创作
    LimitUseProjectConfigNamespaceAndClass, // 限制使用配置后的命名空间与类
};

class ProjectManager {
public:
    static std::string projectPath;
    static class ProjectData* data();
    static EUseDefineType useDefineNamespaceType;
    
    static bool useGenMetaClass;
    static bool compileUseTemplateClassGenClassFunction;
    static bool isUseNamespaceSearch;
    static bool isUseForceSemiColonInLineEnd;
    static bool isFirstPosMustUseThisBaseOrStaticClassName;
    
    static std::string rootPath;
    static class MetaData* globalData;
    
    static void Run(const std::string& path, CommandInputArgs* cinputArgs);

private:
    static class ProjectData* m_Data;
};

} // namespace Parse
} // namespace SimpleLanguage
