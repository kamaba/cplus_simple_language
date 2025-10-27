#include "ProjectData.h"
#include "ProjectManager.h"
#include "../Core/MetaData.h"
#include "../Core/MetaMemberData.h"
#include "../Core/MetaVariable.h"
#include "../Compile/FileMeta/FileMetaMemberData.h"
#include "../Compile/FileMeta/FileMetaClass.h"
#include "../Debug/Log.h"
#include "../Define.h"
#include <algorithm>
#include <iostream>
#include "../Compile/Parse/StructParse.h"

namespace SimpleLanguage {
namespace Project {

// CompileFileData::CompileFileDataUnit implementation
CompileFileData::CompileFileDataUnit::CompileFileDataUnit(Core::MetaMemberData* mmd) {
    std::string findPath = mmd->GetString("path", true);
    if (findPath.empty()) {
        std::cout << "Error ProjectConfig 中的CompileFileData缺少path字段!!" << std::endl;
        return;
    }
    m_Path = findPath;
    m_CompileState = ECompileState::Default;
}

void CompileFileData::Parse(Core::MetaMemberData* mmd) {
    for (auto& v : mmd->GetMetaMemberDataDict()) {
        m_CompileFileDataUnitList.push_back(new CompileFileDataUnit(v.second));
    }
}

// CompileOptionData implementation
void CompileOptionData::Parse(Core::MetaMemberData* mmd) {
    // Implementation for parsing compile options
}

// CompileFilterData implementation
bool CompileFilterData::IsIncludeInGroup(const std::string& group) {
    if (m_IsAllGroup) return true;
    if (m_GroupList.empty()) return true;
    return std::find(m_GroupList.begin(), m_GroupList.end(), group) != m_GroupList.end();
}

bool CompileFilterData::IsIncludeInTag(const std::string& tag) {
    if (m_IsAllTag) return true;
    if (m_TagList.empty()) return true;
    return std::find(m_TagList.begin(), m_TagList.end(), tag) != m_TagList.end();
}

bool CompileFilterData::Parse(Core::MetaMemberData* mmd) {
    if (mmd == nullptr) {
        return false;
    }
    return true;
}

// ImportModuleData implementation
bool ImportModuleData::Parse(Core::MetaMemberData* mmd) {
    return true;
}

// CompileModuleData implementation
void CompileModuleData::Parse(Core::MetaMemberVariable* mmd) {
    // Implementation for parsing compile module data
}

// DefineStruct implementation
DefineStruct::DefineStruct(EDefineStructType edst) : m_Type(edst) {
}

DefineStruct* DefineStruct::Parse(Core::MetaMemberData* mmd) {
    if (mmd == nullptr) {
        return nullptr;
    }
    m_Name = mmd->GetName();

    std::string usetype = "namespace";

    std::vector<Core::MetaMemberData*> mdChild;
    for (auto& ns : mmd->GetMetaMemberDataDict()) {
        Core::MetaMemberData* cmmd = dynamic_cast<Core::MetaMemberData*>(ns.second);
        if (cmmd != nullptr) {
            if (cmmd->GetName() == "type") {
                usetype = cmmd->GetString("type");

                if (usetype == "namespace") {
                    m_Type = EDefineStructType::Namespace;
                } else if (usetype == "class") {
                    m_Type = EDefineStructType::Class;
                } else {
                    std::cout << "Error 未识别的" << usetype << std::endl;
                    continue;
                }
            } else if (cmmd->GetName() == "child") {
                for (auto& ns2 : cmmd->GetMetaMemberDataDict()) {
                    Core::MetaMemberData* cmmd2 = ns2.second;
                    if (cmmd2 != nullptr) {
                        DefineStruct* ds = new DefineStruct(EDefineStructType::Namespace);
                        m_ChildDefineStruct.push_back(ds->Parse(cmmd2));
                    }
                }
            }
        }
    }
    return this;
}

// GlobalVariableData implementation
void GlobalVariableData::Parse(Core::MetaMemberData* mmd) {
    if (mmd == nullptr) {
        return;
    }
    // Implementation for parsing global variables
}

// GlobalImportData implementation
void GlobalImportData::Parse(Core::MetaMemberData* mmd) {
    if (mmd == nullptr) {
        return;
    }
    // Implementation for parsing global imports
}

// GlobalReplaceData implementation
void GlobalReplaceData::Parse(Core::MetaMemberData* mmd) {
    if (mmd == nullptr) {
        return;
    }
    // Implementation for parsing global replacements
}

// ExportDllData implementation
void ExportDllData::Parse(Core::MetaMemberData* mmd) {
    if (mmd == nullptr) {
        return;
    }
    return;
}

// MemorySetData implementation
void MemorySetData::Parse(Core::MetaMemberData* mmd) {
    if (mmd == nullptr) {
        return;
    }
    return;
}

// ProjectData implementation
ProjectData::ProjectData(const std::string& _name, bool isConst) : MetaData(_name, isConst, false, false) {
}

void ProjectData::ParseFileMetaDataMemeberData(Compile::FileMetaClass* fmc) {
    m_Deep = 0;
    for (size_t i = 0; i < fmc->GetMemberDataList().size(); i++) {
        auto v = fmc->GetMemberDataList()[i];
        
        if (v->GetName() == "globalVariable")
            continue;

        auto mmd = new Core::MetaMemberData(this, v, i, true);
        mmd->ParseDefineMetaType();
        mmd->ParseMetaExpress();
        AddMetaMemberData(mmd);
        mmd->ParseChildMemberData();

        ParseBlockNode(mmd);
    }
    int ct = this->GetMetaMemberDataDict().size();
    if (!fmc->GetMemberVariableList().empty() || !fmc->GetMemberFunctionList().empty()) {
        std::cout << "Error Data中不允许有Variable 和 Function!!" << std::endl;
    }
}

void ProjectData::ParseBlockNode(Core::MetaMemberData* mmd) {
    if (mmd == nullptr) return;

    std::string _name = mmd->GetName();
    if (_name == "name" || _name == "desc") {
        if (_name == "name") {
            m_ProjectName = mmd->GetString(_name);
        } else {
            m_ProjectDesc = mmd->GetString(_name);
        }
    } else if (_name == "mainVersion" || _name == "subVersion" || 
               _name == "buildVersion" || _name == "buildSubVersion") {
        if (_name == "mainVersion") {
            m_MainVersion = mmd->GetInt(_name);
        } else if (_name == "subVersion") {
            m_SubVersion = mmd->GetInt(_name);
        } else if (_name == "buildVersion") {
            m_BuildVersion = mmd->GetInt(_name);
        } else {
            m_BuildSubVersion = mmd->GetInt(_name);
        }
    } else if (_name == "compileFileList") {
        m_CompileFileData->Parse(mmd);
    } else if (_name == "compileOption") {
        m_CompileOptionData->Parse(mmd);
    } else if (_name == "compileFilter") {
        m_CompileFilterData->Parse(mmd);
    } else if (_name == "importModule") {
        m_ImportModuleData->Parse(mmd);
    } else if (_name == "globalVariable") {
        m_GlobalVariableData->Parse(mmd);
    } else if (_name == "proojectStruct") {
        for (auto& ns : mmd->GetMetaMemberDataDict()) {
            Core::MetaMemberData* cmmd = ns.second;
            if (cmmd != nullptr) {
                DefineStruct* ds = new DefineStruct(DefineStruct::EDefineStructType::Namespace);
                ds->Parse(cmmd);
                m_NamespaceRoot->childDefineStruct().push_back(ds);
            }
        }
    } else if (_name == "globalImport") {
        m_GlobalImportData->Parse(mmd);
    } else if (_name == "globalReplace") {
        m_GlobalReplaceData->Parse(mmd);
    } else if (_name == "exportDll") {
        m_ExportDllData->Parse(mmd);
    } else if (_name == "memorySet") {
        m_MemorySetData->Parse(mmd);
    }
}

void ProjectData::ParseGlobalVariable() {
    if (m_FileMetaClassDict.empty())
        return;
    
    Compile::FileMetaMemberData* fmd = nullptr;
    for (auto& v : m_FileMetaClassDict) {
        fmd = v.second->GetFileMemberData("globalVariable");
        if (fmd != nullptr)
            break;
    }

    if (fmd == nullptr) {
        return;
    }

    Core::MetaMemberData* mb = ProjectManager::GetGlobalData()->GetMemberDataByName(fmd->GetName());
    if (mb != nullptr) {
        std::cout << "Error ProjectParse ParseGlobalVariable中发现重复: " << GetAllClassName() << " 位置: " 
                  << (fmd->GetToken() != nullptr ? fmd->GetToken()->ToLexemeAllString() : "") << "元组!!" << std::endl;
        return;
    }
    
    auto mmd = new Core::MetaMemberData(ProjectManager::GetGlobalData(), fmd, ProjectManager::GetGlobalData()->GetMetaMemberDataDict().size(), true);
    mmd->ParseDefineMetaType();
    mmd->ParseMetaExpress();
    ProjectManager::GetGlobalData()->AddMetaMemberData(mmd);
}

bool ProjectData::IsIncludeDefineStruct(const std::vector<std::string>& liststr) {
    DefineStruct* dc = m_NamespaceRoot;
    for (size_t k = 0; k < liststr.size(); k++) {
        std::string nameByIndex = liststr[k];

        bool isFind = false;
        for (size_t i = 0; i < dc->childDefineStruct().size(); i++) {
            auto cdsc = dc->childDefineStruct()[i];
            if (cdsc->name() == nameByIndex) {
                isFind = true;
                dc = cdsc;
                break;
            }
        }
        if (!isFind) return false;
    }

    return true;
}

} // namespace Project
} // namespace SimpleLanguage
