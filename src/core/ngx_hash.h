
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_HASH_H_INCLUDED_
#تعریف_کن _NGX_HASH_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    فراغ             *مقدار;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


تعريفنوع بنية {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        اندازه;
} ngx_hash_t;


تعريفنوع بنية {
    ngx_hash_t        hash;
    فراغ             *مقدار;
} ngx_hash_wildcard_t;


تعريفنوع بنية {
    ngx_str_t         کلید;
    ngx_uint_t        کلید_hash;
    فراغ             *مقدار;
} ngx_hash_کلید_t;


تعريفنوع ngx_uint_t (*ngx_hash_کلید_pt) (u_char *داده, اندازه_t len);


تعريفنوع بنية {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_سر;
    ngx_hash_wildcard_t  *wc_دم;
} ngx_hash_combined_t;


تعريفنوع بنية {
    ngx_hash_t       *hash;
    ngx_hash_کلید_pt   کلید;

    ngx_uint_t        max_اندازه;
    ngx_uint_t        سطل_اندازه;

    حرف             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *موقت_pool;
} ngx_hash_init_t;


#تعریف_کن NGX_HASH_SMALL            1
#تعریف_کن NGX_HASH_LARGE            2

#تعریف_کن NGX_HASH_LARGE_ASIZE      16384
#تعریف_کن NGX_HASH_LARGE_HSIZE      10007

#تعریف_کن NGX_HASH_WILDCARD_کلید     1
#تعریف_کن NGX_HASH_READONLY_کلید     2


تعريفنوع بنية {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *موقت_pool;

    ngx_array_t       کلیدها;
    ngx_array_t      *کلیدها_hash;

    ngx_array_t       dns_wc_سر;
    ngx_array_t      *dns_wc_سر_hash;

    ngx_array_t       dns_wc_دم;
    ngx_array_t      *dns_wc_دم_hash;
} ngx_hash_کلیدها_arrays_t;


تعريفنوع بنية ngx_جدول_elt_s  ngx_جدول_elt_t;

بنية ngx_جدول_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         کلید;
    ngx_str_t         مقدار;
    u_char           *lowcase_کلید;
    ngx_جدول_elt_t  *بعدی;
};


فراغ *ngx_hash_پیدا_کن(ngx_hash_t *hash, ngx_uint_t کلید, u_char *name, اندازه_t len);
فراغ *ngx_hash_پیدا_کن_wc_سر(ngx_hash_wildcard_t *hwc, u_char *name, اندازه_t len);
فراغ *ngx_hash_پیدا_کن_wc_دم(ngx_hash_wildcard_t *hwc, u_char *name, اندازه_t len);
فراغ *ngx_hash_پیدا_کن_combined(ngx_hash_combined_t *hash, ngx_uint_t کلید,
    u_char *name, اندازه_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_کلید_t *نامها,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_کلید_t *نامها,
    ngx_uint_t nelts);

#تعریف_کن ngx_hash(کلید, c)   ((ngx_uint_t) کلید * 31 + c)
ngx_uint_t ngx_hash_کلید(u_char *داده, اندازه_t len);
ngx_uint_t ngx_hash_کلید_lc(u_char *داده, اندازه_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, اندازه_t n);


ngx_int_t ngx_hash_کلیدها_array_init(ngx_hash_کلیدها_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_جمع_کن_کلید(ngx_hash_کلیدها_arrays_t *ha, ngx_str_t *کلید,
    فراغ *مقدار, ngx_uint_t flags);


#پایان_اگر /* _NGX_HASH_H_INCLUDED_ */
