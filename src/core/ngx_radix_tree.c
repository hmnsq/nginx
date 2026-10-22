
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ایستا ngx_radix_گره_t *ngx_radix_alloc(ngx_radix_درخت_t *درخت);


ngx_radix_درخت_t *
ngx_radix_درخت_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           کلید, mask, inc;
    ngx_radix_درخت_t  *درخت;

    درخت = ngx_palloc(pool, اندازه(ngx_radix_درخت_t));
    اگر (درخت == پوچ) {
        بازگشت پوچ;
    }

    درخت->pool = pool;
    درخت->آزاد = پوچ;
    درخت->start = پوچ;
    درخت->size = 0;

    درخت->ریشه = ngx_radix_alloc(درخت);
    اگر (درخت->ریشه == پوچ) {
        بازگشت پوچ;
    }

    درخت->ریشه->راست = پوچ;
    درخت->ریشه->چپ = پوچ;
    درخت->ریشه->parent = پوچ;
    درخت->ریشه->مقدار = NGX_RADIX_NO_مقدار;

    اگر (preallocate == 0) {
        بازگشت درخت;
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

    اگر (preallocate == -1) {
        انتخاب (ngx_pagesize / اندازه(ngx_radix_گره_t)) {

        /* amd64 */
        حالت 128:
            preallocate = 6;
            بشکن;

        /* i386, sparc64 */
        حالت 256:
            preallocate = 7;
            بشکن;

        /* sparc64 in 32-bit mode */
        پیشفرض:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    وقتی (preallocate--) {

        کلید = 0;
        mask >>= 1;
        mask |= 0x80000000;

        انجامبده {
            اگر (ngx_radix32tree_درج(درخت, کلید, mask, NGX_RADIX_NO_مقدار)
                != NGX_OK)
            {
                بازگشت پوچ;
            }

            کلید += inc;

        } وقتی (کلید);

        inc >>= 1;
    }

    بازگشت درخت;
}


ngx_int_t
ngx_radix32tree_درج(ngx_radix_درخت_t *درخت, uint32_t کلید, uint32_t mask,
    uintptr_t مقدار)
{
    uint32_t           bit;
    ngx_radix_گره_t  *گره, *next;

    bit = 0x80000000;

    گره = درخت->ریشه;
    next = درخت->ریشه;

    وقتی (bit & mask) {
        اگر (کلید & bit) {
            next = گره->راست;

        } وگرنه {
            next = گره->چپ;
        }

        اگر (next == پوچ) {
            بشکن;
        }

        bit >>= 1;
        گره = next;
    }

    اگر (next) {
        اگر (گره->مقدار != NGX_RADIX_NO_مقدار) {
            بازگشت NGX_BUSY;
        }

        گره->مقدار = مقدار;
        بازگشت NGX_OK;
    }

    وقتی (bit & mask) {
        next = ngx_radix_alloc(درخت);
        اگر (next == پوچ) {
            بازگشت NGX_ERROR;
        }

        next->راست = پوچ;
        next->چپ = پوچ;
        next->parent = گره;
        next->مقدار = NGX_RADIX_NO_مقدار;

        اگر (کلید & bit) {
            گره->راست = next;

        } وگرنه {
            گره->چپ = next;
        }

        bit >>= 1;
        گره = next;
    }

    گره->مقدار = مقدار;

    بازگشت NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_درخت_t *درخت, uint32_t کلید, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_گره_t  *گره;

    bit = 0x80000000;
    گره = درخت->ریشه;

    وقتی (گره && (bit & mask)) {
        اگر (کلید & bit) {
            گره = گره->راست;

        } وگرنه {
            گره = گره->چپ;
        }

        bit >>= 1;
    }

    اگر (گره == پوچ) {
        بازگشت NGX_ERROR;
    }

    اگر (گره->راست || گره->چپ) {
        اگر (گره->مقدار != NGX_RADIX_NO_مقدار) {
            گره->مقدار = NGX_RADIX_NO_مقدار;
            بازگشت NGX_OK;
        }

        بازگشت NGX_ERROR;
    }

    برای ( ;; ) {
        اگر (گره->parent->راست == گره) {
            گره->parent->راست = پوچ;

        } وگرنه {
            گره->parent->چپ = پوچ;
        }

        گره->راست = درخت->آزاد;
        درخت->آزاد = گره;

        گره = گره->parent;

        اگر (گره->راست || گره->چپ) {
            بشکن;
        }

        اگر (گره->مقدار != NGX_RADIX_NO_مقدار) {
            بشکن;
        }

        اگر (گره->parent == پوچ) {
            بشکن;
        }
    }

    بازگشت NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_درخت_t *درخت, uint32_t کلید)
{
    uint32_t           bit;
    uintptr_t          مقدار;
    ngx_radix_گره_t  *گره;

    bit = 0x80000000;
    مقدار = NGX_RADIX_NO_مقدار;
    گره = درخت->ریشه;

    وقتی (گره) {
        اگر (گره->مقدار != NGX_RADIX_NO_مقدار) {
            مقدار = گره->مقدار;
        }

        اگر (کلید & bit) {
            گره = گره->راست;

        } وگرنه {
            گره = گره->چپ;
        }

        bit >>= 1;
    }

    بازگشت مقدار;
}


#اگر (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_درج(ngx_radix_درخت_t *درخت, u_char *کلید, u_char *mask,
    uintptr_t مقدار)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_گره_t  *گره, *next;

    i = 0;
    bit = 0x80;

    گره = درخت->ریشه;
    next = درخت->ریشه;

    وقتی (bit & mask[i]) {
        اگر (کلید[i] & bit) {
            next = گره->راست;

        } وگرنه {
            next = گره->چپ;
        }

        اگر (next == پوچ) {
            بشکن;
        }

        bit >>= 1;
        گره = next;

        اگر (bit == 0) {
            اگر (++i == 16) {
                بشکن;
            }

            bit = 0x80;
        }
    }

    اگر (next) {
        اگر (گره->مقدار != NGX_RADIX_NO_مقدار) {
            بازگشت NGX_BUSY;
        }

        گره->مقدار = مقدار;
        بازگشت NGX_OK;
    }

    وقتی (bit & mask[i]) {
        next = ngx_radix_alloc(درخت);
        اگر (next == پوچ) {
            بازگشت NGX_ERROR;
        }

        next->راست = پوچ;
        next->چپ = پوچ;
        next->parent = گره;
        next->مقدار = NGX_RADIX_NO_مقدار;

        اگر (کلید[i] & bit) {
            گره->راست = next;

        } وگرنه {
            گره->چپ = next;
        }

        bit >>= 1;
        گره = next;

        اگر (bit == 0) {
            اگر (++i == 16) {
                بشکن;
            }

            bit = 0x80;
        }
    }

    گره->مقدار = مقدار;

    بازگشت NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_درخت_t *درخت, u_char *کلید, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_گره_t  *گره;

    i = 0;
    bit = 0x80;
    گره = درخت->ریشه;

    وقتی (گره && (bit & mask[i])) {
        اگر (کلید[i] & bit) {
            گره = گره->راست;

        } وگرنه {
            گره = گره->چپ;
        }

        bit >>= 1;

        اگر (bit == 0) {
            اگر (++i == 16) {
                بشکن;
            }

            bit = 0x80;
        }
    }

    اگر (گره == پوچ) {
        بازگشت NGX_ERROR;
    }

    اگر (گره->راست || گره->چپ) {
        اگر (گره->مقدار != NGX_RADIX_NO_مقدار) {
            گره->مقدار = NGX_RADIX_NO_مقدار;
            بازگشت NGX_OK;
        }

        بازگشت NGX_ERROR;
    }

    برای ( ;; ) {
        اگر (گره->parent->راست == گره) {
            گره->parent->راست = پوچ;

        } وگرنه {
            گره->parent->چپ = پوچ;
        }

        گره->راست = درخت->آزاد;
        درخت->آزاد = گره;

        گره = گره->parent;

        اگر (گره->راست || گره->چپ) {
            بشکن;
        }

        اگر (گره->مقدار != NGX_RADIX_NO_مقدار) {
            بشکن;
        }

        اگر (گره->parent == پوچ) {
            بشکن;
        }
    }

    بازگشت NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_درخت_t *درخت, u_char *کلید)
{
    u_char             bit;
    uintptr_t          مقدار;
    ngx_uint_t         i;
    ngx_radix_گره_t  *گره;

    i = 0;
    bit = 0x80;
    مقدار = NGX_RADIX_NO_مقدار;
    گره = درخت->ریشه;

    وقتی (گره) {
        اگر (گره->مقدار != NGX_RADIX_NO_مقدار) {
            مقدار = گره->مقدار;
        }

        اگر (کلید[i] & bit) {
            گره = گره->راست;

        } وگرنه {
            گره = گره->چپ;
        }

        bit >>= 1;

        اگر (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    بازگشت مقدار;
}

#endif


ایستا ngx_radix_گره_t *
ngx_radix_alloc(ngx_radix_درخت_t *درخت)
{
    ngx_radix_گره_t  *p;

    اگر (درخت->آزاد) {
        p = درخت->آزاد;
        درخت->آزاد = درخت->آزاد->راست;
        بازگشت p;
    }

    اگر (درخت->size < اندازه(ngx_radix_گره_t)) {
        درخت->start = ngx_pmemalign(درخت->pool, ngx_pagesize, ngx_pagesize);
        اگر (درخت->start == پوچ) {
            بازگشت پوچ;
        }

        درخت->size = ngx_pagesize;
    }

    p = (ngx_radix_گره_t *) درخت->start;
    درخت->start += اندازه(ngx_radix_گره_t);
    درخت->size -= اندازه(ngx_radix_گره_t);

    بازگشت p;
}
