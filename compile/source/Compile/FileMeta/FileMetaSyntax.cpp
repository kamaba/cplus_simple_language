#include "FileMetaSyntax.h"
#include "FileMetaExpress.h"
#include "FileMetaCommon.h"
#include "FileMeta.h"
#include "../Parse/Node.h"
#include "../Token.h"
#include "../../Debug/Log.h"
#include "../../Define.h"
#include <algorithm>
#include <sstream>

namespace SimpleLanguage {
namespace Compile {

// FileMetaSyntax implementation
FileMetaSyntax::FileMetaSyntax() {
}

int FileMetaSyntax::GetParseIndex() const {
    return m_ParseIndex;
}

void FileMetaSyntax::SetParseIndex(int value) {
    m_ParseIndex = value;
}

bool FileMetaSyntax::IsAppendSemiColon() const {
    return m_IsAppendSemiColon;
}

void FileMetaSyntax::SetAppendSemiColon(bool value) {
    m_IsAppendSemiColon = value;
}

const std::vector<FileMetaSyntax*>& FileMetaSyntax::GetFileMetaSyntax() const {
    return m_FileMetaSyntax;
}

bool FileMetaSyntax::IsNotEnd() const {
    return m_ParseIndex < static_cast<int>(m_FileMetaSyntax.size());
}

FileMetaSyntax* FileMetaSyntax::GetCurrentSyntaxAndMove(int moveIndex) {
    if (m_ParseIndex < static_cast<int>(m_FileMetaSyntax.size())) {
        FileMetaSyntax* fms = m_FileMetaSyntax[m_ParseIndex];
        m_ParseIndex += moveIndex;
        return fms;
    }
    return nullptr;
}

void FileMetaSyntax::AddFileMetaSyntax(FileMetaSyntax* fms) {
    m_FileMetaSyntax.push_back(fms);
}

std::string FileMetaSyntax::ToFormatString() const {
    return "";
}

// FileMetaConditionExpressSyntax implementation
FileMetaConditionExpressSyntax::FileMetaConditionExpressSyntax(FileMeta* fm, Token* ifToken, FileMetaBaseTerm* condition, FileMetaBlockSyntax* executeBlockSyntax) {
    m_FileMeta = fm;
    m_Token = ifToken;
    m_ConditionExpress = condition;
    m_ExecuteBlockSyntax = executeBlockSyntax;
}

FileMetaBaseTerm* FileMetaConditionExpressSyntax::GetConditionExpress() const {
    return m_ConditionExpress;
}

FileMetaBlockSyntax* FileMetaConditionExpressSyntax::GetExecuteBlockSyntax() const {
    return m_ExecuteBlockSyntax;
}

void FileMetaConditionExpressSyntax::SetDeep(int deep) {
    m_Deep = deep;
    if (m_ExecuteBlockSyntax != nullptr) {
        m_ExecuteBlockSyntax->SetDeep(deep);
    }
}

std::string FileMetaConditionExpressSyntax::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::tabChar;
    }
    if (m_Token != nullptr) {
        sb << m_Token->GetLexeme().ToString() << " ";
    }
    if (m_ConditionExpress != nullptr) {
        sb << m_ConditionExpress->ToFormatString();
    }
    sb << "\n";
    if (m_ExecuteBlockSyntax != nullptr) {
        sb << m_ExecuteBlockSyntax->ToFormatString();
    }
    return sb.str();
}

// FileMetaKeyIfSyntax implementation
FileMetaKeyIfSyntax::FileMetaKeyIfSyntax(FileMeta* fm) {
    m_FileMeta = fm;
}

FileMetaConditionExpressSyntax* FileMetaKeyIfSyntax::GetIfExpressSyntax() const {
    return m_IfExpressSyntax;
}

const std::vector<FileMetaConditionExpressSyntax*>& FileMetaKeyIfSyntax::GetElseIfExpressSyntax() const {
    return m_ElseIfExpressSyntax;
}

FileMetaKeyOnlySyntax* FileMetaKeyIfSyntax::GetElseExpressSyntax() const {
    return m_ElseExpressSyntax;
}

