
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_DATA_H_INCLUDED_
#define _NGX_DATA_H_INCLUDED_


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


வகைவரை கட்டமைப்பு ngx_data_item_s ngx_data_item_t;


வகைவரை கட்டமைப்பு {
    ngx_data_item_t          *item;
    ngx_data_item_t         **next;
} ngx_data_obj_t;


கட்டமைப்பு ngx_data_item_s {
    ngx_data_item_t          *next;
    ngx_str_t                 name;
    ngx_uint_t                type;

    ஒன்றிப்பு {
        ngx_data_obj_t        object;
        ngx_str_t             string;
        int64_t               integer;
        ngx_uint_t            boolean;
    } data;
};


ngx_data_item_t *ngx_data_new_item(ngx_pool_t *pool, ngx_uint_t type);

வெறுமை ngx_data_add_item(ngx_data_item_t *obj, ngx_str_t *name,
    ngx_data_item_t *item);

#define NGX_DATA_OBJECT_TYPE  0
#define NGX_DATA_LIST_TYPE    1
#define NGX_DATA_STRING_TYPE  2
#define NGX_DATA_INTEGER_TYPE 3
#define NGX_DATA_BOOLEAN_TYPE 4
#define NGX_DATA_NULL_TYPE    5


#define ngx_data_new_object(pool)                                             \
    ngx_data_new_item(pool, NGX_DATA_OBJECT_TYPE)
#define ngx_data_new_list(pool)                                               \
    ngx_data_new_item(pool, NGX_DATA_LIST_TYPE)
#define ngx_data_new_string(pool)                                             \
    ngx_data_new_item(pool, NGX_DATA_STRING_TYPE)
#define ngx_data_new_integer(pool)                                            \
    ngx_data_new_item(pool, NGX_DATA_INTEGER_TYPE)
#define ngx_data_new_boolean(pool)                                            \
    ngx_data_new_item(pool, NGX_DATA_BOOLEAN_TYPE)
#define ngx_data_new_null(pool)                                               \
    ngx_data_new_item(pool, NGX_DATA_NULL_TYPE)


வகைவரை கட்டமைப்பு {
    ngx_str_t            name;
    ngx_data_item_t   *(*handler)(uintptr_t data, ngx_pool_t *pool, வெறுமை *ctx);
    uintptr_t            data;
} ngx_data_decl_t;

#define ngx_data_null_decl  { ngx_null_string, வெற்று, 0 }

#define NGX_DATA_DECLINE  (ngx_data_item_t *) -1


ngx_data_item_t *ngx_data_obj_handler(uintptr_t data, ngx_pool_t *pool,
    வெறுமை *ctx);
ngx_data_item_t *ngx_data_obj_fields_handler(uintptr_t data, ngx_pool_t *pool,
    வெறுமை *ctx, ngx_array_t *fields);

ngx_data_item_t *ngx_data_எண்_handler(uintptr_t data, ngx_pool_t *pool,
    வெறுமை *ctx);
ngx_data_item_t *ngx_data_string_handler(uintptr_t data, ngx_pool_t *pool,
    வெறுமை *ctx);
ngx_data_item_t *ngx_data_boolean_handler(uintptr_t data, ngx_pool_t *pool,
    வெறுமை *ctx);
ngx_data_item_t *ngx_data_time_handler(uintptr_t data, ngx_pool_t *pool,
    வெறுமை *ctx);

ngx_data_item_t *ngx_data_struct_int64_handler(uintptr_t data, ngx_pool_t *pool,
    வெறுமை *ctx);
ngx_data_item_t *ngx_data_struct_int_handler(uintptr_t data, ngx_pool_t *pool,
    வெறுமை *ctx);
ngx_data_item_t *ngx_data_struct_atomic_handler(uintptr_t data,
    ngx_pool_t *pool, வெறுமை *ctx);
ngx_data_item_t *ngx_data_struct_str_handler(uintptr_t data, ngx_pool_t *pool,
    வெறுமை *ctx);
ngx_data_item_t *ngx_data_struct_boolean_handler(uintptr_t data,
    ngx_pool_t *pool, வெறுமை *ctx);

#endif /* _NGX_DATA_H_INCLUDED_ */
