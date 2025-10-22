#include "SValue.h"
#include "BoolObject.h"
#include "ByteObject.h"
#include "SByteObject.h"
#include "Int16Object.h"
#include "UInt16Object.h"
#include "Int32Object.h"
#include "UInt32Object.h"
#include "Int64Object.h"
#include "UInt64Object.h"
#include "FloatObject.h"
#include "DoubleObject.h"
#include "StringObject.h"
#include "AnyObject.h"
#include "TemplateObject.h"
#include "../Debug/Log.h" // Assuming Log.h exists
#include <iostream>
#include <sstream>

namespace SimpleLanguage {
namespace VM {

    SValue::SValue() : eType(EType::Null), int8Value(0), sint8Value(0), int16Value(0), uint16Value(0),
                       int32Value(0), uint32Value(0), int64Value(0), uint64Value(0),
                       floatValue(0.0f), doubleValue(0.0), sobject(nullptr) {}

    SValue::~SValue() {
        // No dynamic allocation to clean up
    }

    void SValue::SetNullValue() {
        eType = EType::Null;
    }

    void SValue::SetBoolValue(bool val) {
        eType = EType::Boolean;
        int8Value = val ? 1 : 0;
    }

    void SValue::SetNull() {
        if (sobject != nullptr) {
            sobject->SetNull();
        }
    }

    void SValue::SetInt8Value(uint8_t val) {
        eType = EType::Byte;
        int8Value = val;
    }

    void SValue::SetSInt8Value(int8_t val) {
        eType = EType::SByte;
        sint8Value = val;
    }

    void SValue::SetInt16Value(int16_t val) {
        eType = EType::Int16;
        int16Value = val;
    }

    void SValue::SetUInt16Value(uint16_t val) {
        eType = EType::UInt16;
        uint16Value = val;
    }

    void SValue::SetInt32Value(int32_t val) {
        eType = EType::Int32;
        int32Value = val;
    }

    void SValue::SetUInt32Value(uint32_t val) {
        eType = EType::UInt32;
        uint32Value = val;
    }

    void SValue::SetInt64Value(int64_t val) {
        eType = EType::Int64;
        int64Value = val;
    }

    void SValue::SetUInt64Value(uint64_t val) {
        eType = EType::UInt64;
        uint64Value = val;
    }

    void SValue::SetFloatValue(float val) {
        eType = EType::Float32;
        floatValue = val;
    }

    void SValue::SetDoubleValue(double val) {
        eType = EType::Float64;
        doubleValue = val;
    }

    void SValue::SetStringValue(const std::string& val) {
        eType = EType::String;
        stringValue = val;
    }

    void SValue::ConvertByEType(EType neType) {
        void* cur = GetValueObject();
        
        switch (neType) {
            case EType::Byte: {
                eType = EType::Byte;
                int8Value = static_cast<uint8_t>(*static_cast<int*>(cur));
                break;
            }
            case EType::SByte: {
                eType = EType::SByte;
                sint8Value = static_cast<int8_t>(*static_cast<int*>(cur));
                break;
            }
            case EType::Int16: {
                eType = EType::Int16;
                int16Value = static_cast<int16_t>(*static_cast<int*>(cur));
                break;
            }
            case EType::UInt16: {
                eType = EType::Float64; // Note: This seems to be a bug in the original C# code
                doubleValue = static_cast<uint16_t>(*static_cast<int*>(cur));
                break;
            }
            case EType::Int32: {
                eType = EType::Int32;
                int32Value = *static_cast<int32_t*>(cur);
                break;
            }
            case EType::UInt32: {
                eType = EType::UInt32;
                uint32Value = *static_cast<uint32_t*>(cur);
                break;
            }
            case EType::Int64: {
                eType = EType::Int64;
                int64Value = *static_cast<int64_t*>(cur);
                break;
            }
            case EType::UInt64: {
                eType = EType::UInt64;
                uint64Value = *static_cast<uint64_t*>(cur);
                break;
            }
            case EType::Float32: {
                eType = EType::Float32;
                floatValue = *static_cast<float*>(cur);
                break;
            }
            case EType::Float64: {
                eType = EType::Float64;
                doubleValue = *static_cast<double*>(cur);
                break;
            }
            case EType::String: {
                eType = EType::String;
                stringValue = static_cast<std::string*>(cur)->c_str();
                break;
            }
            default: {
                std::cout << "Error 异常类型在ConvertByEType中" << std::endl;
                break;
            }
        }
    }

