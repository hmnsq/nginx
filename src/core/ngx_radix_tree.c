
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_radix_نود_t *ngx_radix_alloc(ngx_radix_ڤوکوق_t *ڤوکوق);


ngx_radix_ڤوکوق_t *
ngx_radix_ڤوکوق_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           کونچي, mask, inc;
    ngx_radix_ڤوکوق_t  *ڤوکوق;

    ڤوکوق = ngx_palloc(pool, حجم(ngx_radix_ڤوکوق_t));
    إذا (ڤوکوق == عدم) {
        أرجع عدم;
    }

    ڤوکوق->pool = pool;
    ڤوکوق->حرر = عدم;
    ڤوکوق->start = عدم;
    ڤوکوق->size = 0;

    ڤوکوق->اکر = ngx_radix_alloc(ڤوکوق);
    إذا (ڤوکوق->اکر == عدم) {
        أرجع عدم;
    }

    ڤوکوق->اکر->کانن = عدم;
    ڤوکوق->اکر->کيري = عدم;
    ڤوکوق->اکر->parent = عدم;
    ڤوکوق->اکر->نيلاي = NGX_RADIX_NO_نيلاي;

    إذا (preallocate == 0) {
        أرجع ڤوکوق;
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
        بدل (ngx_pagesize / حجم(ngx_radix_نود_t)) {

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

        کونچي = 0;
        mask >>= 1;
        mask |= 0x80000000;

        افعل {
            إذا (ngx_radix32tree_سيسيڤ(ڤوکوق, کونچي, mask, NGX_RADIX_NO_نيلاي)
                != NGX_OK)
            {
                أرجع عدم;
            }

            کونچي += inc;

        } طالما (کونچي);

        inc >>= 1;
    }

    أرجع ڤوکوق;
}


