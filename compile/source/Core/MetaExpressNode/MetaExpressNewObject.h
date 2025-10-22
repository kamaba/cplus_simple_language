//****************************************************************************
//  File:      MetaExpressNewObject.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "MetaExpressBase.h"
#include "../MetaType.h"
#include "../MetaClass.h"
#include "../MetaBlockStatements.h"
#include "../MetaMethodCall.h"
#include "../MetaBraceOrBracketStatementsContent.h"
#include "../BaseMetaClass/CoreMetaClassManager.h"
#include "../Compile/CoreFileMeta/FileMetaBraceTerm.h"
#include "../Compile/CoreFileMeta/FileMetaBracketTerm.h"
#include "../Compile/CoreFileMeta/FileMetaCallTerm.h"
#include "../Compile/CoreFileMeta/FileMetaConstValueTerm.h"
#include "../Compile/CoreFileMeta/FileMetaParTerm.h"
#include <string>
#include <vector>

namespace SimpleLanguage {
namespace Core {

class MetaNewObjectExpressNode : public MetaExpressNode {
private:
    MetaMethodCall* m_MetaConstructFunctionCall = nullptr;
    MetaExpressNode* m_MetaEnumValue = nullptr;
    MetaBraceOrBracketStatementsContent* m_MetaBraceOrBracketStatementsContent = nullptr;
    
    FileMetaParTerm* m_FileMetaParTerm = nullptr;
    FileMetaCallTerm* m_FileMetaCallTerm = nullptr;
    FileMetaConstValueTerm* m_FileMetaConstValueTerm = nullptr;

public:
    MetaMethodCall* GetConstructFunctionCall() const { return m_MetaConstructFunctionCall; }
    MetaBraceOrBracketStatementsContent* GetMetaBraceOrBracketStatementsContent() const { return m_MetaBraceOrBracketStatementsContent; }
    
    // Constructor for array of dynamic classes
    MetaNewObjectExpressNode(MetaClass* ownermc, const std::vector<MetaDynamicClass*>& list) {
        m_OwnerMetaClass = ownermc;
        m_OwnerMetaBlockStatements = nullptr;
        m_MetaBraceOrBracketStatementsContent = nullptr;
        
        auto metaInputTemplateCollection = new MetaInputTemplateCollection();
        m_MetaDefineType = new MetaType(CoreMetaClassManager::GetInstance().GetArrayMetaClass(), nullptr, metaInputTemplateCollection);
    }
    
    // Constructor for range (1..x)
    MetaNewObjectExpressNode(FileMetaConstValueTerm* arrayLinkToken, MetaClass* ownerMC, MetaBlockStatements* mbs) {
        m_FileMetaConstValueTerm = arrayLinkToken;
        m_OwnerMetaClass = ownerMC;
        m_OwnerMetaBlockStatements = mbs;
        
        auto metaInputTemplateCollection = new MetaInputTemplateCollection();
        MetaType* mitp = new MetaType(CoreMetaClassManager::GetInstance().GetInt32MetaClass());
        metaInputTemplateCollection->AddMetaTemplateParamsList(mitp);
        
        m_MetaDefineType = new MetaType(CoreMetaClassManager::GetInstance().GetRangeMetaClass(), nullptr, metaInputTemplateCollection);
        
        MetaInputParamCollection* mdpc = new MetaInputParamCollection(ownerMC, mbs);
        std::string name = m_FileMetaConstValueTerm->GetName();
        std::vector<std::string> arr = SplitString(name, "..");
        
        if (arr.size() == 2) {
            int arr0 = 0;
            if (TryParseInt(arr[0], arr0)) {
                MetaConstExpressNode* mcen1 = new MetaConstExpressNode(EType::Int32, arr0);
                MetaInputParam* mip = new MetaInputParam(mcen1);
                mdpc->AddMetaInputParam(mip);
            }
            
            int arr1 = 0;
            if (TryParseInt(arr[1], arr1)) {
                MetaConstExpressNode* mcen2 = new MetaConstExpressNode(EType::Int32, arr1);
                MetaInputParam* mip2 = new MetaInputParam(mcen2);
                mdpc->AddMetaInputParam(mip2);
            }
            
            MetaInputParam* mip3 = new MetaInputParam(new MetaConstExpressNode(EType::Int32, 1));
            mdpc->AddMetaInputParam(mip3);
        }
        
        auto tfunction = m_MetaDefineType->GetMetaMemberConstructFunction(mdpc);
        if (tfunction != nullptr) {
            m_MetaConstructFunctionCall = new MetaMethodCall(nullptr, nullptr, tfunction, nullptr, mdpc, nullptr, nullptr);
        }
        
        Init();
    }
    
