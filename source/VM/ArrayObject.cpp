#include "ArrayObject.h"

namespace SimpleLanguage {
namespace VM {

    template<typename T>
    ArrayObject<T>::ArrayObject() {}

    template<typename T>
    void ArrayObject<T>::SetValue(uint8_t _val) {
        value = _val;
    }

    template<typename T>
    std::string ArrayObject<T>::ToFormatString() {
        return "";
    }

} // namespace VM
} // namespace SimpleLanguage
