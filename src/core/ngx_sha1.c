
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 *
 * An internal SHA1 implementation.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>
#समावेश <ngx_sha1.h>


अटल स्थिर u_char *ngx_sha1_body(ngx_sha1_t *ctx, स्थिर u_char *तथ्य,
    माप_प्रकार आकार);


व्योम
ngx_sha1_init(ngx_sha1_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;
    ctx->e = 0xc3d2e1f0;

    ctx->bytes = 0;
}


व्योम
ngx_sha1_update(ngx_sha1_t *ctx, स्थिर व्योम *तथ्य, माप_प्रकार आकार)
{
    माप_प्रकार  used, मुक्त;

    used = (माप_प्रकार) (ctx->bytes & 0x3f);
    ctx->bytes += आकार;

    अगर (used) {
        मुक्त = 64 - used;

        अगर (आकार < मुक्त) {
            ngx_memcpy(&ctx->बफर[used], तथ्य, आकार);
            वापस;
        }

        ngx_memcpy(&ctx->बफर[used], तथ्य, मुक्त);
        तथ्य = (u_char *) तथ्य + मुक्त;
        आकार -= मुक्त;
        (व्योम) ngx_sha1_body(ctx, ctx->बफर, 64);
    }

    अगर (आकार >= 64) {
        तथ्य = ngx_sha1_body(ctx, तथ्य, आकार & ~(माप_प्रकार) 0x3f);
        आकार &= 0x3f;
    }

    ngx_memcpy(ctx->बफर, तथ्य, आकार);
}


व्योम
ngx_sha1_final(u_char परिणाम[20], ngx_sha1_t *ctx)
{
    माप_प्रकार  used, मुक्त;

    used = (माप_प्रकार) (ctx->bytes & 0x3f);

    ctx->बफर[used++] = 0x80;

    मुक्त = 64 - used;

    अगर (मुक्त < 8) {
        ngx_memzero(&ctx->बफर[used], मुक्त);
        (व्योम) ngx_sha1_body(ctx, ctx->बफर, 64);
        used = 0;
        मुक्त = 64;
    }

    ngx_memzero(&ctx->बफर[used], मुक्त - 8);

    ctx->bytes <<= 3;
    ctx->बफर[56] = (u_char) (ctx->bytes >> 56);
    ctx->बफर[57] = (u_char) (ctx->bytes >> 48);
    ctx->बफर[58] = (u_char) (ctx->bytes >> 40);
    ctx->बफर[59] = (u_char) (ctx->bytes >> 32);
    ctx->बफर[60] = (u_char) (ctx->bytes >> 24);
    ctx->बफर[61] = (u_char) (ctx->bytes >> 16);
    ctx->बफर[62] = (u_char) (ctx->bytes >> 8);
    ctx->बफर[63] = (u_char) ctx->bytes;

    (व्योम) ngx_sha1_body(ctx, ctx->बफर, 64);

    परिणाम[0] = (u_char) (ctx->a >> 24);
    परिणाम[1] = (u_char) (ctx->a >> 16);
    परिणाम[2] = (u_char) (ctx->a >> 8);
    परिणाम[3] = (u_char) ctx->a;
    परिणाम[4] = (u_char) (ctx->b >> 24);
    परिणाम[5] = (u_char) (ctx->b >> 16);
    परिणाम[6] = (u_char) (ctx->b >> 8);
    परिणाम[7] = (u_char) ctx->b;
    परिणाम[8] = (u_char) (ctx->c >> 24);
    परिणाम[9] = (u_char) (ctx->c >> 16);
    परिणाम[10] = (u_char) (ctx->c >> 8);
    परिणाम[11] = (u_char) ctx->c;
    परिणाम[12] = (u_char) (ctx->d >> 24);
    परिणाम[13] = (u_char) (ctx->d >> 16);
    परिणाम[14] = (u_char) (ctx->d >> 8);
    परिणाम[15] = (u_char) ctx->d;
    परिणाम[16] = (u_char) (ctx->e >> 24);
    परिणाम[17] = (u_char) (ctx->e >> 16);
    परिणाम[18] = (u_char) (ctx->e >> 8);
    परिणाम[19] = (u_char) ctx->e;

    ngx_memzero(ctx, माप(*ctx));
}