    void* SValue::GetValueObject() {
        switch (this->eType) {
            case EType::Byte:
                return &int8Value;
            case EType::SByte:
                return &sint8Value;
            case EType::Int16:
                return &int16Value;
            case EType::UInt16:
                return &uint16Value;
            case EType::Int32:
                return &int32Value;
            case EType::UInt32:
                return &uint32Value;
            case EType::Int64:
                return &int64Value;
            case EType::UInt64:
                return &uint64Value;
            case EType::Float32:
                return &floatValue;
            case EType::Float64:
                return &doubleValue;
            case EType::String:
                return &stringValue;
            default:
                return sobject;
        }
    }

    void SValue::SetSObject(SObject* val) {
        if (val == nullptr) return;

        // Type checking and assignment based on SObject type
        if (BoolObject* boolobj = dynamic_cast<BoolObject*>(val)) {
            eType = EType::Boolean;
            int8Value = boolobj->value ? 1 : 0;
        } else if (ByteObject* int8obj = dynamic_cast<ByteObject*>(val)) {
            eType = EType::Byte;
            int8Value = int8obj->value;
        } else if (SByteObject* sint8obj = dynamic_cast<SByteObject*>(val)) {
            eType = EType::SByte;
            sint8Value = sint8obj->value;
        } else if (Int16Object* int16obj = dynamic_cast<Int16Object*>(val)) {
            eType = EType::Int16;
            int16Value = int16obj->value;
        } else if (UInt16Object* uint16obj = dynamic_cast<UInt16Object*>(val)) {
            eType = EType::UInt16;
            uint16Value = uint16obj->value;
        } else if (Int32Object* int32obj = dynamic_cast<Int32Object*>(val)) {
            eType = EType::Int32;
            int32Value = int32obj->value;
        } else if (UInt32Object* uint32obj = dynamic_cast<UInt32Object*>(val)) {
            eType = EType::UInt32;
            uint32Value = uint32obj->value;
        } else if (Int64Object* int64obj = dynamic_cast<Int64Object*>(val)) {
            eType = EType::Int64;
            int64Value = int64obj->value;
        } else if (UInt64Object* uint64obj = dynamic_cast<UInt64Object*>(val)) {
            eType = EType::UInt64;
            uint64Value = uint64obj->value;
        } else if (FloatObject* floatobj = dynamic_cast<FloatObject*>(val)) {
            eType = EType::Float32;
            floatValue = floatobj->value;
        } else if (DoubleObject* doubleobj = dynamic_cast<DoubleObject*>(val)) {
            eType = EType::Float64;
            doubleValue = doubleobj->value;
        } else if (StringObject* stringobj = dynamic_cast<StringObject*>(val)) {
            eType = EType::String;
            stringValue = stringobj->value;
        } else if (AnyObject* anyobj = dynamic_cast<AnyObject*>(val)) {
            eType = anyobj->eType;
            void* tobj = anyobj->value;
            switch (eType) {
                case EType::Byte:
                    int8Value = *static_cast<uint8_t*>(tobj);
                    break;
                case EType::SByte:
                    sint8Value = *static_cast<int8_t*>(tobj);
                    break;
                case EType::Int16:
                    int16Value = *static_cast<int16_t*>(tobj);
                    break;
                case EType::UInt16:
                    uint16Value = *static_cast<uint16_t*>(tobj);
                    break;
                case EType::Int32:
                    int32Value = *static_cast<int32_t*>(tobj);
                    break;
                case EType::UInt32:
                    uint32Value = *static_cast<uint32_t*>(tobj);
                    break;
                case EType::Int64:
                    int64Value = *static_cast<int64_t*>(tobj);
                    break;
                case EType::UInt64:
                    uint64Value = *static_cast<uint64_t*>(tobj);
                    break;
                case EType::Float32:
                    floatValue = *static_cast<float*>(tobj);
                    break;
                case EType::Float64:
                    doubleValue = *static_cast<double*>(tobj);
                    break;
                case EType::String:
                    stringValue = *static_cast<std::string*>(tobj);
                    break;
                case EType::Class:
                    sobject = static_cast<ClassObject*>(tobj);
                    break;
            }
        } else if (TemplateObject* templateobj = dynamic_cast<TemplateObject*>(val)) {
            if (templateobj->isNull) {
                this->SetNull();
                return;
            }
            eType = templateobj->eType;
            void* tobj = templateobj->value;
            switch (eType) {
                case EType::Byte:
                    int8Value = *static_cast<uint8_t*>(tobj);
                    break;
                case EType::Boolean:
                    int8Value = std::stoi(static_cast<std::string*>(tobj)->c_str()) == 1 ? 1 : 0;
                    break;
                case EType::SByte:
                    sint8Value = *static_cast<int8_t*>(tobj);
                    break;
                case EType::Int16:
                    int16Value = *static_cast<int16_t*>(tobj);
                    break;
                case EType::UInt16:
                    uint16Value = *static_cast<uint16_t*>(tobj);
                    break;
                case EType::Int32:
                    int32Value = *static_cast<int32_t*>(tobj);
                    break;
                case EType::UInt32:
                    uint32Value = *static_cast<uint32_t*>(tobj);
                    break;
                case EType::Int64:
                    int64Value = *static_cast<int64_t*>(tobj);
                    break;
                case EType::UInt64:
                    uint64Value = *static_cast<uint64_t*>(tobj);
                    break;
                case EType::Float32:
                    floatValue = *static_cast<float*>(tobj);
                    break;
                case EType::Float64:
                    doubleValue = *static_cast<double*>(tobj);
                    break;
                case EType::String:
                    stringValue = *static_cast<std::string*>(tobj);
                    break;
                case EType::Class:
                    sobject = static_cast<ClassObject*>(tobj)->value;
                    break;
            }
        } else {
            eType = EType::Class;
            sobject = static_cast<ClassObject*>(val)->value;
        }
    }

