
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_radix_گرہ_t *ngx_radix_alloc(ngx_radix_درخت_t *درخت);


ngx_radix_درخت_t *
ngx_radix_درخت_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           کلید, mask, inc;
    ngx_radix_درخت_t  *درخت;

    درخت = ngx_palloc(pool, حجم(ngx_radix_درخت_t));
    إذا (درخت == عدم) {
        أرجع عدم;
    }

    درخت->pool = pool;
    درخت->حرر = عدم;
    درخت->start = عدم;
    درخت->size = 0;

    درخت->جڑ = ngx_radix_alloc(درخت);
    إذا (درخت->جڑ == عدم) {
        أرجع عدم;
    }

    درخت->جڑ->دایاں = عدم;
    درخت->جڑ->بایاں = عدم;
    درخت->جڑ->parent = عدم;
    درخت->جڑ->قدر = NGX_RADIX_NO_قدر;

    إذا (preallocate == 0) {
        أرجع درخت;
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
        بدل (ngx_pagesize / حجم(ngx_radix_گرہ_t)) {

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

    mask = 0;
    inc = 0x80000000;

    طالما (preallocate--) {

        کلید = 0;
        mask >>= 1;
        mask |= 0x80000000;

        افعل {
            إذا (ngx_radix32tree_داخل_کرو(درخت, کلید, mask, NGX_RADIX_NO_قدر)
                != NGX_OK)
            {
                أرجع عدم;
            }

            کلید += inc;

        } طالما (کلید);

        inc >>= 1;
    }

    أرجع درخت;
}


ngx_int_t
ngx_radix32tree_داخل_کرو(ngx_radix_درخت_t *درخت, uint32_t کلید, uint32_t mask,
    uintptr_t قدر)
{
    uint32_t           bit;
    ngx_radix_گرہ_t  *گرہ, *next;

    bit = 0x80000000;

    گرہ = درخت->جڑ;
    next = درخت->جڑ;

    طالما (bit & mask) {
        إذا (کلید & bit) {
            next = گرہ->دایاں;

        } وإلا {
            next = گرہ->بایاں;
        }

        إذا (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        گرہ = next;
    }

    إذا (next) {
        إذا (گرہ->قدر != NGX_RADIX_NO_قدر) {
            أرجع NGX_BUSY;
        }

        گرہ->قدر = قدر;
        أرجع NGX_OK;
    }

    طالما (bit & mask) {
        next = ngx_radix_alloc(درخت);
        إذا (next == عدم) {
            أرجع NGX_ERROR;
        }

        next->دایاں = عدم;
        next->بایاں = عدم;
        next->parent = گرہ;
        next->قدر = NGX_RADIX_NO_قدر;

        إذا (کلید & bit) {
            گرہ->دایاں = next;

        } وإلا {
            گرہ->بایاں = next;
        }

        bit >>= 1;
        گرہ = next;
    }

    گرہ->قدر = قدر;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_درخت_t *درخت, uint32_t کلید, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_گرہ_t  *گرہ;

    bit = 0x80000000;
    گرہ = درخت->جڑ;

    طالما (گرہ && (bit & mask)) {
        إذا (کلید & bit) {
            گرہ = گرہ->دایاں;

        } وإلا {
            گرہ = گرہ->بایاں;
        }

        bit >>= 1;
    }

    إذا (گرہ == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (گرہ->دایاں || گرہ->بایاں) {
        إذا (گرہ->قدر != NGX_RADIX_NO_قدر) {
            گرہ->قدر = NGX_RADIX_NO_قدر;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (گرہ->parent->دایاں == گرہ) {
            گرہ->parent->دایاں = عدم;

        } وإلا {
            گرہ->parent->بایاں = عدم;
        }

        گرہ->دایاں = درخت->حرر;
        درخت->حرر = گرہ;

        گرہ = گرہ->parent;

        إذا (گرہ->دایاں || گرہ->بایاں) {
            اكسر;
        }

        إذا (گرہ->قدر != NGX_RADIX_NO_قدر) {
            اكسر;
        }

        إذا (گرہ->parent == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_درخت_t *درخت, uint32_t کلید)
{
    uint32_t           bit;
    uintptr_t          قدر;
    ngx_radix_گرہ_t  *گرہ;

    bit = 0x80000000;
    قدر = NGX_RADIX_NO_قدر;
    گرہ = درخت->جڑ;

    طالما (گرہ) {
        إذا (گرہ->قدر != NGX_RADIX_NO_قدر) {
            قدر = گرہ->قدر;
        }

        إذا (کلید & bit) {
            گرہ = گرہ->دایاں;

        } وإلا {
            گرہ = گرہ->بایاں;
        }

        bit >>= 1;
    }

    أرجع قدر;
}


#إذا (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_داخل_کرو(ngx_radix_درخت_t *درخت, u_char *کلید, u_char *mask,
    uintptr_t قدر)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_گرہ_t  *گرہ, *next;

    i = 0;
    bit = 0x80;

    گرہ = درخت->جڑ;
    next = درخت->جڑ;

    طالما (bit & mask[i]) {
        إذا (کلید[i] & bit) {
            next = گرہ->دایاں;

        } وإلا {
            next = گرہ->بایاں;
        }

        إذا (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        گرہ = next;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    إذا (next) {
        إذا (گرہ->قدر != NGX_RADIX_NO_قدر) {
            أرجع NGX_BUSY;
        }

        گرہ->قدر = قدر;
        أرجع NGX_OK;
    }

    طالما (bit & mask[i]) {
        next = ngx_radix_alloc(درخت);
        إذا (next == عدم) {
            أرجع NGX_ERROR;
        }

        next->دایاں = عدم;
        next->بایاں = عدم;
        next->parent = گرہ;
        next->قدر = NGX_RADIX_NO_قدر;

        إذا (کلید[i] & bit) {
            گرہ->دایاں = next;

        } وإلا {
            گرہ->بایاں = next;
        }

        bit >>= 1;
        گرہ = next;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    گرہ->قدر = قدر;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_درخت_t *درخت, u_char *کلید, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_گرہ_t  *گرہ;

    i = 0;
    bit = 0x80;
    گرہ = درخت->جڑ;

    طالما (گرہ && (bit & mask[i])) {
        إذا (کلید[i] & bit) {
            گرہ = گرہ->دایاں;

        } وإلا {
            گرہ = گرہ->بایاں;
        }

        bit >>= 1;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    إذا (گرہ == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (گرہ->دایاں || گرہ->بایاں) {
        إذا (گرہ->قدر != NGX_RADIX_NO_قدر) {
            گرہ->قدر = NGX_RADIX_NO_قدر;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (گرہ->parent->دایاں == گرہ) {
            گرہ->parent->دایاں = عدم;

        } وإلا {
            گرہ->parent->بایاں = عدم;
        }

        گرہ->دایاں = درخت->حرر;
        درخت->حرر = گرہ;

        گرہ = گرہ->parent;

        إذا (گرہ->دایاں || گرہ->بایاں) {
            اكسر;
        }

        إذا (گرہ->قدر != NGX_RADIX_NO_قدر) {
            اكسر;
        }

        إذا (گرہ->parent == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_درخت_t *درخت, u_char *کلید)
{
    u_char             bit;
    uintptr_t          قدر;
    ngx_uint_t         i;
    ngx_radix_گرہ_t  *گرہ;

    i = 0;
    bit = 0x80;
    قدر = NGX_RADIX_NO_قدر;
    گرہ = درخت->جڑ;

    طالما (گرہ) {
        إذا (گرہ->قدر != NGX_RADIX_NO_قدر) {
            قدر = گرہ->قدر;
        }

        إذا (کلید[i] & bit) {
            گرہ = گرہ->دایاں;

        } وإلا {
            گرہ = گرہ->بایاں;
        }

        bit >>= 1;

        إذا (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    أرجع قدر;
}

#endif


ساكن ngx_radix_گرہ_t *
ngx_radix_alloc(ngx_radix_درخت_t *درخت)
{
    ngx_radix_گرہ_t  *p;

    إذا (درخت->حرر) {
        p = درخت->حرر;
        درخت->حرر = درخت->حرر->دایاں;
        أرجع p;
    }

    إذا (درخت->size < حجم(ngx_radix_گرہ_t)) {
        درخت->start = ngx_pmemalign(درخت->pool, ngx_pagesize, ngx_pagesize);
        إذا (درخت->start == عدم) {
            أرجع عدم;
        }

        درخت->size = ngx_pagesize;
    }

    p = (ngx_radix_گرہ_t *) درخت->start;
    درخت->start += حجم(ngx_radix_گرہ_t);
    درخت->size -= حجم(ngx_radix_گرہ_t);

    أرجع p;
}
