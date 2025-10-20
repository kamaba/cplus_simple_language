#pragma once

#include "../Define.h"
#include "../Debug/Log.h"
#include <string>
#include <vector>

class SimpleLanguage::Compile::FileMeta;

namespace SimpleLanguage {
namespace Project {

// Forward declarations
class MetaMemberData;
class MetaNode;
class MetaNamespace;
class MetaClass;
class ProjectData;

class ProjectParse {
public:
    ProjectParse(FileMeta* fm, ProjectData* pd);
    virtual ~ProjectParse() = default;

    void ParseProject();
    void ParseGlobalVariable();
    void BuildDefineNameStruct();
    void BuildDefineNameStructNode(DefineStruct* ds, MetaNode* parentMb);
    void ParseDefineComplete();

private:
    ProjectData* m_ProjectData = nullptr;
    FileMeta* m_FileMetaData = nullptr;
};

} // namespace Project
} // namespace SimpleLanguage
