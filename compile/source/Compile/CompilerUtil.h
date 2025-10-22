#pragma once

#include "../Define.h"
#include "../Debug/Log.h"
#include <string>
#include <vector>
#include <sstream>

namespace SimpleLanguage {
namespace Compile {

class CompilerUtil {
public:
    static std::ostringstream tempBuild;

    static bool CheckNameList(const std::string& ns, std::vector<std::string>* list = nullptr);
    static std::string ToFormatString(SimpleLanguage::EPermission permission);
    static SimpleLanguage::EPermission GetPerMissionByString(const std::string& str);

private:
    CompilerUtil() = default;
    ~CompilerUtil() = default;
};

} // namespace Compile
} // namespace SimpleLanguage

