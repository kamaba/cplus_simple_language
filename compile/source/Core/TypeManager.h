//****************************************************************************
//  File:      TypeManager.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: Type manager class
//****************************************************************************

#pragma once

#include <string>
#include <vector>
#include <memory>

namespace SimpleLanguage {
    namespace Compile
    {
        class FileMetaClassDefine;
        class FileInputTemplateNode;
    }
namespace Core {

    class MetaGenTemplateFunction;
    class MetaType;
    class MetaClass;
    class MetaNode;
    class MetaGenTemplateClass;
    class MetaMemberFunction;

class TypeManager {
public:
    // ����ģʽ
    static TypeManager& GetInstance() {
        static TypeManager instance;
        return instance;
    }

    // ���ÿ�������͸�ֵ
    TypeManager(const TypeManager&) = delete;
    TypeManager& operator=(const TypeManager&) = delete;

    // ��������
    bool UpdateMetaTypeByGenClassAndFunction(MetaType* mt, MetaGenTemplateClass* mgtc, MetaGenTemplateFunction* mgtf);
    MetaType* GetMetaTemplateClassAndRegisterExptendTemplateClassInstance(MetaClass* curMc, Compile::FileMetaClassDefine* fmcd);
    MetaType* GetMetaTypeByInputTemplateList(MetaClass* ownerMc, MetaNode* getmc, const std::vector<Compile::FileInputTemplateNode*>& inputTemplateNodeList, std::vector<MetaType*>* list = nullptr);
    MetaType* GetAndRegisterTemplateDefineMetaTemplateClass(MetaClass* ownerMc, MetaClass* findMc, Compile::FileInputTemplateNode* fmtd);
    MetaType* GetMetaTypeByTemplateFunction(MetaClass* curMc, MetaMemberFunction* findFun, Compile::FileMetaClassDefine* fmcd);
    MetaType* GetMetaTypeByTemplateList(MetaClass* curMc, MetaNode* getmc, MetaMemberFunction* findFun, const std::vector<Compile::FileInputTemplateNode*>& inputTemplateNodeList);
    MetaType* HandleInputTemplateNodeList(MetaClass* findfn, MetaClass* regMc, MetaMemberFunction* findFun, const std::vector<Compile::FileInputTemplateNode*>& inputTemplateNodeList, bool isParse);
    MetaType* RegisterTemplateDefineMetaTemplateFunction(MetaClass* findMc, MetaClass* regMc, MetaMemberFunction* findFun, Compile::FileInputTemplateNode* fmtd, bool isParse = false);

private:
    // ˽�й��캯��
    TypeManager() = default;
    ~TypeManager() = default;
};

} // namespace Core
} // namespace SimpleLanguage
