
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_HASH_H_INCLUDED_
#تعریف_کرو _NGX_HASH_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


قسمتعریف ساخت {
    خالی             *قدر;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


قسمتعریف ساخت {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        سائز;
} ngx_hash_t;


قسمتعریف ساخت {
    ngx_hash_t        hash;
    خالی             *قدر;
} ngx_hash_wildcard_t;


قسمتعریف ساخت {
    ngx_str_t         کلید;
    ngx_uint_t        کلید_hash;
    خالی             *قدر;
} ngx_hash_کلید_t;


قسمتعریف ngx_uint_t (*ngx_hash_کلید_pt) (u_char *ڈیٹا, سائز_t len);


قسمتعریف ساخت {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_سر;
    ngx_hash_wildcard_t  *wc_دم;
} ngx_hash_combined_t;


قسمتعریف ساخت {
    ngx_hash_t       *hash;
    ngx_hash_کلید_pt   کلید;

    ngx_uint_t        max_سائز;
    ngx_uint_t        بالٹی_سائز;

    حرف             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *عارضی_pool;
} ngx_hash_init_t;


#تعریف_کرو NGX_HASH_SMALL            1
#تعریف_کرو NGX_HASH_LARGE            2

#تعریف_کرو NGX_HASH_LARGE_ASIZE      16384
#تعریف_کرو NGX_HASH_LARGE_HSIZE      10007

#تعریف_کرو NGX_HASH_WILDCARD_کلید     1
#تعریف_کرو NGX_HASH_READONLY_کلید     2


قسمتعریف ساخت {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *عارضی_pool;

    ngx_array_t       کلیدیں;
    ngx_array_t      *کلیدیں_hash;

    ngx_array_t       dns_wc_سر;
    ngx_array_t      *dns_wc_سر_hash;

    ngx_array_t       dns_wc_دم;
    ngx_array_t      *dns_wc_دم_hash;
} ngx_hash_کلیدیں_arrays_t;


قسمتعریف ساخت ngx_جدول_elt_s  ngx_جدول_elt_t;

ساخت ngx_جدول_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         کلید;
    ngx_str_t         قدر;
    u_char           *lowcase_کلید;
    ngx_جدول_elt_t  *اگلا;
};


خالی *ngx_hash_ڈھونڈو(ngx_hash_t *hash, ngx_uint_t کلید, u_char *name, سائز_t len);
خالی *ngx_hash_ڈھونڈو_wc_سر(ngx_hash_wildcard_t *hwc, u_char *name, سائز_t len);
خالی *ngx_hash_ڈھونڈو_wc_دم(ngx_hash_wildcard_t *hwc, u_char *name, سائز_t len);
خالی *ngx_hash_ڈھونڈو_combined(ngx_hash_combined_t *hash, ngx_uint_t کلید,
    u_char *name, سائز_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_کلید_t *نام,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_کلید_t *نام,
    ngx_uint_t nelts);

#تعریف_کرو ngx_hash(کلید, c)   ((ngx_uint_t) کلید * 31 + c)
ngx_uint_t ngx_hash_کلید(u_char *ڈیٹا, سائز_t len);
ngx_uint_t ngx_hash_کلید_lc(u_char *ڈیٹا, سائز_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, سائز_t n);


ngx_int_t ngx_hash_کلیدیں_array_init(ngx_hash_کلیدیں_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_جوڑو_کلید(ngx_hash_کلیدیں_arrays_t *ha, ngx_str_t *کلید,
    خالی *قدر, ngx_uint_t flags);


#اگر_ختم /* _NGX_HASH_H_INCLUDED_ */
