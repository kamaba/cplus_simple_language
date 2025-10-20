#pragma once

#include "SObject.h"
#include <string>

namespace SimpleLanguage {
namespace VM {

    class Int32Object : public SObject {
    public:
        int32_t value = 0;

        Int32Object(int32_t obj);
        void SetValue(int32_t _val);
        void AddInt32(Int32Object* int32Obj);
        void MinusInt32(Int32Object* int32Obj);
        void MultiplyInt32(Int32Object* int32Obj);
        void Divide(Int32Object* int32Obj);
        void BeDivide(Int32Object* int32Obj);
        void Modulo(Int32Object* int32Obj);
        uint8_t ToByte();
        int8_t ToSByte();
        char ToChar();
        int16_t ToShort();
        int16_t ToInt16();
        int32_t ToInt();
        int32_t ToInt32();
        float ToFloat();
        double ToDouble();
        std::string Int32ToString();
        std::string ToFormatString() override;
    };

    class UInt32Object : public SObject {
    public:
        uint32_t value;

        UInt32Object();
        UInt32Object(uint32_t obj);
        void SetValue(uint32_t _val);
        std::string ToFormatString() override;
    };

} // namespace VM
} // namespace SimpleLanguage
