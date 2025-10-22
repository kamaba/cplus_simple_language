//****************************************************************************
//  File:      MetaMemberVariable.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/11/30 12:00:00
//  Description: class's memeber variable metadata and member 'data' metadata
//****************************************************************************

#pragma once

#include "MetaVariable.h"
#include "MetaExpressNode/MetaExpressBase.h"
#include "../Compile/FileMeta/FileMetaMemberVariable.h"
#include "../Debug/Log.h"
#include <string>
#include <vector>

namespace SimpleLanguage {
namespace Core {

enum class EFromType : uint8_t {
    Code = 1,         // 写的.s代码
    Manual = 2,       // 手动，通过c#代码
    CodeAndManual = 3,
    CSharp = 4,       // 通过c#的dll文件，或者是编译完的代码的识别
    ManualAndCSharp = 6,  // 手动注入的c#代码进逻辑解析
    All = 7
};

class MetaMemberVariable : public MetaVariable {
private:
    EFromType m_FromType = EFromType::Code;
    int m_Index = -1;
    void* m_FileMetaMemeberVariable = nullptr;  // FileMetaMemberVariable*
    MetaExpressNode* m_Express = nullptr;
    bool m_IsInnerDefine = false;
    MetaMemberVariable* m_SourceMetaMemberVariable = nullptr;
    
    // 构造函数支持标志
    bool m_IsSupportConstructionFunctionOnlyBraceType = true;  // 是否支持构造函数使用 仅{}形式
    bool m_IsSupportConstructionFunctionConnectBraceType = true;  // 是否支持构造函数名称后边加{}形式
    bool m_IsSupportConstructionFunctionOnlyParType = true;  // 是否支持构造函数使用 仅()形式
    bool m_IsSupportInExpressUseStaticMetaMemeberFunction = true;  // 是否在成员支持静态函数的
    bool m_IsSupportInExpressUseStaticMetaVariable = true;  // 是否在成员中支持静态变量
    bool m_IsSupportInExpressUseCurrentClassNotStaticMemberMetaVariable = true;  // 是否支持在表达式中使用本类或父类中的非静态变量
    
    // 静态级别常量
    static int s_ConstLevel;
    static int s_IsHaveRetStaticLevel;
    static int s_NoHaveRetStaticLevel;
    static int s_DefineMetaTypeLevel;
    static int s_ExpressLevel;
    
    int m_ParseLevel = -1;

public:
    // 构造函数
    MetaMemberVariable();
    MetaMemberVariable(const MetaMemberVariable& mmv);
    MetaMemberVariable(MetaClass* mc, const std::string& name);
    MetaMemberVariable(MetaClass* mc, const std::string& name, MetaClass* defineTypeClass, MetaExpressNode* men = nullptr);
    MetaMemberVariable(MetaClass* mc, void* fmmv);  // FileMetaMemberVariable*
    virtual ~MetaMemberVariable() = default;

    // 属性访问器
    MetaMemberVariable* GetSourceMetaMemberVariable() const { return m_SourceMetaMemberVariable; }
    EFromType GetFromType() const { return m_FromType; }
    int GetIndex() const { return m_Index; }
    MetaExpressNode* GetExpress() const { return m_Express; }
    int GetParseLevel() const { return m_ParseLevel; }
    bool IsInnerDefine() const { return m_IsInnerDefine; }
    void* GetFileMetaMemeberVariable() const { return m_FileMetaMemeberVariable; }
    
    // 设置方法
    void SetSourceMetaMemberVariable(MetaMemberVariable* mmv) { m_SourceMetaMemberVariable = mmv; }
    void SetFromType(EFromType fromType) { m_FromType = fromType; }
    void SetIndex(int index) { m_Index = index; }
    void SetExpress(MetaExpressNode* express) { m_Express = express; }
    void SetParseLevel(int level) { m_ParseLevel = level; }
    void SetInnerDefine(bool innerDefine) { m_IsInnerDefine = innerDefine; }
    void SetFileMetaMemeberVariable(void* fmmv) { m_FileMetaMemeberVariable = fmmv; }
    
    // 虚方法重写
    virtual void ParseDefineMetaType() override;
    virtual int CalcParseLevelBeCall(int level);
    virtual void CalcParseLevel();
    virtual bool ParseMetaExpress() override;
    virtual std::string ToFormatString() const override;
    
    // 公共方法
    void CreateExpress();
    void CalcReturnType();
    int CompareTo(const MetaMemberVariable* mmv) const;
    MetaExpressNode* SimulateExpressRun(MetaExpressNode* node);
    MetaExpressNode* CreateExpressNodeInClassMetaVariable();
    void ParseChildMemberData();
    std::string ToTokenString() const;
    
    // 静态方法
    static int GetConstLevel() { return s_ConstLevel; }
    static int GetIsHaveRetStaticLevel() { return s_IsHaveRetStaticLevel; }
    static int GetNoHaveRetStaticLevel() { return s_NoHaveRetStaticLevel; }
    static int GetDefineMetaTypeLevel() { return s_DefineMetaTypeLevel; }
    static int GetExpressLevel() { return s_ExpressLevel; }
    
    static void SetConstLevel(int level) { s_ConstLevel = level; }
    static void SetIsHaveRetStaticLevel(int level) { s_IsHaveRetStaticLevel = level; }
    static void SetNoHaveRetStaticLevel(int level) { s_NoHaveRetStaticLevel = level; }
    static void SetDefineMetaTypeLevel(int level) { s_DefineMetaTypeLevel = level; }
    static void SetExpressLevel(int level) { s_ExpressLevel = level; }
};

} // namespace Core
} // namespace SimpleLanguage