/*
 * Helper functions.
 */

#घोषणा ROTATE(bits, word)  (((word) << (bits)) | ((word) >> (32 - (bits))))

#घोषणा F1(b, c, d)  (((b) & (c)) | ((~(b)) & (d)))
#घोषणा F2(b, c, d)  ((b) ^ (c) ^ (d))
#घोषणा F3(b, c, d)  (((b) & (c)) | ((b) & (d)) | ((c) & (d)))

#घोषणा STEP(f, a, b, c, d, e, w, t)                                          \
    अस्थायी = ROTATE(5, (a)) + f((b), (c), (d)) + (e) + (w) + (t);               \
    (e) = (d);                                                                \
    (d) = (c);                                                                \
    (c) = ROTATE(30, (b));                                                    \
    (b) = (a);                                                                \
    (a) = अस्थायी;


/*
 * GET() reads 4 input bytes in big-endian byte order and returns
 * them as uint32_t.
 */

#घोषणा GET(n)                                                                \
    ((uint32_t) p[n * 4 + 3] |                                                \
    ((uint32_t) p[n * 4 + 2] << 8) |                                          \
    ((uint32_t) p[n * 4 + 1] << 16) |                                         \
    ((uint32_t) p[n * 4] << 24))


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

अटल स्थिर u_char *
ngx_sha1_body(ngx_sha1_t *ctx, स्थिर u_char *तथ्य, माप_प्रकार आकार)
{
    uint32_t       a, b, c, d, e, अस्थायी;
    uint32_t       saved_a, saved_b, saved_c, saved_d, saved_e;
    uint32_t       शब्दगण[80];
    ngx_uint_t     i;
    स्थिर u_char  *p;

    p = तथ्य;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;
    e = ctx->e;

    करो {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;
        saved_e = e;

        /* Load data block into the words array */

        क्रम (i = 0; i < 16; i++) {
            शब्दगण[i] = GET(i);
        }

        क्रम (i = 16; i < 80; i++) {
            शब्दगण[i] = ROTATE(1, शब्दगण[i - 3] ^ शब्दगण[i - 8] ^ शब्दगण[i - 14]
                                 ^ शब्दगण[i - 16]);
        }

        /* Transformations */

        STEP(F1, a, b, c, d, e, शब्दगण[0],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[1],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[2],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[3],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[4],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[5],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[6],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[7],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[8],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[9],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[10], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[11], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[12], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[13], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[14], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[15], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[16], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[17], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[18], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्दगण[19], 0x5a827999);

        STEP(F2, a, b, c, d, e, शब्दगण[20], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[21], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[22], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[23], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[24], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[25], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[26], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[27], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[28], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[29], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[30], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[31], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[32], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[33], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[34], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[35], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[36], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[37], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[38], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्दगण[39], 0x6ed9eba1);

        STEP(F3, a, b, c, d, e, शब्दगण[40], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[41], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[42], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[43], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[44], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[45], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[46], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[47], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[48], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[49], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[50], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[51], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[52], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[53], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[54], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[55], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[56], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[57], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[58], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्दगण[59], 0x8f1bbcdc);

        STEP(F2, a, b, c, d, e, शब्दगण[60], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[61], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[62], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[63], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[64], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[65], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[66], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[67], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[68], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[69], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[70], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[71], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[72], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[73], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[74], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[75], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[76], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[77], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[78], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्दगण[79], 0xca62c1d6);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;
        e += saved_e;

        p += 64;

    } जबतक (आकार -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;
    ctx->e = e;

    वापस p;
}
