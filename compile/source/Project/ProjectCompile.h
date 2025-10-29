#pragma once


#include "../Define.h"
#include "../Debug/Log.h"
#include <string>
#include <vector>


namespace SimpleLanguage 
{
	namespace Compile
	{
		class LexerParse;
		class TokenParse;
		class StructParse;
		class FileMeta;
		class FileParse;
	}
	namespace Project
	{
		// Forward declarations
		class ProjectData;
		class ProjectParse;

		class ProjectCompile {
		public:
			ProjectCompile();
			virtual ~ProjectCompile() = default;

			// Properties
			bool IsThreadCompile() const { return m_IsThreadCompile; }
			void SetThreadCompile(bool value) { m_IsThreadCompile = value; }

			static bool IsLoaded() { return s_IsLoaded; }
			static Compile::FileMeta* ProjectFileMeta() { return s_ProjectFile; }

			static size_t StructParseCount() { return s_StructParseCount; }
			static size_t BuildParseCount() { return s_BuildParseCount; }
			static size_t GrammerParseCount() { return s_GrammerParseCount; }
			static size_t ParseListCount() { return s_ParseListCount; }
			static const std::vector<Compile::FileParse*>& FileParseList() { return s_FileParseList; }

			// Methods
			static void LoadProject();
			static void Compile(const std::string& path, ProjectData* pd);
			static void AddFileParse(const std::string& path);
			static bool CheckFileList();
			static void FileListStructParse();
			static void StructParseComplete();
			static void BuildParseComplete();
			static void GrammerParseComplete();
			static void Update();
			static void CompileFileAllEnd();

		private:
			bool m_IsThreadCompile = false;

			static bool s_IsLoaded;
			static std::string s_FileContentString;
			static Compile::FileMeta* s_ProjectFile;
			static Compile::LexerParse* s_LexerParse;
			static Compile::TokenParse* s_TokenParse;
			static Compile::StructParse* s_ProjectBuild;
			static ProjectParse* s_ProjectParse;
			static ProjectData* s_Data;
			static size_t s_StructParseCount;
			static size_t s_BuildParseCount;
			static size_t s_GrammerParseCount;
			static size_t s_ParseListCount;
			static std::vector<Compile::FileParse*> s_FileParseList;
		};

	} // namespace Project
} // namespace SimpleLanguage
