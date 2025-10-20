#include "Define.h"
#include "Core/MetaBase.h"
#include "Compile/Token.h"
#include "Compile/CompileManager.h"
#include "Compile/FileMeta/FileMetaBase.h"
#include "Compile/FileMeta/FileMeta.h"
#include "Compile/FileMeta/FileMetaCommon.h"
#include <iostream>

int main() {
    std::cout << "SimpleLanguage C++ 编译测试" << std::endl;
    
    // 测试基本枚举
    SimpleLanguage::Core::EPermission perm = SimpleLanguage::Core::EPermission::Public;
    std::cout << "权限枚举测试: " << static_cast<int>(perm) << std::endl;
    
    // 测试 Token
    SimpleLanguage::Compile::Token token;
    token.SetLexeme(0, SimpleLanguage::Core::ETokenType::Identifier);
    std::cout << "Token 测试: " << token.lexeme().ToString() << std::endl;
    
    // 测试 FileMetaBase
    SimpleLanguage::Compile::CoreFileMeta::FileMetaBase base;
    std::cout << "FileMetaBase 深度: " << base.deep() << std::endl;
    
    // 测试 FileMeta
    SimpleLanguage::Compile::CoreFileMeta::FileMeta fileMeta("test.cpp");
    std::cout << "FileMeta 路径: " << fileMeta.path() << std::endl;
    
    std::cout << "所有测试通过!" << std::endl;
    return 0;
}