    void* SValue::CreateCSharpObject() {
        switch (eType) {
            case EType::Byte:
                return &int8Value;
            case EType::SByte:
                return &sint8Value;
            case EType::Int16:
                return &int16Value;
            case EType::UInt16:
                return &uint16Value;
            case EType::Int32:
                return &int32Value;
            case EType::UInt32:
                return &uint32Value;
            case EType::Int64:
                return &int64Value;
            case EType::UInt64:
                return &uint64Value;
            case EType::Float32:
                return &floatValue;
            case EType::Float64:
                return &doubleValue;
            case EType::String:
                return &stringValue;
        }
        return sobject;
    }

    void SValue::CreateSObjectByCSharpObject(void* obj) {
        // This would need to be implemented based on the actual C# object types
        // For now, this is a placeholder
        std::cout << "CreateSObjectByCSharpObject not fully implemented" << std::endl;
    }

    SObject* SValue::CreateSObject() {
        switch (eType) {
            case EType::Byte:
                return new ByteObject(int8Value);
            case EType::SByte:
                return new SByteObject(sint8Value);
            case EType::Boolean:
                return new BoolObject(int8Value == 1);
            case EType::Int16:
                return new Int16Object(int16Value);
            case EType::UInt16:
                return new UInt16Object(uint16Value);
            case EType::Int32:
                return new Int32Object(int32Value);
            case EType::UInt32:
                return new UInt32Object(uint32Value);
            case EType::Int64:
                return new Int64Object(int64Value);
            case EType::UInt64:
                return new UInt64Object(uint64Value);
            case EType::String:
                return new StringObject(stringValue);
            default:
                return sobject;
        }
    }

