//****************************************************************************
//  File:      MetaType.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta type class
//****************************************************************************

#pragma once

#include "MetaBase.h"
#include <vector>
#include <string>

namespace SimpleLanguage {
namespace Core {

// Forward declarations
class MetaClass;
class MetaTemplate;
class MetaGenTemplateClass;
class MetaInputTemplateCollection;
class MetaInputParamCollection;
class MetaMemberFunction;
class MetaMemberEnum;
class MetaExpressNode;
class MetaGenTemplate;

enum class EMetaTypeType : uint8_t
{
    None = 0,
    MetaClass = 1,
    MetaGenClass = 2,
    Template = 3,
    TemplateClassWithTemplate = 4
};

class MetaType : public MetaBase
{
public:
    // Properties
    std::string GetName() const override;
    EMetaTypeType GetEType() const { return m_EType; }
    MetaClass* GetMetaClass() const { return m_MetaClass; }
    MetaClass* GetTypeInferenceClass() const { return m_TypeInferenceClass; }
    bool IsEnum() const;
    bool IsData() const;
    bool IsTemplate() const { return m_EType == EMetaTypeType::Template; }
    MetaMemberEnum* GetEnumValue() const { return m_EnumValue; }
    const std::vector<MetaType*>& GetTemplateMetaTypeList() const { return m_TemplateMetaTypeList; }
    bool IsArray() const;
    bool IsDynamicClass() const;
    bool IsDynamicData() const;
    bool IsDefineMetaClass() const { return m_IsDefineMetaClass; }
    MetaTemplate* GetMetaTemplate() const { return m_MetaTemplate; }

    // Constructors
    MetaType();
    MetaType(MetaTemplate* mt);
    MetaType(MetaGenTemplateClass* mgtc, const std::vector<MetaType*>& mtList);
    MetaType(MetaClass* mc);
    MetaType(MetaClass* mc, const std::vector<MetaType*>& mtList);
    MetaType(MetaClass* mc, MetaClass* templatemc, MetaInputTemplateCollection* mitc);
    MetaType(const MetaType& mt);

    // Methods
    bool IsCanForIn() const;
    MetaClass* GetTemplateMetaClass(bool& isGTC) const;
    MetaClass* GetTemplateMetaClass() const;
    bool IsIncludeTemplate() const;
    bool IsIncludeClassTemplate(MetaClass* ownerClass) const;
    bool IsIncludeFunctionTemplate(MetaMemberFunction* mmf) const;
    MetaMemberFunction* FindTemplateFunctionTemplate(MetaMemberFunction* mmf) const;
    void AddTemplateMetaType(MetaType* mt);
    MetaMemberFunction* GetMetaMemberConstructFunction(MetaInputParamCollection* input = nullptr) const;
    static bool EqualMetaDefineType(const MetaType* mdtL, const MetaType* mdtR);
    void SetMetaClass(MetaClass* mc);
    void SetMetaTemplate(MetaTemplate* mt);
    void SetTypeInferenceClass(MetaClass* mc);
    void SetTemplateMetaClass(MetaClass* mc);
    void UpdateMetaClassByRawMetaClassAndInputTemplateCollection();
    MetaClass* UpdateMetaGenTemplate(const std::vector<MetaGenTemplate*>& metaGenTemplateList);
    MetaType* GetMetaInputTemplateByIndex(int index = 0) const;
    MetaExpressNode* GetDefaultExpressNode() const;

    // Override methods
    std::string ToFormatString() const override;
    std::string ToString() const;

private:
    EMetaTypeType m_EType = EMetaTypeType::None;
    MetaClass* m_MetaClass = nullptr;                       // int a = 0; => int  List<int> => List<int>
    MetaClass* m_TypeInferenceClass = nullptr;              // 推理类
    MetaType* m_ParentMetaType = nullptr;
    MetaTemplate* m_MetaTemplate = nullptr;
    MetaExpressNode* m_DefaultExpressNode = nullptr;        // int a => a = 0;
    MetaMemberEnum* m_EnumValue = nullptr;                  // Enum{ a = 1; } Enum e = Enum.a(20)=> Enum.a(20)
    bool m_IsDefineMetaClass = false;
    std::vector<MetaType*> m_TemplateMetaTypeList;          // Map<T1,T2> 一般用在返回值类型定义中
};

} // namespace Core
} // namespace SimpleLanguage