#pragma once

#include <vector>
#include <string>
#include <stack>
#include "SValue.h"
#include "SObject.h"

namespace SimpleLanguage {
namespace VM {

    // Forward declarations
    class RuntimeType;
    class IRMethod;
    class IRData;
    class IRMetaType;
    class IRMetaClass;
    class InnerCLRRuntimeVM;

    class RuntimeVM {
    public:
        std::string id = "";
        int level = 0;
        bool isPersistent = false;
        std::vector<SObject*> returnObjectArray;

    private:
        std::vector<SValue> m_ValueStack;
        uint16_t m_ValueIndex = 0;

        std::vector<RuntimeType*> m_InputTemplateRuntimeTypeList;
        std::vector<SObject*> m_LocalVariableObjectArray;
        std::vector<SObject*> m_ArgumentObjectArray;
        std::vector<SObject*> m_ReturnObjectArray;

        IRMethod* m_IRMethod = nullptr;
        std::vector<IRData*> m_IRDataList;
        uint16_t m_ExecuteIndex = 0;
        uint16_t m_ExecuteCount = 0;
        std::stack<std::vector<RuntimeType*>> m_NewObjectRuntimeTypeStack;

    public:
        RuntimeVM(const std::vector<RuntimeType*>& inputTemplateTypeList, IRMethod* mmf);
        RuntimeVM(const std::vector<IRData*>& irList);
        ~RuntimeVM();

        void Init();
        SObject* CreateObjectByIRMetaType(IRMetaType* irmt, bool isAdd = false);
        void AddReturnObjectArray(const std::vector<SObject*>& sobjs);
        SObject* GetLocalVariableValue(int index);
        SObject* GetArgumentValue(int index);
        void SetArgumentValue(int index, const SValue& svalue);
        void SetLocalVariableSValue(int index, const SValue& svalue);
        void SetReturnVariableSValue(int index, const SValue& svalue);
        SValue GetCurrentIndexValue(bool isReduce);
        RuntimeType* GetClassRuntimeType(IRMetaType* irmt, bool isAdd = false);
        RuntimeType* GetMethodRuntimeType(IRMetaType* irmt);
        void Run();
        void SetValue(SValue& sValue, SValue& sStore, IRData* iri);
        void RunInstruction(IRData* iri);
    };

} // namespace VM
} // namespace SimpleLanguage
