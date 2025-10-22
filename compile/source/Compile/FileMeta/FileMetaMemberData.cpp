#include "FileMetaMemberData.h"
#include "FileMeta.h"
#include "FileMetaExpress.h"
#include "../../Debug/Log.h"
#include "../../Define.h"
#include "../Parse/Node.h"
#include <algorithm>

namespace SimpleLanguage {
namespace Compile {

FileMetaMemberData::FileMetaMemberData(FileMeta* fm, Node* node, bool isWithName, EMemberDataType dataType)
    : m_MemberDataType(dataType), m_IsWithName(isWithName) {

    m_FileMeta = fm;
    m_Token = node->token;
    if (m_MemberDataType == EMemberDataType::ConstValue) {
        m_FileMetaConstValue = new FileMetaConstValueTerm(m_FileMeta, m_Token);
    } else if (dataType == EMemberDataType::Class) {
        m_FileMetaCallTermValue = new FileMetaCallTerm(m_FileMeta, node);
    }
}
FileMetaMemberData::FileMetaMemberData(FileMeta* fm, const std::vector<Node*>& frontList)
{
    std::vector<Node*> backList;
    FileMetaMemberData(fm, frontList, nullptr, backList, false, EMemberDataType::Class);
}

FileMetaMemberData::FileMetaMemberData(FileMeta* fm, const std::vector<Node*>& frontList, Node* assignNode, 
                                       const std::vector<Node*>& backList, bool isWithName, EMemberDataType dataType)
    :  m_MemberDataType(dataType), m_IsWithName(isWithName) {

    m_FileMeta = fm;
    if (m_IsWithName) {
        if (!frontList.empty()) {
            m_Token = frontList[0]->token;
        } else {
            SimpleLanguage::Debug::Log::AddInStructFileMeta(SimpleLanguage::Debug::EError::None, 
                "Error 带着名称FileMetaMemberData 但没有发现相关的token");
        }
    }
    
    if (dataType == EMemberDataType::Data) {
        // Implementation for Data type
    } else if (m_MemberDataType == EMemberDataType::Array) {
        // Implementation for Array type
    } else if (m_MemberDataType == EMemberDataType::ConstValue) {
        if (!backList.empty()) {
            Token* signalToken = nullptr;
            Token* valueToken = nullptr;
            for (Node* node : backList) {
                if (node->nodeType == SimpleLanguage::Compile::ENodeType::Symbol) {
                    signalToken = node->token;
                } else {
                    valueToken = node->token;
                }
            }
            m_FileMetaConstValue = new FileMetaConstValueTerm(m_FileMeta, valueToken, signalToken);
        }
    } else if (dataType == EMemberDataType::Class) {
        if (!backList.empty()) {
            m_FileMetaCallTermValue = new FileMetaCallTerm(m_FileMeta, backList[backList.size() - 1]);
        }
    }
}

void FileMetaMemberData::AddFileMemberData(FileMetaMemberData* fmmd) {
    m_FileMetaMemberData.push_back(fmmd);
}

void FileMetaMemberData::SetDeep(int _deep) {
    m_Deep = _deep;
    for (size_t i = 0; i < m_FileMetaMemberData.size(); i++) {
        m_FileMetaMemberData[i]->SetDeep(m_Deep + 1);
    }
}

void FileMetaMemberData::SetMetaMemberData(MetaMemberData* mmd) {
    // Implementation for setting meta member data
}

FileMetaMemberData* FileMetaMemberData::GetFileMetaMemberDataByName(const std::string& name) {
    auto it = std::find_if(m_FileMetaMemberData.begin(), m_FileMetaMemberData.end(),
        [&name](FileMetaMemberData* fmmd) { return fmmd->Name() == name; });
    
    if (it != m_FileMetaMemberData.end()) {
        return *it;
    }
    return nullptr;
}

std::string FileMetaMemberData::ToFormatString() const {
    std::ostringstream sb;

    for (int i = 0; i < Deep(); i++)
        sb << SimpleLanguage::Global::tabChar;
    if (m_IsWithName) {
        sb << Name() << " = ";
    }
    if (m_MemberDataType == EMemberDataType::Data) {
        sb << std::endl << "{";
        for (size_t i = 0; i < m_FileMetaMemberData.size(); i++) {
            sb << std::endl << m_FileMetaMemberData[i]->ToFormatString();
        }
        for (int i = 0; i < Deep(); i++)
            sb << SimpleLanguage::Global::tabChar;
        sb << "}";
    } else if (m_MemberDataType == EMemberDataType::Array) {
        sb << "[";
        for (size_t i = 0; i < m_FileMetaMemberData.size(); i++) {
            sb << m_FileMetaMemberData[i]->ToFormatString();
            if (i < m_FileMetaMemberData.size() - 1) {
                sb << ",";
            }
        }
        sb << "]";
    } else if (m_MemberDataType == EMemberDataType::ConstValue) {
        sb << (m_FileMetaConstValue ? m_FileMetaConstValue->ToFormatString() : "");
    } else if (m_MemberDataType == EMemberDataType::Class) {
        sb << (m_FileMetaCallTermValue ? m_FileMetaCallTermValue->ToFormatString() : "");
    }

    return sb.str();
}

std::string FileMetaMemberData::ToString() const {
    std::ostringstream sb;
    if (m_IsWithName) {
        sb << Name() << " = " << std::endl;
    }
    if (m_MemberDataType == EMemberDataType::Data) {
        sb << std::endl << "{";
        for (size_t i = 0; i < m_FileMetaMemberData.size(); i++) {
            sb << m_FileMetaMemberData[i]->ToString();
        }
        sb << "}";
    } else if (m_MemberDataType == EMemberDataType::Array) {
        sb << "[";
        for (size_t i = 0; i < m_FileMetaMemberData.size(); i++) {
            sb << m_FileMetaMemberData[i]->ToFormatString();
            if (i < m_FileMetaMemberData.size() - 1) {
                sb << ",";
            }
        }
        sb << "]";
    } else if (m_MemberDataType == EMemberDataType::ConstValue) {
        sb << (m_FileMetaConstValue ? m_FileMetaConstValue->ToFormatString() : "");
    } else if (m_MemberDataType == EMemberDataType::Class) {
        sb << (m_FileMetaCallTermValue ? m_FileMetaCallTermValue->ToFormatString() : "");
    }
    return sb.str();
}

} // namespace Compile
} // namespace SimpleLanguage
