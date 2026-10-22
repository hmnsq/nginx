
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 *
 * An internal SHA1 implementation.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_sha1.h>


ساكن ثابت u_char *ngx_sha1_body(ngx_sha1_t *ctx, ثابت u_char *انکڑے,
    ناپ_t ناپ);


فراغ
ngx_sha1_init(ngx_sha1_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;
    ctx->e = 0xc3d2e1f0;

    ctx->بائٹاں = 0;
}


فراغ
ngx_sha1_update(ngx_sha1_t *ctx, ثابت فراغ *انکڑے, ناپ_t ناپ)
{
    ناپ_t  ورتیا, حرر;

    ورتیا = (ناپ_t) (ctx->بائٹاں & 0x3f);
    ctx->بائٹاں += ناپ;

    إذا (ورتیا) {
        حرر = 64 - ورتیا;

        إذا (ناپ < حرر) {
            ngx_memcpy(&ctx->بفر[ورتیا], انکڑے, ناپ);
            أرجع;
        }

        ngx_memcpy(&ctx->بفر[ورتیا], انکڑے, حرر);
        انکڑے = (u_char *) انکڑے + حرر;
        ناپ -= حرر;
        (فراغ) ngx_sha1_body(ctx, ctx->بفر, 64);
    }

    إذا (ناپ >= 64) {
        انکڑے = ngx_sha1_body(ctx, انکڑے, ناپ & ~(ناپ_t) 0x3f);
        ناپ &= 0x3f;
    }

    ngx_memcpy(ctx->بفر, انکڑے, ناپ);
}


فراغ
ngx_sha1_final(u_char نتیجہ[20], ngx_sha1_t *ctx)
{
    ناپ_t  ورتیا, حرر;

    ورتیا = (ناپ_t) (ctx->بائٹاں & 0x3f);

    ctx->بفر[ورتیا++] = 0x80;

    حرر = 64 - ورتیا;

    إذا (حرر < 8) {
        ngx_memzero(&ctx->بفر[ورتیا], حرر);
        (فراغ) ngx_sha1_body(ctx, ctx->بفر, 64);
        ورتیا = 0;
        حرر = 64;
    }

    ngx_memzero(&ctx->بفر[ورتیا], حرر - 8);

    ctx->بائٹاں <<= 3;
    ctx->بفر[56] = (u_char) (ctx->بائٹاں >> 56);
    ctx->بفر[57] = (u_char) (ctx->بائٹاں >> 48);
    ctx->بفر[58] = (u_char) (ctx->بائٹاں >> 40);
    ctx->بفر[59] = (u_char) (ctx->بائٹاں >> 32);
    ctx->بفر[60] = (u_char) (ctx->بائٹاں >> 24);
    ctx->بفر[61] = (u_char) (ctx->بائٹاں >> 16);
    ctx->بفر[62] = (u_char) (ctx->بائٹاں >> 8);
    ctx->بفر[63] = (u_char) ctx->بائٹاں;

    (فراغ) ngx_sha1_body(ctx, ctx->بفر, 64);

    نتیجہ[0] = (u_char) (ctx->a >> 24);
    نتیجہ[1] = (u_char) (ctx->a >> 16);
    نتیجہ[2] = (u_char) (ctx->a >> 8);
    نتیجہ[3] = (u_char) ctx->a;
    نتیجہ[4] = (u_char) (ctx->b >> 24);
    نتیجہ[5] = (u_char) (ctx->b >> 16);
    نتیجہ[6] = (u_char) (ctx->b >> 8);
    نتیجہ[7] = (u_char) ctx->b;
    نتیجہ[8] = (u_char) (ctx->c >> 24);
    نتیجہ[9] = (u_char) (ctx->c >> 16);
    نتیجہ[10] = (u_char) (ctx->c >> 8);
    نتیجہ[11] = (u_char) ctx->c;
    نتیجہ[12] = (u_char) (ctx->d >> 24);
    نتیجہ[13] = (u_char) (ctx->d >> 16);
    نتیجہ[14] = (u_char) (ctx->d >> 8);
    نتیجہ[15] = (u_char) ctx->d;
    نتیجہ[16] = (u_char) (ctx->e >> 24);
    نتیجہ[17] = (u_char) (ctx->e >> 16);
    نتیجہ[18] = (u_char) (ctx->e >> 8);
    نتیجہ[19] = (u_char) ctx->e;

    ngx_memzero(ctx, حجم(*ctx));
}


