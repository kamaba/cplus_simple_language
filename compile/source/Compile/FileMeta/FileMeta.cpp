//****************************************************************************
//  File:      FileMeta.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "FileMeta.h"
#include "FileMetaClass.h"
#include "FileMetaNamespace.h"
#include "FileMetaGlobalSyntax.h"
#include "../../Core/MetaNamespace.h"
#include "../../Core/MetaNode.h"
#include "../../Core/ClassManager.h"
#include "../../Core/NamespaceManager.h"
#include "../../Debug/Log.h"
#include "../../Define.h"
#include "../../Project/ProjectManager.h"
#include "../../Project/ProjectData.h"
//#include "../../OtherLanguage/CSharp/CSharpManager.h"
#include <algorithm>
#include <sstream>

namespace SimpleLanguage 
{
	namespace Compile 
	{
		FileMeta::FileMeta(const ::std::string& p) : m_Path(p)
		{
		}

		void FileMeta::AddFileImportSyntax(FileMetaImportSyntax* iss) 
		{
			iss->SetFileMeta(this);
			m_FileImportSyntax.push_back(iss);
		}

		void FileMeta::AddFileDefineNamespace(FileMetaNamespace* fdn)
		{
			fdn->SetFileMeta(this);
			m_FileDefineNamespaceList.push_back(fdn);
		}

		void FileMeta::AddFileSearchNamespace(FileMetaNamespace* fdn)
		{
			fdn->SetFileMeta(this);
			m_FileSearchNamespaceList.push_back(fdn);
		}

		void FileMeta::AddFileMetaAllNamespace(FileMetaNamespace* fmn) 
		{
			fmn->SetFileMeta(this);
			m_FileMetaAllNamespaceList.push_back(fmn);
		}

		void FileMeta::AddFileMetaAllClass(FileMetaClass* fmc)
		{
			fmc->SetFileMeta(this);
			m_FileMetaAllClassList.push_back(fmc);
		}

		void FileMeta::AddImportMetaNamespace(SimpleLanguage::Core::MetaNamespace* mn) 
		{
			auto it = ::std::find(m_ImportMetaNamespaceList.begin(), m_ImportMetaNamespaceList.end(), mn);
			if (it == m_ImportMetaNamespaceList.end()) {
				m_ImportMetaNamespaceList.push_back(mn);
			}
		}

		FileMetaClass* FileMeta::GetFileMetaClassByName(const ::std::string& name)
		{
			auto it = ::std::find_if(m_FileMetaAllClassList.begin(), m_FileMetaAllClassList.end(),
				[&name](FileMetaClass* fmc) { return fmc->GetName() == name; });

			if (it != m_FileMetaAllClassList.end()) {
				return *it;
			}
			return nullptr;
		}

		void FileMeta::AddFileMetaClass(FileMetaClass* mc) 
		{
			m_FileMetaClassList.push_back(mc);
		}

		SimpleLanguage::Core::MetaNode* FileMeta::GetMetaBaseByFileMetaClassRef(FileMetaClassDefine* fmcv) 
		{
			SimpleLanguage::Core::MetaNode* mb = nullptr;
			for (size_t i = 0; i < m_ImportMetaNamespaceList.size(); i++) {
				SimpleLanguage::Core::MetaNode* mn = GetMetaNodeFileMetaClass(fmcv->GetStringList());
				if (mn == nullptr) continue;
				if (mn->IsMetaNamespace() == false) { continue; }
				if (mn->GetMetaNamespace()->GetRefFromType() == SimpleLanguage::Core::RefFromType::CSharp) {
					// Object obj = CSharpManager.GetObject(fmcv, mn.metaNamespace);
				}
				else {
					mb = fmcv->GetChildrenMetaNode(mn);
					if (mb != nullptr)
						return mb;
				}
			}
			return nullptr;
		}

		SimpleLanguage::Core::MetaNode* FileMeta::GetMetaNodeByNamespace(SimpleLanguage::Core::MetaNode* namespaceMN, const ::std::vector<::std::string>& classList) {
			SimpleLanguage::Core::MetaNode* mb = namespaceMN;
			for (size_t j = 0; j < classList.size(); j++) {
				::std::string cname = classList[j];
				mb = mb->GetChildrenMetaNodeByName(cname);
				if (mb == nullptr) {
					if (namespaceMN->GetName() == cname && (namespaceMN->IsMetaModule() || namespaceMN->IsMetaNamespace())) {
						mb = namespaceMN;
						continue;
					}
					break;
				}
			}
			return mb;
		}

		SimpleLanguage::Core::MetaNode* FileMeta::GetMetaNodeFileMetaClass(const ::std::vector<::std::string>& classList)
		{
			if (classList.empty()) return nullptr;

			SimpleLanguage::Core::MetaNode* mb = nullptr;
			SimpleLanguage::Core::MetaNode* searchMN = nullptr;
			for (size_t i = 0; i < m_ImportMetaNamespaceList.size(); i++) {
				searchMN = m_ImportMetaNamespaceList[i]->GetMetaNode();

				while (searchMN != nullptr) {
					mb = GetMetaNodeByNamespace(searchMN, classList);
					if (mb == nullptr) {
						searchMN = searchMN->GetParentNode();
					}
					else {
						break;
					}
				}

				if (mb != nullptr) {
					break;
				}
			}
			return mb;
		}

