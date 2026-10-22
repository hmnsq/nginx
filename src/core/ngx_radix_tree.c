
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_radix_عقدة_t *ngx_radix_alloc(ngx_radix_شجرة_t *شجرة);


ngx_radix_شجرة_t *
ngx_radix_شجرة_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           مفتاح, mask, inc;
    ngx_radix_شجرة_t  *شجرة;

    شجرة = ngx_palloc(pool, حجم(ngx_radix_شجرة_t));
    إذا (شجرة == عدم) {
        إرجاع عدم;
    }

    شجرة->pool = pool;
    شجرة->حرر = عدم;
    شجرة->start = عدم;
    شجرة->size = 0;

    شجرة->جذر = ngx_radix_alloc(شجرة);
    إذا (شجرة->جذر == عدم) {
        إرجاع عدم;
    }

    شجرة->جذر->يمين = عدم;
    شجرة->جذر->يسار = عدم;
    شجرة->جذر->parent = عدم;
    شجرة->جذر->قيمة = NGX_RADIX_NO_قيمة;

    إذا (preallocate == 0) {
        إرجاع شجرة;
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
        بدل (ngx_pagesize / حجم(ngx_radix_عقدة_t)) {

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

    بينما (preallocate--) {

        مفتاح = 0;
        mask >>= 1;
        mask |= 0x80000000;

        افعل {
            إذا (ngx_radix32tree_أدرج(شجرة, مفتاح, mask, NGX_RADIX_NO_قيمة)
                != NGX_OK)
            {
                إرجاع عدم;
            }

            مفتاح += inc;

        } بينما (مفتاح);

        inc >>= 1;
    }

    إرجاع شجرة;
}


ngx_int_t
ngx_radix32tree_أدرج(ngx_radix_شجرة_t *شجرة, uint32_t مفتاح, uint32_t mask,
    uintptr_t قيمة)
{
    uint32_t           bit;
    ngx_radix_عقدة_t  *عقدة, *next;

    bit = 0x80000000;

    عقدة = شجرة->جذر;
    next = شجرة->جذر;

    بينما (bit & mask) {
        إذا (مفتاح & bit) {
            next = عقدة->يمين;

        } وإلا {
            next = عقدة->يسار;
        }

        إذا (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        عقدة = next;
    }

    إذا (next) {
        إذا (عقدة->قيمة != NGX_RADIX_NO_قيمة) {
            إرجاع NGX_BUSY;
        }

        عقدة->قيمة = قيمة;
        إرجاع NGX_OK;
    }

    بينما (bit & mask) {
        next = ngx_radix_alloc(شجرة);
        إذا (next == عدم) {
            إرجاع NGX_ERROR;
        }

        next->يمين = عدم;
        next->يسار = عدم;
        next->parent = عقدة;
        next->قيمة = NGX_RADIX_NO_قيمة;

        إذا (مفتاح & bit) {
            عقدة->يمين = next;

        } وإلا {
            عقدة->يسار = next;
        }

        bit >>= 1;
        عقدة = next;
    }

    عقدة->قيمة = قيمة;

    إرجاع NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_شجرة_t *شجرة, uint32_t مفتاح, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_عقدة_t  *عقدة;

    bit = 0x80000000;
    عقدة = شجرة->جذر;

    بينما (عقدة && (bit & mask)) {
        إذا (مفتاح & bit) {
            عقدة = عقدة->يمين;

        } وإلا {
            عقدة = عقدة->يسار;
        }

        bit >>= 1;
    }

    إذا (عقدة == عدم) {
        إرجاع NGX_ERROR;
    }

    إذا (عقدة->يمين || عقدة->يسار) {
        إذا (عقدة->قيمة != NGX_RADIX_NO_قيمة) {
            عقدة->قيمة = NGX_RADIX_NO_قيمة;
            إرجاع NGX_OK;
        }

        إرجاع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (عقدة->parent->يمين == عقدة) {
            عقدة->parent->يمين = عدم;

        } وإلا {
            عقدة->parent->يسار = عدم;
        }

        عقدة->يمين = شجرة->حرر;
        شجرة->حرر = عقدة;

        عقدة = عقدة->parent;

        إذا (عقدة->يمين || عقدة->يسار) {
            اكسر;
        }

        إذا (عقدة->قيمة != NGX_RADIX_NO_قيمة) {
            اكسر;
        }

        إذا (عقدة->parent == عدم) {
            اكسر;
        }
    }

    إرجاع NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_شجرة_t *شجرة, uint32_t مفتاح)
{
    uint32_t           bit;
    uintptr_t          قيمة;
    ngx_radix_عقدة_t  *عقدة;

    bit = 0x80000000;
    قيمة = NGX_RADIX_NO_قيمة;
    عقدة = شجرة->جذر;

    بينما (عقدة) {
        إذا (عقدة->قيمة != NGX_RADIX_NO_قيمة) {
            قيمة = عقدة->قيمة;
        }

        إذا (مفتاح & bit) {
            عقدة = عقدة->يمين;

        } وإلا {
            عقدة = عقدة->يسار;
        }

        bit >>= 1;
    }

    إرجاع قيمة;
}


#إذا (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_أدرج(ngx_radix_شجرة_t *شجرة, u_char *مفتاح, u_char *mask,
    uintptr_t قيمة)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_عقدة_t  *عقدة, *next;

    i = 0;
    bit = 0x80;

    عقدة = شجرة->جذر;
    next = شجرة->جذر;

    بينما (bit & mask[i]) {
        إذا (مفتاح[i] & bit) {
            next = عقدة->يمين;

        } وإلا {
            next = عقدة->يسار;
        }

        إذا (next == عدم) {
            اكسر;
        }

        bit >>= 1;
        عقدة = next;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    إذا (next) {
        إذا (عقدة->قيمة != NGX_RADIX_NO_قيمة) {
            إرجاع NGX_BUSY;
        }

        عقدة->قيمة = قيمة;
        إرجاع NGX_OK;
    }

    بينما (bit & mask[i]) {
        next = ngx_radix_alloc(شجرة);
        إذا (next == عدم) {
            إرجاع NGX_ERROR;
        }

        next->يمين = عدم;
        next->يسار = عدم;
        next->parent = عقدة;
        next->قيمة = NGX_RADIX_NO_قيمة;

        إذا (مفتاح[i] & bit) {
            عقدة->يمين = next;

        } وإلا {
            عقدة->يسار = next;
        }

        bit >>= 1;
        عقدة = next;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    عقدة->قيمة = قيمة;

    إرجاع NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_شجرة_t *شجرة, u_char *مفتاح, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_عقدة_t  *عقدة;

    i = 0;
    bit = 0x80;
    عقدة = شجرة->جذر;

    بينما (عقدة && (bit & mask[i])) {
        إذا (مفتاح[i] & bit) {
            عقدة = عقدة->يمين;

        } وإلا {
            عقدة = عقدة->يسار;
        }

        bit >>= 1;

        إذا (bit == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            bit = 0x80;
        }
    }

    إذا (عقدة == عدم) {
        إرجاع NGX_ERROR;
    }

    إذا (عقدة->يمين || عقدة->يسار) {
        إذا (عقدة->قيمة != NGX_RADIX_NO_قيمة) {
            عقدة->قيمة = NGX_RADIX_NO_قيمة;
            إرجاع NGX_OK;
        }

        إرجاع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (عقدة->parent->يمين == عقدة) {
            عقدة->parent->يمين = عدم;

        } وإلا {
            عقدة->parent->يسار = عدم;
        }

        عقدة->يمين = شجرة->حرر;
        شجرة->حرر = عقدة;

        عقدة = عقدة->parent;

        إذا (عقدة->يمين || عقدة->يسار) {
            اكسر;
        }

        إذا (عقدة->قيمة != NGX_RADIX_NO_قيمة) {
            اكسر;
        }

        إذا (عقدة->parent == عدم) {
            اكسر;
        }
    }

    إرجاع NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_شجرة_t *شجرة, u_char *مفتاح)
{
    u_char             bit;
    uintptr_t          قيمة;
    ngx_uint_t         i;
    ngx_radix_عقدة_t  *عقدة;

    i = 0;
    bit = 0x80;
    قيمة = NGX_RADIX_NO_قيمة;
    عقدة = شجرة->جذر;

    بينما (عقدة) {
        إذا (عقدة->قيمة != NGX_RADIX_NO_قيمة) {
            قيمة = عقدة->قيمة;
        }

        إذا (مفتاح[i] & bit) {
            عقدة = عقدة->يمين;

        } وإلا {
            عقدة = عقدة->يسار;
        }

        bit >>= 1;

        إذا (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    إرجاع قيمة;
}

#endif


ساكن ngx_radix_عقدة_t *
ngx_radix_alloc(ngx_radix_شجرة_t *شجرة)
{
    ngx_radix_عقدة_t  *p;

    إذا (شجرة->حرر) {
        p = شجرة->حرر;
        شجرة->حرر = شجرة->حرر->يمين;
        إرجاع p;
    }

    إذا (شجرة->size < حجم(ngx_radix_عقدة_t)) {
        شجرة->start = ngx_pmemalign(شجرة->pool, ngx_pagesize, ngx_pagesize);
        إذا (شجرة->start == عدم) {
            إرجاع عدم;
        }

        شجرة->size = ngx_pagesize;
    }

    p = (ngx_radix_عقدة_t *) شجرة->start;
    شجرة->start += حجم(ngx_radix_عقدة_t);
    شجرة->size -= حجم(ngx_radix_عقدة_t);

    إرجاع p;
}
