#include "BoolObject.h"
#include <sstream>

namespace SimpleLanguage {
namespace VM {

    BoolObject::BoolObject(bool flag) : SObject(EType::Boolean) {
        value = flag;
    }

    void BoolObject::SetValue(bool _val) {
        value = _val;
    }

    std::string BoolObject::ToFormatString() {
        return value ? "true" : "false";
    }

} // namespace VM
} // namespace SimpleLanguage
