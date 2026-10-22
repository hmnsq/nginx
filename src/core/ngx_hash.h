
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HASH_H_INCLUDED_
#define _NGX_HASH_H_INCLUDED_


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>


প্রকারনির্দেশ সংরচনা {
    শূন্য             *মান;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


প্রকারনির্দেশ সংরচনা {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        size;
} ngx_hash_t;


প্রকারনির্দেশ সংরচনা {
    ngx_hash_t        hash;
    শূন্য             *মান;
} ngx_hash_wildcard_t;


প্রকারনির্দেশ সংরচনা {
    ngx_str_t         চাবি;
    ngx_uint_t        চাবি_hash;
    শূন্য             *মান;
} ngx_hash_চাবি_t;


প্রকারনির্দেশ ngx_uint_t (*ngx_hash_চাবি_pt) (u_char *data, size_t len);


প্রকারনির্দেশ সংরচনা {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_head;
    ngx_hash_wildcard_t  *wc_tail;
} ngx_hash_combined_t;


প্রকারনির্দেশ সংরচনা {
    ngx_hash_t       *hash;
    ngx_hash_চাবি_pt   চাবি;

    ngx_uint_t        max_size;
    ngx_uint_t        bucket_size;

    ৱর্ণ             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;
} ngx_hash_init_t;


#define NGX_HASH_SMALL            1
#define NGX_HASH_LARGE            2

#define NGX_HASH_LARGE_ASIZE      16384
#define NGX_HASH_LARGE_HSIZE      10007

#define NGX_HASH_WILDCARD_চাবি     1
#define NGX_HASH_READONLY_চাবি     2


প্রকারনির্দেশ সংরচনা {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;

    ngx_array_t       চাবিগুলি;
    ngx_array_t      *চাবিগুলি_hash;

    ngx_array_t       dns_wc_head;
    ngx_array_t      *dns_wc_head_hash;

    ngx_array_t       dns_wc_tail;
    ngx_array_t      *dns_wc_tail_hash;
} ngx_hash_চাবিগুলি_arrays_t;


প্রকারনির্দেশ সংরচনা ngx_table_elt_s  ngx_table_elt_t;

সংরচনা ngx_table_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         চাবি;
    ngx_str_t         মান;
    u_char           *lowcase_চাবি;
    ngx_table_elt_t  *next;
};


শূন্য *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t চাবি, u_char *name, size_t len);
শূন্য *ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
শূন্য *ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
শূন্য *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t চাবি,
    u_char *name, size_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_চাবি_t *names,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_চাবি_t *names,
    ngx_uint_t nelts);

#define ngx_hash(চাবি, c)   ((ngx_uint_t) চাবি * 31 + c)
ngx_uint_t ngx_hash_চাবি(u_char *data, size_t len);
ngx_uint_t ngx_hash_চাবি_lc(u_char *data, size_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, size_t n);


ngx_int_t ngx_hash_চাবিগুলি_array_init(ngx_hash_চাবিগুলি_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_add_চাবি(ngx_hash_চাবিগুলি_arrays_t *ha, ngx_str_t *চাবি,
    শূন্য *মান, ngx_uint_t flags);


#endif /* _NGX_HASH_H_INCLUDED_ */
