#include "SObject.h"

namespace SimpleLanguage {
namespace VM {

    SObject::SObject() {}

    SObject::SObject(EType etype) {
        this->m_Etype = etype;
    }

    SObject::~SObject() {}

    void SObject::SetNull() {
        m_IsNull = true;
    }

    void SObject::SetVoid() {
        m_IsVoid = true;
    }

    std::string SObject::ToFormatString() {
        return "";
    }

} // namespace VM
} // namespace SimpleLanguage
