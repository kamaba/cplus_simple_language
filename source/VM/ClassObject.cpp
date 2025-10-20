#include "ClassObject.h"
#include "ObjectManager.h" // Assuming ObjectManager.h exists
#include "RuntimeTypeManager.h" // Assuming RuntimeTypeManager.h exists
#include "../Debug/Log.h" // Assuming Log.h exists
#include <iostream>

namespace SimpleLanguage {
namespace VM {

    ClassObject::ClassObject(RuntimeType* irmt, bool isStatic) {
        m_RuntimeType = irmt;

        int byteCount = m_RuntimeType->irClass->byteCount;
        m_Data.resize(byteCount);
        typeId = static_cast<short>(m_RuntimeType->irClass->id);
        m_IRTemplateList = irmt->runtimeTemplateList;

        m_IRMetaVariableList = isStatic ? m_RuntimeType->irClass->staticIRMetaVariableList : m_RuntimeType->irClass->localIRMetaVariableList;
        m_MemberObjectArray.resize(m_IRMetaVariableList.size());
        m_Type.resize(m_IRMetaVariableList.size());
        m_Object = this;
    }

    ClassObject::~ClassObject() {
        // Clean up member objects
        for (SObject* obj : m_MemberObjectArray) {
            delete obj;
        }
        m_MemberObjectArray.clear();
    }

    IRMetaClass* ClassObject::IrMetaClass() const {
        return m_RuntimeType ? m_RuntimeType->irClass : nullptr;
    }

    RuntimeType* ClassObject::GetClassRuntimeType(IRMetaType* irmt, bool isAdd) {
        if (irmt->templateIndex != -1) {
            return m_IRTemplateList[irmt->templateIndex];
        } else {
            std::vector<RuntimeType*> rtList;
            if (irmt->irMetaTypeList.size() > 0) {
                for (size_t i = 0; i < irmt->irMetaTypeList.size(); i++) {
                    RuntimeType* crt = GetClassRuntimeType(irmt->irMetaTypeList[i], isAdd);
                    rtList.push_back(crt);
                }
            }
            RuntimeType* rt = RuntimeTypeManager::GetRuntimeTypeByMTAndTemplateMT(irmt->irMetaClass, rtList);
            if (rt == nullptr && isAdd) {
                rt = RuntimeTypeManager::AddRuntimeTypeByClassAndTemplate(irmt->irMetaClass, rtList);
            }
            return rt;
        }
    }

    void ClassObject::CreateObject() {
        for (size_t i = 0; i < m_IRMetaVariableList.size(); i++) {
            IRMetaType* irmv = m_IRMetaVariableList[i]->irMetaType;
            RuntimeType* rt = GetClassRuntimeType(irmv, true);
            SObject* sobj = ObjectManager::CreateObjectByRuntimeType(rt);
            if (sobj == nullptr) {
                continue;
            }
            m_Type[i] = sobj->typeId;
            m_MemberObjectArray[i] = sobj;
        }
    }

    SObject* ClassObject::GetMemberVariable(int index) {
        if (index > static_cast<int>(m_MemberObjectArray.size())) {
            Log::AddVM(EError::None, "执行的参数超出范围!!");
            return nullptr;
        }
        return m_MemberObjectArray[index];
    }

    void ClassObject::SetValue(ClassObject* val) {
        m_Object = val->m_Object;
        val->refCount++;
    }

    void ClassObject::GetMemberVariableSValue(int index, SValue& svalue) {
        if (index < 0) {
            Log::AddVM(EError::None, "执行的参数超出范围!! < 0 ");
            return;
        }
        if (index > static_cast<int>(m_MemberObjectArray.size())) {
            Log::AddVM(EError::None, "执行的参数超出范围!!");
            return;
        }
        SObject* mmv = m_MemberObjectArray[index];
        
        // Type checking and value extraction
        if (ByteObject* byteob = dynamic_cast<ByteObject*>(mmv)) {
            svalue.SetInt8Value(byteob->value);
        } else if (SByteObject* sbyteobj = dynamic_cast<SByteObject*>(mmv)) {
            svalue.SetSInt8Value(sbyteobj->value);
        } else if (Int16Object* int16Obj = dynamic_cast<Int16Object*>(mmv)) {
            svalue.SetInt16Value(int16Obj->value);
        } else if (UInt16Object* uint16Obj = dynamic_cast<UInt16Object*>(mmv)) {
            svalue.SetUInt16Value(uint16Obj->value);
        } else if (Int32Object* int32Obj = dynamic_cast<Int32Object*>(mmv)) {
            svalue.SetInt32Value(int32Obj->value);
        } else if (UInt32Object* uint32Obj = dynamic_cast<UInt32Object*>(mmv)) {
            svalue.SetUInt32Value(uint32Obj->value);
        } else if (Int64Object* int64Obj = dynamic_cast<Int64Object*>(mmv)) {
            svalue.SetInt64Value(int64Obj->value);
        } else if (UInt64Object* uint64Obj = dynamic_cast<UInt64Object*>(mmv)) {
            svalue.SetUInt64Value(uint64Obj->value);
        } else if (FloatObject* floatobj = dynamic_cast<FloatObject*>(mmv)) {
            svalue.SetFloatValue(floatobj->value);
        } else if (DoubleObject* doubleobj = dynamic_cast<DoubleObject*>(mmv)) {
            svalue.SetDoubleValue(doubleobj->value);
        } else if (StringObject* stringObj = dynamic_cast<StringObject*>(mmv)) {
            svalue.SetStringValue(stringObj->value);
        } else if (ClassObject* classObj = dynamic_cast<ClassObject*>(mmv)) {
            svalue.SetSObject(classObj);
        } else if (TemplateObject* templateObj = dynamic_cast<TemplateObject*>(mmv)) {
            // Handle template object
        }
    }

