
/*
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی NGX_معلومات_H_INCLUDED
#تعریف_کړه NGX_معلومات_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_معلومات_توکی_s ngx_معلومات_توکی_t;


تعريفنوع بنية {
    ngx_معلومات_توکی_t          *توکی;
    ngx_معلومات_توکی_t         **بل;
} ngx_معلومات_obj_t;


بنية ngx_معلومات_توکی_s {
    ngx_معلومات_توکی_t          *بل;
    ngx_str_t                 name;
    ngx_uint_t                type;

    اتحاد {
        ngx_معلومات_obj_t        object;
        ngx_str_t             string;
        int64_t               integer;
        ngx_uint_t            boolean;
    } معلومات;
};


ngx_معلومات_توکی_t *ngx_معلومات_new_توکی(ngx_pool_t *pool, ngx_uint_t type);

فراغ ngx_معلومات_جمع_کړه_توکی(ngx_معلومات_توکی_t *obj, ngx_str_t *name,
    ngx_معلومات_توکی_t *توکی);

#تعریف_کړه NGX_معلومات_OBJECT_TYPE  0
#تعریف_کړه NGX_معلومات_LIST_TYPE    1
#تعریف_کړه NGX_معلومات_STRING_TYPE  2
#تعریف_کړه NGX_معلومات_INTEGER_TYPE 3
#تعریف_کړه NGX_معلومات_BOOLEAN_TYPE 4
#تعریف_کړه NGX_معلومات_NULL_TYPE    5


#تعریف_کړه ngx_معلومات_new_object(pool)                                             \
    ngx_معلومات_new_توکی(pool, NGX_معلومات_OBJECT_TYPE)
#تعریف_کړه ngx_معلومات_new_list(pool)                                               \
    ngx_معلومات_new_توکی(pool, NGX_معلومات_LIST_TYPE)
#تعریف_کړه ngx_معلومات_new_string(pool)                                             \
    ngx_معلومات_new_توکی(pool, NGX_معلومات_STRING_TYPE)
#تعریف_کړه ngx_معلومات_new_integer(pool)                                            \
    ngx_معلومات_new_توکی(pool, NGX_معلومات_INTEGER_TYPE)
#تعریف_کړه ngx_معلومات_new_boolean(pool)                                            \
    ngx_معلومات_new_توکی(pool, NGX_معلومات_BOOLEAN_TYPE)
#تعریف_کړه ngx_معلومات_new_null(pool)                                               \
    ngx_معلومات_new_توکی(pool, NGX_معلومات_NULL_TYPE)


تعريفنوع بنية {
    ngx_str_t            name;
    ngx_معلومات_توکی_t   *(*handler)(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx);
    uintptr_t            معلومات;
} ngx_معلومات_decl_t;

#تعریف_کړه ngx_معلومات_null_decl  { ngx_null_string, عدم, 0 }

#تعریف_کړه NGX_معلومات_DECLINE  (ngx_معلومات_توکی_t *) -1


ngx_معلومات_توکی_t *ngx_معلومات_obj_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);
ngx_معلومات_توکی_t *ngx_معلومات_obj_fields_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx, ngx_array_t *fields);

ngx_معلومات_توکی_t *ngx_معلومات_شمېره_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);
ngx_معلومات_توکی_t *ngx_معلومات_string_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);
ngx_معلومات_توکی_t *ngx_معلومات_boolean_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);
ngx_معلومات_توکی_t *ngx_معلومات_وخت_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);

ngx_معلومات_توکی_t *ngx_معلومات_struct_int64_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);
ngx_معلومات_توکی_t *ngx_معلومات_struct_int_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);
ngx_معلومات_توکی_t *ngx_معلومات_struct_atomic_handler(uintptr_t معلومات,
    ngx_pool_t *pool, فراغ *ctx);
ngx_معلومات_توکی_t *ngx_معلومات_struct_str_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);
ngx_معلومات_توکی_t *ngx_معلومات_struct_boolean_handler(uintptr_t معلومات,
    ngx_pool_t *pool, فراغ *ctx);

#د_که_پای /* _NGX_DATA_H_INCLUDED_ */
