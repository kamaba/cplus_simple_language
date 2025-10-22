#include "Int64Object.h"
#include <sstream>

namespace SimpleLanguage {
namespace VM {

    Int64Object::Int64Object(int64_t val) : SObject(EType::Int64) {
        value = val;
    }

    void Int64Object::SetValue(int64_t _val) {
        value = _val;
    }

    std::string Int64Object::ToFormatString() {
        return std::to_string(value);
    }

    // UInt64Object implementation
    UInt64Object::UInt64Object(uint64_t val) : SObject(EType::UInt64) {
        value = val;
    }

    void UInt64Object::SetValue(uint64_t _val) {
        value = _val;
    }

    std::string UInt64Object::ToFormatString() {
        return std::to_string(value);
    }

} // namespace VM
} // namespace SimpleLanguage
