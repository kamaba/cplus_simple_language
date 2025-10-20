//****************************************************************************
//  File:      MetaVisitCall.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/6/12 12:00:00
//  Description:  create visit variable or method call!
//****************************************************************************

#pragma once

#include "MetaBase.h"
#include "MetaVariable.h"
#include "MetaClass.h"
#include "MetaType.h"
#include "MetaMemberFunction.h"
#include "MetaExpressNode/MetaExpressNode.h"
#include "../Compile/AllowUseSettings.h"
#include <string>
#include <vector>
#include <memory>

namespace SimpleLanguage {
namespace Core {

// 前向声明
class MetaFunction;
class MetaInputParamCollection;
class MetaExpressNode;

class MetaMethodCall {
public:
    MetaMethodCall(MetaClass* staticMc, const std::vector<MetaType*>& staticMmitList, MetaFunction* fun, const std::vector<MetaType*>& mpipList, MetaInputParamCollection* paramCollection, MetaVariable* loadMv, MetaVariable* storeMv);
    virtual ~MetaMethodCall() = default;

    // 属性访问器
    MetaVariable* getLoadMetaVariable() const { return m_LoadMetaVariable; }
    MetaVariable* getStoreMetaVariable() const { return m_StoreMetaVariable; }
    MetaClass* getStaticCallerMetaClass() const { return m_StaticCallerMetaClass; }
    const std::vector<MetaType*>& getStaticMetaClassInputTemplateList() const { return m_StaticMetaClassInputTemplateList; }
    MetaFunction* getFunction() const { return m_VMCallMetaFunction; }
    MetaMemberFunction* getMetaMemberFunction() const { return m_MetaMemberFunction; }
    const std::vector<MetaExpressNode*>& getMetaInputParamList() const { return m_MetaInputParamList; }
    const std::vector<MetaType*>& getMetaFunctionInputTemplateList() const { return m_MetaFunctionInputTemplateList; }

    // 方法
    void Parse(AllowUseSettings* auc);
    void CalcReturnType();
    std::string ToFormatString() const;
    std::string ToTokenString() const;
    MetaMethodCall* Clone() const;

protected:
    MetaVariable* m_LoadMetaVariable = nullptr;
    MetaVariable* m_StoreMetaVariable = nullptr;
    MetaClass* m_StaticCallerMetaClass = nullptr;
    std::vector<MetaType*> m_StaticMetaClassInputTemplateList;
    MetaFunction* m_VMCallMetaFunction = nullptr;
    MetaMemberFunction* m_MetaMemberFunction = nullptr;
    std::vector<MetaExpressNode*> m_MetaInputParamList;
    std::vector<MetaType*> m_MetaFunctionInputTemplateList;
};

class MetaVisitCall : public MetaBase {
public:
    MetaVisitCall();
    MetaVisitCall(const std::string& name);
    virtual ~MetaVisitCall() = default;

