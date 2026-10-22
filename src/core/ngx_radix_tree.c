
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ସ୍ଥିର ngx_radix_ଗଣ୍ଠି_t *ngx_radix_alloc(ngx_radix_ବୃକ୍ଷ_t *ବୃକ୍ଷ);


ngx_radix_ବୃକ୍ଷ_t *
ngx_radix_ବୃକ୍ଷ_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           ଚାବି, mask, inc;
    ngx_radix_ବୃକ୍ଷ_t  *ବୃକ୍ଷ;

    ବୃକ୍ଷ = ngx_palloc(pool, ଆକାର(ngx_radix_ବୃକ୍ଷ_t));
    ଯଦି (ବୃକ୍ଷ == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ବୃକ୍ଷ->pool = pool;
    ବୃକ୍ଷ->ମୁକ୍ତ = ଖାଲି;
    ବୃକ୍ଷ->start = ଖାଲି;
    ବୃକ୍ଷ->size = 0;

    ବୃକ୍ଷ->ମୂଳ = ngx_radix_alloc(ବୃକ୍ଷ);
    ଯଦି (ବୃକ୍ଷ->ମୂଳ == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ବୃକ୍ଷ->ମୂଳ->ଡାହାଣ = ଖାଲି;
    ବୃକ୍ଷ->ମୂଳ->ବାମ = ଖାଲି;
    ବୃକ୍ଷ->ମୂଳ->parent = ଖାଲି;
    ବୃକ୍ଷ->ମୂଳ->ମୂଲ୍ୟ = NGX_RADIX_NO_ମୂଲ୍ୟ;

    ଯଦି (preallocate == 0) {
        ଫେରନ୍ତୁ ବୃକ୍ଷ;
    }

    /*
     * Preallocation of first nodes : 0, 1, 00, 01, 10, 11, 000, 001, etc.
     * increases TLB hits even if for first lookup iterations.
     * On 32-bit platforms the 7 preallocated bits takes continuous 4K,
     * 8 - 8K, 9 - 16K, etc.  On 64-bit platforms the 6 preallocated bits
     * takes continuous 4K, 7 - 8K, 8 - 16K, etc.  There is no sense to
     * to preallocate more than one page, because further preallocation
     * distributes the only bit per page.  Instead, a random insertion
     * may distribute several bits per page.
     *
     * Thus, by default we preallocate maximum
     *     6 bits on amd64 (64-bit platform and 4K pages)
     *     7 bits on i386 (32-bit platform and 4K pages)
     *     7 bits on sparc64 in 64-bit mode (8K pages)
     *     8 bits on sparc64 in 32-bit mode (8K pages)
     */

    ଯଦି (preallocate == -1) {
        ବାଛ (ngx_pagesize / ଆକାର(ngx_radix_ଗଣ୍ଠି_t)) {

        /* amd64 */
        ସ୍ଥିତି 128:
            preallocate = 6;
            ଭାଙ୍ଗ;

        /* i386, sparc64 */
        ସ୍ଥିତି 256:
            preallocate = 7;
            ଭାଙ୍ଗ;

        /* sparc64 in 32-bit mode */
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    ଯେତେବେଳେ (preallocate--) {

        ଚାବି = 0;
        mask >>= 1;
        mask |= 0x80000000;

        କର {
            ଯଦି (ngx_radix32tree_ଭର୍ତ୍ତି(ବୃକ୍ଷ, ଚାବି, mask, NGX_RADIX_NO_ମୂଲ୍ୟ)
                != NGX_OK)
            {
                ଫେରନ୍ତୁ ଖାଲି;
            }

            ଚାବି += inc;

        } ଯେତେବେଳେ (ଚାବି);

        inc >>= 1;
    }

    ଫେରନ୍ତୁ ବୃକ୍ଷ;
}


ngx_int_t
ngx_radix32tree_ଭର୍ତ୍ତି(ngx_radix_ବୃକ୍ଷ_t *ବୃକ୍ଷ, uint32_t ଚାବି, uint32_t mask,
    uintptr_t ମୂଲ୍ୟ)
{
    uint32_t           bit;
    ngx_radix_ଗଣ୍ଠି_t  *ଗଣ୍ଠି, *next;

    bit = 0x80000000;

    ଗଣ୍ଠି = ବୃକ୍ଷ->ମୂଳ;
    next = ବୃକ୍ଷ->ମୂଳ;

    ଯେତେବେଳେ (bit & mask) {
        ଯଦି (ଚାବି & bit) {
            next = ଗଣ୍ଠି->ଡାହାଣ;

        } ଅନ୍ୟଥା {
            next = ଗଣ୍ଠି->ବାମ;
        }

        ଯଦି (next == ଖାଲି) {
            ଭାଙ୍ଗ;
        }

        bit >>= 1;
        ଗଣ୍ଠି = next;
    }

    ଯଦି (next) {
        ଯଦି (ଗଣ୍ଠି->ମୂଲ୍ୟ != NGX_RADIX_NO_ମୂଲ୍ୟ) {
            ଫେରନ୍ତୁ NGX_BUSY;
        }

        ଗଣ୍ଠି->ମୂଲ୍ୟ = ମୂଲ୍ୟ;
        ଫେରନ୍ତୁ NGX_OK;
    }

    ଯେତେବେଳେ (bit & mask) {
        next = ngx_radix_alloc(ବୃକ୍ଷ);
        ଯଦି (next == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        next->ଡାହାଣ = ଖାଲି;
        next->ବାମ = ଖାଲି;
        next->parent = ଗଣ୍ଠି;
        next->ମୂଲ୍ୟ = NGX_RADIX_NO_ମୂଲ୍ୟ;

        ଯଦି (ଚାବି & bit) {
            ଗଣ୍ଠି->ଡାହାଣ = next;

        } ଅନ୍ୟଥା {
            ଗଣ୍ଠି->ବାମ = next;
        }

        bit >>= 1;
        ଗଣ୍ଠି = next;
    }

    ଗଣ୍ଠି->ମୂଲ୍ୟ = ମୂଲ୍ୟ;

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_ବୃକ୍ଷ_t *ବୃକ୍ଷ, uint32_t ଚାବି, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_ଗଣ୍ଠି_t  *ଗଣ୍ଠି;

    bit = 0x80000000;
    ଗଣ୍ଠି = ବୃକ୍ଷ->ମୂଳ;

    ଯେତେବେଳେ (ଗଣ୍ଠି && (bit & mask)) {
        ଯଦି (ଚାବି & bit) {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ଡାହାଣ;

        } ଅନ୍ୟଥା {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ବାମ;
        }

        bit >>= 1;
    }

    ଯଦି (ଗଣ୍ଠି == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (ଗଣ୍ଠି->ଡାହାଣ || ଗଣ୍ଠି->ବାମ) {
        ଯଦି (ଗଣ୍ଠି->ମୂଲ୍ୟ != NGX_RADIX_NO_ମୂଲ୍ୟ) {
            ଗଣ୍ଠି->ମୂଲ୍ୟ = NGX_RADIX_NO_ମୂଲ୍ୟ;
            ଫେରନ୍ତୁ NGX_OK;
        }

        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ପାଇଁ ( ;; ) {
        ଯଦି (ଗଣ୍ଠି->parent->ଡାହାଣ == ଗଣ୍ଠି) {
            ଗଣ୍ଠି->parent->ଡାହାଣ = ଖାଲି;

        } ଅନ୍ୟଥା {
            ଗଣ୍ଠି->parent->ବାମ = ଖାଲି;
        }

        ଗଣ୍ଠି->ଡାହାଣ = ବୃକ୍ଷ->ମୁକ୍ତ;
        ବୃକ୍ଷ->ମୁକ୍ତ = ଗଣ୍ଠି;

        ଗଣ୍ଠି = ଗଣ୍ଠି->parent;

        ଯଦି (ଗଣ୍ଠି->ଡାହାଣ || ଗଣ୍ଠି->ବାମ) {
            ଭାଙ୍ଗ;
        }

        ଯଦି (ଗଣ୍ଠି->ମୂଲ୍ୟ != NGX_RADIX_NO_ମୂଲ୍ୟ) {
            ଭାଙ୍ଗ;
        }

        ଯଦି (ଗଣ୍ଠି->parent == ଖାଲି) {
            ଭାଙ୍ଗ;
        }
    }

    ଫେରନ୍ତୁ NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_ବୃକ୍ଷ_t *ବୃକ୍ଷ, uint32_t ଚାବି)
{
    uint32_t           bit;
    uintptr_t          ମୂଲ୍ୟ;
    ngx_radix_ଗଣ୍ଠି_t  *ଗଣ୍ଠି;

    bit = 0x80000000;
    ମୂଲ୍ୟ = NGX_RADIX_NO_ମୂଲ୍ୟ;
    ଗଣ୍ଠି = ବୃକ୍ଷ->ମୂଳ;

    ଯେତେବେଳେ (ଗଣ୍ଠି) {
        ଯଦି (ଗଣ୍ଠି->ମୂଲ୍ୟ != NGX_RADIX_NO_ମୂଲ୍ୟ) {
            ମୂଲ୍ୟ = ଗଣ୍ଠି->ମୂଲ୍ୟ;
        }

        ଯଦି (ଚାବି & bit) {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ଡାହାଣ;

        } ଅନ୍ୟଥା {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ବାମ;
        }

        bit >>= 1;
    }

    ଫେରନ୍ତୁ ମୂଲ୍ୟ;
}


#ଯଦି (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_ଭର୍ତ୍ତି(ngx_radix_ବୃକ୍ଷ_t *ବୃକ୍ଷ, u_char *ଚାବି, u_char *mask,
    uintptr_t ମୂଲ୍ୟ)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_ଗଣ୍ଠି_t  *ଗଣ୍ଠି, *next;

    i = 0;
    bit = 0x80;

    ଗଣ୍ଠି = ବୃକ୍ଷ->ମୂଳ;
    next = ବୃକ୍ଷ->ମୂଳ;

    ଯେତେବେଳେ (bit & mask[i]) {
        ଯଦି (ଚାବି[i] & bit) {
            next = ଗଣ୍ଠି->ଡାହାଣ;

        } ଅନ୍ୟଥା {
            next = ଗଣ୍ଠି->ବାମ;
        }

        ଯଦି (next == ଖାଲି) {
            ଭାଙ୍ଗ;
        }

        bit >>= 1;
        ଗଣ୍ଠି = next;

        ଯଦି (bit == 0) {
            ଯଦି (++i == 16) {
                ଭାଙ୍ଗ;
            }

            bit = 0x80;
        }
    }

    ଯଦି (next) {
        ଯଦି (ଗଣ୍ଠି->ମୂଲ୍ୟ != NGX_RADIX_NO_ମୂଲ୍ୟ) {
            ଫେରନ୍ତୁ NGX_BUSY;
        }

        ଗଣ୍ଠି->ମୂଲ୍ୟ = ମୂଲ୍ୟ;
        ଫେରନ୍ତୁ NGX_OK;
    }

    ଯେତେବେଳେ (bit & mask[i]) {
        next = ngx_radix_alloc(ବୃକ୍ଷ);
        ଯଦି (next == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        next->ଡାହାଣ = ଖାଲି;
        next->ବାମ = ଖାଲି;
        next->parent = ଗଣ୍ଠି;
        next->ମୂଲ୍ୟ = NGX_RADIX_NO_ମୂଲ୍ୟ;

        ଯଦି (ଚାବି[i] & bit) {
            ଗଣ୍ଠି->ଡାହାଣ = next;

        } ଅନ୍ୟଥା {
            ଗଣ୍ଠି->ବାମ = next;
        }

        bit >>= 1;
        ଗଣ୍ଠି = next;

        ଯଦି (bit == 0) {
            ଯଦି (++i == 16) {
                ଭାଙ୍ଗ;
            }

            bit = 0x80;
        }
    }

    ଗଣ୍ଠି->ମୂଲ୍ୟ = ମୂଲ୍ୟ;

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_ବୃକ୍ଷ_t *ବୃକ୍ଷ, u_char *ଚାବି, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_ଗଣ୍ଠି_t  *ଗଣ୍ଠି;

    i = 0;
    bit = 0x80;
    ଗଣ୍ଠି = ବୃକ୍ଷ->ମୂଳ;

    ଯେତେବେଳେ (ଗଣ୍ଠି && (bit & mask[i])) {
        ଯଦି (ଚାବି[i] & bit) {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ଡାହାଣ;

        } ଅନ୍ୟଥା {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ବାମ;
        }

        bit >>= 1;

        ଯଦି (bit == 0) {
            ଯଦି (++i == 16) {
                ଭାଙ୍ଗ;
            }

            bit = 0x80;
        }
    }

    ଯଦି (ଗଣ୍ଠି == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (ଗଣ୍ଠି->ଡାହାଣ || ଗଣ୍ଠି->ବାମ) {
        ଯଦି (ଗଣ୍ଠି->ମୂଲ୍ୟ != NGX_RADIX_NO_ମୂଲ୍ୟ) {
            ଗଣ୍ଠି->ମୂଲ୍ୟ = NGX_RADIX_NO_ମୂଲ୍ୟ;
            ଫେରନ୍ତୁ NGX_OK;
        }

        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ପାଇଁ ( ;; ) {
        ଯଦି (ଗଣ୍ଠି->parent->ଡାହାଣ == ଗଣ୍ଠି) {
            ଗଣ୍ଠି->parent->ଡାହାଣ = ଖାଲି;

        } ଅନ୍ୟଥା {
            ଗଣ୍ଠି->parent->ବାମ = ଖାଲି;
        }

        ଗଣ୍ଠି->ଡାହାଣ = ବୃକ୍ଷ->ମୁକ୍ତ;
        ବୃକ୍ଷ->ମୁକ୍ତ = ଗଣ୍ଠି;

        ଗଣ୍ଠି = ଗଣ୍ଠି->parent;

        ଯଦି (ଗଣ୍ଠି->ଡାହାଣ || ଗଣ୍ଠି->ବାମ) {
            ଭାଙ୍ଗ;
        }

        ଯଦି (ଗଣ୍ଠି->ମୂଲ୍ୟ != NGX_RADIX_NO_ମୂଲ୍ୟ) {
            ଭାଙ୍ଗ;
        }

        ଯଦି (ଗଣ୍ଠି->parent == ଖାଲି) {
            ଭାଙ୍ଗ;
        }
    }

    ଫେରନ୍ତୁ NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_ବୃକ୍ଷ_t *ବୃକ୍ଷ, u_char *ଚାବି)
{
    u_char             bit;
    uintptr_t          ମୂଲ୍ୟ;
    ngx_uint_t         i;
    ngx_radix_ଗଣ୍ଠି_t  *ଗଣ୍ଠି;

    i = 0;
    bit = 0x80;
    ମୂଲ୍ୟ = NGX_RADIX_NO_ମୂଲ୍ୟ;
    ଗଣ୍ଠି = ବୃକ୍ଷ->ମୂଳ;

    ଯେତେବେଳେ (ଗଣ୍ଠି) {
        ଯଦି (ଗଣ୍ଠି->ମୂଲ୍ୟ != NGX_RADIX_NO_ମୂଲ୍ୟ) {
            ମୂଲ୍ୟ = ଗଣ୍ଠି->ମୂଲ୍ୟ;
        }

        ଯଦି (ଚାବି[i] & bit) {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ଡାହାଣ;

        } ଅନ୍ୟଥା {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ବାମ;
        }

        bit >>= 1;

        ଯଦି (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    ଫେରନ୍ତୁ ମୂଲ୍ୟ;
}

#endif


ସ୍ଥିର ngx_radix_ଗଣ୍ଠି_t *
ngx_radix_alloc(ngx_radix_ବୃକ୍ଷ_t *ବୃକ୍ଷ)
{
    ngx_radix_ଗଣ୍ଠି_t  *p;

    ଯଦି (ବୃକ୍ଷ->ମୁକ୍ତ) {
        p = ବୃକ୍ଷ->ମୁକ୍ତ;
        ବୃକ୍ଷ->ମୁକ୍ତ = ବୃକ୍ଷ->ମୁକ୍ତ->ଡାହାଣ;
        ଫେରନ୍ତୁ p;
    }

    ଯଦି (ବୃକ୍ଷ->size < ଆକାର(ngx_radix_ଗଣ୍ଠି_t)) {
        ବୃକ୍ଷ->start = ngx_pmemalign(ବୃକ୍ଷ->pool, ngx_pagesize, ngx_pagesize);
        ଯଦି (ବୃକ୍ଷ->start == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        ବୃକ୍ଷ->size = ngx_pagesize;
    }

    p = (ngx_radix_ଗଣ୍ଠି_t *) ବୃକ୍ଷ->start;
    ବୃକ୍ଷ->start += ଆକାର(ngx_radix_ଗଣ୍ଠି_t);
    ବୃକ୍ଷ->size -= ଆକାର(ngx_radix_ଗଣ୍ଠି_t);

    ଫେରନ୍ତୁ p;
}
