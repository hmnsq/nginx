
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 *
 * An internal SHA1 implementation.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_sha1.h>


ساكن ثابت u_char *ngx_sha1_body(ngx_sha1_t *ctx, ثابت u_char *داده,
    اندازه_t اندازه);


فراغ
ngx_sha1_init(ngx_sha1_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;
    ctx->e = 0xc3d2e1f0;

    ctx->بایتها = 0;
}


فراغ
ngx_sha1_update(ngx_sha1_t *ctx, ثابت فراغ *داده, اندازه_t اندازه)
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
        (فراغ) ngx_sha1_body(ctx, ctx->میانگیر, 64);
    }

    إذا (اندازه >= 64) {
        داده = ngx_sha1_body(ctx, داده, اندازه & ~(اندازه_t) 0x3f);
        اندازه &= 0x3f;
    }

    ngx_memcpy(ctx->میانگیر, داده, اندازه);
}


فراغ
ngx_sha1_final(u_char نتیجه[20], ngx_sha1_t *ctx)
{
    اندازه_t  استفاده_شده, حرر;

    استفاده_شده = (اندازه_t) (ctx->بایتها & 0x3f);

    ctx->میانگیر[استفاده_شده++] = 0x80;

    حرر = 64 - استفاده_شده;

    إذا (حرر < 8) {
        ngx_memzero(&ctx->میانگیر[استفاده_شده], حرر);
        (فراغ) ngx_sha1_body(ctx, ctx->میانگیر, 64);
        استفاده_شده = 0;
        حرر = 64;
    }

    ngx_memzero(&ctx->میانگیر[استفاده_شده], حرر - 8);

    ctx->بایتها <<= 3;
    ctx->میانگیر[56] = (u_char) (ctx->بایتها >> 56);
    ctx->میانگیر[57] = (u_char) (ctx->بایتها >> 48);
    ctx->میانگیر[58] = (u_char) (ctx->بایتها >> 40);
    ctx->میانگیر[59] = (u_char) (ctx->بایتها >> 32);
    ctx->میانگیر[60] = (u_char) (ctx->بایتها >> 24);
    ctx->میانگیر[61] = (u_char) (ctx->بایتها >> 16);
    ctx->میانگیر[62] = (u_char) (ctx->بایتها >> 8);
    ctx->میانگیر[63] = (u_char) ctx->بایتها;

    (فراغ) ngx_sha1_body(ctx, ctx->میانگیر, 64);

    نتیجه[0] = (u_char) (ctx->a >> 24);
    نتیجه[1] = (u_char) (ctx->a >> 16);
    نتیجه[2] = (u_char) (ctx->a >> 8);
    نتیجه[3] = (u_char) ctx->a;
    نتیجه[4] = (u_char) (ctx->b >> 24);
    نتیجه[5] = (u_char) (ctx->b >> 16);
    نتیجه[6] = (u_char) (ctx->b >> 8);
    نتیجه[7] = (u_char) ctx->b;
    نتیجه[8] = (u_char) (ctx->c >> 24);
    نتیجه[9] = (u_char) (ctx->c >> 16);
    نتیجه[10] = (u_char) (ctx->c >> 8);
    نتیجه[11] = (u_char) ctx->c;
    نتیجه[12] = (u_char) (ctx->d >> 24);
    نتیجه[13] = (u_char) (ctx->d >> 16);
    نتیجه[14] = (u_char) (ctx->d >> 8);
    نتیجه[15] = (u_char) ctx->d;
    نتیجه[16] = (u_char) (ctx->e >> 24);
    نتیجه[17] = (u_char) (ctx->e >> 16);
    نتیجه[18] = (u_char) (ctx->e >> 8);
    نتیجه[19] = (u_char) ctx->e;

    ngx_memzero(ctx, حجم(*ctx));
}


/*
 * Helper functions.
 */

#تعریف_کن ROTATE(بیتها, کلمه)  (((کلمه) << (بیتها)) | ((کلمه) >> (32 - (بیتها))))

#تعریف_کن F1(b, c, d)  (((b) & (c)) | ((~(b)) & (d)))
#تعریف_کن F2(b, c, d)  ((b) ^ (c) ^ (d))
#تعریف_کن F3(b, c, d)  (((b) & (c)) | ((b) & (d)) | ((c) & (d)))

#تعریف_کن گام(f, a, b, c, d, e, w, t)                                          \
    موقت = ROTATE(5, (a)) + f((b), (c), (d)) + (e) + (w) + (t);               \
    (e) = (d);                                                                \
    (d) = (c);                                                                \
    (c) = ROTATE(30, (b));                                                    \
    (b) = (a);                                                                \
    (a) = موقت;


/*
 * GET() reads 4 input bytes in big-endian byte order and returns
 * them as uint32_t.
 */

#تعریف_کن GET(n)                                                                \
    ((uint32_t) p[n * 4 + 3] |                                                \
    ((uint32_t) p[n * 4 + 2] << 8) |                                          \
    ((uint32_t) p[n * 4 + 1] << 16) |                                         \
    ((uint32_t) p[n * 4] << 24))


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

ساكن ثابت u_char *
ngx_sha1_body(ngx_sha1_t *ctx, ثابت u_char *داده, اندازه_t اندازه)
{
    uint32_t       a, b, c, d, e, موقت;
    uint32_t       saved_a, saved_b, saved_c, saved_d, saved_e;
    uint32_t       واژگان[80];
    ngx_uint_t     i;
    ثابت u_char  *p;

    p = داده;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;
    e = ctx->e;

    افعل {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;
        saved_e = e;

        /* Load data block into the words array */

        لكل (i = 0; i < 16; i++) {
            واژگان[i] = GET(i);
        }

        لكل (i = 16; i < 80; i++) {
            واژگان[i] = ROTATE(1, واژگان[i - 3] ^ واژگان[i - 8] ^ واژگان[i - 14]
                                 ^ واژگان[i - 16]);
        }

        /* Transformations */

        گام(F1, a, b, c, d, e, واژگان[0],  0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[1],  0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[2],  0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[3],  0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[4],  0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[5],  0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[6],  0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[7],  0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[8],  0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[9],  0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[10], 0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[11], 0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[12], 0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[13], 0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[14], 0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[15], 0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[16], 0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[17], 0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[18], 0x5a827999);
        گام(F1, a, b, c, d, e, واژگان[19], 0x5a827999);

        گام(F2, a, b, c, d, e, واژگان[20], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[21], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[22], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[23], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[24], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[25], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[26], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[27], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[28], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[29], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[30], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[31], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[32], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[33], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[34], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[35], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[36], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[37], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[38], 0x6ed9eba1);
        گام(F2, a, b, c, d, e, واژگان[39], 0x6ed9eba1);

        گام(F3, a, b, c, d, e, واژگان[40], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[41], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[42], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[43], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[44], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[45], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[46], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[47], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[48], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[49], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[50], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[51], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[52], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[53], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[54], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[55], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[56], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[57], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[58], 0x8f1bbcdc);
        گام(F3, a, b, c, d, e, واژگان[59], 0x8f1bbcdc);

        گام(F2, a, b, c, d, e, واژگان[60], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[61], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[62], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[63], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[64], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[65], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[66], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[67], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[68], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[69], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[70], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[71], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[72], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[73], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[74], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[75], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[76], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[77], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[78], 0xca62c1d6);
        گام(F2, a, b, c, d, e, واژگان[79], 0xca62c1d6);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;
        e += saved_e;

        p += 64;

    } طالما (اندازه -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;
    ctx->e = e;

    أرجع p;
}