    // 属性访问器
    MetaMethodCall* GetMetaMethodCall() const { return m_MetaMethodCall; }
    MetaVariable* GetMetaVariable() const { return m_MetaVariable; }
    MetaClass* GetMetaClass() const { return m_MetaClass; }
    MetaType* GetMetaType() const { return m_MetaType; }
    MetaMemberFunction* GetMetaMemberFunction() const { return m_MetaMemberFunction; }
    MetaMemberVariable* GetMetaMemberVariable() const { return m_MetaMemberVariable; }
    MetaMemberData* GetMetaMemberData() const { return m_MetaMemberData; }
    MetaEnum* GetMetaEnum() const { return m_MetaEnum; }
    MetaData* GetMetaData() const { return m_MetaData; }
    MetaExpressNode* GetMetaExpressNode() const { return m_MetaExpressNode; }
    bool IsParsed() const { return m_IsParsed; }
    bool IsStatic() const { return m_IsStatic; }
    bool IsConst() const { return m_IsConst; }
    bool IsTemplate() const { return m_IsTemplate; }
    bool IsArray() const { return m_IsArray; }
    bool IsRange() const { return m_IsRange; }
    bool IsDynamic() const { return m_IsDynamic; }
    bool IsData() const { return m_IsData; }
    bool IsEnum() const { return m_IsEnum; }
    bool IsClass() const { return m_IsClass; }
    bool IsType() const { return m_IsType; }
    bool IsFunction() const { return m_IsFunction; }
    bool IsVariable() const { return m_IsVariable; }
    bool IsMember() const { return m_IsMember; }
    bool IsInner() const { return m_IsInner; }
    bool IsOuter() const { return m_IsOuter; }
    bool IsGlobal() const { return m_IsGlobal; }
    bool IsLocal() const { return m_IsLocal; }
    bool IsArgument() const { return m_IsArgument; }
    bool IsReturn() const { return m_IsReturn; }
    bool IsVoid() const { return m_IsVoid; }
    bool IsNull() const { return m_IsNull; }
    bool IsObject() const { return m_IsObject; }
    bool IsString() const { return m_IsString; }
    bool IsBoolean() const { return m_IsBoolean; }
    bool IsByte() const { return m_IsByte; }
    bool IsInt() const { return m_IsInt; }
    bool IsFloat() const { return m_IsFloat; }
    bool IsDouble() const { return m_IsDouble; }
    bool IsChar() const { return m_IsChar; }
    bool IsLong() const { return m_IsLong; }
    bool IsShort() const { return m_IsShort; }
    bool IsUInt() const { return m_IsUInt; }
    bool IsULong() const { return m_IsULong; }
    bool IsUShort() const { return m_IsUShort; }
    bool IsSByte() const { return m_IsSByte; }
    bool IsUByte() const { return m_IsUByte; }
    bool IsFloat32() const { return m_IsFloat32; }
    bool IsFloat64() const { return m_IsFloat64; }
    bool IsInt16() const { return m_IsInt16; }
    bool IsInt32() const { return m_IsInt32; }
    bool IsInt64() const { return m_IsInt64; }
    bool IsUInt16() const { return m_IsUInt16; }
    bool IsUInt32() const { return m_IsUInt32; }
    bool IsUInt64() const { return m_IsUInt64; }
    bool IsThis() const { return m_IsThis; }
    bool IsBase() const { return m_IsBase; }
    bool IsNew() const { return m_IsNew; }
    bool IsDelete() const { return m_IsDelete; }
    bool IsCast() const { return m_IsCast; }
    bool IsAs() const { return m_IsAs; }
    bool IsIs() const { return m_IsIs; }
    bool IsOf() const { return m_IsOf; }
    bool IsIn() const { return m_IsIn; }
    bool IsOut() const { return m_IsOut; }
    bool IsRef() const { return m_IsRef; }
    bool IsVal() const { return m_IsVal; }
    bool IsVar() const { return m_IsVar; }
    bool IsLet() const { return m_IsLet; }
    bool IsConst() const { return m_IsConst; }
    bool IsReadonly() const { return m_IsReadonly; }
    bool IsWriteonly() const { return m_IsWriteonly; }
    bool IsPublic() const { return m_IsPublic; }
    bool IsPrivate() const { return m_IsPrivate; }
    bool IsProtected() const { return m_IsProtected; }
    bool IsInternal() const { return m_IsInternal; }
    bool IsSealed() const { return m_IsSealed; }
    bool IsAbstract() const { return m_IsAbstract; }
    bool IsVirtual() const { return m_IsVirtual; }
    bool IsOverride() const { return m_IsOverride; }
    bool IsNew() const { return m_IsNew; }
    bool IsExtern() const { return m_IsExtern; }
    bool IsPartial() const { return m_IsPartial; }
    bool IsStatic() const { return m_IsStatic; }
    bool IsReadonly() const { return m_IsReadonly; }
    bool IsVolatile() const { return m_IsVolatile; }
    bool IsUnsafe() const { return m_IsUnsafe; }
    bool IsFixed() const { return m_IsFixed; }
    bool IsLock() const { return m_IsLock; }
    bool IsUsing() const { return m_IsUsing; }
    bool IsNamespace() const { return m_IsNamespace; }
    bool IsClass() const { return m_IsClass; }
    bool IsStruct() const { return m_IsStruct; }
    bool IsInterface() const { return m_IsInterface; }
    bool IsEnum() const { return m_IsEnum; }
    bool IsDelegate() const { return m_IsDelegate; }
    bool IsEvent() const { return m_IsEvent; }
    bool IsProperty() const { return m_IsProperty; }
    bool IsIndexer() const { return m_IsIndexer; }
    bool IsOperator() const { return m_IsOperator; }
    bool IsConstructor() const { return m_IsConstructor; }
    bool IsDestructor() const { return m_IsDestructor; }
    bool IsMethod() const { return m_IsMethod; }
    bool IsField() const { return m_IsField; }
    bool IsParameter() const { return m_IsParameter; }
    bool IsLocal() const { return m_IsLocal; }
    bool IsReturn() const { return m_IsReturn; }
    bool IsThis() const { return m_IsThis; }
    bool IsBase() const { return m_IsBase; }
    bool IsSuper() const { return m_IsSuper; }
    bool IsSelf() const { return m_IsSelf; }
    bool IsParent() const { return m_IsParent; }
    bool IsChild() const { return m_IsChild; }
    bool IsSibling() const { return m_IsSibling; }
    bool IsAncestor() const { return m_IsAncestor; }
    bool IsDescendant() const { return m_IsDescendant; }
    bool IsRoot() const { return m_IsRoot; }
    bool IsLeaf() const { return m_IsLeaf; }
    bool IsBranch() const { return m_IsBranch; }
    bool IsTrunk() const { return m_IsTrunk; }
    bool IsNode() const { return m_IsNode; }
    bool IsEdge() const { return m_IsEdge; }
    bool IsVertex() const { return m_IsVertex; }
    bool IsArc() const { return m_IsArc; }
    bool IsPath() const { return m_IsPath; }
    bool IsRoute() const { return m_IsRoute; }
    bool IsTrail() const { return m_IsTrail; }
    bool IsWalk() const { return m_IsWalk; }
    bool IsTour() const { return m_IsTour; }
    bool IsCircuit() const { return m_IsCircuit; }
    bool IsCycle() const { return m_IsCycle; }
    bool IsLoop() const { return m_IsLoop; }
    bool IsRing() const { return m_IsRing; }
    bool IsChain() const { return m_IsChain; }
    bool IsSequence() const { return m_IsSequence; }
    bool IsSeries() const { return m_IsSeries; }
    bool IsList() const { return m_IsList; }
    bool IsArray() const { return m_IsArray; }
    bool IsVector() const { return m_IsVector; }
    bool IsMatrix() const { return m_IsMatrix; }
    bool IsTensor() const { return m_IsTensor; }
    bool IsScalar() const { return m_IsScalar; }
    bool IsVector() const { return m_IsVector; }
    bool IsPoint() const { return m_IsPoint; }
    bool IsLine() const { return m_IsLine; }
    bool IsPlane() const { return m_IsPlane; }
    bool IsSpace() const { return m_IsSpace; }
    bool IsVolume() const { return m_IsVolume; }
    bool IsArea() const { return m_IsArea; }
    bool IsLength() const { return m_IsLength; }
    bool IsWidth() const { return m_IsWidth; }
    bool IsHeight() const { return m_IsHeight; }
    bool IsDepth() const { return m_IsDepth; }
    bool IsRadius() const { return m_IsRadius; }
    bool IsDiameter() const { return m_IsDiameter; }
    bool IsCircumference() const { return m_IsCircumference; }
    bool IsPerimeter() const { return m_IsPerimeter; }
    bool IsSurface() const { return m_IsSurface; }
    bool IsFace() const { return m_IsFace; }
    bool IsEdge() const { return m_IsEdge; }
    bool IsVertex() const { return m_IsVertex; }
    bool IsCorner() const { return m_IsCorner; }
    bool IsAngle() const { return m_IsAngle; }
    bool IsDegree() const { return m_IsDegree; }
    bool IsRadian() const { return m_IsRadian; }
    bool IsGradient() const { return m_IsGradient; }
    bool IsSlope() const { return m_IsSlope; }
    bool IsTangent() const { return m_IsTangent; }
    bool IsNormal() const { return m_IsNormal; }
    bool IsBinormal() const { return m_IsBinormal; }
    bool IsCurvature() const { return m_IsCurvature; }
    bool IsTorsion() const { return m_IsTorsion; }
    bool IsFrenet() const { return m_IsFrenet; }
    bool IsSerret() const { return m_IsSerret; }
    bool IsDarboux() const { return m_IsDarboux; }
    bool IsRicci() const { return m_IsRicci; }
    bool IsEinstein() const { return m_IsEinstein; }
    bool IsRiemann() const { return m_IsRiemann; }
    bool IsLevi() const { return m_IsLevi; }
    bool IsCivita() const { return m_IsCivita; }
    bool IsChristoffel() const { return m_IsChristoffel; }
    bool IsGamma() const { return m_IsGamma; }
    bool IsDelta() const { return m_IsDelta; }
    bool IsEpsilon() const { return m_IsEpsilon; }
    bool IsZeta() const { return m_IsZeta; }
    bool IsEta() const { return m_IsEta; }
    bool IsTheta() const { return m_IsTheta; }
    bool IsIota() const { return m_IsIota; }
    bool IsKappa() const { return m_IsKappa; }
    bool IsLambda() const { return m_IsLambda; }
    bool IsMu() const { return m_IsMu; }
    bool IsNu() const { return m_IsNu; }
    bool IsXi() const { return m_IsXi; }
    bool IsOmicron() const { return m_IsOmicron; }
    bool IsPi() const { return m_IsPi; }
    bool IsRho() const { return m_IsRho; }
    bool IsSigma() const { return m_IsSigma; }
    bool IsTau() const { return m_IsTau; }
    bool IsUpsilon() const { return m_IsUpsilon; }
    bool IsPhi() const { return m_IsPhi; }
    bool IsChi() const { return m_IsChi; }
    bool IsPsi() const { return m_IsPsi; }
    bool IsOmega() const { return m_IsOmega; }

