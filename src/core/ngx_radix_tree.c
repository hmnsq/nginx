
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_radix_ڳنڍ_t *ngx_radix_alloc(ngx_radix_وڻ_t *وڻ);


ngx_radix_وڻ_t *
ngx_radix_وڻ_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           ڪنجي, نقاب, inc;
    ngx_radix_وڻ_t  *وڻ;

    وڻ = ngx_palloc(pool, حجم(ngx_radix_وڻ_t));
    إذا (وڻ == عدم) {
        أرجع عدم;
    }

    وڻ->pool = pool;
    وڻ->حرر = عدم;
    وڻ->شروعات = عدم;
    وڻ->سائيز = 0;

    وڻ->پاڙ = ngx_radix_alloc(وڻ);
    إذا (وڻ->پاڙ == عدم) {
        أرجع عدم;
    }

    وڻ->پاڙ->ساڄو = عدم;
    وڻ->پاڙ->کاٻو = عدم;
    وڻ->پاڙ->والد = عدم;
    وڻ->پاڙ->قدر = NGX_RADIX_NO_قدر;

    إذا (preallocate == 0) {
        أرجع وڻ;
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

    إذا (preallocate == -1) {
        بدل (ngx_pagesize / حجم(ngx_radix_ڳنڍ_t)) {

        /* amd64 */
        حالة 128:
            preallocate = 6;
            اكسر;

        /* i386, sparc64 */
        حالة 256:
            preallocate = 7;
            اكسر;

        /* sparc64 in 32-bit mode */
        افتراضي:
            preallocate = 8;
        }
    }

    نقاب = 0;
    inc = 0x80000000;

    طالما (preallocate--) {

        ڪنجي = 0;
        نقاب >>= 1;
        نقاب |= 0x80000000;

        افعل {
            إذا (ngx_radix32tree_وجهو(وڻ, ڪنجي, نقاب, NGX_RADIX_NO_قدر)
                != NGX_OK)
            {
                أرجع عدم;
            }

            ڪنجي += inc;

        } طالما (ڪنجي);

        inc >>= 1;
    }

    أرجع وڻ;
}


