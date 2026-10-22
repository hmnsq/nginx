
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_radix_گرێ_t *ngx_radix_alloc(ngx_radix_درەخت_t *درەخت);


ngx_radix_درەخت_t *
ngx_radix_درەخت_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           کلیل, mask, inc;
    ngx_radix_درەخت_t  *درەخت;

    درەخت = ngx_palloc(pool, حجم(ngx_radix_درەخت_t));
    إذا (درەخت == عدم) {
        أرجع عدم;
    }

    درەخت->pool = pool;
    درەخت->حرر = عدم;
    درەخت->start = عدم;
    درەخت->size = 0;

    درەخت->ڕەگ = ngx_radix_alloc(درەخت);
    إذا (درەخت->ڕەگ == عدم) {
        أرجع عدم;
    }

    درەخت->ڕەگ->ڕاست = عدم;
    درەخت->ڕەگ->چەپ = عدم;
    درەخت->ڕەگ->parent = عدم;
    درەخت->ڕەگ->بەها = NGX_RADIX_NO_بەها;

    إذا (preallocate == 0) {
        أرجع درەخت;
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
        بدل (ngx_pagesize / حجم(ngx_radix_گرێ_t)) {

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

        کلیل = 0;
        mask >>= 1;
        mask |= 0x80000000;

        افعل {
            إذا (ngx_radix32tree_زیادکردن(درەخت, کلیل, mask, NGX_RADIX_NO_بەها)
                != NGX_OK)
            {
                أرجع عدم;
            }

            کلیل += inc;

        } طالما (کلیل);

        inc >>= 1;
    }

    أرجع درەخت;
}


