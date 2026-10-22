
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_তথ্য_H_INCLUDED
#define NGX_তথ্য_H_INCLUDED


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


প্রকারলেখ কাঠামো ngx_তথ্য_item_s ngx_তথ্য_item_t;


প্রকারলেখ কাঠামো {
    ngx_তথ্য_item_t          *item;
    ngx_তথ্য_item_t         **পরবর্তী;
} ngx_তথ্য_obj_t;


কাঠামো ngx_তথ্য_item_s {
    ngx_তথ্য_item_t          *পরবর্তী;
    ngx_str_t                 name;
    ngx_uint_t                type;

    সংঘ {
        ngx_তথ্য_obj_t        object;
        ngx_str_t             string;
        int64_t               integer;
        ngx_uint_t            boolean;
    } তথ্য;
};


ngx_তথ্য_item_t *ngx_তথ্য_new_item(ngx_pool_t *pool, ngx_uint_t type);

শূন্য ngx_তথ্য_add_item(ngx_তথ্য_item_t *obj, ngx_str_t *name,
    ngx_তথ্য_item_t *item);

#define NGX_তথ্য_OBJECT_TYPE  0
#define NGX_তথ্য_LIST_TYPE    1
#define NGX_তথ্য_STRING_TYPE  2
#define NGX_তথ্য_INTEGER_TYPE 3
#define NGX_তথ্য_BOOLEAN_TYPE 4
#define NGX_তথ্য_NULL_TYPE    5


#define ngx_তথ্য_new_object(pool)                                             \
    ngx_তথ্য_new_item(pool, NGX_তথ্য_OBJECT_TYPE)
#define ngx_তথ্য_new_list(pool)                                               \
    ngx_তথ্য_new_item(pool, NGX_তথ্য_LIST_TYPE)
#define ngx_তথ্য_new_string(pool)                                             \
    ngx_তথ্য_new_item(pool, NGX_তথ্য_STRING_TYPE)
#define ngx_তথ্য_new_integer(pool)                                            \
    ngx_তথ্য_new_item(pool, NGX_তথ্য_INTEGER_TYPE)
#define ngx_তথ্য_new_boolean(pool)                                            \
    ngx_তথ্য_new_item(pool, NGX_তথ্য_BOOLEAN_TYPE)
#define ngx_তথ্য_new_null(pool)                                               \
    ngx_তথ্য_new_item(pool, NGX_তথ্য_NULL_TYPE)


প্রকারলেখ কাঠামো {
    ngx_str_t            name;
    ngx_তথ্য_item_t   *(*handler)(uintptr_t তথ্য, ngx_pool_t *pool, শূন্য *ctx);
    uintptr_t            তথ্য;
} ngx_তথ্য_decl_t;

#define ngx_তথ্য_null_decl  { ngx_null_string, ফাঁকা, 0 }

#define NGX_তথ্য_DECLINE  (ngx_তথ্য_item_t *) -1


ngx_তথ্য_item_t *ngx_তথ্য_obj_handler(uintptr_t তথ্য, ngx_pool_t *pool,
    শূন্য *ctx);
ngx_তথ্য_item_t *ngx_তথ্য_obj_fields_handler(uintptr_t তথ্য, ngx_pool_t *pool,
    শূন্য *ctx, ngx_array_t *fields);

ngx_তথ্য_item_t *ngx_তথ্য_সংখ্যা_handler(uintptr_t তথ্য, ngx_pool_t *pool,
    শূন্য *ctx);
ngx_তথ্য_item_t *ngx_তথ্য_string_handler(uintptr_t তথ্য, ngx_pool_t *pool,
    শূন্য *ctx);
ngx_তথ্য_item_t *ngx_তথ্য_boolean_handler(uintptr_t তথ্য, ngx_pool_t *pool,
    শূন্য *ctx);
ngx_তথ্য_item_t *ngx_তথ্য_time_handler(uintptr_t তথ্য, ngx_pool_t *pool,
    শূন্য *ctx);

ngx_তথ্য_item_t *ngx_তথ্য_struct_int64_handler(uintptr_t তথ্য, ngx_pool_t *pool,
    শূন্য *ctx);
ngx_তথ্য_item_t *ngx_তথ্য_struct_int_handler(uintptr_t তথ্য, ngx_pool_t *pool,
    শূন্য *ctx);
ngx_তথ্য_item_t *ngx_তথ্য_struct_atomic_handler(uintptr_t তথ্য,
    ngx_pool_t *pool, শূন্য *ctx);
ngx_তথ্য_item_t *ngx_তথ্য_struct_str_handler(uintptr_t তথ্য, ngx_pool_t *pool,
    শূন্য *ctx);
ngx_তথ্য_item_t *ngx_তথ্য_struct_boolean_handler(uintptr_t তথ্য,
    ngx_pool_t *pool, শূন্য *ctx);

#endif /* _NGX_DATA_H_INCLUDED_ */
