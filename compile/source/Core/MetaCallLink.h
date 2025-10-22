//****************************************************************************
//  File:      MetaCallLink.h
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/7/29 12:00:00
//  Description:  this's a common node handles
//****************************************************************************

#pragma once

#include "MetaCallNode.h"
#include "../Compile/CoreFileMeta/FileMetaCallLink.h"
#include "../Compile/CoreFileMeta/FileMetaCallNode.h"
#include "../Compile/CoreFileMeta/FileMetaBase.h"
#include "../Compile/AllowUseSettings.h"
#include "MetaClass.h"
#include "Statements/MetaBlockStatements.h"
#include "MetaType.h"
#include "MetaVariable.h"
#include <string>
#include <vector>
#include <memory>

namespace SimpleLanguage {
namespace Core {

// 前向声明
class MetaVisitNode;
class MetaClass;
class MetaBlockStatements;
class MetaType;
class MetaVariable;
class AllowUseSettings;

class MetaCallLink {
public:
    MetaCallLink(FileMetaCallLink* fmcl, MetaClass* metaClass, MetaBlockStatements* mbs, MetaType* frontDefineMt, MetaVariable* mv);
    ~MetaCallLink() = default;

    // 属性访问器
    MetaVisitNode* getFinalCallNode() const { return m_FinalCallNode; }
    const std::vector<MetaVisitNode*>& getCallNodeList() const { return m_VisitNodeList; }
    AllowUseSettings* getAllowUseSettings() const { return m_AllowUseSettings; }

    // 方法
    void parse(AllowUseSettings* auc);
    void calcReturnType();
    std::string toFormatString() const;
    std::string toTokenString() const;
    MetaCallLink* clone() const;

private:
    void createCallLinkNode(MetaType* frontDefineMt, MetaVariable* mv);
    void parseCallNodeList(AllowUseSettings* auc);
    void calcCallNodeListReturnType();
    void createVisitNodeList();
    void parseVisitNodeList(AllowUseSettings* auc);
    void calcVisitNodeListReturnType();
    void createFinalCallNode();
    void parseFinalCallNode(AllowUseSettings* auc);
    void calcFinalCallNodeReturnType();

    FileMetaCallLink* m_FileMetaCallLink = nullptr;
    MetaClass* m_OwnerMetaClass = nullptr;
    MetaBlockStatements* m_OwnerMetaBlockStatements = nullptr;
    std::vector<MetaCallNode*> m_CallNodeList;
    MetaVisitNode* m_FinalCallNode = nullptr;
    std::vector<MetaVisitNode*> m_VisitNodeList;
    AllowUseSettings* m_AllowUseSettings = nullptr;
};

} // namespace Core
} // namespace SimpleLanguage