ngx_int_t
ngx_radix32tree_زیادکردن(ngx_radix_درەخت_t *درەخت, uint32_t کلیل, uint32_t mask,
    uintptr_t بەها)
{
    uint32_t           bit;
    ngx_radix_گرێ_t  *گرێ, *next;

    bit = 0x80000000;

    گرێ = درەخت->ڕەگ;
    next = درەخت->ڕەگ;

    طالما (bit & mask) {
        إذا (کلیل & bit) {
            next = گرێ->ڕاست;

        } وإلا {
            next = گرێ->چەپ;
        }

        إذا (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        گرێ = next;
    }

    إذا (next) {
        إذا (گرێ->بەها != NGX_RADIX_NO_بەها) {
            أرجع NGX_BUSY;
        }

        گرێ->بەها = بەها;
        أرجع NGX_OK;
    }

    طالما (bit & mask) {
        next = ngx_radix_alloc(درەخت);
        إذا (next == عدم) {
            أرجع NGX_ERROR;
        }

        next->ڕاست = عدم;
        next->چەپ = عدم;
        next->parent = گرێ;
        next->بەها = NGX_RADIX_NO_بەها;

        إذا (کلیل & bit) {
            گرێ->ڕاست = next;

        } وإلا {
            گرێ->چەپ = next;
        }

        bit >>= 1;
        گرێ = next;
    }

    گرێ->بەها = بەها;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_درەخت_t *درەخت, uint32_t کلیل, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_گرێ_t  *گرێ;

    bit = 0x80000000;
    گرێ = درەخت->ڕەگ;

    طالما (گرێ && (bit & mask)) {
        إذا (کلیل & bit) {
            گرێ = گرێ->ڕاست;

        } وإلا {
            گرێ = گرێ->چەپ;
        }

        bit >>= 1;
    }

    إذا (گرێ == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (گرێ->ڕاست || گرێ->چەپ) {
        إذا (گرێ->بەها != NGX_RADIX_NO_بەها) {
            گرێ->بەها = NGX_RADIX_NO_بەها;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (گرێ->parent->ڕاست == گرێ) {
            گرێ->parent->ڕاست = عدم;

        } وإلا {
            گرێ->parent->چەپ = عدم;
        }

        گرێ->ڕاست = درەخت->حرر;
        درەخت->حرر = گرێ;

        گرێ = گرێ->parent;

        إذا (گرێ->ڕاست || گرێ->چەپ) {
            اكسر;
        }

        إذا (گرێ->بەها != NGX_RADIX_NO_بەها) {
            اكسر;
        }

        إذا (گرێ->parent == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_درەخت_t *درەخت, uint32_t کلیل)
{
    uint32_t           bit;
    uintptr_t          بەها;
    ngx_radix_گرێ_t  *گرێ;

    bit = 0x80000000;
    بەها = NGX_RADIX_NO_بەها;
    گرێ = درەخت->ڕەگ;

    طالما (گرێ) {
        إذا (گرێ->بەها != NGX_RADIX_NO_بەها) {
            بەها = گرێ->بەها;
        }

        إذا (کلیل & bit) {
            گرێ = گرێ->ڕاست;

        } وإلا {
            گرێ = گرێ->چەپ;
        }

        bit >>= 1;
    }

    أرجع بەها;
}


#إذا (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_زیادکردن(ngx_radix_درەخت_t *درەخت, u_char *کلیل, u_char *mask,
    uintptr_t بەها)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_گرێ_t  *گرێ, *next;

    i = 0;
    bit = 0x80;

    گرێ = درەخت->ڕەگ;
    next = درەخت->ڕەگ;

    طالما (bit & mask[i]) {
        إذا (کلیل[i] & bit) {
            next = گرێ->ڕاست;

        } وإلا {
            next = گرێ->چەپ;
        }

        إذا (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        گرێ = next;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    إذا (next) {
        إذا (گرێ->بەها != NGX_RADIX_NO_بەها) {
            أرجع NGX_BUSY;
        }

        گرێ->بەها = بەها;
        أرجع NGX_OK;
    }

    طالما (bit & mask[i]) {
        next = ngx_radix_alloc(درەخت);
        إذا (next == عدم) {
            أرجع NGX_ERROR;
        }

        next->ڕاست = عدم;
        next->چەپ = عدم;
        next->parent = گرێ;
        next->بەها = NGX_RADIX_NO_بەها;

        إذا (کلیل[i] & bit) {
            گرێ->ڕاست = next;

        } وإلا {
            گرێ->چەپ = next;
        }

        bit >>= 1;
        گرێ = next;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    گرێ->بەها = بەها;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_درەخت_t *درەخت, u_char *کلیل, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_گرێ_t  *گرێ;

    i = 0;
    bit = 0x80;
    گرێ = درەخت->ڕەگ;

    طالما (گرێ && (bit & mask[i])) {
        إذا (کلیل[i] & bit) {
            گرێ = گرێ->ڕاست;

        } وإلا {
            گرێ = گرێ->چەپ;
        }

        bit >>= 1;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    إذا (گرێ == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (گرێ->ڕاست || گرێ->چەپ) {
        إذا (گرێ->بەها != NGX_RADIX_NO_بەها) {
            گرێ->بەها = NGX_RADIX_NO_بەها;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (گرێ->parent->ڕاست == گرێ) {
            گرێ->parent->ڕاست = عدم;

        } وإلا {
            گرێ->parent->چەپ = عدم;
        }

        گرێ->ڕاست = درەخت->حرر;
        درەخت->حرر = گرێ;

        گرێ = گرێ->parent;

        إذا (گرێ->ڕاست || گرێ->چەپ) {
            اكسر;
        }

        إذا (گرێ->بەها != NGX_RADIX_NO_بەها) {
            اكسر;
        }

        إذا (گرێ->parent == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_درەخت_t *درەخت, u_char *کلیل)
{
    u_char             bit;
    uintptr_t          بەها;
    ngx_uint_t         i;
    ngx_radix_گرێ_t  *گرێ;

    i = 0;
    bit = 0x80;
    بەها = NGX_RADIX_NO_بەها;
    گرێ = درەخت->ڕەگ;

    طالما (گرێ) {
        إذا (گرێ->بەها != NGX_RADIX_NO_بەها) {
            بەها = گرێ->بەها;
        }

        إذا (کلیل[i] & bit) {
            گرێ = گرێ->ڕاست;

        } وإلا {
            گرێ = گرێ->چەپ;
        }

        bit >>= 1;

        إذا (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    أرجع بەها;
}

#endif


ساكن ngx_radix_گرێ_t *
ngx_radix_alloc(ngx_radix_درەخت_t *درەخت)
{
    ngx_radix_گرێ_t  *p;

    إذا (درەخت->حرر) {
        p = درەخت->حرر;
        درەخت->حرر = درەخت->حرر->ڕاست;
        أرجع p;
    }

    إذا (درەخت->size < حجم(ngx_radix_گرێ_t)) {
        درەخت->start = ngx_pmemalign(درەخت->pool, ngx_pagesize, ngx_pagesize);
        إذا (درەخت->start == عدم) {
            أرجع عدم;
        }

        درەخت->size = ngx_pagesize;
    }

    p = (ngx_radix_گرێ_t *) درەخت->start;
    درەخت->start += حجم(ngx_radix_گرێ_t);
    درەخت->size -= حجم(ngx_radix_گرێ_t);

    أرجع p;
}
