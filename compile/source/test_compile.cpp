#include "Define.h"
#include "Core/MetaBase.h"
#include "Compile/Token.h"
#include "Compile/CompileManager.h"
#include "Compile/FileMeta/FileMetaBase.h"
#include "Compile/FileMeta/FileMeta.h"
#include "Compile/FileMeta/FileMetaCommon.h"
#include <iostream>

int main() {
    std::cout << "SimpleLanguage C++ ±àÒë²âÊÔ" << std::endl;
    
    // ²âÊÔ»ù±¾Ã¶¾Ù
    SimpleLanguage::Core::EPermission perm = SimpleLanguage::Core::EPermission::Public;
    std::cout << "È¨ÏÞÃ¶¾Ù²âÊÔ: " << static_cast<int>(perm) << std::endl;
    
    // ²âÊÔ Token
    SimpleLanguage::Compile::Token token;
    token.SetLexeme(0, SimpleLanguage::Core::ETokenType::Identifier);
    std::cout << "Token ²âÊÔ: " << token.lexeme().ToString() << std::endl;
    
    // ²âÊÔ FileMetaBase
    SimpleLanguage::Compile::CoreFileMeta::FileMetaBase base;
    std::cout << "FileMetaBase Éî¶È: " << base.deep() << std::endl;
    
    // ²âÊÔ FileMeta
    SimpleLanguage::Compile::CoreFileMeta::FileMeta fileMeta("test.cpp");
    std::cout << "FileMeta Â·¾¶: " << fileMeta.path() << std::endl;
    
    std::cout << "ËùÓÐ²âÊÔÍ¨¹ý!" << std::endl;
    return 0;
}

