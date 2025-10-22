//****************************************************************************
//  File:      MetaNode.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta class's attribute
//****************************************************************************

#include "MetaNode.h"
#include "MetaModule.h"
#include "MetaNamespace.h"
#include "MetaData.h"
#include "MetaEnum.h"
#include "MetaClass.h"
#include <sstream>

namespace SimpleLanguage {
namespace Core {

MetaNode::MetaNode() {
    m_Deep = 0;
    m_AnchorDeep = 0;
    m_ParentNode = nullptr;
    m_EStructNodeType = EStructNodeType::Namespace;
    m_MetaModule = nullptr;
    m_MetaNamespace = nullptr;
    m_MetaData = nullptr;
    m_MetaEnum = nullptr;
    m_Name = "";
    m_AllName = "";
}

MetaNode::MetaNode(MetaModule* mm) : MetaNode() {
    m_MetaModule = mm;
    m_EStructNodeType = EStructNodeType::Module;
    if (mm != nullptr) {
        m_Name = mm->GetName();
    }
}

MetaNode::MetaNode(MetaNamespace* mn) : MetaNode() {
    m_MetaNamespace = mn;
    m_EStructNodeType = EStructNodeType::Namespace;
    if (mn != nullptr) {
        m_Name = mn->GetName();
    }
}

MetaNode::MetaNode(MetaData* md) : MetaNode() {
    m_MetaData = md;
    m_EStructNodeType = EStructNodeType::Data;
    if (md != nullptr) {
        m_Name = md->GetName();
    }
}

MetaNode::MetaNode(MetaEnum* me) : MetaNode() {
    m_MetaEnum = me;
    m_EStructNodeType = EStructNodeType::Enum;
    if (me != nullptr) {
        m_Name = me->GetName();
    }
}

MetaNode::MetaNode(MetaClass* mc) : MetaNode() {
    m_EStructNodeType = EStructNodeType::Class;
    if (mc != nullptr) {
        m_Name = mc->GetName();
    }
}

std::vector<MetaClass*> MetaNode::GetMetaClassList() const {
    std::vector<MetaClass*> result;
    for (const auto& pair : m_MetaTemplateClassDict) {
        result.push_back(pair.second);
    }
    return result;
}

std::string MetaNode::GetAllName() const {
    if (!m_AllName.empty()) {
        return m_AllName;
    }
    
    std::stringstream ss;
    if (m_ParentNode != nullptr && m_ParentNode->m_ParentNode != nullptr) {
        ss << m_ParentNode->GetAllName() << "::";
    }
    ss << m_Name;
    return ss.str();
}

std::string MetaNode::GetAllNameIncludeModule() const {
    std::stringstream ss;
    if (m_MetaModule != nullptr) {
        ss << m_MetaModule->GetName() << "::";
    }
    ss << GetAllName();
    return ss.str();
}

MetaNode* MetaNode::AddMetaNamespace(MetaNamespace* namespaceName) {
    if (namespaceName == nullptr) {
        return nullptr;
    }
    
    std::string name = namespaceName->GetName();
    auto it = m_ChildrenMetaNodeDict.find(name);
    if (it != m_ChildrenMetaNodeDict.end()) {
        return it->second;
    }
    
    MetaNode* mn = new MetaNode(namespaceName);
    mn->m_ParentNode = this;
    mn->SetDeep(m_Deep + 1);
    m_ChildrenMetaNodeDict[name] = mn;
    return mn;
}

MetaNode* MetaNode::AddMetaEnum(MetaEnum* me) {
    if (me == nullptr) {
        return nullptr;
    }
    
    std::string name = me->GetName();
    auto it = m_ChildrenMetaNodeDict.find(name);
    if (it != m_ChildrenMetaNodeDict.end()) {
        return it->second;
    }
    
    MetaNode* mn = new MetaNode(me);
    mn->m_ParentNode = this;
    mn->SetDeep(m_Deep + 1);
    m_ChildrenMetaNodeDict[name] = mn;
    return mn;
}

MetaNode* MetaNode::AddMetaData(MetaData* me) {
    if (me == nullptr) {
        return nullptr;
    }
    
    std::string name = me->GetName();
    auto it = m_ChildrenMetaNodeDict.find(name);
    if (it != m_ChildrenMetaNodeDict.end()) {
        return it->second;
    }
    
    MetaNode* mn = new MetaNode(me);
    mn->m_ParentNode = this;
    mn->SetDeep(m_Deep + 1);
    m_ChildrenMetaNodeDict[name] = mn;
    return mn;
}

MetaNode* MetaNode::AddMetaClass(MetaClass* mc) {
    if (mc == nullptr) {
        return nullptr;
    }
    
    std::string name = mc->GetName();
    auto it = m_ChildrenMetaNodeDict.find(name);
    if (it != m_ChildrenMetaNodeDict.end()) {
        return it->second;
    }
    
    MetaNode* mn = new MetaNode(mc);
    mn->m_ParentNode = this;
    mn->SetDeep(m_Deep + 1);
    m_ChildrenMetaNodeDict[name] = mn;
    return mn;
}

bool MetaNode::AddMetaNode(MetaNode* mn) {
    if (mn == nullptr) {
        return false;
    }
    
    std::string name = mn->GetName();
    if (m_ChildrenMetaNodeDict.find(name) != m_ChildrenMetaNodeDict.end()) {
        return false;
    }
    
    mn->m_ParentNode = this;
    mn->SetDeep(m_Deep + 1);
    m_ChildrenMetaNodeDict[name] = mn;
    return true;
}

bool MetaNode::IsMetaClass() const {
    return m_EStructNodeType == EStructNodeType::Class;
}

bool MetaNode::IsIncludeMetaNode(const std::string& name) const {
    return m_ChildrenMetaNodeDict.find(name) != m_ChildrenMetaNodeDict.end();
}

MetaNode* MetaNode::GetChildrenMetaNodeByName(const std::string& name) const {
    auto it = m_ChildrenMetaNodeDict.find(name);
    if (it != m_ChildrenMetaNodeDict.end()) {
        return it->second;
    }
    return nullptr;
}

MetaNamespace* MetaNode::GetMetaNamespaceByName(const std::string& name) const {
    auto it = m_ChildrenMetaNodeDict.find(name);
    if (it != m_ChildrenMetaNodeDict.end() && it->second->IsMetaNamespace()) {
        return it->second->GetMetaNamespace();
    }
    return nullptr;
}

MetaClass* MetaNode::GetMetaClassByTemplateCount(int count) const {
    auto it = m_MetaTemplateClassDict.find(count);
    if (it != m_MetaTemplateClassDict.end()) {
        return it->second;
    }
    return nullptr;
}

void MetaNode::SetDeep(int deep) {
    m_Deep = deep;
}

void MetaNode::SetAnchorDeep(int addep) {
    m_AnchorDeep = addep;
}

MetaNode* MetaNode::GetMetaBaseInParentByName(const std::string& inputname, bool isInclude) const {
    if (m_ParentNode == nullptr) {
        return nullptr;
    }
    
    MetaNode* result = m_ParentNode->GetChildrenMetaNodeByName(inputname);
    if (result != nullptr) {
        return result;
    }
    
    if (isInclude) {
        return m_ParentNode->GetMetaBaseInParentByName(inputname, isInclude);
    }
    
    return nullptr;
}

bool MetaNode::RemoveMetaChildNode(MetaNode* mb) {
    if (mb == nullptr) {
        return false;
    }
    
    std::string name = mb->GetName();
    auto it = m_ChildrenMetaNodeDict.find(name);
    if (it != m_ChildrenMetaNodeDict.end() && it->second == mb) {
        m_ChildrenMetaNodeDict.erase(it);
        delete mb;
        return true;
    }
    
    return false;
}

std::string MetaNode::ToFormatString() const {
    return GetAllName();
}

std::string MetaNode::ToString() const {
    return m_Name;
}

} // namespace Core
} // namespace SimpleLanguage