    // 设置方法
    void setMetaMethodCall(MetaMethodCall* metaMethodCall) { m_MetaMethodCall = metaMethodCall; }
    void setMetaVariable(MetaVariable* metaVariable) { m_MetaVariable = metaVariable; }
    void setMetaClass(MetaClass* metaClass) { m_MetaClass = metaClass; }
    void setMetaType(MetaType* metaType) { m_MetaType = metaType; }
    void setMetaMemberFunction(MetaMemberFunction* metaMemberFunction) { m_MetaMemberFunction = metaMemberFunction; }
    void setMetaMemberVariable(MetaMemberVariable* metaMemberVariable) { m_MetaMemberVariable = metaMemberVariable; }
    void setMetaMemberData(MetaMemberData* metaMemberData) { m_MetaMemberData = metaMemberData; }
    void setMetaEnum(MetaEnum* metaEnum) { m_MetaEnum = metaEnum; }
    void setMetaData(MetaData* metaData) { m_MetaData = metaData; }
    void setMetaExpressNode(MetaExpressNode* metaExpressNode) { m_MetaExpressNode = metaExpressNode; }
    void setParsed(bool parsed) { m_IsParsed = parsed; }
    void setStatic(bool isStatic) { m_IsStatic = isStatic; }
    void setConst(bool isConst) { m_IsConst = isConst; }
    void setTemplate(bool isTemplate) { m_IsTemplate = isTemplate; }
    void setArray(bool isArray) { m_IsArray = isArray; }
    void setRange(bool isRange) { m_IsRange = isRange; }
    void setDynamic(bool isDynamic) { m_IsDynamic = isDynamic; }
    void setData(bool isData) { m_IsData = isData; }
    void setEnum(bool isEnum) { m_IsEnum = isEnum; }
    void setClass(bool isClass) { m_IsClass = isClass; }
    void setType(bool isType) { m_IsType = isType; }
    void setFunction(bool isFunction) { m_IsFunction = isFunction; }
    void setVariable(bool isVariable) { m_IsVariable = isVariable; }
    void setMember(bool isMember) { m_IsMember = isMember; }
    void setInner(bool isInner) { m_IsInner = isInner; }
    void setOuter(bool isOuter) { m_IsOuter = isOuter; }
    void setGlobal(bool isGlobal) { m_IsGlobal = isGlobal; }
    void setLocal(bool isLocal) { m_IsLocal = isLocal; }
    void setArgument(bool isArgument) { m_IsArgument = isArgument; }
    void setReturn(bool isReturn) { m_IsReturn = isReturn; }
    void setVoid(bool isVoid) { m_IsVoid = isVoid; }
    void setNull(bool isNull) { m_IsNull = isNull; }
    void setObject(bool isObject) { m_IsObject = isObject; }
    void setString(bool isString) { m_IsString = isString; }
    void setBoolean(bool isBoolean) { m_IsBoolean = isBoolean; }
    void setByte(bool isByte) { m_IsByte = isByte; }
    void setInt(bool isInt) { m_IsInt = isInt; }
    void setFloat(bool isFloat) { m_IsFloat = isFloat; }
    void setDouble(bool isDouble) { m_IsDouble = isDouble; }
    void setChar(bool isChar) { m_IsChar = isChar; }
    void setLong(bool isLong) { m_IsLong = isLong; }
    void setShort(bool isShort) { m_IsShort = isShort; }
    void setUInt(bool isUInt) { m_IsUInt = isUInt; }
    void setULong(bool isULong) { m_IsULong = isULong; }
    void setUShort(bool isUShort) { m_IsUShort = isUShort; }
    void setSByte(bool isSByte) { m_IsSByte = isSByte; }
    void setUByte(bool isUByte) { m_IsUByte = isUByte; }
    void setFloat32(bool isFloat32) { m_IsFloat32 = isFloat32; }
    void setFloat64(bool isFloat64) { m_IsFloat64 = isFloat64; }
    void setInt16(bool isInt16) { m_IsInt16 = isInt16; }
    void setInt32(bool isInt32) { m_IsInt32 = isInt32; }
    void setInt64(bool isInt64) { m_IsInt64 = isInt64; }
    void setUInt16(bool isUInt16) { m_IsUInt16 = isUInt16; }
    void setUInt32(bool isUInt32) { m_IsUInt32 = isUInt32; }
    void setUInt64(bool isUInt64) { m_IsUInt64 = isUInt64; }
    void setThis(bool isThis) { m_IsThis = isThis; }
    void setBase(bool isBase) { m_IsBase = isBase; }
    void setNew(bool isNew) { m_IsNew = isNew; }
    void setDelete(bool isDelete) { m_IsDelete = isDelete; }
    void setCast(bool isCast) { m_IsCast = isCast; }
    void setAs(bool isAs) { m_IsAs = isAs; }
    void setIs(bool isIs) { m_IsIs = isIs; }
    void setOf(bool isOf) { m_IsOf = isOf; }
    void setIn(bool isIn) { m_IsIn = isIn; }
    void setOut(bool isOut) { m_IsOut = isOut; }
    void setRef(bool isRef) { m_IsRef = isRef; }
    void setVal(bool isVal) { m_IsVal = isVal; }
    void setVar(bool isVar) { m_IsVar = isVar; }
    void setLet(bool isLet) { m_IsLet = isLet; }
    void setConst(bool isConst) { m_IsConst = isConst; }
    void setReadonly(bool isReadonly) { m_IsReadonly = isReadonly; }
    void setWriteonly(bool isWriteonly) { m_IsWriteonly = isWriteonly; }
    void setPublic(bool isPublic) { m_IsPublic = isPublic; }
    void setPrivate(bool isPrivate) { m_IsPrivate = isPrivate; }
    void setProtected(bool isProtected) { m_IsProtected = isProtected; }
    void setInternal(bool isInternal) { m_IsInternal = isInternal; }
    void setSealed(bool isSealed) { m_IsSealed = isSealed; }
    void setAbstract(bool isAbstract) { m_IsAbstract = isAbstract; }
    void setVirtual(bool isVirtual) { m_IsVirtual = isVirtual; }
    void setOverride(bool isOverride) { m_IsOverride = isOverride; }
    void setNew(bool isNew) { m_IsNew = isNew; }
    void setExtern(bool isExtern) { m_IsExtern = isExtern; }
    void setPartial(bool isPartial) { m_IsPartial = isPartial; }
    void setStatic(bool isStatic) { m_IsStatic = isStatic; }
    void setReadonly(bool isReadonly) { m_IsReadonly = isReadonly; }
    void setVolatile(bool isVolatile) { m_IsVolatile = isVolatile; }
    void setUnsafe(bool isUnsafe) { m_IsUnsafe = isUnsafe; }
    void setFixed(bool isFixed) { m_IsFixed = isFixed; }
    void setLock(bool isLock) { m_IsLock = isLock; }
    void setUsing(bool isUsing) { m_IsUsing = isUsing; }
    void setNamespace(bool isNamespace) { m_IsNamespace = isNamespace; }
    void setClass(bool isClass) { m_IsClass = isClass; }
    void setStruct(bool isStruct) { m_IsStruct = isStruct; }
    void setInterface(bool isInterface) { m_IsInterface = isInterface; }
    void setEnum(bool isEnum) { m_IsEnum = isEnum; }
    void setDelegate(bool isDelegate) { m_IsDelegate = isDelegate; }
    void setEvent(bool isEvent) { m_IsEvent = isEvent; }
    void setProperty(bool isProperty) { m_IsProperty = isProperty; }
    void setIndexer(bool isIndexer) { m_IsIndexer = isIndexer; }
    void setOperator(bool isOperator) { m_IsOperator = isOperator; }
    void setConstructor(bool isConstructor) { m_IsConstructor = isConstructor; }
    void setDestructor(bool isDestructor) { m_IsDestructor = isDestructor; }
    void setMethod(bool isMethod) { m_IsMethod = isMethod; }
    void setField(bool isField) { m_IsField = isField; }
    void setParameter(bool isParameter) { m_IsParameter = isParameter; }
    void setLocal(bool isLocal) { m_IsLocal = isLocal; }
    void setReturn(bool isReturn) { m_IsReturn = isReturn; }
    void setThis(bool isThis) { m_IsThis = isThis; }
    void setBase(bool isBase) { m_IsBase = isBase; }
    void setSuper(bool isSuper) { m_IsSuper = isSuper; }
    void setSelf(bool isSelf) { m_IsSelf = isSelf; }
    void setParent(bool isParent) { m_IsParent = isParent; }
    void setChild(bool isChild) { m_IsChild = isChild; }
    void setSibling(bool isSibling) { m_IsSibling = isSibling; }
    void setAncestor(bool isAncestor) { m_IsAncestor = isAncestor; }
    void setDescendant(bool isDescendant) { m_IsDescendant = isDescendant; }
    void setRoot(bool isRoot) { m_IsRoot = isRoot; }
    void setLeaf(bool isLeaf) { m_IsLeaf = isLeaf; }
    void setBranch(bool isBranch) { m_IsBranch = isBranch; }
    void setTrunk(bool isTrunk) { m_IsTrunk = isTrunk; }
    void setNode(bool isNode) { m_IsNode = isNode; }
    void setEdge(bool isEdge) { m_IsEdge = isEdge; }
    void setVertex(bool isVertex) { m_IsVertex = isVertex; }
    void setArc(bool isArc) { m_IsArc = isArc; }
    void setPath(bool isPath) { m_IsPath = isPath; }
    void setRoute(bool isRoute) { m_IsRoute = isRoute; }
    void setTrail(bool isTrail) { m_IsTrail = isTrail; }
    void setWalk(bool isWalk) { m_IsWalk = isWalk; }
    void setTour(bool isTour) { m_IsTour = isTour; }
    void setCircuit(bool isCircuit) { m_IsCircuit = isCircuit; }
    void setCycle(bool isCycle) { m_IsCycle = isCycle; }
    void setLoop(bool isLoop) { m_IsLoop = isLoop; }
    void setRing(bool isRing) { m_IsRing = isRing; }
    void setChain(bool isChain) { m_IsChain = isChain; }
    void setSequence(bool isSequence) { m_IsSequence = isSequence; }
    void setSeries(bool isSeries) { m_IsSeries = isSeries; }
    void setList(bool isList) { m_IsList = isList; }
    void setArray(bool isArray) { m_IsArray = isArray; }
    void setVector(bool isVector) { m_IsVector = isVector; }
    void setMatrix(bool isMatrix) { m_IsMatrix = isMatrix; }
    void setTensor(bool isTensor) { m_IsTensor = isTensor; }
    void setScalar(bool isScalar) { m_IsScalar = isScalar; }
    void setVector(bool isVector) { m_IsVector = isVector; }
    void setPoint(bool isPoint) { m_IsPoint = isPoint; }
    void setLine(bool isLine) { m_IsLine = isLine; }
    void setPlane(bool isPlane) { m_IsPlane = isPlane; }
    void setSpace(bool isSpace) { m_IsSpace = isSpace; }
    void setVolume(bool isVolume) { m_IsVolume = isVolume; }
    void setArea(bool isArea) { m_IsArea = isArea; }
    void setLength(bool isLength) { m_IsLength = isLength; }
    void setWidth(bool isWidth) { m_IsWidth = isWidth; }
    void setHeight(bool isHeight) { m_IsHeight = isHeight; }
    void setDepth(bool isDepth) { m_IsDepth = isDepth; }
    void setRadius(bool isRadius) { m_IsRadius = isRadius; }
    void setDiameter(bool isDiameter) { m_IsDiameter = isDiameter; }
    void setCircumference(bool isCircumference) { m_IsCircumference = isCircumference; }
    void setPerimeter(bool isPerimeter) { m_IsPerimeter = isPerimeter; }
    void setSurface(bool isSurface) { m_IsSurface = isSurface; }
    void setFace(bool isFace) { m_IsFace = isFace; }
    void setEdge(bool isEdge) { m_IsEdge = isEdge; }
    void setVertex(bool isVertex) { m_IsVertex = isVertex; }
    void setCorner(bool isCorner) { m_IsCorner = isCorner; }
    void setAngle(bool isAngle) { m_IsAngle = isAngle; }
    void setDegree(bool isDegree) { m_IsDegree = isDegree; }
    void setRadian(bool isRadian) { m_IsRadian = isRadian; }
    void setGradient(bool isGradient) { m_IsGradient = isGradient; }
    void setSlope(bool isSlope) { m_IsSlope = isSlope; }
    void setTangent(bool isTangent) { m_IsTangent = isTangent; }
    void setNormal(bool isNormal) { m_IsNormal = isNormal; }
    void setBinormal(bool isBinormal) { m_IsBinormal = isBinormal; }
    void setCurvature(bool isCurvature) { m_IsCurvature = isCurvature; }
    void setTorsion(bool isTorsion) { m_IsTorsion = isTorsion; }
    void setFrenet(bool isFrenet) { m_IsFrenet = isFrenet; }
    void setSerret(bool isSerret) { m_IsSerret = isSerret; }
    void setDarboux(bool isDarboux) { m_IsDarboux = isDarboux; }
    void setRicci(bool isRicci) { m_IsRicci = isRicci; }
    void setEinstein(bool isEinstein) { m_IsEinstein = isEinstein; }
    void setRiemann(bool isRiemann) { m_IsRiemann = isRiemann; }
    void setLevi(bool isLevi) { m_IsLevi = isLevi; }
    void setCivita(bool isCivita) { m_IsCivita = isCivita; }
    void setChristoffel(bool isChristoffel) { m_IsChristoffel = isChristoffel; }
    void setGamma(bool isGamma) { m_IsGamma = isGamma; }
    void setDelta(bool isDelta) { m_IsDelta = isDelta; }
    void setEpsilon(bool isEpsilon) { m_IsEpsilon = isEpsilon; }
    void setZeta(bool isZeta) { m_IsZeta = isZeta; }
    void setEta(bool isEta) { m_IsEta = isEta; }
    void setTheta(bool isTheta) { m_IsTheta = isTheta; }
    void setIota(bool isIota) { m_IsIota = isIota; }
    void setKappa(bool isKappa) { m_IsKappa = isKappa; }
    void setLambda(bool isLambda) { m_IsLambda = isLambda; }
    void setMu(bool isMu) { m_IsMu = isMu; }
    void setNu(bool isNu) { m_IsNu = isNu; }
    void setXi(bool isXi) { m_IsXi = isXi; }
    void setOmicron(bool isOmicron) { m_IsOmicron = isOmicron; }
    void setPi(bool isPi) { m_IsPi = isPi; }
    void setRho(bool isRho) { m_IsRho = isRho; }
    void setSigma(bool isSigma) { m_IsSigma = isSigma; }
    void setTau(bool isTau) { m_IsTau = isTau; }
    void setUpsilon(bool isUpsilon) { m_IsUpsilon = isUpsilon; }
    void setPhi(bool isPhi) { m_IsPhi = isPhi; }
    void setChi(bool isChi) { m_IsChi = isChi; }
    void setPsi(bool isPsi) { m_IsPsi = isPsi; }
    void setOmega(bool isOmega) { m_IsOmega = isOmega; }

