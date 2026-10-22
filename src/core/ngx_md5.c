
/*
 * An internal implementation, based on Alexander Peslyak's
 * public domain implementation:
 * http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_md5.h>


ਸਥਿਰ ਅਚਲ u_char *ngx_md5_body(ngx_md5_t *ctx, ਅਚਲ u_char *ਅੰਕੜੇ,
    ਮਾਪ_t ਮਾਪ);


ਖਾਲੀ
ngx_md5_init(ngx_md5_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;

    ctx->ਬਾਈਟਾਂ = 0;
}


ਖਾਲੀ
ngx_md5_update(ngx_md5_t *ctx, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਮਾਪ_t ਮਾਪ)
{
    ਮਾਪ_t  ਵਰਤਿਆ, ਮੁਕਤ;

    ਵਰਤਿਆ = (ਮਾਪ_t) (ctx->ਬਾਈਟਾਂ & 0x3f);
    ctx->ਬਾਈਟਾਂ += ਮਾਪ;

    ਜੇ (ਵਰਤਿਆ) {
        ਮੁਕਤ = 64 - ਵਰਤਿਆ;

        ਜੇ (ਮਾਪ < ਮੁਕਤ) {
            ngx_memcpy(&ctx->ਬਫਰ[ਵਰਤਿਆ], ਅੰਕੜੇ, ਮਾਪ);
            ਵਾਪਸ;
        }

        ngx_memcpy(&ctx->ਬਫਰ[ਵਰਤਿਆ], ਅੰਕੜੇ, ਮੁਕਤ);
        ਅੰਕੜੇ = (u_char *) ਅੰਕੜੇ + ਮੁਕਤ;
        ਮਾਪ -= ਮੁਕਤ;
        (ਖਾਲੀ) ngx_md5_body(ctx, ctx->ਬਫਰ, 64);
    }

    ਜੇ (ਮਾਪ >= 64) {
        ਅੰਕੜੇ = ngx_md5_body(ctx, ਅੰਕੜੇ, ਮਾਪ & ~(ਮਾਪ_t) 0x3f);
        ਮਾਪ &= 0x3f;
    }

    ngx_memcpy(ctx->ਬਫਰ, ਅੰਕੜੇ, ਮਾਪ);
}


ਖਾਲੀ
ngx_md5_final(u_char ਨਤੀਜਾ[16], ngx_md5_t *ctx)
{
    ਮਾਪ_t  ਵਰਤਿਆ, ਮੁਕਤ;

    ਵਰਤਿਆ = (ਮਾਪ_t) (ctx->ਬਾਈਟਾਂ & 0x3f);

    ctx->ਬਫਰ[ਵਰਤਿਆ++] = 0x80;

    ਮੁਕਤ = 64 - ਵਰਤਿਆ;

    ਜੇ (ਮੁਕਤ < 8) {
        ngx_memzero(&ctx->ਬਫਰ[ਵਰਤਿਆ], ਮੁਕਤ);
        (ਖਾਲੀ) ngx_md5_body(ctx, ctx->ਬਫਰ, 64);
        ਵਰਤਿਆ = 0;
        ਮੁਕਤ = 64;
    }

    ngx_memzero(&ctx->ਬਫਰ[ਵਰਤਿਆ], ਮੁਕਤ - 8);

    ctx->ਬਾਈਟਾਂ <<= 3;
    ctx->ਬਫਰ[56] = (u_char) ctx->ਬਾਈਟਾਂ;
    ctx->ਬਫਰ[57] = (u_char) (ctx->ਬਾਈਟਾਂ >> 8);
    ctx->ਬਫਰ[58] = (u_char) (ctx->ਬਾਈਟਾਂ >> 16);
    ctx->ਬਫਰ[59] = (u_char) (ctx->ਬਾਈਟਾਂ >> 24);
    ctx->ਬਫਰ[60] = (u_char) (ctx->ਬਾਈਟਾਂ >> 32);
    ctx->ਬਫਰ[61] = (u_char) (ctx->ਬਾਈਟਾਂ >> 40);
    ctx->ਬਫਰ[62] = (u_char) (ctx->ਬਾਈਟਾਂ >> 48);
    ctx->ਬਫਰ[63] = (u_char) (ctx->ਬਾਈਟਾਂ >> 56);

    (ਖਾਲੀ) ngx_md5_body(ctx, ctx->ਬਫਰ, 64);

    ਨਤੀਜਾ[0] = (u_char) ctx->a;
    ਨਤੀਜਾ[1] = (u_char) (ctx->a >> 8);
    ਨਤੀਜਾ[2] = (u_char) (ctx->a >> 16);
    ਨਤੀਜਾ[3] = (u_char) (ctx->a >> 24);
    ਨਤੀਜਾ[4] = (u_char) ctx->b;
    ਨਤੀਜਾ[5] = (u_char) (ctx->b >> 8);
    ਨਤੀਜਾ[6] = (u_char) (ctx->b >> 16);
    ਨਤੀਜਾ[7] = (u_char) (ctx->b >> 24);
    ਨਤੀਜਾ[8] = (u_char) ctx->c;
    ਨਤੀਜਾ[9] = (u_char) (ctx->c >> 8);
    ਨਤੀਜਾ[10] = (u_char) (ctx->c >> 16);
    ਨਤੀਜਾ[11] = (u_char) (ctx->c >> 24);
    ਨਤੀਜਾ[12] = (u_char) ctx->d;
    ਨਤੀਜਾ[13] = (u_char) (ctx->d >> 8);
    ਨਤੀਜਾ[14] = (u_char) (ctx->d >> 16);
    ਨਤੀਜਾ[15] = (u_char) (ctx->d >> 24);

    ngx_memzero(ctx, ਆਕਾਰ(*ctx));
}


/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in
 * Colin Plumb's implementation.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ F(x, y, z)  ((z) ^ ((x) & ((y) ^ (z))))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ G(x, y, z)  ((y) ^ ((z) & ((x) ^ (y))))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ H(x, y, z)  ((x) ^ (y) ^ (z))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ I(x, y, z)  ((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਦਮ(f, a, b, c, d, x, t, s)                                          \
    (a) += f((b), (c), (d)) + (x) + (t);                                      \
    (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s))));                \
    (a) += (b)

/*
 * SET() reads 4 input bytes in little-endian byte order and stores them
 * in a properly aligned word in host byte order.
 *
 * The check for little-endian architectures that tolerate unaligned
 * memory accesses is just an optimization.  Nothing will break if it
 * does not work.
 */