    // Constructor for simple type
    MetaNewObjectExpressNode(MetaType* mt, MetaClass* ownerMC, MetaBlockStatements* mbs) {
        m_OwnerMetaClass = ownerMC;
        m_OwnerMetaBlockStatements = mbs;
        m_MetaDefineType = new MetaType(mt);
        Init();
        m_MetaConstructFunctionCall = new MetaMethodCall(mt->GetMetaClass(), mt->GetTemplateMetaTypeList(), m_OwnerMetaBlockStatements->GetOwnerMetaFunction(), nullptr, nullptr, nullptr, nullptr);
    }
    
    // Constructor for call term with method call
    MetaNewObjectExpressNode(FileMetaCallTerm* fmct, MetaCallLink* mcl, MetaType* mt, MetaClass* ownerMC, MetaBlockStatements* mbs, MetaMethodCall* mmf) {
        m_FileMetaCallTerm = fmct;
        m_OwnerMetaClass = ownerMC;
        m_OwnerMetaBlockStatements = mbs;
        m_MetaConstructFunctionCall = mmf;
        m_MetaDefineType = new MetaType(mt);
        
        // Implementation details would go here...
        Init();
    }
    
    // Constructor for brace term
    MetaNewObjectExpressNode(FileMetaBraceTerm* fmbt, MetaType* mt, MetaClass* ownerMC, MetaBlockStatements* mbs, MetaVariable* equalMV) {
        m_MetaBraceOrBracketStatementsContent = new MetaBraceOrBracketStatementsContent(fmbt, mbs, ownerMC, equalMV);
        m_OwnerMetaClass = ownerMC;
        m_OwnerMetaBlockStatements = mbs;
        m_MetaDefineType = new MetaType(mt);
        m_MetaBraceOrBracketStatementsContent->SetMetaType(m_MetaDefineType);
        m_MetaBraceOrBracketStatementsContent->Parse();
        
        if (m_MetaBraceOrBracketStatementsContent->GetContentType() == MetaBraceOrBracketStatementsContent::EStatementsContentType::DynamicClass) {
            m_MetaDefineType = m_MetaBraceOrBracketStatementsContent->GetDefineMetaType();
        }
        if (m_MetaBraceOrBracketStatementsContent->GetContentType() == MetaBraceOrBracketStatementsContent::EStatementsContentType::DynamicData) {
            m_MetaDefineType = m_MetaBraceOrBracketStatementsContent->GetDefineMetaType();
        } else if (m_MetaBraceOrBracketStatementsContent->GetContentType() == MetaBraceOrBracketStatementsContent::EStatementsContentType::ArrayValue) {
            auto metaInputTemplateCollection = new MetaInputTemplateCollection();
            MetaClass* mc = m_MetaBraceOrBracketStatementsContent->GetMaxLevelMetaClassType();
            metaInputTemplateCollection->AddMetaTemplateParamsList(new MetaType(mc));
            m_MetaDefineType = new MetaType(CoreMetaClassManager::GetInstance().GetArrayMetaClass(), nullptr, metaInputTemplateCollection);
        }
        Init();
    }
    
    // Constructor for par term
    MetaNewObjectExpressNode(FileMetaParTerm* fmpt, MetaType* mt, MetaClass* mc, MetaBlockStatements* mbs) {
        m_FileMetaParTerm = fmpt;
        m_OwnerMetaClass = mc;
        m_OwnerMetaBlockStatements = mbs;
        auto mmf = new MetaMethodCall(nullptr, nullptr, mbs->GetOwnerMetaFunction(), nullptr, nullptr, nullptr, nullptr);
        m_MetaConstructFunctionCall = mmf;
        m_MetaDefineType = new MetaType(mt);
        Init();
    }
    
