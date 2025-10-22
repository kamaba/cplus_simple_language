#pragma once

#include "SObject.h"
#include <string>
#include <any>

namespace SimpleLanguage {
namespace VM {

    class AnyObject : public SObject {
    public:
        std::any value;

        AnyObject();
        void SetValue(EType _eType, const std::any& val);
        std::string ToFormatString() override;
    };

} // namespace VM
} // namespace SimpleLanguage
