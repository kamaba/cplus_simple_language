//****************************************************************************
//  File:      ExpressManager.h
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/5/17 12:00:00
//  Description:  
//****************************************************************************

#pragma once

#include <string>

namespace SimpleLanguage {
    namespace Compile {
        class FileMetaBaseTerm;
        class FileMetaIfSyntaxTerm;
        class FileMetaMatchSyntaxTerm;
        class FileMetaParTerm;
        class FileMetaCallTerm;
        class FileMetaConstValueTerm;
        class FileMetaBraceTerm;
        class FileMetaBracketTerm;
        class FileMetaSymbolTerm;
        class FileMetaTermExpress;
        class FileMetaCallLink;
    }
    namespace Core {
        class MetaClass;
        class MetaType;
        class MetaVariable;
        class MetaBlockStatements;
        class MetaExpressNode;
        class MetaNewObjectExpressNode;
        class MetaCallLinkExpressNode;
        class MetaConstExpressNode;
        class MetaOpExpressNode;
        class MetaUnaryOpExpressNode;
        class MetaExecuteStatementsNode;
        class MetaCallLink;
        class MetaMemberFunction;
        class MetaVisitNode;
        class AllowUseSettings;
    }
}

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
    CreateExpressParam(const CreateExpressParam& clone);
    
    // Setter methods
    void SetOwnerMBS(MetaBlockStatements* mbs);
    void SetOwnerMetaClass(MetaClass* mc);
    void SetMetaType(MetaType* mt);
    void SetParentMetaType(MetaType* pmt);
    void SetEqualMetaVariable(MetaVariable* mv);
    void SetFme(Compile::FileMetaBaseTerm* fme);
    void SetIsStatic(bool isStatic);
    void SetIsConst(bool isConst);
    void SetAllowUseIfSyntax(bool allow);
    void SetAllowUseSwitchSyntax(bool allow);
    void SetAllowUseParSyntax(bool allow);
    void SetAllowUseBraceSyntax(bool allow);
    void SetParseFrom(EParseFrom parseFrom);
};

// ExpressManager class
class ExpressManager {
public:
    // 单例模式
    static ExpressManager& GetInstance();
    ExpressManager(const ExpressManager&) = delete;
    ExpressManager& operator=(const ExpressManager&) = delete;

    // 静态成员
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