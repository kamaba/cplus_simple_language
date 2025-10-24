//****************************************************************************
//  File:      MetaExpressNewObject.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description: 
//****************************************************************************

#include "MetaExpressNewObject.h"
#include "MetaExpressCallLink.h"
#include "MetaExpressConst.h"
#include "../Statements/MetaBlockStatements.h"
#include "../BaseMetaClass/CoreMetaClassManager.h"
#include "../MetaDynamicClass.h"
#include "../MetaParam.h"
#include "../MetaData.h"
#include "../MetaType.h"
#include "../MetaMemberData.h"
#include "../MetaMemberVariable.h"
#include "../ClassManager.h"
#include "../ExpressManager.h"
#include "MetaExpressBase.h"
#include "../../Compile/FileMeta/FileMetaBase.h"
#include "../../Compile/FileMeta/FileMetaClass.h"
#include "../../Compile/FileMeta/FileMetaExpress.h"
#include "../../Compile/FileMeta/FileMetaCommon.h"
#include "../../Compile/FileMeta/FileMetaSyntax.h"
#include "../../Debug/Log.h"
#include <sstream>
#include <algorithm>

using namespace SimpleLanguage::Compile;
using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Core {

// MetaBraceAssignStatements implementation
MetaBraceAssignStatements::MetaBraceAssignStatements(MetaBlockStatements* mbs, MetaType* mt, Compile::FileMetaCallLink* fmcl) {
    m_OwnerMetaBlockStatements = mbs;
    if (fmcl != nullptr) {
        m_MetaExpress = new MetaCallLinkExpressNode(fmcl, mt->GetMetaClass(), mbs, nullptr);
        AllowUseSettings* auc = new AllowUseSettings();
        auc->SetUseNotConst(false);
        auc->SetUseNotStatic(false);
        m_MetaExpress->Parse(auc);
        m_MetaExpress->CalcReturnType();
    }
}

MetaBraceAssignStatements::MetaBraceAssignStatements(MetaBlockStatements* mbs, MetaType* mc, MetaExpressNode* men) {
    m_OwnerMetaBlockStatements = mbs;
    m_MetaExpress = men;
}

MetaBraceAssignStatements::MetaBraceAssignStatements(MetaBlockStatements* mbs, MetaExpressNode* men, MetaMemberVariable* mmv) {
    m_OwnerMetaBlockStatements = mbs;
    m_MetaExpress = men;
    m_MetaMemberVariable = mmv;
}

MetaBraceAssignStatements::MetaBraceAssignStatements(MetaBlockStatements* mbs, MetaType* mt, Compile::FileMetaOpAssignSyntax* fmos) {
    m_FileMetaOpAssignSyntax = fmos;
    m_OwnerMetaBlockStatements = mbs;
    if (fmos != nullptr) {
        m_AssignToken = fmos->GetAssignToken();
        if (fmos->GetVariableRef()->IsOnlyName()) {
            m_DefineName = fmos->GetVariableRef()->GetName();
            if (mt->IsDynamicClass()) {
                m_MetaMemberVariable = new MetaMemberVariable(nullptr, m_DefineName);
            } else if (mt->IsDynamicData()) {
                m_MetaMemberData = new MetaMemberData(mt->GetMetaClass(), fmos);
                m_MetaMemberData->SetOwnerBlockstatements(m_OwnerMetaBlockStatements);
                m_MetaMemberData->ParseDefineMetaType();
                m_MetaExpress = m_MetaMemberData->GetExpressNode();
                m_MetaMemberData->ParseMetaExpress();
                m_MetaMemberData->ParseChildMemberData();
            } else {
                if (mt->IsData()) {
                    m_MetaMemberData = dynamic_cast<MetaData*>(mt->GetMetaClass())->GetMemberDataByName(m_DefineName);
                    if (m_MetaMemberData == nullptr) {
                        Log::Write("Error 在类" + mt->GetMetaClass()->GetAllClassName() + "函数: " + mbs->GetOwnerMetaFunction()->GetName()
                            + " 没有找到: 类" + mt->GetMetaClass()->GetAllClassName() + " 变量:" + m_DefineName);
                    }
                    m_MetaExpress = CreateExpressNodeInNewObjectStatements(m_MetaMemberData, m_OwnerMetaBlockStatements, m_FileMetaOpAssignSyntax->GetExpress());
                } else if (mt->IsEnum()) {
                    Log::Write("-----------------------------------Enum-------------------------");
                } else {
                    m_MetaMemberVariable = mt->GetMetaClass()->GetMetaMemberVariableByName(m_DefineName);
                    if (m_MetaMemberVariable == nullptr) {
                        Log::Write("Error 在类" + mt->GetMetaClass()->GetAllClassName() + "函数: " + mbs->GetOwnerMetaFunction()->GetName()
                            + " 没有找到: 类" + mt->GetMetaClass()->GetAllClassName() + " 变量:" + m_DefineName);
                    }
                    m_MetaExpress = CreateExpressNodeInNewObjectStatements(m_MetaMemberVariable, m_OwnerMetaBlockStatements, m_FileMetaOpAssignSyntax->GetExpress());
                }
            }
        } else {
            Log::Write("Error 在类" + mbs->GetOwnerMetaClass()->GetAllClassName() + "函数: " + mbs->GetOwnerMetaFunction()->GetName()
                + " 语句: " + fmos->GetVariableRef()->ToTokenString());
        }
    }
}

MetaBraceAssignStatements::MetaBraceAssignStatements(MetaBlockStatements* mbs, MetaType* mt, Compile::FileMetaDefineVariableSyntax* fmdvs) {
    m_FileMetaDefineVariableSyntax = fmdvs;
    m_OwnerMetaBlockStatements = mbs;
    if (fmdvs != nullptr) {
        m_AssignToken = fmdvs->GetAssignToken();
        m_DefineName = m_FileMetaDefineVariableSyntax->GetName();
        
        auto fmcd = m_FileMetaDefineVariableSyntax->GetFileMetaClassDefine();
        auto getMC = ClassManager::GetInstance().GetMetaClassAndRegisterExptendTemplateClassInstance(mbs->GetOwnerMetaClass(), fmcd);
        auto mdt = new MetaType(getMC);
        m_MetaMemberVariable = new MetaMemberVariable(nullptr, m_DefineName, mdt->GetMetaClass());
        
        auto fileExpress = m_FileMetaDefineVariableSyntax->GetExpress();
        m_MetaExpress = CreateExpressNodeInNewObjectStatements(m_MetaMemberVariable, m_OwnerMetaBlockStatements, fileExpress);
    }
}

MetaClass* MetaBraceAssignStatements::GetRetMetaClass() {
    if (m_MetaMemberVariable != nullptr) {
        return m_MetaMemberVariable->GetOwnerMetaClass();
    }
    if (m_MetaExpress != nullptr) {
        return m_MetaExpress->GetReturnMetaClass();
    }
    return nullptr;
}

void MetaBraceAssignStatements::SetDefineName(const std::string& definaname) {
    m_DefineName = definaname;
}

bool MetaBraceAssignStatements::CalcReturnType() {
    if (m_MetaExpress != nullptr) {
        m_MetaExpress->CalcReturnType();
        
        if (m_MetaMemberVariable != nullptr) {
            MetaClass* retMetaClass = m_MetaMemberVariable->GetMetaDefineType()->GetMetaClass();
            MetaClass* ownerMetaClass = m_MetaMemberVariable->GetOwnerMetaClass();
            
            m_MetaMemberVariable->SetMetaDefineType(m_MetaExpress->GetReturnMetaDefineType());
        } else if (m_MetaMemberData != nullptr) {
            MetaClass* retMetaClass = m_MetaMemberData->GetMetaDefineType()->GetMetaClass();
            MetaClass* ownerMetaClass = m_MetaMemberData->GetOwnerMetaClass();
            
            m_MetaMemberData->SetMetaDefineType(m_MetaExpress->GetReturnMetaDefineType());
        }
    } else {
        Log::Write("使用{}赋值，表达式不允许为空!!");
    }
    return true;
}

MetaExpressNode* MetaBraceAssignStatements::CreateExpressNodeInNewObjectStatements(MetaVariable* mv, MetaBlockStatements* mbs, Compile::FileMetaBaseTerm* fme) {
    if (fme == nullptr) {
        Log::Write("Error !!!!!!!!!!");
        return nullptr;
    }
    
    Compile::FileMetaBaseTerm* curFMBT = fme;
    if (fme->GetLeft() == nullptr && fme->GetRight() == nullptr) {
        if (auto fmte = dynamic_cast<Compile::FileMetaTermExpress*>(fme)) {
            curFMBT = fmte->GetRoot();
        }
    }
    
    MetaClass* mc = mbs->GetOwnerMetaClass();
    MetaClass* selfMC = mv->GetMetaDefineType()->GetMetaClass();
    
    if (auto constValueTerm = dynamic_cast<Compile::FileMetaConstValueTerm*>(curFMBT)) {
        MetaExpressNode* men = new MetaConstExpressNode(constValueTerm);
        return men;
    } else if (auto callTerm = dynamic_cast<Compile::FileMetaCallTerm*>(curFMBT)) {
        MetaCallLinkExpressNode* clen = new MetaCallLinkExpressNode(callTerm->GetCallLink(), mc, mbs, nullptr);
        AllowUseSettings* auc = new AllowUseSettings();
        auc->SetUseNotConst(false);
        auc->SetUseNotStatic(false);
        auc->SetCallConstructFunction(true);
        auc->SetCallFunction(true);
        clen->Parse(auc);
        return clen;
    } else if (auto fmbt = dynamic_cast<Compile::FileMetaBraceTerm*>(curFMBT)) {
        MetaType* mt = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
        MetaNewObjectExpressNode* mnoe = new MetaNewObjectExpressNode(fmbt, mt, mc, mbs, mv);
        return mnoe;
    } else if (auto fmbt = dynamic_cast<Compile::FileMetaBracketTerm*>(curFMBT)) {
        if (dynamic_cast<MetaMemberData*>(mv) != nullptr) {
            MetaType* mt = new MetaType(CoreMetaClassManager::GetInstance().GetArrayMetaClass());
            MetaNewObjectExpressNode* mnoe = new MetaNewObjectExpressNode(fmbt, mt->GetMetaClass(), mbs, mv);
            return mnoe;
        } else {
            Log::Write("Error 只有在data varname = {} 支持 { cha1 = [] } 的格式,其它的表达式中不支持");
        }
    } else {
        Log::Write("Error 暂不支持该类型的在NewObject中的解析!!");
    }
    return nullptr;
}

std::string MetaBraceAssignStatements::ToFormatString() {
    std::ostringstream sb;
    
    if (m_MetaMemberVariable != nullptr) {
        sb << m_MetaMemberVariable->GetName();
    }
    if (m_AssignToken != nullptr) {
        sb << m_AssignToken->GetLexemeString();
    }
    if (m_MetaExpress != nullptr) {
        sb << m_MetaExpress->ToFormatString();
    }
    
    return sb.str();
}

// MetaBraceOrBracketStatementsContent implementation
MetaBraceOrBracketStatementsContent::MetaBraceOrBracketStatementsContent(MetaBlockStatements* mbs, MetaClass* mc) {
    m_OwnerMetaBlockStatements = mbs;
    m_OwnerMetaClass = mc;
    m_DefineMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
}

MetaBraceOrBracketStatementsContent::MetaBraceOrBracketStatementsContent(Compile::FileMetaBraceTerm* fileMetaBraceTerm, MetaBlockStatements* mbs, MetaClass* mc) {
    m_FileMetaBraceTerm = fileMetaBraceTerm;
    m_OwnerMetaBlockStatements = mbs;
    m_OwnerMetaClass = mc;
    m_DefineMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
}

MetaBraceOrBracketStatementsContent::MetaBraceOrBracketStatementsContent(Compile::FileMetaBraceTerm* fileMetaBraceTerm, MetaBlockStatements* mbs, MetaClass* mc, MetaVariable* parentMt) {
    m_FileMetaBraceTerm = fileMetaBraceTerm;
    m_OwnerMetaBlockStatements = mbs;
    m_OwnerMetaClass = mc;
    m_EqualMetaVariable = parentMt;
    m_DefineMetaType = new MetaType(CoreMetaClassManager::GetInstance().GetObjectMetaClass());
}

MetaBraceOrBracketStatementsContent::MetaBraceOrBracketStatementsContent(Compile::FileMetaBracketTerm* fileMetaBracketTerm, MetaBlockStatements* mbs, MetaClass* mc, MetaVariable* parentMt) {
    m_FileMetaBracketTerm = fileMetaBracketTerm;
    m_OwnerMetaBlockStatements = mbs;
    m_OwnerMetaClass = mc;
    m_EqualMetaVariable = parentMt;
}

void MetaBraceOrBracketStatementsContent::SetMetaType(MetaType* mt) {
    m_DefineMetaType = mt;
}

void MetaBraceOrBracketStatementsContent::Parse() {
    if (m_FileMetaBracketTerm != nullptr) {
        std::vector<FileInputParamNode*> list;
        auto splitList = m_FileMetaBracketTerm->SplitParamList();
        for (size_t i = 0; i < splitList.size(); i++) {
            auto fas = splitList[i];
            
            CreateExpressParam* cep = new CreateExpressParam();
            cep->SetOwnerMetaClass(m_OwnerMetaClass);
            cep->SetOwnerMBS(m_OwnerMetaBlockStatements);
            cep->SetMetaType(new MetaType(CoreMetaClassManager::GetInstance().GetInt32MetaClass()));
            cep->SetFme(fas);
            cep->SetEqualMetaVariable(m_EqualMetaVariable);
            MetaExpressNode* men = ExpressManager::GetInstance().CreateExpressNode(cep);
            MetaBraceAssignStatements* mas = new MetaBraceAssignStatements(m_OwnerMetaBlockStatements, new MetaType(m_OwnerMetaClass), men);
            mas->CalcReturnType();
            m_AssignStatementsList.push_back(mas);
        }
        m_ContentType = EStatementsContentType::ArrayValue;
    }
    
    if (m_FileMetaBraceTerm != nullptr) {
        if (m_FileMetaBraceTerm->GetFileMetaCallLinkList().size() > 0) {
            Log::Write("-------------------------------------------------------------------");
            for (size_t i = 0; i < m_FileMetaBraceTerm->GetFileMetaCallLinkList().size(); i++) {
                auto fas = m_FileMetaBraceTerm->GetFileMetaCallLinkList()[i];
                MetaBraceAssignStatements* mas = new MetaBraceAssignStatements(m_OwnerMetaBlockStatements, new MetaType(m_OwnerMetaClass), fas);
                m_AssignStatementsList.push_back(mas);
                m_ContentType = EStatementsContentType::ClassValueAssign;
            }
        }
        if (m_FileMetaBraceTerm->GetFileMetaAssignSyntaxList().size() > 0) {
            if (m_DefineMetaType->IsData()) {
                if (m_DefineMetaType->IsDynamicData()) {
                    std::string anname = "DynamicData_";
                    if (m_EqualMetaVariable != nullptr) {
                        anname += m_EqualMetaVariable->GetName() + "_";
                    }
                    if (m_FileMetaBraceTerm != nullptr) {
                        anname += m_FileMetaBraceTerm->GetToken()->GetPath() + "_" + 
                                std::to_string(m_FileMetaBraceTerm->GetToken()->GetSourceBeginLine()) + "_" + 
                                std::to_string(reinterpret_cast<uintptr_t>(this));
                    }
                    
                    m_NewTempMetaData = new MetaData(anname, false, false, true);
                    if (m_EqualMetaVariable != nullptr && m_EqualMetaVariable->GetPingToken() != nullptr) {
                        m_NewTempMetaData->AddPingToken(m_EqualMetaVariable->GetPingToken());
                    }
                    m_NewTempMetaData->AddPingToken(m_FileMetaBraceTerm->GetToken());
                    m_DefineMetaType = new MetaType(m_NewTempMetaData);
                    
                    for (size_t i = 0; i < m_FileMetaBraceTerm->GetFileMetaAssignSyntaxList().size(); i++) {
                        auto fas = m_FileMetaBraceTerm->GetFileMetaAssignSyntaxList()[i];
                        auto foas = dynamic_cast<Compile::FileMetaOpAssignSyntax*>(fas);
                        auto fdvs = dynamic_cast<Compile::FileMetaDefineVariableSyntax*>(fas);
                        MetaBraceAssignStatements* mas = nullptr;
                        
                        if (foas != nullptr) {
                            foas->GetExpress()->BuildAST();
                            mas = new MetaBraceAssignStatements(m_OwnerMetaBlockStatements, m_DefineMetaType, foas);
                        } else if (fdvs != nullptr) {
                            Log::Write("Error 不允许的表达式类形在 a = { int a; } 这种的形式里边");
                        } else {
                            Log::Write("Error 不允许的表达式类形在 a = {} 这种的形式里边");
                        }
                        if (mas != nullptr) {
                            mas->CalcReturnType();
                            m_AssignStatementsList.push_back(mas);
                        }
                    }
                    
                    for (size_t i = 0; i < m_AssignStatementsList.size(); i++) {
                        auto mmv = m_AssignStatementsList[i]->GetMetaMemberData();
                        m_NewTempMetaData->AddMetaMemberData(mmv);
                    }
                    
                    MetaData* retClass = ClassManager::GetInstance().FindMetaData(m_NewTempMetaData);
                    if (retClass == nullptr) {
                        ClassManager::GetInstance().AddMetaData(m_NewTempMetaData);
                        retClass = m_NewTempMetaData;
                    }
                    m_NewMetaData = retClass;
                    
                    m_DefineMetaType->SetMetaClass(m_NewMetaData);
                    m_DefineMetaType->SetTemplateMetaClass(m_NewMetaData);
                    m_ContentType = EStatementsContentType::DynamicData;
                    if (m_EqualMetaVariable != nullptr) {
                        m_EqualMetaVariable->SetMetaDefineType(m_DefineMetaType);
                    }
                } else {
                    for (size_t i = 0; i < m_FileMetaBraceTerm->GetFileMetaAssignSyntaxList().size(); i++) {
                        auto fas = m_FileMetaBraceTerm->GetFileMetaAssignSyntaxList()[i];
                        auto foas = dynamic_cast<Compile::FileMetaOpAssignSyntax*>(fas);
                        if (foas != nullptr) {
                            foas->GetExpress()->BuildAST();
                            MetaBraceAssignStatements* mas = new MetaBraceAssignStatements(m_OwnerMetaBlockStatements, m_DefineMetaType, foas);
                            m_AssignStatementsList.push_back(mas);
                        } else {
                            Log::Write("Error 该处应该是使用赋值，不能有其它表达式!!" + (fas->GetToken() != nullptr ? fas->GetToken()->ToLexemeAllString() : ""));
                            continue;
                        }
                    }
                    m_ContentType = EStatementsContentType::DataValueAssign;
                }
            } else {
                if (m_DefineMetaType->IsDynamicClass()) {
                    MetaDynamicClass* anonClass = new MetaDynamicClass("DynamicClass__" + std::to_string(reinterpret_cast<uintptr_t>(this)));
                    
                    for (size_t i = 0; i < m_FileMetaBraceTerm->GetFileMetaAssignSyntaxList().size(); i++) {
                        auto fas = m_FileMetaBraceTerm->GetFileMetaAssignSyntaxList()[i];
                        auto foas = dynamic_cast<Compile::FileMetaOpAssignSyntax*>(fas);
                        auto fdvs = dynamic_cast<Compile::FileMetaDefineVariableSyntax*>(fas);
                        MetaBraceAssignStatements* mas = nullptr;
                        
                        if (foas != nullptr) {
                            foas->GetExpress()->BuildAST();
                            mas = new MetaBraceAssignStatements(m_OwnerMetaBlockStatements, m_DefineMetaType, foas);
                        } else if (fdvs != nullptr) {
                            fdvs->GetExpress()->BuildAST();
                            mas = new MetaBraceAssignStatements(m_OwnerMetaBlockStatements, m_DefineMetaType, fdvs);
                        }
                        if (mas != nullptr) {
                            mas->CalcReturnType();
                            m_AssignStatementsList.push_back(mas);
                        }
                    }
                    
                    for (size_t i = 0; i < m_AssignStatementsList.size(); i++) {
                        auto mmv = m_AssignStatementsList[i]->GetMetaMemberVariable();
                        anonClass->AddMetaMemberVariable(mmv, false);
                    }
                    
                    MetaClass* retClass = ClassManager::GetInstance().FindDynamicClass(anonClass);
                    if (retClass == nullptr) {
                        for (size_t i = 0; i < m_AssignStatementsList.size(); i++) {
                            auto mmv = m_AssignStatementsList[i]->GetMetaMemberVariable();
                            mmv->SetOwnerMetaClass(retClass);
                        }
                        ClassManager::GetInstance().AddDynamicClass(anonClass);
                        retClass = anonClass;
                    } else {
                        auto list = anonClass->GetAllMetaMemberVariableList();
                        if (list.size() == m_AssignStatementsList.size()) {
                            // Handle same count case
                        }
                    }
                    m_DefineMetaType = new MetaType(retClass);
                    m_ContentType = EStatementsContentType::DynamicClass;
                } else {
                    for (size_t i = 0; i < m_FileMetaBraceTerm->GetFileMetaAssignSyntaxList().size(); i++) {
                        auto fas = m_FileMetaBraceTerm->GetFileMetaAssignSyntaxList()[i];
                        auto foas = dynamic_cast<Compile::FileMetaOpAssignSyntax*>(fas);
                        if (foas != nullptr) {
                            foas->GetExpress()->BuildAST();
                            MetaBraceAssignStatements* mas = new MetaBraceAssignStatements(m_OwnerMetaBlockStatements, m_DefineMetaType, foas);
                            m_AssignStatementsList.push_back(mas);
                        } else {
                            Log::Write("Error 该处应该是使用赋值，不能有其它表达式!!" + (fas->GetToken() != nullptr ? fas->GetToken()->ToLexemeAllString() : ""));
                            continue;
                        }
                    }
                    m_ContentType = EStatementsContentType::ClassValueAssign;
                }
            }
        }
    }
}

MetaClass* MetaBraceOrBracketStatementsContent::GetMaxLevelMetaClassType() {
    MetaClass* mc = CoreMetaClassManager::GetInstance().GetObjectMetaClass();
    bool isAllSame = true;
    
    for (size_t i = 0; i < m_AssignStatementsList.size() - 1; i++) {
        MetaBraceAssignStatements* cmc = m_AssignStatementsList[i];
        MetaBraceAssignStatements* nmc = m_AssignStatementsList[i + 1];
        
        if (cmc->GetOpLevel() == nmc->GetOpLevel()) {
            if (cmc->GetOpLevel() == 10) {
                auto cur = cmc->GetRetMetaClass();
                auto next = nmc->GetRetMetaClass();
                auto relation = ClassManager::GetInstance().ValidateClassRelationByMetaClass(cur, next);
                
                if (relation == ClassManager::EClassRelation::Same || relation == ClassManager::EClassRelation::Child) {
                    mc = next;
                } else if (relation == ClassManager::EClassRelation::Parent) {
                    mc = cur;
                } else {
                    isAllSame = false;
                    break;
                }
            } else {
                mc = cmc->GetRetMetaClass();
                isAllSame = true;
            }
        } else {
            if (cmc->GetOpLevel() > nmc->GetOpLevel()) {
                mc = cmc->GetRetMetaClass();
            } else {
                mc = nmc->GetRetMetaClass();
            }
        }
    }
    return mc;
}

std::string MetaBraceOrBracketStatementsContent::ToFormatString() {
    std::ostringstream sb;
    
    if (m_AssignStatementsList.size() > 0) {
        sb << "{";
        for (size_t i = 0; i < m_AssignStatementsList.size(); i++) {
            auto mas = m_AssignStatementsList[i];
            sb << mas->ToFormatString();
            if (i < m_AssignStatementsList.size() - 1) {
                sb << ", ";
            }
        }
        sb << "}";
    }
    return sb.str();
}

// MetaNewObjectExpressNode implementation
MetaNewObjectExpressNode::MetaNewObjectExpressNode(MetaClass* ownermc, const std::vector<MetaDynamicClass*>& list) {
    m_OwnerMetaClass = ownermc;
    m_OwnerMetaBlockStatements = nullptr;
    m_MetaBraceOrBracketStatementsContent = nullptr;
    
    auto metaInputTemplateCollection = new MetaInputTemplateCollection();
    m_MetaDefineType = new MetaType(CoreMetaClassManager::GetInstance().GetArrayMetaClass(), nullptr, metaInputTemplateCollection);
}

MetaNewObjectExpressNode::MetaNewObjectExpressNode(Compile::FileMetaConstValueTerm* arrayLinkToken, MetaClass* ownerMC, MetaBlockStatements* mbs) {
    m_FileMetaConstValueTerm = arrayLinkToken;
    m_OwnerMetaClass = ownerMC;
    m_OwnerMetaBlockStatements = mbs;
    
    auto metaInputTemplateCollection = new MetaInputTemplateCollection();
    MetaType* mitp = new MetaType(CoreMetaClassManager::GetInstance().GetInt32MetaClass());
    metaInputTemplateCollection->AddMetaTemplateParamsList(mitp);
    
    m_MetaDefineType = new MetaType(CoreMetaClassManager::GetInstance().GetRangeMetaClass(), nullptr, metaInputTemplateCollection);
    
    MetaInputParamCollection* mdpc = new MetaInputParamCollection(ownerMC, mbs);
    std::string name = arrayLinkToken->GetName();
    size_t pos = name.find("..");
    if (pos != std::string::npos) {
        std::string arr0 = name.substr(0, pos);
        std::string arr1 = name.substr(pos + 2);
        
        int val0 = 0, val1 = 0;
        if (std::stoi(arr0, &val0)) {
            MetaConstExpressNode* mcen1 = new MetaConstExpressNode(EType::Int32, val0);
            MetaInputParam* mip = new MetaInputParam(mcen1);
            mdpc->AddMetaInputParam(mip);
        }
        
        if (std::stoi(arr1, &val1)) {
            MetaConstExpressNode* mcen2 = new MetaConstExpressNode(EType::Int32, val1);
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

MetaNewObjectExpressNode::MetaNewObjectExpressNode(MetaType* mt, MetaClass* ownerMC, MetaBlockStatements* mbs) {
    m_OwnerMetaClass = ownerMC;
    m_OwnerMetaBlockStatements = mbs;
    m_MetaDefineType = new MetaType(mt);
    Init();
    m_MetaConstructFunctionCall = new MetaMethodCall(mt->GetMetaClass(), mt->GetTemplateMetaTypeList(), m_OwnerMetaBlockStatements->GetOwnerMetaFunction(),
        nullptr, nullptr, nullptr, nullptr);
}

MetaNewObjectExpressNode::MetaNewObjectExpressNode(Compile::FileMetaCallTerm* fmct, MetaCallLink* mcl, MetaType* mt, MetaClass* ownerMC, MetaBlockStatements* mbs, MetaMethodCall* mmf) {
    m_FileMetaCallTerm = fmct;
    m_OwnerMetaClass = ownerMC;
    m_OwnerMetaBlockStatements = mbs;
    m_MetaConstructFunctionCall = mmf;
    m_MetaDefineType = new MetaType(mt);
    auto fmcn = mcl->GetFinalCallNode();
    
    bool needByFileMetaParTermSetTemplate = false;
    MetaInputTemplateCollection* mitc = new MetaInputTemplateCollection();
    MetaClass* createMC = nullptr;
    
    if (!m_MetaDefineType->IsDefineMetaClass()) {
        if (fmcn->GetVisitType() == MetaVisitNode::EVisitType::New) {
            m_MetaDefineType->SetMetaClass(fmcn->GetMethodCall()->GetFunction()->GetOwnerMetaClass());
        }
    } else if (fmcn->GetVisitType() == MetaVisitNode::EVisitType::MethodCall) {
        createMC = mmf->GetFunction()->GetOwnerMetaClass();
        m_MetaDefineType->SetMetaClass(createMC);
        if (createMC->GetMetaTemplateList().size() > 0) {
            // Handle template parameters
        }
    }
    
    auto list = fmct->GetCallLink()->GetCallNodeList();
    if (list.size() > 0) {
        auto listfinalNode = list[list.size() - 1];
        auto fmbt = listfinalNode->GetFileMetaBraceTerm();
        if (fmbt != nullptr) {
            m_MetaBraceOrBracketStatementsContent = new MetaBraceOrBracketStatementsContent(fmbt, m_OwnerMetaBlockStatements, m_OwnerMetaClass);
            m_MetaBraceOrBracketStatementsContent->SetMetaType(m_MetaDefineType);
            m_MetaBraceOrBracketStatementsContent->Parse();
            if (fmbt->IsArray()) {
                auto metaInputTemplateCollection = new MetaInputTemplateCollection();
                MetaClass* mc = m_MetaBraceOrBracketStatementsContent->GetMaxLevelMetaClassType();
                metaInputTemplateCollection->AddMetaTemplateParamsList(new MetaType(mc));
                
                m_MetaDefineType->SetTemplateMetaClass(CoreMetaClassManager::GetInstance().GetArrayMetaClass());
                m_MetaDefineType->UpdateMetaClassByRawMetaClassAndInputTemplateCollection();
            }
        }
        
        auto fmpt = listfinalNode->GetFileMetaParTerm();
        if (fmpt != nullptr) {
            m_FileMetaParTerm = fmpt;
            if (needByFileMetaParTermSetTemplate) {
                Log::Write("Error 待测试!!!");
                std::vector<MetaClass*> mtList;
                MetaInputParamCollection* mipc = new MetaInputParamCollection(m_FileMetaParTerm, ownerMC, mbs);
                for (int i = 0; i < mipc->GetCount(); i++) {
                    auto mp = mipc->GetMetaInputParamList()[i];
                    mp->CalcReturnType();
                    mtList.push_back(mp->GetRetMetaClass());
                }
                if (createMC == CoreMetaClassManager::GetInstance().GetRangeMetaClass()) {
                    bool isSame = true;
                    for (size_t i = 0; i < mtList.size() - 1; i++) {
                        auto curMc = mtList[i];
                        auto nextMc = mtList[i + 1];
                        if (curMc != nextMc) {
                            isSame = false;
                            break;
                        }
                    }
                    if (isSame) {
                        m_MetaDefineType->AddTemplateMetaType(new MetaType(mtList[0]));
                    }
                }
            }
        }
    }
    Init();
}

MetaNewObjectExpressNode::MetaNewObjectExpressNode(Compile::FileMetaBraceTerm* fmbt, MetaType* mt, MetaClass* ownerMC, MetaBlockStatements* mbs, MetaVariable* equalMV) {
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

MetaNewObjectExpressNode::MetaNewObjectExpressNode(Compile::FileMetaParTerm* fmpt, MetaType* mt, MetaClass* mc, MetaBlockStatements* mbs) {
    m_FileMetaParTerm = fmpt;
    m_OwnerMetaClass = mc;
    m_OwnerMetaBlockStatements = mbs;
    auto mmf = new MetaMethodCall(nullptr, nullptr, mbs->GetOwnerMetaFunction(), nullptr, nullptr, nullptr, nullptr);
    m_MetaConstructFunctionCall = mmf;
    m_MetaDefineType = new MetaType(mt);
    Init();
}

MetaNewObjectExpressNode::MetaNewObjectExpressNode(Compile::FileMetaBracketTerm* fmbt, MetaClass* mc, MetaBlockStatements* mbs, MetaVariable* equalMV) {
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

void MetaNewObjectExpressNode::Init() {
    // Implementation for Init method
}

void MetaNewObjectExpressNode::Parse(AllowUseSettings* auc) {
    // Implementation for Parse method
}

int MetaNewObjectExpressNode::CalcParseLevel(int level) {
    return level;
}

void MetaNewObjectExpressNode::CalcReturnType() {
    MetaExpressNode::CalcReturnType();
}

MetaType* MetaNewObjectExpressNode::GetReturnMetaDefineType() {
    if (m_MetaDefineType != nullptr) {
        return m_MetaDefineType;
    }
    if (m_MetaConstructFunctionCall != nullptr) {
        m_MetaDefineType = m_MetaConstructFunctionCall->GetMetaDefineType();
    }
    return m_MetaDefineType;
}

std::string MetaNewObjectExpressNode::ToTokenString() {
    return MetaExpressNode::ToTokenString();
}

std::string MetaNewObjectExpressNode::ToFormatString() {
    std::ostringstream sb;
    
    if (m_MetaDefineType->IsEnum()) {
        sb << m_MetaDefineType->GetName();
        sb << ".";
        sb << m_MetaDefineType->GetEnumValue()->GetName();
        if (m_MetaEnumValue != nullptr) {
            sb << "(";
            sb << m_MetaEnumValue->ToFormatString();
            sb << ")";
        }
    } else if (m_MetaDefineType->IsData()) {
        sb << m_MetaDefineType->GetName();
        sb << "{";
        if (m_MetaBraceOrBracketStatementsContent != nullptr) {
            for (int i = 0; i < m_MetaBraceOrBracketStatementsContent->GetCount(); i++) {
                auto bsc = m_MetaBraceOrBracketStatementsContent->GetAssignStatementsList()[i];
                if (bsc == nullptr) {
                    continue;
                }
                sb << bsc->GetMetaMemberData()->ToFormatString2(m_MetaDefineType->IsDynamicData());
                
                if (i < m_MetaBraceOrBracketStatementsContent->GetCount() - 1) {
                    sb << ",";
                }
            }
        }
        sb << "}";
    } else {
        if (m_MetaDefineType != nullptr) {
            sb << m_MetaDefineType->GetName() << "()";
            sb << ".";
        }
        sb << m_MetaBraceOrBracketStatementsContent->ToFormatString();
    }
    
    return sb.str();
}

MetaNewObjectExpressNode* MetaNewObjectExpressNode::CreateNewObjectExpressNodeByPar(Compile::FileMetaParTerm* root, MetaType* mt, MetaClass* omc, MetaBlockStatements* mbs) {
    auto fmct = dynamic_cast<Compile::FileMetaParTerm*>(root);
    if (fmct == nullptr) return nullptr;
    if (mt == nullptr) return nullptr;
    
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

} // namespace Core
} // namespace SimpleLanguage