#pragma once

#include <stack>
#include <vector>
#include "RuntimeVM.h"
#include "RuntimeType.h"
#include "../IR/IRBase.h"

namespace SimpleLanguage {
namespace VM {

class InnerCLRRuntimeVM {
public:
    static bool isPrint;
    static RuntimeVM* currentCLRRuntime;
    static RuntimeVM* topCLRRuntime;
    static std::stack<RuntimeVM*>& GetClrRuntimeStack();

private:
    static std::stack<RuntimeVM*> m_ClrRuntimeStack;

public:
    InnerCLRRuntimeVM();
    
    static RuntimeVM* GetCLRRuntimeById(const std::string& id);
    static RuntimeVM* CreateCLRRuntime(std::vector<RuntimeType*>* irmtList, IRMethod* _irMethod);
    static void PushCLRRuntime(RuntimeVM* clrRuntime);
    static RuntimeVM* PopCLRRuntime();
    static void Init();
    static void RunIRMethod(std::vector<RuntimeType*>* irmtList, IRMethod* _irMethod);
};

} // namespace VM
} // namespace SimpleLanguage
