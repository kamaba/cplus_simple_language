#pragma once

#include <stack>
#include "../IR/IRData.h" // Assuming IRData.h exists

namespace SimpleLanguage {
namespace VM {
namespace Runtime {

    // Forward declarations
    class IRMethod;
    class IRBase;

    class LocalRuntime {
    public:
        static LocalRuntime* s_Instance;
        static LocalRuntime* instance();

        static std::stack<int> stackInt;

        static void RunFunction(IRMethod* mmf);
        static void RunInstruction(IRBase* iri);
    };

} // namespace Runtime
} // namespace VM
} // namespace SimpleLanguage
