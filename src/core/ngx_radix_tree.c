
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_radix_كلي_t *ngx_radix_alloc(ngx_radix_بشيا_t *بشيا);


ngx_radix_بشيا_t *
ngx_radix_بشيا_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           مبودي, mask, inc;
    ngx_radix_بشيا_t  *بشيا;

    بشيا = ngx_palloc(pool, حجم(ngx_radix_بشيا_t));
    إذا (بشيا == عدم) {
        أرجع عدم;
    }

    بشيا->pool = pool;
    بشيا->حرر = عدم;
    بشيا->start = عدم;
    بشيا->size = 0;

    بشيا->سيو = ngx_radix_alloc(بشيا);
    إذا (بشيا->سيو == عدم) {
        أرجع عدم;
    }

    بشيا->سيو->دام = عدم;
    بشيا->سيو->هغو = عدم;
    بشيا->سيو->parent = عدم;
    بشيا->سيو->درج = NGX_RADIX_NO_درج;

    إذا (preallocate == 0) {
        أرجع بشيا;
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
        بدل (ngx_pagesize / حجم(ngx_radix_كلي_t)) {

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

        مبودي = 0;
        mask >>= 1;
        mask |= 0x80000000;

        افعل {
            إذا (ngx_radix32tree_سك(بشيا, مبودي, mask, NGX_RADIX_NO_درج)
                != NGX_OK)
            {
                أرجع عدم;
            }

            مبودي += inc;

        } طالما (مبودي);

        inc >>= 1;
    }

    أرجع بشيا;
}


