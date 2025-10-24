//****************************************************************************
//  File:      MetaCallNode.h
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/5/17 12:00:00
//  Description:  this's a calllink's node handle
//****************************************************************************

#pragma once

#include "MetaBase.h"
#include "../Define.h"
#include "AllowUseSettings.h"
#include <string>
#include <vector>
#include <memory>

using namespace SimpleLanguage::Compile;

namespace SimpleLanguage {
    namespace Compile
    {
        class FileMetaCallNode;
        class Token;
    }
namespace Core {

enum class ECallNodeSign : uint8_t {
    Null = 0,
    Period = 1,
    Pointer = 2
};

enum class ECallNodeType : uint8_t {
    Null = 0,
    MetaNode = 1,
    MetaType = 2,
    ClassName = 3,
    TypeName = 4,
    TemplateName = 5,
    EnumName = 6,
    EnumDefaultValue = 7,
    EnumValueArray = 8,
    DataName = 9,
    DataValue = 10,
    FunctionInnerVariableName = 11,
    VisitVariable = 12,
    IteratorVariable = 13,
    MemberVariableName = 14,
    MemberDataName = 15,
    NewClass = 16,
    NewTemplate = 17,
    NewData = 18,
    MemberFunctionName = 19,
    ConstValue = 20,
    This = 21,
    Base = 22,
    Global = 23,
    Express = 24
};


class AllowUseSettings {
public:
    bool useNotStatic = false;
    bool useNotConst = true;
    bool callFunction = true;
    bool callConstructFunction = true;
    bool setterFunction = false;
    bool getterFunction = true;
    EParseFrom parseFrom = EParseFrom::None;

    AllowUseSettings() = default;
    AllowUseSettings(const AllowUseSettings& clone) {
        useNotStatic = clone.useNotStatic;
        useNotConst = clone.useNotConst;
        callFunction = clone.callFunction;
        callConstructFunction = clone.callConstructFunction;
        setterFunction = clone.setterFunction;
        getterFunction = clone.getterFunction;
        parseFrom = clone.parseFrom;
    }
};

// 前向声明
class AllowUseSettings;
class MetaClass;
class MetaType;
class MetaNode;
class MetaMemberVariable;
class MetaMemberFunction;
class MetaMemberData;
class MetaEnum;
class MetaData;
class MetaCallLink;
class MetaExpressNode;
class MetaBlockStatements;
class MetaVariable;
class MetaTemplate;
class MetaInputParamCollection;
class MetaBraceOrBracketStatementsContent;
class MetaConstExpressNode;
class MetaVisitVariable;
class MetaMemberEnum;
class MetaGenTemplateClass;
class MetaGenTempalteFunction;
class MetaInputTemplateCollection;
class MetaBase;

class MetaCallNode : public MetaBase {
public:
    // 构造函数
    MetaCallNode();
    MetaCallNode(FileMetaCallNode* fmcn1, FileMetaCallNode* fmcn2, MetaClass* mc, MetaBlockStatements* mbs, MetaType* fdmt);
    virtual ~MetaCallNode() = default;

    // 属性访问器
    std::string GetName() const { return m_Name; }
    MetaCallNode* GetFrontCallNode() const { return m_FrontCallNode; }
    ECallNodeType GetCallNodeType() const { return m_CallNodeType; }
    MetaExpressNode* GetMetaExpressValue() const { return m_ExpressNode; }
    const std::vector<MetaType*>& GetMetaTemplateParamsList() const { return m_MetaTemplateParamsList; }
    MetaBraceOrBracketStatementsContent* GetMetaBraceStatementsContent() const { return m_MetaBraceStatementsContent; }
    MetaInputParamCollection* GetMetaInputParamCollection() const { return m_MetaInputParamCollection; }
    MetaBlockStatements* GetOwnerMetaFunctionBlock() const { return m_OwnerMetaFunctionBlock; }
    MetaVariable* GetStoreMetaVariable() const { return m_StoreMetaVariable; }
    MetaType* GetCallMetaType() const { return m_CallMetaType; }
    MetaVariable* GetMetaVariable() const { return m_MetaVariable; }
    MetaMemberFunction* GetMetaFunction() const { return m_MetaFunction; }
    MetaType* GetMetaType() const { return m_MetaType; }

    // 设置方法
    void SetFrontCallNode(MetaCallNode* mcn);
    void SetDefineMetaVariable(MetaVariable* mv);
    void SetStoreMetaVariable(MetaVariable* mv);

    // 核心方法
    bool ParseNode(AllowUseSettings* _auc);
    bool CreateCallNode();
    bool GetFirstNode(const std::string& inputname, MetaClass* mc, int count);
    bool GetFunctionOrVariableByOwnerClass(MetaClass* mc, const std::string& inputname);
    bool CreateMetaTemplateParams(MetaClass* mc, MetaMemberFunction* mmf);
    MetaMemberData* GetDataValueByMetaData(MetaData* md, const std::string& inputName);
    MetaMemberData* GetDataValueByMetaMemberData(MetaMemberData* md, const std::string& inputName);
    MetaBase* HandleCastFunction(MetaClass* mc);
    virtual std::string ToString() const override;
    virtual std::string ToFormatString() const override;

private:
    AllowUseSettings* m_AllowUseSettings = nullptr;
    ECallNodeType m_CallNodeType = ECallNodeType::Null;
    ECallNodeSign m_CallNodeSign = ECallNodeSign::Null;
    bool m_IsArray = false;
    bool m_IsFunction = false;

    MetaCallNode* m_FrontCallNode = nullptr;
    FileMetaCallNode* m_FileMetaCallSign = nullptr;
    FileMetaCallNode* m_FileMetaCallNode = nullptr;
    Token* m_Token = nullptr;

    MetaType* m_CallMetaType = nullptr;
    MetaBlockStatements* m_OwnerMetaFunctionBlock = nullptr;
    MetaClass* m_OwnerMetaClass = nullptr;
    MetaInputParamCollection* m_MetaInputParamCollection = nullptr;
    std::vector<MetaType*> m_MetaTemplateParamsList;
    MetaBraceOrBracketStatementsContent* m_MetaBraceStatementsContent = nullptr;
    MetaType* m_FrontDefineMetaType = nullptr;
    MetaExpressNode* m_ExpressNode = nullptr;
    std::vector<MetaCallLink*> m_MetaArrayCallNodeList;
    MetaVariable* m_DefineMetaVariable = nullptr;
    MetaVariable* m_StoreMetaVariable = nullptr;

    MetaNode* m_MetaNode = nullptr;
    MetaType* m_MetaType = nullptr;
    MetaClass* m_MetaClass = nullptr;
    MetaData* m_MetaData = nullptr;
    MetaEnum* m_MetaEnum = nullptr;
    MetaTemplate* m_MetaTemplate = nullptr;
    MetaVariable* m_MetaVariable = nullptr;
    MetaMemberFunction* m_MetaFunction = nullptr;
    std::string m_Name;
};

} // namespace Core
} // namespace SimpleLanguage