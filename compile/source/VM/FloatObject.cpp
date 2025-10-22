#include "FloatObject.h"
#include <sstream>
#include <stdexcept>

namespace SimpleLanguage {
namespace VM {

    FloatObject::FloatObject() : SObject(EType::Float32) {}

    void FloatObject::SetValue(float _val) {
        value = _val;
    }

    int32_t FloatObject::ToInt() {
        return static_cast<int32_t>(value);
    }

    std::string FloatObject::ToFormatString() {
        return std::to_string(value);
    }

} // namespace VM
} // namespace SimpleLanguage