    // Constructor for bracket term
    MetaNewObjectExpressNode(FileMetaBracketTerm* fmbt, MetaClass* mc, MetaBlockStatements* mbs, MetaVariable* equalMV) {
        m_OwnerMetaClass = mc;
        m_OwnerMetaBlockStatements = mbs;
        m_MetaBraceOrBracketStatementsContent = new MetaBraceOrBracketStatementsContent(fmbt, m_OwnerMetaBlockStatements, m_OwnerMetaClass, equalMV);
        m_MetaBraceOrBracketStatementsContent->Parse();
        MetaClass* inputType = m_MetaBraceOrBracketStatementsContent->GetMaxLevelMetaClassType();
        
        auto metaInputTemplateCollection = new MetaInputTemplateCollection();
        MetaType* mitp = new MetaType(inputType);
        metaInputTemplateCollection->AddMetaTemplateParamsList(mitp);
        m_MetaDefineType = new MetaType(CoreMetaClassManager::GetInstance().GetArrayMetaClass(), nullptr, metaInputTemplateCollection);
        
        MetaInputParamCollection* mipc = new MetaInputParamCollection(mc, mbs);
        mipc->AddMetaInputParam(new MetaInputParam(new MetaConstExpressNode(EType::Int32, m_MetaBraceOrBracketStatementsContent->GetCount())));
        MetaMemberFunction* mmf = m_MetaDefineType->GetMetaClass()->GetMetaMemberConstructFunction(mipc);
        
        m_MetaConstructFunctionCall = new MetaMethodCall(nullptr, nullptr, mmf, nullptr, mipc, nullptr, nullptr);
    }
    
private:
    void Init() {
        // Initialization logic
    }
    
    std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter) {
        std::vector<std::string> result;
        size_t start = 0;
        size_t end = str.find(delimiter);
        
        while (end != std::string::npos) {
            result.push_back(str.substr(start, end - start));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }
        result.push_back(str.substr(start));
        return result;
    }
    
    bool TryParseInt(const std::string& str, int& result) {
        try {
            result = std::stoi(str);
            return true;
        } catch (...) {
            return false;
        }
    }

public:
    virtual void Parse(AllowUseSettings* auc) override {
        // Parse implementation
    }
    
    virtual int CalcParseLevel(int level) override {
        return level;
    }
    
    virtual void CalcReturnType() override {
        MetaExpressNode::CalcReturnType();
    }
    
    virtual MetaType* GetReturnMetaDefineType() override {
        if (m_MetaDefineType != nullptr) {
            return m_MetaDefineType;
        }
        if (m_MetaConstructFunctionCall != nullptr) {
            m_MetaDefineType = m_MetaConstructFunctionCall->GetMetaDefineType();
        }
        return m_MetaDefineType;
    }
    
    virtual std::string ToTokenString() override {
        return MetaExpressNode::ToTokenString();
    }
    
    virtual std::string ToFormatString() override {
        std::string result;
        
        if (m_MetaDefineType != nullptr && m_MetaDefineType->IsEnum()) {
            result = m_MetaDefineType->GetName() + "." + m_MetaDefineType->GetEnumValue()->GetName();
            if (m_MetaEnumValue != nullptr) {
                result += "(" + m_MetaEnumValue->ToFormatString() + ")";
            }
        } else if (m_MetaDefineType != nullptr && m_MetaDefineType->IsData()) {
            result = m_MetaDefineType->GetName() + "{";
            if (m_MetaBraceOrBracketStatementsContent != nullptr) {
                for (int i = 0; i < m_MetaBraceOrBracketStatementsContent->GetCount(); i++) {
                    auto bsc = m_MetaBraceOrBracketStatementsContent->GetAssignStatementsList()[i];
                    if (bsc == nullptr) continue;
                    
                    result += bsc->GetMetaMemberData()->ToFormatString2(m_MetaDefineType->IsDynamicData());
                    
                    if (i < m_MetaBraceOrBracketStatementsContent->GetCount() - 1) {
                        result += ",";
                    }
                }
            }
            result += "}";
        } else {
            if (m_MetaDefineType != nullptr) {
                result = m_MetaDefineType->GetName() + "()";
                result += ".";
            }
            result += m_MetaBraceOrBracketStatementsContent ? m_MetaBraceOrBracketStatementsContent->ToFormatString() : "";
        }
        
        return result;
    }
    
    static MetaNewObjectExpressNode* CreateNewObjectExpressNodeByPar(FileMetaParTerm* root, MetaType* mt, MetaClass* omc, MetaBlockStatements* mbs) {
        if (root == nullptr || mt == nullptr) return nullptr;
        
        MetaInputParamCollection* mpc = new MetaInputParamCollection(root, omc, mbs);
        
        if (mpc->GetMetaInputParamList().size() > 0) {
            MetaMemberFunction* mmf = mt->GetMetaMemberConstructFunction(mpc);
            if (mmf == nullptr) return nullptr;
            
            MetaNewObjectExpressNode* mnoen = new MetaNewObjectExpressNode(root, mt, omc, mbs);
            return mnoen;
        } else {
            MetaNewObjectExpressNode* mnoen = new MetaNewObjectExpressNode(root, mt, omc, mbs);
            return mnoen;
        }
    }
};

} // namespace Core
} // namespace SimpleLanguage
