
/*
 * An internal implementation, based on Alexander Peslyak's
 * public domain implementation:
 * http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <ngx_md5.h>


స్థిర స్థిరరాశి u_char *ngx_md5_body(ngx_md5_t *ctx, స్థిరరాశి u_char *దత్తాంశం,
    కొలత_t కొలత);


శూన్య
ngx_md5_init(ngx_md5_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;

    ctx->బైట్లు = 0;
}


శూన్య
ngx_md5_update(ngx_md5_t *ctx, స్థిరరాశి శూన్య *దత్తాంశం, కొలత_t కొలత)
{
    కొలత_t  వాడిన, విడుదల;

    వాడిన = (కొలత_t) (ctx->బైట్లు & 0x3f);
    ctx->బైట్లు += కొలత;

    ఒకవేళ (వాడిన) {
        విడుదల = 64 - వాడిన;

        ఒకవేళ (కొలత < విడుదల) {
            ngx_memcpy(&ctx->బఫరు[వాడిన], దత్తాంశం, కొలత);
            తిరిగి;
        }

        ngx_memcpy(&ctx->బఫరు[వాడిన], దత్తాంశం, విడుదల);
        దత్తాంశం = (u_char *) దత్తాంశం + విడుదల;
        కొలత -= విడుదల;
        (శూన్య) ngx_md5_body(ctx, ctx->బఫరు, 64);
    }

    ఒకవేళ (కొలత >= 64) {
        దత్తాంశం = ngx_md5_body(ctx, దత్తాంశం, కొలత & ~(కొలత_t) 0x3f);
        కొలత &= 0x3f;
    }

    ngx_memcpy(ctx->బఫరు, దత్తాంశం, కొలత);
}


శూన్య
ngx_md5_final(u_char ఫలితం[16], ngx_md5_t *ctx)
{
    కొలత_t  వాడిన, విడుదల;

    వాడిన = (కొలత_t) (ctx->బైట్లు & 0x3f);

    ctx->బఫరు[వాడిన++] = 0x80;

    విడుదల = 64 - వాడిన;

    ఒకవేళ (విడుదల < 8) {
        ngx_memzero(&ctx->బఫరు[వాడిన], విడుదల);
        (శూన్య) ngx_md5_body(ctx, ctx->బఫరు, 64);
        వాడిన = 0;
        విడుదల = 64;
    }

    ngx_memzero(&ctx->బఫరు[వాడిన], విడుదల - 8);

    ctx->బైట్లు <<= 3;
    ctx->బఫరు[56] = (u_char) ctx->బైట్లు;
    ctx->బఫరు[57] = (u_char) (ctx->బైట్లు >> 8);
    ctx->బఫరు[58] = (u_char) (ctx->బైట్లు >> 16);
    ctx->బఫరు[59] = (u_char) (ctx->బైట్లు >> 24);
    ctx->బఫరు[60] = (u_char) (ctx->బైట్లు >> 32);
    ctx->బఫరు[61] = (u_char) (ctx->బైట్లు >> 40);
    ctx->బఫరు[62] = (u_char) (ctx->బైట్లు >> 48);
    ctx->బఫరు[63] = (u_char) (ctx->బైట్లు >> 56);

    (శూన్య) ngx_md5_body(ctx, ctx->బఫరు, 64);

    ఫలితం[0] = (u_char) ctx->a;
    ఫలితం[1] = (u_char) (ctx->a >> 8);
    ఫలితం[2] = (u_char) (ctx->a >> 16);
    ఫలితం[3] = (u_char) (ctx->a >> 24);
    ఫలితం[4] = (u_char) ctx->b;
    ఫలితం[5] = (u_char) (ctx->b >> 8);
    ఫలితం[6] = (u_char) (ctx->b >> 16);
    ఫలితం[7] = (u_char) (ctx->b >> 24);
    ఫలితం[8] = (u_char) ctx->c;
    ఫలితం[9] = (u_char) (ctx->c >> 8);
    ఫలితం[10] = (u_char) (ctx->c >> 16);
    ఫలితం[11] = (u_char) (ctx->c >> 24);
    ఫలితం[12] = (u_char) ctx->d;
    ఫలితం[13] = (u_char) (ctx->d >> 8);
    ఫలితం[14] = (u_char) (ctx->d >> 16);
    ఫలితం[15] = (u_char) (ctx->d >> 24);

    ngx_memzero(ctx, పరిమాణం(*ctx));
}


/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in
 * Colin Plumb's implementation.
 */

