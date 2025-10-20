#include "ObjectManager.h"
#include "BoolObject.h"
#include "ByteObject.h"
#include "SByteObject.h"
#include "Int16Object.h"
#include "UInt16Object.h"
#include "Int32Object.h"
#include "UInt32Object.h"
#include "Int64Object.h"
#include "UInt64Object.h"
#include "FloatObject.h"
#include "DoubleObject.h"
#include "StringObject.h"
#include "VoidObject.h"
#include "AnyObject.h"
#include "TemplateObject.h"
#include "../Debug/Log.h" // Assuming Log.h exists
#include <iostream>

namespace SimpleLanguage {
namespace VM {

    std::map<int, ClassObject*> ObjectManager::classObjectDict;

    void ObjectManager::AddClassObject(ClassObject* cl) {
        if (classObjectDict.find(cl->GetHashCode()) == classObjectDict.end()) {
            classObjectDict[cl->GetHashCode()] = cl;
        }
    }

    SValue ObjectManager::CreateValueByDefineType(IRMetaClass* mdt) {
        SValue svalue;
        // Implementation would depend on the specific IRMetaClass structure
        return svalue;
    }

    SObject* ObjectManager::CreateObjectByRuntimeType(RuntimeType* rt, bool isCreateMemObject) {
        SObject* sobj = nullptr;
        std::string name = rt->irClass->irName;
        
        if (name == "Core.Boolean" || name == "Boolean") {
            sobj = new BoolObject(false);
            sobj->typeId = 1;
        } else if (name == "Core.Object" || name == "Object") {
            sobj = new AnyObject();
        } else if (name == "Core.Byte" || name == "Byte") {
            sobj = new ByteObject(0);
            sobj->typeId = 3;
        } else if (name == "Core.SByte" || name == "SByte") {
            sobj = new SByteObject(0);
            sobj->typeId = 3;
        } else if (name == "Core.Int16" || name == "Int16") {
            sobj = new Int16Object(0);
            sobj->typeId = 3;
        } else if (name == "Core.UInt16" || name == "UInt16") {
            sobj = new UInt16Object(0);
            sobj->typeId = 3;
        } else if (name == "Core.Int32" || name == "Int32") {
            sobj = new Int32Object(0);
            sobj->typeId = 3;
        } else if (name == "Core.UInt32" || name == "UInt32") {
            sobj = new UInt32Object(0);
            sobj->typeId = 3;
        } else if (name == "Core.Int64" || name == "Int64") {
            sobj = new Int64Object(0);
            sobj->typeId = 3;
        } else if (name == "Core.UInt64" || name == "UInt64") {
            sobj = new UInt64Object(0);
            sobj->typeId = 3;
        } else if (name == "Core.Float32" || name == "Float32") {
            sobj = new FloatObject();
            sobj->typeId = 4;
        } else if (name == "Core.Float64" || name == "Float64") {
            sobj = new DoubleObject();
            sobj->typeId = 5;
        } else if (name == "Core.String" || name == "String") {
            sobj = new StringObject("");
            sobj->typeId = 10;
        } else if (name == "Core.Void" || name == "Void") {
            sobj = new VoidObject();
            sobj->typeId = 0;
        } else {
            ClassObject* co = new ClassObject(rt);
            if (isCreateMemObject) {
                co->CreateObject();
            }
            sobj = co;
        }
        return sobj;
    }