    // SValueCompute methods implementation
    void SValue::ComputeSValue(const SValue& sval, bool isUnsignCompute) {
        bool isNumber = false;
        bool isUnsign = false;
        
        switch (eType) {
            case EType::Int32:
            case EType::UInt32: {
                // Implementation for Int32/UInt32
                break;
            }
            case EType::String: {
                switch (sval.eType) {
                    case EType::Byte:
                        stringValue += std::to_string(sval.int8Value);
                        break;
                    case EType::SByte:
                        stringValue += std::to_string(sval.sint8Value);
                        break;
                    case EType::Int16:
                        stringValue += std::to_string(sval.int16Value);
                        break;
                    case EType::UInt16:
                        stringValue += std::to_string(sval.uint16Value);
                        break;
                    case EType::Int32:
                        stringValue += std::to_string(sval.int32Value);
                        break;
                    case EType::UInt32:
                        stringValue += std::to_string(sval.uint32Value);
                        break;
                    case EType::Int64:
                        stringValue += std::to_string(sval.int64Value);
                        break;
                    case EType::UInt64:
                        stringValue += std::to_string(sval.uint64Value);
                        break;
                    case EType::String:
                        stringValue += sval.stringValue;
                        break;
                }
                break;
            }
        }
        
        switch (sval.eType) {
            case EType::Int32:
                int32Value += sval.int32Value;
                break;
            case EType::String:
                SetStringValue(std::to_string(int32Value) + sval.stringValue);
                break;
        }
    }

