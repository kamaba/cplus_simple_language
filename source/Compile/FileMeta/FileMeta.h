//****************************************************************************
//  File:      FileMeta.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "FileMetaBase.h"
#include <vector>
#include <string>

namespace SimpleLanguage
{
	namespace Core
	{
		class MetaNamespace;
		class MetaNode;
	}
	namespace Compile
	{
		// Forward declarations
		class FileMetaClass;
		class FileMetaImportSyntax;
		class FileMetaNamespace;

		class FileMeta : public FileMetaBase 
		{
		public:
			FileMeta(const ::std::string& p);
			virtual ~FileMeta() = default;

			// Properties
			::std::string Path() const { return m_Path; }
			const ::std::vector<FileMetaClass*>& FileMetaClassList() const { return m_FileMetaClassList; }

			// Methods
			void AddFileImportSyntax(FileMetaImportSyntax* iss);
			void AddFileDefineNamespace(FileMetaNamespace* fdn);
			void AddFileSearchNamespace(FileMetaNamespace* fdn);
			void AddFileMetaAllNamespace(FileMetaNamespace* fmn);
			void AddFileMetaAllClass(FileMetaClass* fmc);
			void AddImportMetaNamespace(SimpleLanguage::Core::MetaNamespace* mn);
			FileMetaClass* GetFileMetaClassByName(const ::std::string& name);
			void AddFileMetaClass(FileMetaClass* mc);
			SimpleLanguage::Core::MetaNode* GetMetaBaseByFileMetaClassRef(FileMetaClassDefine* fmcv);
			SimpleLanguage::Core::MetaNode* GetMetaNodeByNamespace(SimpleLanguage::Core::MetaNode* namespaceMN, const ::std::vector<::std::string>& classList);
			SimpleLanguage::Core::MetaNode* GetMetaNodeFileMetaClass(const ::std::vector<::std::string>& classList);
			SimpleLanguage::Core::MetaNode* GetMetaNodeByImportNamespace(const ::std::string& name);
			void CreateNamespace();
			void CombineFileMeta();
			void ParseInface();
			virtual void SetDeep(int _deep) override;
			virtual ::std::string ToFormatString() const override;

		private:
			::std::string m_Path;
			::std::vector<FileMetaImportSyntax*> m_FileImportSyntax;
			::std::vector<FileMetaNamespace*> m_FileDefineNamespaceList;
			::std::vector<FileMetaNamespace*> m_FileSearchNamespaceList;
			::std::vector<FileMetaClass*> m_FileMetaClassList;
			::std::vector<FileMetaNamespace*> m_FileMetaAllNamespaceList;
			::std::vector<FileMetaClass*> m_FileMetaAllClassList;
			::std::vector<SimpleLanguage::Core::MetaNamespace*> m_ImportMetaNamespaceList;
		};
	} // namespace Compile
} // namespace SimpleLanguage

