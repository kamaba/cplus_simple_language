#include "CompilerUtil.h"
//#include "../Core/GrammerUtil.h"
#include <sstream>
#include <algorithm>

namespace SimpleLanguage {
namespace Compile {

std::ostringstream CompilerUtil::tempBuild;

bool CompilerUtil::CheckNameList(const std::string& ns, std::vector<std::string>* list) {
    std::vector<std::string> nsArr;
    std::istringstream iss(ns);
    std::string token;
    
    while (std::getline(iss, token, '.')) {
        nsArr.push_back(token);
    }
    
    if (nsArr.empty()) {
        std::cout << "命名空间名称不能为空字符串" << std::endl;
        return false;
    }
    
    if (nsArr.size() == 1) {
        bool isSuc = false;// SimpleLanguage::Core::GrammerUtil::IdentifierCheck(nsArr[0]);
        if (isSuc && list != nullptr) {
            list->push_back(nsArr[0]);
        }
        return isSuc;
    }
    
    bool success = true;
    for (size_t i = 0; i < nsArr.size(); i++) {
        if (nsArr[i].empty()) {
            success = false;
            break;
        }
        //if (!SimpleLanguage::Core::GrammerUtil::IdentifierCheck(nsArr[i]))
        {
            success = false;
            break;
        }
        if (list != nullptr) {
            list->push_back(nsArr[i]);
        }
    }
    return success;
}

std::string CompilerUtil::ToFormatString(SimpleLanguage::EPermission permission) {
    switch (permission) {
        case SimpleLanguage::EPermission::Public: return "public";
        case SimpleLanguage::EPermission::Export: return "export";
        case SimpleLanguage::EPermission::Protected: return "protected";
        case SimpleLanguage::EPermission::Private: return "private";
        default: return "_public";
    }
}

SimpleLanguage::EPermission CompilerUtil::GetPerMissionByString(const std::string& str) {
    if (str == "public") return SimpleLanguage::EPermission::Public;
    if (str == "export") return SimpleLanguage::EPermission::Export;
    if (str == "protected") return SimpleLanguage::EPermission::Protected;
    if (str == "private") return SimpleLanguage::EPermission::Private;
    return SimpleLanguage::EPermission::Null;
}

} // namespace Compile
} // namespace SimpleLanguage