void FileMetaKeyIfSyntax::SetFileMetaConditionExpressSyntax(FileMetaConditionExpressSyntax* fmces) {
    m_IfExpressSyntax = fmces;
}

void FileMetaKeyIfSyntax::SetElseExpressSyntax(FileMetaKeyOnlySyntax* fmces) {
    m_ElseExpressSyntax = fmces;
}

void FileMetaKeyIfSyntax::AddElseIfExpressSyntax(FileMetaConditionExpressSyntax* fmces) {
    m_ElseIfExpressSyntax.push_back(fmces);
}

void FileMetaKeyIfSyntax::SetDeep(int deep) {
    m_Deep = deep;
    if (m_IfExpressSyntax != nullptr) {
        m_IfExpressSyntax->SetDeep(m_Deep);
    }
    for (size_t i = 0; i < m_ElseIfExpressSyntax.size(); i++) {
        m_ElseIfExpressSyntax[i]->SetDeep(m_Deep);
    }
    if (m_ElseExpressSyntax != nullptr) {
        m_ElseExpressSyntax->SetDeep(m_Deep);
    }
}

std::string FileMetaKeyIfSyntax::ToFormatString() const {
    std::ostringstream sb;
    if (m_IfExpressSyntax != nullptr) {
        sb << m_IfExpressSyntax->ToFormatString();
    }
    for (size_t i = 0; i < m_ElseIfExpressSyntax.size(); i++) {
        sb << "\n";
        sb << m_ElseIfExpressSyntax[i]->ToFormatString();
    }
    if (m_ElseExpressSyntax != nullptr) {
        sb << "\n";
        sb << m_ElseExpressSyntax->ToFormatString();
    }
    return sb.str();
}

FileMetaKeyIfSyntax* FileMetaKeyIfSyntax::ParseIfSyntax(FileMeta* fm, void* sns) {
    // TODO: Implement parsing logic
    return new FileMetaKeyIfSyntax(fm);
}

// FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax implementation
FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax::FileMetaKeyCaseSyntax(FileMeta* fm, Token* castToken) {
    m_FileMeta = fm;
    m_Token = castToken;
}

FileMeta* FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax::GetFileMeta() const {
    return m_FileMeta;
}

bool FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax::IsContinueNextCastSyntax() const {
    return m_IsContinueNextCastSyntax;
}

void FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax::SetContinueNextCastSyntax(bool value) {
    m_IsContinueNextCastSyntax = value;
}

Token* FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax::GetVariableToken() const {
    return m_VariableToken;
}

FileMetaCallLink* FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax::GetDefineClassCallLink() const {
    return m_DefineClassToken;
}

FileMetaBlockSyntax* FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax::GetExecuteBlockSyntax() const {
    return m_ExecuteBlockSyntax;
}

const std::vector<FileMetaConstValueTerm*>& FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax::GetConstValueTokenList() const {
    return m_ConstValueTokenList;
}

void FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax::SetDefineClassNode(Node* defineClassNode) {
    m_DefineClassToken = new FileMetaCallLink(m_FileMeta, defineClassNode);
}

void FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax::SetVariableToken(Token* variableToken) {
    m_VariableToken = variableToken;
}

void FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax::SetExecuteBlockSyntax(FileMetaBlockSyntax* ebs) {
    m_ExecuteBlockSyntax = ebs;
}

void FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax::AddConstValueTokenList(FileMetaConstValueTerm* fmcvt) {
    m_ConstValueTokenList.push_back(fmcvt);
}

void FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax::SetDeep(int deep) {
    m_Deep = deep;
    if (m_ExecuteBlockSyntax != nullptr) {
        m_ExecuteBlockSyntax->SetDeep(deep);
    }
}

std::string FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "case ";
    if (m_DefineClassToken != nullptr) {
        if (m_DefineClassToken != nullptr) {
            sb << m_DefineClassToken->ToFormatString() << " ";
        }
        if (m_VariableToken != nullptr) {
            sb << m_VariableToken->GetLexeme().ToString() << " ";
        }
    } else {
        for (size_t i = 0; i < m_ConstValueTokenList.size(); i++) {
            sb << m_ConstValueTokenList[i]->ToFormatString();
            if (i < m_ConstValueTokenList.size() - 1) {
                sb << ",";
            }
        }
    }
    if (m_ExecuteBlockSyntax != nullptr) {
        sb << "\n";
        sb << m_ExecuteBlockSyntax->ToFormatString();
    }
    if (m_IsContinueNextCastSyntax) {
        sb << "next;";
    }
    return sb.str();
}