		SimpleLanguage::Core::MetaNode* FileMeta::GetMetaNodeByImportNamespace(const ::std::string& name)
		{
			for (size_t i = 0; i < m_ImportMetaNamespaceList.size(); i++) {
				auto imn = m_ImportMetaNamespaceList[i];
				if (imn->GetRefFromType() == SimpleLanguage::Core::RefFromType::CSharp) {
					SimpleLanguage::Core::MetaNode* getmb = nullptr;// SimpleLanguage::OtherLanguage::CSharp::CSharpManager::FindCSharpClassOrNameSpace(imn->GetName(), name);
					if (getmb != nullptr)
						return getmb;
				}
				else {
					return imn->GetMetaNode()->GetChildrenMetaNodeByName(name);
				}
			}
			return nullptr;
		}

		void FileMeta::CreateNamespace()
		{
			for (size_t i = 0; i < m_FileDefineNamespaceList.size(); i++)
			{
				auto fmn = m_FileDefineNamespaceList[i];
				SimpleLanguage::Core::NamespaceManager::GetInstance().CreateMetaNamespaceByFineDefineNamespace(fmn);
			}
			for (size_t i = 0; i < m_FileSearchNamespaceList.size(); i++) {
				auto fmn = m_FileSearchNamespaceList[i];
				if (SimpleLanguage::Project::ProjectManager::useDefineNamespaceType != SimpleLanguage::Project::EUseDefineType::NoUseProjectConfigNamespace) {
					if (!(SimpleLanguage::Project::ProjectManager::GetData()->IsIncludeDefineStruct(fmn->GetNamespaceStatementBlock()->NamespaceList()))) {
						SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error 暂不允许使用namespace 定义命名空间!!!" + fmn->ToFormatString() + " 位置: " + fmn->GetToken()->ToLexemeAllString());
					}
				}
				SimpleLanguage::Core::NamespaceManager::GetInstance().CreateMetaNamespaceByFineDefineNamespace(fmn);
			}
		}

		void FileMeta::CombineFileMeta() 
		{
			for (size_t i = 0; i < m_FileImportSyntax.size(); i++) 
			{
				m_FileImportSyntax[i]->ParseImportSyntax();
			}
			for (size_t i = 0; i < m_FileMetaAllClassList.size(); i++) {
				auto fns = m_FileMetaAllClassList[i];
				for (size_t j = 0; j < m_FileSearchNamespaceList.size(); j++) {
					fns->AddExtendMetaNamespace(m_FileSearchNamespaceList[j]);
				}
				SimpleLanguage::Core::ClassManager::GetInstance().AddClass(fns);
			}
		}

		void FileMeta::ParseInface() {
			// Implementation for interface parsing
		}

		void FileMeta::SetDeep(int _deep) {
			m_Deep = _deep;
			for (size_t i = 0; i < m_FileImportSyntax.size(); i++) 
			{
				m_FileImportSyntax[i]->SetDeep(m_Deep);
			}
			for (size_t i = 0; i < m_FileDefineNamespaceList.size(); i++) {
				m_FileDefineNamespaceList[i]->SetDeep(m_Deep);
			}
			for (size_t i = 0; i < m_FileSearchNamespaceList.size(); i++) {
				m_FileSearchNamespaceList[i]->SetDeep(m_Deep);
			}
			for (size_t i = 0; i < m_FileMetaClassList.size(); i++) {
				m_FileMetaClassList[i]->SetDeep(m_Deep);
			}
		}

		::std::string FileMeta::ToFormatString() const {
			::std::ostringstream sb;
			sb << "-------------------FileMeta 文件显示 开始 : Path: " << m_Path << "-----------------------" << ::std::endl;

			for (size_t i = 0; i < m_FileImportSyntax.size(); i++) {
				sb << m_FileImportSyntax[i]->ToFormatString() << ::std::endl;
			}
			for (size_t i = 0; i < m_FileDefineNamespaceList.size(); i++) {
				sb << m_FileDefineNamespaceList[i]->ToFormatString() << ::std::endl;
			}
			for (size_t i = 0; i < m_FileSearchNamespaceList.size(); i++) {
				sb << m_FileSearchNamespaceList[i]->ToFormatString() << ::std::endl;
			}
			for (size_t i = 0; i < m_FileMetaClassList.size(); i++) {
				sb << m_FileMetaClassList[i]->ToFormatString() << ::std::endl;
			}
			sb << "-------------------FileMeta 文件显示 结束 : -----------------------" << ::std::endl;

			return sb.str();
		}
	} // namespace Compile
} // namespace SimpleLanguage

