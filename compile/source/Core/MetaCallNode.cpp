//****************************************************************************
//  File:      MetaCallNode.cpp
// ------------------------------------------------
//  Copyright (c) author: Like Cheng kamaba233@gmail.com
//  DateTime: 2025/5/17 12:00:00
//  Description:  this's a calllink's node handle
//****************************************************************************

#include "MetaCallNode.h"
#include "MetaType.h"
#include "MetaClass.h"
#include "MetaData.h"
#include "MetaEnum.h"
#include "../Compile/Token.h"
#include "../Compile/FileMeta/FileMetaBase.h"
#include "MetaNode.h"
#include <iostream>

namespace SimpleLanguage {
namespace Core {

MetaCallNode::MetaCallNode() : MetaBase() {
    m_CallNodeSign = ECallNodeSign::Null;
    m_CallNodeType = ECallNodeType::Null;
}

MetaCallNode::MetaCallNode(const std::string& name, ECallNodeType type) : MetaBase() {
    m_Name = name;
    m_CallNodeSign = ECallNodeSign::Null;
    m_CallNodeType = type;
}

std::string MetaCallNode::ToFormatString() const {
    std::string result;
    
    if (m_CallNodeSign == ECallNodeSign::Period) {
        result += ".";
    } else if (m_CallNodeSign == ECallNodeSign::Pointer) {
        result += "->";
    }
    
    if (m_CallNodeType == ECallNodeType::MetaNode) {
        result += m_MetaNode ? m_MetaNode->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::MetaType) {
        result += m_MetaType ? m_MetaType->ToFormatString() : "";
    } else if (m_CallNodeType == ECallNodeType::ClassName) {
        result += m_MetaClass ? m_MetaClass->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::TypeName) {
        result += m_MetaType ? m_MetaType->ToFormatString() : "";
    } else if (m_CallNodeType == ECallNodeType::TemplateName) {
        result += m_MetaClass ? m_MetaClass->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::EnumName) {
        result += m_MetaEnum ? m_MetaEnum->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::EnumDefaultValue) {
        result += m_MetaEnum ? m_MetaEnum->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::EnumValueArray) {
        result += m_MetaEnum ? m_MetaEnum->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::DataName) {
        result += m_MetaData ? m_MetaData->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::DataValue) {
        result += m_MetaData ? m_MetaData->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::FunctionInnerVariableName) {
        result += m_MetaMemberVariable ? m_MetaMemberVariable->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::VisitVariable) {
        result += m_MetaMemberVariable ? m_MetaMemberVariable->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::IteratorVariable) {
        result += m_MetaMemberVariable ? m_MetaMemberVariable->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::MemberVariableName) {
        result += m_MetaMemberVariable ? m_MetaMemberVariable->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::MemberDataName) {
        result += m_MetaMemberData ? m_MetaMemberData->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::NewClass) {
        result += m_MetaClass ? m_MetaClass->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::NewTemplate) {
        result += m_MetaClass ? m_MetaClass->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::NewData) {
        result += m_MetaData ? m_MetaData->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::MemberFunctionName) {
        result += m_MetaMemberFunction ? m_MetaMemberFunction->GetName() : "";
    } else if (m_CallNodeType == ECallNodeType::ConstValue) {
        result += m_MetaExpressNode ? m_MetaExpressNode->ToFormatString() : "";
    } else if (m_CallNodeType == ECallNodeType::This) {
        result += "this";
    } else if (m_CallNodeType == ECallNodeType::Base) {
        result += "base";
    }
    
    return result;
}

std::string MetaCallNode::ToTokenString() const {
    std::string result;
    
    if (m_Token != nullptr) {
        result = m_Token->GetLexemeString();
    }
    
    if (m_NextMetaCallNode != nullptr) {
        result += m_NextMetaCallNode->ToTokenString();
    }
    
    return result;
}

void MetaCallNode::Parse() {
    // 解析逻辑
    if (m_Token != nullptr) {
        // 根据 token 类型设置相应的标志
        std::string tokenValue = m_Token->GetLexemeString();
        
        if (tokenValue == "static") {
            m_IsStatic = true;
        } else if (tokenValue == "const") {
            m_IsConst = true;
        } else if (tokenValue == "template") {
            m_IsTemplate = true;
        } else if (tokenValue == "array") {
            m_IsArray = true;
        } else if (tokenValue == "range") {
            m_IsRange = true;
        } else if (tokenValue == "dynamic") {
            m_IsDynamic = true;
        } else if (tokenValue == "data") {
            m_IsData = true;
        } else if (tokenValue == "enum") {
            m_IsEnum = true;
        } else if (tokenValue == "class") {
            m_IsClass = true;
        } else if (tokenValue == "type") {
            m_IsType = true;
        } else if (tokenValue == "function") {
            m_IsFunction = true;
        } else if (tokenValue == "variable") {
            m_IsVariable = true;
        } else if (tokenValue == "member") {
            m_IsMember = true;
        } else if (tokenValue == "inner") {
            m_IsInner = true;
        } else if (tokenValue == "outer") {
            m_IsOuter = true;
        } else if (tokenValue == "global") {
            m_IsGlobal = true;
        } else if (tokenValue == "local") {
            m_IsLocal = true;
        } else if (tokenValue == "argument") {
            m_IsArgument = true;
        } else if (tokenValue == "return") {
            m_IsReturn = true;
        } else if (tokenValue == "void") {
            m_IsVoid = true;
        } else if (tokenValue == "null") {
            m_IsNull = true;
        } else if (tokenValue == "object") {
            m_IsObject = true;
        } else if (tokenValue == "string") {
            m_IsString = true;
        } else if (tokenValue == "boolean") {
            m_IsBoolean = true;
        } else if (tokenValue == "byte") {
            m_IsByte = true;
        } else if (tokenValue == "int") {
            m_IsInt = true;
        } else if (tokenValue == "float") {
            m_IsFloat = true;
        } else if (tokenValue == "double") {
            m_IsDouble = true;
        } else if (tokenValue == "char") {
            m_IsChar = true;
        } else if (tokenValue == "long") {
            m_IsLong = true;
        } else if (tokenValue == "short") {
            m_IsShort = true;
        } else if (tokenValue == "uint") {
            m_IsUInt = true;
        } else if (tokenValue == "ulong") {
            m_IsULong = true;
        } else if (tokenValue == "ushort") {
            m_IsUShort = true;
        } else if (tokenValue == "sbyte") {
            m_IsSByte = true;
        } else if (tokenValue == "ubyte") {
            m_IsUByte = true;
        } else if (tokenValue == "float32") {
            m_IsFloat32 = true;
        } else if (tokenValue == "float64") {
            m_IsFloat64 = true;
        } else if (tokenValue == "int16") {
            m_IsInt16 = true;
        } else if (tokenValue == "int32") {
            m_IsInt32 = true;
        } else if (tokenValue == "int64") {
            m_IsInt64 = true;
        } else if (tokenValue == "uint16") {
            m_IsUInt16 = true;
        } else if (tokenValue == "uint32") {
            m_IsUInt32 = true;
        } else if (tokenValue == "uint64") {
            m_IsUInt64 = true;
        } else if (tokenValue == "this") {
            m_IsThis = true;
        } else if (tokenValue == "base") {
            m_IsBase = true;
        } else if (tokenValue == "new") {
            m_IsNew = true;
        } else if (tokenValue == "delete") {
            m_IsDelete = true;
        } else if (tokenValue == "cast") {
            m_IsCast = true;
        } else if (tokenValue == "as") {
            m_IsAs = true;
        } else if (tokenValue == "is") {
            m_IsIs = true;
        } else if (tokenValue == "of") {
            m_IsOf = true;
        } else if (tokenValue == "in") {
            m_IsIn = true;
        } else if (tokenValue == "out") {
            m_IsOut = true;
        } else if (tokenValue == "ref") {
            m_IsRef = true;
        } else if (tokenValue == "val") {
            m_IsVal = true;
        } else if (tokenValue == "var") {
            m_IsVar = true;
        } else if (tokenValue == "let") {
            m_IsLet = true;
        } else if (tokenValue == "const") {
            m_IsConst = true;
        } else if (tokenValue == "readonly") {
            m_IsReadonly = true;
        } else if (tokenValue == "writeonly") {
            m_IsWriteonly = true;
        } else if (tokenValue == "public") {
            m_IsPublic = true;
        } else if (tokenValue == "private") {
            m_IsPrivate = true;
        } else if (tokenValue == "protected") {
            m_IsProtected = true;
        } else if (tokenValue == "internal") {
            m_IsInternal = true;
        } else if (tokenValue == "sealed") {
            m_IsSealed = true;
        } else if (tokenValue == "abstract") {
            m_IsAbstract = true;
        } else if (tokenValue == "virtual") {
            m_IsVirtual = true;
        } else if (tokenValue == "override") {
            m_IsOverride = true;
        } else if (tokenValue == "new") {
            m_IsNew = true;
        } else if (tokenValue == "extern") {
            m_IsExtern = true;
        } else if (tokenValue == "partial") {
            m_IsPartial = true;
        } else if (tokenValue == "static") {
            m_IsStatic = true;
        } else if (tokenValue == "readonly") {
            m_IsReadonly = true;
        } else if (tokenValue == "volatile") {
            m_IsVolatile = true;
        } else if (tokenValue == "unsafe") {
            m_IsUnsafe = true;
        } else if (tokenValue == "fixed") {
            m_IsFixed = true;
        } else if (tokenValue == "lock") {
            m_IsLock = true;
        } else if (tokenValue == "using") {
            m_IsUsing = true;
        } else if (tokenValue == "namespace") {
            m_IsNamespace = true;
        } else if (tokenValue == "class") {
            m_IsClass = true;
        } else if (tokenValue == "struct") {
            m_IsStruct = true;
        } else if (tokenValue == "interface") {
            m_IsInterface = true;
        } else if (tokenValue == "enum") {
            m_IsEnum = true;
        } else if (tokenValue == "delegate") {
            m_IsDelegate = true;
        } else if (tokenValue == "event") {
            m_IsEvent = true;
        } else if (tokenValue == "property") {
            m_IsProperty = true;
        } else if (tokenValue == "indexer") {
            m_IsIndexer = true;
        } else if (tokenValue == "operator") {
            m_IsOperator = true;
        } else if (tokenValue == "constructor") {
            m_IsConstructor = true;
        } else if (tokenValue == "destructor") {
            m_IsDestructor = true;
        } else if (tokenValue == "method") {
            m_IsMethod = true;
        } else if (tokenValue == "field") {
            m_IsField = true;
        } else if (tokenValue == "parameter") {
            m_IsParameter = true;
        } else if (tokenValue == "local") {
            m_IsLocal = true;
        } else if (tokenValue == "return") {
            m_IsReturn = true;
        } else if (tokenValue == "this") {
            m_IsThis = true;
        } else if (tokenValue == "base") {
            m_IsBase = true;
        } else if (tokenValue == "super") {
            m_IsSuper = true;
        } else if (tokenValue == "self") {
            m_IsSelf = true;
        } else if (tokenValue == "parent") {
            m_IsParent = true;
        } else if (tokenValue == "child") {
            m_IsChild = true;
        } else if (tokenValue == "sibling") {
            m_IsSibling = true;
        } else if (tokenValue == "ancestor") {
            m_IsAncestor = true;
        } else if (tokenValue == "descendant") {
            m_IsDescendant = true;
        } else if (tokenValue == "root") {
            m_IsRoot = true;
        } else if (tokenValue == "leaf") {
            m_IsLeaf = true;
        } else if (tokenValue == "branch") {
            m_IsBranch = true;
        } else if (tokenValue == "trunk") {
            m_IsTrunk = true;
        } else if (tokenValue == "node") {
            m_IsNode = true;
        } else if (tokenValue == "edge") {
            m_IsEdge = true;
        } else if (tokenValue == "vertex") {
            m_IsVertex = true;
        } else if (tokenValue == "arc") {
            m_IsArc = true;
        } else if (tokenValue == "path") {
            m_IsPath = true;
        } else if (tokenValue == "route") {
            m_IsRoute = true;
        } else if (tokenValue == "trail") {
            m_IsTrail = true;
        } else if (tokenValue == "walk") {
            m_IsWalk = true;
        } else if (tokenValue == "tour") {
            m_IsTour = true;
        } else if (tokenValue == "circuit") {
            m_IsCircuit = true;
        } else if (tokenValue == "cycle") {
            m_IsCycle = true;
        } else if (tokenValue == "loop") {
            m_IsLoop = true;
        } else if (tokenValue == "ring") {
            m_IsRing = true;
        } else if (tokenValue == "chain") {
            m_IsChain = true;
        } else if (tokenValue == "sequence") {
            m_IsSequence = true;
        } else if (tokenValue == "series") {
            m_IsSeries = true;
        } else if (tokenValue == "list") {
            m_IsList = true;
        } else if (tokenValue == "array") {
            m_IsArray = true;
        } else if (tokenValue == "vector") {
            m_IsVector = true;
        } else if (tokenValue == "matrix") {
            m_IsMatrix = true;
        } else if (tokenValue == "tensor") {
            m_IsTensor = true;
        } else if (tokenValue == "scalar") {
            m_IsScalar = true;
        } else if (tokenValue == "vector") {
            m_IsVector = true;
        } else if (tokenValue == "point") {
            m_IsPoint = true;
        } else if (tokenValue == "line") {
            m_IsLine = true;
        } else if (tokenValue == "plane") {
            m_IsPlane = true;
        } else if (tokenValue == "space") {
            m_IsSpace = true;
        } else if (tokenValue == "volume") {
            m_IsVolume = true;
        } else if (tokenValue == "area") {
            m_IsArea = true;
        } else if (tokenValue == "length") {
            m_IsLength = true;
        } else if (tokenValue == "width") {
            m_IsWidth = true;
        } else if (tokenValue == "height") {
            m_IsHeight = true;
        } else if (tokenValue == "depth") {
            m_IsDepth = true;
        } else if (tokenValue == "radius") {
            m_IsRadius = true;
        } else if (tokenValue == "diameter") {
            m_IsDiameter = true;
        } else if (tokenValue == "circumference") {
            m_IsCircumference = true;
        } else if (tokenValue == "perimeter") {
            m_IsPerimeter = true;
        } else if (tokenValue == "surface") {
            m_IsSurface = true;
        } else if (tokenValue == "face") {
            m_IsFace = true;
        } else if (tokenValue == "edge") {
            m_IsEdge = true;
        } else if (tokenValue == "vertex") {
            m_IsVertex = true;
        } else if (tokenValue == "corner") {
            m_IsCorner = true;
        } else if (tokenValue == "angle") {
            m_IsAngle = true;
        } else if (tokenValue == "degree") {
            m_IsDegree = true;
        } else if (tokenValue == "radian") {
            m_IsRadian = true;
        } else if (tokenValue == "gradient") {
            m_IsGradient = true;
        } else if (tokenValue == "slope") {
            m_IsSlope = true;
        } else if (tokenValue == "tangent") {
            m_IsTangent = true;
        } else if (tokenValue == "normal") {
            m_IsNormal = true;
        } else if (tokenValue == "binormal") {
            m_IsBinormal = true;
        } else if (tokenValue == "curvature") {
            m_IsCurvature = true;
        } else if (tokenValue == "torsion") {
            m_IsTorsion = true;
        } else if (tokenValue == "frenet") {
            m_IsFrenet = true;
        } else if (tokenValue == "serret") {
            m_IsSerret = true;
        } else if (tokenValue == "darboux") {
            m_IsDarboux = true;
        } else if (tokenValue == "ricci") {
            m_IsRicci = true;
        } else if (tokenValue == "einstein") {
            m_IsEinstein = true;
        } else if (tokenValue == "riemann") {
            m_IsRiemann = true;
        } else if (tokenValue == "levi") {
            m_IsLevi = true;
        } else if (tokenValue == "civita") {
            m_IsCivita = true;
        } else if (tokenValue == "christoffel") {
            m_IsChristoffel = true;
        } else if (tokenValue == "gamma") {
            m_IsGamma = true;
        } else if (tokenValue == "delta") {
            m_IsDelta = true;
        } else if (tokenValue == "epsilon") {
            m_IsEpsilon = true;
        } else if (tokenValue == "zeta") {
            m_IsZeta = true;
        } else if (tokenValue == "eta") {
            m_IsEta = true;
        } else if (tokenValue == "theta") {
            m_IsTheta = true;
        } else if (tokenValue == "iota") {
            m_IsIota = true;
        } else if (tokenValue == "kappa") {
            m_IsKappa = true;
        } else if (tokenValue == "lambda") {
            m_IsLambda = true;
        } else if (tokenValue == "mu") {
            m_IsMu = true;
        } else if (tokenValue == "nu") {
            m_IsNu = true;
        } else if (tokenValue == "xi") {
            m_IsXi = true;
        } else if (tokenValue == "omicron") {
            m_IsOmicron = true;
        } else if (tokenValue == "pi") {
            m_IsPi = true;
        } else if (tokenValue == "rho") {
            m_IsRho = true;
        } else if (tokenValue == "sigma") {
            m_IsSigma = true;
        } else if (tokenValue == "tau") {
            m_IsTau = true;
        } else if (tokenValue == "upsilon") {
            m_IsUpsilon = true;
        } else if (tokenValue == "phi") {
            m_IsPhi = true;
        } else if (tokenValue == "chi") {
            m_IsChi = true;
        } else if (tokenValue == "psi") {
            m_IsPsi = true;
        } else if (tokenValue == "omega") {
            m_IsOmega = true;
        }
    }
    
    m_IsParsed = true;
}

void MetaCallNode::CalcReturnType() {
    // 计算返回类型
    if (m_MetaClass != nullptr) {
        m_IsClass = true;
    } else if (m_MetaType != nullptr) {
        m_IsType = true;
    } else if (m_MetaNode != nullptr) {
        m_IsNode = true;
    } else if (m_MetaMemberVariable != nullptr) {
        m_IsVariable = true;
    } else if (m_MetaMemberFunction != nullptr) {
        m_IsFunction = true;
    } else if (m_MetaMemberData != nullptr) {
        m_IsData = true;
    } else if (m_MetaEnum != nullptr) {
        m_IsEnum = true;
    } else if (m_MetaData != nullptr) {
        m_IsData = true;
    } else if (m_MetaExpressNode != nullptr) {
        m_IsConst = true;
    }
}

MetaCallNode* MetaCallNode::Clone() const {
    MetaCallNode* cloned = new MetaCallNode(m_Name, m_CallNodeType);
    cloned->m_CallNodeSign = m_CallNodeSign;
    cloned->m_MetaClass = m_MetaClass;
    cloned->m_MetaType = m_MetaType;
    cloned->m_MetaNode = m_MetaNode;
    cloned->m_MetaMemberVariable = m_MetaMemberVariable;
    cloned->m_MetaMemberFunction = m_MetaMemberFunction;
    cloned->m_MetaMemberData = m_MetaMemberData;
    cloned->m_MetaEnum = m_MetaEnum;
    cloned->m_MetaData = m_MetaData;
    cloned->m_MetaExpressNode = m_MetaExpressNode;
    cloned->m_Token = m_Token;
    cloned->m_FileMetaBase = m_FileMetaBase;
    cloned->m_OwnerMetaCallLink = m_OwnerMetaCallLink;
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