// FileMetaKeySwitchSyntax implementation
FileMetaKeySwitchSyntax::FileMetaKeySwitchSyntax(FileMeta* fm, Token* switchToken, Token* leftBraceToken, Token* rightBraceToken, FileMetaCallLink* cl) {
    m_FileMeta = fm;
    m_Token = switchToken;
    m_LeftBraceToken = leftBraceToken;
    m_RightBraceToken = rightBraceToken;
    m_FileMetaVariableRef = cl;
}

FileMetaCallLink* FileMetaKeySwitchSyntax::GetFileMetaVariableRef() const {
    return m_FileMetaVariableRef;
}

FileMetaBlockSyntax* FileMetaKeySwitchSyntax::GetDefaultExecuteBlockSyntax() const {
    return m_DefaultExecuteBlockSyntax;
}

const std::vector<FileMetaKeySwitchSyntax::FileMetaKeyCaseSyntax*>& FileMetaKeySwitchSyntax::GetFileMetaKeyCaseSyntaxList() const {
    return m_FileMetaKeyCaseSyntaxList;
}

FileMetaBlockSyntax* FileMetaKeySwitchSyntax::GetExecuteBlockSyntax() const {
    return m_DefaultExecuteBlockSyntax;
}

void FileMetaKeySwitchSyntax::AddFileMetaKeyCaseSyntaxList(FileMetaKeyCaseSyntax* keyCase) {
    m_FileMetaKeyCaseSyntaxList.push_back(keyCase);
}

void FileMetaKeySwitchSyntax::SetDefaultExecuteBlockSyntax(FileMetaBlockSyntax* fmbs) {
    m_DefaultExecuteBlockSyntax = fmbs;
}

void FileMetaKeySwitchSyntax::SetDeep(int deep) {
    m_Deep = deep;
    for (size_t i = 0; i < m_FileMetaKeyCaseSyntaxList.size(); i++) {
        m_FileMetaKeyCaseSyntaxList[i]->SetDeep(m_Deep + 1);
    }
    if (m_DefaultExecuteBlockSyntax != nullptr) {
        m_DefaultExecuteBlockSyntax->SetDeep(m_Deep + 1);
    }
}

std::string FileMetaKeySwitchSyntax::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "switch ";
    if (m_FileMetaVariableRef != nullptr) {
        sb << m_FileMetaVariableRef->ToFormatString();
    }
    sb << "\n";
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "{";
    for (size_t i = 0; i < m_FileMetaKeyCaseSyntaxList.size(); i++) {
        sb << "\n";
        sb << m_FileMetaKeyCaseSyntaxList[i]->ToFormatString();
    }
    if (m_DefaultExecuteBlockSyntax != nullptr) {
        sb << "\n";
        for (int i = 0; i < m_Deep + 1; i++) {
            sb << Global::GetTabChar();
        }
        sb << "default";
        sb << "\n";
        sb << m_DefaultExecuteBlockSyntax->ToFormatString();
    }
    sb << "\n";
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "}";
    return sb.str();
}

// FileMetaKeyMatchSyntax implementation (similar to FileMetaKeySwitchSyntax)
FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax::FileMetaKeyCaseSyntax(FileMeta* fm, Token* castToken)
{
    m_FileMeta = fm;
    m_Token = castToken;
}

FileMeta* FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax::GetFileMeta() const {
    return m_FileMeta;
}

bool FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax::IsContinueNextCastSyntax() const {
    return m_IsContinueNextCastSyntax;
}

void FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax::SetContinueNextCastSyntax(bool value) {
    m_IsContinueNextCastSyntax = value;
}

Token* FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax::GetVariableToken() {
    return m_VariableToken;
}

FileMetaCallLink* FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax::GetDefineClassCallLink() const {
    return m_DefineClassToken;
}

