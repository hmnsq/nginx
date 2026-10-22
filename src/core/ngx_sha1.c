
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 *
 * An internal SHA1 implementation.
 */


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>
#অন্তর্ভাৱ <ngx_sha1.h>


স্থির ধ্রুৱ u_char *ngx_sha1_body(ngx_sha1_t *ctx, ধ্রুৱ u_char *data,
    size_t size);


শূন্য
ngx_sha1_init(ngx_sha1_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;
    ctx->e = 0xc3d2e1f0;

    ctx->bytes = 0;
}


শূন্য
ngx_sha1_update(ngx_sha1_t *ctx, ধ্রুৱ শূন্য *data, size_t size)
{
    size_t  used, মুক্ত;

    used = (size_t) (ctx->bytes & 0x3f);
    ctx->bytes += size;

    যদি (used) {
        মুক্ত = 64 - used;

        যদি (size < মুক্ত) {
            ngx_memcpy(&ctx->buffer[used], data, size);
            ঘূৰাই;
        }

        ngx_memcpy(&ctx->buffer[used], data, মুক্ত);
        data = (u_char *) data + মুক্ত;
        size -= মুক্ত;
        (শূন্য) ngx_sha1_body(ctx, ctx->buffer, 64);
    }

    যদি (size >= 64) {
        data = ngx_sha1_body(ctx, data, size & ~(size_t) 0x3f);
        size &= 0x3f;
    }

    ngx_memcpy(ctx->buffer, data, size);
}


শূন্য
ngx_sha1_final(u_char result[20], ngx_sha1_t *ctx)
{
    size_t  used, মুক্ত;

    used = (size_t) (ctx->bytes & 0x3f);

    ctx->buffer[used++] = 0x80;

    মুক্ত = 64 - used;

    যদি (মুক্ত < 8) {
        ngx_memzero(&ctx->buffer[used], মুক্ত);
        (শূন্য) ngx_sha1_body(ctx, ctx->buffer, 64);
        used = 0;
        মুক্ত = 64;
    }

    ngx_memzero(&ctx->buffer[used], মুক্ত - 8);

    ctx->bytes <<= 3;
    ctx->buffer[56] = (u_char) (ctx->bytes >> 56);
    ctx->buffer[57] = (u_char) (ctx->bytes >> 48);
    ctx->buffer[58] = (u_char) (ctx->bytes >> 40);
    ctx->buffer[59] = (u_char) (ctx->bytes >> 32);
    ctx->buffer[60] = (u_char) (ctx->bytes >> 24);
    ctx->buffer[61] = (u_char) (ctx->bytes >> 16);
    ctx->buffer[62] = (u_char) (ctx->bytes >> 8);
    ctx->buffer[63] = (u_char) ctx->bytes;

    (শূন্য) ngx_sha1_body(ctx, ctx->buffer, 64);

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

    ngx_memzero(ctx, পরিমাণ(*ctx));
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

স্থির ধ্রুৱ u_char *
ngx_sha1_body(ngx_sha1_t *ctx, ধ্রুৱ u_char *data, size_t size)
{
    uint32_t       a, b, c, d, e, temp;
    uint32_t       saved_a, saved_b, saved_c, saved_d, saved_e;
    uint32_t       শব্দগুলি[80];
    ngx_uint_t     i;
    ধ্রুৱ u_char  *p;

    p = data;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;
    e = ctx->e;

    কৰা {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;
        saved_e = e;

        /* Load data block into the words array */

        বাবে (i = 0; i < 16; i++) {
            শব্দগুলি[i] = GET(i);
        }

        বাবে (i = 16; i < 80; i++) {
            শব্দগুলি[i] = ROTATE(1, শব্দগুলি[i - 3] ^ শব্দগুলি[i - 8] ^ শব্দগুলি[i - 14]
                                 ^ শব্দগুলি[i - 16]);
        }

        /* Transformations */

        STEP(F1, a, b, c, d, e, শব্দগুলি[0],  0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[1],  0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[2],  0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[3],  0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[4],  0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[5],  0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[6],  0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[7],  0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[8],  0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[9],  0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[10], 0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[11], 0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[12], 0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[13], 0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[14], 0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[15], 0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[16], 0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[17], 0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[18], 0x5a827999);
        STEP(F1, a, b, c, d, e, শব্দগুলি[19], 0x5a827999);

        STEP(F2, a, b, c, d, e, শব্দগুলি[20], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[21], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[22], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[23], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[24], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[25], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[26], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[27], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[28], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[29], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[30], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[31], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[32], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[33], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[34], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[35], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[36], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[37], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[38], 0x6ed9eba1);
        STEP(F2, a, b, c, d, e, শব্দগুলি[39], 0x6ed9eba1);

        STEP(F3, a, b, c, d, e, শব্দগুলি[40], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[41], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[42], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[43], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[44], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[45], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[46], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[47], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[48], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[49], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[50], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[51], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[52], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[53], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[54], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[55], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[56], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[57], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[58], 0x8f1bbcdc);
        STEP(F3, a, b, c, d, e, শব্দগুলি[59], 0x8f1bbcdc);

        STEP(F2, a, b, c, d, e, শব্দগুলি[60], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[61], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[62], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[63], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[64], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[65], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[66], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[67], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[68], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[69], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[70], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[71], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[72], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[73], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[74], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[75], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[76], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[77], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[78], 0xca62c1d6);
        STEP(F2, a, b, c, d, e, শব্দগুলি[79], 0xca62c1d6);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;
        e += saved_e;

        p += 64;

    } যেতিয়ালৈ (size -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;
    ctx->e = e;

    ঘূৰাই p;
}
