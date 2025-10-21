#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "FileMetaMemberVariable.h"

namespace SimpleLanguage {
namespace Compile {

// Forward declarations
class FileMeta;
class FileMetaConstValueTerm;
class FileMetaCallTerm;
class MetaMemberData;
class Node;

class FileMetaMemberData : public FileMetaBase {
public:
    enum class EMemberDataType {
        Data,
        Array,
        ConstValue,
        Class
    };

    FileMetaMemberData(FileMeta* fm, Node* node, bool isWithName, EMemberDataType dataType);
    FileMetaMemberData(FileMeta* fm, const std::vector<Node*>& frontList, Node* assignNode,
                       const std::vector<Node*>& backList, bool isWithName, EMemberDataType dataType);
    virtual ~FileMetaMemberData() = default;

    // Properties
    Token* nameToken() const { return m_Token; }
    const std::vector<FileMetaMemberData*>& fileMetaMemberData() const { return m_FileMetaMemberData; }
    FileMetaConstValueTerm* fileMetaConstValue() const { return m_FileMetaConstValue; }
    FileMetaCallTerm* fileMetaCallTermValue() const { return m_FileMetaCallTermValue; }
    bool isWithName() const { return m_IsWithName; }
    EMemberDataType DataType() const { return m_MemberDataType; }

    // Methods
    void AddFileMemberData(FileMetaMemberData* fmmd);
    virtual void SetDeep(int _deep) override;
    void SetMetaMemberData(MetaMemberData* mmd);
    FileMetaMemberData* GetFileMetaMemberDataByName(const std::string& name);
    virtual std::string ToFormatString() const override;
    virtual std::string ToString() const override;

private:
    Token* m_AssignToken = nullptr;
    std::vector<Node*> m_NodeList;
    std::vector<FileMetaMemberData*> m_FileMetaMemberData;
    EMemberDataType m_MemberDataType = EMemberDataType::Data;
    FileMetaConstValueTerm* m_FileMetaConstValue = nullptr;
    FileMetaCallTerm* m_FileMetaCallTermValue = nullptr;
    bool m_IsWithName = false;
};

} // namespace Compile
} // namespace SimpleLanguage
