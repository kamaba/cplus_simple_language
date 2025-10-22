#pragma once

#include "SObject.h"
#include "../IR/IRData.h" // Assuming IRData.h exists
#include <vector>
#include <string>
#include <sstream>

namespace SimpleLanguage {
namespace VM {

    // Forward declarations
    class RuntimeType;
    class IRMetaClass;
    class IRMetaVariable;
    class ObjectManager;
    class RuntimeTypeManager;

    struct MemberVariableData {
        int index = 0;
        int start = 0;
        int length = 0;
    };

    class ClassObject : public SObject {
    public:
        ClassObject* value() const { return m_Object; }
        IRMetaClass* irMetaClass() const;

    private:
        ClassObject* m_Object = nullptr;
        std::vector<uint8_t> m_Data;   /*  m_Data  结构  bit形，只有运算时要用 1-> byte 2->sbyte   3-> int16  4-> uint16    */
        std::vector<int16_t> m_Type;
        std::vector<SObject*> m_MemberObjectArray;
        std::vector<IRMetaVariable*> m_IRMetaVariableList;
        std::vector<RuntimeType*> m_IRTemplateList;

    public:
        ClassObject(RuntimeType* irmt, bool isStatic = false);
        ~ClassObject();

        RuntimeType* GetClassRuntimeType(IRMetaType* irmt, bool isAdd = false);
        void CreateObject();
        SObject* GetMemberVariable(int index);
        void SetValue(ClassObject* val);
        void GetMemberVariableSValue(int index, SValue& svalue);
        void SetMemberVariableSValue(int index, const SValue& svalue);
        std::string ToFormatString() override;
        std::string ToString() override;
    };

} // namespace VM
} // namespace SimpleLanguage
