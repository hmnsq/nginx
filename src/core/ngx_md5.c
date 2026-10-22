
/*
 * An internal implementation, based on Alexander Peslyak's
 * public domain implementation:
 * http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_md5.h>


स्थिर ध्रुव u_char *ngx_md5_body(ngx_md5_t *ctx, ध्रुव u_char *दत्त,
    आकार_t आकार);


शून्य
ngx_md5_init(ngx_md5_t *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;

    ctx->अष्टकानि = 0;
}


शून्य
ngx_md5_update(ngx_md5_t *ctx, ध्रुव शून्य *दत्त, आकार_t आकार)
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
        (शून्य) ngx_md5_body(ctx, ctx->बफर, 64);
    }

    यदि (आकार >= 64) {
        दत्त = ngx_md5_body(ctx, दत्त, आकार & ~(आकार_t) 0x3f);
        आकार &= 0x3f;
    }

    ngx_memcpy(ctx->बफर, दत्त, आकार);
}


शून्य
ngx_md5_final(u_char परिणाम[16], ngx_md5_t *ctx)
{
    आकार_t  प्रयुक्तम्, मुक्त;

    प्रयुक्तम् = (आकार_t) (ctx->अष्टकानि & 0x3f);

    ctx->बफर[प्रयुक्तम्++] = 0x80;

    मुक्त = 64 - प्रयुक्तम्;

    यदि (मुक्त < 8) {
        ngx_memzero(&ctx->बफर[प्रयुक्तम्], मुक्त);
        (शून्य) ngx_md5_body(ctx, ctx->बफर, 64);
        प्रयुक्तम् = 0;
        मुक्त = 64;
    }

    ngx_memzero(&ctx->बफर[प्रयुक्तम्], मुक्त - 8);

    ctx->अष्टकानि <<= 3;
    ctx->बफर[56] = (u_char) ctx->अष्टकानि;
    ctx->बफर[57] = (u_char) (ctx->अष्टकानि >> 8);
    ctx->बफर[58] = (u_char) (ctx->अष्टकानि >> 16);
    ctx->बफर[59] = (u_char) (ctx->अष्टकानि >> 24);
    ctx->बफर[60] = (u_char) (ctx->अष्टकानि >> 32);
    ctx->बफर[61] = (u_char) (ctx->अष्टकानि >> 40);
    ctx->बफर[62] = (u_char) (ctx->अष्टकानि >> 48);
    ctx->बफर[63] = (u_char) (ctx->अष्टकानि >> 56);

    (शून्य) ngx_md5_body(ctx, ctx->बफर, 64);

    परिणाम[0] = (u_char) ctx->a;
    परिणाम[1] = (u_char) (ctx->a >> 8);
    परिणाम[2] = (u_char) (ctx->a >> 16);
    परिणाम[3] = (u_char) (ctx->a >> 24);
    परिणाम[4] = (u_char) ctx->b;
    परिणाम[5] = (u_char) (ctx->b >> 8);
    परिणाम[6] = (u_char) (ctx->b >> 16);
    परिणाम[7] = (u_char) (ctx->b >> 24);
    परिणाम[8] = (u_char) ctx->c;
    परिणाम[9] = (u_char) (ctx->c >> 8);
    परिणाम[10] = (u_char) (ctx->c >> 16);
    परिणाम[11] = (u_char) (ctx->c >> 24);
    परिणाम[12] = (u_char) ctx->d;
    परिणाम[13] = (u_char) (ctx->d >> 8);
    परिणाम[14] = (u_char) (ctx->d >> 16);
    परिणाम[15] = (u_char) (ctx->d >> 24);

    ngx_memzero(ctx, परिमाण(*ctx));
}


/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in
 * Colin Plumb's implementation.
 */

#परिभाषय F(x, y, z)  ((z) ^ ((x) & ((y) ^ (z))))
#परिभाषय G(x, y, z)  ((y) ^ ((z) & ((x) ^ (y))))
#परिभाषय H(x, y, z)  ((x) ^ (y) ^ (z))
#परिभाषय I(x, y, z)  ((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */

