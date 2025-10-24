//****************************************************************************
//  File:      ProjectManager.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/30 12:00:00
//  Description:  manager project enter and compile 
//****************************************************************************


#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "ProjectManager.h"
#include "ProjectData.h"
#include "ProjectCompile.h"
#include "ProjectClass.h"
#include "../Core/MetaData.h"

using namespace SimpleLanguage::Core;

namespace SimpleLanguage {
namespace Project {
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

// ProjectManager static members initialization
std::string ProjectManager::projectPath = "";
ProjectData* ProjectManager::m_Data = new ProjectData("ProjectData", false);
EUseDefineType ProjectManager::useDefineNamespaceType = EUseDefineType::NoUseProjectConfigNamespace;
bool ProjectManager::useGenMetaClass = false;
bool ProjectManager::compileUseTemplateClassGenClassFunction = false;
bool ProjectManager::isUseNamespaceSearch = true;
bool ProjectManager::isUseForceSemiColonInLineEnd = false;
bool ProjectManager::isFirstPosMustUseThisBaseOrStaticClassName = false;
std::string ProjectManager::rootPath = "";
Core::MetaData* ProjectManager::globalData = new Core::MetaData("global", false, true, false);

ProjectData* ProjectManager::GetData() {
    return m_Data;
}

void ProjectManager::Run(const std::string& path, CommandInputArgs* cinputArgs) {
    size_t index = path.find_last_of("\\");
    if (index != std::string::npos) {
        rootPath = path.substr(0, index);
    }

    ProjectCompile::Compile(path, m_Data);

    if (!cinputArgs->GetIsTest()) {
        ProjectClass::RunMain();
    } else {
        ProjectClass::RunTest();
    }
}

} // namespace Parse
} // namespace SimpleLanguage