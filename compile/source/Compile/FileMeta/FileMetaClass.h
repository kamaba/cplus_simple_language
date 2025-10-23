//****************************************************************************
//  File:      FileMeta.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "FileMetaBase.h"
#include "FileMetaCommon.h"
#include "../Token.h"
#include "../../Define.h"
#include "../../Debug/Log.h"
#include "../Parse/Node.h"
#include <vector>
#include <string>
#include <sstream>

namespace SimpleLanguage 
{
	namespace Core
	{
		class MetaClass;
	}
	namespace Compile
	{
		// Forward declarations
		class FileMeta;
		class FileMetaNamespace;
		class FileMetaMemberVariable;
		class FileMetaMemberFunction;
		class FileMetaMemberData;
		class FileMetaClassDefine;
		class FileMetaTemplateDefine;
		class NamespaceStatementBlock;
		class Node;

		class FileMetaClass : public FileMetaBase 
		{
			// ParseStructTemp helper class
			class ParseStructTemp {
			public:
				ParseStructTemp* parentPSt = nullptr;
				Node* nameNode = nullptr;
				Node* angleNode = nullptr;
				::std::vector<ParseStructTemp*> angleContentNodeList;

				void AddParseStructTemplate(ParseStructTemp* pst);
				void GenFileInputTemplateNode(Node* node, FileMeta* fm);
			};
		public:
			FileMetaClass(FileMeta* fm, const ::std::vector<Node*>& listNode);
			virtual ~FileMetaClass() = default;

			// Properties
			bool InnerClass() const { return m_InnerClass; }
			void SetInnerClass(bool value) { m_InnerClass = value; }

			bool IsConst() const { return m_ConstToken != nullptr; }
			bool IsStatic() const { return m_StaticToken != nullptr; }
			bool IsEnum() const { return m_EnumToken != nullptr; }
			bool IsData() const { return m_DataToken != nullptr; }
			bool IsPartial() const { return m_PartialToken != nullptr; }

			Token* GetPreInterfaceToken() const { return m_PreInterfaceToken; }
			Core::MetaClass* GetMetaClass() const { return m_MetaClass; }
			FileMetaClassDefine* GetFileMetaExtendClass() const { return m_FileMetaExtendClass; }
			const ::std::vector<FileMetaClassDefine*>& GetInterfaceClassList() const { return m_InterfaceClassList; }
			FileMetaNamespace* GetTopLevelFileMetaNamespace() const { return m_TopLevelFileMetaNamespace; }
			FileMetaClass* GetTopLevelFileMetaClass() const { return m_TopLevelFileMetaClass; }
			const ::std::vector<FileMetaTemplateDefine*>& GetTemplateDefineList() const { return m_TemplateDefineList; }
			NamespaceStatementBlock* GetNamespaceBlock() const { return m_NamespaceBlock; }
			const ::std::vector<FileMetaMemberVariable*>& GetMemberVariableList() const { return m_MemberVariableList; }
			const ::std::vector<FileMetaMemberFunction*>& GetMemberFunctionList() const { return m_MemberFunctionList; }
			const ::std::vector<FileMetaMemberData*>& GetMemberDataList() const { return m_MemberDataList; }

			// Methods
			void AddFileMemberData(FileMetaMemberData* fmmd);
			FileMetaMemberData* GetFileMemberData(const ::std::string& name);
			void AddFileMemberVariable(FileMetaMemberVariable* fmv);
			void AddFileMemberFunction(FileMetaMemberFunction* fmmf);
			void SetMetaNamespace(FileMetaNamespace* mn);
			void AddExtendMetaNamespace(FileMetaNamespace* fmn);
			void SetPartialToken(Token* partialToken);
			void SetPermissionToken(Token* permissionToken);
			void SetMetaClass(Core::MetaClass* mc);
			void AddFileMetaClass(FileMetaClass* fmc);
			void AddInterfaceClass(FileMetaClassDefine* fmcv);
			virtual void SetDeep(int _deep) override;
			virtual ::std::string ToString() const;
			virtual ::std::string ToFormatString() const override;

		private:
			bool Parse();
			int ReadClassDefineStruct(int cAddCount, const ::std::vector<Node*>& m_NodeList, ::std::vector<FileMetaClassDefine*>& fcdList);
			void SetFileMetaClass(FileMetaClass* fmc);

			// Token members
			Token* m_PermissionToken = nullptr;
			Token* m_PartialToken = nullptr;
			Token* m_PreInterfaceToken = nullptr;
			Token* m_SufInterfaceToken = nullptr;
			Token* m_ClassToken = nullptr;
			Token* m_EnumToken = nullptr;
			Token* m_DataToken = nullptr;
			Token* m_ConstToken = nullptr;
			Token* m_StaticToken = nullptr;

			// Class members
			Core::MetaClass* m_MetaClass = nullptr;
			FileMetaNamespace* m_TopLevelFileMetaNamespace = nullptr;
			FileMetaClass* m_TopLevelFileMetaClass = nullptr;
			FileMetaClassDefine* m_FileMetaExtendClass = nullptr;
			::std::vector<FileMetaClassDefine*> m_InterfaceClassList;
			::std::vector<FileMetaClass*> m_ChildrenClassList;
			::std::vector<FileMetaTemplateDefine*> m_TemplateDefineList;
			::std::vector<FileMetaMemberVariable*> m_MemberVariableList;
			::std::vector<FileMetaMemberFunction*> m_MemberFunctionList;
			::std::vector<FileMetaMemberData*> m_MemberDataList;
			NamespaceStatementBlock* m_NamespaceBlock = nullptr;
			::std::vector<Node*> m_NodeList;
			::std::ostringstream stringBuilder;
			bool m_InnerClass = false;
		};
	} // namespace Compile
} // namespace SimpleLanguage

