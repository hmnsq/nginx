
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HASH_H_INCLUDED_
#define _NGX_HASH_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


પ્રકારવ્યાખ્યા સંરચના {
    શૂન્ય             *કિંમત;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        size;
} ngx_hash_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_hash_t        hash;
    શૂન્ય             *કિંમત;
} ngx_hash_wildcard_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_str_t         ચાવી;
    ngx_uint_t        ચાવી_hash;
    શૂન્ય             *કિંમત;
} ngx_hash_ચાવી_t;


પ્રકારવ્યાખ્યા ngx_uint_t (*ngx_hash_ચાવી_pt) (u_char *data, size_t len);


પ્રકારવ્યાખ્યા સંરચના {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_head;
    ngx_hash_wildcard_t  *wc_tail;
} ngx_hash_combined_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_hash_t       *hash;
    ngx_hash_ચાવી_pt   ચાવી;

    ngx_uint_t        max_size;
    ngx_uint_t        bucket_size;

    અક્ષર             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;
} ngx_hash_init_t;


#define NGX_HASH_SMALL            1
#define NGX_HASH_LARGE            2

#define NGX_HASH_LARGE_ASIZE      16384
#define NGX_HASH_LARGE_HSIZE      10007

#define NGX_HASH_WILDCARD_ચાવી     1
#define NGX_HASH_READONLY_ચાવી     2


પ્રકારવ્યાખ્યા સંરચના {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;

    ngx_array_t       ચાવીઓ;
    ngx_array_t      *ચાવીઓ_hash;

    ngx_array_t       dns_wc_head;
    ngx_array_t      *dns_wc_head_hash;

    ngx_array_t       dns_wc_tail;
    ngx_array_t      *dns_wc_tail_hash;
} ngx_hash_ચાવીઓ_arrays_t;


પ્રકારવ્યાખ્યા સંરચના ngx_table_elt_s  ngx_table_elt_t;

સંરચના ngx_table_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         ચાવી;
    ngx_str_t         કિંમત;
    u_char           *lowcase_ચાવી;
    ngx_table_elt_t  *next;
};


શૂન્ય *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t ચાવી, u_char *name, size_t len);
શૂન્ય *ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
શૂન્ય *ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
શૂન્ય *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t ચાવી,
    u_char *name, size_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_ચાવી_t *names,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_ચાવી_t *names,
    ngx_uint_t nelts);

#define ngx_hash(ચાવી, c)   ((ngx_uint_t) ચાવી * 31 + c)
ngx_uint_t ngx_hash_ચાવી(u_char *data, size_t len);
ngx_uint_t ngx_hash_ચાવી_lc(u_char *data, size_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, size_t n);


ngx_int_t ngx_hash_ચાવીઓ_array_init(ngx_hash_ચાવીઓ_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_add_ચાવી(ngx_hash_ચાવીઓ_arrays_t *ha, ngx_str_t *ચાવી,
    શૂન્ય *કિંમત, ngx_uint_t flags);


#endif /* _NGX_HASH_H_INCLUDED_ */
