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
		class ProjectParse;
		class FileMeta;
		class FileParse;
	}
	namespace Project
	{
		// Forward declarations
		class ProjectData;

		class ProjectCompile {
		public:
			ProjectCompile();
			virtual ~ProjectCompile() = default;

			// Properties
			bool IsThreadCompile() const { return m_IsThreadCompile; }
			void SetThreadCompile(bool value) { m_IsThreadCompile = value; }

			static bool IsLoaded() { return s_IsLoaded; }
			static Compile::FileMeta* ProjectFileMeta() { return s_ProjectFile; }

			static int StructParseCount() { return s_StructParseCount; }
			static int BuildParseCount() { return s_BuildParseCount; }
			static int GrammerParseCount() { return s_GrammerParseCount; }
			static int ParseListCount() { return s_ParseListCount; }
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
			static FileMeta* s_ProjectFile;
			static LexerParse* s_LexerParse;
			static TokenParse* s_TokenParse;
			static StructParse* s_ProjectBuild;
			static ProjectParse* s_ProjectParse;
			static ProjectData* s_Data;
			static int s_StructParseCount;
			static int s_BuildParseCount;
			static int s_GrammerParseCount;
			static int s_ParseListCount;
			static std::vector<FileParse*> s_FileParseList;
		};

	} // namespace Project
} // namespace SimpleLanguage
