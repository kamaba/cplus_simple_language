#ifndef SIMPLE_LANGUAGE_VM_OBJECT_H
#define SIMPLE_LANGUAGE_VM_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "define.h"
#include <stddef.h>

// 前向声明
typedef struct IRMetaClass IRMetaClass;

// 虚拟机对象基类
typedef struct SObject {
    bool is_null;                         // 是否为空
    bool is_void;                         // 是否为void
    EType e_type;                         // 类型
    short type_id;                        // 类型ID
    int ref_count;                        // 引用计数
    IRMetaClass* ir_meta_class;           // IR元类
} SObject;

// 布尔对象
typedef struct BoolObject {
    SObject base;
    bool value;                           // 布尔值
} BoolObject;

// 整数对象
typedef struct Int32Object {
    SObject base;
    int32_t value;                       // 整数值
} Int32Object;

// 浮点对象
typedef struct FloatObject {
    SObject base;
    float value;                         // 浮点值
} FloatObject;

// 双精度对象
typedef struct DoubleObject {
    SObject base;
    double value;                        // 双精度值
} DoubleObject;

// 字符串对象
typedef struct StringObject {
    SObject base;
    char* value;                         // 字符串值
    size_t length;                       // 字符串长度
} StringObject;

// 数组对象
typedef struct ArrayObject {
    SObject base;
    void** elements;                     // 数组元素
    size_t length;                       // 数组长度
    size_t capacity;                     // 数组容量
} ArrayObject;

// 类对象
typedef struct ClassObject {
    SObject base;
    IRMetaClass* class_info;             // 类信息
    void** member_variables;             // 成员变量
    size_t member_count;                 // 成员数量
} ClassObject;

// 对象创建函数
SObject* s_object_create(void);
BoolObject* bool_object_create(bool value);
Int32Object* int32_object_create(int32_t value);
FloatObject* float_object_create(float value);
DoubleObject* double_object_create(double value);
StringObject* string_object_create(const char* value);
ArrayObject* array_object_create(size_t initial_capacity);
ClassObject* class_object_create(IRMetaClass* class_info);

// 对象销毁函数
void s_object_destroy(SObject* obj);
void bool_object_destroy(BoolObject* obj);
void int32_object_destroy(Int32Object* obj);
void float_object_destroy(FloatObject* obj);
void double_object_destroy(DoubleObject* obj);
void string_object_destroy(StringObject* obj);
void array_object_destroy(ArrayObject* obj);
void class_object_destroy(ClassObject* obj);

// 对象属性设置函数
void s_object_set_null(SObject* obj);
void s_object_set_void(SObject* obj);
void s_object_set_type_id(SObject* obj, short type_id);
void s_object_set_ref_count(SObject* obj, int ref_count);
void s_object_set_ir_meta_class(SObject* obj, IRMetaClass* ir_meta_class);

// 对象属性获取函数
bool s_object_get_is_null(const SObject* obj);
bool s_object_get_is_void(const SObject* obj);
EType s_object_get_etype(const SObject* obj);
short s_object_get_type_id(const SObject* obj);
int s_object_get_ref_count(const SObject* obj);
IRMetaClass* s_object_get_ir_meta_class(const SObject* obj);

// 数组对象操作
void array_object_add_element(ArrayObject* arr, void* element);
void* array_object_get_element(const ArrayObject* arr, size_t index);
void array_object_set_element(ArrayObject* arr, size_t index, void* element);
size_t array_object_get_length(const ArrayObject* arr);
size_t array_object_get_capacity(const ArrayObject* arr);

// 字符串对象操作
const char* string_object_get_value(const StringObject* str);
size_t string_object_get_length(const StringObject* str);
void string_object_set_value(StringObject* str, const char* value);

// 对象工具函数
const char* s_object_to_format_string(const SObject* obj);
void s_object_increment_ref_count(SObject* obj);
void s_object_decrement_ref_count(SObject* obj);

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LANGUAGE_VM_OBJECT_H 