
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 *
 * An internal SHA1 implementation.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>
#ଅନ୍ତର୍ଭାଵ <ngx_sha1.h>


ସ୍ଥିର ଅଚଳ u_char *ngx_sha1_body(ngx_sha1_t *ctx, ଅଚଳ u_char *data,
    size_t size);


ଶୂନ୍ୟ
ngx_sha1_init(ngx_sha1_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;
    ctx->e = 0xc3d2e1f0;

    ctx->bytes = 0;
}


ଶୂନ୍ୟ
ngx_sha1_update(ngx_sha1_t *ctx, ଅଚଳ ଶୂନ୍ୟ *data, size_t size)
{
    size_t  used, ମୁକ୍ତ;

    used = (size_t) (ctx->bytes & 0x3f);
    ctx->bytes += size;

    ଯଦି (used) {
        ମୁକ୍ତ = 64 - used;

        ଯଦି (size < ମୁକ୍ତ) {
            ngx_memcpy(&ctx->buffer[used], data, size);
            ଫେରନ୍ତୁ;
        }

        ngx_memcpy(&ctx->buffer[used], data, ମୁକ୍ତ);
        data = (u_char *) data + ମୁକ୍ତ;
        size -= ମୁକ୍ତ;
        (ଶୂନ୍ୟ) ngx_sha1_body(ctx, ctx->buffer, 64);
    }

    ଯଦି (size >= 64) {
        data = ngx_sha1_body(ctx, data, size & ~(size_t) 0x3f);
        size &= 0x3f;
    }

    ngx_memcpy(ctx->buffer, data, size);
}


ଶୂନ୍ୟ
ngx_sha1_final(u_char result[20], ngx_sha1_t *ctx)
{
    size_t  used, ମୁକ୍ତ;

    used = (size_t) (ctx->bytes & 0x3f);

    ctx->buffer[used++] = 0x80;

    ମୁକ୍ତ = 64 - used;

    ଯଦି (ମୁକ୍ତ < 8) {
        ngx_memzero(&ctx->buffer[used], ମୁକ୍ତ);
        (ଶୂନ୍ୟ) ngx_sha1_body(ctx, ctx->buffer, 64);
        used = 0;
        ମୁକ୍ତ = 64;
    }

    ngx_memzero(&ctx->buffer[used], ମୁକ୍ତ - 8);

    ctx->bytes <<= 3;
    ctx->buffer[56] = (u_char) (ctx->bytes >> 56);
    ctx->buffer[57] = (u_char) (ctx->bytes >> 48);
    ctx->buffer[58] = (u_char) (ctx->bytes >> 40);
    ctx->buffer[59] = (u_char) (ctx->bytes >> 32);
    ctx->buffer[60] = (u_char) (ctx->bytes >> 24);
    ctx->buffer[61] = (u_char) (ctx->bytes >> 16);
    ctx->buffer[62] = (u_char) (ctx->bytes >> 8);
    ctx->buffer[63] = (u_char) ctx->bytes;

    (ଶୂନ୍ୟ) ngx_sha1_body(ctx, ctx->buffer, 64);

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

    ngx_memzero(ctx, ଆକାର(*ctx));
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

ସ୍ଥିର ଅଚଳ u_char *
ngx_sha1_body(ngx_sha1_t *ctx, ଅଚଳ u_char *data, size_t size)
{
    uint32_t       a, b, c, d, e, temp;
    uint32_t       saved_a, saved_b, saved_c, saved_d, saved_e;
    uint32_t       ଶବ୍ଦଗୁଡ଼ିକ[80];
    ngx_uint_t     i;
    ଅଚଳ u_char  *p;

    p = data;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;
    e = ctx->e;

    କର {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;
        saved_e = e;

        /* Load data block into the words array */

        ପାଇଁ (i = 0; i < 16; i++) {
            ଶବ୍ଦଗୁଡ଼ିକ[i] = GET(i);
        }

        ପାଇଁ (i = 16; i < 80; i++) {
            ଶବ୍ଦଗୁଡ଼ିକ[i] = ROTATE(1, ଶବ୍ଦଗୁଡ଼ିକ[i - 3] ^ ଶବ୍ଦଗୁଡ଼ିକ[i - 8] ^ ଶବ୍ଦଗୁଡ଼ିକ[i - 14]
                                 ^ ଶବ୍ଦଗୁଡ଼ିକ[i - 16]);
        }

        /* Transformations */

        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[0],  0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[1],  0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[2],  0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[3],  0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[4],  0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[5],  0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[6],  0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[7],  0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[8],  0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[9],  0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[10], 0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[11], 0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[12], 0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[13], 0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[14], 0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[15], 0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[16], 0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[17], 0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[18], 0x5a827999);
        STEP(F1, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[19], 0x5a827999);

        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[20], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[21], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[22], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[23], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[24], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[25], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[26], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[27], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[28], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[29], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[30], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[31], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[32], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[33], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[34], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[35], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[36], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[37], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[38], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[39], 0x6ed9eba1);

        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[40], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[41], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[42], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[43], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[44], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[45], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[46], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[47], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[48], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[49], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[50], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[51], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[52], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[53], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[54], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[55], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[56], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[57], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[58], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[59], 0x8f1bbcdc);

        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[60], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[61], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[62], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[63], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[64], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[65], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[66], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[67], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[68], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[69], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[70], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[71], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[72], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[73], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[74], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[75], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[76], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[77], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[78], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, ଶବ୍ଦଗୁଡ଼ିକ[79], 0xca62c1d6);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;
        e += saved_e;

        p += 64;

    } ଯେତେବେଳେ (size -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;
    ctx->e = e;

    ଫେରନ୍ତୁ p;
}
