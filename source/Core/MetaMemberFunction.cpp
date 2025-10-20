//****************************************************************************
//  File:      MetaMemberFunction.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/30 12:00:00
//  Description: Meta enum's attribute
//****************************************************************************

#include "MetaMemberFunction.h"
#include "MetaClass.h"
#include "MetaType.h"
#include "MetaTemplate.h"
#include "MetaDefineParam.h"
#include "MetaDefineParamCollection.h"
#include "MetaTemplateCollection.h"
#include "MetaBlockStatements.h"
#include "MetaStatements.h"
#include "MetaVariable.h"
#include "../Debug/Log.h"
#include <sstream>

namespace SimpleLanguage {
namespace Core {

    // MetaMemberFunctionTemplateNode 实现
    MetaMemberFunctionTemplateNode::MetaMemberFunctionTemplateNode()
    {
    }

    void MetaMemberFunctionTemplateNode::SetDeep(int deep)
    {
        // 简化实现
    }

    MetaMemberFunction* MetaMemberFunctionTemplateNode::IsSameMetaMemeberFunction(MetaMemberFunction* mmf)
    {
        // 简化实现
        return nullptr;
    }

    bool MetaMemberFunctionTemplateNode::AddMetaMemberFunction(MetaMemberFunction* mmf)
    {
        // 简化实现
        return true;
    }

    void MetaMemberFunctionTemplateNode::ParseMemberFunctionDefineMetaType()
    {
        // 简化实现
    }

    // MetaMemberFunctionNode 实现
    MetaMemberFunctionNode::MetaMemberFunctionNode()
    {
    }

    void MetaMemberFunctionNode::SetDeep(int deep)
    {
        // 简化实现
    }

    MetaMemberFunction* MetaMemberFunctionNode::IsSameMetaMemeberFunction(MetaMemberFunction* mmf)
    {
        // 简化实现
        return nullptr;
    }

    bool MetaMemberFunctionNode::AddMetaMemberFunction(MetaMemberFunction* mmf)
    {
        // 简化实现
        return true;
    }

    void MetaMemberFunctionNode::ParseMemberFunctionDefineMetaType()
    {
        // 简化实现
    }

    std::vector<MetaMemberFunction*> MetaMemberFunctionNode::GetMetaMemberFunctionListByParamCount(int count)
    {
        auto it = m_MetaParamFunctionDict.find(count);
        if (it != m_MetaParamFunctionDict.end())
        {
            return it->second;
        }
        return std::vector<MetaMemberFunction*>();
    }

    // MetaMemberFunction 实现
    MetaMemberFunction::MetaMemberFunction()
    {
    }

    MetaMemberFunction::MetaMemberFunction(MetaClass* mc)
        : m_OwnerMetaClass(mc)
    {
    }

    MetaMemberFunction::MetaMemberFunction(MetaClass* mc, void* fmmf)
        : m_OwnerMetaClass(mc)
        , m_FileMetaMemberFunction(fmmf)
    {
        // 简化实现
    }

    MetaMemberFunction::MetaMemberFunction(MetaClass* mc, const std::string& name)
        : m_OwnerMetaClass(mc)
        , m_Name(name)
        , m_IsCanRewrite(true)
    {
        init();
    }

    MetaMemberFunction::MetaMemberFunction(const MetaMemberFunction& mmf)
        : MetaBase(mmf)
        , m_IsTemplateFunction(mmf.m_IsTemplateFunction)
        , m_ConstructInitFunction(mmf.m_ConstructInitFunction)
        , m_IsWithInterface(mmf.m_IsWithInterface)
        , m_FileMetaMemberFunction(mmf.m_FileMetaMemberFunction)
        , m_GenTempalteFunctionList(mmf.m_GenTempalteFunctionList)
        , m_SourceMetaMemberFunction(mmf.m_SourceMetaMemberFunction)
    {
    }

    std::string MetaMemberFunction::GetFunctionAllName() const
    {
        if (!m_FunctionAllName.empty())
        {
            return m_FunctionAllName;
        }
        return MetaBase::GetName();
    }

    int MetaMemberFunction::GetParseLevel() const
    {
        if (m_IsTemplateFunction)
        {
            return 0;
        }
        else if (m_OwnerMetaClass && m_OwnerMetaClass->isTemplateClass())
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }

    void* MetaMemberFunction::GetToken() const
    {
        // 简化实现
        return nullptr;
    }

    bool MetaMemberFunction::IsEqualWithMMFByNameAndParam(MetaMemberFunction* mmf)
    {
        if (mmf->GetName() != m_Name) return false;
        // 简化实现，实际需要比较参数
        return true;
    }

    void MetaMemberFunction::AddMetaDefineParam(MetaDefineParam* mdp)
    {
        if (m_MetaMemberParamCollection)
        {
            // m_MetaMemberParamCollection->addMetaDefineParam(mdp);
        }
    }

    void MetaMemberFunction::AddMetaDefineTemplate(MetaTemplate* mt)
    {
        if (m_MetaMemberTemplateCollection)
        {
            // m_MetaMemberTemplateCollection->addMetaDefineTemplate(mt);
        }
    }