/*
 * Helper functions.
 */

#تعریف_کرو ROTATE(بٹ, لفظ)  (((لفظ) << (بٹ)) | ((لفظ) >> (32 - (بٹ))))

#تعریف_کرو F1(b, c, d)  (((b) & (c)) | ((~(b)) & (d)))
#تعریف_کرو F2(b, c, d)  ((b) ^ (c) ^ (d))
#تعریف_کرو F3(b, c, d)  (((b) & (c)) | ((b) & (d)) | ((c) & (d)))

#تعریف_کرو قدم(f, a, b, c, d, e, w, t)                                          \
    عارضی = ROTATE(5, (a)) + f((b), (c), (d)) + (e) + (w) + (t);               \
    (e) = (d);                                                                \
    (d) = (c);                                                                \
    (c) = ROTATE(30, (b));                                                    \
    (b) = (a);                                                                \
    (a) = عارضی;


/*
 * GET() reads 4 input bytes in big-endian byte order and returns
 * them as uint32_t.
 */

#تعریف_کرو GET(n)                                                                \
    ((uint32_t) p[n * 4 + 3] |                                                \
    ((uint32_t) p[n * 4 + 2] << 8) |                                          \
    ((uint32_t) p[n * 4 + 1] << 16) |                                         \
    ((uint32_t) p[n * 4] << 24))


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

ساكن ثابت u_char *
ngx_sha1_body(ngx_sha1_t *ctx, ثابت u_char *انکڑے, ناپ_t ناپ)
{
    uint32_t       a, b, c, d, e, عارضی;
    uint32_t       saved_a, saved_b, saved_c, saved_d, saved_e;
    uint32_t       الفاظ[80];
    ngx_uint_t     i;
    ثابت u_char  *p;

    p = انکڑے;

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
            الفاظ[i] = GET(i);
        }

        لكل (i = 16; i < 80; i++) {
            الفاظ[i] = ROTATE(1, الفاظ[i - 3] ^ الفاظ[i - 8] ^ الفاظ[i - 14]
                                 ^ الفاظ[i - 16]);
        }

        /* Transformations */

        قدم(F1, a, b, c, d, e, الفاظ[0],  0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[1],  0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[2],  0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[3],  0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[4],  0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[5],  0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[6],  0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[7],  0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[8],  0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[9],  0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[10], 0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[11], 0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[12], 0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[13], 0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[14], 0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[15], 0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[16], 0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[17], 0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[18], 0x5a827999);
        قدم(F1, a, b, c, d, e, الفاظ[19], 0x5a827999);

        قدم(F2, a, b, c, d, e, الفاظ[20], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[21], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[22], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[23], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[24], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[25], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[26], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[27], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[28], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[29], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[30], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[31], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[32], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[33], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[34], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[35], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[36], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[37], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[38], 0x6ed9eba1);
        قدم(F2, a, b, c, d, e, الفاظ[39], 0x6ed9eba1);

        قدم(F3, a, b, c, d, e, الفاظ[40], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[41], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[42], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[43], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[44], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[45], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[46], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[47], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[48], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[49], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[50], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[51], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[52], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[53], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[54], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[55], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[56], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[57], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[58], 0x8f1bbcdc);
        قدم(F3, a, b, c, d, e, الفاظ[59], 0x8f1bbcdc);

        قدم(F2, a, b, c, d, e, الفاظ[60], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[61], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[62], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[63], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[64], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[65], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[66], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[67], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[68], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[69], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[70], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[71], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[72], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[73], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[74], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[75], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[76], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[77], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[78], 0xca62c1d6);
        قدم(F2, a, b, c, d, e, الفاظ[79], 0xca62c1d6);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;
        e += saved_e;

        p += 64;

    } طالما (ناپ -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;
    ctx->e = e;

    أرجع p;
}
