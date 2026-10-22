
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


স্থির ngx_radix_গ্রন্থি_t *ngx_radix_alloc(ngx_radix_বৃক্ষ_t *বৃক্ষ);


ngx_radix_বৃক্ষ_t *
ngx_radix_বৃক্ষ_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           চাবি, mask, inc;
    ngx_radix_বৃক্ষ_t  *বৃক্ষ;

    বৃক্ষ = ngx_palloc(pool, আকার(ngx_radix_বৃক্ষ_t));
    যদি (বৃক্ষ == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    বৃক্ষ->pool = pool;
    বৃক্ষ->মুক্ত = ফাঁকা;
    বৃক্ষ->start = ফাঁকা;
    বৃক্ষ->আকার_2 = 0;

    বৃক্ষ->মূল = ngx_radix_alloc(বৃক্ষ);
    যদি (বৃক্ষ->মূল == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    বৃক্ষ->মূল->ডান = ফাঁকা;
    বৃক্ষ->মূল->বাম = ফাঁকা;
    বৃক্ষ->মূল->parent = ফাঁকা;
    বৃক্ষ->মূল->মান = NGX_RADIX_NO_মান;

    যদি (preallocate == 0) {
        ফেরত বৃক্ষ;
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

    যদি (preallocate == -1) {
        নির্বাচন (ngx_pagesize / আকার(ngx_radix_গ্রন্থি_t)) {

        /* amd64 */
        ক্ষেত্র 128:
            preallocate = 6;
            বিরতি;

        /* i386, sparc64 */
        ক্ষেত্র 256:
            preallocate = 7;
            বিরতি;

        /* sparc64 in 32-bit mode */
        পূর্বনির্ধারিত:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    যখন (preallocate--) {

        চাবি = 0;
        mask >>= 1;
        mask |= 0x80000000;

        করো {
            যদি (ngx_radix32tree_ঢোকাও(বৃক্ষ, চাবি, mask, NGX_RADIX_NO_মান)
                != NGX_OK)
            {
                ফেরত ফাঁকা;
            }

            চাবি += inc;

        } যখন (চাবি);

        inc >>= 1;
    }

    ফেরত বৃক্ষ;
}


ngx_int_t
ngx_radix32tree_ঢোকাও(ngx_radix_বৃক্ষ_t *বৃক্ষ, uint32_t চাবি, uint32_t mask,
    uintptr_t মান)
{
    uint32_t           bit;
    ngx_radix_গ্রন্থি_t  *গ্রন্থি, *পরবর্তী;

    bit = 0x80000000;

    গ্রন্থি = বৃক্ষ->মূল;
    পরবর্তী = বৃক্ষ->মূল;

    যখন (bit & mask) {
        যদি (চাবি & bit) {
            পরবর্তী = গ্রন্থি->ডান;

        } অন্যথা {
            পরবর্তী = গ্রন্থি->বাম;
        }

        যদি (পরবর্তী == ফাঁকা) {
            বিরতি;
        }

        bit >>= 1;
        গ্রন্থি = পরবর্তী;
    }

    যদি (পরবর্তী) {
        যদি (গ্রন্থি->মান != NGX_RADIX_NO_মান) {
            ফেরত NGX_BUSY;
        }

        গ্রন্থি->মান = মান;
        ফেরত NGX_OK;
    }

    যখন (bit & mask) {
        পরবর্তী = ngx_radix_alloc(বৃক্ষ);
        যদি (পরবর্তী == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        পরবর্তী->ডান = ফাঁকা;
        পরবর্তী->বাম = ফাঁকা;
        পরবর্তী->parent = গ্রন্থি;
        পরবর্তী->মান = NGX_RADIX_NO_মান;

        যদি (চাবি & bit) {
            গ্রন্থি->ডান = পরবর্তী;

        } অন্যথা {
            গ্রন্থি->বাম = পরবর্তী;
        }

        bit >>= 1;
        গ্রন্থি = পরবর্তী;
    }

    গ্রন্থি->মান = মান;

    ফেরত NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_বৃক্ষ_t *বৃক্ষ, uint32_t চাবি, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_গ্রন্থি_t  *গ্রন্থি;

    bit = 0x80000000;
    গ্রন্থি = বৃক্ষ->মূল;

    যখন (গ্রন্থি && (bit & mask)) {
        যদি (চাবি & bit) {
            গ্রন্থি = গ্রন্থি->ডান;

        } অন্যথা {
            গ্রন্থি = গ্রন্থি->বাম;
        }

        bit >>= 1;
    }

    যদি (গ্রন্থি == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    যদি (গ্রন্থি->ডান || গ্রন্থি->বাম) {
        যদি (গ্রন্থি->মান != NGX_RADIX_NO_মান) {
            গ্রন্থি->মান = NGX_RADIX_NO_মান;
            ফেরত NGX_OK;
        }

        ফেরত NGX_ERROR;
    }

    জন্য ( ;; ) {
        যদি (গ্রন্থি->parent->ডান == গ্রন্থি) {
            গ্রন্থি->parent->ডান = ফাঁকা;

        } অন্যথা {
            গ্রন্থি->parent->বাম = ফাঁকা;
        }

        গ্রন্থি->ডান = বৃক্ষ->মুক্ত;
        বৃক্ষ->মুক্ত = গ্রন্থি;

        গ্রন্থি = গ্রন্থি->parent;

        যদি (গ্রন্থি->ডান || গ্রন্থি->বাম) {
            বিরতি;
        }

        যদি (গ্রন্থি->মান != NGX_RADIX_NO_মান) {
            বিরতি;
        }

        যদি (গ্রন্থি->parent == ফাঁকা) {
            বিরতি;
        }
    }

    ফেরত NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_বৃক্ষ_t *বৃক্ষ, uint32_t চাবি)
{
    uint32_t           bit;
    uintptr_t          মান;
    ngx_radix_গ্রন্থি_t  *গ্রন্থি;

    bit = 0x80000000;
    মান = NGX_RADIX_NO_মান;
    গ্রন্থি = বৃক্ষ->মূল;

    যখন (গ্রন্থি) {
        যদি (গ্রন্থি->মান != NGX_RADIX_NO_মান) {
            মান = গ্রন্থি->মান;
        }

        যদি (চাবি & bit) {
            গ্রন্থি = গ্রন্থি->ডান;

        } অন্যথা {
            গ্রন্থি = গ্রন্থি->বাম;
        }

        bit >>= 1;
    }

    ফেরত মান;
}


#যদি (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_ঢোকাও(ngx_radix_বৃক্ষ_t *বৃক্ষ, u_char *চাবি, u_char *mask,
    uintptr_t মান)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_গ্রন্থি_t  *গ্রন্থি, *পরবর্তী;

    i = 0;
    bit = 0x80;

    গ্রন্থি = বৃক্ষ->মূল;
    পরবর্তী = বৃক্ষ->মূল;

    যখন (bit & mask[i]) {
        যদি (চাবি[i] & bit) {
            পরবর্তী = গ্রন্থি->ডান;

        } অন্যথা {
            পরবর্তী = গ্রন্থি->বাম;
        }

        যদি (পরবর্তী == ফাঁকা) {
            বিরতি;
        }

        bit >>= 1;
        গ্রন্থি = পরবর্তী;

        যদি (bit == 0) {
            যদি (++i == 16) {
                বিরতি;
            }

            bit = 0x80;
        }
    }

    যদি (পরবর্তী) {
        যদি (গ্রন্থি->মান != NGX_RADIX_NO_মান) {
            ফেরত NGX_BUSY;
        }

        গ্রন্থি->মান = মান;
        ফেরত NGX_OK;
    }

    যখন (bit & mask[i]) {
        পরবর্তী = ngx_radix_alloc(বৃক্ষ);
        যদি (পরবর্তী == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        পরবর্তী->ডান = ফাঁকা;
        পরবর্তী->বাম = ফাঁকা;
        পরবর্তী->parent = গ্রন্থি;
        পরবর্তী->মান = NGX_RADIX_NO_মান;

        যদি (চাবি[i] & bit) {
            গ্রন্থি->ডান = পরবর্তী;

        } অন্যথা {
            গ্রন্থি->বাম = পরবর্তী;
        }

        bit >>= 1;
        গ্রন্থি = পরবর্তী;

        যদি (bit == 0) {
            যদি (++i == 16) {
                বিরতি;
            }

            bit = 0x80;
        }
    }

    গ্রন্থি->মান = মান;

    ফেরত NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_বৃক্ষ_t *বৃক্ষ, u_char *চাবি, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_গ্রন্থি_t  *গ্রন্থি;

    i = 0;
    bit = 0x80;
    গ্রন্থি = বৃক্ষ->মূল;

    যখন (গ্রন্থি && (bit & mask[i])) {
        যদি (চাবি[i] & bit) {
            গ্রন্থি = গ্রন্থি->ডান;

        } অন্যথা {
            গ্রন্থি = গ্রন্থি->বাম;
        }

        bit >>= 1;

        যদি (bit == 0) {
            যদি (++i == 16) {
                বিরতি;
            }

            bit = 0x80;
        }
    }

    যদি (গ্রন্থি == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    যদি (গ্রন্থি->ডান || গ্রন্থি->বাম) {
        যদি (গ্রন্থি->মান != NGX_RADIX_NO_মান) {
            গ্রন্থি->মান = NGX_RADIX_NO_মান;
            ফেরত NGX_OK;
        }

        ফেরত NGX_ERROR;
    }

    জন্য ( ;; ) {
        যদি (গ্রন্থি->parent->ডান == গ্রন্থি) {
            গ্রন্থি->parent->ডান = ফাঁকা;

        } অন্যথা {
            গ্রন্থি->parent->বাম = ফাঁকা;
        }

        গ্রন্থি->ডান = বৃক্ষ->মুক্ত;
        বৃক্ষ->মুক্ত = গ্রন্থি;

        গ্রন্থি = গ্রন্থি->parent;

        যদি (গ্রন্থি->ডান || গ্রন্থি->বাম) {
            বিরতি;
        }

        যদি (গ্রন্থি->মান != NGX_RADIX_NO_মান) {
            বিরতি;
        }

        যদি (গ্রন্থি->parent == ফাঁকা) {
            বিরতি;
        }
    }

    ফেরত NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_বৃক্ষ_t *বৃক্ষ, u_char *চাবি)
{
    u_char             bit;
    uintptr_t          মান;
    ngx_uint_t         i;
    ngx_radix_গ্রন্থি_t  *গ্রন্থি;

    i = 0;
    bit = 0x80;
    মান = NGX_RADIX_NO_মান;
    গ্রন্থি = বৃক্ষ->মূল;

    যখন (গ্রন্থি) {
        যদি (গ্রন্থি->মান != NGX_RADIX_NO_মান) {
            মান = গ্রন্থি->মান;
        }

        যদি (চাবি[i] & bit) {
            গ্রন্থি = গ্রন্থি->ডান;

        } অন্যথা {
            গ্রন্থি = গ্রন্থি->বাম;
        }

        bit >>= 1;

        যদি (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    ফেরত মান;
}

#endif


স্থির ngx_radix_গ্রন্থি_t *
ngx_radix_alloc(ngx_radix_বৃক্ষ_t *বৃক্ষ)
{
    ngx_radix_গ্রন্থি_t  *p;

    যদি (বৃক্ষ->মুক্ত) {
        p = বৃক্ষ->মুক্ত;
        বৃক্ষ->মুক্ত = বৃক্ষ->মুক্ত->ডান;
        ফেরত p;
    }

    যদি (বৃক্ষ->আকার_2 < আকার(ngx_radix_গ্রন্থি_t)) {
        বৃক্ষ->start = ngx_pmemalign(বৃক্ষ->pool, ngx_pagesize, ngx_pagesize);
        যদি (বৃক্ষ->start == ফাঁকা) {
            ফেরত ফাঁকা;
        }

        বৃক্ষ->আকার_2 = ngx_pagesize;
    }

    p = (ngx_radix_গ্রন্থি_t *) বৃক্ষ->start;
    বৃক্ষ->start += আকার(ngx_radix_গ্রন্থি_t);
    বৃক্ষ->আকার_2 -= আকার(ngx_radix_গ্রন্থি_t);

    ফেরত p;
}
