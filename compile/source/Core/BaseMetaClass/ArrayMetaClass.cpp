//****************************************************************************
//  File:      ArrayMetaClass.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "ArrayMetaClass.h"
#include "../MetaTemplate.h"
#include "../MetaMemberVariable.h"
#include "../MetaMemberFunction.h"
#include "../MetaExpressNode/MetaExpressConst.h"
#include <cstdlib>

namespace SimpleLanguage {
namespace Core {

IEnumerableMetaClass::IEnumerableMetaClass() : MetaClass("Array") {
    m_Type = EType::Array;
    m_ClassDefineType = EClassDefineType::InnerDefine;
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
}

ArrayIteratorMetaClass::ArrayIteratorMetaClass() : MetaClass("Array") {
    m_Type = EType::Class;
    m_ClassDefineType = EClassDefineType::InnerDefine;
}

void ArrayIteratorMetaClass::ParseInnerVariable() {
    // MetaConstExpressNode ecen = new MetaConstExpressNode(EType.Int32, 0);
    // MetaMemberVariable index = new MetaMemberVariable(this, "index", CoreMetaClassManager.int32MetaClass, ecen);
    // AddMetaMemberVariable(index);

    // MetaMemberVariable tvalue = new MetaMemberVariable(this, "value", CoreMetaClassManager.templateMetaClass);
    // AddMetaMemberVariable(tvalue);
}

MetaClass* ArrayIteratorMetaClass::CreateMetaClass() {
    ArrayIteratorMetaClass* mc = new ArrayIteratorMetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetCoreModule());
    return mc;
}

ArrayMetaClass::ArrayMetaClass() : MetaClass("Array") {
    m_Type = EType::Array;
    m_ClassDefineType = EClassDefineType::InnerDefine;
    SetExtendClass(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
    m_MetaTemplateList.push_back( new MetaTemplate(this, "T"));
}

void ArrayMetaClass::ParseInnerVariable() {
    // MetaMemberVariable m_Count = new MetaMemberVariable(this, "m_Count", CoreMetaClassManager.uint32MetaClass);
    // AddMetaMemberVariable(m_Count);

    // MetaMemberVariable m_Bound1 = new MetaMemberVariable(this, "m_Bound1", CoreMetaClassManager.uint16MetaClass);
    // AddMetaMemberVariable(m_Bound1);
    // MetaMemberVariable m_Bound2 = new MetaMemberVariable(this, "m_Bound2", CoreMetaClassManager.uint16MetaClass);
    // AddMetaMemberVariable(m_Bound2);

    // MetaMemberVariable m_Index = new MetaMemberVariable(this, "m_Index", CoreMetaClassManager.int32MetaClass);
    // AddMetaMemberVariable(m_Index);

    // MetaMemberVariable m_Value = new MetaMemberVariable(this, "m_Value", new MetaTemplate(this, "T") );
    // AddMetaMemberVariable(m_Value);
}

void ArrayMetaClass::ParseInnerFunction() {
    AddCoreFunction();
}

void ArrayMetaClass::AddCoreFunction() {
    // MetaMemberFunction _init_ = new MetaMemberFunction(this, "_init_");
    // MetaExpressNode men = new MetaConstExpressNode(EType.Int32, 0);
    // _init_.AddMetaDefineParam(new MetaDefineParam("_count", this, null, CoreMetaClassManager.int32MetaClass, men));
    // _init_.AddMetaDefineParam(new MetaDefineParam("bound1", this, null, CoreMetaClassManager.int32MetaClass, men));
    // AddInnerMetaMemberFunction(_init_);

    //////Array.AddMetaDefineParam(new MetaDefineParam("bound2", this, null, CoreMetaClassManager.int16MetaClass, men));
    //////Array.AddMetaDefineParam(new MetaDefineParam("bound3", this, null, CoreMetaClassManager.int16MetaClass, men));
    //////Array.AddMetaDefineParam(new MetaDefineParam("bound4", this, null, CoreMetaClassManager.int16MetaClass, men));

    // MetaMemberFunction AddT = new MetaMemberFunction( this , "Add");
    // AddT.AddMetaDefineParam(new MetaDefineParam("T", this, null, new MetaTemplate(this, "T") ));
    // AddInnerMetaMemberFunction(AddT);

    // MetaMemberFunction RemoveIndex = new MetaMemberFunction(this, "RemoveIndex");
    // RemoveIndex.AddMetaDefineParam(new MetaDefineParam("index", this, null, CoreMetaClassManager.int32MetaClass, null));
    ////RemoveIndex.SetMetaDefineType(new MetaDefineType(CoreMetaClassManager.int32MetaClass));
    // AddInnerMetaMemberFunction(RemoveIndex);

    // MetaMemberFunction Remove = new MetaMemberFunction(this, "Remove");
    ////Remove.AddMetaDefineParam(new MetaDefineParam("T", this, null, CoreMetaClassManager.templateMetaClass, null));
    ////Remove.SetMetaDefineType(new MetaType(CoreMetaClassManager.objectMetaClass));
    ////AddInnerMetaMemberFunction(Remove);

    // MetaMemberFunction SetLength = new MetaMemberFunction(this, "SetLength");
    // SetLength.AddMetaDefineParam(new MetaDefineParam("length", this, null, CoreMetaClassManager.int32MetaClass, null));
    // SetLength.SetMetaDefineType(new MetaType(CoreMetaClassManager.voidMetaClass));
    // AddInnerMetaMemberFunction(SetLength);

    // MetaMemberFunction Count = new MetaMemberFunction(this, "Count");
    // Count.SetMetaDefineType(new MetaType(CoreMetaClassManager.int32MetaClass));
    // AddInnerMetaMemberFunction(Count);

    // MetaMemberFunction IsIn = new MetaMemberFunction(this, "IsIn");
    ////IsIn.AddMetaDefineParam(new MetaDefineParam("T", this, null, CoreMetaClassManager.templateMetaClass, null));
    ////IsIn.SetMetaDefineType(new MetaType(CoreMetaClassManager.voidMetaClass));
    ////AddInnerMetaMemberFunction(IsIn);
}

MetaClass* ArrayMetaClass::CreateMetaClass() {
    MetaClass* mc = new ArrayMetaClass();
    ClassManager::GetInstance().AddMetaClass(mc, ModuleManager::GetInstance().GetSelfModule());
    return mc;
}

int ArrayMetaClass::SetArrayLength(uint32_t v) {
    void* ptr = std::malloc(v);
    if (ptr != nullptr) {
        return reinterpret_cast<int>(ptr);
    }
    return -1;
}

} // namespace Core
} // namespace SimpleLanguage