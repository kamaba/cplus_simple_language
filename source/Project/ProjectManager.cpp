#include "ProjectManager.h"
#include "ProjectData.h"
#include "ProjectCompile.h"
#include "ProjectClass.h"
#include "../Core/MetaData.h"
#include "../Define.h"
#include <algorithm>
#include <iostream>

namespace SimpleLanguage {
namespace Project {

    CommandInputArgs::CommandInputArgs(const char* args)
    {

    }
// CommandInputArgs implementation
CommandInputArgs::CommandInputArgs(const std::vector<std::string>& args) {
    // Parse command line arguments
    for (const auto& arg : args) {
        if (arg == "--test" || arg == "-t") {
            m_IsTest = true;
        } else if (arg == "--print-token" || arg == "-p") {
            m_IsPrintToken = true;
        }
    }
}

// ProjectManager static members
std::string ProjectManager::projectPath = "";
ProjectData* ProjectManager::m_Data = new ProjectData("ProjectData", false);
EUseDefineType ProjectManager::useDefineNamespaceType = EUseDefineType::NoUseProjectConfigNamespace;
bool ProjectManager::useGenMetaClass = false;
bool ProjectManager::compileUseTemplateClassGenClassFunction = false;
bool ProjectManager::isUseNamespaceSearch = true;
bool ProjectManager::isUseForceSemiColonInLineEnd = false;
bool ProjectManager::isFirstPosMustUseThisBaseOrStaticClassName = false;
std::string ProjectManager::rootPath = "";
MetaData* ProjectManager::globalData = new MetaData("global", false, true, false);

ProjectData* ProjectManager::GetData() {
    return m_Data;
}

void ProjectManager::Run(const std::string& path, CommandInputArgs* cinputArgs) {
    size_t index = path.find_last_of("\\");
    if (index != std::string::npos) {
        rootPath = path.substr(0, index);
    }

    ProjectCompile::Compile(path, m_Data);

    if (!cinputArgs->isTest()) {
        ProjectClass::RunMain();
    } else {
        ProjectClass::RunTest();
    }
}

} // namespace Parse
} // namespace SimpleLanguage
