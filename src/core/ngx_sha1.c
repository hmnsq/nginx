
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 *
 * An internal SHA1 implementation.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <ngx_sha1.h>


స్థిర స్థిరరాశి u_char *ngx_sha1_body(ngx_sha1_t *ctx, స్థిరరాశి u_char *దత్తాంశం,
    కొలత_t కొలత);


శూన్య
ngx_sha1_init(ngx_sha1_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;
    ctx->e = 0xc3d2e1f0;

    ctx->బైట్లు = 0;
}


శూన్య
ngx_sha1_update(ngx_sha1_t *ctx, స్థిరరాశి శూన్య *దత్తాంశం, కొలత_t కొలత)
{
    కొలత_t  వాడిన, విడుదల;

    వాడిన = (కొలత_t) (ctx->బైట్లు & 0x3f);
    ctx->బైట్లు += కొలత;

    ఒకవేళ (వాడిన) {
        విడుదల = 64 - వాడిన;

        ఒకవేళ (కొలత < విడుదల) {
            ngx_memcpy(&ctx->బఫరు[వాడిన], దత్తాంశం, కొలత);
            తిరిగి;
        }

        ngx_memcpy(&ctx->బఫరు[వాడిన], దత్తాంశం, విడుదల);
        దత్తాంశం = (u_char *) దత్తాంశం + విడుదల;
        కొలత -= విడుదల;
        (శూన్య) ngx_sha1_body(ctx, ctx->బఫరు, 64);
    }

    ఒకవేళ (కొలత >= 64) {
        దత్తాంశం = ngx_sha1_body(ctx, దత్తాంశం, కొలత & ~(కొలత_t) 0x3f);
        కొలత &= 0x3f;
    }

    ngx_memcpy(ctx->బఫరు, దత్తాంశం, కొలత);
}


శూన్య
ngx_sha1_final(u_char ఫలితం[20], ngx_sha1_t *ctx)
{
    కొలత_t  వాడిన, విడుదల;

    వాడిన = (కొలత_t) (ctx->బైట్లు & 0x3f);

    ctx->బఫరు[వాడిన++] = 0x80;

    విడుదల = 64 - వాడిన;

    ఒకవేళ (విడుదల < 8) {
        ngx_memzero(&ctx->బఫరు[వాడిన], విడుదల);
        (శూన్య) ngx_sha1_body(ctx, ctx->బఫరు, 64);
        వాడిన = 0;
        విడుదల = 64;
    }

    ngx_memzero(&ctx->బఫరు[వాడిన], విడుదల - 8);

    ctx->బైట్లు <<= 3;
    ctx->బఫరు[56] = (u_char) (ctx->బైట్లు >> 56);
    ctx->బఫరు[57] = (u_char) (ctx->బైట్లు >> 48);
    ctx->బఫరు[58] = (u_char) (ctx->బైట్లు >> 40);
    ctx->బఫరు[59] = (u_char) (ctx->బైట్లు >> 32);
    ctx->బఫరు[60] = (u_char) (ctx->బైట్లు >> 24);
    ctx->బఫరు[61] = (u_char) (ctx->బైట్లు >> 16);
    ctx->బఫరు[62] = (u_char) (ctx->బైట్లు >> 8);
    ctx->బఫరు[63] = (u_char) ctx->బైట్లు;

    (శూన్య) ngx_sha1_body(ctx, ctx->బఫరు, 64);

    ఫలితం[0] = (u_char) (ctx->a >> 24);
    ఫలితం[1] = (u_char) (ctx->a >> 16);
    ఫలితం[2] = (u_char) (ctx->a >> 8);
    ఫలితం[3] = (u_char) ctx->a;
    ఫలితం[4] = (u_char) (ctx->b >> 24);
    ఫలితం[5] = (u_char) (ctx->b >> 16);
    ఫలితం[6] = (u_char) (ctx->b >> 8);
    ఫలితం[7] = (u_char) ctx->b;
    ఫలితం[8] = (u_char) (ctx->c >> 24);
    ఫలితం[9] = (u_char) (ctx->c >> 16);
    ఫలితం[10] = (u_char) (ctx->c >> 8);
    ఫలితం[11] = (u_char) ctx->c;
    ఫలితం[12] = (u_char) (ctx->d >> 24);
    ఫలితం[13] = (u_char) (ctx->d >> 16);
    ఫలితం[14] = (u_char) (ctx->d >> 8);
    ఫలితం[15] = (u_char) ctx->d;
    ఫలితం[16] = (u_char) (ctx->e >> 24);
    ఫలితం[17] = (u_char) (ctx->e >> 16);
    ఫలితం[18] = (u_char) (ctx->e >> 8);
    ఫలితం[19] = (u_char) ctx->e;

    ngx_memzero(ctx, పరిమాణం(*ctx));
}


