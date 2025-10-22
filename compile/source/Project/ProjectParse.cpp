#include "ProjectParse.h"
#include "ProjectData.h"
#include "../Compile/FileMeta/FileMeta.h"
#include "../Compile/FileMeta/FileMetaClass.h"
#include "../Debug/Log.h"
#include "../Define.h"
#include "../Core/ModuleManager.h"
#include "../Core/NamespaceManager.h"
#include "../Core/ClassManager.h"
#include <iostream>

using namespace SimpleLanguage::Core;
using namespace SimpleLanguage::Compile;

namespace SimpleLanguage {
namespace Project {

ProjectParse::ProjectParse(FileMeta* fm, ProjectData* pd) 
    : m_FileMetaData(fm), m_ProjectData(pd) {
}

void ProjectParse::ParseProject() {
    if (m_FileMetaData == nullptr) {
        ::std::cout << "Error 没有解析成功Config.sp文件!!" << ::std::endl;
        return;
    }
    
    FileMetaClass* fmc = m_FileMetaData->GetFileMetaClassByName("ProjectConfig");
    if (fmc == nullptr) {
        ::std::cout << "Error 项目配置文件中没有找到ProjectConfig!!" << ::std::endl;
        return;
    }
    
    m_ProjectData->BindFileMetaClass(fmc);
    m_ProjectData->ParseFileMetaDataMemeberData(fmc);
    
    m_ProjectData->ParseMemberVariableDefineMetaType();
    m_ProjectData->ParseMemberFunctionDefineMetaType();
}

void ProjectParse::ParseGlobalVariable() {
    if (m_ProjectData == nullptr) {
        return;
    }
    m_ProjectData->ParseGlobalVariable();
}

void ProjectParse::BuildDefineNameStruct() {
    BuildDefineNameStructNode(m_ProjectData->namespaceRoot(), ModuleManager::GetInstance().SelfModule()->metaNode());
}

void ProjectParse::BuildDefineNameStructNode(DefineStruct* ds, MetaNode* parentMb) {
    MetaNode* newParentMB = nullptr;
    if (ds->type() != DefineStruct::EDefineStructType::Module) {
        if (parentMb->isMetaModule()) {
            if (ds->type() == DefineStruct::EDefineStructType::Namespace) {
                MetaNamespace* mn = new MetaNamespace(ds->name());
                newParentMB = parentMb->AddMetaNamespace(mn);
            } else if (ds->type() == DefineStruct::EDefineStructType::Class) {
                MetaClass* mc = new MetaClass(ds->name(), SimpleLanguage::Core::EClassDefineType::StructDefine);
                newParentMB = parentMb->AddMetaClass(mc);
            }
        } else if (parentMb->isMetaNamespace()) {
            if (ds->type() == DefineStruct::EDefineStructType::Namespace) {
                MetaNamespace* mn = new MetaNamespace(ds->name());
                newParentMB = parentMb->AddMetaNamespace(mn);
            } else if (ds->type() == DefineStruct::EDefineStructType::Class) {
                MetaClass* mc = new MetaClass(ds->name(), SimpleLanguage::Core::EType::Class);
                newParentMB = parentMb->AddMetaClass(mc);
            }
        } else if (parentMb->IsMetaClass()) {
            if (ds->type() == DefineStruct::EDefineStructType::Namespace) {
                std::cout << "Error 不能在class中，添加namespace!" << std::endl;
                return;
            } else if (ds->type() == DefineStruct::EDefineStructType::Class) {
                MetaClass* mc = new MetaClass(ds->name(), SimpleLanguage::Core::EType::Class);
                newParentMB = parentMb->AddMetaClass(mc);
            }
        }
    } else {
        newParentMB = parentMb;
    }

    for (size_t i = 0; i < ds->childDefineStruct().size(); i++) {
        BuildDefineNameStructNode(ds->childDefineStruct()[i], newParentMB);
    }
}

void ProjectParse::ParseDefineComplete() {
    if (m_ProjectData == nullptr) {
        return;
    }
    m_ProjectData->ParseDefineComplete();
}

} // namespace Project
} // namespace SimpleLanguage
