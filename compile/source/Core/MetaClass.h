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

namespace SimpleLanguage 
{
	namespace Core
	{

		enum class EClassDefineType : uint8_t
		{
			StructDefine = 0,
			InnerDefine = 1,
			CodeDefine = 2
		};

		// 前向声明
		class MetaNode;
		class MetaType;
		class MetaMemberVariable;
		class MetaMemberFunction;
		class MetaMemberFunctionTemplateNode;
		class MetaExpressNode;
		class MetaTemplate;

		class MetaClass : public MetaBase
		{
		public:
			MetaClass();
			MetaClass(const std::string& name, EClassDefineType ecdt);
			MetaClass(const std::string& name, EType type = EType::Class);
			MetaClass(const MetaClass& mc);
			virtual ~MetaClass() = default;

			// 属性访问器
			virtual std::string GetAllClassName() const { return m_AllName; }
			EType GetEType() const { return m_Type; }
			EClassDefineType GetClassDefineType() const { return m_ClassDefineType; }
			MetaClass* getExtendClass() const { return m_ExtendClass; }
			MetaType* getExtendClassMetaType() const { return m_ExtendClassMetaType; }
			int getExtendLevel() const { return m_ExtendLevel; }
			bool isInterfaceClass() const { return m_IsInterfaceClass; }
			const std::vector<MetaClass*>& getInterfaceClass() const { return m_InterfaceClass; }
			MetaExpressNode* getDefaultExpressNode() const { return m_DefaultExpressNode; }
			const std::unordered_map<std::string, MetaMemberVariable*>& GetAllMetaMemberVariableDict() const;
			std::vector<MetaMemberVariable*> GetAllMetaMemberVariableList() const;
			const std::vector<MetaMemberFunction*>& getNonStaticVirtualMetaMemberFunctionList() const { return m_NonStaticVirtualMetaMemberFunctionList; }
			const std::vector<MetaMemberFunction*>& getStaticMetaMemberFunctionList() const { return m_StaticMetaMemberFunctionList; }
			const std::unordered_map<std::string, MetaMemberVariable*>& getMetaMemberVariableDict() const { return m_MetaMemberVariableDict; }
			const std::unordered_map<std::string, MetaMemberFunctionTemplateNode*>& getMetaMemberFunctionTemplateNodeDict() const { return m_MetaMemberFunctionTemplateNodeDict; }
			const std::unordered_map<std::string, MetaMemberVariable*>& getMetaExtendMemeberVariableDict() const { return m_MetaExtendMemeberVariableDict; }
			const std::vector<MetaType*>& getBindStructTemplateMetaClassList() const { return m_BindStructTemplateMetaClassList; }
			bool isHandleExtendVariableDirty() const { return m_IsHandleExtendVariableDirty; }

			// 设置方法
			void setDefaultExpressNode(MetaExpressNode* defaultExpressNode) { m_DefaultExpressNode = defaultExpressNode; }
			void setClassDefineType(EClassDefineType ecdt) { m_ClassDefineType = ecdt; }
			void setExtendClass(MetaClass* sec) { m_ExtendClass = sec; }
			void setHandleExtendVariableDirty(bool dirty) { m_IsHandleExtendVariableDirty = dirty; }

			// 虚方法
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
			virtual MetaMemberVariable* GetMetaMemberVariableByName(const std::string& name);
			virtual std::vector<MetaMemberVariable*> GetMetaMemberVariableListByFlag(bool isStatic);
			virtual MetaMemberFunction* GetMetaDefineGetSetMemberFunctionByName(const std::string& name, void* inputParam, bool isGet, bool isSet);
			virtual MetaMemberFunction* GetMetaMemberFunctionByNameAndInputTemplateInputParamCount(const std::string& name, int templateParamCount, void* inputParam, bool isIncludeExtendClass = true);
			virtual MetaMemberFunction* GetMetaMemberFunctionByNameAndInputTemplateInputParam(const std::string& name, const std::vector<MetaClass*>& mtList, void* inputParam, bool isIncludeExtendClass = true);
			virtual MetaMemberFunction* GetMetaMemberConstructDefaultFunction();
			virtual MetaMemberFunction* GetMetaMemberConstructFunction(void* mmpc);
			virtual MetaMemberFunction* GetFirstMetaMemberFunctionByName(const std::string& name);
			virtual std::vector<MetaMemberFunction*> GetMemberInterfaceFunction();
			virtual bool GetMemberInterfaceFunctionByFunc(MetaMemberFunction* func);
			virtual std::string ToDefineTypeString();
			virtual std::string GetFormatString(bool isShowNamespace) const;

			// 其他方法
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
			MetaMemberFunction* GetMetaMemberConstructDefaultFunction();
			MetaMemberFunction* GetMetaMemberConstructFunction(void* mmpc);

			// 模板相关方法
			MetaType* AddMetaPreTemplateClass(MetaType* mt, bool isParse, bool& isGenMetaClass);
			MetaClass* AddMetaTemplateClassByMetaClassAndMetaTemplateMetaTypeList(const std::vector<MetaType*>& templateMetaTypeList);
			MetaType* BindStructTemplateMetaClassList(MetaType* mt);

			// 格式化方法
			std::string ToString() const;
			std::string ToFormatString() const override;

		protected:
			int m_ExtendLevel = 0;
			EType m_Type = EType::None;
			std::unordered_map<void*, void*> m_FileMetaClassDict; // 简化实现，实际应该是 FileMetaClass
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

			// 模板相关
			std::vector<MetaTemplate*> m_MetaTemplateList;
			bool m_IsTemplateClass = false;
			bool m_IsGenTemplate = false;

		public:
			// 模板相关属性访问器
			bool isTemplateClass() const { return m_IsTemplateClass; }
			bool isGenTemplate() const { return m_IsGenTemplate; }
			const std::vector<MetaTemplate*>& getMetaTemplateList() const { return m_MetaTemplateList; }
		};

	} // namespace Core
} // namespace SimpleLanguage
