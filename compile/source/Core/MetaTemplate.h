//****************************************************************************
//  File:      MetaTemplate.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: Meta template class
//****************************************************************************

#pragma once

#include "MetaBase.h"
#include "MetaClass.h"
#include "MetaType.h"
#include "../Compile/FileMeta/FileMetaCommon.h"
#include "ClassManager.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include <string>
#include <vector>

namespace SimpleLanguage {
namespace Core {

    class MetaMethodCall;

    class MetaTemplate : public MetaBase
    {
    public:
        // Properties
        int GetIndex() const { return m_Index; }
        bool IsInFunction() const { return m_IsInFunction; }
        MetaClass* GetExtendsMetaClass() const { return m_ExtendsMetaClass; }
        MetaClass* GetOwnerClass() const { return m_OwnerClass; }
        const std::vector<MetaMethodCall*>& GetBindConstructFunction() const { return m_BindConstructFunction; }

        // Constructors
        MetaTemplate(MetaClass* mc, Compile::FileMetaTemplateDefine* fmtd, int index);
        MetaTemplate(MetaClass* mc, const std::string& name);

        // Methods
        void SetIndex(int index);
        void ParseInConstraint();
        void AddBindConstructFunction(MetaMethodCall* mmc);
        void SetInConstraintMetaClass(MetaClass* mc);
        bool IsInConstraintMetaClass(MetaClass* mc);
        std::string ToFormatString() const override;

    protected:
        Compile::FileMetaTemplateDefine* m_FileMetaTemplateDefine = nullptr;
        MetaClass* m_OwnerClass = nullptr;
        MetaClass* m_ExtendsMetaClass = nullptr;
        bool m_IsInFunction = false;
        int m_Index = -1;
        std::vector<MetaMethodCall*> m_BindConstructFunction;
    };

    class TemplateBindMetaType
    {
    public:
        std::vector<MetaType*> bindMetaType;
    };

    class MetaGenTemplate
    {
    public:
        // Properties
        std::string GetName() const { return m_MetaTemplate != nullptr ? m_MetaTemplate->GetName() : ""; }
        MetaType* GetMetaType() const { return m_MetaType; }
        MetaTemplate* GetMetaTemplate() const { return m_MetaTemplate; }

        // Constructors
        MetaGenTemplate(MetaTemplate* mt);
        MetaGenTemplate(MetaTemplate* mt, MetaType* mtype);

        // Methods
        bool EqualWithMetaType(MetaType* mt);
        void SetMetaType(MetaType* mt);
        std::string ToDefineTypeString();
        std::string ToFormatString();

    private:
        MetaType* m_MetaType = nullptr;
        MetaTemplate* m_MetaTemplate = nullptr;
    };

    class MetaDefineTemplateCollection
    {
    public:
        // Properties
        const std::vector<MetaTemplate*>& GetMetaTemplateList() const { return m_MetaTemplateList; }
        int GetCount() const { return static_cast<int>(m_MetaTemplateList.size()); }

        // Constructors
        MetaDefineTemplateCollection();
        MetaDefineTemplateCollection(const MetaDefineTemplateCollection& mdtc);

        // Methods
        MetaTemplate* GetMetaDefineTemplateByName(const std::string& _name);
        bool IsEqualMetaDefineTemplateCollection(MetaDefineTemplateCollection* mpc);
        bool IsEqualMetaInputTemplateCollection(MetaInputTemplateCollection* mpc);
        virtual bool MatchMetaInputTemplate(MetaTemplate* a, MetaType* b);
        virtual bool MatchMetaDefineTemplate(MetaTemplate* a, MetaTemplate* b);
        virtual void AddMetaDefineTemplate(MetaTemplate* defineTemplate);
        virtual std::string ToFormatString();

    protected:
        std::vector<MetaTemplate*> m_MetaTemplateList;
    };

    // Forward declaration for MetaInputTemplateCollection
    class MetaInputTemplateCollection;

} // namespace Core
} // namespace SimpleLanguage