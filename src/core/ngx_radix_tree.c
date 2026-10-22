
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_radix_گره_t *ngx_radix_alloc(ngx_radix_درخت_t *درخت);


ngx_radix_درخت_t *
ngx_radix_درخت_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           کلید, ماسک, inc;
    ngx_radix_درخت_t  *درخت;

    درخت = ngx_palloc(pool, حجم(ngx_radix_درخت_t));
    إذا (درخت == عدم) {
        أرجع عدم;
    }

    درخت->pool = pool;
    درخت->حرر = عدم;
    درخت->شروع = عدم;
    درخت->اندازه = 0;

    درخت->ریشه = ngx_radix_alloc(درخت);
    إذا (درخت->ریشه == عدم) {
        أرجع عدم;
    }

    درخت->ریشه->راست = عدم;
    درخت->ریشه->چپ = عدم;
    درخت->ریشه->والد = عدم;
    درخت->ریشه->مقدار = NGX_RADIX_NO_مقدار;

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
        بدل (ngx_pagesize / حجم(ngx_radix_گره_t)) {

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

    ماسک = 0;
    inc = 0x80000000;

    طالما (preallocate--) {

        کلید = 0;
        ماسک >>= 1;
        ماسک |= 0x80000000;

        افعل {
            إذا (ngx_radix32tree_درج(درخت, کلید, ماسک, NGX_RADIX_NO_مقدار)
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
ngx_radix32tree_درج(ngx_radix_درخت_t *درخت, uint32_t کلید, uint32_t ماسک,
    uintptr_t مقدار)
{
    uint32_t           بیت;
    ngx_radix_گره_t  *گره, *بعدی;

    بیت = 0x80000000;

    گره = درخت->ریشه;
    بعدی = درخت->ریشه;

    طالما (بیت & ماسک) {
        إذا (کلید & بیت) {
            بعدی = گره->راست;

        } وإلا {
            بعدی = گره->چپ;
        }

        إذا (بعدی == عدم) {
            اكسر;
        }

        بیت >>= 1;
        گره = بعدی;
    }

    إذا (بعدی) {
        إذا (گره->مقدار != NGX_RADIX_NO_مقدار) {
            أرجع NGX_BUSY;
        }

        گره->مقدار = مقدار;
        أرجع NGX_OK;
    }

    طالما (بیت & ماسک) {
        بعدی = ngx_radix_alloc(درخت);
        إذا (بعدی == عدم) {
            أرجع NGX_ERROR;
        }

        بعدی->راست = عدم;
        بعدی->چپ = عدم;
        بعدی->والد = گره;
        بعدی->مقدار = NGX_RADIX_NO_مقدار;

        إذا (کلید & بیت) {
            گره->راست = بعدی;

        } وإلا {
            گره->چپ = بعدی;
        }

        بیت >>= 1;
        گره = بعدی;
    }

    گره->مقدار = مقدار;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_درخت_t *درخت, uint32_t کلید, uint32_t ماسک)
{
    uint32_t           بیت;
    ngx_radix_گره_t  *گره;

    بیت = 0x80000000;
    گره = درخت->ریشه;

    طالما (گره && (بیت & ماسک)) {
        إذا (کلید & بیت) {
            گره = گره->راست;

        } وإلا {
            گره = گره->چپ;
        }

        بیت >>= 1;
    }

    إذا (گره == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (گره->راست || گره->چپ) {
        إذا (گره->مقدار != NGX_RADIX_NO_مقدار) {
            گره->مقدار = NGX_RADIX_NO_مقدار;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (گره->والد->راست == گره) {
            گره->والد->راست = عدم;

        } وإلا {
            گره->والد->چپ = عدم;
        }

        گره->راست = درخت->حرر;
        درخت->حرر = گره;

        گره = گره->والد;

        إذا (گره->راست || گره->چپ) {
            اكسر;
        }

        إذا (گره->مقدار != NGX_RADIX_NO_مقدار) {
            اكسر;
        }

        إذا (گره->والد == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix32tree_پیدا_کن(ngx_radix_درخت_t *درخت, uint32_t کلید)
{
    uint32_t           بیت;
    uintptr_t          مقدار;
    ngx_radix_گره_t  *گره;

    بیت = 0x80000000;
    مقدار = NGX_RADIX_NO_مقدار;
    گره = درخت->ریشه;

    طالما (گره) {
        إذا (گره->مقدار != NGX_RADIX_NO_مقدار) {
            مقدار = گره->مقدار;
        }

        إذا (کلید & بیت) {
            گره = گره->راست;

        } وإلا {
            گره = گره->چپ;
        }

        بیت >>= 1;
    }

    أرجع مقدار;
}


#إذا (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_درج(ngx_radix_درخت_t *درخت, u_char *کلید, u_char *ماسک,
    uintptr_t مقدار)
{
    u_char             بیت;
    ngx_uint_t         i;
    ngx_radix_گره_t  *گره, *بعدی;

    i = 0;
    بیت = 0x80;

    گره = درخت->ریشه;
    بعدی = درخت->ریشه;

    طالما (بیت & ماسک[i]) {
        إذا (کلید[i] & بیت) {
            بعدی = گره->راست;

        } وإلا {
            بعدی = گره->چپ;
        }

        إذا (بعدی == عدم) {
            اكسر;
        }

        بیت >>= 1;
        گره = بعدی;

        إذا (بیت == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            بیت = 0x80;
        }
    }

    إذا (بعدی) {
        إذا (گره->مقدار != NGX_RADIX_NO_مقدار) {
            أرجع NGX_BUSY;
        }

        گره->مقدار = مقدار;
        أرجع NGX_OK;
    }

    طالما (بیت & ماسک[i]) {
        بعدی = ngx_radix_alloc(درخت);
        إذا (بعدی == عدم) {
            أرجع NGX_ERROR;
        }

        بعدی->راست = عدم;
        بعدی->چپ = عدم;
        بعدی->والد = گره;
        بعدی->مقدار = NGX_RADIX_NO_مقدار;

        إذا (کلید[i] & بیت) {
            گره->راست = بعدی;

        } وإلا {
            گره->چپ = بعدی;
        }

        بیت >>= 1;
        گره = بعدی;

        إذا (بیت == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            بیت = 0x80;
        }
    }

    گره->مقدار = مقدار;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_درخت_t *درخت, u_char *کلید, u_char *ماسک)
{
    u_char             بیت;
    ngx_uint_t         i;
    ngx_radix_گره_t  *گره;

    i = 0;
    بیت = 0x80;
    گره = درخت->ریشه;

    طالما (گره && (بیت & ماسک[i])) {
        إذا (کلید[i] & بیت) {
            گره = گره->راست;

        } وإلا {
            گره = گره->چپ;
        }

        بیت >>= 1;

        إذا (بیت == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            بیت = 0x80;
        }
    }

    إذا (گره == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (گره->راست || گره->چپ) {
        إذا (گره->مقدار != NGX_RADIX_NO_مقدار) {
            گره->مقدار = NGX_RADIX_NO_مقدار;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (گره->والد->راست == گره) {
            گره->والد->راست = عدم;

        } وإلا {
            گره->والد->چپ = عدم;
        }

        گره->راست = درخت->حرر;
        درخت->حرر = گره;

        گره = گره->والد;

        إذا (گره->راست || گره->چپ) {
            اكسر;
        }

        إذا (گره->مقدار != NGX_RADIX_NO_مقدار) {
            اكسر;
        }

        إذا (گره->والد == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix128tree_پیدا_کن(ngx_radix_درخت_t *درخت, u_char *کلید)
{
    u_char             بیت;
    uintptr_t          مقدار;
    ngx_uint_t         i;
    ngx_radix_گره_t  *گره;

    i = 0;
    بیت = 0x80;
    مقدار = NGX_RADIX_NO_مقدار;
    گره = درخت->ریشه;

    طالما (گره) {
        إذا (گره->مقدار != NGX_RADIX_NO_مقدار) {
            مقدار = گره->مقدار;
        }

        إذا (کلید[i] & بیت) {
            گره = گره->راست;

        } وإلا {
            گره = گره->چپ;
        }

        بیت >>= 1;

        إذا (بیت == 0) {
            i++;
            بیت = 0x80;
        }
    }

    أرجع مقدار;
}

#پایان_اگر


ساكن ngx_radix_گره_t *
ngx_radix_alloc(ngx_radix_درخت_t *درخت)
{
    ngx_radix_گره_t  *p;

    إذا (درخت->حرر) {
        p = درخت->حرر;
        درخت->حرر = درخت->حرر->راست;
        أرجع p;
    }

    إذا (درخت->اندازه < حجم(ngx_radix_گره_t)) {
        درخت->شروع = ngx_pmemalign(درخت->pool, ngx_pagesize, ngx_pagesize);
        إذا (درخت->شروع == عدم) {
            أرجع عدم;
        }

        درخت->اندازه = ngx_pagesize;
    }

    p = (ngx_radix_گره_t *) درخت->شروع;
    درخت->شروع += حجم(ngx_radix_گره_t);
    درخت->اندازه -= حجم(ngx_radix_گره_t);

    أرجع p;
}
