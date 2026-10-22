
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
    اندازه_t اندازه);


فراغ
ngx_sha1_init(ngx_sha1_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;
    ctx->e = 0xc3d2e1f0;

    ctx->بایټونه = 0;
}


فراغ
ngx_sha1_update(ngx_sha1_t *ctx, ثابت فراغ *معلومات, اندازه_t اندازه)
{
    اندازه_t  کارول_شوی, حرر;

    کارول_شوی = (اندازه_t) (ctx->بایټونه & 0x3f);
    ctx->بایټونه += اندازه;

    إذا (کارول_شوی) {
        حرر = 64 - کارول_شوی;

        إذا (اندازه < حرر) {
            ngx_memcpy(&ctx->بفر[کارول_شوی], معلومات, اندازه);
            أرجع;
        }

        ngx_memcpy(&ctx->بفر[کارول_شوی], معلومات, حرر);
        معلومات = (u_char *) معلومات + حرر;
        اندازه -= حرر;
        (فراغ) ngx_sha1_body(ctx, ctx->بفر, 64);
    }

    إذا (اندازه >= 64) {
        معلومات = ngx_sha1_body(ctx, معلومات, اندازه & ~(اندازه_t) 0x3f);
        اندازه &= 0x3f;
    }

    ngx_memcpy(ctx->بفر, معلومات, اندازه);
}


فراغ
ngx_sha1_final(u_char پایله[20], ngx_sha1_t *ctx)
{
    اندازه_t  کارول_شوی, حرر;

    کارول_شوی = (اندازه_t) (ctx->بایټونه & 0x3f);

    ctx->بفر[کارول_شوی++] = 0x80;

    حرر = 64 - کارول_شوی;

    إذا (حرر < 8) {
        ngx_memzero(&ctx->بفر[کارول_شوی], حرر);
        (فراغ) ngx_sha1_body(ctx, ctx->بفر, 64);
        کارول_شوی = 0;
        حرر = 64;
    }

    ngx_memzero(&ctx->بفر[کارول_شوی], حرر - 8);

    ctx->بایټونه <<= 3;
    ctx->بفر[56] = (u_char) (ctx->بایټونه >> 56);
    ctx->بفر[57] = (u_char) (ctx->بایټونه >> 48);
    ctx->بفر[58] = (u_char) (ctx->بایټونه >> 40);
    ctx->بفر[59] = (u_char) (ctx->بایټونه >> 32);
    ctx->بفر[60] = (u_char) (ctx->بایټونه >> 24);
    ctx->بفر[61] = (u_char) (ctx->بایټونه >> 16);
    ctx->بفر[62] = (u_char) (ctx->بایټونه >> 8);
    ctx->بفر[63] = (u_char) ctx->بایټونه;

    (فراغ) ngx_sha1_body(ctx, ctx->بفر, 64);

    پایله[0] = (u_char) (ctx->a >> 24);
    پایله[1] = (u_char) (ctx->a >> 16);
    پایله[2] = (u_char) (ctx->a >> 8);
    پایله[3] = (u_char) ctx->a;
    پایله[4] = (u_char) (ctx->b >> 24);
    پایله[5] = (u_char) (ctx->b >> 16);
    پایله[6] = (u_char) (ctx->b >> 8);
    پایله[7] = (u_char) ctx->b;
    پایله[8] = (u_char) (ctx->c >> 24);
    پایله[9] = (u_char) (ctx->c >> 16);
    پایله[10] = (u_char) (ctx->c >> 8);
    پایله[11] = (u_char) ctx->c;
    پایله[12] = (u_char) (ctx->d >> 24);
    پایله[13] = (u_char) (ctx->d >> 16);
    پایله[14] = (u_char) (ctx->d >> 8);
    پایله[15] = (u_char) ctx->d;
    پایله[16] = (u_char) (ctx->e >> 24);
    پایله[17] = (u_char) (ctx->e >> 16);
    پایله[18] = (u_char) (ctx->e >> 8);
    پایله[19] = (u_char) ctx->e;

    ngx_memzero(ctx, حجم(*ctx));
}


/*
 * Helper functions.
 */

#تعریف_کړه ROTATE(بټونه, کلمه)  (((کلمه) << (بټونه)) | ((کلمه) >> (32 - (بټونه))))

#تعریف_کړه F1(b, c, d)  (((b) & (c)) | ((~(b)) & (d)))
#تعریف_کړه F2(b, c, d)  ((b) ^ (c) ^ (d))
#تعریف_کړه F3(b, c, d)  (((b) & (c)) | ((b) & (d)) | ((c) & (d)))

#تعریف_کړه ګام(f, a, b, c, d, e, w, t)                                          \
    لنډمهاله = ROTATE(5, (a)) + f((b), (c), (d)) + (e) + (w) + (t);               \
    (e) = (d);                                                                \
    (d) = (c);                                                                \
    (c) = ROTATE(30, (b));                                                    \
    (b) = (a);                                                                \
    (a) = لنډمهاله;


/*
 * GET() reads 4 input bytes in big-endian byte order and returns
 * them as uint32_t.
 */

#تعریف_کړه GET(n)                                                                \
    ((uint32_t) p[n * 4 + 3] |                                                \
    ((uint32_t) p[n * 4 + 2] << 8) |                                          \
    ((uint32_t) p[n * 4 + 1] << 16) |                                         \
    ((uint32_t) p[n * 4] << 24))


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

ساكن ثابت u_char *
ngx_sha1_body(ngx_sha1_t *ctx, ثابت u_char *معلومات, اندازه_t اندازه)
{
    uint32_t       a, b, c, d, e, لنډمهاله;
    uint32_t       saved_a, saved_b, saved_c, saved_d, saved_e;
    uint32_t       ټکي[80];
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
            ټکي[i] = GET(i);
        }

        لكل (i = 16; i < 80; i++) {
            ټکي[i] = ROTATE(1, ټکي[i - 3] ^ ټکي[i - 8] ^ ټکي[i - 14]
                                 ^ ټکي[i - 16]);
        }

        /* Transformations */

        ګام(F1, a, b, c, d, e, ټکي[0],  0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[1],  0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[2],  0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[3],  0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[4],  0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[5],  0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[6],  0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[7],  0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[8],  0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[9],  0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[10], 0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[11], 0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[12], 0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[13], 0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[14], 0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[15], 0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[16], 0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[17], 0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[18], 0x5a827999);
        ګام(F1, a, b, c, d, e, ټکي[19], 0x5a827999);

        ګام(F2, a, b, c, d, e, ټکي[20], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[21], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[22], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[23], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[24], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[25], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[26], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[27], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[28], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[29], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[30], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[31], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[32], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[33], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[34], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[35], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[36], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[37], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[38], 0x6ed9eba1);
        ګام(F2, a, b, c, d, e, ټکي[39], 0x6ed9eba1);

        ګام(F3, a, b, c, d, e, ټکي[40], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[41], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[42], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[43], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[44], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[45], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[46], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[47], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[48], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[49], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[50], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[51], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[52], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[53], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[54], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[55], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[56], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[57], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[58], 0x8f1bbcdc);
        ګام(F3, a, b, c, d, e, ټکي[59], 0x8f1bbcdc);

        ګام(F2, a, b, c, d, e, ټکي[60], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[61], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[62], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[63], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[64], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[65], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[66], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[67], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[68], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[69], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[70], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[71], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[72], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[73], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[74], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[75], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[76], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[77], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[78], 0xca62c1d6);
        ګام(F2, a, b, c, d, e, ټکي[79], 0xca62c1d6);

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
