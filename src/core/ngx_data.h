
/*
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف NGX_انکڑے_H_INCLUDED
#تعریف_کرو NGX_انکڑے_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_انکڑے_شے_s ngx_انکڑے_شے_t;


تعريفنوع بنية {
    ngx_انکڑے_شے_t          *شے;
    ngx_انکڑے_شے_t         **اگلا;
} ngx_انکڑے_obj_t;


بنية ngx_انکڑے_شے_s {
    ngx_انکڑے_شے_t          *اگلا;
    ngx_str_t                 name;
    ngx_uint_t                type;

    اتحاد {
        ngx_انکڑے_obj_t        object;
        ngx_str_t             string;
        int64_t               integer;
        ngx_uint_t            boolean;
    } انکڑے;
};


ngx_انکڑے_شے_t *ngx_انکڑے_new_شے(ngx_pool_t *pool, ngx_uint_t type);

فراغ ngx_انکڑے_جوڑو_شے(ngx_انکڑے_شے_t *obj, ngx_str_t *name,
    ngx_انکڑے_شے_t *شے);

#تعریف_کرو NGX_انکڑے_OBJECT_TYPE  0
#تعریف_کرو NGX_انکڑے_LIST_TYPE    1
#تعریف_کرو NGX_انکڑے_STRING_TYPE  2
#تعریف_کرو NGX_انکڑے_INTEGER_TYPE 3
#تعریف_کرو NGX_انکڑے_BOOLEAN_TYPE 4
#تعریف_کرو NGX_انکڑے_NULL_TYPE    5


#تعریف_کرو ngx_انکڑے_new_object(pool)                                             \
    ngx_انکڑے_new_شے(pool, NGX_انکڑے_OBJECT_TYPE)
#تعریف_کرو ngx_انکڑے_new_list(pool)                                               \
    ngx_انکڑے_new_شے(pool, NGX_انکڑے_LIST_TYPE)
#تعریف_کرو ngx_انکڑے_new_string(pool)                                             \
    ngx_انکڑے_new_شے(pool, NGX_انکڑے_STRING_TYPE)
#تعریف_کرو ngx_انکڑے_new_integer(pool)                                            \
    ngx_انکڑے_new_شے(pool, NGX_انکڑے_INTEGER_TYPE)
#تعریف_کرو ngx_انکڑے_new_boolean(pool)                                            \
    ngx_انکڑے_new_شے(pool, NGX_انکڑے_BOOLEAN_TYPE)
#تعریف_کرو ngx_انکڑے_new_null(pool)                                               \
    ngx_انکڑے_new_شے(pool, NGX_انکڑے_NULL_TYPE)


تعريفنوع بنية {
    ngx_str_t            name;
    ngx_انکڑے_شے_t   *(*handler)(uintptr_t انکڑے, ngx_pool_t *pool, فراغ *ctx);
    uintptr_t            انکڑے;
} ngx_انکڑے_decl_t;

#تعریف_کرو ngx_انکڑے_null_decl  { ngx_null_string, عدم, 0 }

#تعریف_کرو NGX_انکڑے_DECLINE  (ngx_انکڑے_شے_t *) -1


ngx_انکڑے_شے_t *ngx_انکڑے_obj_handler(uintptr_t انکڑے, ngx_pool_t *pool,
    فراغ *ctx);
ngx_انکڑے_شے_t *ngx_انکڑے_obj_fields_handler(uintptr_t انکڑے, ngx_pool_t *pool,
    فراغ *ctx, ngx_array_t *fields);

ngx_انکڑے_شے_t *ngx_انکڑے_نمبر_handler(uintptr_t انکڑے, ngx_pool_t *pool,
    فراغ *ctx);
ngx_انکڑے_شے_t *ngx_انکڑے_string_handler(uintptr_t انکڑے, ngx_pool_t *pool,
    فراغ *ctx);
ngx_انکڑے_شے_t *ngx_انکڑے_boolean_handler(uintptr_t انکڑے, ngx_pool_t *pool,
    فراغ *ctx);
ngx_انکڑے_شے_t *ngx_انکڑے_ویلا_handler(uintptr_t انکڑے, ngx_pool_t *pool,
    فراغ *ctx);

ngx_انکڑے_شے_t *ngx_انکڑے_struct_int64_handler(uintptr_t انکڑے, ngx_pool_t *pool,
    فراغ *ctx);
ngx_انکڑے_شے_t *ngx_انکڑے_struct_int_handler(uintptr_t انکڑے, ngx_pool_t *pool,
    فراغ *ctx);
ngx_انکڑے_شے_t *ngx_انکڑے_struct_atomic_handler(uintptr_t انکڑے,
    ngx_pool_t *pool, فراغ *ctx);
ngx_انکڑے_شے_t *ngx_انکڑے_struct_str_handler(uintptr_t انکڑے, ngx_pool_t *pool,
    فراغ *ctx);
ngx_انکڑے_شے_t *ngx_انکڑے_struct_boolean_handler(uintptr_t انکڑے,
    ngx_pool_t *pool, فراغ *ctx);

#جے_انت /* _NGX_DATA_H_INCLUDED_ */
