#pragma once

#include "SObject.h"
#include <string>

namespace SimpleLanguage {
namespace VM {

    class FloatObject : public SObject {
    public:
        float value;

        FloatObject();
        void SetValue(float _val);
        int32_t ToInt();
        std::string ToFormatString() override;
    };

} // namespace VM
} // namespace SimpleLanguage
