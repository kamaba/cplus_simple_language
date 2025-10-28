//****************************************************************************
//  File:      ExpressManager.cpp
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/5/17 12:00:00
//  Description:  
//****************************************************************************

#include "ExpressManager.h"
#include "MetaExpressNode/MetaExpressBase.h"
#include "MetaExpressNode/MetaExpressConst.h"
#include "MetaExpressNode/MetaExecuteStatementsNode.h"
#include "MetaExpressNode/MetaExpressNewObject.h"
#include "MetaExpressNode/MetaExpressCalllink.h"
#include "MetaExpressNode/MetaExpressOperator.h"
#include "MetaCallLink.h"
#include "Statements/MetaBlockStatements.h"
#include "MetaClass.h"
#include "MetaType.h"
#include "MetaVariable.h"
#include "MetaVisitCall.h"
#include "AllowUseSettings.h"
#include "MetaMemberFunction.h"
#include "BaseMetaClass/CoreMetaClassManager.h"
#include "../Compile/FileMeta/FileMetaBase.h"
#include "../Compile/FileMeta/FileMetaCommon.h"
#include "../Compile/FileMeta/FileMetaExpress.h"
#include "../Compile/FileMeta/FileMetaSyntax.h"
#include "../Compile/Token.h"
#include "../Debug/Log.h"
#include <iostream>

using namespace SimpleLanguage::Debug;

