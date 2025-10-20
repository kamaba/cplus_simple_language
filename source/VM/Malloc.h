#pragma once

#include <cstdlib>
#include <cstdint>

namespace SimpleLanguage {
namespace VM {
namespace Runtime {
namespace Memory {

    class Malloc {
    public:
        static void* NewClass(int bitLong);
    };

} // namespace Memory
} // namespace Runtime
} // namespace VM
} // namespace SimpleLanguage
