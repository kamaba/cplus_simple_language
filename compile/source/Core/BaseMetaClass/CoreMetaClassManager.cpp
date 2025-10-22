//****************************************************************************
//  File:      CoreMetaClassManager.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "CoreMetaClassManager.h"
#include "NullMetaClass.h"
#include "ObjectMetaClass.h"
#include "VoidMetaClass.h"
#include "BooleanMetaClass.h"
#include "ByteMetaClass.h"
#include "IntMetaClass.h"
#include "RealMetaClass.h"
#include "StringMetaClass.h"
#include "ArrayMetaClass.h"
#include "RangeMetaClass.h"
#include "TypeMetaClass.h"
#include "DynamicMetaClass.h"
#include "DynamicMetaData.h"
#include "EnumMetaClass.h"
#include <iostream>

namespace SimpleLanguage {
namespace Core {
namespace SelfMeta {

std::unique_ptr<CoreMetaClassManager> CoreMetaClassManager::s_Instance = nullptr;

MetaClass* CoreMetaClassManager::nullMetaClass = nullptr;
MetaClass* CoreMetaClassManager::objectMetaClass = nullptr;
MetaClass* CoreMetaClassManager::stringMetaClass = nullptr;
MetaClass* CoreMetaClassManager::voidMetaClass = nullptr;
MetaClass* CoreMetaClassManager::booleanMetaClass = nullptr;
MetaClass* CoreMetaClassManager::byteMetaClass = nullptr;
MetaClass* CoreMetaClassManager::sbyteMetaClass = nullptr;
MetaClass* CoreMetaClassManager::int16MetaClass = nullptr;
MetaClass* CoreMetaClassManager::uint16MetaClass = nullptr;
MetaClass* CoreMetaClassManager::int32MetaClass = nullptr;
MetaClass* CoreMetaClassManager::uint32MetaClass = nullptr;
MetaClass* CoreMetaClassManager::int64MetaClass = nullptr;
MetaClass* CoreMetaClassManager::uint64MetaClass = nullptr;
MetaClass* CoreMetaClassManager::float32MetaClass = nullptr;
MetaClass* CoreMetaClassManager::float64MetaClass = nullptr;
MetaClass* CoreMetaClassManager::arrayMetaClass = nullptr;
MetaClass* CoreMetaClassManager::rangeMetaClass = nullptr;
MetaClass* CoreMetaClassManager::typeMetaClass = nullptr;
MetaClass* CoreMetaClassManager::dynamicMetaClass = nullptr;
MetaClass* CoreMetaClassManager::dynamicMetaData = nullptr;
MetaClass* CoreMetaClassManager::enumMetaData = nullptr;
MetaClass* CoreMetaClassManager::arrayIteratorMetaClass = nullptr;

std::vector<MetaClass*> CoreMetaClassManager::s_InnerDefineMetaClassList;

void CoreMetaClassManager::Initialize() {
    nullMetaClass = NullMetaClass::CreateMetaClass();
    objectMetaClass = ObjectMetaClass::CreateMetaClass();
    voidMetaClass = VoidMetaClass::CreateMetaClass();
    booleanMetaClass = BooleanMetaClass::CreateMetaClass();
    byteMetaClass = ByteMetaClass::CreateMetaClass();
    sbyteMetaClass = SByteMetaClass::CreateMetaClass();
    int16MetaClass = Int16MetaClass::CreateMetaClass();
    uint16MetaClass = UInt16MetaClass::CreateMetaClass();
    int32MetaClass = Int32MetaClass::CreateMetaClass();
    uint32MetaClass = UInt32MetaClass::CreateMetaClass();
    int64MetaClass = Int64MetaClass::CreateMetaClass();
    uint64MetaClass = UInt64MetaClass::CreateMetaClass();
    float32MetaClass = Float32MetaClass::CreateMetaClass();
    float64MetaClass = Float64MetaClass::CreateMetaClass();
    stringMetaClass = StringMetaClass::CreateMetaClass();
    arrayIteratorMetaClass = ArrayIteratorMetaClass::CreateMetaClass();
    arrayMetaClass = ArrayMetaClass::CreateMetaClass();
    rangeMetaClass = RangeMetaClass::CreateMetaClass();
    typeMetaClass = TypeMetaClass::CreateMetaClass();
    dynamicMetaClass = DynamicMetaClass::CreateMetaClass();
    dynamicMetaData = DynamicMetaData::CreateMetaClass();
    enumMetaData = EnumMetaClass::CreateMetaClass();

    s_InnerDefineMetaClassList.push_back(objectMetaClass);
    s_InnerDefineMetaClassList.push_back(voidMetaClass);
    s_InnerDefineMetaClassList.push_back(booleanMetaClass);
    s_InnerDefineMetaClassList.push_back(byteMetaClass);
    s_InnerDefineMetaClassList.push_back(sbyteMetaClass);
    s_InnerDefineMetaClassList.push_back(int16MetaClass);
    s_InnerDefineMetaClassList.push_back(uint16MetaClass);
    s_InnerDefineMetaClassList.push_back(int32MetaClass);
    s_InnerDefineMetaClassList.push_back(uint32MetaClass);
    s_InnerDefineMetaClassList.push_back(int64MetaClass);
    s_InnerDefineMetaClassList.push_back(uint64MetaClass);
    s_InnerDefineMetaClassList.push_back(float32MetaClass);
    s_InnerDefineMetaClassList.push_back(float64MetaClass);
    s_InnerDefineMetaClassList.push_back(stringMetaClass);
    s_InnerDefineMetaClassList.push_back(arrayIteratorMetaClass);
    s_InnerDefineMetaClassList.push_back(arrayMetaClass);
    s_InnerDefineMetaClassList.push_back(rangeMetaClass);
    s_InnerDefineMetaClassList.push_back(typeMetaClass);
    s_InnerDefineMetaClassList.push_back(dynamicMetaClass);
    s_InnerDefineMetaClassList.push_back(dynamicMetaData);
    s_InnerDefineMetaClassList.push_back(enumMetaData);
}

void CoreMetaClassManager::Init() {
    for (auto v : s_InnerDefineMetaClassList) {
        v->ParseInner();
        ClassManager::GetInstance().AddRuntimeMetaClass(v);
    }
}

bool CoreMetaClassManager::IsIncludeMetaClass(MetaClass* metaclass) {
    for (auto mc : s_InnerDefineMetaClassList) {
        if (mc == metaclass) {
            return true;
        }
    }
    return false;
}

MetaClass* CoreMetaClassManager::GetMetaClassByEType(EType etype) {
    switch (etype) {
        case EType::Void:
            return voidMetaClass;
        case EType::Null:
            return nullMetaClass;
        case EType::Boolean:
            return booleanMetaClass;
        case EType::Byte:
            return byteMetaClass;
        case EType::SByte:
            return sbyteMetaClass;
        case EType::Int16:
            return int16MetaClass;
        case EType::UInt16:
            return uint16MetaClass;
        case EType::Int32:
            return int32MetaClass;
        case EType::UInt32:
            return uint32MetaClass;
        case EType::Int64:
            return int64MetaClass;
        case EType::UInt64:
            return uint64MetaClass;
        case EType::Float32:
            return float32MetaClass;
        case EType::Float64:
            return float64MetaClass;
        case EType::String:
            return stringMetaClass;
        case EType::Array:
            return arrayMetaClass;
        case EType::Range:
            return rangeMetaClass;
        default:
            std::cout << "Warning ClassManager GetMetaClassByEType 1111" << std::endl;
            break;
    }
    return nullptr;
}

std::string CoreMetaClassManager::GetSelfMetaName(const std::string& name) {
    if (name == "void") {
        return "Void";
    } else if (name == "null") {
        return "Null";
    } else if (name == "object") {
        return "Object";
    } else if (name == "bool") {
        return "Boolean";
    } else if (name == "byte") {
        return "Byte";
    } else if (name == "sbyte") {
        return "SByte";
    } else if (name == "long" || name == "Int64") {
        return "Int64";
    } else if (name == "ulong" || name == "UInt64") {
        return "UInt64";
    } else if (name == "int" || name == "Int32") {
        return "Int32";
    } else if (name == "uint" || name == "UInt32") {
        return "UInt32";
    } else if (name == "short" || name == "Int16") {
        return "Int16";
    } else if (name == "ushort" || name == "UInt16") {
        return "UInt16";
    } else if (name == "char" || name == "Char") {
        return "Char";
    } else if (name == "string" || name == "String") {
        return "String";
    } else if (name == "half") {
        return "";
    } else if (name == "float" || name == "Float32") {
        return "Float32";
    } else if (name == "double" || name == "Float64") {
        return "Float64";
    } else if (name == "range" || name == "Range") {
        return "Range";
    } else if (name == "dynamic") {
        return "Dynamic";
    } else if (name == "data") {
        return "Data";
    } else {
        return name;
    }
}

MetaNode* CoreMetaClassManager::GetCoreMetaClass(const std::string& name) {
    std::string name1 = GetSelfMetaName(name);
    return ModuleManager::GetInstance().GetCoreModule()->GetMetaNode()->GetChildrenMetaNodeByName(name1);
}

} // namespace SelfMeta
} // namespace Core
} // namespace SimpleLanguage