/*
 * Helper functions.
 */

#నిర్వచించు ROTATE(బిట్లు, మాట)  (((మాట) << (బిట్లు)) | ((మాట) >> (32 - (బిట్లు))))

#నిర్వచించు F1(b, c, d)  (((b) & (c)) | ((~(b)) & (d)))
#నిర్వచించు F2(b, c, d)  ((b) ^ (c) ^ (d))
#నిర్వచించు F3(b, c, d)  (((b) & (c)) | ((b) & (d)) | ((c) & (d)))

#నిర్వచించు అడుగు(f, a, b, c, d, e, w, t)                                          \
    తాత్కాలిక = ROTATE(5, (a)) + f((b), (c), (d)) + (e) + (w) + (t);               \
    (e) = (d);                                                                \
    (d) = (c);                                                                \
    (c) = ROTATE(30, (b));                                                    \
    (b) = (a);                                                                \
    (a) = తాత్కాలిక;


/*
 * GET() reads 4 input bytes in big-endian byte order and returns
 * them as uint32_t.
 */

#నిర్వచించు GET(n)                                                                \
    ((uint32_t) p[n * 4 + 3] |                                                \
    ((uint32_t) p[n * 4 + 2] << 8) |                                          \
    ((uint32_t) p[n * 4 + 1] << 16) |                                         \
    ((uint32_t) p[n * 4] << 24))


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

స్థిర స్థిరరాశి u_char *
ngx_sha1_body(ngx_sha1_t *ctx, స్థిరరాశి u_char *దత్తాంశం, కొలత_t కొలత)
{
    uint32_t       a, b, c, d, e, తాత్కాలిక;
    uint32_t       saved_a, saved_b, saved_c, saved_d, saved_e;
    uint32_t       పదాలు[80];
    ngx_uint_t     i;
    స్థిరరాశి u_char  *p;

    p = దత్తాంశం;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;
    e = ctx->e;

    చేయి {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;
        saved_e = e;

        /* Load data block into the words array */

        కొరకు (i = 0; i < 16; i++) {
            పదాలు[i] = GET(i);
        }

        కొరకు (i = 16; i < 80; i++) {
            పదాలు[i] = ROTATE(1, పదాలు[i - 3] ^ పదాలు[i - 8] ^ పదాలు[i - 14]
                                 ^ పదాలు[i - 16]);
        }

        /* Transformations */

        అడుగు(F1, a, b, c, d, e, పదాలు[0],  0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[1],  0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[2],  0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[3],  0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[4],  0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[5],  0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[6],  0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[7],  0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[8],  0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[9],  0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[10], 0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[11], 0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[12], 0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[13], 0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[14], 0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[15], 0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[16], 0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[17], 0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[18], 0x5a827999);
        అడుగు(F1, a, b, c, d, e, పదాలు[19], 0x5a827999);

        అడుగు(F2, a, b, c, d, e, పదాలు[20], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[21], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[22], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[23], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[24], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[25], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[26], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[27], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[28], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[29], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[30], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[31], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[32], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[33], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[34], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[35], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[36], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[37], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[38], 0x6ed9eba1);
        అడుగు(F2, a, b, c, d, e, పదాలు[39], 0x6ed9eba1);

        అడుగు(F3, a, b, c, d, e, పదాలు[40], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[41], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[42], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[43], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[44], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[45], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[46], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[47], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[48], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[49], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[50], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[51], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[52], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[53], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[54], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[55], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[56], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[57], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[58], 0x8f1bbcdc);
        అడుగు(F3, a, b, c, d, e, పదాలు[59], 0x8f1bbcdc);

        అడుగు(F2, a, b, c, d, e, పదాలు[60], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[61], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[62], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[63], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[64], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[65], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[66], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[67], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[68], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[69], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[70], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[71], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[72], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[73], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[74], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[75], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[76], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[77], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[78], 0xca62c1d6);
        అడుగు(F2, a, b, c, d, e, పదాలు[79], 0xca62c1d6);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;
        e += saved_e;

        p += 64;

    } అయితే (కొలత -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;
    ctx->e = e;

    తిరిగి p;
}
