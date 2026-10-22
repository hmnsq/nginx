
/*
 * An internal implementation, based on Alexander Peslyak's
 * public domain implementation:
 * http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_md5.h>


ساكن ثابت u_char *ngx_md5_body(ngx_md5_t *ctx, ثابت u_char *داده,
    اندازه_t اندازه);


فراغ
ngx_md5_init(ngx_md5_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;

    ctx->بایتها = 0;
}


فراغ
ngx_md5_update(ngx_md5_t *ctx, ثابت فراغ *داده, اندازه_t اندازه)
{
    اندازه_t  استفاده_شده, حرر;

    استفاده_شده = (اندازه_t) (ctx->بایتها & 0x3f);
    ctx->بایتها += اندازه;

    إذا (استفاده_شده) {
        حرر = 64 - استفاده_شده;

        إذا (اندازه < حرر) {
            ngx_memcpy(&ctx->میانگیر[استفاده_شده], داده, اندازه);
            أرجع;
        }

        ngx_memcpy(&ctx->میانگیر[استفاده_شده], داده, حرر);
        داده = (u_char *) داده + حرر;
        اندازه -= حرر;
        (فراغ) ngx_md5_body(ctx, ctx->میانگیر, 64);
    }

    إذا (اندازه >= 64) {
        داده = ngx_md5_body(ctx, داده, اندازه & ~(اندازه_t) 0x3f);
        اندازه &= 0x3f;
    }

    ngx_memcpy(ctx->میانگیر, داده, اندازه);
}


فراغ
ngx_md5_final(u_char نتیجه[16], ngx_md5_t *ctx)
{
    اندازه_t  استفاده_شده, حرر;

    استفاده_شده = (اندازه_t) (ctx->بایتها & 0x3f);

    ctx->میانگیر[استفاده_شده++] = 0x80;

    حرر = 64 - استفاده_شده;

    إذا (حرر < 8) {
        ngx_memzero(&ctx->میانگیر[استفاده_شده], حرر);
        (فراغ) ngx_md5_body(ctx, ctx->میانگیر, 64);
        استفاده_شده = 0;
        حرر = 64;
    }

    ngx_memzero(&ctx->میانگیر[استفاده_شده], حرر - 8);

    ctx->بایتها <<= 3;
    ctx->میانگیر[56] = (u_char) ctx->بایتها;
    ctx->میانگیر[57] = (u_char) (ctx->بایتها >> 8);
    ctx->میانگیر[58] = (u_char) (ctx->بایتها >> 16);
    ctx->میانگیر[59] = (u_char) (ctx->بایتها >> 24);
    ctx->میانگیر[60] = (u_char) (ctx->بایتها >> 32);
    ctx->میانگیر[61] = (u_char) (ctx->بایتها >> 40);
    ctx->میانگیر[62] = (u_char) (ctx->بایتها >> 48);
    ctx->میانگیر[63] = (u_char) (ctx->بایتها >> 56);

    (فراغ) ngx_md5_body(ctx, ctx->میانگیر, 64);

    نتیجه[0] = (u_char) ctx->a;
    نتیجه[1] = (u_char) (ctx->a >> 8);
    نتیجه[2] = (u_char) (ctx->a >> 16);
    نتیجه[3] = (u_char) (ctx->a >> 24);
    نتیجه[4] = (u_char) ctx->b;
    نتیجه[5] = (u_char) (ctx->b >> 8);
    نتیجه[6] = (u_char) (ctx->b >> 16);
    نتیجه[7] = (u_char) (ctx->b >> 24);
    نتیجه[8] = (u_char) ctx->c;
    نتیجه[9] = (u_char) (ctx->c >> 8);
    نتیجه[10] = (u_char) (ctx->c >> 16);
    نتیجه[11] = (u_char) (ctx->c >> 24);
    نتیجه[12] = (u_char) ctx->d;
    نتیجه[13] = (u_char) (ctx->d >> 8);
    نتیجه[14] = (u_char) (ctx->d >> 16);
    نتیجه[15] = (u_char) (ctx->d >> 24);

    ngx_memzero(ctx, حجم(*ctx));
}


/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in
 * Colin Plumb's implementation.
 */

#تعریف_کن F(x, y, z)  ((z) ^ ((x) & ((y) ^ (z))))
#تعریف_کن G(x, y, z)  ((y) ^ ((z) & ((x) ^ (y))))
#تعریف_کن H(x, y, z)  ((x) ^ (y) ^ (z))
#تعریف_کن I(x, y, z)  ((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */

#تعریف_کن گام(f, a, b, c, d, x, t, s)                                          \
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

#إذا (NGX_HAVE_LITTLE_ENDIAN && NGX_HAVE_NONALIGNED)

#تعریف_کن مجموعه_2(n)      (*(uint32_t *) &p[n * 4])
#تعریف_کن GET(n)      (*(uint32_t *) &p[n * 4])

#وإلا

#تعریف_کن مجموعه_2(n)                                                                \
    (بلوک[n] =                                                               \
    (uint32_t) p[n * 4] |                                                     \
    ((uint32_t) p[n * 4 + 1] << 8) |                                          \
    ((uint32_t) p[n * 4 + 2] << 16) |                                         \
    ((uint32_t) p[n * 4 + 3] << 24))

#تعریف_کن GET(n)      بلوک[n]

#پایان_اگر


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

