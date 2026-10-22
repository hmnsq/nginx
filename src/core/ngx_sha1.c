
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 *
 * An internal SHA1 implementation.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_sha1.h>


स्थिर स्थिरांक u_char *ngx_sha1_body(ngx_sha1_t *ctx, स्थिरांक u_char *data,
    माप_प्रकार size);


शून्य
ngx_sha1_init(ngx_sha1_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;
    ctx->e = 0xc3d2e1f0;

    ctx->bytes = 0;
}


शून्य
ngx_sha1_update(ngx_sha1_t *ctx, स्थिरांक शून्य *data, माप_प्रकार size)
{
    माप_प्रकार  used, मुक्त;

    used = (माप_प्रकार) (ctx->bytes & 0x3f);
    ctx->bytes += size;

    जर (used) {
        मुक्त = 64 - used;

        जर (size < मुक्त) {
            ngx_memcpy(&ctx->buffer[used], data, size);
            परत;
        }

        ngx_memcpy(&ctx->buffer[used], data, मुक्त);
        data = (u_char *) data + मुक्त;
        size -= मुक्त;
        (शून्य) ngx_sha1_body(ctx, ctx->buffer, 64);
    }

    जर (size >= 64) {
        data = ngx_sha1_body(ctx, data, size & ~(माप_प्रकार) 0x3f);
        size &= 0x3f;
    }

    ngx_memcpy(ctx->buffer, data, size);
}


शून्य
ngx_sha1_final(u_char result[20], ngx_sha1_t *ctx)
{
    माप_प्रकार  used, मुक्त;

    used = (माप_प्रकार) (ctx->bytes & 0x3f);

    ctx->buffer[used++] = 0x80;

    मुक्त = 64 - used;

    जर (मुक्त < 8) {
        ngx_memzero(&ctx->buffer[used], मुक्त);
        (शून्य) ngx_sha1_body(ctx, ctx->buffer, 64);
        used = 0;
        मुक्त = 64;
    }

    ngx_memzero(&ctx->buffer[used], मुक्त - 8);

    ctx->bytes <<= 3;
    ctx->buffer[56] = (u_char) (ctx->bytes >> 56);
    ctx->buffer[57] = (u_char) (ctx->bytes >> 48);
    ctx->buffer[58] = (u_char) (ctx->bytes >> 40);
    ctx->buffer[59] = (u_char) (ctx->bytes >> 32);
    ctx->buffer[60] = (u_char) (ctx->bytes >> 24);
    ctx->buffer[61] = (u_char) (ctx->bytes >> 16);
    ctx->buffer[62] = (u_char) (ctx->bytes >> 8);
    ctx->buffer[63] = (u_char) ctx->bytes;

    (शून्य) ngx_sha1_body(ctx, ctx->buffer, 64);

    result[0] = (u_char) (ctx->a >> 24);
    result[1] = (u_char) (ctx->a >> 16);
    result[2] = (u_char) (ctx->a >> 8);
    result[3] = (u_char) ctx->a;
    result[4] = (u_char) (ctx->b >> 24);
    result[5] = (u_char) (ctx->b >> 16);
    result[6] = (u_char) (ctx->b >> 8);
    result[7] = (u_char) ctx->b;
    result[8] = (u_char) (ctx->c >> 24);
    result[9] = (u_char) (ctx->c >> 16);
    result[10] = (u_char) (ctx->c >> 8);
    result[11] = (u_char) ctx->c;
    result[12] = (u_char) (ctx->d >> 24);
    result[13] = (u_char) (ctx->d >> 16);
    result[14] = (u_char) (ctx->d >> 8);
    result[15] = (u_char) ctx->d;
    result[16] = (u_char) (ctx->e >> 24);
    result[17] = (u_char) (ctx->e >> 16);
    result[18] = (u_char) (ctx->e >> 8);
    result[19] = (u_char) ctx->e;

    ngx_memzero(ctx, आकार(*ctx));
}


/*
 * Helper functions.
 */

#define ROTATE(bits, word)  (((word) << (bits)) | ((word) >> (32 - (bits))))

#define F1(b, c, d)  (((b) & (c)) | ((~(b)) & (d)))
#define F2(b, c, d)  ((b) ^ (c) ^ (d))
#define F3(b, c, d)  (((b) & (c)) | ((b) & (d)) | ((c) & (d)))

#define STEP(f, a, b, c, d, e, w, t)                                          \
    temp = ROTATE(5, (a)) + f((b), (c), (d)) + (e) + (w) + (t);               \
    (e) = (d);                                                                \
    (d) = (c);                                                                \
    (c) = ROTATE(30, (b));                                                    \
    (b) = (a);                                                                \
    (a) = temp;


/*
 * GET() reads 4 input bytes in big-endian byte order and returns
 * them as uint32_t.
 */

#define GET(n)                                                                \
    ((uint32_t) p[n * 4 + 3] |                                                \
    ((uint32_t) p[n * 4 + 2] << 8) |                                          \
    ((uint32_t) p[n * 4 + 1] << 16) |                                         \
    ((uint32_t) p[n * 4] << 24))


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

स्थिर स्थिरांक u_char *
ngx_sha1_body(ngx_sha1_t *ctx, स्थिरांक u_char *data, माप_प्रकार size)
{
    uint32_t       a, b, c, d, e, temp;
    uint32_t       saved_a, saved_b, saved_c, saved_d, saved_e;
    uint32_t       शब्द[80];
    ngx_uint_t     i;
    स्थिरांक u_char  *p;

    p = data;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;
    e = ctx->e;

    करा {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;
        saved_e = e;

        /* Load data block into the words array */

        साठी (i = 0; i < 16; i++) {
            शब्द[i] = GET(i);
        }

        साठी (i = 16; i < 80; i++) {
            शब्द[i] = ROTATE(1, शब्द[i - 3] ^ शब्द[i - 8] ^ शब्द[i - 14]
                                 ^ शब्द[i - 16]);
        }

        /* Transformations */

        STEP(F1, a, b, c, d, e, शब्द[0],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[1],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[2],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[3],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[4],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[5],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[6],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[7],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[8],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[9],  0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[10], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[11], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[12], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[13], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[14], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[15], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[16], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[17], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[18], 0x5a827999);
        STEP(F1, a, b, c, d, e, शब्द[19], 0x5a827999);

        STEP(F2, a, b, c, d, e, शब्द[20], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[21], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[22], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[23], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[24], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[25], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[26], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[27], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[28], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[29], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[30], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[31], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[32], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[33], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[34], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[35], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[36], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[37], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[38], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, शब्द[39], 0x6ed9eba1);

        STEP(F3, a, b, c, d, e, शब्द[40], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[41], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[42], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[43], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[44], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[45], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[46], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[47], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[48], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[49], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[50], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[51], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[52], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[53], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[54], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[55], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[56], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[57], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[58], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, शब्द[59], 0x8f1bbcdc);

        STEP(F2, a, b, c, d, e, शब्द[60], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[61], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[62], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[63], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[64], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[65], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[66], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[67], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[68], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[69], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[70], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[71], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[72], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[73], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[74], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[75], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[76], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[77], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[78], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, शब्द[79], 0xca62c1d6);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;
        e += saved_e;

        p += 64;

    } पर्यंत (size -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;
    ctx->e = e;

    परत p;
}
