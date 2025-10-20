#include "VoidObject.h"
#include "StringObject.h"

namespace SimpleLanguage {
namespace VM {

    VoidObject::VoidObject() {
        m_IsVoid = true;
    }

    void VoidObject::SetValue(const std::string& _val) {
        // Void object doesn't store values
    }

    StringObject* VoidObject::SetToString() {
        StringObject* s = new StringObject("");
        return s;
    }

    std::string VoidObject::ToFormatString() {
        return "void";
    }

} // namespace VM
} // namespace SimpleLanguage
