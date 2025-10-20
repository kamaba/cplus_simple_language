#pragma once

#include <map>
#include <vector>
#include "../IR/IRBase.h"

namespace SimpleLanguage {
namespace VM {

class ClassObject;
class IRMetaClass;

class RuntimeClass {
public:
    int id;
    IRMetaClass* irMetaClass;
    ClassObject** m_StaticMetaMemberVariableArray;

public:
    RuntimeClass(IRMetaClass* irmc);
    ~RuntimeClass();
    
    ClassObject* GetStaticMetaMemberVaraible(int index);
};

class InnerCLRRegisterClass {
public:
    static InnerCLRRegisterClass* s_Instance;
    static InnerCLRRegisterClass* GetInstance();
    
    std::map<int, RuntimeClass*> m_RuntimeClassDict;
    std::vector<IRMetaClass*> m_IRMetaClassList;

public:
    InnerCLRRegisterClass();
    ~InnerCLRRegisterClass();
    
    void RegisterDymnicClass();
    ClassObject* GetStaticMetaMemberVariable(int classid, int index);
};

} // namespace VM
} // namespace SimpleLanguage
