//****************************************************************************
//  File:      MetaClass.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta class's attribute
//****************************************************************************

#pragma once

#include "MetaBase.h"
#include "../Define.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>

namespace SimpleLanguage {
namespace Compile {
    class FileMetaClass;
    class Token;
}

namespace Core {

enum class EClassDefineType : uint8_t
{
    StructDefine = 0,
    InnerDefine = 1,
    CodeDefine = 2
};

// Forward declarations
class MetaNode;
class MetaType;
class MetaMemberVariable;
class MetaMemberFunction;
class MetaMemberFunctionTemplateNode;
class MetaExpressNode;
class MetaTemplate;
class MetaInputParamCollection;
class MetaGenTemplateClass;
class MetaGenTemplate;
class MetaInputTemplateCollection;

class MetaClass : public MetaBase
{
public:
    // Constructors
    MetaClass();
    MetaClass(const std::string& name, EClassDefineType ecdt);
    MetaClass(const std::string& name, EType type = EType::Class);
    MetaClass(const MetaClass& mc);
    virtual ~MetaClass() = default;

    // Override methods
    void SetDeep(int deep) override;

    // Property accessors
    virtual std::string GetAllClassName() const { return m_AllName; }
    EType GetEType()  { return m_Type; }
    EClassDefineType GetClassDefineType() const { return m_ClassDefineType; }
    MetaClass* GetExtendClass() const { return m_ExtendClass; }
    MetaType* GetExtendClassMetaType() const { return m_ExtendClassMetaType; }
    int GetExtendLevel() const { return m_ExtendLevel; }
    bool IsInterfaceClass() const { return m_IsInterfaceClass; }
    const std::vector<MetaClass*>& GetInterfaceClass() const { return m_InterfaceClass; }
    MetaExpressNode* GetDefaultExpressNode() const { return m_DefaultExpressNode; }
    const std::unordered_map<std::string, MetaMemberVariable*>& GetAllMetaMemberVariableDict() const;
    std::vector<MetaMemberVariable*> GetAllMetaMemberVariableList() const;
    const std::vector<MetaMemberFunction*>& GetNonStaticVirtualMetaMemberFunctionList() const { return m_NonStaticVirtualMetaMemberFunctionList; }
    const std::vector<MetaMemberFunction*>& GetStaticMetaMemberFunctionList() const { return m_StaticMetaMemberFunctionList; }
    const std::unordered_map<std::string, MetaMemberVariable*>& GetMetaMemberVariableDict() const { return m_MetaMemberVariableDict; }
    const std::unordered_map<std::string, MetaMemberFunctionTemplateNode*>& GetMetaMemberFunctionTemplateNodeDict() const { return m_MetaMemberFunctionTemplateNodeDict; }
    const std::unordered_map<std::string, MetaMemberVariable*>& GetMetaExtendMemeberVariableDict() const { return m_MetaExtendMemeberVariableDict; }
    const std::vector<MetaType*>& GetBindStructTemplateMetaClassList() const { return m_BindStructTemplateMetaClassList; }
    const std::unordered_map<Compile::Token*, Compile::FileMetaClass*>& GetFileMetaClassDict() const { return m_FileMetaClassDict; }
    bool IsHandleExtendVariableDirty() const { return m_IsHandleExtendVariableDirty; }

    // Setter methods
    void SetDefaultExpressNode(MetaExpressNode* defaultExpressNode) { m_DefaultExpressNode = defaultExpressNode; }
    void SetClassDefineType(EClassDefineType ecdt) { m_ClassDefineType = ecdt; }
    void SetExtendClass(MetaClass* sec) { m_ExtendClass = sec; }
    void SetHandleExtendVariableDirty(bool dirty) { m_IsHandleExtendVariableDirty = dirty; }

    // Parse methods
    virtual void Parse();
    virtual void ParseInnerVariable();
    virtual void ParseInnerFunction();
    virtual void ParseInner();
    virtual void ParseExtendsRelation();
    virtual void ParseInterfaceRelation();
    virtual void HandleExtendMemberVariable();
    virtual void HandleExtendMemberFunction();
    virtual void ParseMemberVariableDefineMetaType();
    virtual void ParseMemberFunctionDefineMetaType();
    virtual bool CheckInterface();
    virtual void ParseDefineComplete();
    virtual void ParseGenTemplateClassMetaType();

    // Search methods
    virtual MetaMemberVariable* GetMetaMemberVariableByName(const std::string& name);
    virtual std::vector<MetaMemberVariable*> GetMetaMemberVariableListByFlag(bool isStatic);
    virtual MetaMemberFunction* GetMetaDefineGetSetMemberFunctionByName(const std::string& name, MetaInputParamCollection* inputParam, bool isGet, bool isSet);
    virtual MetaMemberFunction* GetMetaMemberFunctionByNameAndInputTemplateInputParamCount(const std::string& name, int templateParamCount, MetaInputParamCollection* inputParam, bool isIncludeExtendClass = true);
    virtual MetaMemberFunction* GetMetaMemberFunctionByNameAndInputTemplateInputParam(const std::string& name, const std::vector<MetaClass*>& mtList, MetaInputParamCollection* inputParam, bool isIncludeExtendClass = true);
    virtual MetaMemberFunction* GetMetaMemberConstructDefaultFunction();
    virtual MetaMemberFunction* GetMetaMemberConstructFunction(MetaInputParamCollection* mmpc);
    virtual MetaMemberFunction* GetFirstMetaMemberFunctionByName(const std::string& name);
    virtual std::vector<MetaMemberFunction*> GetMemberInterfaceFunction();
    virtual bool GetMemberInterfaceFunctionByFunc(MetaMemberFunction* func);

