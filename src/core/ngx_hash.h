
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_HASH_H_INCLUDED_
#وصف_ڪريو _NGX_HASH_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    فراغ             *قدر;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


تعريفنوع بنية {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        سائيز;
} ngx_hash_t;


تعريفنوع بنية {
    ngx_hash_t        hash;
    فراغ             *قدر;
} ngx_hash_wildcard_t;


تعريفنوع بنية {
    ngx_str_t         ڪنجي;
    ngx_uint_t        ڪنجي_hash;
    فراغ             *قدر;
} ngx_hash_ڪنجي_t;


تعريفنوع ngx_uint_t (*ngx_hash_ڪنجي_pt) (u_char *معلومات, سائيز_t len);


تعريفنوع بنية {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_سر;
    ngx_hash_wildcard_t  *wc_پڇ;
} ngx_hash_combined_t;


تعريفنوع بنية {
    ngx_hash_t       *hash;
    ngx_hash_ڪنجي_pt   ڪنجي;

    ngx_uint_t        max_سائيز;
    ngx_uint_t        ٻالٽي_سائيز;

    حرف             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *عارضي_pool;
} ngx_hash_init_t;


#وصف_ڪريو NGX_HASH_SMALL            1
#وصف_ڪريو NGX_HASH_LARGE            2

#وصف_ڪريو NGX_HASH_LARGE_ASIZE      16384
#وصف_ڪريو NGX_HASH_LARGE_HSIZE      10007

#وصف_ڪريو NGX_HASH_WILDCARD_ڪنجي     1
#وصف_ڪريو NGX_HASH_READONLY_ڪنجي     2


تعريفنوع بنية {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *عارضي_pool;

    ngx_array_t       ڪنجيون;
    ngx_array_t      *ڪنجيون_hash;

    ngx_array_t       dns_wc_سر;
    ngx_array_t      *dns_wc_سر_hash;

    ngx_array_t       dns_wc_پڇ;
    ngx_array_t      *dns_wc_پڇ_hash;
} ngx_hash_ڪنجيون_arrays_t;


تعريفنوع بنية ngx_جدول_elt_s  ngx_جدول_elt_t;

بنية ngx_جدول_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         ڪنجي;
    ngx_str_t         قدر;
    u_char           *lowcase_ڪنجي;
    ngx_جدول_elt_t  *ايندڙ;
};


فراغ *ngx_hash_ڳوليو(ngx_hash_t *hash, ngx_uint_t ڪنجي, u_char *name, سائيز_t len);
فراغ *ngx_hash_ڳوليو_wc_سر(ngx_hash_wildcard_t *hwc, u_char *name, سائيز_t len);
فراغ *ngx_hash_ڳوليو_wc_پڇ(ngx_hash_wildcard_t *hwc, u_char *name, سائيز_t len);
فراغ *ngx_hash_ڳوليو_combined(ngx_hash_combined_t *hash, ngx_uint_t ڪنجي,
    u_char *name, سائيز_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_ڪنجي_t *نالا,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_ڪنجي_t *نالا,
    ngx_uint_t nelts);

#وصف_ڪريو ngx_hash(ڪنجي, c)   ((ngx_uint_t) ڪنجي * 31 + c)
ngx_uint_t ngx_hash_ڪنجي(u_char *معلومات, سائيز_t len);
ngx_uint_t ngx_hash_ڪنجي_lc(u_char *معلومات, سائيز_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, سائيز_t n);


ngx_int_t ngx_hash_ڪنجيون_array_init(ngx_hash_ڪنجيون_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_جوڙيو_ڪنجي(ngx_hash_ڪنجيون_arrays_t *ha, ngx_str_t *ڪنجي,
    فراغ *قدر, ngx_uint_t flags);


#جيڪڏهن_پورو /* _NGX_HASH_H_INCLUDED_ */