FileMetaBlockSyntax* FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax::GetExecuteBlockSyntax() const {
    return m_ExecuteBlockSyntax;
}

const std::vector<FileMetaConstValueTerm*>& FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax::GetConstValueTokenList() const {
    return m_ConstValueTokenList;
}

void FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax::SetDefineClassNode(Node* defineClassNode) {
    m_DefineClassToken = new FileMetaCallLink(m_FileMeta, defineClassNode);
}

void FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax::SetVariableToken(Token* variableToken) {
    m_VariableToken = variableToken;
}

void FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax::SetExecuteBlockSyntax(FileMetaBlockSyntax* ebs) {
    m_ExecuteBlockSyntax = ebs;
}

void FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax::AddConstValueTokenList(FileMetaConstValueTerm* fmcvt) {
    m_ConstValueTokenList.push_back(fmcvt);
}

void FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax::SetDeep(int deep) {
    m_Deep = deep;
    if (m_ExecuteBlockSyntax != nullptr) {
        m_ExecuteBlockSyntax->SetDeep(deep);
    }
}

std::string FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "case ";
    if (m_DefineClassToken != nullptr) {
        if (m_DefineClassToken != nullptr) {
            sb << m_DefineClassToken->ToFormatString() << " ";
        }
        if (m_VariableToken != nullptr) {
            sb << m_VariableToken->GetLexeme().ToString() << " ";
        }
    } else {
        for (size_t i = 0; i < m_ConstValueTokenList.size(); i++) {
            sb << m_ConstValueTokenList[i]->ToFormatString();
            if (i < m_ConstValueTokenList.size() - 1) {
                sb << ",";
            }
        }
    }
    if (m_ExecuteBlockSyntax != nullptr) {
        sb << "\n";
        sb << m_ExecuteBlockSyntax->ToFormatString();
    }
    if (m_IsContinueNextCastSyntax) {
        sb << "next;";
    }
    return sb.str();
}

FileMetaKeyMatchSyntax::FileMetaKeyMatchSyntax(FileMeta* fm, Token* switchToken, Token* leftBraceToken, Token* rightBraceToken, FileMetaCallLink* cl) {
    m_FileMeta = fm;
    m_Token = switchToken;
    m_LeftBraceToken = leftBraceToken;
    m_RightBraceToken = rightBraceToken;
    m_FileMetaVariableRef = cl;
}

FileMetaCallLink* FileMetaKeyMatchSyntax::GetFileMetaVariableRef() const {
    return m_FileMetaVariableRef;
}

FileMetaBlockSyntax* FileMetaKeyMatchSyntax::GetDefaultExecuteBlockSyntax() const {
    return m_DefaultExecuteBlockSyntax;
}

const std::vector<FileMetaKeyMatchSyntax::FileMetaKeyCaseSyntax*>& FileMetaKeyMatchSyntax::GetFileMetaKeyCaseSyntaxList() const {
    return m_FileMetaKeyCaseSyntaxList;
}

FileMetaBlockSyntax* FileMetaKeyMatchSyntax::GetExecuteBlockSyntax() const {
    return m_DefaultExecuteBlockSyntax;
}

void FileMetaKeyMatchSyntax::AddFileMetaKeyCaseSyntaxList(FileMetaKeyCaseSyntax* keyCase) {
    m_FileMetaKeyCaseSyntaxList.push_back(keyCase);
}

void FileMetaKeyMatchSyntax::SetDefaultExecuteBlockSyntax(FileMetaBlockSyntax* fmbs) {
    m_DefaultExecuteBlockSyntax = fmbs;
}

void FileMetaKeyMatchSyntax::SetDeep(int deep) {
    m_Deep = deep;
    for (size_t i = 0; i < m_FileMetaKeyCaseSyntaxList.size(); i++) {
        m_FileMetaKeyCaseSyntaxList[i]->SetDeep(m_Deep + 1);
    }
    if (m_DefaultExecuteBlockSyntax != nullptr) {
        m_DefaultExecuteBlockSyntax->SetDeep(m_Deep + 1);
    }
}

