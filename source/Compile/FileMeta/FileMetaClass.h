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
		class MetaClass;

		class FileMetaClass : public FileMetaBase 
		{
		public:
			FileMetaClass(FileMeta* fm, const ::std::vector<Node*>& listNode);
			virtual ~FileMetaClass() = default;

			// Properties
			bool innerClass() const { return m_InnerClass; }
			void setInnerClass(bool value) { m_InnerClass = value; }

			bool isConst() const { return m_ConstToken != nullptr; }
			bool isStatic() const { return m_StaticToken != nullptr; }
			bool isEnum() const { return m_EnumToken != nullptr; }
			bool isData() const { return m_DataToken != nullptr; }
			bool isPartial() const { return m_PartialToken != nullptr; }

			Token* preInterfaceToken() const { return m_PreInterfaceToken; }
			MetaClass* metaClass() const { return m_MetaClass; }
			FileMetaClassDefine* fileMetaExtendClass() const { return m_FileMetaExtendClass; }
			const ::std::vector<FileMetaClassDefine*>& interfaceClassList() const { return m_InterfaceClassList; }
			FileMetaNamespace* topLevelFileMetaNamespace() const { return m_TopLevelFileMetaNamespace; }
			FileMetaClass* topLevelFileMetaClass() const { return m_TopLevelFileMetaClass; }
			const ::std::vector<FileMetaTemplateDefine*>& templateDefineList() const { return m_TemplateDefineList; }
			NamespaceStatementBlock* namespaceBlock() const { return m_NamespaceBlock; }
			const ::std::vector<FileMetaMemberVariable*>& memberVariableList() const { return m_MemberVariableList; }
			const ::std::vector<FileMetaMemberFunction*>& memberFunctionList() const { return m_MemberFunctionList; }
			const ::std::vector<FileMetaMemberData*>& memberDataList() const { return m_MemberDataList; }

			// Methods
			void AddFileMemberData(FileMetaMemberData* fmmd);
			FileMetaMemberData* GetFileMemberData(const ::std::string& name);
			void AddFileMemberVariable(FileMetaMemberVariable* fmv);
			void AddFileMemberFunction(FileMetaMemberFunction* fmmf);
			void SetMetaNamespace(FileMetaNamespace* mn);
			void AddExtendMetaNamespace(FileMetaNamespace* fmn);
			void SetPartialToken(Token* partialToken);
			void SetPermissionToken(Token* permissionToken);
			void SetMetaClass(MetaClass* mc);
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
			MetaClass* m_MetaClass = nullptr;
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
		};
	} // namespace Compile
} // namespace SimpleLanguage

