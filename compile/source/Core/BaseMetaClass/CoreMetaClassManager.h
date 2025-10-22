//****************************************************************************
//  File:      CoreMetaClassManager.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "../MetaClass.h"
#include "../MetaNode.h"
#include "ClassManager.h"
#include "ModuleManager.h"
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
#include <vector>
#include <memory>
#include <iostream>

namespace SimpleLanguage {
namespace Core {
namespace SelfMeta {

enum class DefaultObject {
    Void,
    Null,
    Object,
    Boolean,
    Byte,
    SByte,
    Char,
    Int16,
    UInt16,
    Int32,
    UInt32,
    Int64,
    UInt64,
    Float32,
    Float64,
    String,
    Array,
    Range,
    Template,
    Class,
    Type,
    Dynamic,
    Data,
    Enum
};

class CoreMetaClassManager {
private:
    static std::unique_ptr<CoreMetaClassManager> s_Instance;
    
public:
    static CoreMetaClassManager& GetInstance() {
        if (!s_Instance) {
            s_Instance = std::make_unique<CoreMetaClassManager>();
        }
        return *s_Instance;
    }
    
    static MetaClass* nullMetaClass;
    static MetaClass* objectMetaClass;
    static MetaClass* stringMetaClass;
    static MetaClass* voidMetaClass;
    static MetaClass* booleanMetaClass;
    static MetaClass* byteMetaClass;
    static MetaClass* sbyteMetaClass;
    static MetaClass* int16MetaClass;
    static MetaClass* uint16MetaClass;
    static MetaClass* int32MetaClass;
    static MetaClass* uint32MetaClass;
    static MetaClass* int64MetaClass;
    static MetaClass* uint64MetaClass;
    static MetaClass* float32MetaClass;
    static MetaClass* float64MetaClass;
    static MetaClass* arrayMetaClass;
    static MetaClass* rangeMetaClass;
    static MetaClass* typeMetaClass;
    static MetaClass* dynamicMetaClass;
    static MetaClass* dynamicMetaData;
    static MetaClass* enumMetaData;
    static MetaClass* arrayIteratorMetaClass;

    static std::vector<MetaClass*> s_InnerDefineMetaClassList;

    static void Initialize() {
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
    
    void Init() {
        for (auto v : s_InnerDefineMetaClassList) {
            v->ParseInner();
            ClassManager::GetInstance().AddRuntimeMetaClass(v);
        }
    }
    
    static bool IsIncludeMetaClass(MetaClass* metaclass) {
        for (auto mc : s_InnerDefineMetaClassList) {
            if (mc == metaclass) {
                return true;
            }
        }
        return false;
    }
    
    static MetaClass* GetMetaClassByEType(EType etype) {
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
    
    static std::string GetSelfMetaName(const std::string& name) {
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
    
    static MetaNode* GetCoreMetaClass(const std::string& name) {
        std::string name1 = GetSelfMetaName(name);
        return ModuleManager::GetInstance().GetCoreModule()->GetMetaNode()->GetChildrenMetaNodeByName(name1);
    }
    
    // Getter methods for accessing meta classes
    MetaClass* GetObjectMetaClass() const { return objectMetaClass; }
    MetaClass* GetVoidMetaClass() const { return voidMetaClass; }
    MetaClass* GetBooleanMetaClass() const { return booleanMetaClass; }
    MetaClass* GetByteMetaClass() const { return byteMetaClass; }
    MetaClass* GetSByteMetaClass() const { return sbyteMetaClass; }
    MetaClass* GetInt16MetaClass() const { return int16MetaClass; }
    MetaClass* GetUInt16MetaClass() const { return uint16MetaClass; }
    MetaClass* GetInt32MetaClass() const { return int32MetaClass; }
    MetaClass* GetUInt32MetaClass() const { return uint32MetaClass; }
    MetaClass* GetInt64MetaClass() const { return int64MetaClass; }
    MetaClass* GetUInt64MetaClass() const { return uint64MetaClass; }
    MetaClass* GetFloat32MetaClass() const { return float32MetaClass; }
    MetaClass* GetFloat64MetaClass() const { return float64MetaClass; }
    MetaClass* GetStringMetaClass() const { return stringMetaClass; }
    MetaClass* GetArrayMetaClass() const { return arrayMetaClass; }
    MetaClass* GetRangeMetaClass() const { return rangeMetaClass; }
    MetaClass* GetTypeMetaClass() const { return typeMetaClass; }
    MetaClass* GetDynamicMetaClass() const { return dynamicMetaClass; }
    MetaClass* GetDynamicMetaData() const { return dynamicMetaData; }
    MetaClass* GetEnumMetaData() const { return enumMetaData; }
    MetaClass* GetArrayIteratorMetaClass() const { return arrayIteratorMetaClass; }
};

} // namespace SelfMeta
} // namespace Core
} // namespace SimpleLanguage
