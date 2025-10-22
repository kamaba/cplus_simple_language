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
    Code = 1,         // д��.s����
    Manual = 2,       // �ֶ���ͨ��c#����
    CodeAndManual = 3,
    CSharp = 4,       // ͨ��c#��dll�ļ��������Ǳ�����Ĵ����ʶ��
    ManualAndCSharp = 6,  // �ֶ�ע���c#������߼�����
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
    
    // ���캯��֧�ֱ�־
    bool m_IsSupportConstructionFunctionOnlyBraceType = true;  // �Ƿ�֧�ֹ��캯��ʹ�� ��{}��ʽ
    bool m_IsSupportConstructionFunctionConnectBraceType = true;  // �Ƿ�֧�ֹ��캯�����ƺ�߼�{}��ʽ
    bool m_IsSupportConstructionFunctionOnlyParType = true;  // �Ƿ�֧�ֹ��캯��ʹ�� ��()��ʽ
    bool m_IsSupportInExpressUseStaticMetaMemeberFunction = true;  // �Ƿ��ڳ�Ա֧�־�̬������
    bool m_IsSupportInExpressUseStaticMetaVariable = true;  // �Ƿ��ڳ�Ա��֧�־�̬����
    bool m_IsSupportInExpressUseCurrentClassNotStaticMemberMetaVariable = true;  // �Ƿ�֧���ڱ��ʽ��ʹ�ñ�������еķǾ�̬����
    
    // ��̬������
    static int s_ConstLevel;
    static int s_IsHaveRetStaticLevel;
    static int s_NoHaveRetStaticLevel;
    static int s_DefineMetaTypeLevel;
    static int s_ExpressLevel;
    
    int m_ParseLevel = -1;

public:
    // ���캯��
    MetaMemberVariable();
    MetaMemberVariable(const MetaMemberVariable& mmv);
    MetaMemberVariable(MetaClass* mc, const std::string& name);
    MetaMemberVariable(MetaClass* mc, const std::string& name, MetaClass* defineTypeClass, MetaExpressNode* men = nullptr);
    MetaMemberVariable(MetaClass* mc, void* fmmv);  // FileMetaMemberVariable*
    virtual ~MetaMemberVariable() = default;

    // ���Է�����
    MetaMemberVariable* GetSourceMetaMemberVariable() const { return m_SourceMetaMemberVariable; }
    EFromType GetFromType() const { return m_FromType; }
    int GetIndex() const { return m_Index; }
    MetaExpressNode* GetExpress() const { return m_Express; }
    int GetParseLevel() const { return m_ParseLevel; }
    bool IsInnerDefine() const { return m_IsInnerDefine; }
    void* GetFileMetaMemeberVariable() const { return m_FileMetaMemeberVariable; }
    
    // ���÷���
    void SetSourceMetaMemberVariable(MetaMemberVariable* mmv) { m_SourceMetaMemberVariable = mmv; }
    void SetFromType(EFromType fromType) { m_FromType = fromType; }
    void SetIndex(int index) { m_Index = index; }
    void SetExpress(MetaExpressNode* express) { m_Express = express; }
    void SetParseLevel(int level) { m_ParseLevel = level; }
    void SetInnerDefine(bool innerDefine) { m_IsInnerDefine = innerDefine; }
    void SetFileMetaMemeberVariable(void* fmmv) { m_FileMetaMemeberVariable = fmmv; }
    
    // �鷽����д
    virtual void ParseDefineMetaType() override;
    virtual int CalcParseLevelBeCall(int level);
    virtual void CalcParseLevel();
    virtual bool ParseMetaExpress() override;
    virtual std::string ToFormatString() const override;
    
    // ��������
    void CreateExpress();
    void CalcReturnType();
    int CompareTo(const MetaMemberVariable* mmv) const;
    MetaExpressNode* SimulateExpressRun(MetaExpressNode* node);
    MetaExpressNode* CreateExpressNodeInClassMetaVariable();
    void ParseChildMemberData();
    std::string ToTokenString() const;
    
    // ��̬����
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