#include "InnerCLRRRuntimeVM.h"
#include "RuntimeVM.h"
#include "RuntimeType.h"
#include "../IR/IRBase.h"
#include "../IR/IRManager.h"
#include <iostream>

namespace SimpleLanguage {
namespace VM {

// Static member initialization
bool InnerCLRRuntimeVM::isPrint = false;
RuntimeVM* InnerCLRRuntimeVM::currentCLRRuntime = nullptr;
RuntimeVM* InnerCLRRuntimeVM::topCLRRuntime = nullptr;
std::stack<RuntimeVM*> InnerCLRRuntimeVM::m_ClrRuntimeStack;

InnerCLRRuntimeVM::InnerCLRRuntimeVM() {
    // Constructor
}

std::stack<RuntimeVM*>& InnerCLRRuntimeVM::GetClrRuntimeStack() {
    return m_ClrRuntimeStack;
}

RuntimeVM* InnerCLRRuntimeVM::GetCLRRuntimeById(const std::string& id) {
    std::stack<RuntimeVM*> tempStack = m_ClrRuntimeStack;
    while (!tempStack.empty()) {
        RuntimeVM* v = tempStack.top();
        tempStack.pop();
        if (v->id == id) {
            return v;
        }
    }
    return nullptr;
}

RuntimeVM* InnerCLRRuntimeVM::CreateCLRRuntime(std::vector<RuntimeType*>* irmtList, IRMethod* _irMethod) {
    RuntimeVM* getrt = GetCLRRuntimeById(_irMethod->id);
    //if (getrt != nullptr) {
    //    return getrt;
    //} else {
        RuntimeVM* clrRuntime = new RuntimeVM(irmtList, _irMethod);
        clrRuntime->id = _irMethod->id;
        m_ClrRuntimeStack.push(clrRuntime);
        return clrRuntime;
    //}
}

void InnerCLRRuntimeVM::PushCLRRuntime(RuntimeVM* clrRuntime) {
    m_ClrRuntimeStack.push(clrRuntime);
}

RuntimeVM* InnerCLRRuntimeVM::PopCLRRuntime() {
    RuntimeVM* result = m_ClrRuntimeStack.top();
    m_ClrRuntimeStack.pop();
    return result;
}

void InnerCLRRuntimeVM::Init() {
    //var staticArray = IRManager.instance.staticVariableList;
    //m_StaticVariableValueArray = new SValue[staticArray.Count];
    //for (int i = 0; i < staticArray.Count; i++)
    //{
    //    m_StaticVariableValueArray[i] = ObjectManager.CreateValueByDefineType( staticArray[i].irMetaClass );
    //}
    //InnverCLRRuntimeVM.RootInnerCLRRuntime 
    RuntimeVM* clrRuntime = new RuntimeVM(IRManager::instance->irDataList);
    clrRuntime->isPersistent = true;
    clrRuntime->id = "InnverCLRRuntimeVM.CLRRuntime.EntryMethod()";
    InnerCLRRuntimeVM::PushCLRRuntime(clrRuntime);
    clrRuntime->Run();
}

void InnerCLRRuntimeVM::RunIRMethod(std::vector<RuntimeType*>* irmtList, IRMethod* _irMethod) {
    topCLRRuntime = m_ClrRuntimeStack.top();
    RuntimeVM* clrRuntime = InnerCLRRuntimeVM::CreateCLRRuntime(irmtList, _irMethod);
    clrRuntime->Run();
    InnerCLRRuntimeVM::PopCLRRuntime();
    RuntimeVM* topt2 = m_ClrRuntimeStack.top();
    topt2->AddReturnObjectArray(clrRuntime->returnObjectArray);
    //if (!clrRuntime.isPersistent)
    {
    }
}

} // namespace VM
} // namespace SimpleLanguage
