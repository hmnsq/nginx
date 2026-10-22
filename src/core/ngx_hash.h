
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_HASH_H_INCLUDED_
#घोषणा _NGX_HASH_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


प्रकार काष्ठा {
    व्योम             *मूल्य;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


प्रकार काष्ठा {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        आकार;
} ngx_hash_t;


प्रकार काष्ठा {
    ngx_hash_t        hash;
    व्योम             *मूल्य;
} ngx_hash_wildcard_t;


प्रकार काष्ठा {
    ngx_str_t         कुंजी;
    ngx_uint_t        कुंजी_hash;
    व्योम             *मूल्य;
} ngx_hash_कुंजी_t;


प्रकार ngx_uint_t (*ngx_hash_कुंजी_pt) (u_char *तथ्य, माप_प्रकार len);


प्रकार काष्ठा {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_शीर्ष;
    ngx_hash_wildcard_t  *wc_पुच्छ;
} ngx_hash_combined_t;


प्रकार काष्ठा {
    ngx_hash_t       *hash;
    ngx_hash_कुंजी_pt   कुंजी;

    ngx_uint_t        max_आकार;
    ngx_uint_t        bucket_आकार;

    अक्षर             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *अस्थायी_pool;
} ngx_hash_init_t;


#घोषणा NGX_HASH_SMALL            1
#घोषणा NGX_HASH_LARGE            2

#घोषणा NGX_HASH_LARGE_ASIZE      16384
#घोषणा NGX_HASH_LARGE_HSIZE      10007

#घोषणा NGX_HASH_WILDCARD_कुंजी     1
#घोषणा NGX_HASH_READONLY_कुंजी     2


प्रकार काष्ठा {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *अस्थायी_pool;

    ngx_array_t       कुंजियाँ;
    ngx_array_t      *कुंजियाँ_hash;

    ngx_array_t       dns_wc_शीर्ष;
    ngx_array_t      *dns_wc_शीर्ष_hash;

    ngx_array_t       dns_wc_पुच्छ;
    ngx_array_t      *dns_wc_पुच्छ_hash;
} ngx_hash_कुंजियाँ_arrays_t;


प्रकार काष्ठा ngx_table_elt_s  ngx_table_elt_t;

काष्ठा ngx_table_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         कुंजी;
    ngx_str_t         मूल्य;
    u_char           *lowcase_कुंजी;
    ngx_table_elt_t  *अग्र;
};


व्योम *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t कुंजी, u_char *name, माप_प्रकार len);
व्योम *ngx_hash_find_wc_शीर्ष(ngx_hash_wildcard_t *hwc, u_char *name, माप_प्रकार len);
व्योम *ngx_hash_find_wc_पुच्छ(ngx_hash_wildcard_t *hwc, u_char *name, माप_प्रकार len);
व्योम *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t कुंजी,
    u_char *name, माप_प्रकार len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_कुंजी_t *names,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_कुंजी_t *names,
    ngx_uint_t nelts);

#घोषणा ngx_hash(कुंजी, c)   ((ngx_uint_t) कुंजी * 31 + c)
ngx_uint_t ngx_hash_कुंजी(u_char *तथ्य, माप_प्रकार len);
ngx_uint_t ngx_hash_कुंजी_lc(u_char *तथ्य, माप_प्रकार len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, माप_प्रकार n);


ngx_int_t ngx_hash_कुंजियाँ_array_init(ngx_hash_कुंजियाँ_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_add_कुंजी(ngx_hash_कुंजियाँ_arrays_t *ha, ngx_str_t *कुंजी,
    व्योम *मूल्य, ngx_uint_t flags);


#पूर्ण_अगर /* _NGX_HASH_H_INCLUDED_ */
