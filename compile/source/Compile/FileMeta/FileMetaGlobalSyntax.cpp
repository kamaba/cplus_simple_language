#include "FileMetaGlobalSyntax.h"
#include "FileMeta.h"
#include "FileMetaNamespace.h"
#include "FileMetaClass.h"
#include "FileMetatUtil.h"
#include "../../Debug/Log.h"
#include "../../Define.h"
#include <algorithm>

namespace SimpleLanguage {
namespace Compile {

// FileMetaImportSyntax implementation
FileMetaImportSyntax::FileMetaImportSyntax(const std::vector<Node*>& _nodeList) : m_NodeList(_nodeList) {
    ParseImportSyntax();
}

bool FileMetaImportSyntax::ParseImportSyntax() {
    if (m_NodeList.size() < 2) {
        SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, "Error import必须有2个节点!!");
        return false;
    }
    
    auto namespaceNode = m_NodeList[0];
    if (namespaceNode != nullptr && namespaceNode->token != nullptr && 
        namespaceNode->token->GetType() == SimpleLanguage::ETokenType::Import) {
        m_Token = namespaceNode->token;
    }
    
    auto namespaceNameNode = m_NodeList[1];
    m_ImportNameListToken = namespaceNameNode->GetLinkTokenList();
    
    // Parse namespace statement
    m_NamespaceStatement = NamespaceStatementBlock::CreateStateBlock(m_ImportNameListToken);
    
    // Check for 'as' keyword
    if (m_NodeList.size() > 2) {
        auto asNode = m_NodeList[2];
        if (asNode != nullptr && asNode->token != nullptr && 
            asNode->token->GetType() == SimpleLanguage::ETokenType::As) {
            m_AsToken = asNode->token;
            
            if (m_NodeList.size() > 3) {
                auto asNameNode = m_NodeList[3];
                if (asNameNode != nullptr && asNameNode->token != nullptr) {
                    m_AsNameToken = asNameNode->token;
                    std::vector<Token*> asNameList = {asNameNode->token};
                    m_AsNameStatement = NamespaceStatementBlock::CreateStateBlock(asNameList);
                }
            }
        }
    }
    
    return true;
}

std::string FileMetaImportSyntax::ToFormatString() const {
    std::ostringstream sb;
    sb << "import ";
    if (m_NamespaceStatement != nullptr) {
        sb << m_NamespaceStatement->ToFormatString();
    }
    if (m_AsToken != nullptr && m_AsNameToken != nullptr) {
        sb << " as " << m_AsNameToken->GetLexemeString();
    }
    return sb.str();
}

// FileMetaGlobalSyntax implementation
FileMetaGlobalSyntax::FileMetaGlobalSyntax(FileMeta* fm, const std::vector<Node*>& nodeList) 
    : m_NodeList(nodeList) {
    m_FileMeta = fm;
}

void FileMetaGlobalSyntax::AddImportSyntax(FileMetaImportSyntax* importSyntax) {
    m_ImportSyntaxList.push_back(importSyntax);
}

void FileMetaGlobalSyntax::AddNamespaceSyntax(FileMetaNamespace* namespaceSyntax) {
    m_NamespaceSyntaxList.push_back(namespaceSyntax);
}

void FileMetaGlobalSyntax::AddClassSyntax(FileMetaClass* classSyntax) {
    m_ClassSyntaxList.push_back(classSyntax);
}

std::string FileMetaGlobalSyntax::ToFormatString() const {
    std::ostringstream sb;
    
    // Format import statements
    for (auto import : m_ImportSyntaxList) {
        sb << import->ToFormatString() << std::endl;
    }
    
    // Format namespace statements
    for (auto ns : m_NamespaceSyntaxList) {
        sb << ns->ToFormatString() << std::endl;
    }
    
    // Format class statements
    for (auto cls : m_ClassSyntaxList) {
        sb << cls->ToFormatString() << std::endl;
    }
    
    return sb.str();
}

} // namespace Compile
} // namespace SimpleLanguage
