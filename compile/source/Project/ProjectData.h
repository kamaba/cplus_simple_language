#pragma once

#include "../Define.h"
#include "../Core/MetaData.h"
#include <string>
#include <vector>
#include <unordered_map>


namespace SimpleLanguage {
namespace Project {

// Forward declarations
class MetaMemberData;
class MetaVariable;
class FileMetaClass;

class CompileFileData {
public:
    enum class ECompileState {
        Default,
        Ignore
    };

    class CompileFileDataUnit {
    public:
        CompileFileDataUnit(MetaMemberData* mmd);
        
        std::string path() const { return m_Path; }
        void setPath(const std::string& value) { m_Path = value; }
        
        std::string group() const { return m_Group; }
        void setGroup(const std::string& value) { m_Group = value; }
        
        std::string tag() const { return m_Tag; }
        void setTag(const std::string& value) { m_Tag = value; }
        
        ECompileState compileState() const { return m_CompileState; }
        void setCompileState(ECompileState value) { m_CompileState = value; }
        
        int priority() const { return m_Priority; }
        void setPriority(int value) { m_Priority = value; }

    private:
        std::string m_Path;
        std::string m_Group;
        std::string m_Tag;
        ECompileState m_CompileState = ECompileState::Default;
        int m_Priority = 0;
    };

    std::vector<CompileFileDataUnit*> compileFileDataUnitList() const { return m_CompileFileDataUnitList; }
    void Parse(MetaMemberData* mmd);

private:
    std::vector<CompileFileDataUnit*> m_CompileFileDataUnitList;
};

class CompileOptionData {
public:
    bool isForceUseClassKey() const { return m_IsForceUseClassKey; }
    void setForceUseClassKey(bool value) { m_IsForceUseClassKey = value; }
    
    bool isSupportDoublePlus() const { return m_IsSupportDoublePlus; }
    void setSupportDoublePlus(bool value) { m_IsSupportDoublePlus = value; }

    void Parse(MetaMemberData* mmd);

private:
    bool m_IsForceUseClassKey = false;
    bool m_IsSupportDoublePlus = false;
};

class CompileFilterData {
public:
    const std::vector<std::string>& groupList() const { return m_GroupList; }
    const std::vector<std::string>& tagList() const { return m_TagList; }
    
    bool isAllGroup() const { return m_IsAllGroup; }
    void setAllGroup(bool value) { m_IsAllGroup = value; }
    
    bool isAllTag() const { return m_IsAllTag; }
    void setAllTag(bool value) { m_IsAllTag = value; }

    bool IsIncludeInGroup(const std::string& group);
    bool IsIncludeInTag(const std::string& tag);
    bool Parse(MetaMemberData* mmd);

private:
    std::vector<std::string> m_GroupList;
    std::vector<std::string> m_TagList;
    bool m_IsAllGroup = false;
    bool m_IsAllTag = false;
};

class ImportModuleData {
public:
    class ImportModuleDataUnit {
    public:
        std::string moduleName() const { return m_ModuleName; }
        void setModuleName(const std::string& value) { m_ModuleName = value; }
        
        std::string modulePath() const { return m_ModulePath; }
        void setModulePath(const std::string& value) { m_ModulePath = value; }

    private:
        std::string m_ModuleName = "";
        std::string m_ModulePath = "";
    };

    const std::vector<ImportModuleDataUnit*>& importModuleDataUnitList() const { return m_ImportModuleDataUnitList; }
    bool Parse(MetaMemberData* mmd);

private:
    std::vector<ImportModuleDataUnit*> m_ImportModuleDataUnitList;
};

class MetaMemberVariable;
class CompileModuleData {
public:
    class CompileModuleDataUnit {
    public:
        std::string name() const { return m_Name; }
        void setName(const std::string& value) { m_Name = value; }
        
        std::string path() const { return m_Path; }
        void setPath(const std::string& value) { m_Path = value; }

    private:
        std::string m_Name;
        std::string m_Path;
    };

    void Parse(MetaMemberVariable* mmd);

private:
    std::vector<CompileModuleDataUnit*> m_CompileModuleDataUnitList;
};

class DefineStruct {
public:
    enum class EDefineStructType {
        Module,
        Namespace,
        Class,
        Data,
        Enum
    };

    DefineStruct(EDefineStructType edst);
    
    std::string name() const { return m_Name; }
    void setName(const std::string& value) { m_Name = value; }
    
    EDefineStructType type() const { return m_Type; }
    void setType(EDefineStructType value) { m_Type = value; }
    
    const std::vector<DefineStruct*>& childDefineStruct() const { return m_ChildDefineStruct; }

