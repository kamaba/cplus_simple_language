#include "TemplateObject.h"
#include "ClassObject.h"

namespace SimpleLanguage {
namespace VM {

    TemplateObject::TemplateObject() {}

    void TemplateObject::SetClassObject(ClassObject* val) {
        value = val;
        val->refCount++;
    }

    void TemplateObject::SetValue(EType _eType, const std::any& val) {
        m_Etype = _eType;
        value = val;
        m_IsVoid = false;
    }

    std::string TemplateObject::ToFormatString() {
        return "";
    }

} // namespace VM
} // namespace SimpleLanguage
