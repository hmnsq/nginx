
/*
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित NGX_तथ्य_H_INCLUDED
#घोषणा NGX_तथ्य_H_INCLUDED


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


प्रकार काष्ठा ngx_तथ्य_item_s ngx_तथ्य_item_t;


प्रकार काष्ठा {
    ngx_तथ्य_item_t          *item;
    ngx_तथ्य_item_t         **अग्र;
} ngx_तथ्य_obj_t;


काष्ठा ngx_तथ्य_item_s {
    ngx_तथ्य_item_t          *अग्र;
    ngx_str_t                 name;
    ngx_uint_t                type;

    जोड़ {
        ngx_तथ्य_obj_t        object;
        ngx_str_t             string;
        int64_t               integer;
        ngx_uint_t            boolean;
    } तथ्य;
};


ngx_तथ्य_item_t *ngx_तथ्य_new_item(ngx_pool_t *pool, ngx_uint_t type);

व्योम ngx_तथ्य_add_item(ngx_तथ्य_item_t *obj, ngx_str_t *name,
    ngx_तथ्य_item_t *item);

#घोषणा NGX_तथ्य_OBJECT_TYPE  0
#घोषणा NGX_तथ्य_LIST_TYPE    1
#घोषणा NGX_तथ्य_STRING_TYPE  2
#घोषणा NGX_तथ्य_INTEGER_TYPE 3
#घोषणा NGX_तथ्य_BOOLEAN_TYPE 4
#घोषणा NGX_तथ्य_NULL_TYPE    5


#घोषणा ngx_तथ्य_new_object(pool)                                             \
    ngx_तथ्य_new_item(pool, NGX_तथ्य_OBJECT_TYPE)
#घोषणा ngx_तथ्य_new_list(pool)                                               \
    ngx_तथ्य_new_item(pool, NGX_तथ्य_LIST_TYPE)
#घोषणा ngx_तथ्य_new_string(pool)                                             \
    ngx_तथ्य_new_item(pool, NGX_तथ्य_STRING_TYPE)
#घोषणा ngx_तथ्य_new_integer(pool)                                            \
    ngx_तथ्य_new_item(pool, NGX_तथ्य_INTEGER_TYPE)
#घोषणा ngx_तथ्य_new_boolean(pool)                                            \
    ngx_तथ्य_new_item(pool, NGX_तथ्य_BOOLEAN_TYPE)
#घोषणा ngx_तथ्य_new_null(pool)                                               \
    ngx_तथ्य_new_item(pool, NGX_तथ्य_NULL_TYPE)


प्रकार काष्ठा {
    ngx_str_t            name;
    ngx_तथ्य_item_t   *(*handler)(uintptr_t तथ्य, ngx_pool_t *pool, व्योम *ctx);
    uintptr_t            तथ्य;
} ngx_तथ्य_decl_t;

#घोषणा ngx_तथ्य_null_decl  { ngx_null_string, शून्य, 0 }

#घोषणा NGX_तथ्य_DECLINE  (ngx_तथ्य_item_t *) -1


ngx_तथ्य_item_t *ngx_तथ्य_obj_handler(uintptr_t तथ्य, ngx_pool_t *pool,
    व्योम *ctx);
ngx_तथ्य_item_t *ngx_तथ्य_obj_fields_handler(uintptr_t तथ्य, ngx_pool_t *pool,
    व्योम *ctx, ngx_array_t *fields);

ngx_तथ्य_item_t *ngx_तथ्य_संख्या_handler(uintptr_t तथ्य, ngx_pool_t *pool,
    व्योम *ctx);
ngx_तथ्य_item_t *ngx_तथ्य_string_handler(uintptr_t तथ्य, ngx_pool_t *pool,
    व्योम *ctx);
ngx_तथ्य_item_t *ngx_तथ्य_boolean_handler(uintptr_t तथ्य, ngx_pool_t *pool,
    व्योम *ctx);
ngx_तथ्य_item_t *ngx_तथ्य_time_handler(uintptr_t तथ्य, ngx_pool_t *pool,
    व्योम *ctx);

ngx_तथ्य_item_t *ngx_तथ्य_struct_int64_handler(uintptr_t तथ्य, ngx_pool_t *pool,
    व्योम *ctx);
ngx_तथ्य_item_t *ngx_तथ्य_struct_int_handler(uintptr_t तथ्य, ngx_pool_t *pool,
    व्योम *ctx);
ngx_तथ्य_item_t *ngx_तथ्य_struct_atomic_handler(uintptr_t तथ्य,
    ngx_pool_t *pool, व्योम *ctx);
ngx_तथ्य_item_t *ngx_तथ्य_struct_str_handler(uintptr_t तथ्य, ngx_pool_t *pool,
    व्योम *ctx);
ngx_तथ्य_item_t *ngx_तथ्य_struct_boolean_handler(uintptr_t तथ्य,
    ngx_pool_t *pool, व्योम *ctx);

#पूर्ण_अगर /* _NGX_DATA_H_INCLUDED_ */
