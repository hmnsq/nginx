
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 *
 * An internal SHA1 implementation.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_sha1.h>


ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ u_char *ngx_sha1_body(ngx_sha1_t *ctx, ಸ್ಥಿರಾಂಕ u_char *ದತ್ತಾಂಶ,
    ಗಾತ್ರ_t ಗಾತ್ರ_2);


ಶೂನ್ಯ
ngx_sha1_init(ngx_sha1_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;
    ctx->e = 0xc3d2e1f0;

    ctx->ಬೈಟುಗಳು = 0;
}


ಶೂನ್ಯ
ngx_sha1_update(ngx_sha1_t *ctx, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, ಗಾತ್ರ_t ಗಾತ್ರ_2)
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
        (ಶೂನ್ಯ) ngx_sha1_body(ctx, ctx->ಬಫರ್, 64);
    }

    ಆದರೆ (ಗಾತ್ರ_2 >= 64) {
        ದತ್ತಾಂಶ = ngx_sha1_body(ctx, ದತ್ತಾಂಶ, ಗಾತ್ರ_2 & ~(ಗಾತ್ರ_t) 0x3f);
        ಗಾತ್ರ_2 &= 0x3f;
    }

    ngx_memcpy(ctx->ಬಫರ್, ದತ್ತಾಂಶ, ಗಾತ್ರ_2);
}


ಶೂನ್ಯ
ngx_sha1_final(u_char ಫಲಿತಾಂಶ[20], ngx_sha1_t *ctx)
{
    ಗಾತ್ರ_t  ಬಳಸಿದ, ಬಿಡುಗಡೆ;

    ಬಳಸಿದ = (ಗಾತ್ರ_t) (ctx->ಬೈಟುಗಳು & 0x3f);

    ctx->ಬಫರ್[ಬಳಸಿದ++] = 0x80;

    ಬಿಡುಗಡೆ = 64 - ಬಳಸಿದ;

    ಆದರೆ (ಬಿಡುಗಡೆ < 8) {
        ngx_memzero(&ctx->ಬಫರ್[ಬಳಸಿದ], ಬಿಡುಗಡೆ);
        (ಶೂನ್ಯ) ngx_sha1_body(ctx, ctx->ಬಫರ್, 64);
        ಬಳಸಿದ = 0;
        ಬಿಡುಗಡೆ = 64;
    }

    ngx_memzero(&ctx->ಬಫರ್[ಬಳಸಿದ], ಬಿಡುಗಡೆ - 8);

    ctx->ಬೈಟುಗಳು <<= 3;
    ctx->ಬಫರ್[56] = (u_char) (ctx->ಬೈಟುಗಳು >> 56);
    ctx->ಬಫರ್[57] = (u_char) (ctx->ಬೈಟುಗಳು >> 48);
    ctx->ಬಫರ್[58] = (u_char) (ctx->ಬೈಟುಗಳು >> 40);
    ctx->ಬಫರ್[59] = (u_char) (ctx->ಬೈಟುಗಳು >> 32);
    ctx->ಬಫರ್[60] = (u_char) (ctx->ಬೈಟುಗಳು >> 24);
    ctx->ಬಫರ್[61] = (u_char) (ctx->ಬೈಟುಗಳು >> 16);
    ctx->ಬಫರ್[62] = (u_char) (ctx->ಬೈಟುಗಳು >> 8);
    ctx->ಬಫರ್[63] = (u_char) ctx->ಬೈಟುಗಳು;

    (ಶೂನ್ಯ) ngx_sha1_body(ctx, ctx->ಬಫರ್, 64);

    ಫಲಿತಾಂಶ[0] = (u_char) (ctx->a >> 24);
    ಫಲಿತಾಂಶ[1] = (u_char) (ctx->a >> 16);
    ಫಲಿತಾಂಶ[2] = (u_char) (ctx->a >> 8);
    ಫಲಿತಾಂಶ[3] = (u_char) ctx->a;
    ಫಲಿತಾಂಶ[4] = (u_char) (ctx->b >> 24);
    ಫಲಿತಾಂಶ[5] = (u_char) (ctx->b >> 16);
    ಫಲಿತಾಂಶ[6] = (u_char) (ctx->b >> 8);
    ಫಲಿತಾಂಶ[7] = (u_char) ctx->b;
    ಫಲಿತಾಂಶ[8] = (u_char) (ctx->c >> 24);
    ಫಲಿತಾಂಶ[9] = (u_char) (ctx->c >> 16);
    ಫಲಿತಾಂಶ[10] = (u_char) (ctx->c >> 8);
    ಫಲಿತಾಂಶ[11] = (u_char) ctx->c;
    ಫಲಿತಾಂಶ[12] = (u_char) (ctx->d >> 24);
    ಫಲಿತಾಂಶ[13] = (u_char) (ctx->d >> 16);
    ಫಲಿತಾಂಶ[14] = (u_char) (ctx->d >> 8);
    ಫಲಿತಾಂಶ[15] = (u_char) ctx->d;
    ಫಲಿತಾಂಶ[16] = (u_char) (ctx->e >> 24);
    ಫಲಿತಾಂಶ[17] = (u_char) (ctx->e >> 16);
    ಫಲಿತಾಂಶ[18] = (u_char) (ctx->e >> 8);
    ಫಲಿತಾಂಶ[19] = (u_char) ctx->e;

    ngx_memzero(ctx, ಗಾತ್ರ(*ctx));
}


