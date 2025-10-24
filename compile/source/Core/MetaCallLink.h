//****************************************************************************
//  File:      MetaCallLink.h
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/7/29 12:00:00
//  Description:  this's a common node handles
//****************************************************************************

#pragma once

#include <string>
#include <vector>
#include <memory>

namespace SimpleLanguage {
    namespace Compile{
        class FileMetaCallLink;
    }
namespace Core {

// 前向声明
class MetaVisitNode;
class MetaClass;
class MetaBlockStatements;
class MetaType;
class MetaVariable;
class AllowUseSettings;
class MetaCallNode;
class Token;
class MetaExpressNode;

class MetaCallLink {
public:
    // 构造函数
    MetaCallLink(SimpleLanguage::Compile::FileMetaCallLink* fmcl, MetaClass* metaClass, MetaBlockStatements* mbs, MetaType* frontDefineMt, MetaVariable* mv);
    ~MetaCallLink() = default;

    // 属性访问器
    MetaVisitNode* GetFinalCallNode() const { return m_FinalCallNode; }
    const std::vector<MetaVisitNode*>& GetCallNodeList() const { return m_VisitNodeList; }
    AllowUseSettings* GetAllowUseSettings() const { return m_AllowUseSettings; }

    // 核心方法
    Token* GetToken();
    bool Parse(AllowUseSettings* _useConst);
    int CalcParseLevel(int level);
    void CalcReturnType();
    MetaVariable* ExecuteGetMetaVariable();
    MetaClass* ExecuteGetMetaClass();
    MetaExpressNode* GetMetaExpressNode();
    MetaType* GetMetaDeineType();
    std::string ToFormatString() const;
    std::string ToTokenString() const;

private:
    void CreateCallLinkNode(MetaType* frontDefineMt, MetaVariable* mv);

    SimpleLanguage::Compile::FileMetaCallLink* m_FileMetaCallLink = nullptr;
    MetaClass* m_OwnerMetaClass = nullptr;
    MetaBlockStatements* m_OwnerMetaBlockStatements = nullptr;
    std::vector<MetaCallNode*> m_CallNodeList;
    MetaVisitNode* m_FinalCallNode = nullptr;
    std::vector<MetaVisitNode*> m_VisitNodeList;
    AllowUseSettings* m_AllowUseSettings = nullptr;
};

} // namespace Core
} // namespace SimpleLanguage