    DefineStruct* Parse(MetaMemberData* mmd);

private:
    std::string m_Name = "";
    EDefineStructType m_Type = EDefineStructType::Namespace;
    std::vector<DefineStruct*> m_ChildDefineStruct;
};

class GlobalVariableData {
public:
    const std::unordered_map<std::string, MetaVariable*>& variableDict() const { return m_VariableDict; }
    void Parse(MetaMemberData* mmd);

private:
    std::unordered_map<std::string, MetaVariable*> m_VariableDict;
};

class GlobalImportData {
public:
    const std::vector<std::string>& globalImportList() const { return m_GlobalImportList; }
    void Parse(MetaMemberData* mmd);

private:
    std::vector<std::string> m_GlobalImportList;
};

class GlobalReplaceData {
public:
    void Parse(MetaMemberData* mmd);

private:
    std::unordered_map<std::string, std::string> m_ReplaceStringDict;
};

class ExportDllData {
public:
    std::string spaceName() const { return m_SpaceName; }
    void setSpaceName(const std::string& value) { m_SpaceName = value; }

    void Parse(MetaMemberData* mmd);

private:
    std::string m_SpaceName = "";
};

class MemorySetData {
public:
    void Parse(MetaMemberData* mmd);
};

class ProjectData : public SimpleLanguage::Core::MetaData {
public:
    ProjectData(const std::string& _name, bool isConst);
    
    std::string projectName() const { return m_ProjectName; }
    void setProjectName(const std::string& value) { m_ProjectName = value; }
    
    std::string projectDesc() const { return m_ProjectDesc; }
    void setProjectDesc(const std::string& value) { m_ProjectDesc = value; }
    
    int mainVersion() const { return m_MainVersion; }
    void setMainVersion(int value) { m_MainVersion = value; }
    
    int subVersion() const { return m_SubVersion; }
    void setSubVersion(int value) { m_SubVersion = value; }
    
    int buildVersion() const { return m_BuildVersion; }
    void setBuildVersion(int value) { m_BuildVersion = value; }
    
    int buildSubVersion() const { return m_BuildSubVersion; }
    void setBuildSubVersion(int value) { m_BuildSubVersion = value; }
    
    bool isUseForceSemiColonInLineEnd() const { return m_IsUseForceSemiColonInLineEnd; }
    void setUseForceSemiColonInLineEnd(bool value) { m_IsUseForceSemiColonInLineEnd = value; }
    
    CompileFileData* compileFileData() const { return m_CompileFileData; }
    CompileOptionData* compileOptionData() const { return m_CompileOptionData; }
    CompileFilterData* compileFilterData() const { return m_CompileFilterData; }
    DefineStruct* namespaceRoot() const { return m_NamespaceRoot; }
    GlobalImportData* globalImportData() const { return m_GlobalImportData; }
    GlobalReplaceData* globalReplaceData() const { return m_GlobalReplaceData; }
    GlobalVariableData* globalVariableData() const { return m_GlobalVariableData; }
    ExportDllData* exportDllData() const { return m_ExportDllData; }
    CompileModuleData* compileModuleData() const { return m_CompileModuleData; }
    ImportModuleData* importModuleData() const { return m_ImportModuleData; }
    MemorySetData* memorySetData() const { return m_MemorySetData; }

    virtual void ParseFileMetaDataMemeberData(FileMetaClass* fmc);
    void ParseBlockNode(MetaMemberData* mmd);
    void ParseGlobalVariable();
    bool IsIncludeDefineStruct(const std::vector<std::string>& liststr);

private:
    std::string m_ProjectName;
    std::string m_ProjectDesc;
    int m_MainVersion = 0;
    int m_SubVersion = 0;
    int m_BuildVersion = 0;
    int m_BuildSubVersion = 0;
    bool m_IsUseForceSemiColonInLineEnd = true;
    CompileFileData* m_CompileFileData = new CompileFileData();
    CompileOptionData* m_CompileOptionData = new CompileOptionData();
    CompileFilterData* m_CompileFilterData = new CompileFilterData();
    DefineStruct* m_NamespaceRoot = new DefineStruct(DefineStruct::EDefineStructType::Module);
    GlobalImportData* m_GlobalImportData = new GlobalImportData();
    GlobalReplaceData* m_GlobalReplaceData = new GlobalReplaceData();
    GlobalVariableData* m_GlobalVariableData = new GlobalVariableData();
    ExportDllData* m_ExportDllData = new ExportDllData();
    CompileModuleData* m_CompileModuleData = new CompileModuleData();
    ImportModuleData* m_ImportModuleData = new ImportModuleData();
    MemorySetData* m_MemorySetData = new MemorySetData();
};

} // namespace Project
} // namespace SimpleLanguage