ngx_int_t
ngx_radix32tree_سيسيڤ(ngx_radix_ڤوکوق_t *ڤوکوق, uint32_t کونچي, uint32_t mask,
    uintptr_t نيلاي)
{
    uint32_t           bit;
    ngx_radix_نود_t  *نود, *next;

    bit = 0x80000000;

    نود = ڤوکوق->اکر;
    next = ڤوکوق->اکر;

    طالما (bit & mask) {
        إذا (کونچي & bit) {
            next = نود->کانن;

        } وإلا {
            next = نود->کيري;
        }

        إذا (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        نود = next;
    }

    إذا (next) {
        إذا (نود->نيلاي != NGX_RADIX_NO_نيلاي) {
            أرجع NGX_BUSY;
        }

        نود->نيلاي = نيلاي;
        أرجع NGX_OK;
    }

    طالما (bit & mask) {
        next = ngx_radix_alloc(ڤوکوق);
        إذا (next == عدم) {
            أرجع NGX_ERROR;
        }

        next->کانن = عدم;
        next->کيري = عدم;
        next->parent = نود;
        next->نيلاي = NGX_RADIX_NO_نيلاي;

        إذا (کونچي & bit) {
            نود->کانن = next;

        } وإلا {
            نود->کيري = next;
        }

        bit >>= 1;
        نود = next;
    }

    نود->نيلاي = نيلاي;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_ڤوکوق_t *ڤوکوق, uint32_t کونچي, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_نود_t  *نود;

    bit = 0x80000000;
    نود = ڤوکوق->اکر;

    طالما (نود && (bit & mask)) {
        إذا (کونچي & bit) {
            نود = نود->کانن;

        } وإلا {
            نود = نود->کيري;
        }

        bit >>= 1;
    }

    إذا (نود == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (نود->کانن || نود->کيري) {
        إذا (نود->نيلاي != NGX_RADIX_NO_نيلاي) {
            نود->نيلاي = NGX_RADIX_NO_نيلاي;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (نود->parent->کانن == نود) {
            نود->parent->کانن = عدم;

        } وإلا {
            نود->parent->کيري = عدم;
        }

        نود->کانن = ڤوکوق->حرر;
        ڤوکوق->حرر = نود;

        نود = نود->parent;

        إذا (نود->کانن || نود->کيري) {
            اكسر;
        }

        إذا (نود->نيلاي != NGX_RADIX_NO_نيلاي) {
            اكسر;
        }

        إذا (نود->parent == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_ڤوکوق_t *ڤوکوق, uint32_t کونچي)
{
    uint32_t           bit;
    uintptr_t          نيلاي;
    ngx_radix_نود_t  *نود;

    bit = 0x80000000;
    نيلاي = NGX_RADIX_NO_نيلاي;
    نود = ڤوکوق->اکر;

    طالما (نود) {
        إذا (نود->نيلاي != NGX_RADIX_NO_نيلاي) {
            نيلاي = نود->نيلاي;
        }

        إذا (کونچي & bit) {
            نود = نود->کانن;

        } وإلا {
            نود = نود->کيري;
        }

        bit >>= 1;
    }

    أرجع نيلاي;
}


#إذا (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_سيسيڤ(ngx_radix_ڤوکوق_t *ڤوکوق, u_char *کونچي, u_char *mask,
    uintptr_t نيلاي)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_نود_t  *نود, *next;

    i = 0;
    bit = 0x80;

    نود = ڤوکوق->اکر;
    next = ڤوکوق->اکر;

    طالما (bit & mask[i]) {
        إذا (کونچي[i] & bit) {
            next = نود->کانن;

        } وإلا {
            next = نود->کيري;
        }

        إذا (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        نود = next;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    إذا (next) {
        إذا (نود->نيلاي != NGX_RADIX_NO_نيلاي) {
            أرجع NGX_BUSY;
        }

        نود->نيلاي = نيلاي;
        أرجع NGX_OK;
    }

    طالما (bit & mask[i]) {
        next = ngx_radix_alloc(ڤوکوق);
        إذا (next == عدم) {
            أرجع NGX_ERROR;
        }

        next->کانن = عدم;
        next->کيري = عدم;
        next->parent = نود;
        next->نيلاي = NGX_RADIX_NO_نيلاي;

        إذا (کونچي[i] & bit) {
            نود->کانن = next;

        } وإلا {
            نود->کيري = next;
        }

        bit >>= 1;
        نود = next;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    نود->نيلاي = نيلاي;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_ڤوکوق_t *ڤوکوق, u_char *کونچي, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_نود_t  *نود;

    i = 0;
    bit = 0x80;
    نود = ڤوکوق->اکر;

    طالما (نود && (bit & mask[i])) {
        إذا (کونچي[i] & bit) {
            نود = نود->کانن;

        } وإلا {
            نود = نود->کيري;
        }

        bit >>= 1;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    إذا (نود == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (نود->کانن || نود->کيري) {
        إذا (نود->نيلاي != NGX_RADIX_NO_نيلاي) {
            نود->نيلاي = NGX_RADIX_NO_نيلاي;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (نود->parent->کانن == نود) {
            نود->parent->کانن = عدم;

        } وإلا {
            نود->parent->کيري = عدم;
        }

        نود->کانن = ڤوکوق->حرر;
        ڤوکوق->حرر = نود;

        نود = نود->parent;

        إذا (نود->کانن || نود->کيري) {
            اكسر;
        }

        إذا (نود->نيلاي != NGX_RADIX_NO_نيلاي) {
            اكسر;
        }

        إذا (نود->parent == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_ڤوکوق_t *ڤوکوق, u_char *کونچي)
{
    u_char             bit;
    uintptr_t          نيلاي;
    ngx_uint_t         i;
    ngx_radix_نود_t  *نود;

    i = 0;
    bit = 0x80;
    نيلاي = NGX_RADIX_NO_نيلاي;
    نود = ڤوکوق->اکر;

    طالما (نود) {
        إذا (نود->نيلاي != NGX_RADIX_NO_نيلاي) {
            نيلاي = نود->نيلاي;
        }

        إذا (کونچي[i] & bit) {
            نود = نود->کانن;

        } وإلا {
            نود = نود->کيري;
        }

        bit >>= 1;

        إذا (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    أرجع نيلاي;
}

#endif


ساكن ngx_radix_نود_t *
ngx_radix_alloc(ngx_radix_ڤوکوق_t *ڤوکوق)
{
    ngx_radix_نود_t  *p;

    إذا (ڤوکوق->حرر) {
        p = ڤوکوق->حرر;
        ڤوکوق->حرر = ڤوکوق->حرر->کانن;
        أرجع p;
    }

    إذا (ڤوکوق->size < حجم(ngx_radix_نود_t)) {
        ڤوکوق->start = ngx_pmemalign(ڤوکوق->pool, ngx_pagesize, ngx_pagesize);
        إذا (ڤوکوق->start == عدم) {
            أرجع عدم;
        }

        ڤوکوق->size = ngx_pagesize;
    }

    p = (ngx_radix_نود_t *) ڤوکوق->start;
    ڤوکوق->start += حجم(ngx_radix_نود_t);
    ڤوکوق->size -= حجم(ngx_radix_نود_t);

    أرجع p;
}
