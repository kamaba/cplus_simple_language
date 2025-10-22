//****************************************************************************
//  File:      MetaTypeFactory.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#include "MetaTypeFactory.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "MetaType.h"
#include <iostream>

namespace SimpleLanguage {
namespace Core {

// 根据MetaType获取操作级别
int MetaTypeFactory::GetOpLevelByMetaType(MetaType* mt) {
    if (mt == nullptr) {
        return 10;
    }
    
    auto& coreMetaClassManager = CoreMetaClassManager::GetInstance();
    
    if (mt->GetMetaClass() == coreMetaClassManager.GetBooleanMetaClass()) {
        return 0;
    }
    else if (mt->GetMetaClass() == coreMetaClassManager.GetByteMetaClass() ||
             mt->GetMetaClass() == coreMetaClassManager.GetSByteMetaClass()) {
        return 1;
    }
    else if (mt->GetMetaClass() == coreMetaClassManager.GetInt16MetaClass() ||
             mt->GetMetaClass() == coreMetaClassManager.GetUInt16MetaClass()) {
        return 2;
    }
    else if (mt->GetMetaClass() == coreMetaClassManager.GetInt32MetaClass() ||
             mt->GetMetaClass() == coreMetaClassManager.GetUInt32MetaClass()) {
        return 3;
    }
    else if (mt->GetMetaClass() == coreMetaClassManager.GetInt64MetaClass() ||
             mt->GetMetaClass() == coreMetaClassManager.GetUInt64MetaClass()) {
        return 4;
    }
    else if (mt->GetMetaClass() == coreMetaClassManager.GetFloat32MetaClass()) {
        return 5;
    }
    else if (mt->GetMetaClass() == coreMetaClassManager.GetFloat64MetaClass()) {
        return 6;
    }
    else if (mt->GetMetaClass() == coreMetaClassManager.GetArrayMetaClass()) {
        return 8;
    }
    else if (mt->GetMetaClass() == coreMetaClassManager.GetStringMetaClass()) {
        return 11;
    }
    
    return 10;
}

// 根据EType获取操作级别
int MetaTypeFactory::GetOpLevel(EType defineType) {
    if (defineType == EType::Boolean) {
        return 0;
    }
    else if (defineType == EType::Int16 || defineType == EType::UInt16) {
        return 2;
    }
    else if (defineType == EType::Int32 || defineType == EType::UInt32) {
        return 3;
    }
    else if (defineType == EType::Int64 || defineType == EType::UInt64) {
        return 4;
    }
    else if (defineType == EType::Float32) {
        return 5;
    }
    else if (defineType == EType::Float64) {
        return 6;
    }
    else if (defineType == EType::Array) {
        return 8;
    }
    else if (defineType == EType::String) {
        return 11;
    }
    
    return 10;
}

// 将定义类型转换为类名
std::string MetaTypeFactory::DefineTypeToClassName(const std::string& tokenname) {
    std::string className = tokenname;
    
    if (tokenname == "short") {
        className = "Int16";
    }
    else if (tokenname == "ushort") {
        className = "UInt16";
    }
    else if (tokenname == "int") {
        className = "Int32";
    }
    else if (tokenname == "uint") {
        className = "UInt32";
    }
    else if (tokenname == "long") {
        className = "Int64";
    }
    else if (tokenname == "ulong") {
        className = "UInt64";
    }
    else if (tokenname == "float") {
        className = "Float";
    }
    else if (tokenname == "double") {
        className = "Double";
    }
    else if (tokenname == "char") {
        className = "Char";
    }
    else if (tokenname == "byte") {
        className = "Byte";
    }
    else if (tokenname == "sbyte") {
        className = "SByte";
    }
    else if (tokenname == "string") {
        className = "String";
    }
    
    return className;
}

} // namespace Core
} // namespace SimpleLanguage
