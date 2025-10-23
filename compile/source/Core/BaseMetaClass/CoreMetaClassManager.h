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
#include <vector>
#include <memory>
#include <iostream>

namespace SimpleLanguage {
namespace Core {
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

    static void Initialize();
    
    void Init();
    
    static bool IsIncludeMetaClass(MetaClass* metaclass);    
    static MetaClass* GetMetaClassByEType(EType etype);    
    static std::string GetSelfMetaName(const std::string& name);    
    static MetaNode* GetCoreMetaClass(const std::string& name);
    
    // Getter methods for accessing meta classes
    MetaClass* GetObjectMetaClass() const { return objectMetaClass; }
    MetaClass* GetNullMetaClass() const { return nullMetaClass; }
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

} // namespace Core
} // namespace SimpleLanguage
