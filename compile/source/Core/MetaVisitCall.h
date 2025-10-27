//****************************************************************************
//  File:      MetaVisitCall.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description:  create visit variable or method call!
//****************************************************************************

#pragma once

#include "MetaBase.h"
#include <string>
#include <vector>
#include <memory>

namespace SimpleLanguage {
namespace Core {

// Forward declarations
class MetaFunction;
class MetaInputParamCollection;
class MetaExpressNode;
class MetaVariable;
class MetaClass;
class MetaType;
class MetaMemberFunction;
class MetaMemberData;
class MetaEnum;
class MetaData;
class MetaMemberVariable;
class MetaVisitVariable;
class MetaBraceOrBracketStatementsContent;
class MetaConstExpressNode;
class MetaGenTemplateFunction;
class MetaGenTemplateClass;

enum class EVisitType : uint8_t {
    ConstValue = 0,
    Variable = 1,
    VisitVariable = 2,
    IteratorVariable = 3,
    MethodCall = 4,
    New = 5,
    Enum = 6
};

class MetaMethodCall {
public:
    MetaMethodCall(MetaClass* staticMc, const std::vector<MetaType*>& staticMmitList, MetaFunction* fun, const std::vector<MetaType*>& mpipList, MetaInputParamCollection* paramCollection, MetaVariable* loadMv, MetaVariable* storeMv);
    virtual ~MetaMethodCall() = default;

    // Properties
    bool isRecieveReturnValue() const { return m_IsRecieveReturnValue; }
    MetaVariable* loadMetaVariable() const { return m_LoadMetaVariable; }
    MetaVariable* storeMetaVariable() const { return m_StoreMetaVariable; }
    MetaClass* staticCallerMetaClass() const { return m_StaticCallerMetaClass; }
    const std::vector<MetaType*>& staticMetaClassInputTemplateList() const { return m_StaticMetaClassInputTemplateList; }
    MetaFunction* function() const { return m_VMCallMetaFunction; }
    MetaMemberFunction* metaMemberFunction() const { return m_MetaMemberFunction; }
    const std::vector<MetaExpressNode*>& metaInputParamList() const { return m_MetaInputParamList; }
    const std::vector<MetaType*>& metaFunctionInputTemplateList() const { return m_MetaFunctionInputTemplateList; }

    // Methods
    void SetStoreMetaVariable(MetaVariable* mv);
    MetaType* GetMetaDefineType();
    MetaFunction* GetTemplateMemberFunction();
    std::string ToCommonString() const;
    std::string ToFormatString() const;
    std::string ToString() const;

protected:
    MetaVariable* m_LoadMetaVariable = nullptr;
    MetaVariable* m_StoreMetaVariable = nullptr;
    MetaClass* m_StaticCallerMetaClass = nullptr;
    bool m_IsRecieveReturnValue = true;
    std::vector<MetaType*> m_StaticMetaClassInputTemplateList;
    MetaFunction* m_VMCallMetaFunction = nullptr;
    MetaMemberFunction* m_MetaMemberFunction = nullptr;
    std::vector<MetaExpressNode*> m_MetaInputParamList;
    std::vector<MetaType*> m_MetaFunctionInputTemplateList;
};

class MetaVisitNode {
public:
    MetaVisitNode();
    virtual ~MetaVisitNode() = default;

    // Properties
    MetaConstExpressNode* constValueExpress() const { return m_ConstValueExpress; }
    EVisitType visitType() const { return m_VisitType; }
    MetaVariable* variable() const { return m_Variable; }
    MetaVisitVariable* visitVariable() const { return m_VisitVariable; }
    MetaMethodCall* methodCall() const { return m_MethodCall; }
    MetaType* callMetaType() const { return m_CallMetaType; }
    MetaBraceOrBracketStatementsContent* metaBraceStatementsContent() const { return m_MetaBraceStatementsContent; }

    // Static factory methods
    static MetaVisitNode* CreateByNewTemplate(MetaType* mt, MetaFunction* mf, MetaVariable* mv);
    static MetaVisitNode* CraeteByNewClass(MetaType* mt, MetaBraceOrBracketStatementsContent* mb, MetaVariable* mv = nullptr);
    static MetaVisitNode* CraeteByNewData(MetaType* mt, MetaBraceOrBracketStatementsContent* mb);
    static MetaVisitNode* CreateByConstExpress(MetaConstExpressNode* constExpress, MetaVariable* variable);
    static MetaVisitNode* CreateByEnumDefaultValue(MetaType* mt, MetaVariable* variable);
    static MetaVisitNode* CreateByMethodCall(MetaMethodCall* methodCall);
    static MetaVisitNode* CreateByVisitVariable(MetaVisitVariable* variable);
    static MetaVisitNode* CreateByVariable(MetaVariable* variable, MetaType* callerMt = nullptr);
    static MetaVisitNode* CreateByThis(MetaVariable* variable);
    static MetaVisitNode* CreateByBase(MetaVariable* variable);

    // Methods
    void SetMethodCall(MetaMethodCall* methodCall);
    MetaVariable* GetOrgTemplateMetaVariable();
    MetaType* GetMetaDefineType();
    MetaClass* GetMetaClass();
    MetaVariable* GetRetMetaVariable();
    int CalcParseLevel(int level);
    void CalcReturnType();
    std::string ToFormatString() const;

private:
    MetaConstExpressNode* m_ConstValueExpress = nullptr;
    EVisitType m_VisitType = EVisitType::Variable;
    MetaVariable* m_Variable = nullptr;
    MetaVisitVariable* m_VisitVariable = nullptr;
    MetaMethodCall* m_MethodCall = nullptr;
    MetaBraceOrBracketStatementsContent* m_MetaBraceStatementsContent = nullptr;
    MetaType* m_ReturnMetaType = nullptr;
    MetaType* m_CallMetaType = nullptr;
};

} // namespace Core
} // namespace SimpleLanguage