
/*
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NGX_ದತ್ತಾಂಶ_H_INCLUDED
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ದತ್ತಾಂಶ_H_INCLUDED


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_ದತ್ತಾಂಶ_ಅಂಶ_s ngx_ದತ್ತಾಂಶ_ಅಂಶ_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t          *ಅಂಶ;
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t         **ಮುಂದಿನ;
} ngx_ದತ್ತಾಂಶ_obj_t;


ರಚನೆ ngx_ದತ್ತಾಂಶ_ಅಂಶ_s {
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t          *ಮುಂದಿನ;
    ngx_str_t                 name;
    ngx_uint_t                type;

    ಒಕ್ಕೂಟ {
        ngx_ದತ್ತಾಂಶ_obj_t        object;
        ngx_str_t             string;
        int64_t               integer;
        ngx_uint_t            boolean;
    } ದತ್ತಾಂಶ;
};


ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ngx_ದತ್ತಾಂಶ_new_ಅಂಶ(ngx_pool_t *pool, ngx_uint_t type);

ಶೂನ್ಯ ngx_ದತ್ತಾಂಶ_ಕೂಡಿಸು_ಅಂಶ(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *obj, ngx_str_t *name,
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ);

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ದತ್ತಾಂಶ_OBJECT_TYPE  0
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ದತ್ತಾಂಶ_LIST_TYPE    1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ದತ್ತಾಂಶ_STRING_TYPE  2
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ದತ್ತಾಂಶ_INTEGER_TYPE 3
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ದತ್ತಾಂಶ_BOOLEAN_TYPE 4
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ದತ್ತಾಂಶ_NULL_TYPE    5


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_ದತ್ತಾಂಶ_new_object(pool)                                             \
    ngx_ದತ್ತಾಂಶ_new_ಅಂಶ(pool, NGX_ದತ್ತಾಂಶ_OBJECT_TYPE)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_ದತ್ತಾಂಶ_new_list(pool)                                               \
    ngx_ದತ್ತಾಂಶ_new_ಅಂಶ(pool, NGX_ದತ್ತಾಂಶ_LIST_TYPE)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_ದತ್ತಾಂಶ_new_string(pool)                                             \
    ngx_ದತ್ತಾಂಶ_new_ಅಂಶ(pool, NGX_ದತ್ತಾಂಶ_STRING_TYPE)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_ದತ್ತಾಂಶ_new_integer(pool)                                            \
    ngx_ದತ್ತಾಂಶ_new_ಅಂಶ(pool, NGX_ದತ್ತಾಂಶ_INTEGER_TYPE)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_ದತ್ತಾಂಶ_new_boolean(pool)                                            \
    ngx_ದತ್ತಾಂಶ_new_ಅಂಶ(pool, NGX_ದತ್ತಾಂಶ_BOOLEAN_TYPE)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_ದತ್ತಾಂಶ_new_null(pool)                                               \
    ngx_ದತ್ತಾಂಶ_new_ಅಂಶ(pool, NGX_ದತ್ತಾಂಶ_NULL_TYPE)


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_str_t            name;
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t   *(*handler)(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool, ಶೂನ್ಯ *ctx);
    uintptr_t            ದತ್ತಾಂಶ;
} ngx_ದತ್ತಾಂಶ_decl_t;

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_ದತ್ತಾಂಶ_null_decl  { ngx_null_string, ಖಾಲಿ, 0 }

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ದತ್ತಾಂಶ_DECLINE  (ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *) -1


ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ngx_ದತ್ತಾಂಶ_obj_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool,
    ಶೂನ್ಯ *ctx);
ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ngx_ದತ್ತಾಂಶ_obj_fields_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool,
    ಶೂನ್ಯ *ctx, ngx_array_t *fields);

ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ngx_ದತ್ತಾಂಶ_ಸಂಖ್ಯೆ_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool,
    ಶೂನ್ಯ *ctx);
ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ngx_ದತ್ತಾಂಶ_string_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool,
    ಶೂನ್ಯ *ctx);
ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ngx_ದತ್ತಾಂಶ_boolean_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool,
    ಶೂನ್ಯ *ctx);
ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ngx_ದತ್ತಾಂಶ_ಸಮಯ_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool,
    ಶೂನ್ಯ *ctx);

ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ngx_ದತ್ತಾಂಶ_struct_int64_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool,
    ಶೂನ್ಯ *ctx);
ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ngx_ದತ್ತಾಂಶ_struct_int_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool,
    ಶೂನ್ಯ *ctx);
ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ngx_ದತ್ತಾಂಶ_struct_atomic_handler(uintptr_t ದತ್ತಾಂಶ,
    ngx_pool_t *pool, ಶೂನ್ಯ *ctx);
ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ngx_ದತ್ತಾಂಶ_struct_str_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool,
    ಶೂನ್ಯ *ctx);
ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ngx_ದತ್ತಾಂಶ_struct_boolean_handler(uintptr_t ದತ್ತಾಂಶ,
    ngx_pool_t *pool, ಶೂನ್ಯ *ctx);

#ಆದರೆ_ಅಂತ್ಯ /* _NGX_DATA_H_INCLUDED_ */
