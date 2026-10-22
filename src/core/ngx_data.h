
/*
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే NGX_దత్తాంశం_H_INCLUDED
#నిర్వచించు NGX_దత్తాంశం_H_INCLUDED


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం నిర్మాణం ngx_దత్తాంశం_అంశం_s ngx_దత్తాంశం_అంశం_t;


రకనిర్వచనం నిర్మాణం {
    ngx_దత్తాంశం_అంశం_t          *అంశం;
    ngx_దత్తాంశం_అంశం_t         **తదుపరి;
} ngx_దత్తాంశం_obj_t;


నిర్మాణం ngx_దత్తాంశం_అంశం_s {
    ngx_దత్తాంశం_అంశం_t          *తదుపరి;
    ngx_str_t                 name;
    ngx_uint_t                type;

    సంఘం {
        ngx_దత్తాంశం_obj_t        object;
        ngx_str_t             string;
        int64_t               integer;
        ngx_uint_t            boolean;
    } దత్తాంశం;
};


ngx_దత్తాంశం_అంశం_t *ngx_దత్తాంశం_new_అంశం(ngx_pool_t *pool, ngx_uint_t type);

శూన్య ngx_దత్తాంశం_కూడు_అంశం(ngx_దత్తాంశం_అంశం_t *obj, ngx_str_t *name,
    ngx_దత్తాంశం_అంశం_t *అంశం);

#నిర్వచించు NGX_దత్తాంశం_OBJECT_TYPE  0
#నిర్వచించు NGX_దత్తాంశం_LIST_TYPE    1
#నిర్వచించు NGX_దత్తాంశం_STRING_TYPE  2
#నిర్వచించు NGX_దత్తాంశం_INTEGER_TYPE 3
#నిర్వచించు NGX_దత్తాంశం_BOOLEAN_TYPE 4
#నిర్వచించు NGX_దత్తాంశం_NULL_TYPE    5


#నిర్వచించు ngx_దత్తాంశం_new_object(pool)                                             \
    ngx_దత్తాంశం_new_అంశం(pool, NGX_దత్తాంశం_OBJECT_TYPE)
#నిర్వచించు ngx_దత్తాంశం_new_list(pool)                                               \
    ngx_దత్తాంశం_new_అంశం(pool, NGX_దత్తాంశం_LIST_TYPE)
#నిర్వచించు ngx_దత్తాంశం_new_string(pool)                                             \
    ngx_దత్తాంశం_new_అంశం(pool, NGX_దత్తాంశం_STRING_TYPE)
#నిర్వచించు ngx_దత్తాంశం_new_integer(pool)                                            \
    ngx_దత్తాంశం_new_అంశం(pool, NGX_దత్తాంశం_INTEGER_TYPE)
#నిర్వచించు ngx_దత్తాంశం_new_boolean(pool)                                            \
    ngx_దత్తాంశం_new_అంశం(pool, NGX_దత్తాంశం_BOOLEAN_TYPE)
#నిర్వచించు ngx_దత్తాంశం_new_null(pool)                                               \
    ngx_దత్తాంశం_new_అంశం(pool, NGX_దత్తాంశం_NULL_TYPE)


రకనిర్వచనం నిర్మాణం {
    ngx_str_t            name;
    ngx_దత్తాంశం_అంశం_t   *(*handler)(uintptr_t దత్తాంశం, ngx_pool_t *pool, శూన్య *ctx);
    uintptr_t            దత్తాంశం;
} ngx_దత్తాంశం_decl_t;

#నిర్వచించు ngx_దత్తాంశం_null_decl  { ngx_null_string, ఖాళీ, 0 }

#నిర్వచించు NGX_దత్తాంశం_DECLINE  (ngx_దత్తాంశం_అంశం_t *) -1


ngx_దత్తాంశం_అంశం_t *ngx_దత్తాంశం_obj_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool,
    శూన్య *ctx);
ngx_దత్తాంశం_అంశం_t *ngx_దత్తాంశం_obj_fields_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool,
    శూన్య *ctx, ngx_array_t *fields);

ngx_దత్తాంశం_అంశం_t *ngx_దత్తాంశం_సంఖ్య_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool,
    శూన్య *ctx);
ngx_దత్తాంశం_అంశం_t *ngx_దత్తాంశం_string_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool,
    శూన్య *ctx);
ngx_దత్తాంశం_అంశం_t *ngx_దత్తాంశం_boolean_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool,
    శూన్య *ctx);
ngx_దత్తాంశం_అంశం_t *ngx_దత్తాంశం_సమయం_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool,
    శూన్య *ctx);

ngx_దత్తాంశం_అంశం_t *ngx_దత్తాంశం_struct_int64_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool,
    శూన్య *ctx);
ngx_దత్తాంశం_అంశం_t *ngx_దత్తాంశం_struct_int_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool,
    శూన్య *ctx);
ngx_దత్తాంశం_అంశం_t *ngx_దత్తాంశం_struct_atomic_handler(uintptr_t దత్తాంశం,
    ngx_pool_t *pool, శూన్య *ctx);
ngx_దత్తాంశం_అంశం_t *ngx_దత్తాంశం_struct_str_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool,
    శూన్య *ctx);
ngx_దత్తాంశం_అంశం_t *ngx_దత్తాంశం_struct_boolean_handler(uintptr_t దత్తాంశం,
    ngx_pool_t *pool, శూన్య *ctx);

#అయితే_ముగింపు /* _NGX_DATA_H_INCLUDED_ */