    void SValue::ComputeSVAlue(int sign, const SValue& svalue, bool isUnSign) {
        switch (this->eType) {
            case EType::Int64: {
                int64_t svalLong = *static_cast<int64_t*>(svalue.GetValueObject());
                if (sign == 0)
                    int64Value += svalLong;
                else if (sign == 1)
                    int64Value -= svalLong;
                else if (sign == 2)
                    int64Value *= svalLong;
                else if (sign == 3)
                    int64Value /= svalLong;
                else if (sign == 4)
                    int64Value %= svalLong;
                else if (sign == 5)
                    int64Value &= svalLong;
                else if (sign == 6)
                    int64Value |= svalLong;
                else if (sign == 7)
                    int64Value = int64Value ^ svalLong;
                else if (sign == 8)
                    int64Value = int64Value << static_cast<int>(svalLong);
                else if (sign == 9)
                    int64Value = int64Value >> static_cast<int>(svalLong);
                break;
            }
            case EType::UInt64: {
                if (svalue.eType == EType::UInt64) {
                    if (sign == 0)
                        uint64Value += svalue.uint64Value;
                    else if (sign == 1)
                        uint64Value -= svalue.uint64Value;
                    else if (sign == 2)
                        uint64Value *= svalue.uint64Value;
                    else if (sign == 3)
                        uint64Value /= svalue.uint64Value;
                    else if (sign == 4)
                        uint64Value %= svalue.uint64Value;
                    else if (sign == 5)
                        uint64Value &= svalue.uint64Value;
                    else if (sign == 6)
                        uint64Value |= svalue.uint64Value;
                    else if (sign == 7)
                        uint64Value = uint64Value ^ svalue.uint64Value;
                    else if (sign == 8)
                        uint64Value = uint64Value << static_cast<int>(svalue.uint64Value);
                    else if (sign == 9)
                        uint64Value = uint64Value >> static_cast<int>(svalue.uint64Value);
                } else {
                    int64_t svalLong = *static_cast<int64_t*>(svalue.GetValueObject());
                    if (sign == 0)
                        int64Value += svalLong;
                    else if (sign == 1)
                        int64Value -= svalLong;
                    else if (sign == 2)
                        int64Value *= svalLong;
                    else if (sign == 3)
                        int64Value /= svalLong;
                    else if (sign == 4)
                        int64Value %= svalLong;
                    else if (sign == 5)
                        int64Value &= svalLong;
                    else if (sign == 6)
                        int64Value |= svalLong;
                    else if (sign == 7)
                        int64Value = int64Value ^ svalLong;
                    else if (sign == 8)
                        int64Value = int64Value << static_cast<int>(svalLong);
                    else if (sign == 9)
                        int64Value = int64Value >> static_cast<int>(svalLong);
                }
                break;
            }
            case EType::Int32: {
                int32_t svalInt = *static_cast<int32_t*>(svalue.GetValueObject());
                if (sign == 0)
                    int32Value += svalInt;
                else if (sign == 1)
                    int32Value -= svalInt;
                else if (sign == 2)
                    int32Value *= svalInt;
                else if (sign == 3)
                    int32Value /= svalInt;
                else if (sign == 4)
                    int32Value %= svalInt;
                else if (sign == 5)
                    int32Value &= svalInt;
                else if (sign == 6)
                    int32Value |= svalInt;
                else if (sign == 7)
                    int32Value = int32Value ^ svalInt;
                else if (sign == 8)
                    int32Value = int32Value << static_cast<int>(svalInt);
                else if (sign == 9)
                    int32Value = int32Value >> static_cast<int>(svalInt);
                break;
            }
            case EType::UInt32: {
                uint32_t svalUInt = *static_cast<uint32_t*>(svalue.GetValueObject());
                if (sign == 0)
                    uint32Value += svalUInt;
                else if (sign == 1)
                    uint32Value -= svalUInt;
                else if (sign == 2)
                    uint32Value *= svalUInt;
                else if (sign == 3)
                    uint32Value /= svalUInt;
                else if (sign == 4)
                    uint32Value %= svalUInt;
                else if (sign == 5)
                    uint32Value &= svalUInt;
                else if (sign == 6)
                    uint32Value |= svalUInt;
                else if (sign == 7)
                    uint32Value = uint32Value ^ svalUInt;
                else if (sign == 8)
                    uint32Value = uint32Value << static_cast<int>(svalUInt);
                else if (sign == 9)
                    uint32Value = uint32Value >> static_cast<int>(svalUInt);
                break;
            }
            case EType::Int16: {
                int16_t svalShort = *static_cast<int16_t*>(svalue.GetValueObject());
                if (sign == 0)
                    int16Value += svalShort;
                else if (sign == 1)
                    int16Value -= svalShort;
                else if (sign == 2)
                    int16Value *= svalShort;
                else if (sign == 3)
                    int16Value /= svalShort;
                else if (sign == 4)
                    int16Value %= svalShort;
                else if (sign == 5)
                    int16Value &= svalShort;
                else if (sign == 6)
                    int16Value |= svalShort;
                else if (sign == 7)
                    int16Value = static_cast<int16_t>(int16Value ^ svalShort);
                else if (sign == 8)
                    int16Value = static_cast<int16_t>(int16Value << svalShort);
                else if (sign == 9)
                    int16Value = static_cast<int16_t>(int16Value >> svalShort);
                break;
            }
            case EType::UInt16: {
                int16_t svalShort = *static_cast<int16_t*>(svalue.GetValueObject());
                if (sign == 0)
                    int16Value += svalShort;
                else if (sign == 1)
                    int16Value -= svalShort;
                else if (sign == 2)
                    int16Value *= svalShort;
                else if (sign == 3)
                    int16Value /= svalShort;
                else if (sign == 4)
                    int16Value %= svalShort;
                else if (sign == 5)
                    int16Value &= svalShort;
                else if (sign == 6)
                    int16Value |= svalShort;
                else if (sign == 7)
                    int16Value = static_cast<int16_t>(int16Value ^ svalShort);
                else if (sign == 8)
                    int16Value = static_cast<int16_t>(int16Value << svalShort);
                else if (sign == 9)
                    int16Value = static_cast<int16_t>(int16Value >> svalShort);
                break;
            }
            case EType::Byte: {
                uint8_t svalByte = *static_cast<uint8_t*>(svalue.GetValueObject());
                if (sign == 0)
                    int8Value += svalByte;
                else if (sign == 1)
                    int8Value -= svalByte;
                else if (sign == 2)
                    int8Value *= svalByte;
                else if (sign == 3)
                    int8Value /= svalByte;
                else if (sign == 4)
                    int8Value %= svalByte;
                else if (sign == 5)
                    int8Value &= svalByte;
                else if (sign == 6)
                    int8Value |= svalByte;
                else if (sign == 7)
                    int8Value = static_cast<uint8_t>(int16Value ^ svalByte);
                else if (sign == 8)
                    int8Value = static_cast<uint8_t>(int16Value << svalByte);
                else if (sign == 9)
                    int8Value = static_cast<uint8_t>(int16Value >> svalByte);
                break;
            }
            case EType::SByte: {
                int8_t svalSbyte = *static_cast<int8_t*>(svalue.GetValueObject());
                if (sign == 0)
                    sint8Value += svalSbyte;
                else if (sign == 1)
                    sint8Value -= svalSbyte;
                else if (sign == 2)
                    sint8Value *= svalSbyte;
                else if (sign == 3)
                    sint8Value /= svalSbyte;
                else if (sign == 4)
                    sint8Value %= svalSbyte;
                else if (sign == 5)
                    sint8Value &= svalSbyte;
                else if (sign == 6)
                    sint8Value |= svalSbyte;
                else if (sign == 7)
                    sint8Value = static_cast<int8_t>(sint8Value ^ svalSbyte);
                else if (sign == 8)
                    sint8Value = static_cast<int8_t>(sint8Value << svalSbyte);
                else if (sign == 9)
                    sint8Value = static_cast<int8_t>(sint8Value >> svalSbyte);
                break;
            }
            case EType::Float32: {
                float svalFloat = *static_cast<float*>(svalue.GetValueObject());
                if (sign == 0)
                    floatValue += svalFloat;
                else if (sign == 1)
                    floatValue -= svalFloat;
                else if (sign == 2)
                    floatValue *= svalFloat;
                else if (sign == 3)
                    floatValue /= svalFloat;
                else if (sign == 4)
                    floatValue = std::fmod(floatValue, svalFloat);
                else {
                    std::cout << "Error 不支持Float 的这种类型的操作" << std::endl;
                }
                break;
            }
            case EType::Float64: {
                double svalDouble = *static_cast<double*>(svalue.GetValueObject());
                if (sign == 0)
                    doubleValue += svalDouble;
                else if (sign == 1)
                    doubleValue -= svalDouble;
                else if (sign == 2)
                    doubleValue *= svalDouble;
                else if (sign == 3)
                    doubleValue /= svalDouble;
                else if (sign == 4)
                    doubleValue = std::fmod(doubleValue, svalDouble);
                else {
                    std::cout << "Error 不支持Double 的这种类型的操作" << std::endl;
                }
                break;
            }
            default: {
                std::cout << "Error -------------" << std::endl;
                break;
            }
        }
    }

