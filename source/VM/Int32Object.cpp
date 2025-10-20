#include "Int32Object.h"
#include <sstream>
#include <limits>

namespace SimpleLanguage {
namespace VM {

    Int32Object::Int32Object(int32_t obj) : SObject(EType::Int32) {
        value = obj;
        m_Etype = EType::Int32;
    }

    void Int32Object::SetValue(int32_t _val) {
        value = _val;
    }

    void Int32Object::AddInt32(Int32Object* int32Obj) {
        value = value + int32Obj->value;
    }

    void Int32Object::MinusInt32(Int32Object* int32Obj) {
        value = value - int32Obj->value;
    }

    void Int32Object::MultiplyInt32(Int32Object* int32Obj) {
        value = value * int32Obj->value;
    }

    void Int32Object::Divide(Int32Object* int32Obj) {
        if (int32Obj->value != 0) {
            value = value / int32Obj->value;
        }
    }

    void Int32Object::BeDivide(Int32Object* int32Obj) {
        if (value != 0) {
            value = int32Obj->value / value;
        }
    }

    void Int32Object::Modulo(Int32Object* int32Obj) {
        if (int32Obj->value != 0) {
            value = value % int32Obj->value;
        }
    }

    uint8_t Int32Object::ToByte() {
        return static_cast<uint8_t>(value);
    }

    int8_t Int32Object::ToSByte() {
        return static_cast<int8_t>(value);
    }

    char Int32Object::ToChar() {
        return static_cast<char>(value);
    }

    int16_t Int32Object::ToShort() {
        return static_cast<int16_t>(value);
    }

    int16_t Int32Object::ToInt16() {
        return static_cast<int16_t>(value);
    }

    int32_t Int32Object::ToInt() {
        return value;
    }

    int32_t Int32Object::ToInt32() {
        return value;
    }

    float Int32Object::ToFloat() {
        return static_cast<float>(value);
    }

    double Int32Object::ToDouble() {
        return static_cast<double>(value);
    }

    std::string Int32Object::Int32ToString() {
        return std::to_string(value);
    }

    std::string Int32Object::ToFormatString() {
        return std::to_string(value);
    }

    // UInt32Object implementation
    UInt32Object::UInt32Object() {}

    UInt32Object::UInt32Object(uint32_t obj) : SObject(EType::UInt32) {
        value = obj;
        m_Etype = EType::UInt32;
    }

    void UInt32Object::SetValue(uint32_t _val) {
        value = _val;
    }

    std::string UInt32Object::ToFormatString() {
        return std::to_string(value);
    }

} // namespace VM
} // namespace SimpleLanguage
