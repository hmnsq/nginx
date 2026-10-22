
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 *
 * An internal SHA1 implementation.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_sha1.h>


स्थिर ध्रुव u_char *ngx_sha1_body(ngx_sha1_t *ctx, ध्रुव u_char *दत्त,
    आकार_t आकार);


शून्य
ngx_sha1_init(ngx_sha1_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;
    ctx->e = 0xc3d2e1f0;

    ctx->अष्टकानि = 0;
}


शून्य
ngx_sha1_update(ngx_sha1_t *ctx, ध्रुव शून्य *दत्त, आकार_t आकार)
{
    आकार_t  प्रयुक्तम्, मुक्त;

    प्रयुक्तम् = (आकार_t) (ctx->अष्टकानि & 0x3f);
    ctx->अष्टकानि += आकार;

    यदि (प्रयुक्तम्) {
        मुक्त = 64 - प्रयुक्तम्;

        यदि (आकार < मुक्त) {
            ngx_memcpy(&ctx->बफर[प्रयुक्तम्], दत्त, आकार);
            प्रत्यागम;
        }

        ngx_memcpy(&ctx->बफर[प्रयुक्तम्], दत्त, मुक्त);
        दत्त = (u_char *) दत्त + मुक्त;
        आकार -= मुक्त;
        (शून्य) ngx_sha1_body(ctx, ctx->बफर, 64);
    }

    यदि (आकार >= 64) {
        दत्त = ngx_sha1_body(ctx, दत्त, आकार & ~(आकार_t) 0x3f);
        आकार &= 0x3f;
    }

    ngx_memcpy(ctx->बफर, दत्त, आकार);
}


शून्य
ngx_sha1_final(u_char परिणाम[20], ngx_sha1_t *ctx)
{
    आकार_t  प्रयुक्तम्, मुक्त;

    प्रयुक्तम् = (आकार_t) (ctx->अष्टकानि & 0x3f);

    ctx->बफर[प्रयुक्तम्++] = 0x80;

    मुक्त = 64 - प्रयुक्तम्;

    यदि (मुक्त < 8) {
        ngx_memzero(&ctx->बफर[प्रयुक्तम्], मुक्त);
        (शून्य) ngx_sha1_body(ctx, ctx->बफर, 64);
        प्रयुक्तम् = 0;
        मुक्त = 64;
    }

    ngx_memzero(&ctx->बफर[प्रयुक्तम्], मुक्त - 8);

    ctx->अष्टकानि <<= 3;
    ctx->बफर[56] = (u_char) (ctx->अष्टकानि >> 56);
    ctx->बफर[57] = (u_char) (ctx->अष्टकानि >> 48);
    ctx->बफर[58] = (u_char) (ctx->अष्टकानि >> 40);
    ctx->बफर[59] = (u_char) (ctx->अष्टकानि >> 32);
    ctx->बफर[60] = (u_char) (ctx->अष्टकानि >> 24);
    ctx->बफर[61] = (u_char) (ctx->अष्टकानि >> 16);
    ctx->बफर[62] = (u_char) (ctx->अष्टकानि >> 8);
    ctx->बफर[63] = (u_char) ctx->अष्टकानि;

    (शून्य) ngx_sha1_body(ctx, ctx->बफर, 64);

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

    ngx_memzero(ctx, परिमाण(*ctx));
}


/*
 * Helper functions.
 */

#परिभाषय ROTATE(द्वयङ्काः, word)  (((word) << (द्वयङ्काः)) | ((word) >> (32 - (द्वयङ्काः))))

#परिभाषय F1(b, c, d)  (((b) & (c)) | ((~(b)) & (d)))
#परिभाषय F2(b, c, d)  ((b) ^ (c) ^ (d))
#परिभाषय F3(b, c, d)  (((b) & (c)) | ((b) & (d)) | ((c) & (d)))

#परिभाषय चरणः(f, a, b, c, d, e, w, t)                                          \
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

#परिभाषय GET(n)                                                                \
    ((uint32_t) p[n * 4 + 3] |                                                \
    ((uint32_t) p[n * 4 + 2] << 8) |                                          \
    ((uint32_t) p[n * 4 + 1] << 16) |                                         \
    ((uint32_t) p[n * 4] << 24))


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

स्थिर ध्रुव u_char *
ngx_sha1_body(ngx_sha1_t *ctx, ध्रुव u_char *दत्त, आकार_t आकार)
{
    uint32_t       a, b, c, d, e, अस्थायी;
    uint32_t       saved_a, saved_b, saved_c, saved_d, saved_e;
    uint32_t       शब्दाः[80];
    ngx_uint_t     i;
    ध्रुव u_char  *p;

    p = दत्त;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;
    e = ctx->e;

    कुरु {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;
        saved_e = e;

        /* Load data block into the words array */

        प्रति (i = 0; i < 16; i++) {
            शब्दाः[i] = GET(i);
        }

        प्रति (i = 16; i < 80; i++) {
            शब्दाः[i] = ROTATE(1, शब्दाः[i - 3] ^ शब्दाः[i - 8] ^ शब्दाः[i - 14]
                                 ^ शब्दाः[i - 16]);
        }

        /* Transformations */

        चरणः(F1, a, b, c, d, e, शब्दाः[0],  0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[1],  0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[2],  0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[3],  0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[4],  0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[5],  0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[6],  0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[7],  0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[8],  0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[9],  0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[10], 0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[11], 0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[12], 0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[13], 0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[14], 0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[15], 0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[16], 0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[17], 0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[18], 0x5a827999);
        चरणः(F1, a, b, c, d, e, शब्दाः[19], 0x5a827999);

        चरणः(F2, a, b, c, d, e, शब्दाः[20], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[21], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[22], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[23], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[24], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[25], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[26], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[27], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[28], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[29], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[30], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[31], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[32], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[33], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[34], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[35], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[36], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[37], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[38], 0x6ed9eba1);
        चरणः(F2, a, b, c, d, e, शब्दाः[39], 0x6ed9eba1);

        चरणः(F3, a, b, c, d, e, शब्दाः[40], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[41], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[42], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[43], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[44], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[45], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[46], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[47], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[48], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[49], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[50], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[51], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[52], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[53], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[54], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[55], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[56], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[57], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[58], 0x8f1bbcdc);
        चरणः(F3, a, b, c, d, e, शब्दाः[59], 0x8f1bbcdc);

        चरणः(F2, a, b, c, d, e, शब्दाः[60], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[61], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[62], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[63], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[64], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[65], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[66], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[67], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[68], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[69], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[70], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[71], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[72], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[73], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[74], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[75], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[76], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[77], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[78], 0xca62c1d6);
        चरणः(F2, a, b, c, d, e, शब्दाः[79], 0xca62c1d6);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;
        e += saved_e;

        p += 64;

    } यावत् (आकार -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;
    ctx->e = e;

    प्रत्यागम p;
}
