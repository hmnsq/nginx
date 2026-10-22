
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 *
 * An internal SHA1 implementation.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_sha1.h>


ਸਥਿਰ ਅਚਲ u_char *ngx_sha1_body(ngx_sha1_t *ctx, ਅਚਲ u_char *ਅੰਕੜੇ,
    ਮਾਪ_t ਮਾਪ);


ਖਾਲੀ
ngx_sha1_init(ngx_sha1_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;
    ctx->e = 0xc3d2e1f0;

    ctx->ਬਾਈਟਾਂ = 0;
}


ਖਾਲੀ
ngx_sha1_update(ngx_sha1_t *ctx, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਮਾਪ_t ਮਾਪ)
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
        (ਖਾਲੀ) ngx_sha1_body(ctx, ctx->ਬਫਰ, 64);
    }

    ਜੇ (ਮਾਪ >= 64) {
        ਅੰਕੜੇ = ngx_sha1_body(ctx, ਅੰਕੜੇ, ਮਾਪ & ~(ਮਾਪ_t) 0x3f);
        ਮਾਪ &= 0x3f;
    }

    ngx_memcpy(ctx->ਬਫਰ, ਅੰਕੜੇ, ਮਾਪ);
}


ਖਾਲੀ
ngx_sha1_final(u_char ਨਤੀਜਾ[20], ngx_sha1_t *ctx)
{
    ਮਾਪ_t  ਵਰਤਿਆ, ਮੁਕਤ;

    ਵਰਤਿਆ = (ਮਾਪ_t) (ctx->ਬਾਈਟਾਂ & 0x3f);

    ctx->ਬਫਰ[ਵਰਤਿਆ++] = 0x80;

    ਮੁਕਤ = 64 - ਵਰਤਿਆ;

    ਜੇ (ਮੁਕਤ < 8) {
        ngx_memzero(&ctx->ਬਫਰ[ਵਰਤਿਆ], ਮੁਕਤ);
        (ਖਾਲੀ) ngx_sha1_body(ctx, ctx->ਬਫਰ, 64);
        ਵਰਤਿਆ = 0;
        ਮੁਕਤ = 64;
    }

    ngx_memzero(&ctx->ਬਫਰ[ਵਰਤਿਆ], ਮੁਕਤ - 8);

    ctx->ਬਾਈਟਾਂ <<= 3;
    ctx->ਬਫਰ[56] = (u_char) (ctx->ਬਾਈਟਾਂ >> 56);
    ctx->ਬਫਰ[57] = (u_char) (ctx->ਬਾਈਟਾਂ >> 48);
    ctx->ਬਫਰ[58] = (u_char) (ctx->ਬਾਈਟਾਂ >> 40);
    ctx->ਬਫਰ[59] = (u_char) (ctx->ਬਾਈਟਾਂ >> 32);
    ctx->ਬਫਰ[60] = (u_char) (ctx->ਬਾਈਟਾਂ >> 24);
    ctx->ਬਫਰ[61] = (u_char) (ctx->ਬਾਈਟਾਂ >> 16);
    ctx->ਬਫਰ[62] = (u_char) (ctx->ਬਾਈਟਾਂ >> 8);
    ctx->ਬਫਰ[63] = (u_char) ctx->ਬਾਈਟਾਂ;

    (ਖਾਲੀ) ngx_sha1_body(ctx, ctx->ਬਫਰ, 64);

    ਨਤੀਜਾ[0] = (u_char) (ctx->a >> 24);
    ਨਤੀਜਾ[1] = (u_char) (ctx->a >> 16);
    ਨਤੀਜਾ[2] = (u_char) (ctx->a >> 8);
    ਨਤੀਜਾ[3] = (u_char) ctx->a;
    ਨਤੀਜਾ[4] = (u_char) (ctx->b >> 24);
    ਨਤੀਜਾ[5] = (u_char) (ctx->b >> 16);
    ਨਤੀਜਾ[6] = (u_char) (ctx->b >> 8);
    ਨਤੀਜਾ[7] = (u_char) ctx->b;
    ਨਤੀਜਾ[8] = (u_char) (ctx->c >> 24);
    ਨਤੀਜਾ[9] = (u_char) (ctx->c >> 16);
    ਨਤੀਜਾ[10] = (u_char) (ctx->c >> 8);
    ਨਤੀਜਾ[11] = (u_char) ctx->c;
    ਨਤੀਜਾ[12] = (u_char) (ctx->d >> 24);
    ਨਤੀਜਾ[13] = (u_char) (ctx->d >> 16);
    ਨਤੀਜਾ[14] = (u_char) (ctx->d >> 8);
    ਨਤੀਜਾ[15] = (u_char) ctx->d;
    ਨਤੀਜਾ[16] = (u_char) (ctx->e >> 24);
    ਨਤੀਜਾ[17] = (u_char) (ctx->e >> 16);
    ਨਤੀਜਾ[18] = (u_char) (ctx->e >> 8);
    ਨਤੀਜਾ[19] = (u_char) ctx->e;

    ngx_memzero(ctx, ਆਕਾਰ(*ctx));
}


