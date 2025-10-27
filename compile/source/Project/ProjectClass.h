#pragma once


#include "../Define.h"
#include "../Debug/Log.h"
#include "ProjectData.h"
#include <string>
#include <vector>

namespace SimpleLanguage {
    namespace Core
    {
        class MetaClass;
        class MetaFunction;
        class MetaNode;
        class MetaNamespace;
    }
namespace Project {

// Forward declarations
class DefineStruct;
class CompileFilterData;
class CompileFileData;

class ProjectClass {
public:
    static Core::MetaClass* projectEnter;
    static Core::MetaClass* projectDll;
    static Core::MetaClass* compile;
           
    static Core::MetaFunction* s_MainFunction;
    static Core::MetaFunction* s_TestFunction;
    static Core::MetaFunction* s_LoadStartFunction;
    static Core::MetaFunction* s_LoadEndFunction;
    static Core::MetaFunction* s_CompileBeforeFunction;
    static Core::MetaFunction* s_CompileAfterFunction;

    // Methods
    static void ParseCompileClass();
    static void RunTest();
    static void RunMain();
    static void AddDefineNamespace(Core::MetaNode* parentRoot, DefineStruct* dns, bool isAddCurrent = true);
    static void ProjectCompileBefore();
    static bool IsCanAddFile(CompileFilterData* cfd, CompileFileData::CompileFileDataUnit* fileData);
    static void ProjectCompileAfter();

private:
    ProjectClass() = default;
    ~ProjectClass() = default;
};

} // namespace Project
} // namespace SimpleLanguage