#ਜੇ (NGX_HAVE_LITTLE_ENDIAN && NGX_HAVE_NONALIGNED)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੈੱਟ_2(n)      (*(uint32_t *) &p[n * 4])
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET(n)      (*(uint32_t *) &p[n * 4])

#ਨਹੀਂਤਾਂ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੈੱਟ_2(n)                                                                \
    (ਖੰਡ[n] =                                                               \
    (uint32_t) p[n * 4] |                                                     \
    ((uint32_t) p[n * 4 + 1] << 8) |                                          \
    ((uint32_t) p[n * 4 + 2] << 16) |                                         \
    ((uint32_t) p[n * 4 + 3] << 24))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET(n)      ਖੰਡ[n]

#ਜੇ_ਅੰਤ


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

ਸਥਿਰ ਅਚਲ u_char *
ngx_md5_body(ngx_md5_t *ctx, ਅਚਲ u_char *ਅੰਕੜੇ, ਮਾਪ_t ਮਾਪ)
{
    uint32_t       a, b, c, d;
    uint32_t       saved_a, saved_b, saved_c, saved_d;
    ਅਚਲ u_char  *p;
#ਜੇ !(NGX_HAVE_LITTLE_ENDIAN && NGX_HAVE_NONALIGNED)
    uint32_t       ਖੰਡ[16];
#ਜੇ_ਅੰਤ

    p = ਅੰਕੜੇ;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;

    ਕਰੋ {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;

        /* Round 1 */

        ਕਦਮ(F, a, b, c, d, ਸੈੱਟ_2(0),  0xd76aa478, 7);
        ਕਦਮ(F, d, a, b, c, ਸੈੱਟ_2(1),  0xe8c7b756, 12);
        ਕਦਮ(F, c, d, a, b, ਸੈੱਟ_2(2),  0x242070db, 17);
        ਕਦਮ(F, b, c, d, a, ਸੈੱਟ_2(3),  0xc1bdceee, 22);
        ਕਦਮ(F, a, b, c, d, ਸੈੱਟ_2(4),  0xf57c0faf, 7);
        ਕਦਮ(F, d, a, b, c, ਸੈੱਟ_2(5),  0x4787c62a, 12);
        ਕਦਮ(F, c, d, a, b, ਸੈੱਟ_2(6),  0xa8304613, 17);
        ਕਦਮ(F, b, c, d, a, ਸੈੱਟ_2(7),  0xfd469501, 22);
        ਕਦਮ(F, a, b, c, d, ਸੈੱਟ_2(8),  0x698098d8, 7);
        ਕਦਮ(F, d, a, b, c, ਸੈੱਟ_2(9),  0x8b44f7af, 12);
        ਕਦਮ(F, c, d, a, b, ਸੈੱਟ_2(10), 0xffff5bb1, 17);
        ਕਦਮ(F, b, c, d, a, ਸੈੱਟ_2(11), 0x895cd7be, 22);
        ਕਦਮ(F, a, b, c, d, ਸੈੱਟ_2(12), 0x6b901122, 7);
        ਕਦਮ(F, d, a, b, c, ਸੈੱਟ_2(13), 0xfd987193, 12);
        ਕਦਮ(F, c, d, a, b, ਸੈੱਟ_2(14), 0xa679438e, 17);
        ਕਦਮ(F, b, c, d, a, ਸੈੱਟ_2(15), 0x49b40821, 22);

        /* Round 2 */

        ਕਦਮ(G, a, b, c, d, GET(1),  0xf61e2562, 5);
        ਕਦਮ(G, d, a, b, c, GET(6),  0xc040b340, 9);
        ਕਦਮ(G, c, d, a, b, GET(11), 0x265e5a51, 14);
        ਕਦਮ(G, b, c, d, a, GET(0),  0xe9b6c7aa, 20);
        ਕਦਮ(G, a, b, c, d, GET(5),  0xd62f105d, 5);
        ਕਦਮ(G, d, a, b, c, GET(10), 0x02441453, 9);
        ਕਦਮ(G, c, d, a, b, GET(15), 0xd8a1e681, 14);
        ਕਦਮ(G, b, c, d, a, GET(4),  0xe7d3fbc8, 20);
        ਕਦਮ(G, a, b, c, d, GET(9),  0x21e1cde6, 5);
        ਕਦਮ(G, d, a, b, c, GET(14), 0xc33707d6, 9);
        ਕਦਮ(G, c, d, a, b, GET(3),  0xf4d50d87, 14);
        ਕਦਮ(G, b, c, d, a, GET(8),  0x455a14ed, 20);
        ਕਦਮ(G, a, b, c, d, GET(13), 0xa9e3e905, 5);
        ਕਦਮ(G, d, a, b, c, GET(2),  0xfcefa3f8, 9);
        ਕਦਮ(G, c, d, a, b, GET(7),  0x676f02d9, 14);
        ਕਦਮ(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20);

        /* Round 3 */

        ਕਦਮ(H, a, b, c, d, GET(5),  0xfffa3942, 4);
        ਕਦਮ(H, d, a, b, c, GET(8),  0x8771f681, 11);
        ਕਦਮ(H, c, d, a, b, GET(11), 0x6d9d6122, 16);
        ਕਦਮ(H, b, c, d, a, GET(14), 0xfde5380c, 23);
        ਕਦਮ(H, a, b, c, d, GET(1),  0xa4beea44, 4);
        ਕਦਮ(H, d, a, b, c, GET(4),  0x4bdecfa9, 11);
        ਕਦਮ(H, c, d, a, b, GET(7),  0xf6bb4b60, 16);
        ਕਦਮ(H, b, c, d, a, GET(10), 0xbebfbc70, 23);
        ਕਦਮ(H, a, b, c, d, GET(13), 0x289b7ec6, 4);
        ਕਦਮ(H, d, a, b, c, GET(0),  0xeaa127fa, 11);
        ਕਦਮ(H, c, d, a, b, GET(3),  0xd4ef3085, 16);
        ਕਦਮ(H, b, c, d, a, GET(6),  0x04881d05, 23);
        ਕਦਮ(H, a, b, c, d, GET(9),  0xd9d4d039, 4);
        ਕਦਮ(H, d, a, b, c, GET(12), 0xe6db99e5, 11);
        ਕਦਮ(H, c, d, a, b, GET(15), 0x1fa27cf8, 16);
        ਕਦਮ(H, b, c, d, a, GET(2),  0xc4ac5665, 23);

        /* Round 4 */

        ਕਦਮ(I, a, b, c, d, GET(0),  0xf4292244, 6);
        ਕਦਮ(I, d, a, b, c, GET(7),  0x432aff97, 10);
        ਕਦਮ(I, c, d, a, b, GET(14), 0xab9423a7, 15);
        ਕਦਮ(I, b, c, d, a, GET(5),  0xfc93a039, 21);
        ਕਦਮ(I, a, b, c, d, GET(12), 0x655b59c3, 6);
        ਕਦਮ(I, d, a, b, c, GET(3),  0x8f0ccc92, 10);
        ਕਦਮ(I, c, d, a, b, GET(10), 0xffeff47d, 15);
        ਕਦਮ(I, b, c, d, a, GET(1),  0x85845dd1, 21);
        ਕਦਮ(I, a, b, c, d, GET(8),  0x6fa87e4f, 6);
        ਕਦਮ(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10);
        ਕਦਮ(I, c, d, a, b, GET(6),  0xa3014314, 15);
        ਕਦਮ(I, b, c, d, a, GET(13), 0x4e0811a1, 21);
        ਕਦਮ(I, a, b, c, d, GET(4),  0xf7537e82, 6);
        ਕਦਮ(I, d, a, b, c, GET(11), 0xbd3af235, 10);
        ਕਦਮ(I, c, d, a, b, GET(2),  0x2ad7d2bb, 15);
        ਕਦਮ(I, b, c, d, a, GET(9),  0xeb86d391, 21);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;

        p += 64;

    } ਜਦੋਂ (ਮਾਪ -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;

    ਵਾਪਸ p;
}