/*
 * Helper functions.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು ROTATE(ಬಿಟ್ಟುಗಳು, ಶಬ್ದ)  (((ಶಬ್ದ) << (ಬಿಟ್ಟುಗಳು)) | ((ಶಬ್ದ) >> (32 - (ಬಿಟ್ಟುಗಳು))))

#ವ್ಯಾಖ್ಯಾನಿಸು F1(b, c, d)  (((b) & (c)) | ((~(b)) & (d)))
#ವ್ಯಾಖ್ಯಾನಿಸು F2(b, c, d)  ((b) ^ (c) ^ (d))
#ವ್ಯಾಖ್ಯಾನಿಸು F3(b, c, d)  (((b) & (c)) | ((b) & (d)) | ((c) & (d)))

#ವ್ಯಾಖ್ಯಾನಿಸು ಹೆಜ್ಜೆ(f, a, b, c, d, e, w, t)                                          \
    ತಾತ್ಕಾಲಿಕ = ROTATE(5, (a)) + f((b), (c), (d)) + (e) + (w) + (t);               \
    (e) = (d);                                                                \
    (d) = (c);                                                                \
    (c) = ROTATE(30, (b));                                                    \
    (b) = (a);                                                                \
    (a) = ತಾತ್ಕಾಲಿಕ;


/*
 * GET() reads 4 input bytes in big-endian byte order and returns
 * them as uint32_t.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು GET(n)                                                                \
    ((uint32_t) p[n * 4 + 3] |                                                \
    ((uint32_t) p[n * 4 + 2] << 8) |                                          \
    ((uint32_t) p[n * 4 + 1] << 16) |                                         \
    ((uint32_t) p[n * 4] << 24))


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ u_char *
ngx_sha1_body(ngx_sha1_t *ctx, ಸ್ಥಿರಾಂಕ u_char *ದತ್ತಾಂಶ, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    uint32_t       a, b, c, d, e, ತಾತ್ಕಾಲಿಕ;
    uint32_t       saved_a, saved_b, saved_c, saved_d, saved_e;
    uint32_t       ಪದಗಳು[80];
    ngx_uint_t     i;
    ಸ್ಥಿರಾಂಕ u_char  *p;

    p = ದತ್ತಾಂಶ;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;
    e = ctx->e;

    ಮಾಡು {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;
        saved_e = e;

        /* Load data block into the words array */

        ಗಾಗಿ (i = 0; i < 16; i++) {
            ಪದಗಳು[i] = GET(i);
        }

        ಗಾಗಿ (i = 16; i < 80; i++) {
            ಪದಗಳು[i] = ROTATE(1, ಪದಗಳು[i - 3] ^ ಪದಗಳು[i - 8] ^ ಪದಗಳು[i - 14]
                                 ^ ಪದಗಳು[i - 16]);
        }

        /* Transformations */

        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[0],  0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[1],  0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[2],  0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[3],  0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[4],  0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[5],  0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[6],  0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[7],  0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[8],  0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[9],  0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[10], 0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[11], 0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[12], 0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[13], 0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[14], 0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[15], 0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[16], 0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[17], 0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[18], 0x5a827999);
        ಹೆಜ್ಜೆ(F1, a, b, c, d, e, ಪದಗಳು[19], 0x5a827999);

        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[20], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[21], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[22], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[23], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[24], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[25], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[26], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[27], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[28], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[29], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[30], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[31], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[32], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[33], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[34], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[35], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[36], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[37], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[38], 0x6ed9eba1);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[39], 0x6ed9eba1);

        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[40], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[41], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[42], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[43], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[44], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[45], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[46], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[47], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[48], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[49], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[50], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[51], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[52], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[53], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[54], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[55], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[56], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[57], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[58], 0x8f1bbcdc);
        ಹೆಜ್ಜೆ(F3, a, b, c, d, e, ಪದಗಳು[59], 0x8f1bbcdc);

        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[60], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[61], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[62], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[63], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[64], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[65], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[66], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[67], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[68], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[69], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[70], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[71], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[72], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[73], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[74], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[75], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[76], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[77], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[78], 0xca62c1d6);
        ಹೆಜ್ಜೆ(F2, a, b, c, d, e, ಪದಗಳು[79], 0xca62c1d6);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;
        e += saved_e;

        p += 64;

    } ಆಗ (ಗಾತ್ರ_2 -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;
    ctx->e = e;

    ಹಿಂತಿರುಗಿ p;
}
