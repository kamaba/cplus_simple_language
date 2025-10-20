//****************************************************************************
//  File:      MetaMemberEnum.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2025/1/17 12:00:00
//  Description: enum's memeber variable metadata and member 'data' metadata
//****************************************************************************

#pragma once

#include "MetaVariable.h"
#include "MetaExpressNode.h"
#include "MetaType.h"
#include "MetaClass.h"
#include "FileMetaMemberVariable.h"
#include "MetaConstExpressNode.h"
#include "MetaCallLinkExpressNode.h"
#include "MetaNewObjectExpressNode.h"
#include "Log.h"
#include "CoreMetaClassManager.h"
#include "ExpressManager.h"
#include "AllowUseSettings.h"
#include "CreateExpressParam.h"
#include "Global.h"
#include "CompilerUtil.h"
#include "MetaInputParamCollection.h"
#include "MetaMemberFunction.h"
#include "MetaGenTemplate.h"
#include <vector>
#include <string>
#include <sstream>

namespace SimpleLanguage {
namespace Core {

class MetaMemberEnum : public MetaVariable {
public:
    // Properties
    EFromType GetFromType() const { return m_FromType; }
    int GetIndex() const { return m_Index; }
    MetaExpressNode* GetExpress() const { return m_Express; }
    int GetParseLevel() const { return parseLevel; }
    void SetParseLevel(int level) { parseLevel = level; }
    bool IsInnerDefine() const { return m_IsInnerDefine; }
    MetaConstExpressNode* GetConstExpressNode() const { return dynamic_cast<MetaConstExpressNode*>(m_Express); }

    // Static constants
    static int s_ConstLevel;
    static int s_IsHaveRetStaticLevel;
    static int s_NoHaveRetStaticLevel;
    static int s_DefineMetaTypeLevel;
    static int s_ExpressLevel;

    // Constructor
    MetaMemberEnum(MetaClass* mc, FileMetaMemberVariable* fmmv);

    // Methods
    void ParseDefineMetaType() override;
    bool ParseMetaExpress() override;
    void SetExpress(MetaConstExpressNode* mcen);
    std::string ToFormatString() override;
    std::string ToTokenString();

private:
    MetaExpressNode* CreateExpressNodeInClassMetaVariable();

    EFromType m_FromType = EFromType::Code;
    int m_Index = -1;
    FileMetaMemberVariable* m_FileMetaMemeberVariable = nullptr;
    MetaExpressNode* m_Express = nullptr;
    bool m_IsInnerDefine = false;
    std::vector<MetaGenTemplate*> m_MetaGenTemplateList;

    bool m_IsSupportConstructionFunctionOnlyBraceType = false;
    bool m_IsSupportConstructionFunctionConnectBraceType = true;
    bool m_IsSupportConstructionFunctionOnlyParType = true;
    bool m_IsSupportInExpressUseStaticMetaMemeberFunction = true;
    bool m_IsSupportInExpressUseStaticMetaVariable = true;
    bool m_IsSupportInExpressUseCurrentClassNotStaticMemberMetaVariable = true;

    int parseLevel = -1;
};

} // namespace Core
} // namespace SimpleLanguage
