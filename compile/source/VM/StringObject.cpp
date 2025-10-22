#include "StringObject.h"

namespace SimpleLanguage {
namespace VM {

    StringObject::StringObject(const std::string& str) : SObject(EType::String) {
        value = str;
    }

    void StringObject::SetValue(const std::string& _val) {
        value = _val;
    }

    StringObject* StringObject::SetToString() {
        StringObject* s = new StringObject("");
        return s;
    }

    std::string StringObject::ToFormatString() {
        return value;
    }

} // namespace VM
} // namespace SimpleLanguage