    void SValue::AddSValue(const SValue& sval, bool isUnsign) {
        if (sval.eType == EType::String) {
            stringValue = std::string(static_cast<char*>(this->GetValueObject())) + std::string(static_cast<char*>(sval.GetValueObject()));
        } else if (this->eType == EType::String) {
            eType = EType::String;
            stringValue = std::string(static_cast<char*>(GetValueObject())) + std::string(static_cast<char*>(sval.GetValueObject()));
        } else {
            ComputeSVAlue(0, sval, isUnsign);
        }
    }

    void SValue::MinusSValue(const SValue& sval, bool isUnsign) {
        // Implementation for minus operation
    }

    void SValue::MultiplySValue(const SValue& sval, bool isUnsign) {
        // Implementation for multiply operation
    }

    void SValue::DivSValue(const SValue& sval, bool isUnsign) {
        // Implementation for division operation
    }

    void SValue::ModuloSValue(const SValue& sval, bool isUnsign) {
        // Implementation for modulo operation
    }

    void SValue::CombineSValue(const SValue& sval, bool isUnsign) {
        // Implementation for combine operation
    }

    void SValue::InclusiveOrSValue(const SValue& sval, bool isUnsign) {
        // Implementation for inclusive or operation
    }

    void SValue::XORSValue(const SValue& sval, bool isUnsign) {
        // Implementation for XOR operation
    }

