#include "../include/vm_object.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 前向声明 - 这里需要IRMetaClass的定义
typedef struct IRMetaClass {
    char name[256];
    // 其他字段...
} IRMetaClass;

// SObject创建函数
SObject* s_object_create(void) {
    SObject* obj = (SObject*)malloc(sizeof(SObject));
    if (!obj) {
        return NULL;
    }
    
    obj->is_null = false;
    obj->is_void = false;
    obj->e_type = TYPE_CLASS;
    obj->type_id = 0;
    obj->ref_count = 1;
    obj->ir_meta_class = NULL;
    
    return obj;
}

// BoolObject创建函数
BoolObject* bool_object_create(bool value) {
    BoolObject* obj = (BoolObject*)malloc(sizeof(BoolObject));
    if (!obj) {
        return NULL;
    }
    
    obj->base.is_null = false;
    obj->base.is_void = false;
    obj->base.e_type = TYPE_BOOLEAN;
    obj->base.type_id = 1;
    obj->base.ref_count = 1;
    obj->base.ir_meta_class = NULL;
    obj->value = value;
    
    return obj;
}

// Int32Object创建函数
Int32Object* int32_object_create(int32_t value) {
    Int32Object* obj = (Int32Object*)malloc(sizeof(Int32Object));
    if (!obj) {
        return NULL;
    }
    
    obj->base.is_null = false;
    obj->base.is_void = false;
    obj->base.e_type = TYPE_INT32;
    obj->base.type_id = 2;
    obj->base.ref_count = 1;
    obj->base.ir_meta_class = NULL;
    obj->value = value;
    
    return obj;
}

// FloatObject创建函数
FloatObject* float_object_create(float value) {
    FloatObject* obj = (FloatObject*)malloc(sizeof(FloatObject));
    if (!obj) {
        return NULL;
    }
    
    obj->base.is_null = false;
    obj->base.is_void = false;
    obj->base.e_type = TYPE_FLOAT32;
    obj->base.type_id = 3;
    obj->base.ref_count = 1;
    obj->base.ir_meta_class = NULL;
    obj->value = value;
    
    return obj;
}

// DoubleObject创建函数
DoubleObject* double_object_create(double value) {
    DoubleObject* obj = (DoubleObject*)malloc(sizeof(DoubleObject));
    if (!obj) {
        return NULL;
    }
    
    obj->base.is_null = false;
    obj->base.is_void = false;
    obj->base.e_type = TYPE_FLOAT64;
    obj->base.type_id = 4;
    obj->base.ref_count = 1;
    obj->base.ir_meta_class = NULL;
    obj->value = value;
    
    return obj;
}

// StringObject创建函数
StringObject* string_object_create(const char* value) {
    StringObject* obj = (StringObject*)malloc(sizeof(StringObject));
    if (!obj) {
        return NULL;
    }
    
    obj->base.is_null = false;
    obj->base.is_void = false;
    obj->base.e_type = TYPE_STRING;
    obj->base.type_id = 5;
    obj->base.ref_count = 1;
    obj->base.ir_meta_class = NULL;
    
    if (value) {
        obj->length = strlen(value);
        obj->value = (char*)malloc(obj->length + 1);
        if (obj->value) {
            strcpy(obj->value, value);
        } else {
            obj->value = NULL;
            obj->length = 0;
        }
    } else {
        obj->value = NULL;
        obj->length = 0;
    }
    
    return obj;
}

// ArrayObject创建函数
ArrayObject* array_object_create(size_t initial_capacity) {
    ArrayObject* obj = (ArrayObject*)malloc(sizeof(ArrayObject));
    if (!obj) {
        return NULL;
    }
    
    obj->base.is_null = false;
    obj->base.is_void = false;
    obj->base.e_type = TYPE_ARRAY;
    obj->base.type_id = 6;
    obj->base.ref_count = 1;
    obj->base.ir_meta_class = NULL;
    
    obj->length = 0;
    obj->capacity = initial_capacity > 0 ? initial_capacity : 10;
    obj->elements = (void**)malloc(obj->capacity * sizeof(void*));
    if (!obj->elements) {
        obj->capacity = 0;
    }
    
    return obj;
}

// ClassObject创建函数
ClassObject* class_object_create(IRMetaClass* class_info) {
    ClassObject* obj = (ClassObject*)malloc(sizeof(ClassObject));
    if (!obj) {
        return NULL;
    }
    
    obj->base.is_null = false;
    obj->base.is_void = false;
    obj->base.e_type = TYPE_CLASS;
    obj->base.type_id = 7;
    obj->base.ref_count = 1;
    obj->base.ir_meta_class = class_info;
    
    obj->class_info = class_info;
    obj->member_variables = NULL;
    obj->member_count = 0;
    
    return obj;
}

// 对象销毁函数
void s_object_destroy(SObject* obj) {
    if (obj) {
        free(obj);
    }
}