    // 方法
    virtual std::string toFormatString() const override;
    virtual std::string toTokenString() const;
    virtual void parse();
    virtual void calcReturnType();
    virtual MetaVisitCall* clone() const;

private:
    MetaMethodCall* m_MetaMethodCall = nullptr;
    MetaVariable* m_MetaVariable = nullptr;
    MetaClass* m_MetaClass = nullptr;
    MetaType* m_MetaType = nullptr;
    MetaMemberFunction* m_MetaMemberFunction = nullptr;
    MetaMemberVariable* m_MetaMemberVariable = nullptr;
    MetaMemberData* m_MetaMemberData = nullptr;
    MetaEnum* m_MetaEnum = nullptr;
    MetaData* m_MetaData = nullptr;
    MetaExpressNode* m_MetaExpressNode = nullptr;
    bool m_IsParsed = false;
    bool m_IsStatic = false;
    bool m_IsConst = false;
    bool m_IsTemplate = false;
    bool m_IsArray = false;
    bool m_IsRange = false;
    bool m_IsDynamic = false;
    bool m_IsData = false;
    bool m_IsEnum = false;
    bool m_IsClass = false;
    bool m_IsType = false;
    bool m_IsFunction = false;
    bool m_IsVariable = false;
    bool m_IsMember = false;
    bool m_IsInner = false;
    bool m_IsOuter = false;
    bool m_IsGlobal = false;
    bool m_IsLocal = false;
    bool m_IsArgument = false;
    bool m_IsReturn = false;
    bool m_IsVoid = false;
    bool m_IsNull = false;
    bool m_IsObject = false;
    bool m_IsString = false;
    bool m_IsBoolean = false;
    bool m_IsByte = false;
    bool m_IsInt = false;
    bool m_IsFloat = false;
    bool m_IsDouble = false;
    bool m_IsChar = false;
    bool m_IsLong = false;
    bool m_IsShort = false;
    bool m_IsUInt = false;
    bool m_IsULong = false;
    bool m_IsUShort = false;
    bool m_IsSByte = false;
    bool m_IsUByte = false;
    bool m_IsFloat32 = false;
    bool m_IsFloat64 = false;
    bool m_IsInt16 = false;
    bool m_IsInt32 = false;
    bool m_IsInt64 = false;
    bool m_IsUInt16 = false;
    bool m_IsUInt32 = false;
    bool m_IsUInt64 = false;
    bool m_IsThis = false;
    bool m_IsBase = false;
    bool m_IsNew = false;
    bool m_IsDelete = false;
    bool m_IsCast = false;
    bool m_IsAs = false;
    bool m_IsIs = false;
    bool m_IsOf = false;
    bool m_IsIn = false;
    bool m_IsOut = false;
    bool m_IsRef = false;
    bool m_IsVal = false;
    bool m_IsVar = false;
    bool m_IsLet = false;
    bool m_IsConst = false;
    bool m_IsReadonly = false;
    bool m_IsWriteonly = false;
    bool m_IsPublic = false;
    bool m_IsPrivate = false;
    bool m_IsProtected = false;
    bool m_IsInternal = false;
    bool m_IsSealed = false;
    bool m_IsAbstract = false;
    bool m_IsVirtual = false;
    bool m_IsOverride = false;
    bool m_IsNew = false;
    bool m_IsExtern = false;
    bool m_IsPartial = false;
    bool m_IsStatic = false;
    bool m_IsReadonly = false;
    bool m_IsVolatile = false;
    bool m_IsUnsafe = false;
    bool m_IsFixed = false;
    bool m_IsLock = false;
    bool m_IsUsing = false;
    bool m_IsNamespace = false;
    bool m_IsClass = false;
    bool m_IsStruct = false;
    bool m_IsInterface = false;
    bool m_IsEnum = false;
    bool m_IsDelegate = false;
    bool m_IsEvent = false;
    bool m_IsProperty = false;
    bool m_IsIndexer = false;
    bool m_IsOperator = false;
    bool m_IsConstructor = false;
    bool m_IsDestructor = false;
    bool m_IsMethod = false;
    bool m_IsField = false;
    bool m_IsParameter = false;
    bool m_IsLocal = false;
    bool m_IsReturn = false;
    bool m_IsThis = false;
    bool m_IsBase = false;
    bool m_IsSuper = false;
    bool m_IsSelf = false;
    bool m_IsParent = false;
    bool m_IsChild = false;
    bool m_IsSibling = false;
    bool m_IsAncestor = false;
    bool m_IsDescendant = false;
    bool m_IsRoot = false;
    bool m_IsLeaf = false;
    bool m_IsBranch = false;
    bool m_IsTrunk = false;
    bool m_IsNode = false;
    bool m_IsEdge = false;
    bool m_IsVertex = false;
    bool m_IsArc = false;
    bool m_IsPath = false;
    bool m_IsRoute = false;
    bool m_IsTrail = false;
    bool m_IsWalk = false;
    bool m_IsTour = false;
    bool m_IsCircuit = false;
    bool m_IsCycle = false;
    bool m_IsLoop = false;
    bool m_IsRing = false;
    bool m_IsChain = false;
    bool m_IsSequence = false;
    bool m_IsSeries = false;
    bool m_IsList = false;
    bool m_IsArray = false;
    bool m_IsVector = false;
    bool m_IsMatrix = false;
    bool m_IsTensor = false;
    bool m_IsScalar = false;
    bool m_IsVector = false;
    bool m_IsPoint = false;
    bool m_IsLine = false;
    bool m_IsPlane = false;
    bool m_IsSpace = false;
    bool m_IsVolume = false;
    bool m_IsArea = false;
    bool m_IsLength = false;
    bool m_IsWidth = false;
    bool m_IsHeight = false;
    bool m_IsDepth = false;
    bool m_IsRadius = false;
    bool m_IsDiameter = false;
    bool m_IsCircumference = false;
    bool m_IsPerimeter = false;
    bool m_IsSurface = false;
    bool m_IsFace = false;
    bool m_IsEdge = false;
    bool m_IsVertex = false;
    bool m_IsCorner = false;
    bool m_IsAngle = false;
    bool m_IsDegree = false;
    bool m_IsRadian = false;
    bool m_IsGradient = false;
    bool m_IsSlope = false;
    bool m_IsTangent = false;
    bool m_IsNormal = false;
    bool m_IsBinormal = false;
    bool m_IsCurvature = false;
    bool m_IsTorsion = false;
    bool m_IsFrenet = false;
    bool m_IsSerret = false;
    bool m_IsDarboux = false;
    bool m_IsRicci = false;
    bool m_IsEinstein = false;
    bool m_IsRiemann = false;
    bool m_IsLevi = false;
    bool m_IsCivita = false;
    bool m_IsChristoffel = false;
    bool m_IsGamma = false;
    bool m_IsDelta = false;
    bool m_IsEpsilon = false;
    bool m_IsZeta = false;
    bool m_IsEta = false;
    bool m_IsTheta = false;
    bool m_IsIota = false;
    bool m_IsKappa = false;
    bool m_IsLambda = false;
    bool m_IsMu = false;
    bool m_IsNu = false;
    bool m_IsXi = false;
    bool m_IsOmicron = false;
    bool m_IsPi = false;
    bool m_IsRho = false;
    bool m_IsSigma = false;
    bool m_IsTau = false;
    bool m_IsUpsilon = false;
    bool m_IsPhi = false;
    bool m_IsChi = false;
    bool m_IsPsi = false;
    bool m_IsOmega = false;
};

} // namespace Core
} // namespace SimpleLanguage

