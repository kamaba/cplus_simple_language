#pragma once

#include "SObject.h"
#include <string>

namespace SimpleLanguage {
namespace VM {

    class ByteObject : public SObject {
    public:
        uint8_t value;

        ByteObject(uint8_t _val);
        void SetValue(uint8_t _val);
        std::string ToFormatString() override;
    };

    class SByteObject : public SObject {
    public:
        int8_t value;

        SByteObject(int8_t _val);
        void SetValue(int8_t _val);
        std::string ToFormatString() override;
    };

} // namespace VM
} // namespace SimpleLanguage
