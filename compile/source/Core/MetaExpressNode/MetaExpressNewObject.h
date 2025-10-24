//****************************************************************************
//  File:      MetaExpressNewObject.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "MetaExpressBase.h"
#include <string>
#include <vector>

namespace SimpleLanguage {
    namespace Compile {
        class Token;
        class FileMetaOpAssignSyntax;
        class FileInputParamNode;
        class FileMetaOpAssignSyntax;
        class FileMetaDefineVariableSyntax;
        class FileMetaParTerm;
    }
namespace Core {

// Forward declarations
    class MetaExpressNode;
class MetaCallLinkExpressNode;
class MetaConstExpressNode;
class MetaInputParamCollection;
class MetaInputTemplateCollection;
class MetaDynamicClass;
class MetaMemberVariable;
class MetaData;
class MetaMemberData;
class MetaType;
class MetaClass;
class ClassManager;
class ExpressManager;
class CreateExpressParam;
class MetaBlockStatements;
class MetaMethodCall;
class MetaBraceAssignStatements;

// MetaBraceAssignStatements class
class MetaBraceAssignStatements {
public:
    // Properties
    MetaMemberVariable* GetMetaMemberVariable() const { return m_MetaMemberVariable; }
    MetaMemberData* GetMetaMemberData() const { return m_MetaMemberData; }
    MetaExpressNode* GetExpressNode() const { return m_MetaExpress; }
    int GetOpLevel() const { return m_MetaExpress != nullptr ? m_MetaExpress->GetOpLevel() : 0; }

    // Constructors
    MetaBraceAssignStatements(MetaBlockStatements* mbs, MetaType* mt, Compile::FileMetaCallLink* fmcl);
    MetaBraceAssignStatements(MetaBlockStatements* mbs, MetaType* mc, MetaExpressNode* men);
    MetaBraceAssignStatements(MetaBlockStatements* mbs, MetaExpressNode* men, MetaMemberVariable* mmv);
    MetaBraceAssignStatements(MetaBlockStatements* mbs, MetaType* mt, Compile::FileMetaOpAssignSyntax* fmos);
    MetaBraceAssignStatements(MetaBlockStatements* mbs, MetaType* mt, Compile::FileMetaDefineVariableSyntax* fmdvs);

    // Methods
    MetaClass* GetRetMetaClass();
    void SetDefineName(const std::string& definaname);
    bool CalcReturnType();
    MetaExpressNode* CreateExpressNodeInNewObjectStatements(MetaVariable* mv, MetaBlockStatements* mbs, Compile::FileMetaBaseTerm* fme);
    std::string ToFormatString();

private:
    MetaMemberVariable* m_MetaMemberVariable = nullptr;
    MetaMemberData* m_MetaMemberData = nullptr;
    Compile::Token* m_AssignToken = nullptr;
    MetaExpressNode* m_MetaExpress = nullptr;
    MetaBlockStatements* m_OwnerMetaBlockStatements = nullptr;
    std::string m_DefineName = "";
    Compile::FileMetaDefineVariableSyntax* m_FileMetaDefineVariableSyntax = nullptr;
    Compile::FileMetaOpAssignSyntax* m_FileMetaOpAssignSyntax = nullptr;
};

// MetaBraceOrBracketStatementsContent class
class MetaBraceOrBracketStatementsContent {
public:
    enum class EStatementsContentType {
        None,
        ArrayValue,
        ClassValueAssign,
        DataValueAssign,
        DynamicClass,
        DynamicData,
    };

    // Properties
    EStatementsContentType GetContentType() const { return m_ContentType; }
    int GetCount() const { return static_cast<int>(m_AssignStatementsList.size()); }
    const std::vector<MetaBraceAssignStatements*>& GetAssignStatementsList() const { return m_AssignStatementsList; }
    MetaType* GetDefineMetaType() const { return m_DefineMetaType; }

