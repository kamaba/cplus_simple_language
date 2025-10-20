#include "Int16Object.h"
#include <sstream>

namespace SimpleLanguage {
namespace VM {

    Int16Object::Int16Object(int16_t val) : SObject(EType::Int16) {
        value = val;
    }

    void Int16Object::SetValue(int16_t _val) {
        value = _val;
    }

    std::string Int16Object::ToFormatString() {
        return std::to_string(value);
    }

    // UInt16Object implementation
    UInt16Object::UInt16Object(uint16_t val) : SObject(EType::UInt16) {
        value = val;
    }

    void UInt16Object::SetValue(uint16_t _val) {
        value = _val;
    }

    std::string UInt16Object::ToFormatString() {
        return std::to_string(value);
    }

} // namespace VM
} // namespace SimpleLanguage
