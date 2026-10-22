
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HASH_H_INCLUDED_
#define _NGX_HASH_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಶೂನ್ಯ             *ಮೌಲ್ಯ;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        size;
} ngx_hash_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_hash_t        hash;
    ಶೂನ್ಯ             *ಮೌಲ್ಯ;
} ngx_hash_wildcard_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_str_t         ಕೀಲಿ;
    ngx_uint_t        ಕೀಲಿ_hash;
    ಶೂನ್ಯ             *ಮೌಲ್ಯ;
} ngx_hash_ಕೀಲಿ_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ngx_uint_t (*ngx_hash_ಕೀಲಿ_pt) (u_char *data, size_t len);


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_head;
    ngx_hash_wildcard_t  *wc_tail;
} ngx_hash_combined_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_hash_t       *hash;
    ngx_hash_ಕೀಲಿ_pt   ಕೀಲಿ;

    ngx_uint_t        max_size;
    ngx_uint_t        bucket_size;

    ವರ್ಣ             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;
} ngx_hash_init_t;


#define NGX_HASH_SMALL            1
#define NGX_HASH_LARGE            2

#define NGX_HASH_LARGE_ASIZE      16384
#define NGX_HASH_LARGE_HSIZE      10007

#define NGX_HASH_WILDCARD_ಕೀಲಿ     1
#define NGX_HASH_READONLY_ಕೀಲಿ     2


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;

    ngx_array_t       ಕೀಲಿಗಳು;
    ngx_array_t      *ಕೀಲಿಗಳು_hash;

    ngx_array_t       dns_wc_head;
    ngx_array_t      *dns_wc_head_hash;

    ngx_array_t       dns_wc_tail;
    ngx_array_t      *dns_wc_tail_hash;
} ngx_hash_ಕೀಲಿಗಳು_arrays_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_table_elt_s  ngx_table_elt_t;

ಸಂರಚನಾ ngx_table_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         ಕೀಲಿ;
    ngx_str_t         ಮೌಲ್ಯ;
    u_char           *lowcase_ಕೀಲಿ;
    ngx_table_elt_t  *next;
};


ಶೂನ್ಯ *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t ಕೀಲಿ, u_char *name, size_t len);
ಶೂನ್ಯ *ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
ಶೂನ್ಯ *ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
ಶೂನ್ಯ *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t ಕೀಲಿ,
    u_char *name, size_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_ಕೀಲಿ_t *names,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_ಕೀಲಿ_t *names,
    ngx_uint_t nelts);

#define ngx_hash(ಕೀಲಿ, c)   ((ngx_uint_t) ಕೀಲಿ * 31 + c)
ngx_uint_t ngx_hash_ಕೀಲಿ(u_char *data, size_t len);
ngx_uint_t ngx_hash_ಕೀಲಿ_lc(u_char *data, size_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, size_t n);


ngx_int_t ngx_hash_ಕೀಲಿಗಳು_array_init(ngx_hash_ಕೀಲಿಗಳು_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_add_ಕೀಲಿ(ngx_hash_ಕೀಲಿಗಳು_arrays_t *ha, ngx_str_t *ಕೀಲಿ,
    ಶೂನ್ಯ *ಮೌಲ್ಯ, ngx_uint_t flags);


#endif /* _NGX_HASH_H_INCLUDED_ */
