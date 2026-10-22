
/*
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NGX_ਅੰਕੜੇ_H_INCLUDED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ਅੰਕੜੇ_H_INCLUDED


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_ਅੰਕੜੇ_ਵਸਤੂ_s ngx_ਅੰਕੜੇ_ਵਸਤੂ_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t          *ਵਸਤੂ;
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t         **ਅਗਲਾ;
} ngx_ਅੰਕੜੇ_obj_t;


ਬਣਤਰ ngx_ਅੰਕੜੇ_ਵਸਤੂ_s {
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t          *ਅਗਲਾ;
    ngx_str_t                 name;
    ngx_uint_t                type;

    ਸੰਘ {
        ngx_ਅੰਕੜੇ_obj_t        object;
        ngx_str_t             string;
        int64_t               integer;
        ngx_uint_t            boolean;
    } ਅੰਕੜੇ;
};


ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ngx_ਅੰਕੜੇ_new_ਵਸਤੂ(ngx_pool_t *pool, ngx_uint_t type);

ਖਾਲੀ ngx_ਅੰਕੜੇ_ਜੋੜੋ_ਵਸਤੂ(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *obj, ngx_str_t *name,
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ਅੰਕੜੇ_OBJECT_TYPE  0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ਅੰਕੜੇ_LIST_TYPE    1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ਅੰਕੜੇ_STRING_TYPE  2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ਅੰਕੜੇ_INTEGER_TYPE 3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ਅੰਕੜੇ_BOOLEAN_TYPE 4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ਅੰਕੜੇ_NULL_TYPE    5


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਅੰਕੜੇ_new_object(pool)                                             \
    ngx_ਅੰਕੜੇ_new_ਵਸਤੂ(pool, NGX_ਅੰਕੜੇ_OBJECT_TYPE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਅੰਕੜੇ_new_list(pool)                                               \
    ngx_ਅੰਕੜੇ_new_ਵਸਤੂ(pool, NGX_ਅੰਕੜੇ_LIST_TYPE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਅੰਕੜੇ_new_string(pool)                                             \
    ngx_ਅੰਕੜੇ_new_ਵਸਤੂ(pool, NGX_ਅੰਕੜੇ_STRING_TYPE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਅੰਕੜੇ_new_integer(pool)                                            \
    ngx_ਅੰਕੜੇ_new_ਵਸਤੂ(pool, NGX_ਅੰਕੜੇ_INTEGER_TYPE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਅੰਕੜੇ_new_boolean(pool)                                            \
    ngx_ਅੰਕੜੇ_new_ਵਸਤੂ(pool, NGX_ਅੰਕੜੇ_BOOLEAN_TYPE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਅੰਕੜੇ_new_null(pool)                                               \
    ngx_ਅੰਕੜੇ_new_ਵਸਤੂ(pool, NGX_ਅੰਕੜੇ_NULL_TYPE)


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_str_t            name;
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t   *(*handler)(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool, ਖਾਲੀ *ctx);
    uintptr_t            ਅੰਕੜੇ;
} ngx_ਅੰਕੜੇ_decl_t;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਅੰਕੜੇ_null_decl  { ngx_null_string, ਨਿਰਾ, 0 }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ਅੰਕੜੇ_DECLINE  (ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *) -1


ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ngx_ਅੰਕੜੇ_obj_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool,
    ਖਾਲੀ *ctx);
ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ngx_ਅੰਕੜੇ_obj_fields_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool,
    ਖਾਲੀ *ctx, ngx_array_t *fields);

ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ngx_ਅੰਕੜੇ_ਸੰਖਿਆ_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool,
    ਖਾਲੀ *ctx);
ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ngx_ਅੰਕੜੇ_string_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool,
    ਖਾਲੀ *ctx);
ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ngx_ਅੰਕੜੇ_boolean_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool,
    ਖਾਲੀ *ctx);
ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ngx_ਅੰਕੜੇ_ਸਮਾਂ_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool,
    ਖਾਲੀ *ctx);

ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ngx_ਅੰਕੜੇ_struct_int64_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool,
    ਖਾਲੀ *ctx);
ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ngx_ਅੰਕੜੇ_struct_int_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool,
    ਖਾਲੀ *ctx);
ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ngx_ਅੰਕੜੇ_struct_atomic_handler(uintptr_t ਅੰਕੜੇ,
    ngx_pool_t *pool, ਖਾਲੀ *ctx);
ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ngx_ਅੰਕੜੇ_struct_str_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool,
    ਖਾਲੀ *ctx);
ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ngx_ਅੰਕੜੇ_struct_boolean_handler(uintptr_t ਅੰਕੜੇ,
    ngx_pool_t *pool, ਖਾਲੀ *ctx);

#ਜੇ_ਅੰਤ /* _NGX_DATA_H_INCLUDED_ */
