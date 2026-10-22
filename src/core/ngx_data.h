
/*
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ NGX_ڈیٹا_H_INCLUDED
#تعریف_کرو NGX_ڈیٹا_H_INCLUDED


#شامل <ngx_config.h>
#شامل <ngx_core.h>


قسمتعریف ساخت ngx_ڈیٹا_شے_s ngx_ڈیٹا_شے_t;


قسمتعریف ساخت {
    ngx_ڈیٹا_شے_t          *شے;
    ngx_ڈیٹا_شے_t         **اگلا;
} ngx_ڈیٹا_obj_t;


ساخت ngx_ڈیٹا_شے_s {
    ngx_ڈیٹا_شے_t          *اگلا;
    ngx_str_t                 name;
    ngx_uint_t                type;

    اتحاد {
        ngx_ڈیٹا_obj_t        object;
        ngx_str_t             string;
        int64_t               integer;
        ngx_uint_t            boolean;
    } ڈیٹا;
};


ngx_ڈیٹا_شے_t *ngx_ڈیٹا_new_شے(ngx_pool_t *pool, ngx_uint_t type);

خالی ngx_ڈیٹا_جوڑو_شے(ngx_ڈیٹا_شے_t *obj, ngx_str_t *name,
    ngx_ڈیٹا_شے_t *شے);

#تعریف_کرو NGX_ڈیٹا_OBJECT_TYPE  0
#تعریف_کرو NGX_ڈیٹا_LIST_TYPE    1
#تعریف_کرو NGX_ڈیٹا_STRING_TYPE  2
#تعریف_کرو NGX_ڈیٹا_INTEGER_TYPE 3
#تعریف_کرو NGX_ڈیٹا_BOOLEAN_TYPE 4
#تعریف_کرو NGX_ڈیٹا_NULL_TYPE    5


#تعریف_کرو ngx_ڈیٹا_new_object(pool)                                             \
    ngx_ڈیٹا_new_شے(pool, NGX_ڈیٹا_OBJECT_TYPE)
#تعریف_کرو ngx_ڈیٹا_new_list(pool)                                               \
    ngx_ڈیٹا_new_شے(pool, NGX_ڈیٹا_LIST_TYPE)
#تعریف_کرو ngx_ڈیٹا_new_string(pool)                                             \
    ngx_ڈیٹا_new_شے(pool, NGX_ڈیٹا_STRING_TYPE)
#تعریف_کرو ngx_ڈیٹا_new_integer(pool)                                            \
    ngx_ڈیٹا_new_شے(pool, NGX_ڈیٹا_INTEGER_TYPE)
#تعریف_کرو ngx_ڈیٹا_new_boolean(pool)                                            \
    ngx_ڈیٹا_new_شے(pool, NGX_ڈیٹا_BOOLEAN_TYPE)
#تعریف_کرو ngx_ڈیٹا_new_null(pool)                                               \
    ngx_ڈیٹا_new_شے(pool, NGX_ڈیٹا_NULL_TYPE)


قسمتعریف ساخت {
    ngx_str_t            name;
    ngx_ڈیٹا_شے_t   *(*handler)(uintptr_t ڈیٹا, ngx_pool_t *pool, خالی *ctx);
    uintptr_t            ڈیٹا;
} ngx_ڈیٹا_decl_t;

#تعریف_کرو ngx_ڈیٹا_null_decl  { ngx_null_string, تہی, 0 }

#تعریف_کرو NGX_ڈیٹا_DECLINE  (ngx_ڈیٹا_شے_t *) -1


ngx_ڈیٹا_شے_t *ngx_ڈیٹا_obj_handler(uintptr_t ڈیٹا, ngx_pool_t *pool,
    خالی *ctx);
ngx_ڈیٹا_شے_t *ngx_ڈیٹا_obj_fields_handler(uintptr_t ڈیٹا, ngx_pool_t *pool,
    خالی *ctx, ngx_array_t *fields);

ngx_ڈیٹا_شے_t *ngx_ڈیٹا_نمبر_handler(uintptr_t ڈیٹا, ngx_pool_t *pool,
    خالی *ctx);
ngx_ڈیٹا_شے_t *ngx_ڈیٹا_string_handler(uintptr_t ڈیٹا, ngx_pool_t *pool,
    خالی *ctx);
ngx_ڈیٹا_شے_t *ngx_ڈیٹا_boolean_handler(uintptr_t ڈیٹا, ngx_pool_t *pool,
    خالی *ctx);
ngx_ڈیٹا_شے_t *ngx_ڈیٹا_وقت_handler(uintptr_t ڈیٹا, ngx_pool_t *pool,
    خالی *ctx);

ngx_ڈیٹا_شے_t *ngx_ڈیٹا_struct_int64_handler(uintptr_t ڈیٹا, ngx_pool_t *pool,
    خالی *ctx);
ngx_ڈیٹا_شے_t *ngx_ڈیٹا_struct_int_handler(uintptr_t ڈیٹا, ngx_pool_t *pool,
    خالی *ctx);
ngx_ڈیٹا_شے_t *ngx_ڈیٹا_struct_atomic_handler(uintptr_t ڈیٹا,
    ngx_pool_t *pool, خالی *ctx);
ngx_ڈیٹا_شے_t *ngx_ڈیٹا_struct_str_handler(uintptr_t ڈیٹا, ngx_pool_t *pool,
    خالی *ctx);
ngx_ڈیٹا_شے_t *ngx_ڈیٹا_struct_boolean_handler(uintptr_t ڈیٹا,
    ngx_pool_t *pool, خالی *ctx);

#اگر_ختم /* _NGX_DATA_H_INCLUDED_ */
