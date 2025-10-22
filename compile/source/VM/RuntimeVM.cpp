#include "RuntimeVM.h"
#include "ObjectManager.h"
#include "RuntimeTypeManager.h" // Assuming RuntimeTypeManager.h exists
#include "InnerCLRRuntimeVM.h" // Assuming InnerCLRRuntimeVM.h exists
#include "../Debug/Log.h" // Assuming Log.h exists
#include <iostream>
#include <algorithm>

namespace SimpleLanguage {
namespace VM {

    RuntimeVM::RuntimeVM(const std::vector<RuntimeType*>& inputTemplateTypeList, IRMethod* mmf) {
        if (!inputTemplateTypeList.empty()) {
            m_InputTemplateRuntimeTypeList = inputTemplateTypeList;
        }
        m_IRMethod = mmf;
        m_IRDataList = mmf->GetIRDataList();
        m_ExecuteCount = static_cast<uint16_t>(m_IRDataList.size());

        id = mmf->id;

        Init();
    }

    RuntimeVM::RuntimeVM(const std::vector<IRData*>& irList) {
        m_IRDataList = irList;
        m_ExecuteCount = static_cast<uint16_t>(m_IRDataList.size());
        Init();
    }

    RuntimeVM::~RuntimeVM() {
        // Clean up allocated objects
        for (SObject* obj : m_LocalVariableObjectArray) {
            delete obj;
        }
        for (SObject* obj : m_ArgumentObjectArray) {
            delete obj;
        }
        for (SObject* obj : m_ReturnObjectArray) {
            delete obj;
        }
    }

    void RuntimeVM::Init() {
        // 参数列表 argument variable table
        if (m_IRMethod != nullptr) {
            m_ReturnObjectArray.resize(m_IRMethod->GetMethodReturnVariableList().size());
            for (size_t i = 0; i < m_IRMethod->GetMethodReturnVariableList().size(); i++) {
                IRMetaType* imt = m_IRMethod->GetMethodReturnVariableList()[i]->irMetaType;
                SObject* sobj = CreateObjectByIRMetaType(imt, true);
                m_ReturnObjectArray[i] = sobj;
            }

            m_ArgumentObjectArray.resize(m_IRMethod->GetMethodArgumentList().size());
            for (size_t i = 0; i < m_IRMethod->GetMethodArgumentList().size(); i++) {
                IRMetaType* imt = m_IRMethod->GetMethodArgumentList()[i]->irMetaType;
                SObject* sobj = CreateObjectByIRMetaType(imt, true);
                m_ArgumentObjectArray[i] = sobj;
            }
            for (size_t i = 0; i < m_ArgumentObjectArray.size(); i++) {
                Log::AddVM(EError::None, "Argu_" + std::to_string(i) + "_Value: [" + m_ArgumentObjectArray[i]->ToString() + "]");
            }

            // 局部变量列表 local variable table
            m_LocalVariableObjectArray.resize(m_IRMethod->GetMethodLocalVariableList().size());
            for (size_t i = 0; i < m_IRMethod->GetMethodLocalVariableList().size(); i++) {
                auto mev = m_IRMethod->GetMethodLocalVariableList()[i];
                IRMetaType* imt = mev->irMetaType;
                SObject* sobj = CreateObjectByIRMetaType(imt, true);
                m_LocalVariableObjectArray[i] = sobj;
            }
            for (size_t i = 0; i < m_LocalVariableObjectArray.size(); i++) {
                Log::AddVM(EError::None, "Variable_" + std::to_string(i) + m_LocalVariableObjectArray[i]->ToString());
            }

            size_t count = m_IRMethod->GetIRDataList().size();
            if (count < 48) {
                m_ValueStack.resize(128);
            } else if (count >= 48 && count < 150) {
                m_ValueStack.resize(160);
            } else if (count >= 150 && count < 300) {
                m_ValueStack.resize(200);
            } else if (count >= 300 && count < 500) {
                m_ValueStack.resize(300);
            } else if (count >= 500 && count < 800) {
                m_ValueStack.resize(400);
            } else {
                m_ValueStack.resize(500);
            }
        } else {
            m_ArgumentObjectArray.clear();
            m_LocalVariableObjectArray.clear();
            m_ValueStack.resize(255);
        }
    }

    SObject* RuntimeVM::CreateObjectByIRMetaType(IRMetaType* irmt, bool isAdd) {
        if (irmt->templateIndex != -1) {
            return new TemplateObject();
        } else {
            RuntimeType* rt = GetClassRuntimeType(irmt, isAdd);
            return ObjectManager::CreateObjectByRuntimeType(rt);
        }
    }