void bool_object_destroy(BoolObject* obj) {
    if (obj) {
        free(obj);
    }
}

void int32_object_destroy(Int32Object* obj) {
    if (obj) {
        free(obj);
    }
}

void float_object_destroy(FloatObject* obj) {
    if (obj) {
        free(obj);
    }
}

void double_object_destroy(DoubleObject* obj) {
    if (obj) {
        free(obj);
    }
}

void string_object_destroy(StringObject* obj) {
    if (obj) {
        if (obj->value) {
            free(obj->value);
        }
        free(obj);
    }
}

void array_object_destroy(ArrayObject* obj) {
    if (obj) {
        if (obj->elements) {
            free(obj->elements);
        }
        free(obj);
    }
}

void class_object_destroy(ClassObject* obj) {
    if (obj) {
        if (obj->member_variables) {
            free(obj->member_variables);
        }
        free(obj);
    }
}

// 对象属性设置函数
void s_object_set_null(SObject* obj) {
    if (obj) {
        obj->is_null = true;
    }
}

void s_object_set_void(SObject* obj) {
    if (obj) {
        obj->is_void = true;
    }
}

void s_object_set_type_id(SObject* obj, short type_id) {
    if (obj) {
        obj->type_id = type_id;
    }
}

void s_object_set_ref_count(SObject* obj, int ref_count) {
    if (obj) {
        obj->ref_count = ref_count;
    }
}

void s_object_set_ir_meta_class(SObject* obj, IRMetaClass* ir_meta_class) {
    if (obj) {
        obj->ir_meta_class = ir_meta_class;
    }
}

// 对象属性获取函数
bool s_object_get_is_null(const SObject* obj) {
    return obj ? obj->is_null : true;
}

bool s_object_get_is_void(const SObject* obj) {
    return obj ? obj->is_void : true;
}

EType s_object_get_etype(const SObject* obj) {
    return obj ? obj->e_type : TYPE_NONE;
}

short s_object_get_type_id(const SObject* obj) {
    return obj ? obj->type_id : 0;
}

int s_object_get_ref_count(const SObject* obj) {
    return obj ? obj->ref_count : 0;
}

IRMetaClass* s_object_get_ir_meta_class(const SObject* obj) {
    return obj ? obj->ir_meta_class : NULL;
}

// 数组对象操作
void array_object_add_element(ArrayObject* arr, void* element) {
    if (!arr || !element) {
        return;
    }
    
    if (arr->length >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        void** new_elements = (void**)realloc(arr->elements, new_capacity * sizeof(void*));
        if (!new_elements) {
            return;
        }
        arr->elements = new_elements;
        arr->capacity = new_capacity;
    }
    
    arr->elements[arr->length++] = element;
}

void* array_object_get_element(const ArrayObject* arr, size_t index) {
    if (!arr || index >= arr->length) {
        return NULL;
    }
    return arr->elements[index];
}

void array_object_set_element(ArrayObject* arr, size_t index, void* element) {
    if (!arr || index >= arr->length || !element) {
        return;
    }
    arr->elements[index] = element;
}

size_t array_object_get_length(const ArrayObject* arr) {
    return arr ? arr->length : 0;
}

size_t array_object_get_capacity(const ArrayObject* arr) {
    return arr ? arr->capacity : 0;
}

// 字符串对象操作
const char* string_object_get_value(const StringObject* str) {
    return str ? str->value : "";
}

size_t string_object_get_length(const StringObject* str) {
    return str ? str->length : 0;
}

void string_object_set_value(StringObject* str, const char* value) {
    if (!str) {
        return;
    }
    
    if (str->value) {
        free(str->value);
    }
    
    if (value) {
        str->length = strlen(value);
        str->value = (char*)malloc(str->length + 1);
        if (str->value) {
            strcpy(str->value, value);
        } else {
            str->length = 0;
        }
    } else {
        str->value = NULL;
        str->length = 0;
    }
}

// 对象工具函数
const char* s_object_to_format_string(const SObject* obj) {
    if (!obj) {
        return "NULL";
    }
    
    static char buffer[1024];
    switch (obj->e_type) {
        case TYPE_BOOLEAN:
            return obj->is_null ? "null" : "boolean";
        case TYPE_INT32:
            return obj->is_null ? "null" : "int32";
        case TYPE_FLOAT32:
            return obj->is_null ? "null" : "float32";
        case TYPE_FLOAT64:
            return obj->is_null ? "null" : "float64";
        case TYPE_STRING:
            return obj->is_null ? "null" : "string";
        case TYPE_ARRAY:
            return obj->is_null ? "null" : "array";
        case TYPE_CLASS:
            return obj->is_null ? "null" : "class";
        default:
            return "unknown";
    }
}

void s_object_increment_ref_count(SObject* obj) {
    if (obj) {
        obj->ref_count++;
    }
}

void s_object_decrement_ref_count(SObject* obj) {
    if (obj && obj->ref_count > 0) {
        obj->ref_count--;
    }
} 