std::string FileMetaKeyMatchSyntax::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "switch ";
    if (m_FileMetaVariableRef != nullptr) {
        sb << m_FileMetaVariableRef->ToFormatString();
    }
    sb << "\n";
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "{";
    for (size_t i = 0; i < m_FileMetaKeyCaseSyntaxList.size(); i++) {
        sb << "\n";
        sb << m_FileMetaKeyCaseSyntaxList[i]->ToFormatString();
    }
    if (m_DefaultExecuteBlockSyntax != nullptr) {
        sb << "\n";
        for (int i = 0; i < m_Deep + 1; i++) {
            sb << Global::GetTabChar();
        }
        sb << "default";
        sb << "\n";
        sb << m_DefaultExecuteBlockSyntax->ToFormatString();
    }
    sb << "\n";
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "}";
    return sb.str();
}

// FileMetaKeyForSyntax implementation
FileMetaKeyForSyntax::FileMetaKeyForSyntax(FileMeta* fm, Token* forToken, FileMetaBlockSyntax* fmbs) {
    m_FileMeta = fm;
    m_Token = forToken;
    m_ExecuteBlockSyntax = fmbs;
}

FileMetaBlockSyntax* FileMetaKeyForSyntax::GetExecuteBlockSyntax() const {
    return m_ExecuteBlockSyntax;
}

FileMetaSyntax* FileMetaKeyForSyntax::GetFileMetaClassDefine() const {
    return m_FileMetaClassDefine;
}

FileMetaBaseTerm* FileMetaKeyForSyntax::GetConditionExpress() const {
    return m_ConditionExpress;
}

FileMetaOpAssignSyntax* FileMetaKeyForSyntax::GetStepFileMetaOpAssignSyntax() const {
    return m_StepFileMetaOpAssignSyntax;
}

bool FileMetaKeyForSyntax::IsInFor() const {
    return m_InToken != nullptr;
}

void FileMetaKeyForSyntax::SetFileMetaClassDefine(FileMetaSyntax* fmdvs) {
    m_FileMetaClassDefine = fmdvs;
    // TODO: Handle FileMetaCallSyntax case
}

void FileMetaKeyForSyntax::SetInKeyAndArrayVariable(Token* inToken, FileMetaBaseTerm* cn) {
    m_InToken = inToken;
    m_ConditionExpress = cn;
}

void FileMetaKeyForSyntax::SetConditionExpress(FileMetaBaseTerm* fmte) {
    m_ConditionExpress = fmte;
}

void FileMetaKeyForSyntax::SetStepFileMetaOpAssignSyntax(FileMetaOpAssignSyntax* fmoas) {
    m_StepFileMetaOpAssignSyntax = fmoas;
}

void FileMetaKeyForSyntax::SetDeep(int deep) {
    m_Deep = deep;
    if (m_ExecuteBlockSyntax != nullptr) {
        m_ExecuteBlockSyntax->SetDeep(m_Deep);
    }
}

std::string FileMetaKeyForSyntax::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "\n";
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "for ";
    if (m_FileMetaClassDefine != nullptr) {
        sb << m_FileMetaClassDefine->ToFormatString();
    }
    if (m_InToken != nullptr) {
        sb << " " << m_InToken->GetLexeme().ToString();
        sb << " " << (m_ConditionExpress ? m_ConditionExpress->ToFormatString() : "");
    } else {
        if (m_ConditionExpress != nullptr) {
            sb << ", " << m_ConditionExpress->ToFormatString();
        }
        if (m_StepFileMetaOpAssignSyntax != nullptr) {
            sb << ", " << m_StepFileMetaOpAssignSyntax->ToFormatString();
        }
    }
    if (m_ExecuteBlockSyntax != nullptr) {
        sb << "\n";
        sb << m_ExecuteBlockSyntax->ToFormatString();
    }
    return sb.str();
}

// FileMetaKeyOnlySyntax implementation
FileMetaKeyOnlySyntax::FileMetaKeyOnlySyntax(FileMeta* fm, Token* token, FileMetaBlockSyntax* executeBlockSyntax) {
    m_FileMeta = fm;
    m_Token = token;
    m_ExecuteBlockSyntax = executeBlockSyntax;
}

