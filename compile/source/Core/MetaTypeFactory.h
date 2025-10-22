//****************************************************************************
//  File:      MetaTypeFactory.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2022/8/12 12:00:00
//  Description: 
//****************************************************************************

#pragma once

#include <string>

namespace SimpleLanguage {
namespace Core {
class MetaType;

class MetaTypeFactory {
public:
    // 根据MetaType获取操作级别
    static int GetOpLevelByMetaType(MetaType* mt);
    
    // 根据EType获取操作级别
    static int GetOpLevel(EType defineType);
    
    // 将定义类型转换为类名
    static std::string DefineTypeToClassName(const std::string& tokenname);
};

} // namespace Core
} // namespace SimpleLanguage
