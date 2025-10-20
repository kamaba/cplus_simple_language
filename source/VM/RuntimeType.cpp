#include "RuntimeType.h"
#include "SValue.h"
#include "SObject.h"
#include "ObjectManager.h"
#include "Log.h"
#include "../IR/IRBase.h"
#include "../IR/IRExpress.h"
#include <iostream>

namespace SimpleLanguage {
namespace VM {

// RuntimeType implementation
RuntimeType::RuntimeType(IRMetaClass* rc, std::vector<RuntimeType*>* rtList) {
    irClass = rc;
    if (rtList != nullptr) {
        runtimeTemplateList = *rtList;
    }

    m_StaticMemObjectList = new SObject*[irClass->staticIRMetaVariableList.size()];
    for (size_t i = 0; i < irClass->staticIRMetaVariableList.size(); i++) {
        RuntimeType* rt = GetClassRuntimeType(irClass->staticIRMetaVariableList[i]->irMetaType, true);
        
        m_StaticMemObjectList[i] = ObjectManager::CreateObjectByRuntimeType(rt, true);
    }
}

RuntimeType::~RuntimeType() {
    if (m_StaticMemObjectList != nullptr) {
        for (size_t i = 0; i < irClass->staticIRMetaVariableList.size(); i++) {
            delete m_StaticMemObjectList[i];
        }
        delete[] m_StaticMemObjectList;
    }
}

RuntimeType* RuntimeType::GetClassRuntimeType(IRMetaType* irmt, bool isAdd) {
    if (irmt->templateIndex != -1) {
        return runtimeTemplateList[irmt->templateIndex];
    } else {
        std::vector<RuntimeType*> rtList;
        if (irmt->irMetaTypeList.size() > 0) {
            for (size_t i = 0; i < irmt->irMetaTypeList.size(); i++) {
                RuntimeType* crt = GetClassRuntimeType(irmt->irMetaTypeList[i], isAdd);
                rtList.push_back(crt);
            }
        }
        RuntimeType* rt = RuntimeTypeManager::GetRuntimeTypeByMTAndTemplateMT(irmt->irMetaClass, &rtList);
        if (rt == nullptr && isAdd) {
            rt = RuntimeTypeManager::AddRuntimeTypeByClassAndTemplate(irmt->irMetaClass, &rtList);
        }
        return rt;
    }
}

void RuntimeType::GetMemberVariableSValue(int index, SValue& svalue) {
    if (index < 0) {
        Log::AddVM(EError::None, "执行的参数超出范围!! < 0 ");
        return;
    }
    if (index >= static_cast<int>(irClass->staticIRMetaVariableList.size())) {
        Log::AddVM(EError::None, "执行的参数超出范围!!");
        return;
    }
    SObject* mmv = m_StaticMemObjectList[index];
    
    // Use dynamic_cast for type checking
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

void RuntimeType::SetMemberVariableSValue(int index, const SValue& svalue) {
    if (index >= static_cast<int>(irClass->staticIRMetaVariableList.size())) {
        Log::AddVM(EError::None, "执行的参数超出范围!!");
        return;
    }
    
    switch (svalue.eType) {
        case EType::Null: {
            ClassObject* classObj = dynamic_cast<ClassObject*>(m_StaticMemObjectList[index]);
            if (classObj == nullptr) {
                Log::AddVM(EError::None, "Null 该类型不是Int32类型!!");
                return;
            }
            classObj->SetNull();
            break;
        }
        case EType::Boolean: {
            // Handle boolean
            break;
        }
        case EType::Byte: {
            ByteObject* byteObj = dynamic_cast<ByteObject*>(m_StaticMemObjectList[index]);
            if (byteObj == nullptr) {
                Log::AddVM(EError::None, "Byte 该类型不是Int32类型!!");
                return;
            }
            byteObj->SetValue(svalue.int8Value);
            break;
        }
        case EType::SByte: {
            SByteObject* sbyteObj = dynamic_cast<SByteObject*>(m_StaticMemObjectList[index]);
            if (sbyteObj == nullptr) {
                Log::AddVM(EError::None, "Sbyte 该类型不是Int32类型!!");
                return;
            }
            sbyteObj->SetValue(svalue.sint8Value);
            break;
        }
        case EType::Int16: {
            Int16Object* int32Obj = dynamic_cast<Int16Object*>(m_StaticMemObjectList[index]);
            if (int32Obj == nullptr) {
                Log::AddVM(EError::None, "Int16 该类型不是Int32类型!!");
                return;
            }
            int32Obj->SetValue(svalue.int16Value);
            break;
        }
        case EType::UInt16: {
            UInt16Object* uint16Obj = dynamic_cast<UInt16Object*>(m_StaticMemObjectList[index]);
            if (uint16Obj == nullptr) {
                Log::AddVM(EError::None, "UInt16 该类型不是Int16类型!!");
                return;
            }
            uint16Obj->SetValue(svalue.uint16Value);
            break;
        }
        case EType::Int32: {
            Int32Object* int32Obj = dynamic_cast<Int32Object*>(m_StaticMemObjectList[index]);
            if (int32Obj == nullptr) {
                Log::AddVM(EError::None, "Int32 该类型不是Int32类型!!");
                return;
            }
            int32Obj->SetValue(svalue.int32Value);
            break;
        }
        case EType::UInt32: {
            UInt32Object* uint32Obj = dynamic_cast<UInt32Object*>(m_StaticMemObjectList[index]);
            if (uint32Obj == nullptr) {
                Log::AddVM(EError::None, "UInt32 该类型不是UInt32类型!!");
                return;
            }
            uint32Obj->SetValue(svalue.uint32Value);
            break;
        }
        case EType::Int64: {
            Int64Object* int64Obj = dynamic_cast<Int64Object*>(m_StaticMemObjectList[index]);
            if (int64Obj == nullptr) {
                Log::AddVM(EError::None, "Int64 该类型不是Int32类型!!");
                return;
            }
            int64Obj->SetValue(svalue.int64Value);
            break;
        }
        case EType::UInt64: {
            UInt64Object* uint64Obj = dynamic_cast<UInt64Object*>(m_StaticMemObjectList[index]);
            if (uint64Obj == nullptr) {
                Log::AddVM(EError::None, "UInt64 该类型不是Int64类型!!");
                return;
            }
            uint64Obj->SetValue(svalue.uint64Value);
            break;
        }
        case EType::Float32: {
            FloatObject* floatObj = dynamic_cast<FloatObject*>(m_StaticMemObjectList[index]);
            if (floatObj == nullptr) {
                Log::AddVM(EError::None, "Float 该类型不是float类型!!");
                return;
            }
            floatObj->SetValue(svalue.floatValue);
            break;
        }
        case EType::Float64: {
            DoubleObject* doubleObj = dynamic_cast<DoubleObject*>(m_StaticMemObjectList[index]);
            if (doubleObj == nullptr) {
                Log::AddVM(EError::None, "Double 该类型不是Double类型!!");
                return;
            }
            doubleObj->SetValue(svalue.doubleValue);
            break;
        }
        case EType::String: {
            StringObject* stringObj = dynamic_cast<StringObject*>(m_StaticMemObjectList[index]);
            if (stringObj == nullptr) {
                Log::AddVM(EError::None, "String 该类型不是Int32类型!!");
                return;
            }
            stringObj->SetValue(svalue.stringValue);
            break;
        }
        case EType::Class: {
            SObject* mva = m_StaticMemObjectList[index];
            if (mva->eType == EType::Byte) {
                ByteObject* byteObj = dynamic_cast<ByteObject*>(mva);
                if (byteObj == nullptr) {
                    Log::AddVM(EError::None, "Class ByteObject 该类型不是Int32类型!!");
                    return;
                }
                byteObj->SetValue(svalue.int8Value);
            } else if (mva->eType == EType::SByte) {
                SByteObject* sbyteObj = dynamic_cast<SByteObject*>(mva);
                if (sbyteObj == nullptr) {
                    Log::AddVM(EError::None, "Class SByteObject 该类型不是Int32类型!!");
                    return;
                }
                sbyteObj->SetValue(svalue.sint8Value);
            } else if (mva->eType == EType::Int16) {
                Int16Object* int16Obj = dynamic_cast<Int16Object*>(mva);
                if (int16Obj == nullptr) {
                    Log::AddVM(EError::None, "Class Int16Object 该类型不是Int16类型!!");
                    return;
                }
                int16Obj->SetValue(svalue.int16Value);
            } else if (mva->eType == EType::UInt16) {
                UInt32Object* uint32Obj = dynamic_cast<UInt32Object*>(mva);
                if (uint32Obj == nullptr) {
                    Log::AddVM(EError::None, "Class UInt32Object 该类型不是UInt32类型!!");
                    return;
                }
                uint32Obj->SetValue(svalue.uint32Value);
            } else if (mva->eType == EType::Int32) {
                Int32Object* int32Obj = dynamic_cast<Int32Object*>(mva);
                if (int32Obj == nullptr) {
                    Log::AddVM(EError::None, "Class Int32Object 该类型不是Int32类型!!");
                    return;
                }
                int32Obj->SetValue(svalue.int32Value);
            } else if (mva->eType == EType::UInt32) {
                UInt32Object* uint32Obj = dynamic_cast<UInt32Object*>(mva);
                if (uint32Obj == nullptr) {
                    Log::AddVM(EError::None, "Class UInt32Object 该类型不是Int32类型!!");
                    return;
                }
                uint32Obj->SetValue(svalue.uint32Value);
            } else if (mva->eType == EType::Int64) {
                Int64Object* int64Obj = dynamic_cast<Int64Object*>(mva);
                if (int64Obj == nullptr) {
                    Log::AddVM(EError::None, "该类型不是Int64类型!!");
                    return;
                }
                int64Obj->SetValue(svalue.int64Value);
            } else if (mva->eType == EType::UInt64) {
                UInt64Object* uint64Obj = dynamic_cast<UInt64Object*>(mva);
                if (uint64Obj == nullptr) {
                    Log::AddVM(EError::None, "该类型不是Int64类型!!");
                    return;
                }
                uint64Obj->SetValue(svalue.uint64Value);
            } else if (mva->eType == EType::String) {
                StringObject* stringObj = dynamic_cast<StringObject*>(mva);
                if (stringObj == nullptr) {
                    Log::AddVM(EError::None, "该类型不是stringObj类型!!");
                    return;
                }
                stringObj->SetValue(svalue.stringValue);
            } else {
                ClassObject* classObj = dynamic_cast<ClassObject*>(m_StaticMemObjectList[index]);
                if (classObj == nullptr) {
                    Log::AddVM(EError::None, "该类型不是classObj类型!!");
                    return;
                }
                m_StaticMemObjectList[index] = dynamic_cast<ClassObject*>(svalue.sobject);
            }
            break;
        }
    }
}

std::vector<IRData*> RuntimeType::CreateStaticMetaMetaVariableIRList() {
    std::vector<IRData*> list;

    for (auto v : irClass->localIRMetaVariableList) {
        IRExpress* irexp = new IRExpress(IRManager::instance, v->express);

        v->SetIRDataList(irexp->IRDataList);

        list.insert(list.end(), irexp->IRDataList.begin(), irexp->IRDataList.end());
    }

    return list;
}

bool RuntimeType::SameRuntimeType(RuntimeType* rt1, RuntimeType* rt2) {
    if (rt1->irClass != rt2->irClass) {
        return false;
    }
    if (rt1->runtimeTemplateList.size() != rt2->runtimeTemplateList.size()) {
        return false;
    }
    for (size_t i = 0; i < rt1->runtimeTemplateList.size(); i++) {
        if (!SameRuntimeType(rt1->runtimeTemplateList[i], rt2->runtimeTemplateList[i])) {
            return false;
        }
    }
    return true;
}

std::string RuntimeType::ToString() {
    std::stringstream sb;

    sb << irClass->irName;

    if (runtimeTemplateList.size() > 0) {
        sb << '<';
        for (size_t i = 0; i < runtimeTemplateList.size(); i++) {
            sb << runtimeTemplateList[i]->ToString();
            if (i < runtimeTemplateList.size() - 1)
                sb << ",";
        }
        sb << ">";
    }

    return sb.str();
}

// RuntimeTypeManager implementation
std::vector<RuntimeType*> RuntimeTypeManager::s_RuntimeList;

std::vector<RuntimeType*>& RuntimeTypeManager::GetRuntimeList() {
    return s_RuntimeList;
}

RuntimeType* RuntimeTypeManager::GetRuntimeTypeByMTAndTemplateMT(IRMetaClass* rmc, std::vector<RuntimeType*>* inputTemplateTypeList) {
    for (auto v : s_RuntimeList) {
        if (v->irClass != rmc) {
            continue;
        }

        if (v->runtimeTemplateList.size() == inputTemplateTypeList->size()) {
            if (v->runtimeTemplateList.size() == 0) {
                return v;
            }
            bool flag = true;
            for (size_t i = 0; i < inputTemplateTypeList->size(); i++) {
                if (!RuntimeType::SameRuntimeType((*inputTemplateTypeList)[i], v->runtimeTemplateList[i])) {
                    flag = false;
                    break;
                }
            }
            if (flag)
                return v;
        }
    }
    return nullptr;
}

RuntimeType* RuntimeTypeManager::GetRuntimeTypeByMTAndIRMetaClass(IRMetaClass* rmc) {
    for (auto v : s_RuntimeList) {
        if (v->runtimeTemplateList.size() != 0) {
            continue;
        }
        if (v->irClass == rmc) {
            return v;
        }
    }
    return nullptr;
}

RuntimeType* RuntimeTypeManager::AddRuntimeTypeByClassAndTemplate(IRMetaClass* rmc, std::vector<RuntimeType*>* inputTemplateTypeList) {
    RuntimeType* rt = new RuntimeType(rmc, inputTemplateTypeList);

    s_RuntimeList.push_back(rt);

    return rt;
}

RuntimeType* RuntimeTypeManager::AddRuntimeTypeByClass(IRMetaClass* rmc) {
    RuntimeType* rt = new RuntimeType(rmc, nullptr);

    s_RuntimeList.push_back(rt);

    return rt;
}

} // namespace VM
} // namespace SimpleLanguage