#నిర్వచించు F(x, y, z)  ((z) ^ ((x) & ((y) ^ (z))))
#నిర్వచించు G(x, y, z)  ((y) ^ ((z) & ((x) ^ (y))))
#నిర్వచించు H(x, y, z)  ((x) ^ (y) ^ (z))
#నిర్వచించు I(x, y, z)  ((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */

#నిర్వచించు అడుగు(f, a, b, c, d, x, t, s)                                          \
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

#ఒకవేళ (NGX_HAVE_LITTLE_ENDIAN && NGX_HAVE_NONALIGNED)

#నిర్వచించు సమితి_2(n)      (*(uint32_t *) &p[n * 4])
#నిర్వచించు GET(n)      (*(uint32_t *) &p[n * 4])

#లేకపోతే

#నిర్వచించు సమితి_2(n)                                                                \
    (ఖండం[n] =                                                               \
    (uint32_t) p[n * 4] |                                                     \
    ((uint32_t) p[n * 4 + 1] << 8) |                                          \
    ((uint32_t) p[n * 4 + 2] << 16) |                                         \
    ((uint32_t) p[n * 4 + 3] << 24))

#నిర్వచించు GET(n)      ఖండం[n]

#అయితే_ముగింపు


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

స్థిర స్థిరరాశి u_char *
ngx_md5_body(ngx_md5_t *ctx, స్థిరరాశి u_char *దత్తాంశం, కొలత_t కొలత)
{
    uint32_t       a, b, c, d;
    uint32_t       saved_a, saved_b, saved_c, saved_d;
    స్థిరరాశి u_char  *p;
#ఒకవేళ !(NGX_HAVE_LITTLE_ENDIAN && NGX_HAVE_NONALIGNED)
    uint32_t       ఖండం[16];
#అయితే_ముగింపు

    p = దత్తాంశం;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;

    చేయి {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;

        /* Round 1 */

        అడుగు(F, a, b, c, d, సమితి_2(0),  0xd76aa478, 7);
        అడుగు(F, d, a, b, c, సమితి_2(1),  0xe8c7b756, 12);
        అడుగు(F, c, d, a, b, సమితి_2(2),  0x242070db, 17);
        అడుగు(F, b, c, d, a, సమితి_2(3),  0xc1bdceee, 22);
        అడుగు(F, a, b, c, d, సమితి_2(4),  0xf57c0faf, 7);
        అడుగు(F, d, a, b, c, సమితి_2(5),  0x4787c62a, 12);
        అడుగు(F, c, d, a, b, సమితి_2(6),  0xa8304613, 17);
        అడుగు(F, b, c, d, a, సమితి_2(7),  0xfd469501, 22);
        అడుగు(F, a, b, c, d, సమితి_2(8),  0x698098d8, 7);
        అడుగు(F, d, a, b, c, సమితి_2(9),  0x8b44f7af, 12);
        అడుగు(F, c, d, a, b, సమితి_2(10), 0xffff5bb1, 17);
        అడుగు(F, b, c, d, a, సమితి_2(11), 0x895cd7be, 22);
        అడుగు(F, a, b, c, d, సమితి_2(12), 0x6b901122, 7);
        అడుగు(F, d, a, b, c, సమితి_2(13), 0xfd987193, 12);
        అడుగు(F, c, d, a, b, సమితి_2(14), 0xa679438e, 17);
        అడుగు(F, b, c, d, a, సమితి_2(15), 0x49b40821, 22);

        /* Round 2 */

        అడుగు(G, a, b, c, d, GET(1),  0xf61e2562, 5);
        అడుగు(G, d, a, b, c, GET(6),  0xc040b340, 9);
        అడుగు(G, c, d, a, b, GET(11), 0x265e5a51, 14);
        అడుగు(G, b, c, d, a, GET(0),  0xe9b6c7aa, 20);
        అడుగు(G, a, b, c, d, GET(5),  0xd62f105d, 5);
        అడుగు(G, d, a, b, c, GET(10), 0x02441453, 9);
        అడుగు(G, c, d, a, b, GET(15), 0xd8a1e681, 14);
        అడుగు(G, b, c, d, a, GET(4),  0xe7d3fbc8, 20);
        అడుగు(G, a, b, c, d, GET(9),  0x21e1cde6, 5);
        అడుగు(G, d, a, b, c, GET(14), 0xc33707d6, 9);
        అడుగు(G, c, d, a, b, GET(3),  0xf4d50d87, 14);
        అడుగు(G, b, c, d, a, GET(8),  0x455a14ed, 20);
        అడుగు(G, a, b, c, d, GET(13), 0xa9e3e905, 5);
        అడుగు(G, d, a, b, c, GET(2),  0xfcefa3f8, 9);
        అడుగు(G, c, d, a, b, GET(7),  0x676f02d9, 14);
        అడుగు(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20);

        /* Round 3 */

        అడుగు(H, a, b, c, d, GET(5),  0xfffa3942, 4);
        అడుగు(H, d, a, b, c, GET(8),  0x8771f681, 11);
        అడుగు(H, c, d, a, b, GET(11), 0x6d9d6122, 16);
        అడుగు(H, b, c, d, a, GET(14), 0xfde5380c, 23);
        అడుగు(H, a, b, c, d, GET(1),  0xa4beea44, 4);
        అడుగు(H, d, a, b, c, GET(4),  0x4bdecfa9, 11);
        అడుగు(H, c, d, a, b, GET(7),  0xf6bb4b60, 16);
        అడుగు(H, b, c, d, a, GET(10), 0xbebfbc70, 23);
        అడుగు(H, a, b, c, d, GET(13), 0x289b7ec6, 4);
        అడుగు(H, d, a, b, c, GET(0),  0xeaa127fa, 11);
        అడుగు(H, c, d, a, b, GET(3),  0xd4ef3085, 16);
        అడుగు(H, b, c, d, a, GET(6),  0x04881d05, 23);
        అడుగు(H, a, b, c, d, GET(9),  0xd9d4d039, 4);
        అడుగు(H, d, a, b, c, GET(12), 0xe6db99e5, 11);
        అడుగు(H, c, d, a, b, GET(15), 0x1fa27cf8, 16);
        అడుగు(H, b, c, d, a, GET(2),  0xc4ac5665, 23);

        /* Round 4 */

        అడుగు(I, a, b, c, d, GET(0),  0xf4292244, 6);
        అడుగు(I, d, a, b, c, GET(7),  0x432aff97, 10);
        అడుగు(I, c, d, a, b, GET(14), 0xab9423a7, 15);
        అడుగు(I, b, c, d, a, GET(5),  0xfc93a039, 21);
        అడుగు(I, a, b, c, d, GET(12), 0x655b59c3, 6);
        అడుగు(I, d, a, b, c, GET(3),  0x8f0ccc92, 10);
        అడుగు(I, c, d, a, b, GET(10), 0xffeff47d, 15);
        అడుగు(I, b, c, d, a, GET(1),  0x85845dd1, 21);
        అడుగు(I, a, b, c, d, GET(8),  0x6fa87e4f, 6);
        అడుగు(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10);
        అడుగు(I, c, d, a, b, GET(6),  0xa3014314, 15);
        అడుగు(I, b, c, d, a, GET(13), 0x4e0811a1, 21);
        అడుగు(I, a, b, c, d, GET(4),  0xf7537e82, 6);
        అడుగు(I, d, a, b, c, GET(11), 0xbd3af235, 10);
        అడుగు(I, c, d, a, b, GET(2),  0x2ad7d2bb, 15);
        అడుగు(I, b, c, d, a, GET(9),  0xeb86d391, 21);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;

        p += 64;

    } అయితే (కొలత -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;

    తిరిగి p;
}
