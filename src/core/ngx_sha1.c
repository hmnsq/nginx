
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 *
 * An internal SHA1 implementation.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_sha1.h>


ساكن ثابت u_char *ngx_sha1_body(ngx_sha1_t *ctx, ثابت u_char *معلومات,
    سائيز_t سائيز);


فراغ
ngx_sha1_init(ngx_sha1_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;
    ctx->e = 0xc3d2e1f0;

    ctx->بائيٽون = 0;
}


فراغ
ngx_sha1_update(ngx_sha1_t *ctx, ثابت فراغ *معلومات, سائيز_t سائيز)
{
    سائيز_t  استعمال_ٿيل, حرر;

    استعمال_ٿيل = (سائيز_t) (ctx->بائيٽون & 0x3f);
    ctx->بائيٽون += سائيز;

    إذا (استعمال_ٿيل) {
        حرر = 64 - استعمال_ٿيل;

        إذا (سائيز < حرر) {
            ngx_memcpy(&ctx->بفر[استعمال_ٿيل], معلومات, سائيز);
            أرجع;
        }

        ngx_memcpy(&ctx->بفر[استعمال_ٿيل], معلومات, حرر);
        معلومات = (u_char *) معلومات + حرر;
        سائيز -= حرر;
        (فراغ) ngx_sha1_body(ctx, ctx->بفر, 64);
    }

    إذا (سائيز >= 64) {
        معلومات = ngx_sha1_body(ctx, معلومات, سائيز & ~(سائيز_t) 0x3f);
        سائيز &= 0x3f;
    }

    ngx_memcpy(ctx->بفر, معلومات, سائيز);
}


فراغ
ngx_sha1_final(u_char نتيجو[20], ngx_sha1_t *ctx)
{
    سائيز_t  استعمال_ٿيل, حرر;

    استعمال_ٿيل = (سائيز_t) (ctx->بائيٽون & 0x3f);

    ctx->بفر[استعمال_ٿيل++] = 0x80;

    حرر = 64 - استعمال_ٿيل;

    إذا (حرر < 8) {
        ngx_memzero(&ctx->بفر[استعمال_ٿيل], حرر);
        (فراغ) ngx_sha1_body(ctx, ctx->بفر, 64);
        استعمال_ٿيل = 0;
        حرر = 64;
    }

    ngx_memzero(&ctx->بفر[استعمال_ٿيل], حرر - 8);

    ctx->بائيٽون <<= 3;
    ctx->بفر[56] = (u_char) (ctx->بائيٽون >> 56);
    ctx->بفر[57] = (u_char) (ctx->بائيٽون >> 48);
    ctx->بفر[58] = (u_char) (ctx->بائيٽون >> 40);
    ctx->بفر[59] = (u_char) (ctx->بائيٽون >> 32);
    ctx->بفر[60] = (u_char) (ctx->بائيٽون >> 24);
    ctx->بفر[61] = (u_char) (ctx->بائيٽون >> 16);
    ctx->بفر[62] = (u_char) (ctx->بائيٽون >> 8);
    ctx->بفر[63] = (u_char) ctx->بائيٽون;

    (فراغ) ngx_sha1_body(ctx, ctx->بفر, 64);

    نتيجو[0] = (u_char) (ctx->a >> 24);
    نتيجو[1] = (u_char) (ctx->a >> 16);
    نتيجو[2] = (u_char) (ctx->a >> 8);
    نتيجو[3] = (u_char) ctx->a;
    نتيجو[4] = (u_char) (ctx->b >> 24);
    نتيجو[5] = (u_char) (ctx->b >> 16);
    نتيجو[6] = (u_char) (ctx->b >> 8);
    نتيجو[7] = (u_char) ctx->b;
    نتيجو[8] = (u_char) (ctx->c >> 24);
    نتيجو[9] = (u_char) (ctx->c >> 16);
    نتيجو[10] = (u_char) (ctx->c >> 8);
    نتيجو[11] = (u_char) ctx->c;
    نتيجو[12] = (u_char) (ctx->d >> 24);
    نتيجو[13] = (u_char) (ctx->d >> 16);
    نتيجو[14] = (u_char) (ctx->d >> 8);
    نتيجو[15] = (u_char) ctx->d;
    نتيجو[16] = (u_char) (ctx->e >> 24);
    نتيجو[17] = (u_char) (ctx->e >> 16);
    نتيجو[18] = (u_char) (ctx->e >> 8);
    نتيجو[19] = (u_char) ctx->e;

    ngx_memzero(ctx, حجم(*ctx));
}