    void RuntimeVM::AddReturnObjectArray(const std::vector<SObject*>& sobjs) {
        for (size_t i = 0; i < sobjs.size(); i++) {
            if (!sobjs[i]->isVoid()) {
                m_ValueStack[m_ValueIndex++].SetSObject(sobjs[i]);
            }
        }
    }

    SObject* RuntimeVM::GetLocalVariableValue(int index) {
        if (index > static_cast<int>(m_LocalVariableObjectArray.size())) {
            Log::AddVM(EError::None, "执行的栈超出范围!!");
            return nullptr;
        }
        if (index < 0) {
            Log::AddVM(EError::None, "执行的栈超出范围!!-");
            return nullptr;
        }

        return m_LocalVariableObjectArray[index];
    }

    SObject* RuntimeVM::GetArgumentValue(int index) {
        if (index > static_cast<int>(m_ArgumentObjectArray.size())) {
            Log::AddVM(EError::None, "SVM Error FunctionName:" + this->id + " 执行的参数超出范围!!");
            return nullptr;
        }
        return m_ArgumentObjectArray[index];
    }

    void RuntimeVM::SetArgumentValue(int index, const SValue& svalue) {
        if (index > static_cast<int>(m_ArgumentObjectArray.size())) {
            Log::AddVM(EError::None, "执行的参数超出范围!!");
            return;
        }
        ObjectManager::SetObjectByValue(m_ArgumentObjectArray[index], const_cast<SValue&>(svalue));
    }

    void RuntimeVM::SetLocalVariableSValue(int index, const SValue& svalue) {
        if (index > static_cast<int>(m_LocalVariableObjectArray.size())) {
            Log::AddVM(EError::None, "执行的栈超出范围!!");
            return;
        }
        ObjectManager::SetObjectByValue(m_LocalVariableObjectArray[index], const_cast<SValue&>(svalue));
    }

    void RuntimeVM::SetReturnVariableSValue(int index, const SValue& svalue) {
        if (index > static_cast<int>(m_ReturnObjectArray.size())) {
            Log::AddVM(EError::None, "执行的栈超出范围!!");
            return;
        }
        ObjectManager::SetObjectByValue(m_ReturnObjectArray[index], const_cast<SValue&>(svalue));
    }

    SValue RuntimeVM::GetCurrentIndexValue(bool isReduce) {
        if (isReduce) {
            m_ValueIndex--;
            return m_ValueStack[m_ValueIndex];
        } else {
            return m_ValueStack[m_ValueIndex];
        }
    }

