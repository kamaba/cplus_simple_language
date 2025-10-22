#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "../IR/IRBase.h"

namespace SimpleLanguage {
namespace VM {

class SValue;
class SObject;
class ByteObject;
class SByteObject;
class Int16Object;
class UInt16Object;
class Int32Object;
class UInt32Object;
class Int64Object;
class UInt64Object;
class FloatObject;
class DoubleObject;
class StringObject;
class ClassObject;
class TemplateObject;
class ObjectManager;
class Log;
class EError;
class IRManager;
class IRExpress;

class RuntimeType {
public:
    IRMetaClass* irClass;
    std::vector<RuntimeType*> runtimeTemplateList;

private:
    SObject** m_StaticMemObjectList;

public:
    RuntimeType(IRMetaClass* rc, std::vector<RuntimeType*>* rtList);
    ~RuntimeType();
    
    RuntimeType* GetClassRuntimeType(IRMetaType* irmt, bool isAdd = false);
    void GetMemberVariableSValue(int index, SValue& svalue);
    void SetMemberVariableSValue(int index, const SValue& svalue);
    
    std::vector<IRData*> CreateStaticMetaMetaVariableIRList();
    static bool SameRuntimeType(RuntimeType* rt1, RuntimeType* rt2);
    
    std::string ToString();
};

class RuntimeTypeManager {
public:
    static std::vector<RuntimeType*>& GetRuntimeList();
    
    static RuntimeType* GetRuntimeTypeByMTAndTemplateMT(IRMetaClass* rmc, std::vector<RuntimeType*>* inputTemplateTypeList);
    static RuntimeType* GetRuntimeTypeByMTAndIRMetaClass(IRMetaClass* rmc);
    static RuntimeType* AddRuntimeTypeByClassAndTemplate(IRMetaClass* rmc, std::vector<RuntimeType*>* inputTemplateTypeList);
    static RuntimeType* AddRuntimeTypeByClass(IRMetaClass* rmc);

private:
    static std::vector<RuntimeType*> s_RuntimeList;
};

} // namespace VM
} // namespace SimpleLanguage
