//****************************************************************************
//  File:      FileMeta.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "FileMetaNamespace.h"
#include "FileMeta.h"
#include "FileMetaClass.h"
#include "../Parse/Node.h"
#include "../../Debug/Log.h"
#include "../../Define.h"
#include <sstream>

namespace SimpleLanguage {
namespace Compile {

FileMetaNamespace::FileMetaNamespace(FileMetaNamespace* fmn) {
    m_NamespaceNode = fmn->m_NamespaceNode;
    m_NamespaceNameNode = fmn->m_NamespaceNameNode;
}

FileMetaNamespace::FileMetaNamespace(Node* namespaceNode, Node* namespaceNameNode) 
    : m_NamespaceNode(namespaceNode), m_NamespaceNameNode(namespaceNameNode) {
    
    if (namespaceNode == nullptr) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error 在解析namespace 中，没有找到namespace设置的名称!!");
        return;
    }

    Node* blockNode = namespaceNode->blockNode;

    m_Token = m_NamespaceNode->token;
    m_IsSearchNamespace = true;
    if (blockNode != nullptr) {
        m_BraceBeginToken = blockNode->token;
        m_BraceEndToken = blockNode->endToken;
        m_IsSearchNamespace = false;
    }
    m_NamespaceStateBlock = NamespaceStatementBlock::CreateStateBlock(m_NamespaceNameNode->GetLinkTokenList());
}

std::string FileMetaNamespace::Name() const {
    if (m_NamespaceStateBlock != nullptr) {
        return m_NamespaceStateBlock->NamespaceString();
    }
    return "";
}

std::stack<FileMetaNamespace*> FileMetaNamespace::NamespaceStack() const {
    s_MetaNamespaceStack = std::stack<FileMetaNamespace*>();
    auto t = topLevelFileMetaNamespace;
    while (t != nullptr) {
        s_MetaNamespaceStack.push(t);
        t = t->topLevelFileMetaNamespace;
    }
    s_MetaNamespaceStack.push(const_cast<FileMetaNamespace*>(this));

    return s_MetaNamespaceStack;
}

FileMetaNamespace* FileMetaNamespace::AddFileNamespace(FileMetaNamespace* dln) {
    dln->topLevelFileMetaNamespace = this;
    m_MetaNamespaceList.push_back(dln);
    dln->m_Deep = Deep() + 1;

    return dln;
}

void FileMetaNamespace::AddFileMetaClass(FileMetaClass* mc) {
    mc->SetMetaNamespace(this);
    m_ChildrenClassList.push_back(mc);
}

std::string FileMetaNamespace::ToString() const {
    return "namespace " + Name() + "{}";
}

void FileMetaNamespace::SetDeep(int _deep) {
    m_Deep = _deep;
    for (size_t i = 0; i < m_MetaNamespaceList.size(); i++) {
        m_MetaNamespaceList[i]->SetDeep(_deep + 1);
    }
    for (size_t i = 0; i < m_ChildrenClassList.size(); i++) {
        m_ChildrenClassList[i]->SetDeep(_deep + 1);
    }
}

std::string FileMetaNamespace::ToFormatString() const {
    std::ostringstream sb;
    /*for (int i = 0; i < deep(); i++)
        sb << SimpleLanguage::Global::tabChar;
    sb << m_Token->lexeme().ToString() << " " << m_NamespaceStateBlock->ToFormatString();
    sb << std::endl;
    for (int i = 0; i < deep(); i++)
        sb << SimpleLanguage::Global::tabChar;
    sb << "{" << std::endl;
    for (size_t i = 0; i < m_MetaNamespaceList.size(); i++) {
        sb << m_MetaNamespaceList[i]->ToFormatString() << std::endl;
    }
    for (size_t i = 0; i < m_ChildrenClassList.size(); i++) {
        sb << m_ChildrenClassList[i]->ToFormatString() << std::endl;
    }
    for (int i = 0; i < deep(); i++)
        sb << SimpleLanguage::Global::tabChar;
    sb << "}";*/

    return sb.str();
}

} // namespace Compile
} // namespace SimpleLanguage