    void SValue::ShrSValue(const SValue& sval, bool isUnsign) {
        // Implementation for shift right operation
    }

    void SValue::ShiSValue(const SValue& sval, bool isUnsign) {
        // Implementation for shift left operation
    }

    void SValue::NotSValue() {
        switch (eType) {
            case EType::Byte: {
                eType = EType::Boolean;
                int8Value = (int8Value == 0) ? 1 : 0;
                break;
            }
            case EType::SByte: {
                eType = EType::Boolean;
                int8Value = (sint8Value == 0) ? 1 : 0;
                break;
            }
            case EType::Boolean: {
                int8Value = (int8Value == 0) ? 1 : 0;
                break;
            }
            case EType::Int16: {
                eType = EType::Boolean;
                int8Value = (int16Value == 0) ? 1 : 0;
                break;
            }
            case EType::UInt16: {
                eType = EType::Boolean;
                int8Value = (uint16Value == 0) ? 1 : 0;
                break;
            }
            case EType::Int32: {
                eType = EType::Boolean;
                int8Value = (int32Value == 0) ? 1 : 0;
                break;
            }
            case EType::UInt32: {
                eType = EType::Boolean;
                int8Value = (uint32Value == 0) ? 1 : 0;
                break;
            }
            case EType::Int64: {
                eType = EType::Boolean;
                int8Value = (uint32Value == 0) ? 1 : 0;
                break;
            }
            case EType::UInt64: {
                eType = EType::Boolean;
                int8Value = (uint64Value == 0) ? 1 : 0;
                break;
            }
        }
    }

    void SValue::NegSValue(bool isUnsign) {
        switch (eType) {
            case EType::Byte: {
                eType = EType::Int32;
                int32Value = -int8Value;
                break;
            }
            case EType::SByte: {
                eType = EType::Int32;
                int32Value = -sint8Value;
                break;
            }
            case EType::Int16: {
                int16Value = static_cast<int16_t>(-int16Value);
                break;
            }
            case EType::UInt16: {
                eType = EType::Int32;
                int32Value = (-uint16Value);
                break;
            }
            case EType::Int32: {
                int32Value = -int32Value;
                break;
            }
            case EType::UInt32: {
                eType = EType::Int64;
                int64Value = -uint32Value;
                break;
            }
            case EType::Int64: {
                int64Value = -int64Value;
                break;
            }
            case EType::UInt64: {
                std::cout << "Error -value 1" << std::endl;
                break;
            }
            default: {
                std::cout << "Error -value 2" << std::endl;
                break;
            }
        }
    }

    // SValueCompare methods implementation
    void SValue::SetInt8Compare(uint8_t a, uint8_t b, int compareSign, bool isOrEqual) {
        if (compareSign == 0) {
            SetBoolValue(a == b);
        } else if (compareSign == 1) {
            SetBoolValue(a != b);
        } else if (compareSign == 2) {
            if (isOrEqual) {
                SetBoolValue(a >= b);
            } else {
                SetBoolValue(a > b);
            }
        } else if (compareSign == 3) {
            if (isOrEqual) {
                SetBoolValue(a <= b);
            } else {
                SetBoolValue(a < b);
            }
        }
    }