#परिभाषय चरणः(f, a, b, c, d, x, t, s)                                          \
    (a) += f((b), (c), (d)) + (x) + (t);                                      \
    (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s))));                \
    (a) += (b)

/*
 * SET() reads 4 input bytes in little-endian byte order and stores them
 * in a properly aligned word in host byte order.
 *
 * The check for little-endian architectures that tolerate unaligned
 * memory accesses is just an optimization.  Nothing will break if it
 * does not work.
 */

#यदि (NGX_HAVE_LITTLE_ENDIAN && NGX_HAVE_NONALIGNED)

#परिभाषय समुच्चयः_2(n)      (*(uint32_t *) &p[n * 4])
#परिभाषय GET(n)      (*(uint32_t *) &p[n * 4])

#अन्यथा

#परिभाषय समुच्चयः_2(n)                                                                \
    (खण्डम्[n] =                                                               \
    (uint32_t) p[n * 4] |                                                     \
    ((uint32_t) p[n * 4 + 1] << 8) |                                          \
    ((uint32_t) p[n * 4 + 2] << 16) |                                         \
    ((uint32_t) p[n * 4 + 3] << 24))

#परिभाषय GET(n)      खण्डम्[n]

#यदि_अन्तः


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

स्थिर ध्रुव u_char *
ngx_md5_body(ngx_md5_t *ctx, ध्रुव u_char *दत्त, आकार_t आकार)
{
    uint32_t       a, b, c, d;
    uint32_t       saved_a, saved_b, saved_c, saved_d;
    ध्रुव u_char  *p;
#यदि !(NGX_HAVE_LITTLE_ENDIAN && NGX_HAVE_NONALIGNED)
    uint32_t       खण्डम्[16];
#यदि_अन्तः

    p = दत्त;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;

    कुरु {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;

        /* Round 1 */

        चरणः(F, a, b, c, d, समुच्चयः_2(0),  0xd76aa478, 7);
        चरणः(F, d, a, b, c, समुच्चयः_2(1),  0xe8c7b756, 12);
        चरणः(F, c, d, a, b, समुच्चयः_2(2),  0x242070db, 17);
        चरणः(F, b, c, d, a, समुच्चयः_2(3),  0xc1bdceee, 22);
        चरणः(F, a, b, c, d, समुच्चयः_2(4),  0xf57c0faf, 7);
        चरणः(F, d, a, b, c, समुच्चयः_2(5),  0x4787c62a, 12);
        चरणः(F, c, d, a, b, समुच्चयः_2(6),  0xa8304613, 17);
        चरणः(F, b, c, d, a, समुच्चयः_2(7),  0xfd469501, 22);
        चरणः(F, a, b, c, d, समुच्चयः_2(8),  0x698098d8, 7);
        चरणः(F, d, a, b, c, समुच्चयः_2(9),  0x8b44f7af, 12);
        चरणः(F, c, d, a, b, समुच्चयः_2(10), 0xffff5bb1, 17);
        चरणः(F, b, c, d, a, समुच्चयः_2(11), 0x895cd7be, 22);
        चरणः(F, a, b, c, d, समुच्चयः_2(12), 0x6b901122, 7);
        चरणः(F, d, a, b, c, समुच्चयः_2(13), 0xfd987193, 12);
        चरणः(F, c, d, a, b, समुच्चयः_2(14), 0xa679438e, 17);
        चरणः(F, b, c, d, a, समुच्चयः_2(15), 0x49b40821, 22);

        /* Round 2 */

        चरणः(G, a, b, c, d, GET(1),  0xf61e2562, 5);
        चरणः(G, d, a, b, c, GET(6),  0xc040b340, 9);
        चरणः(G, c, d, a, b, GET(11), 0x265e5a51, 14);
        चरणः(G, b, c, d, a, GET(0),  0xe9b6c7aa, 20);
        चरणः(G, a, b, c, d, GET(5),  0xd62f105d, 5);
        चरणः(G, d, a, b, c, GET(10), 0x02441453, 9);
        चरणः(G, c, d, a, b, GET(15), 0xd8a1e681, 14);
        चरणः(G, b, c, d, a, GET(4),  0xe7d3fbc8, 20);
        चरणः(G, a, b, c, d, GET(9),  0x21e1cde6, 5);
        चरणः(G, d, a, b, c, GET(14), 0xc33707d6, 9);
        चरणः(G, c, d, a, b, GET(3),  0xf4d50d87, 14);
        चरणः(G, b, c, d, a, GET(8),  0x455a14ed, 20);
        चरणः(G, a, b, c, d, GET(13), 0xa9e3e905, 5);
        चरणः(G, d, a, b, c, GET(2),  0xfcefa3f8, 9);
        चरणः(G, c, d, a, b, GET(7),  0x676f02d9, 14);
        चरणः(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20);

        /* Round 3 */

        चरणः(H, a, b, c, d, GET(5),  0xfffa3942, 4);
        चरणः(H, d, a, b, c, GET(8),  0x8771f681, 11);
        चरणः(H, c, d, a, b, GET(11), 0x6d9d6122, 16);
        चरणः(H, b, c, d, a, GET(14), 0xfde5380c, 23);
        चरणः(H, a, b, c, d, GET(1),  0xa4beea44, 4);
        चरणः(H, d, a, b, c, GET(4),  0x4bdecfa9, 11);
        चरणः(H, c, d, a, b, GET(7),  0xf6bb4b60, 16);
        चरणः(H, b, c, d, a, GET(10), 0xbebfbc70, 23);
        चरणः(H, a, b, c, d, GET(13), 0x289b7ec6, 4);
        चरणः(H, d, a, b, c, GET(0),  0xeaa127fa, 11);
        चरणः(H, c, d, a, b, GET(3),  0xd4ef3085, 16);
        चरणः(H, b, c, d, a, GET(6),  0x04881d05, 23);
        चरणः(H, a, b, c, d, GET(9),  0xd9d4d039, 4);
        चरणः(H, d, a, b, c, GET(12), 0xe6db99e5, 11);
        चरणः(H, c, d, a, b, GET(15), 0x1fa27cf8, 16);
        चरणः(H, b, c, d, a, GET(2),  0xc4ac5665, 23);

        /* Round 4 */

        चरणः(I, a, b, c, d, GET(0),  0xf4292244, 6);
        चरणः(I, d, a, b, c, GET(7),  0x432aff97, 10);
        चरणः(I, c, d, a, b, GET(14), 0xab9423a7, 15);
        चरणः(I, b, c, d, a, GET(5),  0xfc93a039, 21);
        चरणः(I, a, b, c, d, GET(12), 0x655b59c3, 6);
        चरणः(I, d, a, b, c, GET(3),  0x8f0ccc92, 10);
        चरणः(I, c, d, a, b, GET(10), 0xffeff47d, 15);
        चरणः(I, b, c, d, a, GET(1),  0x85845dd1, 21);
        चरणः(I, a, b, c, d, GET(8),  0x6fa87e4f, 6);
        चरणः(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10);
        चरणः(I, c, d, a, b, GET(6),  0xa3014314, 15);
        चरणः(I, b, c, d, a, GET(13), 0x4e0811a1, 21);
        चरणः(I, a, b, c, d, GET(4),  0xf7537e82, 6);
        चरणः(I, d, a, b, c, GET(11), 0xbd3af235, 10);
        चरणः(I, c, d, a, b, GET(2),  0x2ad7d2bb, 15);
        चरणः(I, b, c, d, a, GET(9),  0xeb86d391, 21);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;

        p += 64;

    } यावत् (आकार -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;

    प्रत्यागम p;
}
