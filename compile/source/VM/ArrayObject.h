#pragma once

#include "SObject.h"
#include <string>

namespace SimpleLanguage {
namespace VM {

    // Forward declaration for Array type
    class Array;

    template<typename T>
    class ArrayObject : public SObject {
    public:
        uint8_t value;

        Array* m_Array = nullptr;

        ArrayObject();
        void SetValue(uint8_t _val);
        std::string ToFormatString() override;
    };

} // namespace VM
} // namespace SimpleLanguage
