
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HASH_H_INCLUDED_
#define _NGX_HASH_H_INCLUDED_


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


வகைவரை கட்டமைப்பு {
    வெறுமை             *மதிப்பு;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


வகைவரை கட்டமைப்பு {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        size;
} ngx_hash_t;


வகைவரை கட்டமைப்பு {
    ngx_hash_t        hash;
    வெறுமை             *மதிப்பு;
} ngx_hash_wildcard_t;


வகைவரை கட்டமைப்பு {
    ngx_str_t         சாவி;
    ngx_uint_t        சாவி_hash;
    வெறுமை             *மதிப்பு;
} ngx_hash_சாவி_t;


வகைவரை ngx_uint_t (*ngx_hash_சாவி_pt) (u_char *data, size_t len);


வகைவரை கட்டமைப்பு {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_head;
    ngx_hash_wildcard_t  *wc_tail;
} ngx_hash_combined_t;


வகைவரை கட்டமைப்பு {
    ngx_hash_t       *hash;
    ngx_hash_சாவி_pt   சாவி;

    ngx_uint_t        max_size;
    ngx_uint_t        bucket_size;

    எழுத்து             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;
} ngx_hash_init_t;


#define NGX_HASH_SMALL            1
#define NGX_HASH_LARGE            2

#define NGX_HASH_LARGE_ASIZE      16384
#define NGX_HASH_LARGE_HSIZE      10007

#define NGX_HASH_WILDCARD_சாவி     1
#define NGX_HASH_READONLY_சாவி     2


வகைவரை கட்டமைப்பு {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;

    ngx_array_t       சாவிகள்;
    ngx_array_t      *சாவிகள்_hash;

    ngx_array_t       dns_wc_head;
    ngx_array_t      *dns_wc_head_hash;

    ngx_array_t       dns_wc_tail;
    ngx_array_t      *dns_wc_tail_hash;
} ngx_hash_சாவிகள்_arrays_t;


வகைவரை கட்டமைப்பு ngx_table_elt_s  ngx_table_elt_t;

கட்டமைப்பு ngx_table_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         சாவி;
    ngx_str_t         மதிப்பு;
    u_char           *lowcase_சாவி;
    ngx_table_elt_t  *next;
};


வெறுமை *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t சாவி, u_char *name, size_t len);
வெறுமை *ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
வெறுமை *ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
வெறுமை *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t சாவி,
    u_char *name, size_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_சாவி_t *names,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_சாவி_t *names,
    ngx_uint_t nelts);

#define ngx_hash(சாவி, c)   ((ngx_uint_t) சாவி * 31 + c)
ngx_uint_t ngx_hash_சாவி(u_char *data, size_t len);
ngx_uint_t ngx_hash_சாவி_lc(u_char *data, size_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, size_t n);


ngx_int_t ngx_hash_சாவிகள்_array_init(ngx_hash_சாவிகள்_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_add_சாவி(ngx_hash_சாவிகள்_arrays_t *ha, ngx_str_t *சாவி,
    வெறுமை *மதிப்பு, ngx_uint_t flags);


#endif /* _NGX_HASH_H_INCLUDED_ */
