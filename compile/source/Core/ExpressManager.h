//****************************************************************************
//  File:      ExpressManager.h
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/5/17 12:00:00
//  Description:  
//****************************************************************************

#pragma once

#include "../Debug/Log.h"
#include "AllowUseSettings.h"
#include <string>

namespace SimpleLanguage {
    namespace Compile
    {
        class FileMetaBaseTerm;
        class FileMetaIfSyntaxTerm;
    }
namespace Core {
    class MetaClass;
    class MetaType;
    class MetaVariable;
    class MetaBlockStatements;
    class MetaExpressNode;
    class MetaNewObjectExpressNode;
    class MetaCallLinkExpressNode;

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
    // ����ģʽ
    static ExpressManager& GetInstance() {
        static ExpressManager instance;
        return instance;
    }

    // ���ÿ�������͸�ֵ
    ExpressManager(const ExpressManager&) = delete;
    ExpressManager& operator=(const ExpressManager&) = delete;

    // ��̬����
    static ExpressOptimizeConfig expressOptimizeConfig;

    // ��̬����
    static bool IsCanExpressCampute(MetaClass* mc);
    static MetaExpressNode* CreateExpressNodeByCEP(const CreateExpressParam& cep);
    static MetaExpressNode* CreateExpressNode(const CreateExpressParam& cep);
    static MetaExpressNode* CreateOptimizeAfterExpress(MetaExpressNode* men, ExpressOptimizeConfig* config = nullptr);
    static void CreateNewOrCalllink(const CreateExpressParam& cep, MetaNewObjectExpressNode*& mnoen, MetaCallLinkExpressNode*& men2);
    static int CalcParseLevel(int level, MetaExpressNode* men);

private:
    // ˽�й��캯��
    ExpressManager() = default;
    ~ExpressManager() = default;
};

} // namespace Core
} // namespace SimpleLanguage
