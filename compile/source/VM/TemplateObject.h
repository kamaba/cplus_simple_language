#pragma once

#include "SObject.h"
#include <string>
#include <any>

namespace SimpleLanguage {
namespace VM {

    // Forward declaration
    class ClassObject;

    class TemplateObject : public SObject {
    private:
        ClassObject* m_ExtendObject = nullptr;

    public:
        std::any value;

        TemplateObject();
        void SetClassObject(ClassObject* val);
        void SetValue(EType _eType, const std::any& val);
        std::string ToFormatString() override;
    };

} // namespace VM
} // namespace SimpleLanguage
