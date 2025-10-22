//****************************************************************************
//  File:      TypeManager.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: Type manager class
//****************************************************************************

#pragma once

#include "../Debug/Log.h"
#include "ClassManager.h"
#include <string>
#include <vector>
#include <memory>

namespace SimpleLanguage {
namespace Core {

    class MetaGenTempalteFunction;
    class MetaType;
    class MetaGenTemplateClass;

class TypeManager {
public:
    // 单例模式
    static TypeManager& GetInstance() {
        static TypeManager instance;
        return instance;
    }

    // 禁用拷贝构造和赋值
    TypeManager(const TypeManager&) = delete;
    TypeManager& operator=(const TypeManager&) = delete;

    // 方法声明
    bool UpdateMetaTypeByGenClassAndFunction(MetaType* mt, MetaGenTemplateClass* mgtc, MetaGenTempalteFunction* mgtf);
    MetaType* GetMetaTemplateClassAndRegisterExptendTemplateClassInstance(MetaClass* curMc, Compile::FileMetaClassDefine* fmcd);
    MetaType* GetMetaTypeByInputTemplateList(MetaClass* ownerMc, MetaNode* getmc, const std::vector<Compile::FileInputTemplateNode*>& inputTemplateNodeList, std::vector<MetaType*>* list = nullptr);
    MetaType* GetAndRegisterTemplateDefineMetaTemplateClass(MetaClass* ownerMc, MetaClass* findMc, Compile::FileInputTemplateNode* fmtd);
    MetaType* GetMetaTypeByTemplateFunction(MetaClass* curMc, MetaMemberFunction* findFun, Compile::FileMetaClassDefine* fmcd);
    MetaType* GetMetaTypeByTemplateList(MetaClass* curMc, MetaNode* getmc, MetaMemberFunction* findFun, const std::vector<Compile::FileInputTemplateNode*>& inputTemplateNodeList);
    MetaType* HandleInputTemplateNodeList(MetaClass* findfn, MetaClass* regMc, MetaMemberFunction* findFun, const std::vector<Compile::FileInputTemplateNode*>& inputTemplateNodeList, bool isParse);
    MetaType* RegisterTemplateDefineMetaTemplateFunction(MetaClass* findMc, MetaClass* regMc, MetaMemberFunction* findFun, Compile::FileInputTemplateNode* fmtd, bool isParse = false);

private:
    // 私有构造函数
    TypeManager() = default;
    ~TypeManager() = default;
};

} // namespace Core
} // namespace SimpleLanguage
