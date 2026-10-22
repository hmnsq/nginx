
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_HASH_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_HASH_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಶೂನ್ಯ             *ಮೌಲ್ಯ;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        ಗಾತ್ರ_2;
} ngx_hash_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_hash_t        hash;
    ಶೂನ್ಯ             *ಮೌಲ್ಯ;
} ngx_hash_wildcard_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_str_t         ಕೀಲಿ;
    ngx_uint_t        ಕೀಲಿ_hash;
    ಶೂನ್ಯ             *ಮೌಲ್ಯ;
} ngx_hash_ಕೀಲಿ_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ngx_uint_t (*ngx_hash_ಕೀಲಿ_pt) (u_char *ದತ್ತಾಂಶ, ಗಾತ್ರ_t len);


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_ತಲೆ;
    ngx_hash_wildcard_t  *wc_ಬಾಲ;
} ngx_hash_combined_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_hash_t       *hash;
    ngx_hash_ಕೀಲಿ_pt   ಕೀಲಿ;

    ngx_uint_t        max_ಗಾತ್ರ;
    ngx_uint_t        ಪಾತ್ರೆ_ಗಾತ್ರ;

    ಅಕ್ಷರ             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *ತಾತ್ಕಾಲಿಕ_pool;
} ngx_hash_init_t;


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_HASH_SMALL            1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_HASH_LARGE            2

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_HASH_LARGE_ASIZE      16384
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_HASH_LARGE_HSIZE      10007

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_HASH_WILDCARD_ಕೀಲಿ     1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_HASH_READONLY_ಕೀಲಿ     2


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *ತಾತ್ಕಾಲಿಕ_pool;

    ngx_array_t       ಕೀಲಿಗಳು;
    ngx_array_t      *ಕೀಲಿಗಳು_hash;

    ngx_array_t       dns_wc_ತಲೆ;
    ngx_array_t      *dns_wc_ತಲೆ_hash;

    ngx_array_t       dns_wc_ಬಾಲ;
    ngx_array_t      *dns_wc_ಬಾಲ_hash;
} ngx_hash_ಕೀಲಿಗಳು_arrays_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_ಕೋಷ್ಟಕ_elt_s  ngx_ಕೋಷ್ಟಕ_elt_t;

ರಚನೆ ngx_ಕೋಷ್ಟಕ_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         ಕೀಲಿ;
    ngx_str_t         ಮೌಲ್ಯ;
    u_char           *lowcase_ಕೀಲಿ;
    ngx_ಕೋಷ್ಟಕ_elt_t  *ಮುಂದಿನ;
};


ಶೂನ್ಯ *ngx_hash_ಕಂಡುಹಿಡಿ(ngx_hash_t *hash, ngx_uint_t ಕೀಲಿ, u_char *name, ಗಾತ್ರ_t len);
ಶೂನ್ಯ *ngx_hash_ಕಂಡುಹಿಡಿ_wc_ತಲೆ(ngx_hash_wildcard_t *hwc, u_char *name, ಗಾತ್ರ_t len);
ಶೂನ್ಯ *ngx_hash_ಕಂಡುಹಿಡಿ_wc_ಬಾಲ(ngx_hash_wildcard_t *hwc, u_char *name, ಗಾತ್ರ_t len);
ಶೂನ್ಯ *ngx_hash_ಕಂಡುಹಿಡಿ_combined(ngx_hash_combined_t *hash, ngx_uint_t ಕೀಲಿ,
    u_char *name, ಗಾತ್ರ_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_ಕೀಲಿ_t *ಹೆಸರುಗಳು,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_ಕೀಲಿ_t *ಹೆಸರುಗಳು,
    ngx_uint_t nelts);

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_hash(ಕೀಲಿ, c)   ((ngx_uint_t) ಕೀಲಿ * 31 + c)
ngx_uint_t ngx_hash_ಕೀಲಿ(u_char *ದತ್ತಾಂಶ, ಗಾತ್ರ_t len);
ngx_uint_t ngx_hash_ಕೀಲಿ_lc(u_char *ದತ್ತಾಂಶ, ಗಾತ್ರ_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, ಗಾತ್ರ_t n);


ngx_int_t ngx_hash_ಕೀಲಿಗಳು_array_init(ngx_hash_ಕೀಲಿಗಳು_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_ಕೂಡಿಸು_ಕೀಲಿ(ngx_hash_ಕೀಲಿಗಳು_arrays_t *ha, ngx_str_t *ಕೀಲಿ,
    ಶೂನ್ಯ *ಮೌಲ್ಯ, ngx_uint_t flags);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_HASH_H_INCLUDED_ */
