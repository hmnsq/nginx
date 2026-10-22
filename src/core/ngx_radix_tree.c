
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

    বৃক্ষ = ngx_palloc(pool, পরিমাণ(ngx_radix_বৃক্ষ_t));
    যদি (বৃক্ষ == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    বৃক্ষ->pool = pool;
    বৃক্ষ->মুক্ত = রিক্ত;
    বৃক্ষ->start = রিক্ত;
    বৃক্ষ->size = 0;

    বৃক্ষ->মূল = ngx_radix_alloc(বৃক্ষ);
    যদি (বৃক্ষ->মূল == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    বৃক্ষ->মূল->ডান = রিক্ত;
    বৃক্ষ->মূল->বাম = রিক্ত;
    বৃক্ষ->মূল->parent = রিক্ত;
    বৃক্ষ->মূল->মান = NGX_RADIX_NO_মান;

    যদি (preallocate == 0) {
        প্রত্যাগম বৃক্ষ;
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
        বিকল্প (ngx_pagesize / পরিমাণ(ngx_radix_গ্রন্থি_t)) {

        /* amd64 */
        পক্ষ 128:
            preallocate = 6;
            ভঙ্গ;

        /* i386, sparc64 */
        পক্ষ 256:
            preallocate = 7;
            ভঙ্গ;

        /* sparc64 in 32-bit mode */
        স্বভাব:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    যাবত্ (preallocate--) {

        চাবি = 0;
        mask >>= 1;
        mask |= 0x80000000;

        কুরু {
            যদি (ngx_radix32tree_ঢোকাও(বৃক্ষ, চাবি, mask, NGX_RADIX_NO_মান)
                != NGX_OK)
            {
                প্রত্যাগম রিক্ত;
            }

            চাবি += inc;

        } যাবত্ (চাবি);

        inc >>= 1;
    }

    প্রত্যাগম বৃক্ষ;
}


ngx_int_t
ngx_radix32tree_ঢোকাও(ngx_radix_বৃক্ষ_t *বৃক্ষ, uint32_t চাবি, uint32_t mask,
    uintptr_t মান)
{
    uint32_t           bit;
    ngx_radix_গ্রন্থি_t  *গ্রন্থি, *next;

    bit = 0x80000000;

    গ্রন্থি = বৃক্ষ->মূল;
    next = বৃক্ষ->মূল;

    যাবত্ (bit & mask) {
        যদি (চাবি & bit) {
            next = গ্রন্থি->ডান;

        } অন্যথা {
            next = গ্রন্থি->বাম;
        }

        যদি (next == রিক্ত) {
            ভঙ্গ;
        }

        bit >>= 1;
        গ্রন্থি = next;
    }

    যদি (next) {
        যদি (গ্রন্থি->মান != NGX_RADIX_NO_মান) {
            প্রত্যাগম NGX_BUSY;
        }

        গ্রন্থি->মান = মান;
        প্রত্যাগম NGX_OK;
    }

    যাবত্ (bit & mask) {
        next = ngx_radix_alloc(বৃক্ষ);
        যদি (next == রিক্ত) {
            প্রত্যাগম NGX_ERROR;
        }

        next->ডান = রিক্ত;
        next->বাম = রিক্ত;
        next->parent = গ্রন্থি;
        next->মান = NGX_RADIX_NO_মান;

        যদি (চাবি & bit) {
            গ্রন্থি->ডান = next;

        } অন্যথা {
            গ্রন্থি->বাম = next;
        }

        bit >>= 1;
        গ্রন্থি = next;
    }

    গ্রন্থি->মান = মান;

    প্রত্যাগম NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_বৃক্ষ_t *বৃক্ষ, uint32_t চাবি, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_গ্রন্থি_t  *গ্রন্থি;

    bit = 0x80000000;
    গ্রন্থি = বৃক্ষ->মূল;

    যাবত্ (গ্রন্থি && (bit & mask)) {
        যদি (চাবি & bit) {
            গ্রন্থি = গ্রন্থি->ডান;

        } অন্যথা {
            গ্রন্থি = গ্রন্থি->বাম;
        }

        bit >>= 1;
    }

    যদি (গ্রন্থি == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    যদি (গ্রন্থি->ডান || গ্রন্থি->বাম) {
        যদি (গ্রন্থি->মান != NGX_RADIX_NO_মান) {
            গ্রন্থি->মান = NGX_RADIX_NO_মান;
            প্রত্যাগম NGX_OK;
        }

        প্রত্যাগম NGX_ERROR;
    }

    প্রতি ( ;; ) {
        যদি (গ্রন্থি->parent->ডান == গ্রন্থি) {
            গ্রন্থি->parent->ডান = রিক্ত;

        } অন্যথা {
            গ্রন্থি->parent->বাম = রিক্ত;
        }

        গ্রন্থি->ডান = বৃক্ষ->মুক্ত;
        বৃক্ষ->মুক্ত = গ্রন্থি;

        গ্রন্থি = গ্রন্থি->parent;

        যদি (গ্রন্থি->ডান || গ্রন্থি->বাম) {
            ভঙ্গ;
        }

        যদি (গ্রন্থি->মান != NGX_RADIX_NO_মান) {
            ভঙ্গ;
        }

        যদি (গ্রন্থি->parent == রিক্ত) {
            ভঙ্গ;
        }
    }

    প্রত্যাগম NGX_OK;
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

    যাবত্ (গ্রন্থি) {
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

    প্রত্যাগম মান;
}


#যদি (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_ঢোকাও(ngx_radix_বৃক্ষ_t *বৃক্ষ, u_char *চাবি, u_char *mask,
    uintptr_t মান)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_গ্রন্থি_t  *গ্রন্থি, *next;

    i = 0;
    bit = 0x80;

    গ্রন্থি = বৃক্ষ->মূল;
    next = বৃক্ষ->মূল;

    যাবত্ (bit & mask[i]) {
        যদি (চাবি[i] & bit) {
            next = গ্রন্থি->ডান;

        } অন্যথা {
            next = গ্রন্থি->বাম;
        }

        যদি (next == রিক্ত) {
            ভঙ্গ;
        }

        bit >>= 1;
        গ্রন্থি = next;

        যদি (bit == 0) {
            যদি (++i == 16) {
                ভঙ্গ;
            }

            bit = 0x80;
        }
    }

    যদি (next) {
        যদি (গ্রন্থি->মান != NGX_RADIX_NO_মান) {
            প্রত্যাগম NGX_BUSY;
        }

        গ্রন্থি->মান = মান;
        প্রত্যাগম NGX_OK;
    }

    যাবত্ (bit & mask[i]) {
        next = ngx_radix_alloc(বৃক্ষ);
        যদি (next == রিক্ত) {
            প্রত্যাগম NGX_ERROR;
        }

        next->ডান = রিক্ত;
        next->বাম = রিক্ত;
        next->parent = গ্রন্থি;
        next->মান = NGX_RADIX_NO_মান;

        যদি (চাবি[i] & bit) {
            গ্রন্থি->ডান = next;

        } অন্যথা {
            গ্রন্থি->বাম = next;
        }

        bit >>= 1;
        গ্রন্থি = next;

        যদি (bit == 0) {
            যদি (++i == 16) {
                ভঙ্গ;
            }

            bit = 0x80;
        }
    }

    গ্রন্থি->মান = মান;

    প্রত্যাগম NGX_OK;
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

    যাবত্ (গ্রন্থি && (bit & mask[i])) {
        যদি (চাবি[i] & bit) {
            গ্রন্থি = গ্রন্থি->ডান;

        } অন্যথা {
            গ্রন্থি = গ্রন্থি->বাম;
        }

        bit >>= 1;

        যদি (bit == 0) {
            যদি (++i == 16) {
                ভঙ্গ;
            }

            bit = 0x80;
        }
    }

    যদি (গ্রন্থি == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    যদি (গ্রন্থি->ডান || গ্রন্থি->বাম) {
        যদি (গ্রন্থি->মান != NGX_RADIX_NO_মান) {
            গ্রন্থি->মান = NGX_RADIX_NO_মান;
            প্রত্যাগম NGX_OK;
        }

        প্রত্যাগম NGX_ERROR;
    }

    প্রতি ( ;; ) {
        যদি (গ্রন্থি->parent->ডান == গ্রন্থি) {
            গ্রন্থি->parent->ডান = রিক্ত;

        } অন্যথা {
            গ্রন্থি->parent->বাম = রিক্ত;
        }

        গ্রন্থি->ডান = বৃক্ষ->মুক্ত;
        বৃক্ষ->মুক্ত = গ্রন্থি;

        গ্রন্থি = গ্রন্থি->parent;

        যদি (গ্রন্থি->ডান || গ্রন্থি->বাম) {
            ভঙ্গ;
        }

        যদি (গ্রন্থি->মান != NGX_RADIX_NO_মান) {
            ভঙ্গ;
        }

        যদি (গ্রন্থি->parent == রিক্ত) {
            ভঙ্গ;
        }
    }

    প্রত্যাগম NGX_OK;
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

    যাবত্ (গ্রন্থি) {
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

    প্রত্যাগম মান;
}

#endif


স্থির ngx_radix_গ্রন্থি_t *
ngx_radix_alloc(ngx_radix_বৃক্ষ_t *বৃক্ষ)
{
    ngx_radix_গ্রন্থি_t  *p;

    যদি (বৃক্ষ->মুক্ত) {
        p = বৃক্ষ->মুক্ত;
        বৃক্ষ->মুক্ত = বৃক্ষ->মুক্ত->ডান;
        প্রত্যাগম p;
    }

    যদি (বৃক্ষ->size < পরিমাণ(ngx_radix_গ্রন্থি_t)) {
        বৃক্ষ->start = ngx_pmemalign(বৃক্ষ->pool, ngx_pagesize, ngx_pagesize);
        যদি (বৃক্ষ->start == রিক্ত) {
            প্রত্যাগম রিক্ত;
        }

        বৃক্ষ->size = ngx_pagesize;
    }

    p = (ngx_radix_গ্রন্থি_t *) বৃক্ষ->start;
    বৃক্ষ->start += পরিমাণ(ngx_radix_গ্রন্থি_t);
    বৃক্ষ->size -= পরিমাণ(ngx_radix_গ্রন্থি_t);

    প্রত্যাগম p;
}