/*
 * Helper functions.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ROTATE(ਬਿੱਟ, ਸ਼ਬਦ)  (((ਸ਼ਬਦ) << (ਬਿੱਟ)) | ((ਸ਼ਬਦ) >> (32 - (ਬਿੱਟ))))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ F1(b, c, d)  (((b) & (c)) | ((~(b)) & (d)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ F2(b, c, d)  ((b) ^ (c) ^ (d))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ F3(b, c, d)  (((b) & (c)) | ((b) & (d)) | ((c) & (d)))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਦਮ(f, a, b, c, d, e, w, t)                                          \
    ਆਰਜ਼ੀ = ROTATE(5, (a)) + f((b), (c), (d)) + (e) + (w) + (t);               \
    (e) = (d);                                                                \
    (d) = (c);                                                                \
    (c) = ROTATE(30, (b));                                                    \
    (b) = (a);                                                                \
    (a) = ਆਰਜ਼ੀ;


/*
 * GET() reads 4 input bytes in big-endian byte order and returns
 * them as uint32_t.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET(n)                                                                \
    ((uint32_t) p[n * 4 + 3] |                                                \
    ((uint32_t) p[n * 4 + 2] << 8) |                                          \
    ((uint32_t) p[n * 4 + 1] << 16) |                                         \
    ((uint32_t) p[n * 4] << 24))


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

ਸਥਿਰ ਅਚਲ u_char *
ngx_sha1_body(ngx_sha1_t *ctx, ਅਚਲ u_char *ਅੰਕੜੇ, ਮਾਪ_t ਮਾਪ)
{
    uint32_t       a, b, c, d, e, ਆਰਜ਼ੀ;
    uint32_t       saved_a, saved_b, saved_c, saved_d, saved_e;
    uint32_t       ਸ਼ਬਦ_2[80];
    ngx_uint_t     i;
    ਅਚਲ u_char  *p;

    p = ਅੰਕੜੇ;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;
    e = ctx->e;

    ਕਰੋ {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;
        saved_e = e;

        /* Load data block into the words array */

        ਲਈ (i = 0; i < 16; i++) {
            ਸ਼ਬਦ_2[i] = GET(i);
        }

        ਲਈ (i = 16; i < 80; i++) {
            ਸ਼ਬਦ_2[i] = ROTATE(1, ਸ਼ਬਦ_2[i - 3] ^ ਸ਼ਬਦ_2[i - 8] ^ ਸ਼ਬਦ_2[i - 14]
                                 ^ ਸ਼ਬਦ_2[i - 16]);
        }

        /* Transformations */

        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[0],  0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[1],  0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[2],  0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[3],  0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[4],  0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[5],  0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[6],  0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[7],  0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[8],  0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[9],  0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[10], 0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[11], 0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[12], 0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[13], 0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[14], 0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[15], 0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[16], 0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[17], 0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[18], 0x5a827999);
        ਕਦਮ(F1, a, b, c, d, e, ਸ਼ਬਦ_2[19], 0x5a827999);

        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[20], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[21], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[22], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[23], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[24], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[25], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[26], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[27], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[28], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[29], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[30], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[31], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[32], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[33], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[34], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[35], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[36], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[37], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[38], 0x6ed9eba1);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[39], 0x6ed9eba1);

        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[40], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[41], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[42], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[43], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[44], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[45], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[46], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[47], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[48], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[49], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[50], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[51], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[52], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[53], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[54], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[55], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[56], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[57], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[58], 0x8f1bbcdc);
        ਕਦਮ(F3, a, b, c, d, e, ਸ਼ਬਦ_2[59], 0x8f1bbcdc);

        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[60], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[61], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[62], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[63], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[64], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[65], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[66], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[67], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[68], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[69], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[70], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[71], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[72], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[73], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[74], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[75], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[76], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[77], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[78], 0xca62c1d6);
        ਕਦਮ(F2, a, b, c, d, e, ਸ਼ਬਦ_2[79], 0xca62c1d6);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;
        e += saved_e;

        p += 64;

    } ਜਦੋਂ (ਮਾਪ -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;
    ctx->e = e;

    ਵਾਪਸ p;
}
