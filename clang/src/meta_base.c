#include "../include/meta_base.h"
#include <stdlib.h>
#include <string.h>

// MetaBase创建函数
MetaBase* meta_base_create(void) {
    MetaBase* mb = (MetaBase*)malloc(sizeof(MetaBase));
    if (!mb) {
        return NULL;
    }
    
    // 初始化所有字段
    mb->permission = PERMISSION_PUBLIC;
    mb->ref_from_type = REF_FROM_LOCAL;
    mb->name[0] = '\0';
    mb->all_name[0] = '\0';
    mb->meta_node = NULL;
    mb->deep = 0;
    mb->anchor_deep = 0;
    
    return mb;
}

// MetaBase复制创建函数
MetaBase* meta_base_create_copy(const MetaBase* mb) {
    if (!mb) {
        return NULL;
    }
    
    MetaBase* new_mb = meta_base_create();
    if (!new_mb) {
        return NULL;
    }
    
    // 复制所有字段
    new_mb->permission = mb->permission;
    new_mb->ref_from_type = mb->ref_from_type;
    strncpy(new_mb->name, mb->name, MAX_NAME_LENGTH - 1);
    new_mb->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(new_mb->all_name, mb->all_name, MAX_NAME_LENGTH - 1);
    new_mb->all_name[MAX_NAME_LENGTH - 1] = '\0';
    new_mb->meta_node = mb->meta_node;
    new_mb->deep = mb->deep;
    new_mb->anchor_deep = mb->anchor_deep;
    
    return new_mb;
}

// MetaBase销毁函数
void meta_base_destroy(MetaBase* mb) {
    if (mb) {
        free(mb);
    }
}

// MetaBase属性设置函数
void meta_base_set_ref_from_type(MetaBase* mb, RefFromType type) {
    if (mb) {
        mb->ref_from_type = type;
    }
}

void meta_base_set_name(MetaBase* mb, const char* name) {
    if (mb && name) {
        strncpy(mb->name, name, MAX_NAME_LENGTH - 1);
        mb->name[MAX_NAME_LENGTH - 1] = '\0';
    }
}

void meta_base_set_anchor_deep(MetaBase* mb, int add_deep) {
    if (mb) {
        mb->anchor_deep = add_deep;
    }
}

void meta_base_set_deep(MetaBase* mb, int deep) {
    if (mb) {
        mb->deep = deep;
    }
}

void meta_base_set_meta_node(MetaBase* mb, MetaNode* meta_node) {
    if (mb) {
        mb->meta_node = meta_node;
    }
}

// MetaBase属性获取函数
int meta_base_get_deep(const MetaBase* mb) {
    return mb ? mb->deep : 0;
}

int meta_base_get_real_deep(const MetaBase* mb) {
    if (!mb) {
        return 0;
    }
    return mb->deep - mb->anchor_deep;
}

EPermission meta_base_get_permission(const MetaBase* mb) {
    return mb ? mb->permission : PERMISSION_NULL;
}

const char* meta_base_get_name(const MetaBase* mb) {
    return mb ? mb->name : "";
}

RefFromType meta_base_get_ref_from_type(const MetaBase* mb) {
    return mb ? mb->ref_from_type : REF_FROM_LOCAL;
}

MetaNode* meta_base_get_meta_node(const MetaBase* mb) {
    return mb ? mb->meta_node : NULL;
}

const char* meta_base_get_path_name(const MetaBase* mb) {
    // 这里需要MetaNode的实现来获取路径名
    // 暂时返回空字符串
    return "";
}

// MetaBase工具函数
const char* meta_base_get_format_string(const MetaBase* mb) {
    // 暂时返回空字符串，需要具体实现
    return "";
}

const char* meta_base_to_format_string(const MetaBase* mb) {
    // 暂时返回空字符串，需要具体实现
    return "";
} 