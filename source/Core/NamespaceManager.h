//****************************************************************************
//  File:      NamespaceManager.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include <map>
#include <vector>
#include <string>
#include "MetaNamespace.h"
#include "MetaNode.h"
#include "ModuleManager.h"
#include "../Compile/FileMeta/FileMetaNamespace.h"
#include "../Compile/FileMeta/FileMetaCommon.h"
#include "../Compile/CompilerUtil.h"
#include "../Project/ProjectManager.h"
#include "../Debug/Log.h"

namespace SimpleLanguage {
namespace Core {

class NamespaceManager {
public:
    static NamespaceManager* s_Instance;
    static NamespaceManager* GetInstance();

    std::map<std::string, MetaNamespace*> metaNamespaceDict;

public:
    NamespaceManager();
    ~NamespaceManager();
    
    MetaNode* SearchTopLevelFileMetaNamespace(SimpleLanguage::Compile::FileMetaNamespace* fns, MetaNode* parentNode = nullptr);
    MetaNode* GetParentChildrenNode(SimpleLanguage::Compile::FileMetaNamespace* fns, MetaNode* parentNode);
    MetaNode* SearchFinalNamespace(SimpleLanguage::Compile::FileMetaNamespace* fns);
    void CreateMetaNamespaceByFineDefineNamespace(SimpleLanguage::Compile::FileMetaNamespace* fns, MetaNode* parentNode = nullptr);
    void CreateMetaNamespaceByFileMetaNamespace(SimpleLanguage::Compile::FileMetaNamespace* fmn);
    MetaNode* FindFinalMetaNamespaceByNSBlock(SimpleLanguage::Compile::NamespaceStatementBlock* nsb, MetaNode* root = nullptr);
    void AddNamespaceString(const std::string& nsString);
    static MetaBase* FindMetaBaseByNamespaceToParentAndName(MetaNamespace* mn, const std::string& nodeName);

private:
    void CreateMetaNamespaceHandle(SimpleLanguage::Compile::FileMetaNamespace* fns, MetaNode* parentNode = nullptr);
};

} // namespace Core
} // namespace SimpleLanguage