ngx_int_t
ngx_radix32tree_وجهو(ngx_radix_وڻ_t *وڻ, uint32_t ڪنجي, uint32_t نقاب,
    uintptr_t قدر)
{
    uint32_t           ٻٽو_انگ;
    ngx_radix_ڳنڍ_t  *ڳنڍ, *ايندڙ;

    ٻٽو_انگ = 0x80000000;

    ڳنڍ = وڻ->پاڙ;
    ايندڙ = وڻ->پاڙ;

    طالما (ٻٽو_انگ & نقاب) {
        إذا (ڪنجي & ٻٽو_انگ) {
            ايندڙ = ڳنڍ->ساڄو;

        } وإلا {
            ايندڙ = ڳنڍ->کاٻو;
        }

        إذا (ايندڙ == عدم) {
            اكسر;
        }

        ٻٽو_انگ >>= 1;
        ڳنڍ = ايندڙ;
    }

    إذا (ايندڙ) {
        إذا (ڳنڍ->قدر != NGX_RADIX_NO_قدر) {
            أرجع NGX_BUSY;
        }

        ڳنڍ->قدر = قدر;
        أرجع NGX_OK;
    }

    طالما (ٻٽو_انگ & نقاب) {
        ايندڙ = ngx_radix_alloc(وڻ);
        إذا (ايندڙ == عدم) {
            أرجع NGX_ERROR;
        }

        ايندڙ->ساڄو = عدم;
        ايندڙ->کاٻو = عدم;
        ايندڙ->والد = ڳنڍ;
        ايندڙ->قدر = NGX_RADIX_NO_قدر;

        إذا (ڪنجي & ٻٽو_انگ) {
            ڳنڍ->ساڄو = ايندڙ;

        } وإلا {
            ڳنڍ->کاٻو = ايندڙ;
        }

        ٻٽو_انگ >>= 1;
        ڳنڍ = ايندڙ;
    }

    ڳنڍ->قدر = قدر;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_وڻ_t *وڻ, uint32_t ڪنجي, uint32_t نقاب)
{
    uint32_t           ٻٽو_انگ;
    ngx_radix_ڳنڍ_t  *ڳنڍ;

    ٻٽو_انگ = 0x80000000;
    ڳنڍ = وڻ->پاڙ;

    طالما (ڳنڍ && (ٻٽو_انگ & نقاب)) {
        إذا (ڪنجي & ٻٽو_انگ) {
            ڳنڍ = ڳنڍ->ساڄو;

        } وإلا {
            ڳنڍ = ڳنڍ->کاٻو;
        }

        ٻٽو_انگ >>= 1;
    }

    إذا (ڳنڍ == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (ڳنڍ->ساڄو || ڳنڍ->کاٻو) {
        إذا (ڳنڍ->قدر != NGX_RADIX_NO_قدر) {
            ڳنڍ->قدر = NGX_RADIX_NO_قدر;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (ڳنڍ->والد->ساڄو == ڳنڍ) {
            ڳنڍ->والد->ساڄو = عدم;

        } وإلا {
            ڳنڍ->والد->کاٻو = عدم;
        }

        ڳنڍ->ساڄو = وڻ->حرر;
        وڻ->حرر = ڳنڍ;

        ڳنڍ = ڳنڍ->والد;

        إذا (ڳنڍ->ساڄو || ڳنڍ->کاٻو) {
            اكسر;
        }

        إذا (ڳنڍ->قدر != NGX_RADIX_NO_قدر) {
            اكسر;
        }

        إذا (ڳنڍ->والد == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix32tree_ڳوليو(ngx_radix_وڻ_t *وڻ, uint32_t ڪنجي)
{
    uint32_t           ٻٽو_انگ;
    uintptr_t          قدر;
    ngx_radix_ڳنڍ_t  *ڳنڍ;

    ٻٽو_انگ = 0x80000000;
    قدر = NGX_RADIX_NO_قدر;
    ڳنڍ = وڻ->پاڙ;

    طالما (ڳنڍ) {
        إذا (ڳنڍ->قدر != NGX_RADIX_NO_قدر) {
            قدر = ڳنڍ->قدر;
        }

        إذا (ڪنجي & ٻٽو_انگ) {
            ڳنڍ = ڳنڍ->ساڄو;

        } وإلا {
            ڳنڍ = ڳنڍ->کاٻو;
        }

        ٻٽو_انگ >>= 1;
    }

    أرجع قدر;
}


#إذا (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_وجهو(ngx_radix_وڻ_t *وڻ, u_char *ڪنجي, u_char *نقاب,
    uintptr_t قدر)
{
    u_char             ٻٽو_انگ;
    ngx_uint_t         i;
    ngx_radix_ڳنڍ_t  *ڳنڍ, *ايندڙ;

    i = 0;
    ٻٽو_انگ = 0x80;

    ڳنڍ = وڻ->پاڙ;
    ايندڙ = وڻ->پاڙ;

    طالما (ٻٽو_انگ & نقاب[i]) {
        إذا (ڪنجي[i] & ٻٽو_انگ) {
            ايندڙ = ڳنڍ->ساڄو;

        } وإلا {
            ايندڙ = ڳنڍ->کاٻو;
        }

        إذا (ايندڙ == عدم) {
            اكسر;
        }

        ٻٽو_انگ >>= 1;
        ڳنڍ = ايندڙ;

        إذا (ٻٽو_انگ == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            ٻٽو_انگ = 0x80;
        }
    }

    إذا (ايندڙ) {
        إذا (ڳنڍ->قدر != NGX_RADIX_NO_قدر) {
            أرجع NGX_BUSY;
        }

        ڳنڍ->قدر = قدر;
        أرجع NGX_OK;
    }

    طالما (ٻٽو_انگ & نقاب[i]) {
        ايندڙ = ngx_radix_alloc(وڻ);
        إذا (ايندڙ == عدم) {
            أرجع NGX_ERROR;
        }

        ايندڙ->ساڄو = عدم;
        ايندڙ->کاٻو = عدم;
        ايندڙ->والد = ڳنڍ;
        ايندڙ->قدر = NGX_RADIX_NO_قدر;

        إذا (ڪنجي[i] & ٻٽو_انگ) {
            ڳنڍ->ساڄو = ايندڙ;

        } وإلا {
            ڳنڍ->کاٻو = ايندڙ;
        }

        ٻٽو_انگ >>= 1;
        ڳنڍ = ايندڙ;

        إذا (ٻٽو_انگ == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            ٻٽو_انگ = 0x80;
        }
    }

    ڳنڍ->قدر = قدر;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_وڻ_t *وڻ, u_char *ڪنجي, u_char *نقاب)
{
    u_char             ٻٽو_انگ;
    ngx_uint_t         i;
    ngx_radix_ڳنڍ_t  *ڳنڍ;

    i = 0;
    ٻٽو_انگ = 0x80;
    ڳنڍ = وڻ->پاڙ;

    طالما (ڳنڍ && (ٻٽو_انگ & نقاب[i])) {
        إذا (ڪنجي[i] & ٻٽو_انگ) {
            ڳنڍ = ڳنڍ->ساڄو;

        } وإلا {
            ڳنڍ = ڳنڍ->کاٻو;
        }

        ٻٽو_انگ >>= 1;

        إذا (ٻٽو_انگ == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            ٻٽو_انگ = 0x80;
        }
    }

    إذا (ڳنڍ == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (ڳنڍ->ساڄو || ڳنڍ->کاٻو) {
        إذا (ڳنڍ->قدر != NGX_RADIX_NO_قدر) {
            ڳنڍ->قدر = NGX_RADIX_NO_قدر;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (ڳنڍ->والد->ساڄو == ڳنڍ) {
            ڳنڍ->والد->ساڄو = عدم;

        } وإلا {
            ڳنڍ->والد->کاٻو = عدم;
        }

        ڳنڍ->ساڄو = وڻ->حرر;
        وڻ->حرر = ڳنڍ;

        ڳنڍ = ڳنڍ->والد;

        إذا (ڳنڍ->ساڄو || ڳنڍ->کاٻو) {
            اكسر;
        }

        إذا (ڳنڍ->قدر != NGX_RADIX_NO_قدر) {
            اكسر;
        }

        إذا (ڳنڍ->والد == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix128tree_ڳوليو(ngx_radix_وڻ_t *وڻ, u_char *ڪنجي)
{
    u_char             ٻٽو_انگ;
    uintptr_t          قدر;
    ngx_uint_t         i;
    ngx_radix_ڳنڍ_t  *ڳنڍ;

    i = 0;
    ٻٽو_انگ = 0x80;
    قدر = NGX_RADIX_NO_قدر;
    ڳنڍ = وڻ->پاڙ;

    طالما (ڳنڍ) {
        إذا (ڳنڍ->قدر != NGX_RADIX_NO_قدر) {
            قدر = ڳنڍ->قدر;
        }

        إذا (ڪنجي[i] & ٻٽو_انگ) {
            ڳنڍ = ڳنڍ->ساڄو;

        } وإلا {
            ڳنڍ = ڳنڍ->کاٻو;
        }

        ٻٽو_انگ >>= 1;

        إذا (ٻٽو_انگ == 0) {
            i++;
            ٻٽو_انگ = 0x80;
        }
    }

    أرجع قدر;
}

#جيڪڏهن_پورو


ساكن ngx_radix_ڳنڍ_t *
ngx_radix_alloc(ngx_radix_وڻ_t *وڻ)
{
    ngx_radix_ڳنڍ_t  *p;

    إذا (وڻ->حرر) {
        p = وڻ->حرر;
        وڻ->حرر = وڻ->حرر->ساڄو;
        أرجع p;
    }

    إذا (وڻ->سائيز < حجم(ngx_radix_ڳنڍ_t)) {
        وڻ->شروعات = ngx_pmemalign(وڻ->pool, ngx_pagesize, ngx_pagesize);
        إذا (وڻ->شروعات == عدم) {
            أرجع عدم;
        }

        وڻ->سائيز = ngx_pagesize;
    }

    p = (ngx_radix_ڳنڍ_t *) وڻ->شروعات;
    وڻ->شروعات += حجم(ngx_radix_ڳنڍ_t);
    وڻ->سائيز -= حجم(ngx_radix_ڳنڍ_t);

    أرجع p;
}
