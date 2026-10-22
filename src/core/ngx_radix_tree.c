
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_radix_غوټه_t *ngx_radix_alloc(ngx_radix_ونه_t *ونه);


ngx_radix_ونه_t *
ngx_radix_ونه_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           کلی, ماسک, inc;
    ngx_radix_ونه_t  *ونه;

    ونه = ngx_palloc(pool, حجم(ngx_radix_ونه_t));
    إذا (ونه == عدم) {
        أرجع عدم;
    }

    ونه->pool = pool;
    ونه->حرر = عدم;
    ونه->پیل = عدم;
    ونه->اندازه = 0;

    ونه->ریښه = ngx_radix_alloc(ونه);
    إذا (ونه->ریښه == عدم) {
        أرجع عدم;
    }

    ونه->ریښه->ښی = عدم;
    ونه->ریښه->کیڼ = عدم;
    ونه->ریښه->پلار = عدم;
    ونه->ریښه->ارزښت = NGX_RADIX_NO_ارزښت;

    إذا (preallocate == 0) {
        أرجع ونه;
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
        بدل (ngx_pagesize / حجم(ngx_radix_غوټه_t)) {

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

        کلی = 0;
        ماسک >>= 1;
        ماسک |= 0x80000000;

        افعل {
            إذا (ngx_radix32tree_ورننباسه(ونه, کلی, ماسک, NGX_RADIX_NO_ارزښت)
                != NGX_OK)
            {
                أرجع عدم;
            }

            کلی += inc;

        } طالما (کلی);

        inc >>= 1;
    }

    أرجع ونه;
}


