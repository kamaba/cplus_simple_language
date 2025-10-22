#include "AnyObject.h"
#include <sstream>

namespace SimpleLanguage {
namespace VM {

    AnyObject::AnyObject() {}

    void AnyObject::SetValue(EType _eType, const std::any& val) {
        m_Etype = _eType;
        value = val;
        m_IsVoid = false;
    }

    std::string AnyObject::ToFormatString() {
        // This is a simplified version - in practice you'd need to handle different types
        return "AnyObject";
    }

} // namespace VM
} // namespace SimpleLanguage
