
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HASH_H_INCLUDED_
#define _NGX_HASH_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


تعریفنوع ساختار {
    تهی             *مقدار;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


تعریفنوع ساختار {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        size;
} ngx_hash_t;


تعریفنوع ساختار {
    ngx_hash_t        hash;
    تهی             *مقدار;
} ngx_hash_wildcard_t;


تعریفنوع ساختار {
    ngx_str_t         کلید;
    ngx_uint_t        کلید_hash;
    تهی             *مقدار;
} ngx_hash_کلید_t;


تعریفنوع ngx_uint_t (*ngx_hash_کلید_pt) (u_char *data, size_t len);


تعریفنوع ساختار {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_head;
    ngx_hash_wildcard_t  *wc_tail;
} ngx_hash_combined_t;


تعریفنوع ساختار {
    ngx_hash_t       *hash;
    ngx_hash_کلید_pt   کلید;

    ngx_uint_t        max_size;
    ngx_uint_t        bucket_size;

    نویسه             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;
} ngx_hash_init_t;


#define NGX_HASH_SMALL            1
#define NGX_HASH_LARGE            2

#define NGX_HASH_LARGE_ASIZE      16384
#define NGX_HASH_LARGE_HSIZE      10007

#define NGX_HASH_WILDCARD_کلید     1
#define NGX_HASH_READONLY_کلید     2


تعریفنوع ساختار {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;

    ngx_array_t       کلیدها;
    ngx_array_t      *کلیدها_hash;

    ngx_array_t       dns_wc_head;
    ngx_array_t      *dns_wc_head_hash;

    ngx_array_t       dns_wc_tail;
    ngx_array_t      *dns_wc_tail_hash;
} ngx_hash_کلیدها_arrays_t;


تعریفنوع ساختار ngx_table_elt_s  ngx_table_elt_t;

ساختار ngx_table_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         کلید;
    ngx_str_t         مقدار;
    u_char           *lowcase_کلید;
    ngx_table_elt_t  *next;
};


تهی *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t کلید, u_char *name, size_t len);
تهی *ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
تهی *ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
تهی *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t کلید,
    u_char *name, size_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_کلید_t *names,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_کلید_t *names,
    ngx_uint_t nelts);

#define ngx_hash(کلید, c)   ((ngx_uint_t) کلید * 31 + c)
ngx_uint_t ngx_hash_کلید(u_char *data, size_t len);
ngx_uint_t ngx_hash_کلید_lc(u_char *data, size_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, size_t n);


ngx_int_t ngx_hash_کلیدها_array_init(ngx_hash_کلیدها_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_add_کلید(ngx_hash_کلیدها_arrays_t *ha, ngx_str_t *کلید,
    تهی *مقدار, ngx_uint_t flags);


#endif /* _NGX_HASH_H_INCLUDED_ */