    void SValue::SetInt16Compare(int16_t a, int16_t b, int compareSign, bool isOrEqual) {
        if (compareSign == 0) {
            SetBoolValue(a == b);
        } else if (compareSign == 1) {
            SetBoolValue(a != b);
        } else if (compareSign == 2) {
            if (isOrEqual) {
                SetBoolValue(a >= b);
            } else {
                SetBoolValue(a > b);
            }
        } else if (compareSign == 3) {
            if (isOrEqual) {
                SetBoolValue(a <= b);
            } else {
                SetBoolValue(a < b);
            }
        }
    }

    void SValue::SetInt32Compare(int32_t a, int32_t b, int compareSign, bool isOrEqual) {
        if (compareSign == 0) {
            SetBoolValue(a == b);
        } else if (compareSign == 1) {
            SetBoolValue(a != b);
        } else if (compareSign == 2) {
            if (isOrEqual) {
                SetBoolValue(a >= b);
            } else {
                SetBoolValue(a > b);
            }
        } else if (compareSign == 3) {
            if (isOrEqual) {
                SetBoolValue(a <= b);
            } else {
                SetBoolValue(a < b);
            }
        }
    }

    void SValue::SetInt64Compare(int64_t a, int64_t b, int compareSign, bool isOrEqual) {
        if (compareSign == 0) {
            SetBoolValue(a == b);
        } else if (compareSign == 1) {
            SetBoolValue(a != b);
        } else if (compareSign == 2) {
            if (isOrEqual) {
                SetBoolValue(a >= b);
            } else {
                SetBoolValue(a > b);
            }
        } else if (compareSign == 3) {
            if (isOrEqual) {
                SetBoolValue(a <= b);
            } else {
                SetBoolValue(a < b);
            }
        }
    }

    void SValue::CompareSValue(const SValue& sval, int compareSign, bool isOrEqual) {
        switch (this->eType) {
            case EType::Int32: {
                switch (sval.eType) {
                    case EType::Byte:
                        SetInt32Compare(int32Value, sval.int8Value, compareSign, isOrEqual);
                        break;
                    case EType::SByte:
                        SetInt32Compare(int32Value, sval.sint8Value, compareSign, isOrEqual);
                        break;
                    case EType::Int16:
                        SetInt32Compare(int32Value, sval.int16Value, compareSign, isOrEqual);
                        break;
                    case EType::UInt16:
                        SetInt32Compare(int32Value, sval.uint16Value, compareSign, isOrEqual);
                        break;
                    case EType::Int32:
                        SetInt32Compare(int32Value, sval.int32Value, compareSign, isOrEqual);
                        break;
                    case EType::UInt32:
                        SetInt64Compare(int32Value, static_cast<int64_t>(sval.uint32Value), compareSign, isOrEqual);
                        break;
                    case EType::Int64:
                        SetInt32Compare(int32Value, sval.int32Value, compareSign, isOrEqual);
                        break;
                    case EType::UInt64:
                        SetInt64Compare(static_cast<int64_t>(int32Value), static_cast<int64_t>(sval.uint64Value), compareSign, isOrEqual);
                        break;
                    case EType::Null:
                        SetBoolValue(false);
                        break;
                }
                break;
            }
            case EType::Boolean: {
                switch (sval.eType) {
                    case EType::Boolean: {
                        if (compareSign == 4) {
                            SetBoolValue(int8Value == sval.int8Value);
                        } else if (compareSign == 5) {
                            SetBoolValue(int8Value == 1 || sval.int8Value == 1);
                        }
                        break;
                    }
                }
                break;
            }
            case EType::Class: {
                switch (sval.eType) {
                    case EType::Null: {
                        SetBoolValue(this->sobject == nullptr);
                        break;
                    }
                }
                break;
            }
        }
    }

} // namespace VM
} // namespace SimpleLanguage