namespace SimpleLanguage {
namespace Core {

// 静态成员初始化
ExpressOptimizeConfig ExpressManager::expressOptimizeConfig;

ExpressManager& ExpressManager::GetInstance() {
    static ExpressManager instance;
    return instance;
}

// CreateExpressParam implementation
CreateExpressParam::CreateExpressParam(const CreateExpressParam& clone) {
    ownerMBS = clone.ownerMBS;
    ownerMetaClass = clone.ownerMetaClass;
    equalMetaVariable = clone.equalMetaVariable;
    metaType = clone.metaType;
    parentMetaType = clone.parentMetaType;
    fme = clone.fme;
    isStatic = clone.isStatic;
    isConst = clone.isConst;
    allowUseIfSyntax = clone.allowUseIfSyntax;
    allowUseSwitchSyntax = clone.allowUseSwitchSyntax;
    allowUseParSyntax = clone.allowUseParSyntax;
    allowUseBraceSyntax = clone.allowUseBraceSyntax;
    parseFrom = clone.parseFrom;
}

void CreateExpressParam::SetOwnerMBS(MetaBlockStatements* mbs) {
    ownerMBS = mbs;
}

void CreateExpressParam::SetOwnerMetaClass(MetaClass* mc) {
    ownerMetaClass = mc;
}

void CreateExpressParam::SetMetaType(MetaType* mt) {
    metaType = mt;
}

void CreateExpressParam::SetParentMetaType(MetaType* pmt) {
    parentMetaType = pmt;
}

void CreateExpressParam::SetEqualMetaVariable(MetaVariable* mv) {
    equalMetaVariable = mv;
}

void CreateExpressParam::SetFme(Compile::FileMetaBaseTerm* fme) {
    this->fme = fme;
}

void CreateExpressParam::SetIsStatic(bool isStatic) {
    this->isStatic = isStatic;
}

void CreateExpressParam::SetIsConst(bool isConst) {
    this->isConst = isConst;
}

void CreateExpressParam::SetAllowUseIfSyntax(bool allow) {
    allowUseIfSyntax = allow;
}

void CreateExpressParam::SetAllowUseSwitchSyntax(bool allow) {
    allowUseSwitchSyntax = allow;
}

void CreateExpressParam::SetAllowUseParSyntax(bool allow) {
    allowUseParSyntax = allow;
}

void CreateExpressParam::SetAllowUseBraceSyntax(bool allow) {
    allowUseBraceSyntax = allow;
}

void CreateExpressParam::SetParseFrom(EParseFrom _parseFrom) {
    parseFrom = _parseFrom;
}

bool ExpressManager::IsCanExpressCampute(MetaClass* mc) {
    if (mc == CoreMetaClassManager::GetInstance().GetInt16MetaClass()
        || mc == CoreMetaClassManager::GetInstance().GetInt32MetaClass()
        || mc == CoreMetaClassManager::GetInstance().GetInt64MetaClass()
        || mc == CoreMetaClassManager::GetInstance().GetFloat32MetaClass()
        || mc == CoreMetaClassManager::GetInstance().GetFloat64MetaClass())
        return true;
    return false;
}

MetaExpressNode* ExpressManager::CreateExpressNodeByCEP(const CreateExpressParam& cep) {
    Compile::FileMetaBaseTerm* fmte = cep.fme;
    MetaBlockStatements* mbs = cep.ownerMBS;
    MetaType* mdt = cep.metaType;
    MetaVariable* equalMetaVariable = cep.equalMetaVariable;
    MetaClass* mc = cep.ownerMetaClass;

    if (fmte == nullptr) {
        Log::AddInStructMeta(EError::None, "CreateExpressNode FileMetaBaseTerm 为空 !!");
        return nullptr;
    }
    
    auto ifExpressTerm = dynamic_cast<Compile::FileMetaIfSyntaxTerm*>(fmte);
    if (cep.allowUseIfSyntax) {
        if (ifExpressTerm != nullptr) {
            MetaExecuteStatementsNode* mesn = MetaExecuteStatementsNode::CreateMetaExecuteStatementsNodeByIfExpress(mdt, mc, mbs, ifExpressTerm->GetIfSyntax());
            if (mesn != nullptr) {
                return mesn;
            }
        }
    } else if (ifExpressTerm != nullptr) {
        Log::AddInStructMeta(EError::None, "不允许使用If语句!!");
        return nullptr;
    }

    auto switchExpressTerm = dynamic_cast<Compile::FileMetaMatchSyntaxTerm*>(fmte);
    if (cep.allowUseSwitchSyntax) {
        if (switchExpressTerm != nullptr) {
            MetaExecuteStatementsNode* mesn = MetaExecuteStatementsNode::CreateMetaExecuteStatementsNodeBySwitchExpress(mdt, mc, mbs, switchExpressTerm->GetSwitchSyntax());
            if (mesn != nullptr) {
                return mesn;
            }
        }
    } else if (switchExpressTerm != nullptr) {
        Log::AddInStructMeta(EError::None, "不允许使用Switch语句!!");
        return nullptr;
    }

    auto parExpressTerm = dynamic_cast<Compile::FileMetaParTerm*>(fmte);
    if (cep.allowUseParSyntax) {
        if (parExpressTerm != nullptr) {
            MetaNewObjectExpressNode* mnoen = MetaNewObjectExpressNode::CreateNewObjectExpressNodeByPar(parExpressTerm, mdt, mc, mbs);
            if (mnoen != nullptr)
                return mnoen;
        }
    } else if (parExpressTerm != nullptr) {
        Log::AddInStructMeta(EError::None, "不允许使用Switch语句!!");
        return nullptr;
    }

    return CreateExpressNode(cep);
}

MetaExpressNode* ExpressManager::CreateExpressNode(const CreateExpressParam& cep) {
    if (cep.fme == nullptr) {
        return nullptr;
    }
    MetaClass* ownerClass = cep.ownerMetaClass;
    auto root = cep.fme->GetRoot();
    if (root->GetLeft() == nullptr && root->GetRight() == nullptr) {
        MetaExpressNode* men = nullptr;
        
        if (auto fmst = dynamic_cast<Compile::FileMetaSymbolTerm*>(root)) {
            Log::AddInStructMeta(EError::None, "Error CreateExpressNode 创建表达项不能为符号");
        } else if (auto fmcvt = dynamic_cast<Compile::FileMetaConstValueTerm*>(root)) {
            if (fmcvt->GetToken()->GetType() == ETokenType::NumberArrayLink) {
                MetaNewObjectExpressNode* mnoen = new MetaNewObjectExpressNode(fmcvt, ownerClass, cep.ownerMBS);
                return mnoen;
            } else {
                men = new MetaConstExpressNode(fmcvt);
                return men;
            }
        } else if (auto fmct = dynamic_cast<Compile::FileMetaCallTerm*>(root)) {     //className.functionname().varname;
            MetaCallLinkExpressNode* men2 = new MetaCallLinkExpressNode(fmct->GetCallLink(), cep.ownerMetaClass, cep.ownerMBS, cep.equalMetaVariable);
            return men2;
        } else if (auto fmbt = dynamic_cast<Compile::FileMetaBraceTerm*>(root)) {  // {1,2,3} {a=10,b=20}
            men = new MetaNewObjectExpressNode(fmbt, cep.metaType, ownerClass, cep.ownerMBS, cep.equalMetaVariable);
            return men;
        } else if (auto fmpt = dynamic_cast<Compile::FileMetaParTerm*>(root)) {  //  (1,2)
            MetaNewObjectExpressNode* mnoen = MetaNewObjectExpressNode::CreateNewObjectExpressNodeByPar(fmpt, cep.metaType, ownerClass, cep.ownerMBS);
            if (mnoen != nullptr)
                return mnoen;
        } else if (auto fmte = dynamic_cast<Compile::FileMetaTermExpress*>(root)) {
            CreateExpressParam newCep = cep;
            newCep.ownerMetaClass = ownerClass;
            newCep.fme = fmte;
            men = CreateExpressNode(newCep);
            return men;
        } else if (auto fmbt = dynamic_cast<Compile::FileMetaBracketTerm*>(root)) {
            MetaNewObjectExpressNode* mnoen = new MetaNewObjectExpressNode(fmbt, ownerClass, cep.ownerMBS, cep.equalMetaVariable);
            if (mnoen != nullptr)
                return mnoen;
        } else {
            Log::AddInStructMeta(EError::None, "Error CreateExpressNode 创建表达项不能为符号");
        }
    } else {
        CreateExpressParam clonecep = CreateExpressParam(cep);
        clonecep.fme = root->GetLeft();
        MetaExpressNode* leftNode = CreateExpressNode(clonecep);
        clonecep.fme = root->GetRight();
        MetaExpressNode* rightNode = CreateExpressNode(clonecep);

        if (leftNode != nullptr && rightNode != nullptr) {
            if (auto fmst = dynamic_cast<Compile::FileMetaSymbolTerm*>(root)) {
                return new MetaOpExpressNode(fmst, cep.metaType, leftNode, rightNode);
            } else {
                Log::AddInStructMeta(EError::None, " Error VisitFileMetaExpress fileMetaNode 不是符号!!");
            }
        } else if (leftNode != nullptr && rightNode == nullptr) {
            if (auto fmst = dynamic_cast<Compile::FileMetaSymbolTerm*>(root)) {
                return new MetaUnaryOpExpressNode(fmst, leftNode);
            } else {
                return leftNode;
            }
        } else if (leftNode == nullptr && rightNode != nullptr) {
            if (auto fmst = dynamic_cast<Compile::FileMetaSymbolTerm*>(root)) {
                return new MetaUnaryOpExpressNode(fmst, rightNode);
            } else {
                return rightNode;
            }
        } else {
            Log::AddInStructMeta(EError::None, " Error VisitFileMetaExpress left and right都为空!!");
        }
        return nullptr;
    }
    return nullptr;
}

MetaExpressNode* ExpressManager::CreateOptimizeAfterExpress(MetaExpressNode* men, ExpressOptimizeConfig* config) {
    if (config == nullptr) {
        config = &ExpressManager::expressOptimizeConfig;
    }
    
    if (auto muoen = dynamic_cast<MetaUnaryOpExpressNode*>(men)) {
        auto newValue = CreateOptimizeAfterExpress(muoen->GetValue(), config);
        muoen->SetValue(newValue);
        if (config->ifUnaryExpressValueIsConstThenCompute) {
            return muoen->SimulateCompute();
        }
    } else if (auto moen = dynamic_cast<MetaOpExpressNode*>(men)) {
        auto newLeft = CreateOptimizeAfterExpress(moen->GetLeft(), config);
        auto newRight = CreateOptimizeAfterExpress(moen->GetRight(), config);
        moen->SetLeft(newLeft);
        moen->SetRight(newRight);
        moen->SimulateCompute(config);
    } else if (auto mcn = dynamic_cast<MetaCallLinkExpressNode*>(men)) {
        // mcn->GetMetaVariable
    } else if (auto mcen = dynamic_cast<MetaConstExpressNode*>(men)) {
        // 常量节点不需要优化
    } else {
        Log::AddInStructMeta(EError::None, "Error Optimaze don't support that ExpressType");
    }
    return men;
}

void ExpressManager::CreateNewOrCalllink(const CreateExpressParam& cep, MetaNewObjectExpressNode*& mnoen, MetaCallLinkExpressNode*& men2) {
    mnoen = nullptr;
    men2 = nullptr;
    MetaClass* omc = cep.ownerMetaClass;
    MetaBlockStatements* mbs = cep.ownerMBS;
    auto fmct = dynamic_cast<Compile::FileMetaCallTerm*>(cep.fme);
    if (fmct == nullptr) return;
    if (fmct->GetCallLink() == nullptr) return;
    if (fmct->GetCallLink()->GetCallNodeList().size() <= 0) return;

    AllowUseSettings aus;
    aus.SetParseFrom( cep.parseFrom );

    MetaCallLink* mcl = new MetaCallLink(fmct->GetCallLink(), cep.ownerMetaClass, cep.ownerMBS, cep.metaType, cep.equalMetaVariable);
    if (!mcl->Parse(&aus)) return;
    mcl->CalcReturnType();

    bool isNewClass = false;
    bool isNewData = false;
    bool isNewEnum = false;
    if (mcl->GetFinalCallNode() != nullptr && mcl->GetFinalCallNode()->visitType() == EVisitType::New) {
        if (mcl->GetFinalCallNode()->callMetaType()->IsData()) {
            isNewData = true;
        } else {
            isNewClass = true;
        }
    }
    
    if (mcl->GetFinalCallNode() != nullptr && mcl->GetFinalCallNode()->methodCall() != nullptr) {
        if (dynamic_cast<MetaMemberFunction*>(mcl->GetFinalCallNode()->methodCall()->function())->IsConstructInitFunction()) {
            isNewClass = true;
        }
    }
    
    MetaType* retmt = mcl->GetMetaDefineType();
    if (isNewClass) {
        mnoen = new MetaNewObjectExpressNode(fmct, mcl, retmt, omc, mbs, mcl->GetFinalCallNode()->methodCall());
    } else if (isNewData) {
        mnoen = new MetaNewObjectExpressNode(fmct, mcl, retmt, omc, mbs, mcl->GetFinalCallNode()->methodCall());
    } else if (isNewEnum) {
        mnoen = new MetaNewObjectExpressNode(fmct, mcl, retmt, omc, mbs, nullptr);
    } else {
        men2 = new MetaCallLinkExpressNode(fmct->GetCallLink(), omc, mbs, nullptr);
    }
}

int ExpressManager::CalcParseLevel(int level, MetaExpressNode* men) {
    if (auto muoen = dynamic_cast<MetaUnaryOpExpressNode*>(men)) {
        level = CalcParseLevel(level, muoen->GetValue());
    } else if (auto moen = dynamic_cast<MetaOpExpressNode*>(men)) {
        level = CalcParseLevel(level, moen->GetRight());
        level = CalcParseLevel(level, moen->GetLeft());
    } else if (auto mcn = dynamic_cast<MetaCallLinkExpressNode*>(men)) {
        level = mcn->CalcParseLevel(level);
    }
    return level;
}

} // namespace Core
} // namespace SimpleLanguage