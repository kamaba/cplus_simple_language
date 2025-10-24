#ifndef SIMPLE_LANGUAGE_META_CLASS_H
#define SIMPLE_LANGUAGE_META_CLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "meta_base.h"
#include "define.h"
#include <stddef.h>

// 前向声明
typedef struct MetaType MetaType;
typedef struct MetaMemberVariable MetaMemberVariable;
typedef struct MetaMemberFunction MetaMemberFunction;
typedef struct MetaMemberFunctionTemplateNode MetaMemberFunctionTemplateNode;
typedef struct FileMetaClass FileMetaClass;
typedef struct MetaExpressNode MetaExpressNode;

// 元数据成员变量结构
typedef struct MetaMemberVariable {
    MetaBase base;
    MetaType* meta_type;
    bool is_static;
    bool is_const;
    MetaExpressNode* default_express_node;
} MetaMemberVariable;

// 元数据成员函数结构
typedef struct MetaMemberFunction {
    MetaBase base;
    MetaType* return_type;
    MetaType** param_types;
    int param_count;
    bool is_static;
    bool is_virtual;
    bool is_override;
    MetaExpressNode* body_node;
} MetaMemberFunction;

// 元数据成员函数模板节点结构
typedef struct MetaMemberFunctionTemplateNode {
    MetaBase base;
    MetaMemberFunction* function;
    MetaType** template_types;
    int template_type_count;
} MetaMemberFunctionTemplateNode;

// 文件元数据类结构
typedef struct FileMetaClass {
    MetaBase base;
    char file_path[MAX_PATH_LENGTH];
    int line_number;
    MetaClass* meta_class;
} FileMetaClass;

// 元数据类结构
typedef struct MetaClass {
    MetaBase base;
    
    int extend_level;                     // 继承层级
    EType type;                          // 类型
    EClassDefineType class_define_type;   // 类定义类型
    bool is_interface_class;              // 是否为接口类
    
    MetaClass* extend_class;              // 继承的类
    MetaType* extend_class_meta_type;     // 继承类的元类型
    
    MetaClass** interface_classes;        // 实现的接口类
    int interface_class_count;            // 接口类数量
    
    MetaType** bind_struct_template_meta_class_list;  // 绑定的结构模板元类列表
    int bind_struct_template_meta_class_count;
    
    FileMetaClass** file_meta_class_dict; // 文件元类字典
    int file_meta_class_count;
    
    MetaMemberVariable** meta_member_variable_dict;   // 成员变量字典
    int meta_member_variable_count;
    
    MetaMemberVariable** file_collect_meta_member_variable;  // 文件收集的成员变量
    int file_collect_meta_member_variable_count;
    
    MetaMemberVariable** meta_extend_member_variable_dict;   // 继承的成员变量字典
    int meta_extend_member_variable_count;
    
    MetaMemberFunctionTemplateNode** meta_member_function_template_node_dict;  // 成员函数模板节点字典
    int meta_member_function_template_node_count;
    
    MetaMemberFunction** file_collect_meta_member_function_list;  // 文件收集的成员函数列表
    int file_collect_meta_member_function_count;
    
    MetaMemberFunction** non_static_virtual_meta_member_function_list;  // 非静态虚函数列表
    int non_static_virtual_meta_member_function_count;
    
    MetaMemberFunction** static_meta_member_function_list;  // 静态成员函数列表
    int static_meta_member_function_count;
    
    MetaMemberFunction** temp_inner_function_list;  // 临时内部函数列表
    int temp_inner_function_count;
    
    MetaExpressNode* default_express_node;  // 默认表达式节点
    
    bool is_handle_extend_variable_dirty;   // 是否处理继承变量脏标记
} MetaClass;

// MetaClass创建和销毁函数
MetaClass* meta_class_create(const char* name, EClassDefineType ecdt);
void meta_class_destroy(MetaClass* mc);

// MetaClass属性设置函数
void meta_class_set_extend_level(MetaClass* mc, int level);
void meta_class_set_type(MetaClass* mc, EType type);
void meta_class_set_class_define_type(MetaClass* mc, EClassDefineType ecdt);
void meta_class_set_is_interface_class(MetaClass* mc, bool is_interface);
void meta_class_set_extend_class(MetaClass* mc, MetaClass* extend_class);
void meta_class_set_extend_class_meta_type(MetaClass* mc, MetaType* meta_type);

// MetaClass成员变量操作
void meta_class_add_member_variable(MetaClass* mc, MetaMemberVariable* variable);
MetaMemberVariable* meta_class_get_member_variable(const MetaClass* mc, const char* name);
MetaMemberVariable** meta_class_get_all_member_variables(const MetaClass* mc, int* count);

// MetaClass成员函数操作
void meta_class_add_member_function(MetaClass* mc, MetaMemberFunction* function);
MetaMemberFunction* meta_class_get_member_function(const MetaClass* mc, const char* name);
MetaMemberFunction** meta_class_get_all_member_functions(const MetaClass* mc, int* count);

// MetaClass接口操作
void meta_class_add_interface(MetaClass* mc, MetaClass* interface_class);
MetaClass* meta_class_get_interface(const MetaClass* mc, int index);
int meta_class_get_interface_count(const MetaClass* mc);

// MetaClass工具函数
const char* meta_class_get_all_class_name(const MetaClass* mc);
EType meta_class_get_type(const MetaClass* mc);
EClassDefineType meta_class_get_class_define_type(const MetaClass* mc);
MetaClass* meta_class_get_extend_class(const MetaClass* mc);
bool meta_class_get_is_interface_class(const MetaClass* mc);

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LANGUAGE_META_CLASS_H 