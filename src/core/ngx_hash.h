
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HASH_H_INCLUDED_
#define _NGX_HASH_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश संरचना {
    शून्य             *मानम्;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


प्रकारनिर्देश संरचना {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        size;
} ngx_hash_t;


प्रकारनिर्देश संरचना {
    ngx_hash_t        hash;
    शून्य             *मानम्;
} ngx_hash_wildcard_t;


प्रकारनिर्देश संरचना {
    ngx_str_t         कुञ्चिका;
    ngx_uint_t        कुञ्चिका_hash;
    शून्य             *मानम्;
} ngx_hash_कुञ्चिका_t;


प्रकारनिर्देश ngx_uint_t (*ngx_hash_कुञ्चिका_pt) (u_char *data, size_t len);


प्रकारनिर्देश संरचना {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_head;
    ngx_hash_wildcard_t  *wc_tail;
} ngx_hash_combined_t;


प्रकारनिर्देश संरचना {
    ngx_hash_t       *hash;
    ngx_hash_कुञ्चिका_pt   कुञ्चिका;

    ngx_uint_t        max_size;
    ngx_uint_t        bucket_size;

    वर्ण             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;
} ngx_hash_init_t;


#define NGX_HASH_SMALL            1
#define NGX_HASH_LARGE            2

#define NGX_HASH_LARGE_ASIZE      16384
#define NGX_HASH_LARGE_HSIZE      10007

#define NGX_HASH_WILDCARD_कुञ्चिका     1
#define NGX_HASH_READONLY_कुञ्चिका     2


प्रकारनिर्देश संरचना {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *temp_pool;

    ngx_array_t       कुञ्चिकाः;
    ngx_array_t      *कुञ्चिकाः_hash;

    ngx_array_t       dns_wc_head;
    ngx_array_t      *dns_wc_head_hash;

    ngx_array_t       dns_wc_tail;
    ngx_array_t      *dns_wc_tail_hash;
} ngx_hash_कुञ्चिकाः_arrays_t;


प्रकारनिर्देश संरचना ngx_table_elt_s  ngx_table_elt_t;

संरचना ngx_table_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         कुञ्चिका;
    ngx_str_t         मानम्;
    u_char           *lowcase_कुञ्चिका;
    ngx_table_elt_t  *next;
};


शून्य *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t कुञ्चिका, u_char *name, size_t len);
शून्य *ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
शून्य *ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
शून्य *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t कुञ्चिका,
    u_char *name, size_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_कुञ्चिका_t *names,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_कुञ्चिका_t *names,
    ngx_uint_t nelts);

#define ngx_hash(कुञ्चिका, c)   ((ngx_uint_t) कुञ्चिका * 31 + c)
ngx_uint_t ngx_hash_कुञ्चिका(u_char *data, size_t len);
ngx_uint_t ngx_hash_कुञ्चिका_lc(u_char *data, size_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, size_t n);


ngx_int_t ngx_hash_कुञ्चिकाः_array_init(ngx_hash_कुञ्चिकाः_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_add_कुञ्चिका(ngx_hash_कुञ्चिकाः_arrays_t *ha, ngx_str_t *कुञ्चिका,
    शून्य *मानम्, ngx_uint_t flags);


#endif /* _NGX_HASH_H_INCLUDED_ */
