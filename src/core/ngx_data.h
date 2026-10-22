
/*
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् NGX_दत्त_H_INCLUDED
#परिभाषय NGX_दत्त_H_INCLUDED


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश संरचना ngx_दत्त_item_s ngx_दत्त_item_t;


प्रकारनिर्देश संरचना {
    ngx_दत्त_item_t          *item;
    ngx_दत्त_item_t         **अग्र;
} ngx_दत्त_obj_t;


संरचना ngx_दत्त_item_s {
    ngx_दत्त_item_t          *अग्र;
    ngx_str_t                 name;
    ngx_uint_t                type;

    सङ्घ {
        ngx_दत्त_obj_t        object;
        ngx_str_t             string;
        int64_t               integer;
        ngx_uint_t            boolean;
    } दत्त;
};


ngx_दत्त_item_t *ngx_दत्त_new_item(ngx_pool_t *pool, ngx_uint_t type);

शून्य ngx_दत्त_सङ्कलय_item(ngx_दत्त_item_t *obj, ngx_str_t *name,
    ngx_दत्त_item_t *item);

#परिभाषय NGX_दत्त_OBJECT_TYPE  0
#परिभाषय NGX_दत्त_LIST_TYPE    1
#परिभाषय NGX_दत्त_STRING_TYPE  2
#परिभाषय NGX_दत्त_INTEGER_TYPE 3
#परिभाषय NGX_दत्त_BOOLEAN_TYPE 4
#परिभाषय NGX_दत्त_NULL_TYPE    5


#परिभाषय ngx_दत्त_new_object(pool)                                             \
    ngx_दत्त_new_item(pool, NGX_दत्त_OBJECT_TYPE)
#परिभाषय ngx_दत्त_new_list(pool)                                               \
    ngx_दत्त_new_item(pool, NGX_दत्त_LIST_TYPE)
#परिभाषय ngx_दत्त_new_string(pool)                                             \
    ngx_दत्त_new_item(pool, NGX_दत्त_STRING_TYPE)
#परिभाषय ngx_दत्त_new_integer(pool)                                            \
    ngx_दत्त_new_item(pool, NGX_दत्त_INTEGER_TYPE)
#परिभाषय ngx_दत्त_new_boolean(pool)                                            \
    ngx_दत्त_new_item(pool, NGX_दत्त_BOOLEAN_TYPE)
#परिभाषय ngx_दत्त_new_null(pool)                                               \
    ngx_दत्त_new_item(pool, NGX_दत्त_NULL_TYPE)


प्रकारनिर्देश संरचना {
    ngx_str_t            name;
    ngx_दत्त_item_t   *(*handler)(uintptr_t दत्त, ngx_pool_t *pool, शून्य *ctx);
    uintptr_t            दत्त;
} ngx_दत्त_decl_t;

#परिभाषय ngx_दत्त_null_decl  { ngx_null_string, रिक्त, 0 }

#परिभाषय NGX_दत्त_DECLINE  (ngx_दत्त_item_t *) -1


ngx_दत्त_item_t *ngx_दत्त_obj_handler(uintptr_t दत्त, ngx_pool_t *pool,
    शून्य *ctx);
ngx_दत्त_item_t *ngx_दत्त_obj_fields_handler(uintptr_t दत्त, ngx_pool_t *pool,
    शून्य *ctx, ngx_array_t *fields);

ngx_दत्त_item_t *ngx_दत्त_सङ्ख्या_handler(uintptr_t दत्त, ngx_pool_t *pool,
    शून्य *ctx);
ngx_दत्त_item_t *ngx_दत्त_string_handler(uintptr_t दत्त, ngx_pool_t *pool,
    शून्य *ctx);
ngx_दत्त_item_t *ngx_दत्त_boolean_handler(uintptr_t दत्त, ngx_pool_t *pool,
    शून्य *ctx);
ngx_दत्त_item_t *ngx_दत्त_time_handler(uintptr_t दत्त, ngx_pool_t *pool,
    शून्य *ctx);

ngx_दत्त_item_t *ngx_दत्त_struct_int64_handler(uintptr_t दत्त, ngx_pool_t *pool,
    शून्य *ctx);
ngx_दत्त_item_t *ngx_दत्त_struct_int_handler(uintptr_t दत्त, ngx_pool_t *pool,
    शून्य *ctx);
ngx_दत्त_item_t *ngx_दत्त_struct_atomic_handler(uintptr_t दत्त,
    ngx_pool_t *pool, शून्य *ctx);
ngx_दत्त_item_t *ngx_दत्त_struct_str_handler(uintptr_t दत्त, ngx_pool_t *pool,
    शून्य *ctx);
ngx_दत्त_item_t *ngx_दत्त_struct_boolean_handler(uintptr_t दत्त,
    ngx_pool_t *pool, शून्य *ctx);

#यदि_अन्तः /* _NGX_DATA_H_INCLUDED_ */
