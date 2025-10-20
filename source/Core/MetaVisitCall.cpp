//****************************************************************************
//  File:      MetaVisitCall.cpp
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description:  create visit variable or method call!
//****************************************************************************

#include "MetaVisitCall.h"
#include "../Compile/AllowUseSettings.h"
#include <iostream>

namespace SimpleLanguage {
namespace Core {

MetaMethodCall::MetaMethodCall(MetaClass* staticMc, const std::vector<MetaType*>& staticMmitList, MetaFunction* fun, const std::vector<MetaType*>& mpipList, MetaInputParamCollection* paramCollection, MetaVariable* loadMv, MetaVariable* storeMv) {
    m_StaticCallerMetaClass = staticMc;
    if (!staticMmitList.empty()) {
        m_StaticMetaClassInputTemplateList = staticMmitList;
    }
    m_VMCallMetaFunction = fun;
    if (!mpipList.empty()) {
        m_MetaFunctionInputTemplateList = mpipList;
    }
    m_LoadMetaVariable = loadMv;
    m_StoreMetaVariable = storeMv;
}

void MetaMethodCall::Parse(AllowUseSettings* auc) {
    // 解析方法调用
    if (m_VMCallMetaFunction != nullptr) {
        // 解析函数
    }
    
    for (auto& param : m_MetaInputParamList) {
        if (param != nullptr) {
            param->parse();
        }
    }
}

void MetaMethodCall::CalcReturnType() {
    // 计算返回类型
    if (m_VMCallMetaFunction != nullptr) {
        // 计算函数返回类型
    }
    
    for (auto& param : m_MetaInputParamList) {
        if (param != nullptr) {
            param->calcReturnType();
        }
    }
}

std::string MetaMethodCall::ToFormatString() const {
    std::string result;
    
    if (m_StaticCallerMetaClass != nullptr) {
        result += m_StaticCallerMetaClass->GetName() + "::";
    }
    
    if (m_VMCallMetaFunction != nullptr) {
        result += m_VMCallMetaFunction->getName() + "(";
    }
    
    for (size_t i = 0; i < m_MetaInputParamList.size(); ++i) {
        if (i > 0) {
            result += ", ";
        }
        if (m_MetaInputParamList[i] != nullptr) {
            result += m_MetaInputParamList[i]->toFormatString();
        }
    }
    
    result += ")";
    
    return result;
}

std::string MetaMethodCall::ToTokenString() const {
    std::string result;
    
    if (m_StaticCallerMetaClass != nullptr) {
        result += m_StaticCallerMetaClass->GetName() + "::";
    }
    
    if (m_VMCallMetaFunction != nullptr) {
        result += m_VMCallMetaFunction->getName() + "(";
    }
    
    for (size_t i = 0; i < m_MetaInputParamList.size(); ++i) {
        if (i > 0) {
            result += ", ";
        }
        if (m_MetaInputParamList[i] != nullptr) {
            result += m_MetaInputParamList[i]->toTokenString();
        }
    }
    
    result += ")";
    
    return result;
}

MetaMethodCall* MetaMethodCall::Clone() const {
    MetaMethodCall* cloned = new MetaMethodCall(m_StaticCallerMetaClass, m_StaticMetaClassInputTemplateList, m_VMCallMetaFunction, m_MetaFunctionInputTemplateList, nullptr, m_LoadMetaVariable, m_StoreMetaVariable);
    
    // 克隆输入参数列表
    for (auto& param : m_MetaInputParamList) {
        if (param != nullptr) {
            cloned->m_MetaInputParamList.push_back(param->clone());
        }
    }
    
    return cloned;
}

MetaVisitCall::MetaVisitCall() : MetaBase() {
    // 默认构造函数
}

MetaVisitCall::MetaVisitCall(const std::string& name) : MetaBase() {
    m_Name = name;
}

std::string MetaVisitCall::ToFormatString() const {
    std::string result;
    
    if (m_MetaMethodCall != nullptr) {
        result += m_MetaMethodCall->toFormatString();
    } else if (m_MetaVariable != nullptr) {
        result += m_MetaVariable->getName();
    } else if (m_MetaClass != nullptr) {
        result += m_MetaClass->getName();
    } else if (m_MetaType != nullptr) {
        result += m_MetaType->toFormatString();
    } else if (m_MetaMemberFunction != nullptr) {
        result += m_MetaMemberFunction->getName();
    } else if (m_MetaMemberVariable != nullptr) {
        result += m_MetaMemberVariable->getName();
    } else if (m_MetaMemberData != nullptr) {
        result += m_MetaMemberData->getName();
    } else if (m_MetaEnum != nullptr) {
        result += m_MetaEnum->getName();
    } else if (m_MetaData != nullptr) {
        result += m_MetaData->getName();
    } else if (m_MetaExpressNode != nullptr) {
        result += m_MetaExpressNode->toFormatString();
    }
    
    return result;
}

std::string MetaVisitCall::ToTokenString() const {
    std::string result;
    
    if (m_MetaMethodCall != nullptr) {
        result += m_MetaMethodCall->toTokenString();
    } else if (m_MetaVariable != nullptr) {
        result += m_MetaVariable->getName();
    } else if (m_MetaClass != nullptr) {
        result += m_MetaClass->getName();
    } else if (m_MetaType != nullptr) {
        result += m_MetaType->toFormatString();
    } else if (m_MetaMemberFunction != nullptr) {
        result += m_MetaMemberFunction->getName();
    } else if (m_MetaMemberVariable != nullptr) {
        result += m_MetaMemberVariable->getName();
    } else if (m_MetaMemberData != nullptr) {
        result += m_MetaMemberData->getName();
    } else if (m_MetaEnum != nullptr) {
        result += m_MetaEnum->getName();
    } else if (m_MetaData != nullptr) {
        result += m_MetaData->getName();
    } else if (m_MetaExpressNode != nullptr) {
        result += m_MetaExpressNode->toTokenString();
    }
    
    return result;
}

void MetaVisitCall::Parse() {
    // 解析访问调用
    if (m_MetaMethodCall != nullptr) {
        m_MetaMethodCall->parse(nullptr);
    }
    
    if (m_MetaVariable != nullptr) {
        // 解析变量
    }
    
    if (m_MetaClass != nullptr) {
        // 解析类
    }
    
    if (m_MetaType != nullptr) {
        // 解析类型
    }
    
    if (m_MetaMemberFunction != nullptr) {
        // 解析成员函数
    }
    
    if (m_MetaMemberVariable != nullptr) {
        // 解析成员变量
    }
    
    if (m_MetaMemberData != nullptr) {
        // 解析成员数据
    }
    
    if (m_MetaEnum != nullptr) {
        // 解析枚举
    }
    
    if (m_MetaData != nullptr) {
        // 解析数据
    }
    
    if (m_MetaExpressNode != nullptr) {
        m_MetaExpressNode->parse();
    }
    
    m_IsParsed = true;
}

void MetaVisitCall::CalcReturnType() {
    // 计算返回类型
    if (m_MetaMethodCall != nullptr) {
        m_MetaMethodCall->calcReturnType();
    }
    
    if (m_MetaVariable != nullptr) {
        // 计算变量类型
    }
    
    if (m_MetaClass != nullptr) {
        // 计算类类型
    }
    
    if (m_MetaType != nullptr) {
        // 计算类型
    }
    
    if (m_MetaMemberFunction != nullptr) {
        // 计算成员函数类型
    }
    
    if (m_MetaMemberVariable != nullptr) {
        // 计算成员变量类型
    }
    
    if (m_MetaMemberData != nullptr) {
        // 计算成员数据类型
    }
    
    if (m_MetaEnum != nullptr) {
        // 计算枚举类型
    }
    
    if (m_MetaData != nullptr) {
        // 计算数据类型
    }
    
    if (m_MetaExpressNode != nullptr) {
        m_MetaExpressNode->calcReturnType();
    }
}

MetaVisitCall* MetaVisitCall::Clone() const {
    MetaVisitCall* cloned = new MetaVisitCall(m_Name);
    
    cloned->m_MetaMethodCall = m_MetaMethodCall ? m_MetaMethodCall->clone() : nullptr;
    cloned->m_MetaVariable = m_MetaVariable;
    cloned->m_MetaClass = m_MetaClass;
    cloned->m_MetaType = m_MetaType;
    cloned->m_MetaMemberFunction = m_MetaMemberFunction;
    cloned->m_MetaMemberVariable = m_MetaMemberVariable;
    cloned->m_MetaMemberData = m_MetaMemberData;
    cloned->m_MetaEnum = m_MetaEnum;
    cloned->m_MetaData = m_MetaData;
    cloned->m_MetaExpressNode = m_MetaExpressNode ? m_MetaExpressNode->clone() : nullptr;
    cloned->m_IsParsed = m_IsParsed;
    cloned->m_IsStatic = m_IsStatic;
    cloned->m_IsConst = m_IsConst;
    cloned->m_IsTemplate = m_IsTemplate;
    cloned->m_IsArray = m_IsArray;
    cloned->m_IsRange = m_IsRange;
    cloned->m_IsDynamic = m_IsDynamic;
    cloned->m_IsData = m_IsData;
    cloned->m_IsEnum = m_IsEnum;
    cloned->m_IsClass = m_IsClass;
    cloned->m_IsType = m_IsType;
    cloned->m_IsFunction = m_IsFunction;
    cloned->m_IsVariable = m_IsVariable;
    cloned->m_IsMember = m_IsMember;
    cloned->m_IsInner = m_IsInner;
    cloned->m_IsOuter = m_IsOuter;
    cloned->m_IsGlobal = m_IsGlobal;
    cloned->m_IsLocal = m_IsLocal;
    cloned->m_IsArgument = m_IsArgument;
    cloned->m_IsReturn = m_IsReturn;
    cloned->m_IsVoid = m_IsVoid;
    cloned->m_IsNull = m_IsNull;
    cloned->m_IsObject = m_IsObject;
    cloned->m_IsString = m_IsString;
    cloned->m_IsBoolean = m_IsBoolean;
    cloned->m_IsByte = m_IsByte;
    cloned->m_IsInt = m_IsInt;
    cloned->m_IsFloat = m_IsFloat;
    cloned->m_IsDouble = m_IsDouble;
    cloned->m_IsChar = m_IsChar;
    cloned->m_IsLong = m_IsLong;
    cloned->m_IsShort = m_IsShort;
    cloned->m_IsUInt = m_IsUInt;
    cloned->m_IsULong = m_IsULong;
    cloned->m_IsUShort = m_IsUShort;
    cloned->m_IsSByte = m_IsSByte;
    cloned->m_IsUByte = m_IsUByte;
    cloned->m_IsFloat32 = m_IsFloat32;
    cloned->m_IsFloat64 = m_IsFloat64;
    cloned->m_IsInt16 = m_IsInt16;
    cloned->m_IsInt32 = m_IsInt32;
    cloned->m_IsInt64 = m_IsInt64;
    cloned->m_IsUInt16 = m_IsUInt16;
    cloned->m_IsUInt32 = m_IsUInt32;
    cloned->m_IsUInt64 = m_IsUInt64;
    cloned->m_IsThis = m_IsThis;
    cloned->m_IsBase = m_IsBase;
    cloned->m_IsNew = m_IsNew;
    cloned->m_IsDelete = m_IsDelete;
    cloned->m_IsCast = m_IsCast;
    cloned->m_IsAs = m_IsAs;
    cloned->m_IsIs = m_IsIs;
    cloned->m_IsOf = m_IsOf;
    cloned->m_IsIn = m_IsIn;
    cloned->m_IsOut = m_IsOut;
    cloned->m_IsRef = m_IsRef;
    cloned->m_IsVal = m_IsVal;
    cloned->m_IsVar = m_IsVar;
    cloned->m_IsLet = m_IsLet;
    cloned->m_IsConst = m_IsConst;
    cloned->m_IsReadonly = m_IsReadonly;
    cloned->m_IsWriteonly = m_IsWriteonly;
    cloned->m_IsPublic = m_IsPublic;
    cloned->m_IsPrivate = m_IsPrivate;
    cloned->m_IsProtected = m_IsProtected;
    cloned->m_IsInternal = m_IsInternal;
    cloned->m_IsSealed = m_IsSealed;
    cloned->m_IsAbstract = m_IsAbstract;
    cloned->m_IsVirtual = m_IsVirtual;
    cloned->m_IsOverride = m_IsOverride;
    cloned->m_IsNew = m_IsNew;
    cloned->m_IsExtern = m_IsExtern;
    cloned->m_IsPartial = m_IsPartial;
    cloned->m_IsStatic = m_IsStatic;
    cloned->m_IsReadonly = m_IsReadonly;
    cloned->m_IsVolatile = m_IsVolatile;
    cloned->m_IsUnsafe = m_IsUnsafe;
    cloned->m_IsFixed = m_IsFixed;
    cloned->m_IsLock = m_IsLock;
    cloned->m_IsUsing = m_IsUsing;
    cloned->m_IsNamespace = m_IsNamespace;
    cloned->m_IsClass = m_IsClass;
    cloned->m_IsStruct = m_IsStruct;
    cloned->m_IsInterface = m_IsInterface;
    cloned->m_IsEnum = m_IsEnum;
    cloned->m_IsDelegate = m_IsDelegate;
    cloned->m_IsEvent = m_IsEvent;
    cloned->m_IsProperty = m_IsProperty;
    cloned->m_IsIndexer = m_IsIndexer;
    cloned->m_IsOperator = m_IsOperator;
    cloned->m_IsConstructor = m_IsConstructor;
    cloned->m_IsDestructor = m_IsDestructor;
    cloned->m_IsMethod = m_IsMethod;
    cloned->m_IsField = m_IsField;
    cloned->m_IsParameter = m_IsParameter;
    cloned->m_IsLocal = m_IsLocal;
    cloned->m_IsReturn = m_IsReturn;
    cloned->m_IsThis = m_IsThis;
    cloned->m_IsBase = m_IsBase;
    cloned->m_IsSuper = m_IsSuper;
    cloned->m_IsSelf = m_IsSelf;
    cloned->m_IsParent = m_IsParent;
    cloned->m_IsChild = m_IsChild;
    cloned->m_IsSibling = m_IsSibling;
    cloned->m_IsAncestor = m_IsAncestor;
    cloned->m_IsDescendant = m_IsDescendant;
    cloned->m_IsRoot = m_IsRoot;
    cloned->m_IsLeaf = m_IsLeaf;
    cloned->m_IsBranch = m_IsBranch;
    cloned->m_IsTrunk = m_IsTrunk;
    cloned->m_IsNode = m_IsNode;
    cloned->m_IsEdge = m_IsEdge;
    cloned->m_IsVertex = m_IsVertex;
    cloned->m_IsArc = m_IsArc;
    cloned->m_IsPath = m_IsPath;
    cloned->m_IsRoute = m_IsRoute;
    cloned->m_IsTrail = m_IsTrail;
    cloned->m_IsWalk = m_IsWalk;
    cloned->m_IsTour = m_IsTour;
    cloned->m_IsCircuit = m_IsCircuit;
    cloned->m_IsCycle = m_IsCycle;
    cloned->m_IsLoop = m_IsLoop;
    cloned->m_IsRing = m_IsRing;
    cloned->m_IsChain = m_IsChain;
    cloned->m_IsSequence = m_IsSequence;
    cloned->m_IsSeries = m_IsSeries;
    cloned->m_IsList = m_IsList;
    cloned->m_IsArray = m_IsArray;
    cloned->m_IsVector = m_IsVector;
    cloned->m_IsMatrix = m_IsMatrix;
    cloned->m_IsTensor = m_IsTensor;
    cloned->m_IsScalar = m_IsScalar;
    cloned->m_IsVector = m_IsVector;
    cloned->m_IsPoint = m_IsPoint;
    cloned->m_IsLine = m_IsLine;
    cloned->m_IsPlane = m_IsPlane;
    cloned->m_IsSpace = m_IsSpace;
    cloned->m_IsVolume = m_IsVolume;
    cloned->m_IsArea = m_IsArea;
    cloned->m_IsLength = m_IsLength;
    cloned->m_IsWidth = m_IsWidth;
    cloned->m_IsHeight = m_IsHeight;
    cloned->m_IsDepth = m_IsDepth;
    cloned->m_IsRadius = m_IsRadius;
    cloned->m_IsDiameter = m_IsDiameter;
    cloned->m_IsCircumference = m_IsCircumference;
    cloned->m_IsPerimeter = m_IsPerimeter;
    cloned->m_IsSurface = m_IsSurface;
    cloned->m_IsFace = m_IsFace;
    cloned->m_IsEdge = m_IsEdge;
    cloned->m_IsVertex = m_IsVertex;
    cloned->m_IsCorner = m_IsCorner;
    cloned->m_IsAngle = m_IsAngle;
    cloned->m_IsDegree = m_IsDegree;
    cloned->m_IsRadian = m_IsRadian;
    cloned->m_IsGradient = m_IsGradient;
    cloned->m_IsSlope = m_IsSlope;
    cloned->m_IsTangent = m_IsTangent;
    cloned->m_IsNormal = m_IsNormal;
    cloned->m_IsBinormal = m_IsBinormal;
    cloned->m_IsCurvature = m_IsCurvature;
    cloned->m_IsTorsion = m_IsTorsion;
    cloned->m_IsFrenet = m_IsFrenet;
    cloned->m_IsSerret = m_IsSerret;
    cloned->m_IsDarboux = m_IsDarboux;
    cloned->m_IsRicci = m_IsRicci;
    cloned->m_IsEinstein = m_IsEinstein;
    cloned->m_IsRiemann = m_IsRiemann;
    cloned->m_IsLevi = m_IsLevi;
    cloned->m_IsCivita = m_IsCivita;
    cloned->m_IsChristoffel = m_IsChristoffel;
    cloned->m_IsGamma = m_IsGamma;
    cloned->m_IsDelta = m_IsDelta;
    cloned->m_IsEpsilon = m_IsEpsilon;
    cloned->m_IsZeta = m_IsZeta;
    cloned->m_IsEta = m_IsEta;
    cloned->m_IsTheta = m_IsTheta;
    cloned->m_IsIota = m_IsIota;
    cloned->m_IsKappa = m_IsKappa;
    cloned->m_IsLambda = m_IsLambda;
    cloned->m_IsMu = m_IsMu;
    cloned->m_IsNu = m_IsNu;
    cloned->m_IsXi = m_IsXi;
    cloned->m_IsOmicron = m_IsOmicron;
    cloned->m_IsPi = m_IsPi;
    cloned->m_IsRho = m_IsRho;
    cloned->m_IsSigma = m_IsSigma;
    cloned->m_IsTau = m_IsTau;
    cloned->m_IsUpsilon = m_IsUpsilon;
    cloned->m_IsPhi = m_IsPhi;
    cloned->m_IsChi = m_IsChi;
    cloned->m_IsPsi = m_IsPsi;
    cloned->m_IsOmega = m_IsOmega;
    
    return cloned;
}

} // namespace Core
} // namespace SimpleLanguage