    // Utility methods
    void UpdateClassAllName();
    void CalcExtendLevel();
    bool IsParseMetaClass(MetaClass* parentClass, bool isIncludeSelf = true);
    void AddInterfaceClass(MetaClass* aic);
    void AddMetaMemberVariable(MetaMemberVariable* mmv, bool isAddManager = true);
    void AddInnerMetaMemberFunction(MetaMemberFunction* mmf);
    bool AddMetaMemberFunction(MetaMemberFunction* mmf);
    void AddDefineConstructFunction();
    void AddDefineInstanceValue();
    void HandleExtendClassVariable();

    // Template related methods
    MetaType* AddMetaPreTemplateClass(MetaType* mt, bool isParse, bool& isGenMetaClass);
    MetaGenTemplateClass* AddMetaTemplateClassByMetaClassAndMetaTemplateMetaTypeList(const std::vector<MetaType*>& templateMetaTypeList);
    MetaType* BindStructTemplateMetaClassList(MetaType* mt);
    
    // CRITICAL MISSING METHOD: AddInstanceMetaClass
    MetaGenTemplateClass* AddInstanceMetaClass(const std::vector<MetaClass*>& mcList);

    // File meta class binding and parsing methods
    void BindFileMetaClass(Compile::FileMetaClass* fmc);
    void ParseFileMetaClassMemeberVarAndFunc(Compile::FileMetaClass* fmc);

    // Format methods
    std::string ToString() const override;
    std::string ToFormatString() const override;
    virtual std::string ToDefineTypeString() const;
    virtual std::string GetFormatString(bool isShowNamespace) const;

    // Template related property accessors
    bool IsTemplateClass() const { return m_IsTemplateClass; }
    virtual bool IsGenTemplate() const { return m_IsGenTemplate; }
    const std::vector<MetaTemplate*>& GetMetaTemplateList() const { return m_MetaTemplateList; }
    const std::vector<MetaGenTemplateClass*>& GetMetaGenTemplateClassList() const { return m_MetaGenTemplateClassList; }

    // Template related methods from MetaTemplateClass.cs
    bool IsDefineTemplate(const std::string& name);
    void ParseMetaInConstraint();
    MetaClass* ParseFileMetaClassTemplate(Compile::FileMetaClass* fmc);
    bool CompareInputTemplateList(MetaInputTemplateCollection* mitc);
    MetaTemplate* GetMetaTemplateByName(const std::string& name);
    MetaTemplate* GetMetaTemplateByIndex(int index);
    bool IsTemplateMetaClassByName(const std::string& name);
    void AddGenTemplateMetaClass(MetaGenTemplateClass* mtc);
    MetaGenTemplateClass* GetGenTemplateMetaClass(MetaInputTemplateCollection* mitc);
    MetaGenTemplateClass* AddInstanceMetaClass(const std::vector<MetaType*>& inputlist);
    MetaGenTemplateClass* GetGenTemplateMetaClassByTemplateList(const std::vector<MetaGenTemplate*>& list);

protected:
    int m_ExtendLevel = 0;
    EType m_Type = EType::None;
    std::unordered_map<Compile::Token*, Compile::FileMetaClass*> m_FileMetaClassDict;
    MetaClass* m_ExtendClass = nullptr;
    MetaType* m_ExtendClassMetaType = nullptr;
    std::vector<MetaType*> m_BindStructTemplateMetaClassList;
    std::vector<MetaClass*> m_InterfaceClass;
    std::vector<MetaType*> m_InterfaceMetaType;
    std::unordered_map<std::string, MetaMemberVariable*> m_MetaMemberVariableDict;
    std::vector<MetaMemberVariable*> m_FileCollectMetaMemberVariable;
    std::unordered_map<std::string, MetaMemberVariable*> m_MetaExtendMemeberVariableDict;
    std::unordered_map<std::string, MetaMemberFunctionTemplateNode*> m_MetaMemberFunctionTemplateNodeDict;
    std::vector<MetaMemberFunction*> m_FileCollectMetaMemberFunctionList;
    std::vector<MetaMemberFunction*> m_NonStaticVirtualMetaMemberFunctionList;
    std::vector<MetaMemberFunction*> m_StaticMetaMemberFunctionList;
    std::vector<MetaMemberFunction*> m_TempInnerFunctionList;
    MetaExpressNode* m_DefaultExpressNode = nullptr;
    EClassDefineType m_ClassDefineType = EClassDefineType::InnerDefine;
    bool m_IsInterfaceClass = false;
    bool m_IsHandleExtendVariableDirty = false;

    // Template related
    std::vector<MetaTemplate*> m_MetaTemplateList;
    std::vector<MetaGenTemplateClass*> m_MetaGenTemplateClassList;
    bool m_IsTemplateClass = false;
    bool m_IsGenTemplate = false;
};

} // namespace Core
} // namespace SimpleLanguage