#include "ProjectClass.h"
#include "ProjectCompile.h"
#include "ProjectData.h"
#include "ProjectManager.h"
#include "../Compile/FileMeta/FileMetaClass.h"
#include "../Compile/FileMeta/FileMeta.h"
#include "../Core/ClassManager.h"
#include "../Core/ModuleManager.h"
#include "../Core/NamespaceManager.h"
#include "../Core/MetaMemberFunction.h"
#include "../Core/ModuleManager.h"
#include "../Core/MetaModule.h"
#include "../Core/BaseMetaClass/CoreMetaClassManager.h"
#include "../Debug/Log.h"
#include "../Define.h"
#include <iostream>

using namespace SimpleLanguage::Core;

namespace SimpleLanguage {
namespace Project {

// Static member definitions
MetaClass* ProjectClass::projectEnter = nullptr;
MetaClass* ProjectClass::projectDll = nullptr;
MetaClass* ProjectClass::compile = nullptr;

MetaFunction* ProjectClass::s_MainFunction = nullptr;
MetaFunction* ProjectClass::s_TestFunction = nullptr;
MetaFunction* ProjectClass::s_LoadStartFunction = nullptr;
MetaFunction* ProjectClass::s_LoadEndFunction = nullptr;
MetaFunction* ProjectClass::s_CompileBeforeFunction = nullptr;
MetaFunction* ProjectClass::s_CompileAfterFunction = nullptr;

void ProjectClass::ParseCompileClass() {
    Compile::FileMetaClass* fmc = ProjectCompile::ProjectFileMeta()->GetFileMetaClassByName("Compile");

    if (fmc == nullptr) return;

    Core::ClassManager::GetInstance().AddClass(fmc);
    
    compile = fmc->GetMetaClass();
    if (compile == nullptr) return;

    compile->ParseDefineComplete();
    auto flist = compile->GetStaticMetaMemberFunctionList();
    for (size_t i = 0; i < flist.size(); i++) {
        flist[i]->ParseStatements();
    }
}

void ProjectClass::RunTest() {
    MetaClass* project = ClassManager::GetInstance().GetClassByName("Project");
    if (project == nullptr) {
        std::cout << "Error project!!" << std::endl;
        return;
    }
    MetaMemberFunction* mmf = project->GetFirstMetaMemberFunctionByName("Test");
    if (mmf == nullptr) {
        std::cout << "Error project.Main函数!!" << std::endl;
        return;
    }
    // Implementation for running test
}

void ProjectClass::RunMain() {
    MetaClass* projectEntoer = ClassManager::GetInstance().GetClassByName("S.Project", 0);
    if (projectEntoer == nullptr) {
        std::cout << "Error 没有找到Project!!" << std::endl;
        return;
    }
    MetaMemberFunction* mmf = projectEntoer->GetFirstMetaMemberFunctionByName("Main");
    if (mmf == nullptr) {
        std::cout << "Error 没有找到Project.Main函数!!" << std::endl;
        return;
    }
   /* auto irmethod = IRManager::instance().GetIRMethod(mmf->functionAllName());
    InnerCLRRuntimeVM::Init();
    InnerCLRRuntimeVM::RunIRMethod(nullptr, irmethod);*/
}

void ProjectClass::AddDefineNamespace(MetaNode* parentRoot, DefineStruct* dns, bool isAddCurrent) {
    if (parentRoot == nullptr) return;

    MetaNode* parMS = nullptr;
    if (dns != nullptr) {
        MetaNamespace* nodeNS = nullptr;
        if (isAddCurrent) {
            auto cfindNode = parentRoot->GetChildrenMetaNodeByName(dns->name());
            if (cfindNode == nullptr) {
                if (dns->type() == DefineStruct::EDefineStructType::Class) {
                    auto gcmc = CoreMetaClassManager::GetCoreMetaClass(dns->name());
                    if (gcmc != nullptr) {
                        parMS = parentRoot->AddMetaClass(gcmc->GetMetaClassByTemplateCount(0));
                    } else {
                        auto nodens = new MetaClass(dns->name(), SimpleLanguage::Core::EClassDefineType::StructDefine);
                        parMS = parentRoot->AddMetaClass(nodens);
                    }
                } else {
                    nodeNS = new MetaNamespace(dns->name());
                    parMS = parentRoot->AddMetaNamespace(nodeNS);
                }
            } else {
                if (!(cfindNode->IsMetaNamespace())) {
                    //Log::AddInStructMeta(SimpleLanguage::Debug::EError::None, "Error 解析namespace添加命名空间节点时，发现已有定义类!!");
                    return;
                }
                nodeNS = cfindNode->GetMetaNamespace();
                parMS = parentRoot->AddMetaNamespace(nodeNS);
            }
        } else {
            parMS = parentRoot;
        }
        for (size_t i = 0; i < dns->childDefineStruct().size(); i++) {
            AddDefineNamespace(parMS, dns->childDefineStruct()[i]);
        }
    }
}

void ProjectClass::ProjectCompileBefore() {
    NamespaceManager::GetInstance().metaNamespaceDict.clear();

    ProjectData* data = ProjectManager::GetData();
    AddDefineNamespace(ModuleManager::GetInstance().GetSelfModule()->GetMetaNode(), data->namespaceRoot(), false);

    auto fileList = data->compileFileData()->compileFileDataUnitList();
    auto filter = data->compileFilterData();

    for (size_t i = 0; i < fileList.size(); i++) {
        auto fld = fileList[i];

        if (IsCanAddFile(filter, fld)) {
            ProjectCompile::AddFileParse(fld->path());
        }
    }

    if (s_CompileBeforeFunction != nullptr) {
        // Implementation for compile before function
    }
}

bool ProjectClass::IsCanAddFile(CompileFilterData* cfd, CompileFileData::CompileFileDataUnit* fileData) {
    if (cfd == nullptr) return true;
    if (!cfd->IsIncludeInGroup(fileData->group())) {
        return false;
    }
    if (!cfd->IsIncludeInTag(fileData->tag())) {
        return false;
    }
    return true;
}

void ProjectClass::ProjectCompileAfter() {
    if (s_CompileBeforeFunction != nullptr) {
        // Implementation for compile after function
    }
}

} // namespace Project
} // namespace SimpleLanguage
