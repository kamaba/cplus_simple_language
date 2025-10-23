//****************************************************************************
//  File:      MetaBlockStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description:  this's a statement in function! same link table model!
//****************************************************************************

#pragma once

#include "MetaStatements.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace SimpleLanguage {
    namespace Compile
    {
        class Token;
        class FileMetaBlockSyntax;
    }
namespace Core {

    // 前向声明
    class MetaFunction;
    class MetaVariable;
    class MetaDefineParamCollection;
    class MetaForStatements;
    class MetaWhileDoWhileStatements;

class MetaBlockStatements : public MetaStatements {
private:
    MetaFunction* m_OwnerMetaFunction = nullptr;
    bool m_IsOnFunction = false;  // on function or inner function
    
    std::unordered_map<std::string, MetaVariable*> m_MetaVariableDict;
    std::vector<MetaBlockStatements*> m_ChildrenMetaBlockStatementsList;
    MetaStatements* m_OwnerMetaStatements = nullptr;
    Compile::FileMetaBlockSyntax* m_FileMetaBlockSyntax = nullptr;

public:
    MetaBlockStatements* parent = nullptr;
    
    // 构造函数
    MetaBlockStatements(MetaBlockStatements* mbs);
    MetaBlockStatements(MetaFunction* mf);
    MetaBlockStatements(MetaFunction* mf, Compile::FileMetaBlockSyntax* fmbs);
    MetaBlockStatements(MetaBlockStatements* mbs, Compile::FileMetaBlockSyntax* fmbs);
    
    // 属性访问器
    virtual MetaFunction* GetOwnerMetaFunction() override;
    MetaStatements* GetOwnerMetaStatements() const;
    Compile::FileMetaBlockSyntax* GetFileMetaBlockSyntax() const;
    bool IsOnFunction() const { return m_IsOnFunction; }
    void SetIsOnFunction(bool value) { m_IsOnFunction = value; }
    
    // 设置方法
    void SetFileMetaBlockSyntax(Compile::FileMetaBlockSyntax* blockSyntax);
    void SetOwnerMetaStatements(MetaStatements* ms);
    virtual void SetNextStatements(MetaStatements* ms) override;
    
    // 查找和遍历方法
    MetaStatements* FindNearestMetaForStatementsOrMetaWhileOrDoWhileStatements();
    virtual void SetDeep(int dp) override;
    
    // 变量管理方法
    MetaVariable* GetMetaVariable(const std::string& name);
    bool AddMetaVariable(MetaVariable* mv);
    void AddFrontStatements(MetaStatements* ms);
    void AddFrontToEndStatements(MetaStatements* ms);
    bool UpdateMetaVariableDict(MetaVariable* mv);
    void GetCalcMetaVariableList(std::vector<MetaVariable*>& list);
    bool GetIsMetaVariable(const std::string& name, bool isFromParent = true);
    bool AddOnlyNameMetaVariable(const std::string& name);
    MetaVariable* GetMetaVariableByName(const std::string& name, bool isFromParent = true);
    void SetMetaMemberParamCollection(MetaDefineParamCollection* mmpc);
    
    // 格式化方法
    virtual std::string ToFormatString() const override;
};

} // namespace Core
} // namespace SimpleLanguage