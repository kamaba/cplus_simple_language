//****************************************************************************
//  File:      NamespaceManager.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "NamespaceManager.h"
#include "MetaNamespace.h"
#include "MetaNode.h"
#include "ModuleManager.h"
#include "../Compile/FileMeta/FileMetaNamespace.h"
#include "../Compile/FileMeta/FileMetaCommon.h"
#include "../Compile/CompilerUtil.h"
#include "../Project/ProjectManager.h"
#include "../Debug/Log.h"
#include <iostream>

namespace SimpleLanguage {
namespace Core {

// Static member initialization
NamespaceManager* NamespaceManager::s_Instance = nullptr;

NamespaceManager* NamespaceManager::GetInstance() {
    if (s_Instance == nullptr) {
        s_Instance = new NamespaceManager();
    }
    return s_Instance;
}

NamespaceManager::NamespaceManager() {
    // Constructor
}

NamespaceManager::~NamespaceManager() {
    // Clean up metaNamespaceDict
    for (auto& pair : metaNamespaceDict) {
        delete pair.second;
    }
    metaNamespaceDict.clear();
}

MetaNode* NamespaceManager::SearchTopLevelFileMetaNamespace(SimpleLanguage::Compile::CoreFileMeta::FileMetaNamespace* fns, MetaNode* parentNode) {
    MetaNode* findNode = parentNode;
    if (fns->topLevelFileMetaNamespace != nullptr) {
        findNode = SearchTopLevelFileMetaNamespace(fns->topLevelFileMetaNamespace, findNode);
        for (size_t i = 0; i < fns->namespaceStatementBlock()->tokenList().size(); i++) {
            std::string name = fns->namespaceStatementBlock()->tokenList()[i]->lexeme().ToString();
            MetaNode* findNode2 = findNode->GetChildrenMetaNodeByName(name);
            if (findNode2 == nullptr) {
                break;
            }
            findNode = findNode2;
        }
    }
    return findNode;
}

MetaNode* NamespaceManager::GetParentChildrenNode(SimpleLanguage::Compile::CoreFileMeta::FileMetaNamespace* fns, MetaNode* parentNode) {
    MetaNode* findNode = parentNode;
    for (size_t i = 0; i < fns->namespaceStatementBlock()->tokenList().size(); i++) {
        std::string name = fns->namespaceStatementBlock()->tokenList()[i]->lexeme().ToString();
        MetaNode* findNode2 = findNode->GetChildrenMetaNodeByName(name);
        if (findNode2 == nullptr) {
            break;
        }
        findNode = findNode2;
    }
    return findNode;
}

MetaNode* NamespaceManager::SearchFinalNamespace(SimpleLanguage::Compile::CoreFileMeta::FileMetaNamespace* fns) {
    MetaNode* findNode = ModuleManager::GetInstance()->GetSelfModule()->getMetaNode();

    std::vector<SimpleLanguage::Compile::CoreFileMeta::FileMetaNamespace*> list;

    SimpleLanguage::Compile::CoreFileMeta::FileMetaNamespace* PS_fmn = fns;
    list.push_back(PS_fmn);
    while (true) {
        if (PS_fmn->topLevelFileMetaNamespace != nullptr) {
            PS_fmn = PS_fmn->topLevelFileMetaNamespace;
            list.push_back(PS_fmn);
        } else {
            break;
        }
    }

    SimpleLanguage::Compile::CoreFileMeta::FileMetaNamespace* fmn = nullptr;
    for (int i = static_cast<int>(list.size()) - 1; i >= 0; i--) {
        fmn = list[i];
        findNode = GetParentChildrenNode(fmn, findNode);
        if (findNode == nullptr) {
            break;
        }
    }

    return findNode;
}

void NamespaceManager::CreateMetaNamespaceByFineDefineNamespace(SimpleLanguage::Compile::CoreFileMeta::FileMetaNamespace* fns, MetaNode* parentNode) {
    SimpleLanguage::Compile::CoreFileMeta::FileMetaNamespace* fnsc = fns;
    if (parentNode == nullptr) {
        parentNode = ModuleManager::GetInstance()->GetSelfModule()->getMetaNode();
    }
    parentNode = SearchTopLevelFileMetaNamespace(fns, parentNode);

    CreateMetaNamespaceHandle(fnsc, parentNode);
}

void NamespaceManager::CreateMetaNamespaceHandle(SimpleLanguage::Compile::CoreFileMeta::FileMetaNamespace* fns, MetaNode* parentNode) {
    MetaNode* mnode = parentNode;
    if (parentNode == nullptr) {
        parentNode = ModuleManager::GetInstance()->GetSelfModule()->getMetaNode();
    }
    //fns.metaNamespaceList.Clear();
    for (size_t i = 0; i < fns->namespaceStatementBlock()->tokenList().size(); i++) {
        std::string name = fns->namespaceStatementBlock()->tokenList()[i]->lexeme().ToString();
        parentNode = parentNode->GetChildrenMetaNodeByName(name);
        bool isCreate = true;
        if (parentNode != nullptr) {
            if (parentNode->getMetaNamespace() == nullptr) {
                isCreate = true;
            } else {
                isCreate = false;
            }
        } else {
            isCreate = true;
        }
        if (isCreate) {
            MetaNamespace* mn = new MetaNamespace(name);
            if (ProjectManager::useDefineNamespaceType != EUseDefineType::NoUseProjectConfigNamespace) {
                mn->isNotAllowCreateName = true;
                Log::AddInStructMeta(EError::None, "Error 在使用namespace 时，在项目定义中，没有找到相关的定义!!  位置:" + fns->namespaceStatementBlock()->tokenList()[i]->ToLexemeAllString());
            }
            parentNode = parentNode->AddMetaNamespace(mn);
        }
    }
}

void NamespaceManager::CreateMetaNamespaceByFileMetaNamespace(SimpleLanguage::Compile::CoreFileMeta::FileMetaNamespace* fmn) {
    MetaBase* mn = nullptr;
    if (fmn->topLevelFileMetaNamespace != nullptr) {
        //mn = fmn.topLevelFileMetaNamespace.namespaceStatementBlock;
    }
    //CreateMetaNamespaceByFineDefineNamespace(fmn, mn);
}

MetaNode* NamespaceManager::FindFinalMetaNamespaceByNSBlock(SimpleLanguage::Compile::CoreFileMeta::NamespaceStatementBlock* nsb, MetaNode* root) {
    if (nsb->namespaceList().size() == 0) {
        return nullptr;
    }

    if (root == nullptr) {
        root = ModuleManager::GetInstance()->GetSelfModule()->getMetaNode();
    }
    for (size_t i = 0; i < nsb->tokenList().size(); i++) {
        std::string name = nsb->tokenList()[i]->lexeme().ToString();
        MetaNode* findNode2 = root->GetChildrenMetaNodeByName(name);
        if (findNode2 == nullptr) {
            break;
        }
        root = findNode2;
        if (i == nsb->tokenList().size() - 1) {
            if (findNode2->getMetaNamespace() != nullptr) {
                return findNode2;
            }
        }
    }
    return nullptr;
}

void NamespaceManager::AddNamespaceString(const std::string& nsString) {
    if (metaNamespaceDict.find(nsString) != metaNamespaceDict.end()) {
        return;
    }
    std::vector<std::string> list;
    MetaModule* selfModule = ModuleManager::GetInstance()->GetSelfModule();
    std::string tempname = "";
    if (SimpleLanguage::Compile::CompilerUtil::CheckNameList(nsString, &list)) {
        //MetaNamespace parentMetaNamespace = nullptr;
        //for ( int i = 0; i < list.Count; i++ )
        //{
        //    tempname = list[i];
        //    if ( i == 0 )
        //    {
        //        var metabase = selfModule.GetChildrenMetaBaseByName(tempname);
        //        if (metabase != null)
        //        {
        //            parentMetaNamespace = metabase as MetaNamespace;
        //            if(parentMetaNamespace == null )
        //            {
        //                Debug.Write("已有类: " + tempname + "与添加的命名空间冲突!!");
        //                return;
        //            }
        //        }
        //        else
        //        {
        //            parentMetaNamespace = new MetaNamespace(tempname);
        //            selfModule.AddMetaNamespace(parentMetaNamespace);
        //        }                
        //    }
        //    else
        //    {
        //        var metabase = parentMetaNamespace.GetChildrenMetaBaseByName(tempname);
        //        if( metabase != null )
        //        {
        //            parentMetaNamespace = metabase as MetaNamespace;
        //            if (parentMetaNamespace == null)
        //            {
        //                Debug.Write("已有类: " + tempname + "与添加的命名空间冲突!!");
        //                return;
        //            }
        //        }
        //        else
        //        {
        //            var mn = new MetaNamespace(tempname);
        //            parentMetaNamespace.AddMetaNamespace(mn);
        //            parentMetaNamespace = mn;
        //        }
        //    }
        //}
        //metaNamespaceDict.Add(nsString, parentMetaNamespace);
    } else {
        Log::AddInStructMeta(EError::None, "NamespaceManager::AddNamespaceString 命名空间:" + nsString + "解析错误!!");
        return;
    }
}

MetaBase* NamespaceManager::FindMetaBaseByNamespaceToParentAndName(MetaNamespace* mn, const std::string& nodeName) {
    MetaBase* cur = mn;
    MetaBase* childMB = nullptr;
    while (cur != nullptr) {
        //childMB = cur.GetChildrenMetaBaseByName(nodeName);
        //if (childMB != nullptr)
        //    return childMB;
        //cur = cur.parentNode;
    }
    return childMB;
}

} // namespace Core
} // namespace SimpleLanguage
