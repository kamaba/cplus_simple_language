//****************************************************************************
//  File:      Log.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <iostream>
#include <sstream>

namespace SimpleLanguage {
    namespace Compile { class Token; }
    namespace Core { class MetaBase; }
namespace Debug {

enum class EError {
    None,
    ParseTokenStart,
    UnMatchChar,
    ParseTokenEnd,
    StructMetaStart,
    MemberNeedExpress,
    AddClassNameSame,
    StructMetaEnd,
    StructFileMetaStart,
    StructClassNameRepeat,
    StructFileMetaEnd,
    ProcessStart,
    ParseFileError,
    ProcessEnd
};

enum class EProcess {
    None,
    ParseToken,
    ParseNode,
    StructFileMeta,
    StructMeta,
    HandleClass,
    HandleMember,
    HandleSyntax
};

enum class EMetaType {
    None,
    MetaNamespace,
    MetaClass,
    MetaExtendsClass,
    MetaData,
    MetaEnum,
    MetaMemberVariable,
    MetaMemberVariableExpress,
    MetaMemberData,
    MetaMemberDataExpress,
    MetaMemberEnumValue,
    MetaMemberEnumValueExpress
};

class LogData {
public:
    enum class EErrorType {
        None,
        InitProject,    // ��ʼ������
        HandleToken,    // ʶ��token
        HandleNode,     // ʶ��node
        StructFileMeta, // ����FileMeta�ļ�
        StructMeta,     // ����Ԫ����
        GenIR,          // ����IR
        VM,             // ʹ��VM
        Process
    };

    EError error = EError::None;
    EErrorType errorType = EErrorType::None;
    std::string message;
    std::string filePath;
    int sourceBeginLine = 0;    // ��ʼ������
    int sourceBeginChar = 0;   // ��ʼ������
    int sourceEndLine = 0;      // ����������
    int sourceEndChar = 0;      // ����������
    std::string demo;
    std::string advan;
    std::chrono::system_clock::time_point time;

    std::map<EMetaType, Core::MetaBase*> valDict;

    LogData();
    LogData(const std::string& msg, const std::string& path, int sline, int schar, int eline, int echar);
    
    std::string ToString() const;
    
private:
    static std::stringstream m_SB;
};

class CodeFileLogData : public LogData {
public:
    CodeFileLogData();
};

class LexelLogData : public LogData {
public:
    LexelLogData();
};

class Log {
private:
    static std::vector<LogData*> logDataList;

public:
    static void AddCodeFileLog(LogData* data);
    static void AddInInitProject(Compile::Token* token, EError err, const std::string& msg);
    static LogData* AddProcess(EProcess proc, EError err, const std::string& msg);
    static LogData* AddInHandleToken(const std::string& path, int sbl, int sel, EError err, const std::string& msg);
    static LogData* AddInHandleNode(Compile::Token* token, EError err, const std::string& msg);
    static LogData* AddInStructFileMeta(EError err, const std::string& msg);
    static LogData* AddInStructFileMeta(EError err, const std::string& msg, Compile::Token* token);
    static LogData* AddInStructMeta(EError err, const std::string& msg);
    static LogData* AddInStructMeta(EError err, const std::string& msg, Compile::Token* token);
    static LogData* AddGenIR(EError err, const std::string& msg);
    static LogData* AddVM(EError err, const std::string& msg);
    
private:
    static void AddLog(Compile::Token* token, EError err, const std::string& msg);

public:
    static void PrintLog();
};

} // namespace Debug
} // namespace SimpleLanguage
