#include "DoubleObject.h"
#include <sstream>

namespace SimpleLanguage {
namespace VM {

    DoubleObject::DoubleObject() : SObject(EType::Float64) {}

    void DoubleObject::SetValue(double _val) {
        value = _val;
    }

    std::string DoubleObject::ToFormatString() {
        return std::to_string(value);
    }

} // namespace VM
} // namespace SimpleLanguage