FileMetaBlockSyntax* FileMetaKeyOnlySyntax::GetExecuteBlockSyntax() const {
    return m_ExecuteBlockSyntax;
}

void FileMetaKeyOnlySyntax::SetDeep(int deep) {
    m_Deep = deep;
    if (m_ExecuteBlockSyntax != nullptr) {
        m_ExecuteBlockSyntax->SetDeep(deep);
    }
}

std::string FileMetaKeyOnlySyntax::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << (m_Token ? m_Token->GetLexeme().ToString() : "");
    if (m_ExecuteBlockSyntax != nullptr) {
        sb << "\n";
        sb << m_ExecuteBlockSyntax->ToFormatString();
    }
    return sb.str();
}

// FileMetaKeyReturnSyntax implementation
FileMetaKeyReturnSyntax::FileMetaKeyReturnSyntax(FileMeta* fm, Token* token, FileMetaBaseTerm* express) {
    m_FileMeta = fm;
    m_Token = token;
    m_ReturnExpress = express;
}

FileMetaBaseTerm* FileMetaKeyReturnSyntax::GetReturnExpress() const {
    return m_ReturnExpress;
}

std::string FileMetaKeyReturnSyntax::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << (m_Token ? m_Token->GetLexeme().ToString() : "") << " ";
    if (m_ReturnExpress != nullptr) {
        sb << m_ReturnExpress->ToFormatString();
    }
    return sb.str();
}

FileMetaKeyReturnSyntax* FileMetaKeyReturnSyntax::ParseIfSyntax(FileMeta* fm, void* akss) {
    // TODO: Implement parsing logic
    return new FileMetaKeyReturnSyntax(fm, nullptr, nullptr);
}

// FileMetaKeyGotoLabelSyntax implementation
FileMetaKeyGotoLabelSyntax::FileMetaKeyGotoLabelSyntax(FileMeta* fm, Token* token, Token* label) {
    m_FileMeta = fm;
    m_Token = token;
    m_LabelToken = label;
}

Token* FileMetaKeyGotoLabelSyntax::GetLabelToken() const {
    return m_LabelToken;
}

std::string FileMetaKeyGotoLabelSyntax::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << (m_Token ? m_Token->GetLexeme().ToString() : "") << " ";
    sb << (m_LabelToken ? m_LabelToken->GetLexeme().ToString() : "");
    return sb.str();
}

FileMetaKeyGotoLabelSyntax* FileMetaKeyGotoLabelSyntax::ParseIfSyntax(FileMeta* fm, void* akss) {
    // TODO: Implement parsing logic
    return new FileMetaKeyGotoLabelSyntax(fm, nullptr, nullptr);
}

// FileMetaCallSyntax implementation
FileMetaCallSyntax::FileMetaCallSyntax(FileMetaCallLink* fmrv) {
    m_FileMetaVariableRef = fmrv;
}

FileMetaCallLink* FileMetaCallSyntax::GetVariableRef() const {
    return m_FileMetaVariableRef;
}

std::string FileMetaCallSyntax::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << (m_FileMetaVariableRef ? m_FileMetaVariableRef->ToFormatString() : "");
    if (IsAppendSemiColon()) {
        sb << ";";
    }
    return sb.str();
}

// FileMetaDefineVariableSyntax implementation
FileMetaDefineVariableSyntax::FileMetaDefineVariableSyntax(FileMeta* fm, FileMetaClassDefine* fmcd, Token* nameToken, Token* assignToken, Token* staticToken, FileMetaBaseTerm* express) {
    m_FileMeta = fm;
    m_Token = nameToken;
    m_StaticToken = staticToken;
    m_AssignToken = assignToken;
    m_FileMetaClassDefine = fmcd;
    m_FileMetaExpress = express;
}

Token* FileMetaDefineVariableSyntax::GetNameToken() const {
    return m_Token;
}

Token* FileMetaDefineVariableSyntax::GetStaticToken() const {
    return m_StaticToken;
}

Token* FileMetaDefineVariableSyntax::GetAssignToken() const {
    return m_AssignToken;
}

FileMetaClassDefine* FileMetaDefineVariableSyntax::GetFileMetaClassDefine() const {
    return m_FileMetaClassDefine;
}

