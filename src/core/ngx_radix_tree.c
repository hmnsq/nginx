
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_radix_تۈگۈن_t *ngx_radix_alloc(ngx_radix_دەرەخ_t *دەرەخ);


ngx_radix_دەرەخ_t *
ngx_radix_دەرەخ_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           ئاچقۇچ, mask, inc;
    ngx_radix_دەرەخ_t  *دەرەخ;

    دەرەخ = ngx_palloc(pool, حجم(ngx_radix_دەرەخ_t));
    إذا (دەرەخ == عدم) {
        أرجع عدم;
    }

    دەرەخ->pool = pool;
    دەرەخ->حرر = عدم;
    دەرەخ->start = عدم;
    دەرەخ->size = 0;

    دەرەخ->يىلتىز = ngx_radix_alloc(دەرەخ);
    إذا (دەرەخ->يىلتىز == عدم) {
        أرجع عدم;
    }

    دەرەخ->يىلتىز->ئوڭ = عدم;
    دەرەخ->يىلتىز->سول = عدم;
    دەرەخ->يىلتىز->parent = عدم;
    دەرەخ->يىلتىز->قىممەت = NGX_RADIX_NO_قىممەت;

    إذا (preallocate == 0) {
        أرجع دەرەخ;
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
        بدل (ngx_pagesize / حجم(ngx_radix_تۈگۈن_t)) {

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

        ئاچقۇچ = 0;
        mask >>= 1;
        mask |= 0x80000000;

        افعل {
            إذا (ngx_radix32tree_قىستۇرۇش(دەرەخ, ئاچقۇچ, mask, NGX_RADIX_NO_قىممەت)
                != NGX_OK)
            {
                أرجع عدم;
            }

            ئاچقۇچ += inc;

        } طالما (ئاچقۇچ);

        inc >>= 1;
    }

    أرجع دەرەخ;
}


