#include "ProjectCompile.h"
#include "ProjectData.h"
#include "ProjectParse.h"
#include "ProjectClass.h"
#include "../Compile/FileMeta/FileMeta.h"
#include "../Compile/FileMeta/FileMetaClass.h"
#include "../Compile/Parse/FileParse.h"
#include "../Compile/Parse/LexerParse.h"
#include "../Compile/Parse/TokenParse.h"
#include "../Compile/Parse/StructParse.h"
#include "../Debug/Log.h"
#include "../Define.h"
#include "../Core/ClassManager.h"
#include "../Core/MetaVariableManager.h"
#include "../Core/MethodManager.h"
#include "../Core/MetaModule.h"
#include "../Core/ModuleManager.h"
#include "../Core/NamespaceManager.h"
#include "../Core/BaseMetaClass/CoreMetaClassManager.h"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace SimpleLanguage::Compile;
using namespace SimpleLanguage::Core;

namespace SimpleLanguage {
namespace Project {

// Static member definitions
bool ProjectCompile::s_IsLoaded = false;
std::string ProjectCompile::s_FileContentString = "";
FileMeta* ProjectCompile::s_ProjectFile = nullptr;
LexerParse* ProjectCompile::s_LexerParse = nullptr;
TokenParse* ProjectCompile::s_TokenParse = nullptr;
StructParse* ProjectCompile::s_ProjectBuild = nullptr;
ProjectParse* ProjectCompile::s_ProjectParse = nullptr;
ProjectData* ProjectCompile::s_Data = nullptr;
int ProjectCompile::s_StructParseCount = 0;
int ProjectCompile::s_BuildParseCount = 0;
int ProjectCompile::s_GrammerParseCount = 0;
int ProjectCompile::s_ParseListCount = 0;
std::vector<Compile::FileParse*> ProjectCompile::s_FileParseList;

ProjectCompile::ProjectCompile() {
}

void ProjectCompile::LoadProject() {
    ::std::filesystem::path directory(ProjectManager::GetProjectPath());

    if (!::std::filesystem::exists(directory)) {
        std::cout << "Error 项目加载路径不正确!!" << std::endl;
    }
    if (!std::filesystem::is_directory(directory))
    {
        std::cout << "Error 项目加载路径不正确2!!" << std::endl;
    }

    std::vector<std::string> paths;
    for (const auto& entry : ::std::filesystem::directory_iterator(directory)) {
        if (entry.path().extension() == ".sp") {
            paths.push_back(entry.path().string());
        }
    }
    
    if (paths.empty()) {
        std::cout << "Error 项目加载路径没有找到sp文件!!" << std::endl;
    }

    if (!::std::filesystem::exists(paths[0])) {
        std::cout << "Error 项目加载路径不正确!!" << std::endl;
        return;
    }
    std::string path = paths[0];

    s_ProjectFile = new FileMeta(path);

    std::ifstream file(path, std::ios::binary);
    if (file.is_open()) {
        std::ostringstream buffer;
        buffer << file.rdbuf();
        s_FileContentString = buffer.str();
        file.close();
    }

    s_LexerParse = new LexerParse(path, s_FileContentString);
    s_LexerParse->ParseToTokenList();

    const std::vector<Token*> vtoken = s_LexerParse->GetListTokensWidthEnd();
    s_TokenParse = new TokenParse(s_ProjectFile, vtoken);
    s_TokenParse->BuildStruct();

    s_ProjectBuild = new StructParse(s_ProjectFile, s_TokenParse->GetRootNode());
    s_ProjectBuild->ParseRootNodeToFileMeta();

    s_ProjectParse = new ProjectParse(s_ProjectFile, s_Data);
    s_ProjectParse->ParseProject();

    s_ProjectFile->SetDeep(0);

    ProjectClass::ParseCompileClass();

    std::cout << s_ProjectFile->ToFormatString() << std::endl;
}

void ProjectCompile::Compile(const std::string& path, ProjectData* pd) {
    if (!s_IsLoaded) {
        ProjectManager::SetProjectPath( path );
        s_Data = pd;
        s_IsLoaded = true;
        LoadProject();
    }

    ProjectClass::ProjectCompileBefore();

    CoreMetaClassManager::GetInstance().Init();

    s_StructParseCount = 0;
    s_BuildParseCount = 0;
    s_GrammerParseCount = 0;

    s_ParseListCount = s_FileParseList.size();

    FileListStructParse();

    ClassManager::GetInstance().AddMetaClass(ProjectManager::GetGlobalData() );

    ProjectClass::ProjectCompileAfter();

    std::cout << ModuleManager::GetInstance().GetSelfModule()->GetMetaNode()->ToFormatString() << std::endl;

    //IRManager::instance().TranslateIR();
}

void ProjectCompile::AddFileParse(const std::string& path) {
   /* auto fp = new FileParse(std::filesystem::path(ProjectManager::rootPath) / path, new ParseFileParam());
    fp->setStructParseComplete(StructParseComplete);
    fp->setBuildParseComplete(BuildParseComplete);
    fp->setGrammerParseComplete(GrammerParseComplete);
    s_FileParseList.push_back(fp);*/
}

bool ProjectCompile::CheckFileList() {
    bool isSuccess = true;
    for (size_t i = 0; i < s_FileParseList.size(); i++) {
        if (!s_FileParseList[i]->IsExists()) {
            isSuccess = false;
            std::cout << "û���ҵ�Ҫ������ļ�: " << s_FileParseList[i]->filePath << std::endl;
            break;
        }
    }
    return isSuccess;
}

void ProjectCompile::FileListStructParse() {
    if (!CheckFileList()) return;
    for (size_t i = 0; i < s_FileParseList.size(); i++) {
        s_FileParseList[i]->HandleStructParse();

        //Log::AddProcess(SimpleLanguage::Core::EProcess::StructMeta, SimpleLanguage::Debug::EError::StructFileMetaEnd, 
        //               s_FileParseList[i]->ToFormatString());
    }
}

void ProjectCompile::StructParseComplete() {
    s_StructParseCount++;
    if (s_StructParseCount >= s_ParseListCount) {
        CompileFileAllEnd();
    }
}

void ProjectCompile::BuildParseComplete() {
    s_BuildParseCount++;
    if (s_BuildParseCount < s_ParseListCount) {
        return;
    }
}

void ProjectCompile::GrammerParseComplete() {
    s_GrammerParseCount++;
    if (s_GrammerParseCount < s_ParseListCount)
        return;

    std::cout << "" << std::endl;
}

void ProjectCompile::Update() {
    // Implementation for timer update
}

void ProjectCompile::CompileFileAllEnd() {
    for (size_t i = 0; i < s_FileParseList.size(); i++) {
        s_FileParseList[i]->CreateNamespace();
    }
    
    for (size_t i = 0; i < s_FileParseList.size(); i++) {
        s_FileParseList[i]->CombineFileMeta();
    }

    for (size_t i = 0; i < s_ProjectFile->FileMetaClassList().size(); i++) {
        auto fns = s_ProjectFile->FileMetaClassList()[i];

        if (fns->GetName() == "ProJectConfig" || fns->GetName() == "Compile") {
            continue;
        }
        ClassManager::GetInstance().AddClass(fns);
    }

    ClassManager::GetInstance().ParseInitMetaClassList();
    s_ProjectParse->ParseGlobalVariable();

    ClassManager::GetInstance().HandleExtendMember();
    ClassManager::GetInstance().CheckInterfaces();
    ClassManager::GetInstance().ParseDefineComplete();

    ClassManager::GetInstance().ParseMemberEnumExpress();
    MetaVariableManager::Instance().ParseMetaDataMemberExpress();
    MetaVariableManager::Instance().ParseMetaClassMemberExpress();

    MethodManager::GetInstance().ParseStatements();

    ModuleManager::GetInstance().GetSelfModule()->GetMetaNode()->SetDeep(0);

    ClassManager::GetInstance().PrintAlllClassContent();

    //Log::PrintLog();

    std::cout << "-------------------------������ɺ�ĸ�ʽ��� ��ʼ--------------------------" << std::endl;
    std::cout << ModuleManager::GetInstance().GetSelfModule()->GetMetaNode()->ToFormatString() << std::endl;
    std::cout << "-------------------------������ɺ�ĸ�ʽ��� ����--------------------------" << std::endl;
}

} // namespace Project
} // namespace SimpleLanguage