    void ClassObject::SetMemberVariableSValue(int index, const SValue& svalue) {
        if (index > static_cast<int>(m_MemberObjectArray.size())) {
            Log::AddVM(EError::None, "执行的参数超出范围!!");
            return;
        }
        
        switch (svalue.eType) {
            case EType::Null: {
                m_MemberObjectArray[index]->SetNull();
                break;
            }
            case EType::Boolean: {
                // Handle boolean case
                break;
            }
            case EType::Byte: {
                ByteObject* byteObj = dynamic_cast<ByteObject*>(m_MemberObjectArray[index]);
                if (byteObj == nullptr) {
                    Log::AddVM(EError::None, "Byte 该类型不是Int32类型!!");
                    return;
                }
                byteObj->SetValue(svalue.int8Value);
                break;
            }
            case EType::SByte: {
                SByteObject* sbyteObj = dynamic_cast<SByteObject*>(m_MemberObjectArray[index]);
                if (sbyteObj == nullptr) {
                    Log::AddVM(EError::None, "Sbyte 该类型不是Int32类型!!");
                    return;
                }
                sbyteObj->SetValue(svalue.sint8Value);
                break;
            }
            case EType::Int16: {
                Int16Object* int16Obj = dynamic_cast<Int16Object*>(m_MemberObjectArray[index]);
                if (int16Obj == nullptr) {
                    Log::AddVM(EError::None, "Int16 该类型不是Int32类型!!");
                    return;
                }
                int16Obj->SetValue(svalue.int16Value);
                break;
            }
            case EType::UInt16: {
                UInt16Object* uint16Obj = dynamic_cast<UInt16Object*>(m_MemberObjectArray[index]);
                if (uint16Obj == nullptr) {
                    Log::AddVM(EError::None, "UInt16 该类型不是Int16类型!!");
                    return;
                }
                uint16Obj->SetValue(svalue.uint16Value);
                break;
            }
            case EType::Int32: {
                Int32Object* int32Obj = nullptr;
                if (m_MemberObjectArray[index] == nullptr) {
                    int32Obj = new Int32Object(0);
                    m_MemberObjectArray[index] = int32Obj;
                }
                int32Obj = dynamic_cast<Int32Object*>(m_MemberObjectArray[index]);
                if (int32Obj == nullptr) {
                    Log::AddVM(EError::None, "Int32 该类型不是Int32类型!!");
                    return;
                }
                int32Obj->SetValue(svalue.int32Value);
                break;
            }
            case EType::UInt32: {
                UInt32Object* uint32Obj = dynamic_cast<UInt32Object*>(m_MemberObjectArray[index]);
                if (uint32Obj == nullptr) {
                    Log::AddVM(EError::None, "UInt32 该类型不是UInt32类型!!");
                    return;
                }
                uint32Obj->SetValue(svalue.uint32Value);
                break;
            }
            case EType::Int64: {
                Int64Object* int64Obj = dynamic_cast<Int64Object*>(m_MemberObjectArray[index]);
                if (int64Obj == nullptr) {
                    Log::AddVM(EError::None, "Int64 该类型不是Int32类型!!");
                    return;
                }
                int64Obj->SetValue(svalue.int64Value);
                break;
            }
            case EType::UInt64: {
                UInt64Object* uint64Obj = dynamic_cast<UInt64Object*>(m_MemberObjectArray[index]);
                if (uint64Obj == nullptr) {
                    Log::AddVM(EError::None, "UInt64 该类型不是Int64类型!!");
                    return;
                }
                uint64Obj->SetValue(svalue.uint64Value);
                break;
            }
            case EType::Float32: {
                FloatObject* floatObj = dynamic_cast<FloatObject*>(m_MemberObjectArray[index]);
                if (floatObj == nullptr) {
                    Log::AddVM(EError::None, "Float 该类型不是float类型!!");
                    return;
                }
                floatObj->SetValue(svalue.floatValue);
                break;
            }
            case EType::Float64: {
                DoubleObject* doubleObj = dynamic_cast<DoubleObject*>(m_MemberObjectArray[index]);
                if (doubleObj == nullptr) {
                    Log::AddVM(EError::None, "Double 该类型不是Double类型!!");
                    return;
                }
                doubleObj->SetValue(svalue.doubleValue);
                break;
            }
            case EType::String: {
                StringObject* stringObj = dynamic_cast<StringObject*>(m_MemberObjectArray[index]);
                if (stringObj == nullptr) {
                    Log::AddVM(EError::None, "String 该类型不是Int32类型!!");
                    return;
                }
                stringObj->SetValue(svalue.stringValue);
                break;
            }
            case EType::Class: {
                SObject* mva = m_MemberObjectArray[index];
                if (mva->eType() == EType::Byte) {
                    ByteObject* byteObj = dynamic_cast<ByteObject*>(mva);
                    if (byteObj == nullptr) {
                        Log::AddVM(EError::None, "Class ByteObject 该类型不是Int32类型!!");
                        return;
                    }
                    byteObj->SetValue(svalue.int8Value);
                } else if (mva->eType() == EType::SByte) {
                    SByteObject* sbyteObj = dynamic_cast<SByteObject*>(mva);
                    if (sbyteObj == nullptr) {
                        Log::AddVM(EError::None, "Class SByteObject 该类型不是Int32类型!!");
                        return;
                    }
                    sbyteObj->SetValue(svalue.sint8Value);
                } else if (mva->eType() == EType::Int16) {
                    Int16Object* int16Obj = dynamic_cast<Int16Object*>(mva);
                    if (int16Obj == nullptr) {
                        Log::AddVM(EError::None, "Class Int16Object 该类型不是Int16类型!!");
                        return;
                    }
                    int16Obj->SetValue(svalue.int16Value);
                } else if (mva->eType() == EType::UInt16) {
                    UInt32Object* uint32Obj = dynamic_cast<UInt32Object*>(mva);
                    if (uint32Obj == nullptr) {
                        Log::AddVM(EError::None, "Class UInt32Object 该类型不是UInt32类型!!");
                        return;
                    }
                    uint32Obj->SetValue(svalue.uint32Value);
                } else if (mva->eType() == EType::Int32) {
                    Int32Object* int32Obj = dynamic_cast<Int32Object*>(mva);
                    if (int32Obj == nullptr) {
                        Log::AddVM(EError::None, "Class Int32Object 该类型不是Int32类型!!");
                        return;
                    }
                    int32Obj->SetValue(svalue.int32Value);
                } else if (mva->eType() == EType::UInt32) {
                    UInt32Object* uint32Obj = dynamic_cast<UInt32Object*>(mva);
                    if (uint32Obj == nullptr) {
                        Log::AddVM(EError::None, "Class UInt32Object 该类型不是Int32类型!!");
                        return;
                    }
                    uint32Obj->SetValue(svalue.uint32Value);
                } else if (mva->eType() == EType::Int64) {
                    Int64Object* int64Obj = dynamic_cast<Int64Object*>(mva);
                    if (int64Obj == nullptr) {
                        Log::AddVM(EError::None, "该类型不是Int64类型!!");
                        return;
                    }
                    int64Obj->SetValue(svalue.int64Value);
                } else if (mva->eType() == EType::UInt64) {
                    UInt64Object* uint64Obj = dynamic_cast<UInt64Object*>(mva);
                    if (uint64Obj == nullptr) {
                        Log::AddVM(EError::None, "该类型不是Int64类型!!");
                        return;
                    }
                    uint64Obj->SetValue(svalue.uint64Value);
                } else if (mva->eType() == EType::String) {
                    StringObject* stringObj = dynamic_cast<StringObject*>(mva);
                    if (stringObj == nullptr) {
                        Log::AddVM(EError::None, "该类型不是stringObj类型!!");
                        return;
                    }
                    stringObj->SetValue(svalue.stringValue);
                } else {
                    ClassObject* classObj = dynamic_cast<ClassObject*>(m_MemberObjectArray[index]);
                    if (classObj == nullptr) {
                        Log::AddVM(EError::None, "该类型不是classObj类型!!");
                        return;
                    }
                    m_MemberObjectArray[index] = dynamic_cast<ClassObject*>(svalue.sobject);
                }
                break;
            }
        }
    }

    std::string ClassObject::ToFormatString() {
        std::stringstream sb;

        if (m_Object != nullptr) {
            sb << m_Object->ToFormatString();
        }
        sb << m_RuntimeType->irClass->ToString();
        
        return sb.str();
    }

    std::string ClassObject::ToString() {
        std::stringstream sb;
        sb << m_RuntimeType->ToString();
        return sb.str();
    }

} // namespace VM
} // namespace SimpleLanguage
