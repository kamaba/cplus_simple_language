#include "LocalRuntime.h"
#include "../IR/IRMethod.h" // Assuming IRMethod.h exists
#include "../IR/IRBase.h" // Assuming IRBase.h exists

namespace SimpleLanguage {
namespace VM {
namespace Runtime {

    LocalRuntime* LocalRuntime::s_Instance = nullptr;
    std::stack<int> LocalRuntime::stackInt;

    LocalRuntime* LocalRuntime::instance() {
        if (s_Instance == nullptr) {
            s_Instance = new LocalRuntime();
        }
        return s_Instance;
    }

    void LocalRuntime::RunFunction(IRMethod* mmf) {
        // Implementation for running function
    }

    void LocalRuntime::RunInstruction(IRBase* iri) {
        // Implementation for running instruction
    }

} // namespace Runtime
} // namespace VM
} // namespace SimpleLanguage
