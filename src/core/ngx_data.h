
/*
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده NGX_داده_H_INCLUDED
#تعریف_کن NGX_داده_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_داده_قلم_s ngx_داده_قلم_t;


تعريفنوع بنية {
    ngx_داده_قلم_t          *قلم;
    ngx_داده_قلم_t         **بعدی;
} ngx_داده_obj_t;


بنية ngx_داده_قلم_s {
    ngx_داده_قلم_t          *بعدی;
    ngx_str_t                 name;
    ngx_uint_t                type;

    اتحاد {
        ngx_داده_obj_t        object;
        ngx_str_t             string;
        int64_t               integer;
        ngx_uint_t            boolean;
    } داده;
};


ngx_داده_قلم_t *ngx_داده_new_قلم(ngx_pool_t *pool, ngx_uint_t type);

فراغ ngx_داده_جمع_کن_قلم(ngx_داده_قلم_t *obj, ngx_str_t *name,
    ngx_داده_قلم_t *قلم);

#تعریف_کن NGX_داده_OBJECT_TYPE  0
#تعریف_کن NGX_داده_LIST_TYPE    1
#تعریف_کن NGX_داده_STRING_TYPE  2
#تعریف_کن NGX_داده_INTEGER_TYPE 3
#تعریف_کن NGX_داده_BOOLEAN_TYPE 4
#تعریف_کن NGX_داده_NULL_TYPE    5


#تعریف_کن ngx_داده_new_object(pool)                                             \
    ngx_داده_new_قلم(pool, NGX_داده_OBJECT_TYPE)
#تعریف_کن ngx_داده_new_list(pool)                                               \
    ngx_داده_new_قلم(pool, NGX_داده_LIST_TYPE)
#تعریف_کن ngx_داده_new_string(pool)                                             \
    ngx_داده_new_قلم(pool, NGX_داده_STRING_TYPE)
#تعریف_کن ngx_داده_new_integer(pool)                                            \
    ngx_داده_new_قلم(pool, NGX_داده_INTEGER_TYPE)
#تعریف_کن ngx_داده_new_boolean(pool)                                            \
    ngx_داده_new_قلم(pool, NGX_داده_BOOLEAN_TYPE)
#تعریف_کن ngx_داده_new_null(pool)                                               \
    ngx_داده_new_قلم(pool, NGX_داده_NULL_TYPE)


تعريفنوع بنية {
    ngx_str_t            name;
    ngx_داده_قلم_t   *(*handler)(uintptr_t داده, ngx_pool_t *pool, فراغ *ctx);
    uintptr_t            داده;
} ngx_داده_decl_t;

#تعریف_کن ngx_داده_null_decl  { ngx_null_string, عدم, 0 }

#تعریف_کن NGX_داده_DECLINE  (ngx_داده_قلم_t *) -1


ngx_داده_قلم_t *ngx_داده_obj_handler(uintptr_t داده, ngx_pool_t *pool,
    فراغ *ctx);
ngx_داده_قلم_t *ngx_داده_obj_fields_handler(uintptr_t داده, ngx_pool_t *pool,
    فراغ *ctx, ngx_array_t *fields);

ngx_داده_قلم_t *ngx_داده_عدد_handler(uintptr_t داده, ngx_pool_t *pool,
    فراغ *ctx);
ngx_داده_قلم_t *ngx_داده_string_handler(uintptr_t داده, ngx_pool_t *pool,
    فراغ *ctx);
ngx_داده_قلم_t *ngx_داده_boolean_handler(uintptr_t داده, ngx_pool_t *pool,
    فراغ *ctx);
ngx_داده_قلم_t *ngx_داده_زمان_handler(uintptr_t داده, ngx_pool_t *pool,
    فراغ *ctx);

ngx_داده_قلم_t *ngx_داده_struct_int64_handler(uintptr_t داده, ngx_pool_t *pool,
    فراغ *ctx);
ngx_داده_قلم_t *ngx_داده_struct_int_handler(uintptr_t داده, ngx_pool_t *pool,
    فراغ *ctx);
ngx_داده_قلم_t *ngx_داده_struct_atomic_handler(uintptr_t داده,
    ngx_pool_t *pool, فراغ *ctx);
ngx_داده_قلم_t *ngx_داده_struct_str_handler(uintptr_t داده, ngx_pool_t *pool,
    فراغ *ctx);
ngx_داده_قلم_t *ngx_داده_struct_boolean_handler(uintptr_t داده,
    ngx_pool_t *pool, فراغ *ctx);

#پایان_اگر /* _NGX_DATA_H_INCLUDED_ */
