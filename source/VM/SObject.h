#pragma once

#include <string>
#include "../Core/MetaType.h" // Assuming MetaType.h exists
#include "../IR/IRData.h" // Assuming IRData.h exists

namespace SimpleLanguage {
namespace VM {

    // Forward declaration
    class RuntimeType;

    class SObject {
    public:
        bool isNull() const { return m_IsNull; }
        bool isVoid() const { return m_IsVoid; }
        EType eType() const { return m_Etype; }
        RuntimeType* runtimeType() const { return m_RuntimeType; }
        short typeId = 0;
        int refCount = 0;

    protected:
        EType m_Etype = EType::Class;
        bool m_IsNull = false;
        bool m_IsVoid = false;
        RuntimeType* m_RuntimeType = nullptr;

    public:
        SObject();
        SObject(EType etype);
        virtual ~SObject();

        virtual void SetNull();
        void SetVoid();
        virtual std::string ToFormatString();
    };

} // namespace VM
} // namespace SimpleLanguage
