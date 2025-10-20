#pragma once

#include "SObject.h"
#include <string>

namespace SimpleLanguage {
namespace VM {

    class Int64Object : public SObject {
    public:
        int64_t value;

        Int64Object(int64_t val);
        void SetValue(int64_t _val);
        std::string ToFormatString() override;
    };

    class UInt64Object : public SObject {
    public:
        uint64_t value;

        UInt64Object(uint64_t val);
        void SetValue(uint64_t _val);
        std::string ToFormatString() override;
    };

} // namespace VM
} // namespace SimpleLanguage
