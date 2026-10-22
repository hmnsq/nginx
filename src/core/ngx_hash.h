
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_HASH_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_HASH_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਖਾਲੀ             *ਮੁੱਲ;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        ਮਾਪ;
} ngx_hash_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_hash_t        hash;
    ਖਾਲੀ             *ਮੁੱਲ;
} ngx_hash_wildcard_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_str_t         ਕੁੰਜੀ;
    ngx_uint_t        ਕੁੰਜੀ_hash;
    ਖਾਲੀ             *ਮੁੱਲ;
} ngx_hash_ਕੁੰਜੀ_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ngx_uint_t (*ngx_hash_ਕੁੰਜੀ_pt) (u_char *ਅੰਕੜੇ, ਮਾਪ_t len);


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_ਸਿਰ;
    ngx_hash_wildcard_t  *wc_ਪੂਛ;
} ngx_hash_combined_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_hash_t       *hash;
    ngx_hash_ਕੁੰਜੀ_pt   ਕੁੰਜੀ;

    ngx_uint_t        max_ਮਾਪ;
    ngx_uint_t        ਬਾਲਟੀ_ਮਾਪ;

    ਅੱਖਰ             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *ਆਰਜ਼ੀ_pool;
} ngx_hash_init_t;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_HASH_SMALL            1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_HASH_LARGE            2

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_HASH_LARGE_ASIZE      16384
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_HASH_LARGE_HSIZE      10007

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_HASH_WILDCARD_ਕੁੰਜੀ     1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_HASH_READONLY_ਕੁੰਜੀ     2


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *ਆਰਜ਼ੀ_pool;

    ngx_array_t       ਕੁੰਜੀਆਂ;
    ngx_array_t      *ਕੁੰਜੀਆਂ_hash;

    ngx_array_t       dns_wc_ਸਿਰ;
    ngx_array_t      *dns_wc_ਸਿਰ_hash;

    ngx_array_t       dns_wc_ਪੂਛ;
    ngx_array_t      *dns_wc_ਪੂਛ_hash;
} ngx_hash_ਕੁੰਜੀਆਂ_arrays_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_ਸਾਰਣੀ_elt_s  ngx_ਸਾਰਣੀ_elt_t;

ਬਣਤਰ ngx_ਸਾਰਣੀ_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         ਕੁੰਜੀ;
    ngx_str_t         ਮੁੱਲ;
    u_char           *lowcase_ਕੁੰਜੀ;
    ngx_ਸਾਰਣੀ_elt_t  *ਅਗਲਾ;
};


ਖਾਲੀ *ngx_hash_ਲੱਭੋ(ngx_hash_t *hash, ngx_uint_t ਕੁੰਜੀ, u_char *name, ਮਾਪ_t len);
ਖਾਲੀ *ngx_hash_ਲੱਭੋ_wc_ਸਿਰ(ngx_hash_wildcard_t *hwc, u_char *name, ਮਾਪ_t len);
ਖਾਲੀ *ngx_hash_ਲੱਭੋ_wc_ਪੂਛ(ngx_hash_wildcard_t *hwc, u_char *name, ਮਾਪ_t len);
ਖਾਲੀ *ngx_hash_ਲੱਭੋ_combined(ngx_hash_combined_t *hash, ngx_uint_t ਕੁੰਜੀ,
    u_char *name, ਮਾਪ_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_ਕੁੰਜੀ_t *ਨਾਂ,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_ਕੁੰਜੀ_t *ਨਾਂ,
    ngx_uint_t nelts);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_hash(ਕੁੰਜੀ, c)   ((ngx_uint_t) ਕੁੰਜੀ * 31 + c)
ngx_uint_t ngx_hash_ਕੁੰਜੀ(u_char *ਅੰਕੜੇ, ਮਾਪ_t len);
ngx_uint_t ngx_hash_ਕੁੰਜੀ_lc(u_char *ਅੰਕੜੇ, ਮਾਪ_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, ਮਾਪ_t n);


ngx_int_t ngx_hash_ਕੁੰਜੀਆਂ_array_init(ngx_hash_ਕੁੰਜੀਆਂ_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_ਜੋੜੋ_ਕੁੰਜੀ(ngx_hash_ਕੁੰਜੀਆਂ_arrays_t *ha, ngx_str_t *ਕੁੰਜੀ,
    ਖਾਲੀ *ਮੁੱਲ, ngx_uint_t flags);


#ਜੇ_ਅੰਤ /* _NGX_HASH_H_INCLUDED_ */