ngx_int_t
ngx_radix32tree_قىستۇرۇش(ngx_radix_دەرەخ_t *دەرەخ, uint32_t ئاچقۇچ, uint32_t mask,
    uintptr_t قىممەت)
{
    uint32_t           bit;
    ngx_radix_تۈگۈن_t  *تۈگۈن, *next;

    bit = 0x80000000;

    تۈگۈن = دەرەخ->يىلتىز;
    next = دەرەخ->يىلتىز;

    طالما (bit & mask) {
        إذا (ئاچقۇچ & bit) {
            next = تۈگۈن->ئوڭ;

        } وإلا {
            next = تۈگۈن->سول;
        }

        إذا (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        تۈگۈن = next;
    }

    إذا (next) {
        إذا (تۈگۈن->قىممەت != NGX_RADIX_NO_قىممەت) {
            أرجع NGX_BUSY;
        }

        تۈگۈن->قىممەت = قىممەت;
        أرجع NGX_OK;
    }

    طالما (bit & mask) {
        next = ngx_radix_alloc(دەرەخ);
        إذا (next == عدم) {
            أرجع NGX_ERROR;
        }

        next->ئوڭ = عدم;
        next->سول = عدم;
        next->parent = تۈگۈن;
        next->قىممەت = NGX_RADIX_NO_قىممەت;

        إذا (ئاچقۇچ & bit) {
            تۈگۈن->ئوڭ = next;

        } وإلا {
            تۈگۈن->سول = next;
        }

        bit >>= 1;
        تۈگۈن = next;
    }

    تۈگۈن->قىممەت = قىممەت;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_دەرەخ_t *دەرەخ, uint32_t ئاچقۇچ, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_تۈگۈن_t  *تۈگۈن;

    bit = 0x80000000;
    تۈگۈن = دەرەخ->يىلتىز;

    طالما (تۈگۈن && (bit & mask)) {
        إذا (ئاچقۇچ & bit) {
            تۈگۈن = تۈگۈن->ئوڭ;

        } وإلا {
            تۈگۈن = تۈگۈن->سول;
        }

        bit >>= 1;
    }

    إذا (تۈگۈن == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (تۈگۈن->ئوڭ || تۈگۈن->سول) {
        إذا (تۈگۈن->قىممەت != NGX_RADIX_NO_قىممەت) {
            تۈگۈن->قىممەت = NGX_RADIX_NO_قىممەت;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (تۈگۈن->parent->ئوڭ == تۈگۈن) {
            تۈگۈن->parent->ئوڭ = عدم;

        } وإلا {
            تۈگۈن->parent->سول = عدم;
        }

        تۈگۈن->ئوڭ = دەرەخ->حرر;
        دەرەخ->حرر = تۈگۈن;

        تۈگۈن = تۈگۈن->parent;

        إذا (تۈگۈن->ئوڭ || تۈگۈن->سول) {
            اكسر;
        }

        إذا (تۈگۈن->قىممەت != NGX_RADIX_NO_قىممەت) {
            اكسر;
        }

        إذا (تۈگۈن->parent == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_دەرەخ_t *دەرەخ, uint32_t ئاچقۇچ)
{
    uint32_t           bit;
    uintptr_t          قىممەت;
    ngx_radix_تۈگۈن_t  *تۈگۈن;

    bit = 0x80000000;
    قىممەت = NGX_RADIX_NO_قىممەت;
    تۈگۈن = دەرەخ->يىلتىز;

    طالما (تۈگۈن) {
        إذا (تۈگۈن->قىممەت != NGX_RADIX_NO_قىممەت) {
            قىممەت = تۈگۈن->قىممەت;
        }

        إذا (ئاچقۇچ & bit) {
            تۈگۈن = تۈگۈن->ئوڭ;

        } وإلا {
            تۈگۈن = تۈگۈن->سول;
        }

        bit >>= 1;
    }

    أرجع قىممەت;
}


#إذا (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_قىستۇرۇش(ngx_radix_دەرەخ_t *دەرەخ, u_char *ئاچقۇچ, u_char *mask,
    uintptr_t قىممەت)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_تۈگۈن_t  *تۈگۈن, *next;

    i = 0;
    bit = 0x80;

    تۈگۈن = دەرەخ->يىلتىز;
    next = دەرەخ->يىلتىز;

    طالما (bit & mask[i]) {
        إذا (ئاچقۇچ[i] & bit) {
            next = تۈگۈن->ئوڭ;

        } وإلا {
            next = تۈگۈن->سول;
        }

        إذا (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        تۈگۈن = next;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    إذا (next) {
        إذا (تۈگۈن->قىممەت != NGX_RADIX_NO_قىممەت) {
            أرجع NGX_BUSY;
        }

        تۈگۈن->قىممەت = قىممەت;
        أرجع NGX_OK;
    }

    طالما (bit & mask[i]) {
        next = ngx_radix_alloc(دەرەخ);
        إذا (next == عدم) {
            أرجع NGX_ERROR;
        }

        next->ئوڭ = عدم;
        next->سول = عدم;
        next->parent = تۈگۈن;
        next->قىممەت = NGX_RADIX_NO_قىممەت;

        إذا (ئاچقۇچ[i] & bit) {
            تۈگۈن->ئوڭ = next;

        } وإلا {
            تۈگۈن->سول = next;
        }

        bit >>= 1;
        تۈگۈن = next;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    تۈگۈن->قىممەت = قىممەت;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_دەرەخ_t *دەرەخ, u_char *ئاچقۇچ, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_تۈگۈن_t  *تۈگۈن;

    i = 0;
    bit = 0x80;
    تۈگۈن = دەرەخ->يىلتىز;

    طالما (تۈگۈن && (bit & mask[i])) {
        إذا (ئاچقۇچ[i] & bit) {
            تۈگۈن = تۈگۈن->ئوڭ;

        } وإلا {
            تۈگۈن = تۈگۈن->سول;
        }

        bit >>= 1;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    إذا (تۈگۈن == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (تۈگۈن->ئوڭ || تۈگۈن->سول) {
        إذا (تۈگۈن->قىممەت != NGX_RADIX_NO_قىممەت) {
            تۈگۈن->قىممەت = NGX_RADIX_NO_قىممەت;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (تۈگۈن->parent->ئوڭ == تۈگۈن) {
            تۈگۈن->parent->ئوڭ = عدم;

        } وإلا {
            تۈگۈن->parent->سول = عدم;
        }

        تۈگۈن->ئوڭ = دەرەخ->حرر;
        دەرەخ->حرر = تۈگۈن;

        تۈگۈن = تۈگۈن->parent;

        إذا (تۈگۈن->ئوڭ || تۈگۈن->سول) {
            اكسر;
        }

        إذا (تۈگۈن->قىممەت != NGX_RADIX_NO_قىممەت) {
            اكسر;
        }

        إذا (تۈگۈن->parent == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_دەرەخ_t *دەرەخ, u_char *ئاچقۇچ)
{
    u_char             bit;
    uintptr_t          قىممەت;
    ngx_uint_t         i;
    ngx_radix_تۈگۈن_t  *تۈگۈن;

    i = 0;
    bit = 0x80;
    قىممەت = NGX_RADIX_NO_قىممەت;
    تۈگۈن = دەرەخ->يىلتىز;

    طالما (تۈگۈن) {
        إذا (تۈگۈن->قىممەت != NGX_RADIX_NO_قىممەت) {
            قىممەت = تۈگۈن->قىممەت;
        }

        إذا (ئاچقۇچ[i] & bit) {
            تۈگۈن = تۈگۈن->ئوڭ;

        } وإلا {
            تۈگۈن = تۈگۈن->سول;
        }

        bit >>= 1;

        إذا (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    أرجع قىممەت;
}

#endif


ساكن ngx_radix_تۈگۈن_t *
ngx_radix_alloc(ngx_radix_دەرەخ_t *دەرەخ)
{
    ngx_radix_تۈگۈن_t  *p;

    إذا (دەرەخ->حرر) {
        p = دەرەخ->حرر;
        دەرەخ->حرر = دەرەخ->حرر->ئوڭ;
        أرجع p;
    }

    إذا (دەرەخ->size < حجم(ngx_radix_تۈگۈن_t)) {
        دەرەخ->start = ngx_pmemalign(دەرەخ->pool, ngx_pagesize, ngx_pagesize);
        إذا (دەرەخ->start == عدم) {
            أرجع عدم;
        }

        دەرەخ->size = ngx_pagesize;
    }

    p = (ngx_radix_تۈگۈن_t *) دەرەخ->start;
    دەرەخ->start += حجم(ngx_radix_تۈگۈن_t);
    دەرەخ->size -= حجم(ngx_radix_تۈگۈن_t);

    أرجع p;
}
