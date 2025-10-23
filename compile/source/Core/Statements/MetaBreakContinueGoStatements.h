//****************************************************************************
//  File:      MetaBreakContinueGoStatements.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include "MetaStatements.h"
#include <string>

namespace SimpleLanguage {
    namespace Compile {
        class FileMetaKeyOnlySyntax;
        class FileMetaKeyGotoLabelSyntax;
        class Token;
    }
    namespace Core {
        class MetaForStatements;
        class MetaWhileDoWhileStatements;
        class MetaFunction;
        class MetaBlockStatements;
        class LabelData;
    }
}

namespace SimpleLanguage {
namespace Core {

class MetaBreakStatements : public MetaStatements {
private:
    Compile::FileMetaKeyOnlySyntax* m_FileMetaKeyOnlySyntax = nullptr;
    MetaForStatements* m_ForStatements = nullptr;
    MetaWhileDoWhileStatements* m_WhileStatements = nullptr;

public:
    MetaBreakStatements(MetaBlockStatements* mbs, Compile::FileMetaKeyOnlySyntax* fmkos);
    virtual ~MetaBreakStatements() = default;
    
    virtual std::string ToFormatString() const override;
};

class MetaContinueStatements : public MetaStatements {
private:
    Compile::FileMetaKeyOnlySyntax* m_FileMetaKeyOnlySyntax = nullptr;
    MetaForStatements* m_ForStatements = nullptr;
    MetaWhileDoWhileStatements* m_WhileStatements = nullptr;

public:
    MetaContinueStatements(MetaBlockStatements* mbs, Compile::FileMetaKeyOnlySyntax* fmkos);
    virtual ~MetaContinueStatements() = default;
    
    virtual std::string ToFormatString() const override;
};

class MetaGotoLabelStatements : public MetaStatements {
public:
    bool isLabel = true;
    LabelData* labelData = nullptr;
    Compile::Token* labelToken = nullptr;
    Compile::FileMetaKeyGotoLabelSyntax* m_FileMetaKeyGotoLabelSyntax = nullptr;

public:
    MetaGotoLabelStatements(MetaBlockStatements* mbs, Compile::FileMetaKeyGotoLabelSyntax* labelSyntax);
    virtual ~MetaGotoLabelStatements() = default;
    
    virtual void SetDeep(int dp) override;
    virtual std::string ToFormatString() const override;
};

} // namespace Core
} // namespace SimpleLanguage