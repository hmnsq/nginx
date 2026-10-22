
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_HASH_H_INCLUDED_
#परिभाषय _NGX_HASH_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश संरचना {
    शून्य             *मानम्;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


प्रकारनिर्देश संरचना {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        आकार;
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


प्रकारनिर्देश ngx_uint_t (*ngx_hash_कुञ्चिका_pt) (u_char *दत्त, आकार_t len);


प्रकारनिर्देश संरचना {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_शीर्ष;
    ngx_hash_wildcard_t  *wc_पुच्छ;
} ngx_hash_combined_t;


प्रकारनिर्देश संरचना {
    ngx_hash_t       *hash;
    ngx_hash_कुञ्चिका_pt   कुञ्चिका;

    ngx_uint_t        max_आकार;
    ngx_uint_t        bucket_आकार;

    वर्ण             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *अस्थायी_pool;
} ngx_hash_init_t;


#परिभाषय NGX_HASH_SMALL            1
#परिभाषय NGX_HASH_LARGE            2

#परिभाषय NGX_HASH_LARGE_ASIZE      16384
#परिभाषय NGX_HASH_LARGE_HSIZE      10007

#परिभाषय NGX_HASH_WILDCARD_कुञ्चिका     1
#परिभाषय NGX_HASH_READONLY_कुञ्चिका     2


प्रकारनिर्देश संरचना {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *अस्थायी_pool;

    ngx_array_t       कुञ्चिकाः;
    ngx_array_t      *कुञ्चिकाः_hash;

    ngx_array_t       dns_wc_शीर्ष;
    ngx_array_t      *dns_wc_शीर्ष_hash;

    ngx_array_t       dns_wc_पुच्छ;
    ngx_array_t      *dns_wc_पुच्छ_hash;
} ngx_hash_कुञ्चिकाः_arrays_t;


प्रकारनिर्देश संरचना ngx_सारणी_elt_s  ngx_सारणी_elt_t;

संरचना ngx_सारणी_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         कुञ्चिका;
    ngx_str_t         मानम्;
    u_char           *lowcase_कुञ्चिका;
    ngx_सारणी_elt_t  *अग्र;
};


शून्य *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t कुञ्चिका, u_char *name, आकार_t len);
शून्य *ngx_hash_find_wc_शीर्ष(ngx_hash_wildcard_t *hwc, u_char *name, आकार_t len);
शून्य *ngx_hash_find_wc_पुच्छ(ngx_hash_wildcard_t *hwc, u_char *name, आकार_t len);
शून्य *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t कुञ्चिका,
    u_char *name, आकार_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_कुञ्चिका_t *नामानि,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_कुञ्चिका_t *नामानि,
    ngx_uint_t nelts);

#परिभाषय ngx_hash(कुञ्चिका, c)   ((ngx_uint_t) कुञ्चिका * 31 + c)
ngx_uint_t ngx_hash_कुञ्चिका(u_char *दत्त, आकार_t len);
ngx_uint_t ngx_hash_कुञ्चिका_lc(u_char *दत्त, आकार_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, आकार_t n);


ngx_int_t ngx_hash_कुञ्चिकाः_array_init(ngx_hash_कुञ्चिकाः_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_सङ्कलय_कुञ्चिका(ngx_hash_कुञ्चिकाः_arrays_t *ha, ngx_str_t *कुञ्चिका,
    शून्य *मानम्, ngx_uint_t flags);


#यदि_अन्तः /* _NGX_HASH_H_INCLUDED_ */