ساكن ثابت u_char *
ngx_md5_body(ngx_md5_t *ctx, ثابت u_char *داده, اندازه_t اندازه)
{
    uint32_t       a, b, c, d;
    uint32_t       saved_a, saved_b, saved_c, saved_d;
    ثابت u_char  *p;
#إذا !(NGX_HAVE_LITTLE_ENDIAN && NGX_HAVE_NONALIGNED)
    uint32_t       بلوک[16];
#پایان_اگر

    p = داده;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;

    افعل {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;

        /* Round 1 */

        گام(F, a, b, c, d, مجموعه_2(0),  0xd76aa478, 7);
        گام(F, d, a, b, c, مجموعه_2(1),  0xe8c7b756, 12);
        گام(F, c, d, a, b, مجموعه_2(2),  0x242070db, 17);
        گام(F, b, c, d, a, مجموعه_2(3),  0xc1bdceee, 22);
        گام(F, a, b, c, d, مجموعه_2(4),  0xf57c0faf, 7);
        گام(F, d, a, b, c, مجموعه_2(5),  0x4787c62a, 12);
        گام(F, c, d, a, b, مجموعه_2(6),  0xa8304613, 17);
        گام(F, b, c, d, a, مجموعه_2(7),  0xfd469501, 22);
        گام(F, a, b, c, d, مجموعه_2(8),  0x698098d8, 7);
        گام(F, d, a, b, c, مجموعه_2(9),  0x8b44f7af, 12);
        گام(F, c, d, a, b, مجموعه_2(10), 0xffff5bb1, 17);
        گام(F, b, c, d, a, مجموعه_2(11), 0x895cd7be, 22);
        گام(F, a, b, c, d, مجموعه_2(12), 0x6b901122, 7);
        گام(F, d, a, b, c, مجموعه_2(13), 0xfd987193, 12);
        گام(F, c, d, a, b, مجموعه_2(14), 0xa679438e, 17);
        گام(F, b, c, d, a, مجموعه_2(15), 0x49b40821, 22);

        /* Round 2 */

        گام(G, a, b, c, d, GET(1),  0xf61e2562, 5);
        گام(G, d, a, b, c, GET(6),  0xc040b340, 9);
        گام(G, c, d, a, b, GET(11), 0x265e5a51, 14);
        گام(G, b, c, d, a, GET(0),  0xe9b6c7aa, 20);
        گام(G, a, b, c, d, GET(5),  0xd62f105d, 5);
        گام(G, d, a, b, c, GET(10), 0x02441453, 9);
        گام(G, c, d, a, b, GET(15), 0xd8a1e681, 14);
        گام(G, b, c, d, a, GET(4),  0xe7d3fbc8, 20);
        گام(G, a, b, c, d, GET(9),  0x21e1cde6, 5);
        گام(G, d, a, b, c, GET(14), 0xc33707d6, 9);
        گام(G, c, d, a, b, GET(3),  0xf4d50d87, 14);
        گام(G, b, c, d, a, GET(8),  0x455a14ed, 20);
        گام(G, a, b, c, d, GET(13), 0xa9e3e905, 5);
        گام(G, d, a, b, c, GET(2),  0xfcefa3f8, 9);
        گام(G, c, d, a, b, GET(7),  0x676f02d9, 14);
        گام(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20);

        /* Round 3 */

        گام(H, a, b, c, d, GET(5),  0xfffa3942, 4);
        گام(H, d, a, b, c, GET(8),  0x8771f681, 11);
        گام(H, c, d, a, b, GET(11), 0x6d9d6122, 16);
        گام(H, b, c, d, a, GET(14), 0xfde5380c, 23);
        گام(H, a, b, c, d, GET(1),  0xa4beea44, 4);
        گام(H, d, a, b, c, GET(4),  0x4bdecfa9, 11);
        گام(H, c, d, a, b, GET(7),  0xf6bb4b60, 16);
        گام(H, b, c, d, a, GET(10), 0xbebfbc70, 23);
        گام(H, a, b, c, d, GET(13), 0x289b7ec6, 4);
        گام(H, d, a, b, c, GET(0),  0xeaa127fa, 11);
        گام(H, c, d, a, b, GET(3),  0xd4ef3085, 16);
        گام(H, b, c, d, a, GET(6),  0x04881d05, 23);
        گام(H, a, b, c, d, GET(9),  0xd9d4d039, 4);
        گام(H, d, a, b, c, GET(12), 0xe6db99e5, 11);
        گام(H, c, d, a, b, GET(15), 0x1fa27cf8, 16);
        گام(H, b, c, d, a, GET(2),  0xc4ac5665, 23);

        /* Round 4 */

        گام(I, a, b, c, d, GET(0),  0xf4292244, 6);
        گام(I, d, a, b, c, GET(7),  0x432aff97, 10);
        گام(I, c, d, a, b, GET(14), 0xab9423a7, 15);
        گام(I, b, c, d, a, GET(5),  0xfc93a039, 21);
        گام(I, a, b, c, d, GET(12), 0x655b59c3, 6);
        گام(I, d, a, b, c, GET(3),  0x8f0ccc92, 10);
        گام(I, c, d, a, b, GET(10), 0xffeff47d, 15);
        گام(I, b, c, d, a, GET(1),  0x85845dd1, 21);
        گام(I, a, b, c, d, GET(8),  0x6fa87e4f, 6);
        گام(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10);
        گام(I, c, d, a, b, GET(6),  0xa3014314, 15);
        گام(I, b, c, d, a, GET(13), 0x4e0811a1, 21);
        گام(I, a, b, c, d, GET(4),  0xf7537e82, 6);
        گام(I, d, a, b, c, GET(11), 0xbd3af235, 10);
        گام(I, c, d, a, b, GET(2),  0x2ad7d2bb, 15);
        گام(I, b, c, d, a, GET(9),  0xeb86d391, 21);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;

        p += 64;

    } طالما (اندازه -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;

    أرجع p;
}