/*
 * Helper functions.
 */

#وصف_ڪريو ROTATE(بٽ, لفظ)  (((لفظ) << (بٽ)) | ((لفظ) >> (32 - (بٽ))))

#وصف_ڪريو F1(b, c, d)  (((b) & (c)) | ((~(b)) & (d)))
#وصف_ڪريو F2(b, c, d)  ((b) ^ (c) ^ (d))
#وصف_ڪريو F3(b, c, d)  (((b) & (c)) | ((b) & (d)) | ((c) & (d)))

#وصف_ڪريو قدم(f, a, b, c, d, e, w, t)                                          \
    عارضي = ROTATE(5, (a)) + f((b), (c), (d)) + (e) + (w) + (t);               \
    (e) = (d);                                                                \
    (d) = (c);                                                                \
    (c) = ROTATE(30, (b));                                                    \
    (b) = (a);                                                                \
    (a) = عارضي;


/*
 * GET() reads 4 input bytes in big-endian byte order and returns
 * them as uint32_t.
 */

#وصف_ڪريو GET(n)                                                                \
    ((uint32_t) p[n * 4 + 3] |                                                \
    ((uint32_t) p[n * 4 + 2] << 8) |                                          \
    ((uint32_t) p[n * 4 + 1] << 16) |                                         \
    ((uint32_t) p[n * 4] << 24))


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

ساكن ثابت u_char *
ngx_sha1_body(ngx_sha1_t *ctx, ثابت u_char *معلومات, سائيز_t سائيز)
{
    uint32_t       a, b, c, d, e, عارضي;
    uint32_t       saved_a, saved_b, saved_c, saved_d, saved_e;
    uint32_t       لفظ_2[80];
    ngx_uint_t     i;
    ثابت u_char  *p;

    p = معلومات;

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
            لفظ_2[i] = GET(i);
        }

        لكل (i = 16; i < 80; i++) {
            لفظ_2[i] = ROTATE(1, لفظ_2[i - 3] ^ لفظ_2[i - 8] ^ لفظ_2[i - 14]
                                 ^ لفظ_2[i - 16]);
        }

        /* Transformations */

        قدم(F1, a, b, c, d, e, لفظ_2[0],  0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[1],  0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[2],  0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[3],  0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[4],  0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[5],  0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[6],  0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[7],  0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[8],  0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[9],  0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[10], 0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[11], 0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[12], 0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[13], 0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[14], 0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[15], 0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[16], 0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[17], 0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[18], 0x5a827999);
        قدم(F1, a, b, c, d, e, لفظ_2[19], 0x5a827999);

        قدم(F2, a, b, c, d, e, لفظ_2[20], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[21], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[22], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[23], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[24], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[25], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[26], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[27], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[28], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[29], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[30], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[31], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[32], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[33], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[34], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[35], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[36], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[37], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[38], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, لفظ_2[39], 0x6ed9eba1);

        قدم(F3, a, b, c, d, e, لفظ_2[40], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[41], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[42], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[43], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[44], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[45], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[46], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[47], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[48], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[49], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[50], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[51], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[52], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[53], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[54], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[55], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[56], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[57], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[58], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, لفظ_2[59], 0x8f1bbcdc);

        قدم(F2, a, b, c, d, e, لفظ_2[60], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[61], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[62], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[63], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[64], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[65], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[66], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[67], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[68], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[69], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[70], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[71], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[72], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[73], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[74], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[75], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[76], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[77], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[78], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, لفظ_2[79], 0xca62c1d6);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;
        e += saved_e;

        p += 64;

    } طالما (سائيز -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;
    ctx->e = e;

    أرجع p;
}