FileMetaBaseTerm* FileMetaDefineVariableSyntax::GetExpress() const {
    return m_FileMetaExpress;
}

std::string FileMetaDefineVariableSyntax::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    if (m_StaticToken != nullptr) {
        sb << m_StaticToken->GetLexeme().ToString() << " ";
    }
    if (m_FileMetaClassDefine != nullptr) {
        sb << m_FileMetaClassDefine->ToFormatString() << " ";
    }
    sb << m_Token->GetLexeme().ToString() << " ";
    if (m_AssignToken != nullptr) {
        sb << m_AssignToken->GetLexeme().ToString() << " ";
        if (m_FileMetaExpress != nullptr) {
            sb << m_FileMetaExpress->ToFormatString();
        }
        if (IsAppendSemiColon()) {
            sb << ";";
        }
    }
    return sb.str();
}

// FileMetaOpAssignSyntax implementation
FileMetaOpAssignSyntax::FileMetaOpAssignSyntax(FileMetaCallLink* fileMetaVariableRef, Token* opAssignToken, Token* dynamicClassToken, Token* dynamicDataToken, Token* varToken, FileMetaBaseTerm* fme, bool flag) {
    m_VariableRef = fileMetaVariableRef;
    m_AssignToken = opAssignToken;
    m_DynamicToken = dynamicClassToken;
    m_DataToken = dynamicDataToken;
    m_VarToken = varToken;
    m_Express = fme;
    SetAppendSemiColon(flag);
}

FileMetaCallLink* FileMetaOpAssignSyntax::GetVariableRef() const {
    return m_VariableRef;
}

FileMetaBaseTerm* FileMetaOpAssignSyntax::GetExpress() const {
    return m_Express;
}

Token* FileMetaOpAssignSyntax::GetAssignToken() const {
    return m_AssignToken;
}

Token* FileMetaOpAssignSyntax::GetDynamicToken() const {
    return m_DynamicToken;
}

Token* FileMetaOpAssignSyntax::GetConstToken() const {
    return m_ConstToken;
}

Token* FileMetaOpAssignSyntax::GetStaticToken() const {
    return m_StaticToken;
}

bool FileMetaOpAssignSyntax::HasDefine() const {
    return m_DynamicToken != nullptr || m_DataToken != nullptr || m_VarToken != nullptr;
}

void FileMetaOpAssignSyntax::SetDeep(int deep) {
    m_Deep = deep;
    if (m_Express != nullptr) {
        m_Express->SetDeep(deep);
    }
}

std::string FileMetaOpAssignSyntax::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << m_VariableRef->ToFormatString();
    sb << " " << m_AssignToken->GetLexeme().ToString();
    sb << " " << (m_Express ? m_Express->ToFormatString() : "");
    if (IsAppendSemiColon()) {
        sb << ";";
    }
    return sb.str();
}

// FileMetaBlockSyntax implementation
FileMetaBlockSyntax::FileMetaBlockSyntax(FileMeta* fm, Token* bblock, Token* eblock) {
    m_FileMeta = fm;
    m_BeginBlock = bblock;
    m_EndBlock = eblock;
}

Token* FileMetaBlockSyntax::GetBeginBlock() const {
    return m_BeginBlock;
}

Token* FileMetaBlockSyntax::GetEndBlock() const {
    return m_EndBlock;
}

void FileMetaBlockSyntax::SetDeep(int deep) {
    m_Deep = deep;
    for (size_t i = 0; i < m_FileMetaSyntax.size(); i++) {
        m_FileMetaSyntax[i]->SetDeep(m_Deep + 1);
    }
}

std::string FileMetaBlockSyntax::ToFormatString() const {
    std::ostringstream sb;
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << (m_BeginBlock ? m_BeginBlock->GetLexeme().ToString() : "{") << "\n";
    for (size_t i = 0; i < m_FileMetaSyntax.size(); i++) {
        sb << m_FileMetaSyntax[i]->ToFormatString();
        sb << "\n";
    }
    for (int i = 0; i < m_Deep; i++) {
        sb << Global::GetTabChar();
    }
    sb << "}";
    return sb.str();
}

} // namespace SimpleLanguage
} // namespace SimpleLanguage