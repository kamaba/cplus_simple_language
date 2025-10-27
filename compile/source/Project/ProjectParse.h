#pragma once

#include "../Define.h"
#include "../Debug/Log.h"
#include <string>
#include <vector>

namespace SimpleLanguage {
    namespace Compile
    {
        class FileMeta;
    }
    namespace Core {
        class MetaMemberData;
        class MetaNode;
        class MetaNamespace;
        class MetaClass;
    }
namespace Project {

// Forward declarations
class ProjectData;
class DefineStruct;

class ProjectParse {
public:
    ProjectParse(Compile::FileMeta* fm, ProjectData* pd);
    virtual ~ProjectParse() = default;

    void ParseProject();
    void ParseGlobalVariable();
    void BuildDefineNameStruct();
    void BuildDefineNameStructNode(DefineStruct* ds, Core::MetaNode* parentMb);
    void ParseDefineComplete();

private:
    ProjectData* m_ProjectData = nullptr;
    Compile::FileMeta* m_FileMetaData = nullptr;
};

} // namespace Project
} // namespace SimpleLanguage