    // Constructors
    MetaBraceOrBracketStatementsContent(MetaBlockStatements* mbs, MetaClass* mc);
    MetaBraceOrBracketStatementsContent(Compile::FileMetaBraceTerm* fileMetaBraceTerm, MetaBlockStatements* mbs, MetaClass* mc);
    MetaBraceOrBracketStatementsContent(Compile::FileMetaBraceTerm* fileMetaBraceTerm, MetaBlockStatements* mbs, MetaClass* mc, MetaVariable* parentMt);
    MetaBraceOrBracketStatementsContent(Compile::FileMetaBracketTerm* fileMetaBracketTerm, MetaBlockStatements* mbs, MetaClass* mc, MetaVariable* parentMt);

    // Methods
    void SetMetaType(MetaType* mt);
    void Parse();
    MetaClass* GetMaxLevelMetaClassType();
    std::string ToFormatString();

private:
    std::vector<MetaBraceAssignStatements*> m_AssignStatementsList;
    Compile::FileMetaBraceTerm* m_FileMetaBraceTerm = nullptr;
    Compile::FileMetaBracketTerm* m_FileMetaBracketTerm = nullptr;
    MetaClass* m_OwnerMetaClass = nullptr;
    MetaBlockStatements* m_OwnerMetaBlockStatements = nullptr;
    MetaType* m_DefineMetaType = nullptr;
    MetaVariable* m_EqualMetaVariable = nullptr;
    MetaData* m_NewMetaData = nullptr;
    MetaData* m_NewTempMetaData = nullptr;
    EStatementsContentType m_ContentType = EStatementsContentType::None;
};

// MetaNewObjectExpressNode class
class MetaNewObjectExpressNode : public MetaExpressNode {
public:
    // Properties
    MetaMethodCall* GetConstructFunctionCall() const { return m_MetaConstructFunctionCall; }
    MetaBraceOrBracketStatementsContent* GetMetaBraceOrBracketStatementsContent() const { return m_MetaBraceOrBracketStatementsContent; }

    // Constructors
    MetaNewObjectExpressNode(MetaClass* ownermc, const std::vector<MetaDynamicClass*>& list);
    MetaNewObjectExpressNode(Compile::FileMetaConstValueTerm* arrayLinkToken, MetaClass* ownerMC, MetaBlockStatements* mbs);
    MetaNewObjectExpressNode(MetaType* mt, MetaClass* ownerMC, MetaBlockStatements* mbs);
    MetaNewObjectExpressNode(Compile::FileMetaCallTerm* fmct, MetaCallLink* mcl, MetaType* mt, MetaClass* ownerMC, MetaBlockStatements* mbs, MetaMethodCall* mmf);
    MetaNewObjectExpressNode(Compile::FileMetaBraceTerm* fmbt, MetaType* mt, MetaClass* ownerMC, MetaBlockStatements* mbs, MetaVariable* equalMV);
    MetaNewObjectExpressNode(Compile::FileMetaParTerm* fmpt, MetaType* mt, MetaClass* mc, MetaBlockStatements* mbs);
    MetaNewObjectExpressNode(Compile::FileMetaBracketTerm* fmbt, MetaClass* mc, MetaBlockStatements* mbs, MetaVariable* equalMV);

    // Override methods
    virtual void Parse(AllowUseSettings* auc) override;
    virtual int CalcParseLevel(int level) override;
    virtual void CalcReturnType() override;
    virtual MetaType* GetReturnMetaDefineType() override;
    virtual std::string ToTokenString() override;
    virtual std::string ToFormatString() override;

    // Static factory method
    static MetaNewObjectExpressNode* CreateNewObjectExpressNodeByPar(Compile::FileMetaParTerm* root, MetaType* mt, MetaClass* omc, MetaBlockStatements* mbs);

private:
    void Init();

    MetaMethodCall* m_MetaConstructFunctionCall = nullptr;
    MetaExpressNode* m_MetaEnumValue = nullptr;
    MetaBraceOrBracketStatementsContent* m_MetaBraceOrBracketStatementsContent = nullptr;

    Compile::FileMetaParTerm* m_FileMetaParTerm = nullptr;
    Compile::FileMetaCallTerm* m_FileMetaCallTerm = nullptr;
    Compile::FileMetaConstValueTerm* m_FileMetaConstValueTerm = nullptr;
};

} // namespace Core
} // namespace SimpleLanguage