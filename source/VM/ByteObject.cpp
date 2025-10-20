#include "ByteObject.h"
#include <sstream>

namespace SimpleLanguage {
namespace VM {

    ByteObject::ByteObject(uint8_t _val) : SObject(EType::Byte) {
        value = _val;
    }

    void ByteObject::SetValue(uint8_t _val) {
        value = _val;
    }

    std::string ByteObject::ToFormatString() {
        return std::to_string(value);
    }

    // SByteObject implementation
    SByteObject::SByteObject(int8_t _val) : SObject(EType::SByte) {
        value = _val;
    }

    void SByteObject::SetValue(int8_t _val) {
        value = _val;
    }

    std::string SByteObject::ToFormatString() {
        return std::to_string(value);
    }

} // namespace VM
} // namespace SimpleLanguage
