#pragma once


#include "../Define.h"
#include "../Debug/Log.h"
#include "ProjectData.h"
#include <string>
#include <vector>

namespace SimpleLanguage {
namespace Project {

// Forward declarations
class MetaClass;
class MetaFunction;
class MetaNode;
class MetaNamespace;
class DefineStruct;
class CompileFilterData;
class CompileFileData;

class ProjectClass {
public:
    static MetaClass* projectEnter;
    static MetaClass* projectDll;
    static MetaClass* compile;

    static MetaFunction* s_MainFunction;
    static MetaFunction* s_TestFunction;
    static MetaFunction* s_LoadStartFunction;
    static MetaFunction* s_LoadEndFunction;
    static MetaFunction* s_CompileBeforeFunction;
    static MetaFunction* s_CompileAfterFunction;

    // Methods
    static void ParseCompileClass();
    static void RunTest();
    static void RunMain();
    static void AddDefineNamespace(MetaNode* parentRoot, DefineStruct* dns, bool isAddCurrent = true);
    static void ProjectCompileBefore();
    static bool IsCanAddFile(CompileFilterData* cfd, CompileFileData::CompileFileDataUnit* fileData);
    static void ProjectCompileAfter();

private:
    ProjectClass() = default;
    ~ProjectClass() = default;
};

} // namespace Project
} // namespace SimpleLanguage
