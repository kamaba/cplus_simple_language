#pragma once

#include "../Define.h"
#include "../Debug/Log.h"
#include "../Parse/StructParse.h"
#include "FileMeta/FileMetaCommon.h"
#include "FileMeta/FileMetaExpress.h"
#include "../Parse/Node.h"
#include <vector>
#include <string>

namespace SimpleLanguage {
namespace Compile {

// Forward declarations
class FileMeta;
class FileMetaBaseTerm;
class FileMetaTermExpress;
class FileMetaCallTerm;
class FileMetaConstValueTerm;
class FileMetaParTerm;
class FileMetaBraceTerm;
class FileMetaBracketTerm;

class FileMetatUtil {
public:
    static std::vector<std::string> GetLinkStringMidPeriodList(const std::vector<Token*>& tokenList);
    static bool IsSymbol(Token* token);
    static bool SplitNodeList(const std::vector<Node*>& nodeList, std::vector<Node*>& preNodeList, 
                              std::vector<Node*>& afterNodeList, Token*& assignToken);
    static FileMetaBaseTerm* CreateFileOneTerm(FileMeta* fm, Node* node, FileMetaTermExpress::EExpressType expressType);
    static FileMetaBaseTerm* CreateFileMetaExpress(FileMeta* fm, const std::vector<Node*>& nodeList, 
                                                   FileMetaTermExpress::EExpressType expressType);

private:
    FileMetatUtil() = default;
    ~FileMetatUtil() = default;
};

} // namespace Compile
} // namespace SimpleLanguage

