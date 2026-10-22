
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_SHA1_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_SHA1_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    uint64_t  ਬਾਈਟਾਂ;
    uint32_t  a, b, c, d, e, f;
    u_char    ਬਫਰ[64];
} ngx_sha1_t;


ਖਾਲੀ ngx_sha1_init(ngx_sha1_t *ctx);
ਖਾਲੀ ngx_sha1_update(ngx_sha1_t *ctx, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਮਾਪ_t ਮਾਪ);
ਖਾਲੀ ngx_sha1_final(u_char ਨਤੀਜਾ[20], ngx_sha1_t *ctx);


#ਜੇ_ਅੰਤ /* _NGX_SHA1_H_INCLUDED_ */