ngx_int_t
ngx_radix32tree_سك(ngx_radix_بشيا_t *بشيا, uint32_t مبودي, uint32_t mask,
    uintptr_t درج)
{
    uint32_t           bit;
    ngx_radix_كلي_t  *كلي, *next;

    bit = 0x80000000;

    كلي = بشيا->سيو;
    next = بشيا->سيو;

    طالما (bit & mask) {
        إذا (مبودي & bit) {
            next = كلي->دام;

        } وإلا {
            next = كلي->هغو;
        }

        إذا (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        كلي = next;
    }

    إذا (next) {
        إذا (كلي->درج != NGX_RADIX_NO_درج) {
            أرجع NGX_BUSY;
        }

        كلي->درج = درج;
        أرجع NGX_OK;
    }

    طالما (bit & mask) {
        next = ngx_radix_alloc(بشيا);
        إذا (next == عدم) {
            أرجع NGX_ERROR;
        }

        next->دام = عدم;
        next->هغو = عدم;
        next->parent = كلي;
        next->درج = NGX_RADIX_NO_درج;

        إذا (مبودي & bit) {
            كلي->دام = next;

        } وإلا {
            كلي->هغو = next;
        }

        bit >>= 1;
        كلي = next;
    }

    كلي->درج = درج;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_بشيا_t *بشيا, uint32_t مبودي, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_كلي_t  *كلي;

    bit = 0x80000000;
    كلي = بشيا->سيو;

    طالما (كلي && (bit & mask)) {
        إذا (مبودي & bit) {
            كلي = كلي->دام;

        } وإلا {
            كلي = كلي->هغو;
        }

        bit >>= 1;
    }

    إذا (كلي == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (كلي->دام || كلي->هغو) {
        إذا (كلي->درج != NGX_RADIX_NO_درج) {
            كلي->درج = NGX_RADIX_NO_درج;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (كلي->parent->دام == كلي) {
            كلي->parent->دام = عدم;

        } وإلا {
            كلي->parent->هغو = عدم;
        }

        كلي->دام = بشيا->حرر;
        بشيا->حرر = كلي;

        كلي = كلي->parent;

        إذا (كلي->دام || كلي->هغو) {
            اكسر;
        }

        إذا (كلي->درج != NGX_RADIX_NO_درج) {
            اكسر;
        }

        إذا (كلي->parent == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_بشيا_t *بشيا, uint32_t مبودي)
{
    uint32_t           bit;
    uintptr_t          درج;
    ngx_radix_كلي_t  *كلي;

    bit = 0x80000000;
    درج = NGX_RADIX_NO_درج;
    كلي = بشيا->سيو;

    طالما (كلي) {
        إذا (كلي->درج != NGX_RADIX_NO_درج) {
            درج = كلي->درج;
        }

        إذا (مبودي & bit) {
            كلي = كلي->دام;

        } وإلا {
            كلي = كلي->هغو;
        }

        bit >>= 1;
    }

    أرجع درج;
}


#إذا (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_سك(ngx_radix_بشيا_t *بشيا, u_char *مبودي, u_char *mask,
    uintptr_t درج)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_كلي_t  *كلي, *next;

    i = 0;
    bit = 0x80;

    كلي = بشيا->سيو;
    next = بشيا->سيو;

    طالما (bit & mask[i]) {
        إذا (مبودي[i] & bit) {
            next = كلي->دام;

        } وإلا {
            next = كلي->هغو;
        }

        إذا (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        كلي = next;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    إذا (next) {
        إذا (كلي->درج != NGX_RADIX_NO_درج) {
            أرجع NGX_BUSY;
        }

        كلي->درج = درج;
        أرجع NGX_OK;
    }

    طالما (bit & mask[i]) {
        next = ngx_radix_alloc(بشيا);
        إذا (next == عدم) {
            أرجع NGX_ERROR;
        }

        next->دام = عدم;
        next->هغو = عدم;
        next->parent = كلي;
        next->درج = NGX_RADIX_NO_درج;

        إذا (مبودي[i] & bit) {
            كلي->دام = next;

        } وإلا {
            كلي->هغو = next;
        }

        bit >>= 1;
        كلي = next;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    كلي->درج = درج;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_بشيا_t *بشيا, u_char *مبودي, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_كلي_t  *كلي;

    i = 0;
    bit = 0x80;
    كلي = بشيا->سيو;

    طالما (كلي && (bit & mask[i])) {
        إذا (مبودي[i] & bit) {
            كلي = كلي->دام;

        } وإلا {
            كلي = كلي->هغو;
        }

        bit >>= 1;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    إذا (كلي == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (كلي->دام || كلي->هغو) {
        إذا (كلي->درج != NGX_RADIX_NO_درج) {
            كلي->درج = NGX_RADIX_NO_درج;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (كلي->parent->دام == كلي) {
            كلي->parent->دام = عدم;

        } وإلا {
            كلي->parent->هغو = عدم;
        }

        كلي->دام = بشيا->حرر;
        بشيا->حرر = كلي;

        كلي = كلي->parent;

        إذا (كلي->دام || كلي->هغو) {
            اكسر;
        }

        إذا (كلي->درج != NGX_RADIX_NO_درج) {
            اكسر;
        }

        إذا (كلي->parent == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_بشيا_t *بشيا, u_char *مبودي)
{
    u_char             bit;
    uintptr_t          درج;
    ngx_uint_t         i;
    ngx_radix_كلي_t  *كلي;

    i = 0;
    bit = 0x80;
    درج = NGX_RADIX_NO_درج;
    كلي = بشيا->سيو;

    طالما (كلي) {
        إذا (كلي->درج != NGX_RADIX_NO_درج) {
            درج = كلي->درج;
        }

        إذا (مبودي[i] & bit) {
            كلي = كلي->دام;

        } وإلا {
            كلي = كلي->هغو;
        }

        bit >>= 1;

        إذا (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    أرجع درج;
}

#endif


ساكن ngx_radix_كلي_t *
ngx_radix_alloc(ngx_radix_بشيا_t *بشيا)
{
    ngx_radix_كلي_t  *p;

    إذا (بشيا->حرر) {
        p = بشيا->حرر;
        بشيا->حرر = بشيا->حرر->دام;
        أرجع p;
    }

    إذا (بشيا->size < حجم(ngx_radix_كلي_t)) {
        بشيا->start = ngx_pmemalign(بشيا->pool, ngx_pagesize, ngx_pagesize);
        إذا (بشيا->start == عدم) {
            أرجع عدم;
        }

        بشيا->size = ngx_pagesize;
    }

    p = (ngx_radix_كلي_t *) بشيا->start;
    بشيا->start += حجم(ngx_radix_كلي_t);
    بشيا->size -= حجم(ngx_radix_كلي_t);

    أرجع p;
}