    void* MetaMemberFunction::AddGenTemplateMemberFunctionByMetaTypeList(MetaClass* mc, const std::vector<MetaType*>& list)
    {
        // 简化实现
        return nullptr;
    }

    void* MetaMemberFunction::AddGenTemplateMemberFunctionBySelf(MetaClass* mc, const std::vector<MetaClass*>& list)
    {
        // 简化实现
        return nullptr;
    }

    void* MetaMemberFunction::GetGenTemplateFunction(const std::vector<MetaClass*>& mcList)
    {
        // 简化实现
        return nullptr;
    }

    bool MetaMemberFunction::Parse()
    {
        bool flag = Parse();
        UpdateVritualFunctionName();
        return flag;
    }

    void MetaMemberFunction::ParseDefineMetaType()
    {
        // 简化实现
    }

    void MetaMemberFunction::CreateMetaExpress()
    {
        // 简化实现
    }

    bool MetaMemberFunction::ParseMetaExpress()
    {
        // 简化实现
        return true;
    }

    void MetaMemberFunction::ParseStatements()
    {
        // 简化实现
    }

    void MetaMemberFunction::UpdateVritualFunctionName()
    {
        std::ostringstream sb;
        sb << m_Name;
        sb << "_";
        if (m_ReturnMetaVariable)
        {
            sb << m_ReturnMetaVariable->GetMetaDefineType()->ToFormatString();
        }
        sb << "_";
        if (m_MetaMemberParamCollection)
        {
            sb << m_MetaMemberParamCollection->GetMaxParamCount();
        }
        m_VirtualFunctionName = sb.str();
    }

    MetaType* MetaMemberFunction::AddMetaPreTemplateFunction(MetaType* mt, bool& isGenMetaClass)
    {
        // 简化实现
        isGenMetaClass = false;
        return mt;
    }

    MetaType* MetaMemberFunction::FindBindStructTemplateFunctionMtList(MetaType* mt)
    {
        // 简化实现
        return nullptr;
    }

    MetaType* MetaMemberFunction::FindBindStructTemplateFunctionAndClassMtList(MetaType* mt)
    {
        // 简化实现
        return nullptr;
    }

    MetaStatements* MetaMemberFunction::CreateMetaSyntax(void* rootMs, MetaBlockStatements* currentBlockStatements)
    {
        // 简化实现
        return nullptr;
    }

    MetaStatements* MetaMemberFunction::HandleMetaSyntax(MetaBlockStatements* currentBlockStatements, 
                                                       MetaStatements*& beforeStatements,
                                                       void* childFms)
    {
        // 简化实现
        return nullptr;
    }

    std::string MetaMemberFunction::ToString() const
    {
        std::ostringstream sb;
        if (m_ReturnMetaVariable)
        {
            sb << m_ReturnMetaVariable->GetMetaDefineType()->toFormatString();
        }
        sb << " ";
        
        if (m_OwnerMetaClass)
        {
            sb << m_OwnerMetaClass->GetAllClassName();
            sb << ".";
        }
        sb << m_Name;
        sb << "()";
        return sb.str();
    }

    std::string MetaMemberFunction::ToFormatString() const
    {
        std::ostringstream sb;

        for (int i = 0; i < GetRealDeep(); i++)
            sb << Global::tabChar;

        sb << GetPermissionString() << " ";
        if (IsStatic())
        {
            sb << " static";
        }
        if (IsOverrideFunction())
        {
            sb << " override";
        }
        if (IsGet())
        {
            sb << " get";
        }
        if (IsSet())
        {
            sb << " set";
        }
        if (IsWithInterface())
        {
            sb << " interface";
        }
        sb << " ";
        if (m_ReturnMetaVariable)
        {
            sb << m_ReturnMetaVariable->GetMetaDefineType()->toFormatString();
        }
        sb << " " << GetName();
        sb << "()";
        sb << "\n";

        if (m_MetaBlockStatements)
            sb << m_MetaBlockStatements->toFormatString();

        sb << "\n";
        return sb.str();
    }

    void MetaMemberFunction::init()
    {
        m_ConstructInitFunction = (m_Name == "_init_");

        MetaType* defineMetaType = nullptr;
        if (m_ConstructInitFunction)
        {
            // defineMetaType = new MetaType(CoreMetaClassManager::voidMetaClass);
        }
        else
        {
            // defineMetaType = new MetaType(CoreMetaClassManager::objectMetaClass);
        }
        
        if (IsSet() && !IsGet())
        {
            // defineMetaType = new MetaType(CoreMetaClassManager::voidMetaClass);
        }
        
        if (!IsStatic())
        {
            // 创建 this 变量
            // m_ThisMetaVariable = new MetaVariable(m_OwnerMetaClass->getAllClassName() + "." + m_Name + ".this", MetaVariable::EVariableFrom::Argument, nullptr, m_OwnerMetaClass, mt);
        }
        
        // m_ReturnMetaVariable = new MetaVariable(m_OwnerMetaClass->getAllClassName() + "." + m_Name + ".define", MetaVariable::EVariableFrom::Argument, nullptr, m_OwnerMetaClass, defineMetaType);
    }

} // namespace Core
} // namespace SimpleLanguage
