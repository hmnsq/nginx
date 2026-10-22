
/*
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل NGX_معلومات_H_INCLUDED
#وصف_ڪريو NGX_معلومات_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_معلومات_شي_s ngx_معلومات_شي_t;


تعريفنوع بنية {
    ngx_معلومات_شي_t          *شي;
    ngx_معلومات_شي_t         **ايندڙ;
} ngx_معلومات_obj_t;


بنية ngx_معلومات_شي_s {
    ngx_معلومات_شي_t          *ايندڙ;
    ngx_str_t                 name;
    ngx_uint_t                type;

    اتحاد {
        ngx_معلومات_obj_t        object;
        ngx_str_t             string;
        int64_t               integer;
        ngx_uint_t            boolean;
    } معلومات;
};


ngx_معلومات_شي_t *ngx_معلومات_new_شي(ngx_pool_t *pool, ngx_uint_t type);

فراغ ngx_معلومات_جوڙيو_شي(ngx_معلومات_شي_t *obj, ngx_str_t *name,
    ngx_معلومات_شي_t *شي);

#وصف_ڪريو NGX_معلومات_OBJECT_TYPE  0
#وصف_ڪريو NGX_معلومات_LIST_TYPE    1
#وصف_ڪريو NGX_معلومات_STRING_TYPE  2
#وصف_ڪريو NGX_معلومات_INTEGER_TYPE 3
#وصف_ڪريو NGX_معلومات_BOOLEAN_TYPE 4
#وصف_ڪريو NGX_معلومات_NULL_TYPE    5


#وصف_ڪريو ngx_معلومات_new_object(pool)                                             \
    ngx_معلومات_new_شي(pool, NGX_معلومات_OBJECT_TYPE)
#وصف_ڪريو ngx_معلومات_new_list(pool)                                               \
    ngx_معلومات_new_شي(pool, NGX_معلومات_LIST_TYPE)
#وصف_ڪريو ngx_معلومات_new_string(pool)                                             \
    ngx_معلومات_new_شي(pool, NGX_معلومات_STRING_TYPE)
#وصف_ڪريو ngx_معلومات_new_integer(pool)                                            \
    ngx_معلومات_new_شي(pool, NGX_معلومات_INTEGER_TYPE)
#وصف_ڪريو ngx_معلومات_new_boolean(pool)                                            \
    ngx_معلومات_new_شي(pool, NGX_معلومات_BOOLEAN_TYPE)
#وصف_ڪريو ngx_معلومات_new_null(pool)                                               \
    ngx_معلومات_new_شي(pool, NGX_معلومات_NULL_TYPE)


تعريفنوع بنية {
    ngx_str_t            name;
    ngx_معلومات_شي_t   *(*handler)(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx);
    uintptr_t            معلومات;
} ngx_معلومات_decl_t;

#وصف_ڪريو ngx_معلومات_null_decl  { ngx_null_string, عدم, 0 }

#وصف_ڪريو NGX_معلومات_DECLINE  (ngx_معلومات_شي_t *) -1


ngx_معلومات_شي_t *ngx_معلومات_obj_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);
ngx_معلومات_شي_t *ngx_معلومات_obj_fields_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx, ngx_array_t *fields);

ngx_معلومات_شي_t *ngx_معلومات_عدد_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);
ngx_معلومات_شي_t *ngx_معلومات_string_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);
ngx_معلومات_شي_t *ngx_معلومات_boolean_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);
ngx_معلومات_شي_t *ngx_معلومات_وقت_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);

ngx_معلومات_شي_t *ngx_معلومات_struct_int64_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);
ngx_معلومات_شي_t *ngx_معلومات_struct_int_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);
ngx_معلومات_شي_t *ngx_معلومات_struct_atomic_handler(uintptr_t معلومات,
    ngx_pool_t *pool, فراغ *ctx);
ngx_معلومات_شي_t *ngx_معلومات_struct_str_handler(uintptr_t معلومات, ngx_pool_t *pool,
    فراغ *ctx);
ngx_معلومات_شي_t *ngx_معلومات_struct_boolean_handler(uintptr_t معلومات,
    ngx_pool_t *pool, فراغ *ctx);

#جيڪڏهن_پورو /* _NGX_DATA_H_INCLUDED_ */
