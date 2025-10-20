#pragma once

#include <string>
#include <iostream>
#include "../IR/IRData.h" // Assuming IRData.h exists
#include "SObject.h" // Forward declaration
#include "ClassObject.h" // Forward declaration

namespace SimpleLanguage {
namespace VM {

    // Forward declarations for SObject types
    class BoolObject;
    class ByteObject;
    class SByteObject;
    class Int16Object;
    class UInt16Object;
    class Int32Object;
    class UInt32Object;
    class Int64Object;
    class UInt64Object;
    class FloatObject;
    class DoubleObject;
    class StringObject;
    class AnyObject;
    class TemplateObject;

    // EType enum (assuming it's defined elsewhere, but including here for completeness)
    enum class EType {
        Null,
        Boolean,
        Byte,
        SByte,
        Int16,
        UInt16,
        Int32,
        UInt32,
        Int64,
        UInt64,
        Float32,
        Float64,
        String,
        Class
    };

    struct SValue {
        EType eType;
        uint8_t int8Value;
        int8_t sint8Value;
        int16_t int16Value;
        uint16_t uint16Value;
        int32_t int32Value;
        uint32_t uint32Value;
        int64_t int64Value;
        uint64_t uint64Value;
        float floatValue;
        double doubleValue;
        std::string stringValue;
        ClassObject* sobject;

        SValue();
        ~SValue();

        void SetNullValue();
        void SetBoolValue(bool val);
        void SetNull();
        void SetInt8Value(uint8_t val);
        void SetSInt8Value(int8_t val);
        void SetInt16Value(int16_t val);
        void SetUInt16Value(uint16_t val);
        void SetInt32Value(int32_t val);
        void SetUInt32Value(uint32_t val);
        void SetInt64Value(int64_t val);
        void SetUInt64Value(uint64_t val);
        void SetFloatValue(float val);
        void SetDoubleValue(double val);
        void SetStringValue(const std::string& val);
        void ConvertByEType(EType neType);
        void* GetValueObject();
        void SetSObject(SObject* val);
        void* CreateCSharpObject();
        void CreateSObjectByCSharpObject(void* obj);
        SObject* CreateSObject();
        
        // SValueCompute methods
        void ComputeSValue(const SValue& sval, bool isUnsignCompute);
        void ComputeSVAlue(int sign, const SValue& svalue, bool isUnSign);
        void AddSValue(const SValue& sval, bool isUnsign);
        void MinusSValue(const SValue& sval, bool isUnsign);
        void MultiplySValue(const SValue& sval, bool isUnsign);
        void DivSValue(const SValue& sval, bool isUnsign);
        void ModuloSValue(const SValue& sval, bool isUnsign);
        void CombineSValue(const SValue& sval, bool isUnsign);
        void InclusiveOrSValue(const SValue& sval, bool isUnsign);
        void XORSValue(const SValue& sval, bool isUnsign);
        void ShrSValue(const SValue& sval, bool isUnsign);
        void ShiSValue(const SValue& sval, bool isUnsign);
        void NotSValue();
        void NegSValue(bool isUnsign);
        
        // SValueCompare methods
        void SetInt8Compare(uint8_t a, uint8_t b, int compareSign, bool isOrEqual);
        void SetInt16Compare(int16_t a, int16_t b, int compareSign, bool isOrEqual);
        void SetInt32Compare(int32_t a, int32_t b, int compareSign, bool isOrEqual);
        void SetInt64Compare(int64_t a, int64_t b, int compareSign, bool isOrEqual);
        void CompareSValue(const SValue& sval, int compareSign, bool isOrEqual);
    };

} // namespace VM
} // namespace SimpleLanguage
