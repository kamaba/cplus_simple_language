//****************************************************************************
//  File:      ExpressManager.h
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/5/17 12:00:00
//  Description:  
//****************************************************************************

#pragma once

#include "MetaExpressNode/MetaExpressBase.h"
#include "MetaExpressNode/MetaConstExpressNode.h"
#include "MetaExpressNode/MetaOpExpressNode.h"
#include "MetaExpressNode/MetaUnaryOpExpressNode.h"
#include "MetaExpressNode/MetaCallLinkExpressNode.h"
#include "MetaExpressNode/MetaNewObjectExpressNode.h"
#include "MetaExpressNode/MetaExecuteStatementsNode.h"
#include "MetaCallLink.h"
#include "MetaBlockStatements.h"
#include "MetaClass.h"
#include "MetaType.h"
#include "MetaVariable.h"
#include "AllowUseSettings.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "../Compile/CoreFileMeta/FileMetaBaseTerm.h"
#include "../Compile/CoreFileMeta/FileMetaIfSyntaxTerm.h"
#include "../Compile/CoreFileMeta/FileMetaMatchSyntaxTerm.h"
#include "../Compile/CoreFileMeta/FileMetaParTerm.h"
#include "../Compile/CoreFileMeta/FileMetaConstValueTerm.h"
#include "../Compile/CoreFileMeta/FileMetaCallTerm.h"
#include "../Compile/CoreFileMeta/FileMetaBraceTerm.h"
#include "../Compile/CoreFileMeta/FileMetaBracketTerm.h"
#include "../Compile/CoreFileMeta/FileMetaSymbolTerm.h"
#include "../Compile/CoreFileMeta/FileMetaTermExpress.h"
#include "../Compile/Token.h"
#include "../Debug/Log.h"
#include <string>

namespace SimpleLanguage {
namespace Core {

// ExpressOptimizeConfig class
class ExpressOptimizeConfig {
public:
    bool ifLeftAndRightIsConstThenCompute = true;
    bool notEqualConvertEqualExpress = false;    // not equal express convert equal express
    bool greaterOrEqualConvertGeraterAndEqual = false;
    bool lessOrEqualConvertLessAndEqual = false;
    bool ifUnaryExpressValueIsConstThenCompute = true; // if unary express's value node is const type, then force compute 
    bool ifOpExpressLeftAndRightIsConstThenCompute = true;
};

// CreateExpressParam struct
struct CreateExpressParam {
    MetaBlockStatements* ownerMBS = nullptr;
    MetaClass* ownerMetaClass = nullptr;
    MetaType* metaType = nullptr;
    MetaType* parentMetaType = nullptr;
    MetaVariable* equalMetaVariable = nullptr;
    Compile::FileMetaBaseTerm* fme = nullptr;
    bool isStatic = false;
    bool isConst = false;
    bool allowUseIfSyntax = false;
    bool allowUseSwitchSyntax = false;
    bool allowUseParSyntax = false;
    bool allowUseBraceSyntax = false;
    EParseFrom parsefrom = EParseFrom::None;

    CreateExpressParam() = default;
    CreateExpressParam(const CreateExpressParam& clone) {
        ownerMBS = clone.ownerMBS;
        ownerMetaClass = clone.ownerMetaClass;
        equalMetaVariable = clone.equalMetaVariable;
        metaType = clone.metaType;
        parentMetaType = clone.parentMetaType;
        fme = clone.fme;
        isStatic = clone.isStatic;
        isConst = clone.isConst;
        allowUseIfSyntax = clone.allowUseIfSyntax;
        allowUseSwitchSyntax = clone.allowUseSwitchSyntax;
        allowUseParSyntax = clone.allowUseParSyntax;
        allowUseBraceSyntax = clone.allowUseBraceSyntax;
        parsefrom = clone.parsefrom;
    }
};

// ExpressManager class
class ExpressManager {
public:
    // 单例模式
    static ExpressManager& GetInstance() {
        static ExpressManager instance;
        return instance;
    }

    // 禁用拷贝构造和赋值
    ExpressManager(const ExpressManager&) = delete;
    ExpressManager& operator=(const ExpressManager&) = delete;

    // 静态配置
    static ExpressOptimizeConfig expressOptimizeConfig;

    // 静态方法
    static bool IsCanExpressCampute(MetaClass* mc);
    static MetaExpressNode* CreateExpressNodeByCEP(const CreateExpressParam& cep);
    static MetaExpressNode* CreateExpressNode(const CreateExpressParam& cep);
    static MetaExpressNode* CreateOptimizeAfterExpress(MetaExpressNode* men, ExpressOptimizeConfig* config = nullptr);
    static void CreateNewOrCalllink(const CreateExpressParam& cep, MetaNewObjectExpressNode*& mnoen, MetaCallLinkExpressNode*& men2);
    static int CalcParseLevel(int level, MetaExpressNode* men);

private:
    // 私有构造函数
    ExpressManager() = default;
    ~ExpressManager() = default;
};

} // namespace Core
} // namespace SimpleLanguage
