#pragma once

#include "SObject.h"
#include <string>

namespace SimpleLanguage {
namespace VM {

    class DoubleObject : public SObject {
    public:
        double value;

        DoubleObject();
        void SetValue(double _val);
        std::string ToFormatString() override;
    };

} // namespace VM
} // namespace SimpleLanguage
