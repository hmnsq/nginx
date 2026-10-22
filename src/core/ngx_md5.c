
/*
 * An internal implementation, based on Alexander Peslyak's
 * public domain implementation:
 * http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_md5.h>


ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ u_char *ngx_md5_body(ngx_md5_t *ctx, ಸ್ಥಿರಾಂಕ u_char *ದತ್ತಾಂಶ,
    ಗಾತ್ರ_t ಗಾತ್ರ_2);


ಶೂನ್ಯ
ngx_md5_init(ngx_md5_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;

    ctx->ಬೈಟುಗಳು = 0;
}


ಶೂನ್ಯ
ngx_md5_update(ngx_md5_t *ctx, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಗಾತ್ರ_t  ಬಳಸಿದ, ಬಿಡುಗಡೆ;

    ಬಳಸಿದ = (ಗಾತ್ರ_t) (ctx->ಬೈಟುಗಳು & 0x3f);
    ctx->ಬೈಟುಗಳು += ಗಾತ್ರ_2;

    ಆದರೆ (ಬಳಸಿದ) {
        ಬಿಡುಗಡೆ = 64 - ಬಳಸಿದ;

        ಆದರೆ (ಗಾತ್ರ_2 < ಬಿಡುಗಡೆ) {
            ngx_memcpy(&ctx->ಬಫರ್[ಬಳಸಿದ], ದತ್ತಾಂಶ, ಗಾತ್ರ_2);
            ಹಿಂತಿರುಗಿ;
        }

        ngx_memcpy(&ctx->ಬಫರ್[ಬಳಸಿದ], ದತ್ತಾಂಶ, ಬಿಡುಗಡೆ);
        ದತ್ತಾಂಶ = (u_char *) ದತ್ತಾಂಶ + ಬಿಡುಗಡೆ;
        ಗಾತ್ರ_2 -= ಬಿಡುಗಡೆ;
        (ಶೂನ್ಯ) ngx_md5_body(ctx, ctx->ಬಫರ್, 64);
    }

    ಆದರೆ (ಗಾತ್ರ_2 >= 64) {
        ದತ್ತಾಂಶ = ngx_md5_body(ctx, ದತ್ತಾಂಶ, ಗಾತ್ರ_2 & ~(ಗಾತ್ರ_t) 0x3f);
        ಗಾತ್ರ_2 &= 0x3f;
    }

    ngx_memcpy(ctx->ಬಫರ್, ದತ್ತಾಂಶ, ಗಾತ್ರ_2);
}


ಶೂನ್ಯ
ngx_md5_final(u_char ಫಲಿತಾಂಶ[16], ngx_md5_t *ctx)
{
    ಗಾತ್ರ_t  ಬಳಸಿದ, ಬಿಡುಗಡೆ;

    ಬಳಸಿದ = (ಗಾತ್ರ_t) (ctx->ಬೈಟುಗಳು & 0x3f);

    ctx->ಬಫರ್[ಬಳಸಿದ++] = 0x80;

    ಬಿಡುಗಡೆ = 64 - ಬಳಸಿದ;

    ಆದರೆ (ಬಿಡುಗಡೆ < 8) {
        ngx_memzero(&ctx->ಬಫರ್[ಬಳಸಿದ], ಬಿಡುಗಡೆ);
        (ಶೂನ್ಯ) ngx_md5_body(ctx, ctx->ಬಫರ್, 64);
        ಬಳಸಿದ = 0;
        ಬಿಡುಗಡೆ = 64;
    }

    ngx_memzero(&ctx->ಬಫರ್[ಬಳಸಿದ], ಬಿಡುಗಡೆ - 8);

    ctx->ಬೈಟುಗಳು <<= 3;
    ctx->ಬಫರ್[56] = (u_char) ctx->ಬೈಟುಗಳು;
    ctx->ಬಫರ್[57] = (u_char) (ctx->ಬೈಟುಗಳು >> 8);
    ctx->ಬಫರ್[58] = (u_char) (ctx->ಬೈಟುಗಳು >> 16);
    ctx->ಬಫರ್[59] = (u_char) (ctx->ಬೈಟುಗಳು >> 24);
    ctx->ಬಫರ್[60] = (u_char) (ctx->ಬೈಟುಗಳು >> 32);
    ctx->ಬಫರ್[61] = (u_char) (ctx->ಬೈಟುಗಳು >> 40);
    ctx->ಬಫರ್[62] = (u_char) (ctx->ಬೈಟುಗಳು >> 48);
    ctx->ಬಫರ್[63] = (u_char) (ctx->ಬೈಟುಗಳು >> 56);

    (ಶೂನ್ಯ) ngx_md5_body(ctx, ctx->ಬಫರ್, 64);

    ಫಲಿತಾಂಶ[0] = (u_char) ctx->a;
    ಫಲಿತಾಂಶ[1] = (u_char) (ctx->a >> 8);
    ಫಲಿತಾಂಶ[2] = (u_char) (ctx->a >> 16);
    ಫಲಿತಾಂಶ[3] = (u_char) (ctx->a >> 24);
    ಫಲಿತಾಂಶ[4] = (u_char) ctx->b;
    ಫಲಿತಾಂಶ[5] = (u_char) (ctx->b >> 8);
    ಫಲಿತಾಂಶ[6] = (u_char) (ctx->b >> 16);
    ಫಲಿತಾಂಶ[7] = (u_char) (ctx->b >> 24);
    ಫಲಿತಾಂಶ[8] = (u_char) ctx->c;
    ಫಲಿತಾಂಶ[9] = (u_char) (ctx->c >> 8);
    ಫಲಿತಾಂಶ[10] = (u_char) (ctx->c >> 16);
    ಫಲಿತಾಂಶ[11] = (u_char) (ctx->c >> 24);
    ಫಲಿತಾಂಶ[12] = (u_char) ctx->d;
    ಫಲಿತಾಂಶ[13] = (u_char) (ctx->d >> 8);
    ಫಲಿತಾಂಶ[14] = (u_char) (ctx->d >> 16);
    ಫಲಿತಾಂಶ[15] = (u_char) (ctx->d >> 24);

    ngx_memzero(ctx, ಗಾತ್ರ(*ctx));
}


/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in
 * Colin Plumb's implementation.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು F(x, y, z)  ((z) ^ ((x) & ((y) ^ (z))))
#ವ್ಯಾಖ್ಯಾನಿಸು G(x, y, z)  ((y) ^ ((z) & ((x) ^ (y))))
#ವ್ಯಾಖ್ಯಾನಿಸು H(x, y, z)  ((x) ^ (y) ^ (z))
#ವ್ಯಾಖ್ಯಾನಿಸು I(x, y, z)  ((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು ಹೆಜ್ಜೆ(f, a, b, c, d, x, t, s)                                          \
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

#ಆದರೆ (NGX_HAVE_LITTLE_ENDIAN && NGX_HAVE_NONALIGNED)

#ವ್ಯಾಖ್ಯಾನಿಸು ಗಣ_2(n)      (*(uint32_t *) &p[n * 4])
#ವ್ಯಾಖ್ಯಾನಿಸು GET(n)      (*(uint32_t *) &p[n * 4])

#ಇಲ್ಲದಿದ್ದರೆ

#ವ್ಯಾಖ್ಯಾನಿಸು ಗಣ_2(n)                                                                \
    (ಖಂಡ[n] =                                                               \
    (uint32_t) p[n * 4] |                                                     \
    ((uint32_t) p[n * 4 + 1] << 8) |                                          \
    ((uint32_t) p[n * 4 + 2] << 16) |                                         \
    ((uint32_t) p[n * 4 + 3] << 24))

#ವ್ಯಾಖ್ಯಾನಿಸು GET(n)      ಖಂಡ[n]

#ಆದರೆ_ಅಂತ್ಯ


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ u_char *
ngx_md5_body(ngx_md5_t *ctx, ಸ್ಥಿರಾಂಕ u_char *ದತ್ತಾಂಶ, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    uint32_t       a, b, c, d;
    uint32_t       saved_a, saved_b, saved_c, saved_d;
    ಸ್ಥಿರಾಂಕ u_char  *p;
#ಆದರೆ !(NGX_HAVE_LITTLE_ENDIAN && NGX_HAVE_NONALIGNED)
    uint32_t       ಖಂಡ[16];
#ಆದರೆ_ಅಂತ್ಯ

    p = ದತ್ತಾಂಶ;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;

    ಮಾಡು {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;

        /* Round 1 */

        ಹೆಜ್ಜೆ(F, a, b, c, d, ಗಣ_2(0),  0xd76aa478, 7);
        ಹೆಜ್ಜೆ(F, d, a, b, c, ಗಣ_2(1),  0xe8c7b756, 12);
        ಹೆಜ್ಜೆ(F, c, d, a, b, ಗಣ_2(2),  0x242070db, 17);
        ಹೆಜ್ಜೆ(F, b, c, d, a, ಗಣ_2(3),  0xc1bdceee, 22);
        ಹೆಜ್ಜೆ(F, a, b, c, d, ಗಣ_2(4),  0xf57c0faf, 7);
        ಹೆಜ್ಜೆ(F, d, a, b, c, ಗಣ_2(5),  0x4787c62a, 12);
        ಹೆಜ್ಜೆ(F, c, d, a, b, ಗಣ_2(6),  0xa8304613, 17);
        ಹೆಜ್ಜೆ(F, b, c, d, a, ಗಣ_2(7),  0xfd469501, 22);
        ಹೆಜ್ಜೆ(F, a, b, c, d, ಗಣ_2(8),  0x698098d8, 7);
        ಹೆಜ್ಜೆ(F, d, a, b, c, ಗಣ_2(9),  0x8b44f7af, 12);
        ಹೆಜ್ಜೆ(F, c, d, a, b, ಗಣ_2(10), 0xffff5bb1, 17);
        ಹೆಜ್ಜೆ(F, b, c, d, a, ಗಣ_2(11), 0x895cd7be, 22);
        ಹೆಜ್ಜೆ(F, a, b, c, d, ಗಣ_2(12), 0x6b901122, 7);
        ಹೆಜ್ಜೆ(F, d, a, b, c, ಗಣ_2(13), 0xfd987193, 12);
        ಹೆಜ್ಜೆ(F, c, d, a, b, ಗಣ_2(14), 0xa679438e, 17);
        ಹೆಜ್ಜೆ(F, b, c, d, a, ಗಣ_2(15), 0x49b40821, 22);

        /* Round 2 */

        ಹೆಜ್ಜೆ(G, a, b, c, d, GET(1),  0xf61e2562, 5);
        ಹೆಜ್ಜೆ(G, d, a, b, c, GET(6),  0xc040b340, 9);
        ಹೆಜ್ಜೆ(G, c, d, a, b, GET(11), 0x265e5a51, 14);
        ಹೆಜ್ಜೆ(G, b, c, d, a, GET(0),  0xe9b6c7aa, 20);
        ಹೆಜ್ಜೆ(G, a, b, c, d, GET(5),  0xd62f105d, 5);
        ಹೆಜ್ಜೆ(G, d, a, b, c, GET(10), 0x02441453, 9);
        ಹೆಜ್ಜೆ(G, c, d, a, b, GET(15), 0xd8a1e681, 14);
        ಹೆಜ್ಜೆ(G, b, c, d, a, GET(4),  0xe7d3fbc8, 20);
        ಹೆಜ್ಜೆ(G, a, b, c, d, GET(9),  0x21e1cde6, 5);
        ಹೆಜ್ಜೆ(G, d, a, b, c, GET(14), 0xc33707d6, 9);
        ಹೆಜ್ಜೆ(G, c, d, a, b, GET(3),  0xf4d50d87, 14);
        ಹೆಜ್ಜೆ(G, b, c, d, a, GET(8),  0x455a14ed, 20);
        ಹೆಜ್ಜೆ(G, a, b, c, d, GET(13), 0xa9e3e905, 5);
        ಹೆಜ್ಜೆ(G, d, a, b, c, GET(2),  0xfcefa3f8, 9);
        ಹೆಜ್ಜೆ(G, c, d, a, b, GET(7),  0x676f02d9, 14);
        ಹೆಜ್ಜೆ(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20);

        /* Round 3 */

        ಹೆಜ್ಜೆ(H, a, b, c, d, GET(5),  0xfffa3942, 4);
        ಹೆಜ್ಜೆ(H, d, a, b, c, GET(8),  0x8771f681, 11);
        ಹೆಜ್ಜೆ(H, c, d, a, b, GET(11), 0x6d9d6122, 16);
        ಹೆಜ್ಜೆ(H, b, c, d, a, GET(14), 0xfde5380c, 23);
        ಹೆಜ್ಜೆ(H, a, b, c, d, GET(1),  0xa4beea44, 4);
        ಹೆಜ್ಜೆ(H, d, a, b, c, GET(4),  0x4bdecfa9, 11);
        ಹೆಜ್ಜೆ(H, c, d, a, b, GET(7),  0xf6bb4b60, 16);
        ಹೆಜ್ಜೆ(H, b, c, d, a, GET(10), 0xbebfbc70, 23);
        ಹೆಜ್ಜೆ(H, a, b, c, d, GET(13), 0x289b7ec6, 4);
        ಹೆಜ್ಜೆ(H, d, a, b, c, GET(0),  0xeaa127fa, 11);
        ಹೆಜ್ಜೆ(H, c, d, a, b, GET(3),  0xd4ef3085, 16);
        ಹೆಜ್ಜೆ(H, b, c, d, a, GET(6),  0x04881d05, 23);
        ಹೆಜ್ಜೆ(H, a, b, c, d, GET(9),  0xd9d4d039, 4);
        ಹೆಜ್ಜೆ(H, d, a, b, c, GET(12), 0xe6db99e5, 11);
        ಹೆಜ್ಜೆ(H, c, d, a, b, GET(15), 0x1fa27cf8, 16);
        ಹೆಜ್ಜೆ(H, b, c, d, a, GET(2),  0xc4ac5665, 23);

        /* Round 4 */

        ಹೆಜ್ಜೆ(I, a, b, c, d, GET(0),  0xf4292244, 6);
        ಹೆಜ್ಜೆ(I, d, a, b, c, GET(7),  0x432aff97, 10);
        ಹೆಜ್ಜೆ(I, c, d, a, b, GET(14), 0xab9423a7, 15);
        ಹೆಜ್ಜೆ(I, b, c, d, a, GET(5),  0xfc93a039, 21);
        ಹೆಜ್ಜೆ(I, a, b, c, d, GET(12), 0x655b59c3, 6);
        ಹೆಜ್ಜೆ(I, d, a, b, c, GET(3),  0x8f0ccc92, 10);
        ಹೆಜ್ಜೆ(I, c, d, a, b, GET(10), 0xffeff47d, 15);
        ಹೆಜ್ಜೆ(I, b, c, d, a, GET(1),  0x85845dd1, 21);
        ಹೆಜ್ಜೆ(I, a, b, c, d, GET(8),  0x6fa87e4f, 6);
        ಹೆಜ್ಜೆ(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10);
        ಹೆಜ್ಜೆ(I, c, d, a, b, GET(6),  0xa3014314, 15);
        ಹೆಜ್ಜೆ(I, b, c, d, a, GET(13), 0x4e0811a1, 21);
        ಹೆಜ್ಜೆ(I, a, b, c, d, GET(4),  0xf7537e82, 6);
        ಹೆಜ್ಜೆ(I, d, a, b, c, GET(11), 0xbd3af235, 10);
        ಹೆಜ್ಜೆ(I, c, d, a, b, GET(2),  0x2ad7d2bb, 15);
        ಹೆಜ್ಜೆ(I, b, c, d, a, GET(9),  0xeb86d391, 21);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;

        p += 64;

    } ಆಗ (ಗಾತ್ರ_2 -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;

    ಹಿಂತಿರುಗಿ p;
}
