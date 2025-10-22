//****************************************************************************
//  File:      Log.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../Compile/Token.h"
#include "../Core/MetaBase.h"
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <iostream>
#include <sstream>

namespace SimpleLanguage {
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
        InitProject,    // 初始化工程
        HandleToken,    // 识别token
        HandleNode,     // 识别node
        StructFileMeta, // 构建FileMeta文件
        StructMeta,     // 构建元数据
        GenIR,          // 生成IR
        VM,             // 使用VM
        Process
    };

    EError error = EError::None;
    EErrorType errorType = EErrorType::None;
    std::string message;
    std::string filePath;
    int sourceBeginLine = 0;    // 开始所在行
    int sourceBeginChar = 0;   // 开始所在列
    int sourceEndLine = 0;      // 结束所在行
    int sourceEndChar = 0;      // 结束所在列
    std::string demo;
    std::string advan;
    std::chrono::system_clock::time_point time;

    std::map<EMetaType, MetaBase*> valDict;

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
    static void AddInInitProject(Token* token, EError err, const std::string& msg);
    static LogData* AddProcess(EProcess proc, EError err, const std::string& msg);
    static LogData* AddInHandleToken(const std::string& path, int sbl, int sel, EError err, const std::string& msg);
    static LogData* AddInHandleNode(Token* token, EError err, const std::string& msg);
    static LogData* AddInStructFileMeta(EError err, const std::string& msg);
    static LogData* AddInStructFileMeta(EError err, const std::string& msg, Token* token);
    static LogData* AddInStructMeta(EError err, const std::string& msg);
    static LogData* AddInStructMeta(EError err, const std::string& msg, Token* token);
    static LogData* AddGenIR(EError err, const std::string& msg);
    static LogData* AddVM(EError err, const std::string& msg);
    
private:
    static void AddLog(Token* token, EError err, const std::string& msg);

public:
    static void PrintLog();
};

} // namespace Debug
} // namespace SimpleLanguage
