
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HASH_H_INCLUDED_
#define _NGX_HASH_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    فراغ             *بەها;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


تعريفنوع بنية {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        size;
} ngx_hash_t;


تعريفنوع بنية {
    ngx_hash_t        hash;
    فراغ             *بەها;
} ngx_hash_wildcard_t;


تعريفنوع بنية {
    ngx_str_t         کلیل;
    ngx_uint_t        کلیل_hash;
    فراغ             *بەها;
} ngx_hash_کلیل_t;


تعريفنوع ngx_uint_t (*ngx_hash_کلیل_pt) (u_char *data, size_t len);


تعريفنوع بنية {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_head;
    ngx_hash_wildcard_t  *wc_tail;
} ngx_hash_combined_t;


تعريفنوع بنية {
    ngx_hash_t       *hash;
    ngx_hash_کلیل_pt   کلیل;

    ngx_uint_t        max_size;
    ngx_uint_t        bucket_size;

    حرف             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;
} ngx_hash_init_t;


#define NGX_HASH_SMALL            1
#define NGX_HASH_LARGE            2

#define NGX_HASH_LARGE_ASIZE      16384
#define NGX_HASH_LARGE_HSIZE      10007

#define NGX_HASH_WILDCARD_کلیل     1
#define NGX_HASH_READONLY_کلیل     2


تعريفنوع بنية {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;

    ngx_array_t       کلیلەکان;
    ngx_array_t      *کلیلەکان_hash;

    ngx_array_t       dns_wc_head;
    ngx_array_t      *dns_wc_head_hash;

    ngx_array_t       dns_wc_tail;
    ngx_array_t      *dns_wc_tail_hash;
} ngx_hash_کلیلەکان_arrays_t;


تعريفنوع بنية ngx_table_elt_s  ngx_table_elt_t;

بنية ngx_table_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         کلیل;
    ngx_str_t         بەها;
    u_char           *lowcase_کلیل;
    ngx_table_elt_t  *next;
};


فراغ *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t کلیل, u_char *name, size_t len);
فراغ *ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
فراغ *ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
فراغ *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t کلیل,
    u_char *name, size_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_کلیل_t *names,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_کلیل_t *names,
    ngx_uint_t nelts);

#define ngx_hash(کلیل, c)   ((ngx_uint_t) کلیل * 31 + c)
ngx_uint_t ngx_hash_کلیل(u_char *data, size_t len);
ngx_uint_t ngx_hash_کلیل_lc(u_char *data, size_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, size_t n);


ngx_int_t ngx_hash_کلیلەکان_array_init(ngx_hash_کلیلەکان_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_add_کلیل(ngx_hash_کلیلەکان_arrays_t *ha, ngx_str_t *کلیل,
    فراغ *بەها, ngx_uint_t flags);


#endif /* _NGX_HASH_H_INCLUDED_ */