ngx_int_t
ngx_radix32tree_ورننباسه(ngx_radix_ونه_t *ونه, uint32_t کلی, uint32_t ماسک,
    uintptr_t ارزښت)
{
    uint32_t           بټ;
    ngx_radix_غوټه_t  *غوټه, *بل;

    بټ = 0x80000000;

    غوټه = ونه->ریښه;
    بل = ونه->ریښه;

    طالما (بټ & ماسک) {
        إذا (کلی & بټ) {
            بل = غوټه->ښی;

        } وإلا {
            بل = غوټه->کیڼ;
        }

        إذا (بل == عدم) {
            اكسر;
        }

        بټ >>= 1;
        غوټه = بل;
    }

    إذا (بل) {
        إذا (غوټه->ارزښت != NGX_RADIX_NO_ارزښت) {
            أرجع NGX_BUSY;
        }

        غوټه->ارزښت = ارزښت;
        أرجع NGX_OK;
    }

    طالما (بټ & ماسک) {
        بل = ngx_radix_alloc(ونه);
        إذا (بل == عدم) {
            أرجع NGX_ERROR;
        }

        بل->ښی = عدم;
        بل->کیڼ = عدم;
        بل->پلار = غوټه;
        بل->ارزښت = NGX_RADIX_NO_ارزښت;

        إذا (کلی & بټ) {
            غوټه->ښی = بل;

        } وإلا {
            غوټه->کیڼ = بل;
        }

        بټ >>= 1;
        غوټه = بل;
    }

    غوټه->ارزښت = ارزښت;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_ونه_t *ونه, uint32_t کلی, uint32_t ماسک)
{
    uint32_t           بټ;
    ngx_radix_غوټه_t  *غوټه;

    بټ = 0x80000000;
    غوټه = ونه->ریښه;

    طالما (غوټه && (بټ & ماسک)) {
        إذا (کلی & بټ) {
            غوټه = غوټه->ښی;

        } وإلا {
            غوټه = غوټه->کیڼ;
        }

        بټ >>= 1;
    }

    إذا (غوټه == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (غوټه->ښی || غوټه->کیڼ) {
        إذا (غوټه->ارزښت != NGX_RADIX_NO_ارزښت) {
            غوټه->ارزښت = NGX_RADIX_NO_ارزښت;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (غوټه->پلار->ښی == غوټه) {
            غوټه->پلار->ښی = عدم;

        } وإلا {
            غوټه->پلار->کیڼ = عدم;
        }

        غوټه->ښی = ونه->حرر;
        ونه->حرر = غوټه;

        غوټه = غوټه->پلار;

        إذا (غوټه->ښی || غوټه->کیڼ) {
            اكسر;
        }

        إذا (غوټه->ارزښت != NGX_RADIX_NO_ارزښت) {
            اكسر;
        }

        إذا (غوټه->پلار == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix32tree_پیدا_کړه(ngx_radix_ونه_t *ونه, uint32_t کلی)
{
    uint32_t           بټ;
    uintptr_t          ارزښت;
    ngx_radix_غوټه_t  *غوټه;

    بټ = 0x80000000;
    ارزښت = NGX_RADIX_NO_ارزښت;
    غوټه = ونه->ریښه;

    طالما (غوټه) {
        إذا (غوټه->ارزښت != NGX_RADIX_NO_ارزښت) {
            ارزښت = غوټه->ارزښت;
        }

        إذا (کلی & بټ) {
            غوټه = غوټه->ښی;

        } وإلا {
            غوټه = غوټه->کیڼ;
        }

        بټ >>= 1;
    }

    أرجع ارزښت;
}


#إذا (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_ورننباسه(ngx_radix_ونه_t *ونه, u_char *کلی, u_char *ماسک,
    uintptr_t ارزښت)
{
    u_char             بټ;
    ngx_uint_t         i;
    ngx_radix_غوټه_t  *غوټه, *بل;

    i = 0;
    بټ = 0x80;

    غوټه = ونه->ریښه;
    بل = ونه->ریښه;

    طالما (بټ & ماسک[i]) {
        إذا (کلی[i] & بټ) {
            بل = غوټه->ښی;

        } وإلا {
            بل = غوټه->کیڼ;
        }

        إذا (بل == عدم) {
            اكسر;
        }

        بټ >>= 1;
        غوټه = بل;

        إذا (بټ == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            بټ = 0x80;
        }
    }

    إذا (بل) {
        إذا (غوټه->ارزښت != NGX_RADIX_NO_ارزښت) {
            أرجع NGX_BUSY;
        }

        غوټه->ارزښت = ارزښت;
        أرجع NGX_OK;
    }

    طالما (بټ & ماسک[i]) {
        بل = ngx_radix_alloc(ونه);
        إذا (بل == عدم) {
            أرجع NGX_ERROR;
        }

        بل->ښی = عدم;
        بل->کیڼ = عدم;
        بل->پلار = غوټه;
        بل->ارزښت = NGX_RADIX_NO_ارزښت;

        إذا (کلی[i] & بټ) {
            غوټه->ښی = بل;

        } وإلا {
            غوټه->کیڼ = بل;
        }

        بټ >>= 1;
        غوټه = بل;

        إذا (بټ == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            بټ = 0x80;
        }
    }

    غوټه->ارزښت = ارزښت;

    أرجع NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_ونه_t *ونه, u_char *کلی, u_char *ماسک)
{
    u_char             بټ;
    ngx_uint_t         i;
    ngx_radix_غوټه_t  *غوټه;

    i = 0;
    بټ = 0x80;
    غوټه = ونه->ریښه;

    طالما (غوټه && (بټ & ماسک[i])) {
        إذا (کلی[i] & بټ) {
            غوټه = غوټه->ښی;

        } وإلا {
            غوټه = غوټه->کیڼ;
        }

        بټ >>= 1;

        إذا (بټ == 0) {
            إذا (++i == 16) {
                اكسر;
            }

            بټ = 0x80;
        }
    }

    إذا (غوټه == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (غوټه->ښی || غوټه->کیڼ) {
        إذا (غوټه->ارزښت != NGX_RADIX_NO_ارزښت) {
            غوټه->ارزښت = NGX_RADIX_NO_ارزښت;
            أرجع NGX_OK;
        }

        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        إذا (غوټه->پلار->ښی == غوټه) {
            غوټه->پلار->ښی = عدم;

        } وإلا {
            غوټه->پلار->کیڼ = عدم;
        }

        غوټه->ښی = ونه->حرر;
        ونه->حرر = غوټه;

        غوټه = غوټه->پلار;

        إذا (غوټه->ښی || غوټه->کیڼ) {
            اكسر;
        }

        إذا (غوټه->ارزښت != NGX_RADIX_NO_ارزښت) {
            اكسر;
        }

        إذا (غوټه->پلار == عدم) {
            اكسر;
        }
    }

    أرجع NGX_OK;
}


uintptr_t
ngx_radix128tree_پیدا_کړه(ngx_radix_ونه_t *ونه, u_char *کلی)
{
    u_char             بټ;
    uintptr_t          ارزښت;
    ngx_uint_t         i;
    ngx_radix_غوټه_t  *غوټه;

    i = 0;
    بټ = 0x80;
    ارزښت = NGX_RADIX_NO_ارزښت;
    غوټه = ونه->ریښه;

    طالما (غوټه) {
        إذا (غوټه->ارزښت != NGX_RADIX_NO_ارزښت) {
            ارزښت = غوټه->ارزښت;
        }

        إذا (کلی[i] & بټ) {
            غوټه = غوټه->ښی;

        } وإلا {
            غوټه = غوټه->کیڼ;
        }

        بټ >>= 1;

        إذا (بټ == 0) {
            i++;
            بټ = 0x80;
        }
    }

    أرجع ارزښت;
}

#د_که_پای


ساكن ngx_radix_غوټه_t *
ngx_radix_alloc(ngx_radix_ونه_t *ونه)
{
    ngx_radix_غوټه_t  *p;

    إذا (ونه->حرر) {
        p = ونه->حرر;
        ونه->حرر = ونه->حرر->ښی;
        أرجع p;
    }

    إذا (ونه->اندازه < حجم(ngx_radix_غوټه_t)) {
        ونه->پیل = ngx_pmemalign(ونه->pool, ngx_pagesize, ngx_pagesize);
        إذا (ونه->پیل == عدم) {
            أرجع عدم;
        }

        ونه->اندازه = ngx_pagesize;
    }

    p = (ngx_radix_غوټه_t *) ونه->پیل;
    ونه->پیل += حجم(ngx_radix_غوټه_t);
    ونه->اندازه -= حجم(ngx_radix_غوټه_t);

    أرجع p;
}
