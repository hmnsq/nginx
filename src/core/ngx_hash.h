
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HASH_H_INCLUDED_
#define _NGX_HASH_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଶୂନ୍ୟ             *ମୂଲ୍ୟ;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        size;
} ngx_hash_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_hash_t        hash;
    ଶୂନ୍ୟ             *ମୂଲ୍ୟ;
} ngx_hash_wildcard_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_str_t         ଚାବି;
    ngx_uint_t        ଚାବି_hash;
    ଶୂନ୍ୟ             *ମୂଲ୍ୟ;
} ngx_hash_ଚାବି_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ngx_uint_t (*ngx_hash_ଚାବି_pt) (u_char *data, size_t len);


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_head;
    ngx_hash_wildcard_t  *wc_tail;
} ngx_hash_combined_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_hash_t       *hash;
    ngx_hash_ଚାବି_pt   ଚାବି;

    ngx_uint_t        max_size;
    ngx_uint_t        bucket_size;

    ଅକ୍ଷର             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;
} ngx_hash_init_t;


#define NGX_HASH_SMALL            1
#define NGX_HASH_LARGE            2

#define NGX_HASH_LARGE_ASIZE      16384
#define NGX_HASH_LARGE_HSIZE      10007

#define NGX_HASH_WILDCARD_ଚାବି     1
#define NGX_HASH_READONLY_ଚାବି     2


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;

    ngx_array_t       ଚାବିଗୁଡ଼ିକ;
    ngx_array_t      *ଚାବିଗୁଡ଼ିକ_hash;

    ngx_array_t       dns_wc_head;
    ngx_array_t      *dns_wc_head_hash;

    ngx_array_t       dns_wc_tail;
    ngx_array_t      *dns_wc_tail_hash;
} ngx_hash_ଚାବିଗୁଡ଼ିକ_arrays_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_table_elt_s  ngx_table_elt_t;

ସଂରଚନା ngx_table_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         ଚାବି;
    ngx_str_t         ମୂଲ୍ୟ;
    u_char           *lowcase_ଚାବି;
    ngx_table_elt_t  *next;
};


ଶୂନ୍ୟ *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t ଚାବି, u_char *name, size_t len);
ଶୂନ୍ୟ *ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
ଶୂନ୍ୟ *ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
ଶୂନ୍ୟ *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t ଚାବି,
    u_char *name, size_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_ଚାବି_t *names,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_ଚାବି_t *names,
    ngx_uint_t nelts);

#define ngx_hash(ଚାବି, c)   ((ngx_uint_t) ଚାବି * 31 + c)
ngx_uint_t ngx_hash_ଚାବି(u_char *data, size_t len);
ngx_uint_t ngx_hash_ଚାବି_lc(u_char *data, size_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, size_t n);


ngx_int_t ngx_hash_ଚାବିଗୁଡ଼ିକ_array_init(ngx_hash_ଚାବିଗୁଡ଼ିକ_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_add_ଚାବି(ngx_hash_ଚାବିଗୁଡ଼ିକ_arrays_t *ha, ngx_str_t *ଚାବି,
    ଶୂନ୍ୟ *ମୂଲ୍ୟ, ngx_uint_t flags);


#endif /* _NGX_HASH_H_INCLUDED_ */
