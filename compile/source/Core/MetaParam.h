//****************************************************************************
//  File:      MetaParam.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/5/30 12:00:00
//  Description: Meta params about info class!
//****************************************************************************

#pragma once

#include <string>
#include <vector>
#include <memory>

namespace SimpleLanguage {
    namespace Compile
    {
        class FileMetaParamterDefine;
        class FileInputParamNode;
        class FileMetaBracketTerm;
        class FileMetaParTerm;
    }
namespace Core {
    class MetaClass;
    class MetaFunction;
    class MetaBlockStatements;
    class MetaExpressNode;

// MetaInputParam class
class MetaInputParam {
public:
    MetaInputParam(Compile::FileInputParamNode* fipn, MetaClass* mc, MetaBlockStatements* mbs);
    MetaInputParam(MetaExpressNode* inputExpress);
    virtual ~MetaInputParam() = default;

    MetaExpressNode* GetExpress() const { return m_Express; }
    virtual void Parse();
    virtual void CalcReturnType();
    MetaClass* GetRetMetaClass();
    virtual std::string ToFormatString();
    std::string ToStatementString();

protected:
    Compile::FileInputParamNode* m_FileInputParamNode = nullptr;
    MetaExpressNode* m_Express = nullptr;
    MetaBlockStatements* m_OwnerMetaBlockStatements = nullptr;
    MetaClass* m_OwnerMetaClass = nullptr;
};

// MetaDefineParam class
class MetaDefineParam {
public:
    MetaDefineParam(const std::string& _name, MetaFunction* mf);
    MetaDefineParam(const MetaDefineParam& mdp);
    MetaDefineParam(MetaFunction* mf, Compile::FileMetaParamterDefine* fmp);
    virtual ~MetaDefineParam() = default;

    std::string GetName() const { return m_Name; }
    MetaVariable* GetMetaVariable() { return m_MetaVariable; }
    MetaExpressNode* GetExpressNode() const { return m_MetaExpressNode; }
    bool IsMust() const { return m_MetaExpressNode == nullptr; }
    bool IsExtendParams() const;

    void ParseMetaDefineType();
    void CreateExpress();
    virtual void Parse();
    bool EqualDefineMetaParam(MetaDefineParam* param);
    bool EqualsInputMetaParam(MetaInputParam* mip);
    bool EqualsName(const std::string& name);
    void SetMetaType(MetaType* mt);
    void CaleReturnType();
    virtual std::string ToFormatString();
    std::string ToString();

protected:
    bool m_IsFunctionTemplate = false;
    Compile::FileMetaParamterDefine* m_FileMetaParamter = nullptr;
    MetaExpressNode* m_MetaExpressNode = nullptr;
    MetaVariable* m_MetaVariable = nullptr;
    MetaFunction* m_OwnerMetaFunction = nullptr;
    std::string m_Name = "";
};

// MetaDefineParamCollection class
class MetaDefineParamCollection {
public:
    MetaDefineParamCollection();
    MetaDefineParamCollection(const MetaDefineParamCollection& mdpc);
    MetaDefineParamCollection(bool _isAllConst, bool _isCanCallFunction);
    virtual ~MetaDefineParamCollection() = default;

    bool IsExtendParams() const { return m_IsExtendParams; }
    int GetMaxParamCount() const { return static_cast<int>(m_MetaDefineParamList.size()); }
    const std::vector<MetaDefineParam*>& GetMetaDefineParamList() const { return m_MetaDefineParamList; }
    bool IsCanCallFunction() const { return m_IsCanCallFunction; }
    bool IsAllConst() const { return m_IsAllConst; }
    int GetMinParamCount() const { return m_MinParamCount; }
    bool IsHaveDefaultParamExpress() const { return m_IsHaveDefaultParamExpress; }

    void Clear();
    void SetOwnerMetaClass(MetaClass* ownerclass);
    MetaDefineParam* GetMetaDefineParamByName(const std::string& name);
    bool CheckDefineMetaParam(MetaDefineParam* a, MetaDefineParam* b);
    void AddMetaDefineParam(MetaDefineParam* metaMemberParam);
    bool IsEqualMetaInputParamCollection(MetaInputParamCollection* mpc);
    bool IsEqualMetaDefineParamCollection(MetaDefineParamCollection* mdpc);
    std::string ToParamTypeName();
    std::string ToFormatString();

private:
    bool m_IsCanCallFunction = true;
    bool m_IsExtendParams = false;
    int m_MinParamCount = 0;
    bool m_IsAllConst = false;
    bool m_IsHaveDefaultParamExpress = false;
    std::vector<MetaDefineParam*> m_MetaDefineParamList;
};

// MetaInputParamCollection class
class MetaInputParamCollection {
public:
    MetaInputParamCollection(MetaClass* mc, MetaBlockStatements* mbs);
    MetaInputParamCollection(Compile::FileMetaParTerm* fmpt, MetaClass* mc, MetaBlockStatements* mbs);
    virtual ~MetaInputParamCollection() = default;

    const std::vector<MetaInputParam*>& GetMetaInputParamList() const { return m_MetaInputParamList; }
    int GetCount() const { return static_cast<int>(m_MetaInputParamList.size()); }

    void Clear();
    static bool CheckInputMetaParam(MetaDefineParam* a, MetaInputParam* b);
    void ParseList(const std::vector<Compile::FileInputParamNode*>& splitList);
    void AddMetaInputParam(MetaInputParam* mip);
    void CaleReturnType();
    MetaClass* GetMaxLevelMetaClassType();
    std::string ToFormatString();

private:
    MetaClass* m_OwnerMetaClass = nullptr;
    MetaBlockStatements* m_MetaBlockStatements = nullptr;
    std::vector<MetaInputParam*> m_MetaInputParamList;
};

// MetaInputTemplateCollection class
class MetaInputTemplateCollection {
public:
    MetaInputTemplateCollection();
    virtual ~MetaInputTemplateCollection() = default;

    bool IsTemplateName() const { return m_IsTemplateName; }
    const std::vector<MetaType*>& GetMetaTemplateParamsList() const { return m_MetaTemplateParamsList; }

    std::vector<MetaClass*> GetMetaClassList(bool& isAllMetaClass);
    void AddMetaTemplateParamsList(MetaType* mp);
    std::vector<MetaClass*> GetMetaClassParamsList();
    MetaClass* GetMaxLevelMetaClassType();
    std::string ToFormatString();

private:
    std::vector<MetaType*> m_MetaTemplateParamsList;
    bool m_IsTemplateName = false;
};

// MetaInputArrayCollection class
class MetaInputArrayCollection {
public:
    MetaInputArrayCollection(Compile::FileMetaBracketTerm* fmbt);
    virtual ~MetaInputArrayCollection() = default;
};

} // namespace Core
} // namespace SimpleLanguage
