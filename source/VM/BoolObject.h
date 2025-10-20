#pragma once

#include "SObject.h"
#include <string>

namespace SimpleLanguage {
namespace VM {

    class BoolObject : public SObject {
    public:
        bool value = false;

        BoolObject(bool flag);
        void SetValue(bool _val);
        std::string ToFormatString() override;
    };

} // namespace VM
} // namespace SimpleLanguage
