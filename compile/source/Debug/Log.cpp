//****************************************************************************
//  File:      Log.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: 
//****************************************************************************

#include "Log.h"
#include <iostream>
#include <iomanip>
#include "../Compile/Token.h"

using namespace SimpleLanguage::Compile;

namespace SimpleLanguage {
namespace Debug {

// LogData implementation
std::stringstream LogData::m_SB;

LogData::LogData() {
    time = std::chrono::system_clock::now();
}

LogData::LogData(const std::string& msg, const std::string& path, int sline, int schar, int eline, int echar) {
    message = msg;
    filePath = path;
    sourceBeginLine = sline;
    sourceBeginChar = schar;
    sourceEndLine = eline;
    sourceEndChar = echar;
    time = std::chrono::system_clock::now();
}

std::string LogData::ToString() const {
    m_SB.str("");
    m_SB.clear();

    m_SB << "类型: [" << static_cast<int>(errorType) << "] ";
    m_SB << " Error: [" << static_cast<int>(error) << "] ";

    if (!filePath.empty()) {
        m_SB << " FilePath: [" << filePath << "] ";
        m_SB << " SLine: [" << sourceBeginLine << "] ";
        m_SB << " ELine: [" << sourceEndLine << "] ";
    }

    m_SB << " Info: [" << message << "]";

    return m_SB.str();
}

// CodeFileLogData implementation
CodeFileLogData::CodeFileLogData() : LogData() {
}

// LexelLogData implementation
LexelLogData::LexelLogData() : LogData() {
}

// Log implementation
std::vector<LogData*> Log::logDataList;

void Log::AddCodeFileLog(LogData* data) {
    std::cout << data->ToString() << std::endl;
    logDataList.push_back(data);
}

void Log::AddInInitProject(Token* token, EError err, const std::string& msg) {
    LogData* ld = new LogData();
    ld->filePath = token->GetPath();
    ld->sourceBeginLine = token->GetSourceBeginLine();
    ld->sourceEndLine = token->GetSourceEndLine();
    ld->errorType = LogData::EErrorType::InitProject;
    ld->message = msg;
    ld->error = err;
    AddCodeFileLog(ld);
}

LogData* Log::AddProcess(EProcess proc, EError err, const std::string& msg) {
    LogData* ld = new LogData();
    ld->errorType = LogData::EErrorType::Process;
    ld->message = msg;
    ld->error = err;
    AddCodeFileLog(ld);
    return ld;
}

LogData* Log::AddInHandleToken(const std::string& path, int sbl, int sel, EError err, const std::string& msg) {
    LogData* ld = new LogData();
    ld->filePath = path;
    ld->sourceBeginLine = sbl;
    ld->sourceBeginChar = sel;
    ld->errorType = LogData::EErrorType::HandleToken;
    ld->time = std::chrono::system_clock::now();
    ld->message = msg;
    ld->error = err;
    AddCodeFileLog(ld);
    return ld;
}

LogData* Log::AddInHandleNode(Token* token, EError err, const std::string& msg) {
    LogData* ld = new LogData();
    ld->filePath = token->GetPath();
    ld->sourceBeginLine = token->GetSourceBeginLine();
    ld->sourceEndLine = token->GetSourceEndLine();
    ld->errorType = LogData::EErrorType::HandleNode;
    ld->time = std::chrono::system_clock::now();
    ld->message = msg;
    ld->error = err;
    AddCodeFileLog(ld);
    return ld;
}

LogData* Log::AddInStructFileMeta(EError err, const std::string& msg) {
    LogData* ld = new LogData();
    ld->errorType = LogData::EErrorType::StructFileMeta;
    ld->time = std::chrono::system_clock::now();
    ld->message = msg;
    ld->error = err;
    AddCodeFileLog(ld);
    return ld;
}

LogData* Log::AddInStructFileMeta(EError err, const std::string& msg, Token* token) {
    LogData* ld = new LogData();
    ld->filePath = token->GetPath();
    ld->errorType = LogData::EErrorType::StructFileMeta;
    ld->time = std::chrono::system_clock::now();
    ld->sourceBeginLine = token->GetSourceBeginLine();
    ld->sourceEndLine = token->GetSourceEndLine();
    ld->message = msg;
    ld->error = err;
    AddCodeFileLog(ld);
    return ld;
}

LogData* Log::AddInStructMeta(EError err, const std::string& msg) {
    LogData* ld = new LogData();
    ld->errorType = LogData::EErrorType::StructMeta;
    ld->time = std::chrono::system_clock::now();
    ld->message = msg;
    ld->error = err;
    AddCodeFileLog(ld);
    return ld;
}

LogData* Log::AddInStructMeta(EError err, const std::string& msg, Token* token) {
    if (token == nullptr) {
        token = new Token();
    }
    LogData* ld = new LogData();
    ld->errorType = LogData::EErrorType::StructMeta;
    ld->time = std::chrono::system_clock::now();
    ld->sourceBeginLine = token->GetSourceBeginLine();
    ld->sourceBeginChar = token->GetSourceBeginChar();
    ld->sourceEndLine = token->GetSourceEndLine();
    ld->sourceEndChar = token->GetSourceEndChar();
    ld->filePath = token->GetPath();
    ld->message = msg;
    ld->error = err;
    AddCodeFileLog(ld);
    return ld;
}

LogData* Log::AddGenIR(EError err, const std::string& msg) {
    LogData* ld = new LogData();
    ld->errorType = LogData::EErrorType::GenIR;
    ld->time = std::chrono::system_clock::now();
    ld->message = msg;
    ld->error = err;
    AddCodeFileLog(ld);
    return ld;
}

LogData* Log::AddVM(EError err, const std::string& msg) {
    LogData* ld = new LogData();
    ld->errorType = LogData::EErrorType::VM;
    ld->time = std::chrono::system_clock::now();
    ld->message = msg;
    ld->error = err;
    AddCodeFileLog(ld);
    return ld;
}

void Log::AddLog(Token* token, EError err, const std::string& msg) {
    LogData* ld = new LogData();
    ld->filePath = token->GetPath();
    ld->sourceBeginLine = token->GetSourceBeginLine();
    ld->sourceEndLine = token->GetSourceEndLine();
    ld->message = msg;
    ld->error = err;
    AddCodeFileLog(ld);
}

void Log::PrintLog() {
    std::cout << "----------错误收集 开始---------------------" << std::endl;

    for (auto ld : logDataList) {
        std::cout << ld->ToString() << std::endl;
    }
    std::cout << "----------错误收集 结束---------------------" << std::endl;
}

} // namespace Debug
} // namespace SimpleLanguage
