#pragma once

#include "SObject.h"
#include <string>

namespace SimpleLanguage {
namespace VM {

    class Int16Object : public SObject {
    public:
        int16_t value;

        Int16Object(int16_t val);
        void SetValue(int16_t _val);
        std::string ToFormatString() override;
    };

    class UInt16Object : public SObject {
    public:
        uint16_t value;

        UInt16Object(uint16_t val);
        void SetValue(uint16_t _val);
        std::string ToFormatString() override;
    };

} // namespace VM
} // namespace SimpleLanguage
