#include "Define.h"
#include "Core/MetaBase.h"
#include "Compile/Token.h"
#include "Compile/CompileManager.h"
#include "Compile/FileMeta/FileMetaBase.h"
#include "Compile/FileMeta/FileMeta.h"
#include "Compile/FileMeta/FileMetaCommon.h"
#include <iostream>

int main() {
    std::cout << "SimpleLanguage C++ �������" << std::endl;
    
    // ���Ի���ö��
    SimpleLanguage::Core::EPermission perm = SimpleLanguage::Core::EPermission::Public;
    std::cout << "Ȩ��ö�ٲ���: " << static_cast<int>(perm) << std::endl;
    
    // ���� Token
    SimpleLanguage::Compile::Token token;
    token.SetLexeme(0, SimpleLanguage::Core::ETokenType::Identifier);
    std::cout << "Token ����: " << token.lexeme().ToString() << std::endl;
    
    // ���� FileMetaBase
    SimpleLanguage::Compile::CoreFileMeta::FileMetaBase base;
    std::cout << "FileMetaBase ���: " << base.deep() << std::endl;
    
    // ���� FileMeta
    SimpleLanguage::Compile::CoreFileMeta::FileMeta fileMeta("test.cpp");
    std::cout << "FileMeta ·��: " << fileMeta.path() << std::endl;
    
    std::cout << "���в���ͨ��!" << std::endl;
    return 0;
}