    void ObjectManager::SetObjectByValue(SObject* obj, SValue& svalue) {
        switch (svalue.eType) {
            case EType::Null: {
                obj->SetNull();
                break;
            }
            case EType::Boolean: {
                TemplateObject* to = dynamic_cast<TemplateObject*>(obj);
                if (to != nullptr) {
                    to->SetValue(EType::Boolean, svalue.int8Value);
                }
                AnyObject* anyObject = dynamic_cast<AnyObject*>(obj);
                if (anyObject != nullptr) {
                    anyObject->SetValue(EType::Boolean, svalue.int8Value);
                    return;
                }
                BoolObject* boolObj = dynamic_cast<BoolObject*>(obj);
                if (boolObj == nullptr) {
                    std::cout << "该类型不是Boolean类型!!" << std::endl;
                    return;
                }
                boolObj->SetValue(svalue.int8Value == 1);
                break;
            }
            case EType::Byte: {
                TemplateObject* to = dynamic_cast<TemplateObject*>(obj);
                if (to != nullptr) {
                    to->SetValue(EType::Byte, svalue.int8Value);
                }
                AnyObject* anyObject = dynamic_cast<AnyObject*>(obj);
                if (anyObject != nullptr) {
                    anyObject->SetValue(EType::Byte, svalue.int8Value);
                    return;
                }
                ByteObject* byteObj = dynamic_cast<ByteObject*>(obj);
                if (byteObj == nullptr) {
                    std::cout << "该类型不是Byte类型!!" << std::endl;
                    return;
                }
                byteObj->SetValue(svalue.int8Value);
                break;
            }
            case EType::SByte: {
                TemplateObject* to = dynamic_cast<TemplateObject*>(obj);
                if (to != nullptr) {
                    to->SetValue(EType::SByte, svalue.sint8Value);
                }
                AnyObject* anyObject = dynamic_cast<AnyObject*>(obj);
                if (anyObject != nullptr) {
                    anyObject->SetValue(EType::SByte, svalue.sint8Value);
                    return;
                }
                SByteObject* byteObj = dynamic_cast<SByteObject*>(obj);
                if (byteObj == nullptr) {
                    std::cout << "该类型不是SByte类型!!" << std::endl;
                    return;
                }
                byteObj->SetValue(svalue.sint8Value);
                break;
            }
            case EType::Int16: {
                TemplateObject* to = dynamic_cast<TemplateObject*>(obj);
                if (to != nullptr) {
                    to->SetValue(EType::Int16, svalue.int16Value);
                }
                AnyObject* anyObject = dynamic_cast<AnyObject*>(obj);
                if (anyObject != nullptr) {
                    anyObject->SetValue(EType::Int16, svalue.int16Value);
                    return;
                }
                Int16Object* int16Obj = dynamic_cast<Int16Object*>(obj);
                if (int16Obj == nullptr) {
                    std::cout << "该类型不是Int32类型!!" << std::endl;
                    return;
                }
                int16Obj->SetValue(svalue.int16Value);
                break;
            }
            case EType::UInt16: {
                TemplateObject* to = dynamic_cast<TemplateObject*>(obj);
                if (to != nullptr) {
                    to->SetValue(EType::UInt16, svalue.uint16Value);
                }
                AnyObject* anyObject = dynamic_cast<AnyObject*>(obj);
                if (anyObject != nullptr) {
                    anyObject->SetValue(EType::UInt16, svalue.uint16Value);
                    return;
                }
                UInt16Object* uint16Obj = dynamic_cast<UInt16Object*>(obj);
                if (uint16Obj == nullptr) {
                    std::cout << "该类型不是Int32类型!!" << std::endl;
                    return;
                }
                uint16Obj->SetValue(svalue.uint16Value);
                break;
            }
            case EType::Int32: {
                TemplateObject* to = dynamic_cast<TemplateObject*>(obj);
                if (to != nullptr) {
                    to->SetValue(EType::Int32, svalue.int32Value);
                    return;
                }
                AnyObject* anyObject = dynamic_cast<AnyObject*>(obj);
                if (anyObject != nullptr) {
                    anyObject->SetValue(EType::Int32, svalue.int32Value);
                    return;
                }
                Int32Object* int32Obj = dynamic_cast<Int32Object*>(obj);
                if (int32Obj == nullptr) {
                    std::cout << "该类型不是Int32类型!!" << std::endl;
                    return;
                }
                int32Obj->SetValue(svalue.int32Value);
                break;
            }
            case EType::UInt32: {
                TemplateObject* to = dynamic_cast<TemplateObject*>(obj);
                if (to != nullptr) {
                    to->SetValue(EType::UInt32, svalue.uint32Value);
                }
                AnyObject* anyObject = dynamic_cast<AnyObject*>(obj);
                if (anyObject != nullptr) {
                    anyObject->SetValue(EType::UInt32, svalue.uint32Value);
                    return;
                }
                UInt32Object* uint32Obj = dynamic_cast<UInt32Object*>(obj);
                if (uint32Obj == nullptr) {
                    std::cout << "该类型不是Int32类型!!" << std::endl;
                    return;
                }
                uint32Obj->SetValue(svalue.uint32Value);
                break;
            }
            case EType::Int64: {
                TemplateObject* to = dynamic_cast<TemplateObject*>(obj);
                if (to != nullptr) {
                    to->SetValue(EType::Int64, svalue.int64Value);
                }
                AnyObject* anyObject = dynamic_cast<AnyObject*>(obj);
                if (anyObject != nullptr) {
                    anyObject->SetValue(EType::Int64, svalue.int64Value);
                    return;
                }
                Int64Object* int64Obj = dynamic_cast<Int64Object*>(obj);
                if (int64Obj == nullptr) {
                    std::cout << "该类型不是Int32类型!!" << std::endl;
                    return;
                }
                int64Obj->SetValue(svalue.int64Value);
                break;
            }
            case EType::UInt64: {
                TemplateObject* to = dynamic_cast<TemplateObject*>(obj);
                if (to != nullptr) {
                    to->SetValue(EType::UInt64, svalue.uint64Value);
                }
                AnyObject* anyObject = dynamic_cast<AnyObject*>(obj);
                if (anyObject != nullptr) {
                    anyObject->SetValue(EType::UInt64, svalue.uint64Value);
                    return;
                }
                UInt64Object* uint64Obj = dynamic_cast<UInt64Object*>(obj);
                if (uint64Obj == nullptr) {
                    std::cout << "该类型不是Int32类型!!" << std::endl;
                    return;
                }
                uint64Obj->SetValue(svalue.uint64Value);
                break;
            }
            case EType::String: {
                TemplateObject* to = dynamic_cast<TemplateObject*>(obj);
                if (to != nullptr) {
                    to->SetValue(EType::String, svalue.stringValue);
                    return;
                }
                AnyObject* anyObject = dynamic_cast<AnyObject*>(obj);
                if (anyObject != nullptr) {
                    anyObject->SetValue(EType::String, svalue.stringValue);
                    return;
                }
                ClassObject* classobj = dynamic_cast<ClassObject*>(obj);
                if (classobj != nullptr) {
                    // Handle class object
                }
                StringObject* stringObj = dynamic_cast<StringObject*>(obj);
                if (stringObj == nullptr) {
                    std::cout << "该类型不是Int32类型!!" << std::endl;
                    return;
                }
                stringObj->SetValue(svalue.stringValue);
                break;
            }
            case EType::Float32: {
                TemplateObject* to = dynamic_cast<TemplateObject*>(obj);
                if (to != nullptr) {
                    to->SetValue(EType::Float32, svalue.floatValue);
                }
                AnyObject* anyObject = dynamic_cast<AnyObject*>(obj);
                if (anyObject != nullptr) {
                    anyObject->SetValue(EType::Float32, svalue.floatValue);
                    return;
                }
                FloatObject* floatObj = dynamic_cast<FloatObject*>(obj);
                if (floatObj == nullptr) {
                    std::cout << "该类型不是Int32类型!!" << std::endl;
                    return;
                }
                floatObj->SetValue(svalue.floatValue);
                break;
            }
            case EType::Float64: {
                TemplateObject* to = dynamic_cast<TemplateObject*>(obj);
                if (to != nullptr) {
                    to->SetValue(EType::Float64, svalue.doubleValue);
                }
                AnyObject* anyObject = dynamic_cast<AnyObject*>(obj);
                if (anyObject != nullptr) {
                    anyObject->SetValue(EType::Float64, svalue.doubleValue);
                    return;
                }
                DoubleObject* doubleObj = dynamic_cast<DoubleObject*>(obj);
                if (doubleObj == nullptr) {
                    std::cout << "该类型不是Int32类型!!" << std::endl;
                    return;
                }
                doubleObj->SetValue(svalue.doubleValue);
                break;
            }
            case EType::Class: {
                TemplateObject* to = dynamic_cast<TemplateObject*>(obj);
                if (to != nullptr) {
                    to->SetClassObject(svalue.sobject);
                    return;
                }
                AnyObject* anyObject = dynamic_cast<AnyObject*>(obj);
                if (anyObject != nullptr) {
                    if (ClassObject* co = dynamic_cast<ClassObject*>(svalue.sobject)) {
                        anyObject->SetValue(EType::Class, co->value());
                    }
                    return;
                }
                Int32Object* int32Obj = dynamic_cast<Int32Object*>(obj);
                if (int32Obj != nullptr) {
                    int32Obj->SetValue(svalue.int32Value);
                    return;
                }
                BoolObject* boolObject = dynamic_cast<BoolObject*>(obj);
                if (boolObject != nullptr) {
                    boolObject->SetValue(svalue.int8Value == 1 ? true : false);
                    return;
                }
                ClassObject* classObj = dynamic_cast<ClassObject*>(obj);
                if (classObj == nullptr) {
                    std::cout << "该类型不是Class类型!!" << std::endl;
                    return;
                }
                classObj->SetValue(svalue.sobject);
                break;
            }
        }
    }

    void ObjectManager::SetValueByValue(SValue& target, const SValue& svalue) {
        switch (svalue.eType) {
            case EType::Null: {
                target.SetNullValue();
                break;
            }
            case EType::Byte: {
                target.SetInt8Value(svalue.int8Value);
                break;
            }
            case EType::SByte: {
                target.SetSInt8Value(svalue.sint8Value);
                break;
            }
            case EType::Int16: {
                target.SetInt16Value(svalue.int16Value);
                break;
            }
            case EType::UInt16: {
                target.SetUInt16Value(svalue.uint16Value);
                break;
            }
            case EType::Int32: {
                target.SetInt32Value(svalue.int32Value);
                break;
            }
            case EType::UInt32: {
                target.SetUInt32Value(svalue.uint32Value);
                break;
            }
            case EType::Int64: {
                target.SetInt64Value(svalue.int64Value);
                break;
            }
            case EType::UInt64: {
                target.SetUInt64Value(svalue.uint64Value);
                break;
            }
            case EType::String: {
                target.SetStringValue(svalue.stringValue);
                break;
            }
            case EType::Class: {
                target.SetSObject(svalue.sobject);
                break;
            }
        }
    }

} // namespace VM
} // namespace SimpleLanguage
