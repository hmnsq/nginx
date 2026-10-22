
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_HASH_H_INCLUDED_
#تعریف_کړه _NGX_HASH_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    فراغ             *ارزښت;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


تعريفنوع بنية {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        اندازه;
} ngx_hash_t;


تعريفنوع بنية {
    ngx_hash_t        hash;
    فراغ             *ارزښت;
} ngx_hash_wildcard_t;


تعريفنوع بنية {
    ngx_str_t         کلی;
    ngx_uint_t        کلی_hash;
    فراغ             *ارزښت;
} ngx_hash_کلی_t;


تعريفنوع ngx_uint_t (*ngx_hash_کلی_pt) (u_char *معلومات, اندازه_t len);


تعريفنوع بنية {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_سر;
    ngx_hash_wildcard_t  *wc_لکۍ;
} ngx_hash_combined_t;


تعريفنوع بنية {
    ngx_hash_t       *hash;
    ngx_hash_کلی_pt   کلی;

    ngx_uint_t        max_اندازه;
    ngx_uint_t        لوښی_اندازه;

    حرف             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *لنډمهاله_pool;
} ngx_hash_init_t;


#تعریف_کړه NGX_HASH_SMALL            1
#تعریف_کړه NGX_HASH_LARGE            2

#تعریف_کړه NGX_HASH_LARGE_ASIZE      16384
#تعریف_کړه NGX_HASH_LARGE_HSIZE      10007

#تعریف_کړه NGX_HASH_WILDCARD_کلی     1
#تعریف_کړه NGX_HASH_READONLY_کلی     2


تعريفنوع بنية {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *لنډمهاله_pool;

    ngx_array_t       کلیګانې;
    ngx_array_t      *کلیګانې_hash;

    ngx_array_t       dns_wc_سر;
    ngx_array_t      *dns_wc_سر_hash;

    ngx_array_t       dns_wc_لکۍ;
    ngx_array_t      *dns_wc_لکۍ_hash;
} ngx_hash_کلیګانې_arrays_t;


تعريفنوع بنية ngx_جدول_elt_s  ngx_جدول_elt_t;

بنية ngx_جدول_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         کلی;
    ngx_str_t         ارزښت;
    u_char           *lowcase_کلی;
    ngx_جدول_elt_t  *بل;
};


فراغ *ngx_hash_پیدا_کړه(ngx_hash_t *hash, ngx_uint_t کلی, u_char *name, اندازه_t len);
فراغ *ngx_hash_پیدا_کړه_wc_سر(ngx_hash_wildcard_t *hwc, u_char *name, اندازه_t len);
فراغ *ngx_hash_پیدا_کړه_wc_لکۍ(ngx_hash_wildcard_t *hwc, u_char *name, اندازه_t len);
فراغ *ngx_hash_پیدا_کړه_combined(ngx_hash_combined_t *hash, ngx_uint_t کلی,
    u_char *name, اندازه_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_کلی_t *نومونه,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_کلی_t *نومونه,
    ngx_uint_t nelts);

#تعریف_کړه ngx_hash(کلی, c)   ((ngx_uint_t) کلی * 31 + c)
ngx_uint_t ngx_hash_کلی(u_char *معلومات, اندازه_t len);
ngx_uint_t ngx_hash_کلی_lc(u_char *معلومات, اندازه_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, اندازه_t n);


ngx_int_t ngx_hash_کلیګانې_array_init(ngx_hash_کلیګانې_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_جمع_کړه_کلی(ngx_hash_کلیګانې_arrays_t *ha, ngx_str_t *کلی,
    فراغ *ارزښت, ngx_uint_t flags);


#د_که_پای /* _NGX_HASH_H_INCLUDED_ */
