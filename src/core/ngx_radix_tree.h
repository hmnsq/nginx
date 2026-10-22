
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NGX_RADIX_ವೃಕ್ಷ_H_INCLUDED
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RADIX_ವೃಕ್ಷ_H_INCLUDED


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RADIX_NO_ಮೌಲ್ಯ   (uintptr_t) -1

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_radix_ಗಂಟು_s  ngx_radix_ಗಂಟು_t;

ರಚನೆ ngx_radix_ಗಂಟು_s {
    ngx_radix_ಗಂಟು_t  *ಬಲ;
    ngx_radix_ಗಂಟು_t  *ಎಡ;
    ngx_radix_ಗಂಟು_t  *ಪೋಷಕ;
    uintptr_t          ಮೌಲ್ಯ;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_radix_ಗಂಟು_t  *ಬೇರು;
    ngx_pool_t        *pool;
    ngx_radix_ಗಂಟು_t  *ಬಿಡುಗಡೆ;
    ಅಕ್ಷರ              *ಆರಂಭ;
    ಗಾತ್ರ_t             ಗಾತ್ರ_2;
} ngx_radix_ವೃಕ್ಷ_t;


ngx_radix_ವೃಕ್ಷ_t *ngx_radix_ವೃಕ್ಷ_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_ಸೇರಿಸು(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ,
    uint32_t ಕೀಲಿ, uint32_t ಮುಖವಾಡ, uintptr_t ಮೌಲ್ಯ);
ngx_int_t ngx_radix32tree_delete(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ,
    uint32_t ಕೀಲಿ, uint32_t ಮುಖವಾಡ);
uintptr_t ngx_radix32tree_ಕಂಡುಹಿಡಿ(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ, uint32_t ಕೀಲಿ);

#ಆದರೆ (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_ಸೇರಿಸು(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ,
    u_char *ಕೀಲಿ, u_char *ಮುಖವಾಡ, uintptr_t ಮೌಲ್ಯ);
ngx_int_t ngx_radix128tree_delete(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ,
    u_char *ಕೀಲಿ, u_char *ಮುಖವಾಡ);
uintptr_t ngx_radix128tree_ಕಂಡುಹಿಡಿ(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ, u_char *ಕೀಲಿ);
#ಆದರೆ_ಅಂತ್ಯ


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_RADIX_TREE_H_INCLUDED_ */
