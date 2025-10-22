//****************************************************************************
//  File:      ImportManager.h
// ------------------------------------------------
//  Copyright (c) kamaba233@gmail.com
//  DateTime: 2025/5/17 12:00:00
//  Description: Import manager class
//****************************************************************************

#pragma once

#include <string>

namespace SimpleLanguage {
namespace Core {

class ImportManager {
public:
    // ���캯��
    ImportManager() = default;
    virtual ~ImportManager() = default;

    // ���ÿ�������͸�ֵ
    ImportManager(const ImportManager&) = delete;
    ImportManager& operator=(const ImportManager&) = delete;
};

} // namespace Core
} // namespace SimpleLanguage
