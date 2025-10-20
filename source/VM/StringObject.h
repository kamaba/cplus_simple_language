#pragma once

#include "SObject.h"
#include <string>

namespace SimpleLanguage {
namespace VM {

    class StringObject : public SObject {
    public:
        std::string value;

        StringObject(const std::string& str);
        void SetValue(const std::string& _val);
        static StringObject* SetToString(); // Assuming Int32MetaClass equivalent
        std::string ToFormatString() override;
    };

} // namespace VM
} // namespace SimpleLanguage
