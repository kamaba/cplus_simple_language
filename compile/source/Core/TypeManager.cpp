//****************************************************************************
//  File:      TypeManager.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2023/5/12 12:00:00
//  Description: Type manager class
//****************************************************************************

#include "TypeManager.h"
#include "MetaType.h"
#include "MetaClass.h"
#include "MetaTemplate.h"
#include "MetaGenTemplateClass.h"
#include "MetaGenTemplateFunction.h"
#include "MetaMemberFunction.h"
#include "MetaNode.h"
#include "../Compile/FileMeta/FileMetaClass.h"
#include "../Compile/FileMeta/FileMetaCommon.h"
#include "../Compile/FileMeta/FileMetatUtil.h"
#include "../Debug/Log.h"
#include "ClassManager.h"
#include <string>
#include <vector>
#include <memory>

using namespace SimpleLanguage::Debug;
using namespace SimpleLanguage::Compile;

namespace SimpleLanguage {
namespace Core {

bool TypeManager::UpdateMetaTypeByGenClassAndFunction(MetaType* mt, MetaGenTemplateClass* mgtc, MetaGenTemplateFunction* mgtf) {
    bool isNeedReg = false;
    MetaClass* findfn = nullptr;
    std::vector<MetaClass*> regMCList;
    
    if (mt->GetTemplateMetaTypeList().size() > 0) {
        for (size_t i = 0; i < mt->GetTemplateMetaTypeList().size(); i++) {
            if (UpdateMetaTypeByGenClassAndFunction(mt->GetTemplateMetaTypeList()[i], mgtc, mgtf)) {
                isNeedReg = true;
            }
            regMCList.push_back(mt->GetTemplateMetaTypeList()[i]->GetMetaClass());
        }
    }
    
    if (isNeedReg) {
        MetaGenTemplateClass* newmc = mt->GetMetaClass()->AddInstanceMetaClass(regMCList);
        if (newmc == nullptr) {
            Log::AddInStructMeta(EError::None, "MetaClass is Null");
            return false;
        }
        mt->SetMetaClass(newmc);
        return true;
    }
    
    if (mt->IsTemplate()) {
        MetaGenTemplate* gmgt = mgtc->GetMetaGenTemplate(mt->GetMetaTemplate()->GetName());
        if (gmgt != nullptr) {
            if (gmgt->GetMetaType()->GetMetaClass() == nullptr) {
                Log::AddInStructMeta(EError::None, "MetaClass is Null");
                return false;
            }
            mt->SetMetaClass(gmgt->GetMetaType()->GetMetaClass());
            mt->SetMetaTemplate(nullptr);
            findfn = gmgt->GetMetaType()->GetMetaClass();
        } else {
            gmgt = mgtf ? mgtf->GetMetaGenTemplate(mt->GetMetaTemplate()->GetName()) : nullptr;
            if (gmgt != nullptr) {
                if (gmgt->GetMetaType()->GetMetaClass() == nullptr) {
                    Log::AddInStructMeta(EError::None, "MetaClass is Null");
                    return false;
                }
                mt->SetMetaClass(gmgt->GetMetaType()->GetMetaClass());
                mt->SetMetaTemplate(nullptr);
                findfn = gmgt->GetMetaType()->GetMetaClass();
            } else {
                Log::AddInStructMeta(EError::None, "没有找到模板中定义的模板内容!" + mt->GetMetaTemplate()->GetName());
            }
        }
    } else {
        return false;
    }

    return true;
}

MetaType* TypeManager::GetMetaTemplateClassAndRegisterExptendTemplateClassInstance(MetaClass* curMc, Compile::FileMetaClassDefine* fmcd) {
    if (fmcd == nullptr) return nullptr;

    MetaNode* getmc = ClassManager::GetInstance().GetMetaClassByRef(curMc, fmcd);
    if (getmc == nullptr) {
        auto mt = curMc->GetMetaTemplateByName(fmcd->GetStringList()[0]);
        if (mt == nullptr) {
            //Log::AddInStructMeta(EError::None, "没有找到模板类中，对应的模板，名称为" + fmcd->GetStringList()[0] + "请仔细检查模板的命名与使用模板命名是否对应", fmcd->GetClassNameToken());
        } else {
            auto retmt = new MetaType(mt);
            return retmt;
        }
    } else {
        return GetMetaTypeByInputTemplateList(curMc, getmc, fmcd->InputTemplateNodeList());
    }
    return nullptr;
}

MetaType* TypeManager::GetMetaTypeByInputTemplateList(MetaClass* ownerMc, MetaNode* getmc, const std::vector<Compile::FileInputTemplateNode*>& inputTemplateNodeList, std::vector<MetaType*>* list) {
    if (inputTemplateNodeList.size() == 0) {
        return new MetaType(getmc->GetMetaClassByTemplateCount(0));
    }
    
    auto findfn = getmc->GetMetaClassByTemplateCount(static_cast<int>(inputTemplateNodeList.size()));
    if (findfn == nullptr) {
        return nullptr;
    }
    
    if (inputTemplateNodeList.size() == 0) {
        return new MetaType(findfn);
    }
    
    auto mt = new MetaType();
    mt->SetTemplateMetaClass(findfn);
    
    // 这里，要注册实体模板类
    for (size_t i = 0; i < inputTemplateNodeList.size(); i++) {
        MetaType* mt2 = GetAndRegisterTemplateDefineMetaTemplateClass(ownerMc, findfn, inputTemplateNodeList[i]);
        mt->AddTemplateMetaType(mt2);
    }
    
    bool igmc = false;
    return ownerMc->AddMetaPreTemplateClass(mt, false, igmc);
}

MetaType* TypeManager::GetAndRegisterTemplateDefineMetaTemplateClass(MetaClass* ownerMc, MetaClass* findMc, Compile::FileInputTemplateNode* fmtd) {
    auto newmn = ClassManager::GetInstance().GetMetaClassByNameAndFileMeta(ownerMc, fmtd->GetFileMeta(), fmtd->GetNameList());
    Compile::FileMetaCallNode* cnode = nullptr;
    
    if (newmn != nullptr) {
        auto findfn = newmn->GetMetaClassByTemplateCount(fmtd->GetInputTemplateCount());
        if (findfn == nullptr) {
            Log::AddInStructMeta(EError::None, "没有发现" + fmtd->GetNameList()[0] + "找到的类!");
            return nullptr;
        }
        
        if (fmtd->GetInputTemplateCount() == 0) {
            return new MetaType(findfn);
        } else {
            auto mt = new MetaType();
            mt->SetTemplateMetaClass(findfn);
            std::vector<MetaGenTemplate*> mgtList;
            
            for (size_t i = 0; i < fmtd->DefineClassCallLink()->GetCallNodeList().size(); i++) {
                auto dcc = fmtd->DefineClassCallLink()->GetCallNodeList()[i];
                for (size_t j = 0; j < dcc->InputTemplateNodeList().size(); j++) {
                    auto itn = dcc->InputTemplateNodeList()[j];
                    auto mt2 = GetAndRegisterTemplateDefineMetaTemplateClass(ownerMc, findfn, itn);
                    mt->AddTemplateMetaType(mt2);
                }
            }

            bool igmc = false;
            return ownerMc->AddMetaPreTemplateClass(mt, false, igmc);
        }
    } else {
        if (fmtd->GetNameList().size() == 1) {
            auto mt = ownerMc->GetMetaTemplateByName(fmtd->GetNameList()[0]);
            if (mt == nullptr) {
                Log::AddInStructMeta(EError::None, "没有找到模板类中，对应的模板，请仔细检查模板的命名与使用模板命名是否对应", cnode ? cnode->GetToken() : nullptr);
            } else {
                return new MetaType(mt);
            }
        } else {
            Log::AddInStructMeta(EError::None, "使用模板类中使用.连接符号，模板中不允许使用.");
        }
    }
    return nullptr;
}

MetaType* TypeManager::GetMetaTypeByTemplateFunction(MetaClass* curMc, MetaMemberFunction* findFun, Compile::FileMetaClassDefine* fmcd) {
    if (fmcd == nullptr) return nullptr;

    MetaNode* getmc = ClassManager::GetInstance().GetMetaClassByRef(curMc, fmcd);
    if (getmc == nullptr) {
        auto gmtbn = curMc->GetMetaTemplateByName(fmcd->GetStringList()[0]);
        if (gmtbn != nullptr) {
            auto mt = new MetaType(gmtbn);
            return mt;
        } else if (findFun != nullptr) {
            auto mt = findFun->GetMetaDefineTemplateByName(fmcd->GetStringList()[0]);
            if (mt == nullptr) {
                return nullptr;
            }
            return new MetaType(mt);
        } else {
            Log::AddInStructMeta(EError::None, "没有找到" + fmcd->GetStringList()[0] + " 的相关类!");
        }
    } else {
        return GetMetaTypeByTemplateList(curMc, getmc, findFun, fmcd->InputTemplateNodeList());
    }
    return nullptr;
}

MetaType* TypeManager::GetMetaTypeByTemplateList(MetaClass* curMc, MetaNode* getmc, MetaMemberFunction* findFun, const std::vector<Compile::FileInputTemplateNode*>& inputTemplateNodeList) {
    auto findfn = getmc->GetMetaClassByTemplateCount(static_cast<int>(inputTemplateNodeList.size()));
    if (findfn != nullptr) {
        if (inputTemplateNodeList.size() == 0) {
            return new MetaType(findfn);
        }

        auto newmc = HandleInputTemplateNodeList(findfn, findfn, findFun, inputTemplateNodeList, false);
        if (newmc != nullptr) {
            return newmc;
        } else {
            auto mt = new MetaType();
            mt->SetTemplateMetaClass(findfn);
            return mt;
        }
    }

    return nullptr;
}

MetaType* TypeManager::HandleInputTemplateNodeList(MetaClass* findfn, MetaClass* regMc, MetaMemberFunction* findFun, const std::vector<Compile::FileInputTemplateNode*>& inputTemplateNodeList, bool isParse) {
    auto getmc = findfn;
    MetaType* mt = new MetaType();
    
    if (inputTemplateNodeList.size() == 0) {
        mt->SetMetaClass(regMc);
        return mt;
    }
    
    mt->SetTemplateMetaClass(regMc);
    // 这里，要注册实体模板类
    for (size_t i = 0; i < inputTemplateNodeList.size(); i++) {
        auto t = RegisterTemplateDefineMetaTemplateFunction(findfn, regMc, findFun, inputTemplateNodeList[i], isParse);
        mt->AddTemplateMetaType(t);
    }
    
    bool igmc = false;
    mt = regMc->AddMetaPreTemplateClass(mt, isParse, igmc);
    return mt;
}

MetaType* TypeManager::RegisterTemplateDefineMetaTemplateFunction(MetaClass* findMc, MetaClass* regMc, MetaMemberFunction* findFun, Compile::FileInputTemplateNode* fmtd, bool isParse) {
    auto newmc = ClassManager::GetInstance().GetMetaClassByNameAndFileMeta(findMc, fmtd->GetFileMeta(), fmtd->GetNameList());
    
    if (newmc != nullptr) {
        auto findfn = newmc->GetMetaClassByTemplateCount(fmtd->GetInputTemplateCount());

        if (findfn == nullptr) {
            Log::AddInStructMeta(EError::None, "没有找到相对应的模板类!!");
            return nullptr;
        }
        
        if (fmtd->GetInputTemplateCount() > 0) {
            auto dcc = fmtd->DefineClassCallLink()->GetCallNodeList()[fmtd->DefineClassCallLink()->GetCallNodeList().size() - 1];

            auto retmc = HandleInputTemplateNodeList(findMc, findfn, findFun, dcc->InputTemplateNodeList(), isParse);

            if (retmc != nullptr) {
                return retmc;
            }
        } else {
            auto mt = new MetaType(findfn);
            return mt;
        }
    } else {
        if (fmtd->GetNameList().size() == 1) {
            if (findMc != nullptr) {
                auto mgtc2 = findMc->GetMetaTemplateByName(fmtd->GetNameList()[0]);
                if (mgtc2 != nullptr) {
                    return new MetaType(mgtc2);
                }
            }
            if (findFun != nullptr) {
                auto mt = findFun->GetMetaDefineTemplateByName(fmtd->GetNameList()[0]);
                if (mt != nullptr) {
                    return new MetaType(mt);
                }
            }
        } else {
            Log::AddInStructMeta(EError::None, "----fmtd.nameList.count > 1 ");
        }
    }
    return nullptr;
}

} // namespace Core
} // namespace SimpleLanguage
