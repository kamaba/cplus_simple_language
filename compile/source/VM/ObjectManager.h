#pragma once

#include <map>
#include <string>
#include "SValue.h"
#include "ClassObject.h"

namespace SimpleLanguage {
namespace VM {

    // Forward declarations
    class RuntimeType;
    class IRMetaClass;

    class ObjectManager {
    public:
        static std::map<int, ClassObject*> classObjectDict;

        static void AddClassObject(ClassObject* cl);
        static SValue CreateValueByDefineType(IRMetaClass* mdt);
        static SObject* CreateObjectByRuntimeType(RuntimeType* rt, bool isCreateMemObject = false);
        static void SetObjectByValue(SObject* obj, SValue& svalue);
        static void SetValueByValue(SValue& target, const SValue& svalue);
    };

} // namespace VM
} // namespace SimpleLanguage
