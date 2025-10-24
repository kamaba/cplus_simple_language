#ifndef SIMPLE_LANGUAGE_META_BASE_H
#define SIMPLE_LANGUAGE_META_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "define.h"
#include <stddef.h>

// 前向声明
typedef struct MetaNode MetaNode;

// MetaBase结构体
typedef struct MetaBase {
    EPermission permission;               // 权限
    RefFromType ref_from_type;            // 引用来源类型
    char name[MAX_NAME_LENGTH];           // 名称
    char all_name[MAX_NAME_LENGTH];       // 全名
    MetaNode* meta_node;                 // 元数据节点
    int deep;                            // 深度
    int anchor_deep;                     // 锚点深度
} MetaBase;

// MetaBase创建和销毁函数
MetaBase* meta_base_create(void);
MetaBase* meta_base_create_copy(const MetaBase* mb);
void meta_base_destroy(MetaBase* mb);

// MetaBase属性设置函数
void meta_base_set_ref_from_type(MetaBase* mb, RefFromType type);
void meta_base_set_name(MetaBase* mb, const char* name);
void meta_base_set_anchor_deep(MetaBase* mb, int add_deep);
void meta_base_set_deep(MetaBase* mb, int deep);
void meta_base_set_meta_node(MetaBase* mb, MetaNode* meta_node);

// MetaBase属性获取函数
int meta_base_get_deep(const MetaBase* mb);
int meta_base_get_real_deep(const MetaBase* mb);
EPermission meta_base_get_permission(const MetaBase* mb);
const char* meta_base_get_name(const MetaBase* mb);
RefFromType meta_base_get_ref_from_type(const MetaBase* mb);
MetaNode* meta_base_get_meta_node(const MetaBase* mb);
const char* meta_base_get_path_name(const MetaBase* mb);

// MetaBase工具函数
const char* meta_base_get_format_string(const MetaBase* mb);
const char* meta_base_to_format_string(const MetaBase* mb);

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LANGUAGE_META_BASE_H 