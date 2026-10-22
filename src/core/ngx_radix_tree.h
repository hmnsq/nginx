
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_ವೃಕ್ಷ_H_INCLUDED
#define NGX_RADIX_ವೃಕ್ಷ_H_INCLUDED


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#define NGX_RADIX_NO_ಮೌಲ್ಯ   (uintptr_t) -1

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_radix_ಗಂಟು_s  ngx_radix_ಗಂಟು_t;

ಸಂರಚನಾ ngx_radix_ಗಂಟು_s {
    ngx_radix_ಗಂಟು_t  *ಬಲ;
    ngx_radix_ಗಂಟು_t  *ಎಡ;
    ngx_radix_ಗಂಟು_t  *parent;
    uintptr_t          ಮೌಲ್ಯ;
};


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_radix_ಗಂಟು_t  *ಬೇರು;
    ngx_pool_t        *pool;
    ngx_radix_ಗಂಟು_t  *ಮುಕ್ತ;
    ವರ್ಣ              *start;
    size_t             size;
} ngx_radix_ವೃಕ್ಷ_t;


ngx_radix_ವೃಕ್ಷ_t *ngx_radix_ವೃಕ್ಷ_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_ಸೇರಿಸು(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ,
    uint32_t ಕೀಲಿ, uint32_t mask, uintptr_t ಮೌಲ್ಯ);
ngx_int_t ngx_radix32tree_delete(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ,
    uint32_t ಕೀಲಿ, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ, uint32_t ಕೀಲಿ);

#ಯದಿ (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_ಸೇರಿಸು(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ,
    u_char *ಕೀಲಿ, u_char *mask, uintptr_t ಮೌಲ್ಯ);
ngx_int_t ngx_radix128tree_delete(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ,
    u_char *ಕೀಲಿ, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ, u_char *ಕೀಲಿ);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
