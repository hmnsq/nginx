
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NGX_ਚੱਕਰੀ_ਜਾਂਚ_32_H_INCLUDED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ਚੱਕਰੀ_ਜਾਂਚ_32_H_INCLUDED


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਬਾਹਰੀ uint32_t  *ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_ਸਾਰਣੀ_short;
ਬਾਹਰੀ uint32_t   ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_table256[];
ਬਾਹਰੀ uint32_t   ngx_crc32c_table256[];


ਸਥਿਰ ngx_inline uint32_t
ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_short(u_char *p, ਮਾਪ_t len)
{
    u_char    c;
    uint32_t  ਚੱਕਰੀ_ਜਾਂਚ;

    ਚੱਕਰੀ_ਜਾਂਚ = 0xffffffff;

    ਜਦੋਂ (len--) {
        c = *p++;
        ਚੱਕਰੀ_ਜਾਂਚ = ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_ਸਾਰਣੀ_short[(ਚੱਕਰੀ_ਜਾਂਚ ^ (c & 0xf)) & 0xf] ^ (ਚੱਕਰੀ_ਜਾਂਚ >> 4);
        ਚੱਕਰੀ_ਜਾਂਚ = ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_ਸਾਰਣੀ_short[(ਚੱਕਰੀ_ਜਾਂਚ ^ (c >> 4)) & 0xf] ^ (ਚੱਕਰੀ_ਜਾਂਚ >> 4);
    }

    ਵਾਪਸ ਚੱਕਰੀ_ਜਾਂਚ ^ 0xffffffff;
}


ਸਥਿਰ ngx_inline uint32_t
ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_long(u_char *p, ਮਾਪ_t len)
{
    uint32_t  ਚੱਕਰੀ_ਜਾਂਚ;

    ਚੱਕਰੀ_ਜਾਂਚ = 0xffffffff;

    ਜਦੋਂ (len--) {
        ਚੱਕਰੀ_ਜਾਂਚ = ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_table256[(ਚੱਕਰੀ_ਜਾਂਚ ^ *p++) & 0xff] ^ (ਚੱਕਰੀ_ਜਾਂਚ >> 8);
    }

    ਵਾਪਸ ਚੱਕਰੀ_ਜਾਂਚ ^ 0xffffffff;
}


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_init(ਚੱਕਰੀ_ਜਾਂਚ)                                                   \
    ਚੱਕਰੀ_ਜਾਂਚ = 0xffffffff


ਸਥਿਰ ngx_inline ਖਾਲੀ
ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_update(uint32_t *ਚੱਕਰੀ_ਜਾਂਚ, u_char *p, ਮਾਪ_t len)
{
    uint32_t  c;

    c = *ਚੱਕਰੀ_ਜਾਂਚ;

    ਜਦੋਂ (len--) {
        c = ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *ਚੱਕਰੀ_ਜਾਂਚ = c;
}


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_final(ਚੱਕਰੀ_ਜਾਂਚ)                                                  \
    ਚੱਕਰੀ_ਜਾਂਚ ^= 0xffffffff


ਸਥਿਰ ngx_inline uint32_t
ngx_crc32c_long(u_char *p, ਮਾਪ_t len)
{
    uint32_t  ਚੱਕਰੀ_ਜਾਂਚ;

    ਚੱਕਰੀ_ਜਾਂਚ = 0xffffffff;

    ਜਦੋਂ (len--) {
        ਚੱਕਰੀ_ਜਾਂਚ = ngx_crc32c_table256[(ਚੱਕਰੀ_ਜਾਂਚ ^ *p++) & 0xff] ^ (ਚੱਕਰੀ_ਜਾਂਚ >> 8);
    }

    ਵਾਪਸ ਚੱਕਰੀ_ਜਾਂਚ ^ 0xffffffff;
}


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_crc32c_init(ਚੱਕਰੀ_ਜਾਂਚ)                                                  \
    ਚੱਕਰੀ_ਜਾਂਚ = 0xffffffff


ਸਥਿਰ ngx_inline ਖਾਲੀ
ngx_crc32c_update(uint32_t *ਚੱਕਰੀ_ਜਾਂਚ, u_char *p, ਮਾਪ_t len)
{
    uint32_t  c;

    c = *ਚੱਕਰੀ_ਜਾਂਚ;

    ਜਦੋਂ (len--) {
        c = ngx_crc32c_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *ਚੱਕਰੀ_ਜਾਂਚ = c;
}


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_crc32c_final(ਚੱਕਰੀ_ਜਾਂਚ)                                                 \
    ਚੱਕਰੀ_ਜਾਂਚ ^= 0xffffffff


ngx_int_t ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_ਸਾਰਣੀ_init(ਖਾਲੀ);


#ਜੇ_ਅੰਤ /* _NGX_CRC32_H_INCLUDED_ */
