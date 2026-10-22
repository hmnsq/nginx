
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_MD5_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_MD5_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    uint64_t  ਬਾਈਟਾਂ;
    uint32_t  a, b, c, d;
    u_char    ਬਫਰ[64];
} ngx_md5_t;


ਖਾਲੀ ngx_md5_init(ngx_md5_t *ctx);
ਖਾਲੀ ngx_md5_update(ngx_md5_t *ctx, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਮਾਪ_t ਮਾਪ);
ਖਾਲੀ ngx_md5_final(u_char ਨਤੀਜਾ[16], ngx_md5_t *ctx);


#ਜੇ_ਅੰਤ /* _NGX_MD5_H_INCLUDED_ */
