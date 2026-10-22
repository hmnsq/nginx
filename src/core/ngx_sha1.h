
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_SHA1_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_SHA1_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    uint64_t  ಬೈಟುಗಳು;
    uint32_t  a, b, c, d, e, f;
    u_char    ಬಫರ್[64];
} ngx_sha1_t;


ಶೂನ್ಯ ngx_sha1_init(ngx_sha1_t *ctx);
ಶೂನ್ಯ ngx_sha1_update(ngx_sha1_t *ctx, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಶೂನ್ಯ ngx_sha1_final(u_char ಫಲಿತಾಂಶ[20], ngx_sha1_t *ctx);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_SHA1_H_INCLUDED_ */