    RuntimeType* RuntimeVM::GetClassRuntimeType(IRMetaType* irmt, bool isAdd) {
        if (irmt->templateIndex != -1) {
            return m_InputTemplateRuntimeTypeList[irmt->templateIndex];
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

    RuntimeType* RuntimeVM::GetMethodRuntimeType(IRMetaType* irmt) {
        if (irmt->templateIndex != -1) {
            return m_InputTemplateRuntimeTypeList[irmt->templateIndex];
        } else {
            std::vector<RuntimeType*> rtList;
            if (irmt->irMetaTypeList.size() > 0) {
                for (size_t i = 0; i < irmt->irMetaTypeList.size(); i++) {
                    RuntimeType* crt = GetMethodRuntimeType(irmt->irMetaTypeList[i]);
                    rtList.push_back(crt);
                }
            }
            return RuntimeTypeManager::GetRuntimeTypeByMTAndTemplateMT(irmt->irMetaClass, rtList);
        }
    }

    void RuntimeVM::Run() {
        std::string funName = id;

        std::string pushChar = "";
        for (int i = 0; i < level; i++) {
            pushChar = '\t' + pushChar;
        }
        Log::AddVM(EError::None, pushChar + "[VMRuntime] [Push] Method: [" + funName + "]");
        level++;

        InnerCLRRuntimeVM* topClrRuntime = InnerCLRRuntimeVM::topCLRRuntime;
        for (size_t i = 0; i < m_ArgumentObjectArray.size(); i++) {
            SValue sval = topClrRuntime->GetCurrentIndexValue(true);
            SetArgumentValue(static_cast<int>(m_ArgumentObjectArray.size() - i - 1), sval);
        }

        while (true) {
            if (m_ExecuteIndex >= m_ExecuteCount) {
                break;
            }
            RunInstruction(m_IRDataList[m_ExecuteIndex]);
            m_ExecuteIndex++;
        }
        level--;
        pushChar = "";
        for (int i = 0; i < level; i++) {
            pushChar = '\t' + pushChar;
        }
        Log::AddVM(EError::None, pushChar + "[VMRuntime] [Pop] Method: [" + funName + "]");
    }

    void RuntimeVM::SetValue(SValue& sValue, SValue& sStore, IRData* iri) {
        switch (sStore.eType) {
            case EType::Boolean:
            case EType::Byte: 
                sStore.SetInt8Value(sValue.int8Value); 
                break;
            case EType::SByte: 
                sStore.SetSInt8Value(sValue.sint8Value); 
                break;
            case EType::Int16: 
                sStore.SetInt16Value(sValue.int16Value); 
                break;
            case EType::UInt16: 
                sStore.SetUInt16Value(sValue.uint16Value); 
                break;
            case EType::Int32: 
                sStore.SetInt32Value(sValue.int32Value); 
                break;
            case EType::UInt32: 
                sStore.SetUInt32Value(sValue.uint32Value); 
                break;
            case EType::Int64: 
                sStore.SetInt64Value(sValue.int64Value); 
                break;
            case EType::UInt64: 
                sStore.SetUInt64Value(sValue.uint64Value); 
                break;
            case EType::Float32: 
                sStore.SetFloatValue(sValue.floatValue); 
                break;
            case EType::Float64: 
                sStore.SetDoubleValue(sValue.doubleValue); 
                break;
            case EType::String: 
                sStore.SetStringValue(sValue.stringValue); 
                break;
            case EType::Null: {
                sStore.SetNull();
                break;
            }
            case EType::Class: {
                ClassObject* co = dynamic_cast<ClassObject*>(sStore.sobject);
                if (co != nullptr) {
                    co->SetMemberVariableSValue(iri->index, sValue);
                }
                break;
            }
            default: {
                Log::AddVM(EError::None, "Error StoreNotStaticField Path:" + iri->debugInfo.path + " Line: " + std::to_string(iri->debugInfo.beginLine));
                break;
            }
        }
    }

    void RuntimeVM::RunInstruction(IRData* iri) {
        // 栈位的移动的规则，使用当前位为空的概念，只要栈被使用掉，索引则加1，所以索引最少为0
        switch (iri->opCode) {
            case EIROpCode::Nop:
                break;
            case EIROpCode::LoadConstNull: {
                m_ValueStack[m_ValueIndex++].SetNullValue();
                break;
            }
            case EIROpCode::LoadConstByte: {
                m_ValueStack[m_ValueIndex++].SetInt8Value(std::any_cast<uint8_t>(iri->opValue));
                break;
            }
            case EIROpCode::LoadConstSByte: {
                m_ValueStack[m_ValueIndex++].SetSInt8Value(std::any_cast<int8_t>(iri->opValue));
                break;
            }
            case EIROpCode::LoadConstBoolean: {
                m_ValueStack[m_ValueIndex++].SetBoolValue(std::any_cast<bool>(iri->opValue));
                break;
            }
            case EIROpCode::LoadConstInt16: {
                m_ValueStack[m_ValueIndex++].SetInt16Value(std::any_cast<int16_t>(iri->opValue));
                break;
            }
            case EIROpCode::LoadConstUInt16: {
                m_ValueStack[m_ValueIndex++].SetUInt16Value(std::any_cast<uint16_t>(iri->opValue));
                break;
            }
            case EIROpCode::LoadConstInt32: {
                m_ValueStack[m_ValueIndex++].SetInt32Value(std::any_cast<int32_t>(iri->opValue));
                break;
            }
            case EIROpCode::LoadConstUInt32: {
                m_ValueStack[m_ValueIndex++].SetUInt32Value(std::any_cast<uint32_t>(iri->opValue));
                break;
            }
            case EIROpCode::LoadConstInt64: {
                m_ValueStack[m_ValueIndex++].SetInt64Value(std::any_cast<int64_t>(iri->opValue));
                break;
            }
            case EIROpCode::LoadConstUInt64: {
                m_ValueStack[m_ValueIndex++].SetUInt64Value(std::any_cast<uint64_t>(iri->opValue));
                break;
            }
            case EIROpCode::LoadConstFloat: {
                m_ValueStack[m_ValueIndex++].SetFloatValue(std::any_cast<float>(iri->opValue));
                break;
            }
            case EIROpCode::LoadConstDouble: {
                m_ValueStack[m_ValueIndex++].SetDoubleValue(std::any_cast<double>(iri->opValue));
                break;
            }
            case EIROpCode::LoadConstString: {
                m_ValueStack[m_ValueIndex++].SetStringValue(std::any_cast<std::string>(iri->opValue));
                break;
            }
            // ... 继续其他指令的处理
            default: {
                Log::AddVM(EError::None, "Error 暂不支持" + std::to_string(static_cast<int>(iri->opCode)) + "的处理!!");
                break;
            }
        }
    }

} // namespace VM
} // namespace SimpleLanguage
