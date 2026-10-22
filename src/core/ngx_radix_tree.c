
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर ngx_radix_ग्रन्थिः_t *ngx_radix_alloc(ngx_radix_वृक्षः_t *वृक्षः);


ngx_radix_वृक्षः_t *
ngx_radix_वृक्षः_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           कुञ्चिका, mask, inc;
    ngx_radix_वृक्षः_t  *वृक्षः;

    वृक्षः = ngx_palloc(pool, परिमाण(ngx_radix_वृक्षः_t));
    जइ (वृक्षः == रिक्त) {
        प्रत्यागम रिक्त;
    }

    वृक्षः->pool = pool;
    वृक्षः->मुक्त = रिक्त;
    वृक्षः->start = रिक्त;
    वृक्षः->size = 0;

    वृक्षः->मूलम् = ngx_radix_alloc(वृक्षः);
    जइ (वृक्षः->मूलम् == रिक्त) {
        प्रत्यागम रिक्त;
    }

    वृक्षः->मूलम्->दक्षिणः = रिक्त;
    वृक्षः->मूलम्->वामः = रिक्त;
    वृक्षः->मूलम्->parent = रिक्त;
    वृक्षः->मूलम्->मानम् = NGX_RADIX_NO_मानम्;

    जइ (preallocate == 0) {
        प्रत्यागम वृक्षः;
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

    जइ (preallocate == -1) {
        विकल्प (ngx_pagesize / परिमाण(ngx_radix_ग्रन्थिः_t)) {

        /* amd64 */
        पक्ष 128:
            preallocate = 6;
            भङ्ग;

        /* i386, sparc64 */
        पक्ष 256:
            preallocate = 7;
            भङ्ग;

        /* sparc64 in 32-bit mode */
        स्वभाव:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    जाव (preallocate--) {

        कुञ्चिका = 0;
        mask >>= 1;
        mask |= 0x80000000;

        करेहि {
            जइ (ngx_radix32tree_निवेशय(वृक्षः, कुञ्चिका, mask, NGX_RADIX_NO_मानम्)
                != NGX_OK)
            {
                प्रत्यागम रिक्त;
            }

            कुञ्चिका += inc;

        } जाव (कुञ्चिका);

        inc >>= 1;
    }

    प्रत्यागम वृक्षः;
}


ngx_int_t
ngx_radix32tree_निवेशय(ngx_radix_वृक्षः_t *वृक्षः, uint32_t कुञ्चिका, uint32_t mask,
    uintptr_t मानम्)
{
    uint32_t           bit;
    ngx_radix_ग्रन्थिः_t  *ग्रन्थिः, *next;

    bit = 0x80000000;

    ग्रन्थिः = वृक्षः->मूलम्;
    next = वृक्षः->मूलम्;

    जाव (bit & mask) {
        जइ (कुञ्चिका & bit) {
            next = ग्रन्थिः->दक्षिणः;

        } अण्णहा {
            next = ग्रन्थिः->वामः;
        }

        जइ (next == रिक्त) {
            भङ्ग;
        }

        bit >>= 1;
        ग्रन्थिः = next;
    }

    जइ (next) {
        जइ (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            प्रत्यागम NGX_BUSY;
        }

        ग्रन्थिः->मानम् = मानम्;
        प्रत्यागम NGX_OK;
    }

    जाव (bit & mask) {
        next = ngx_radix_alloc(वृक्षः);
        जइ (next == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        next->दक्षिणः = रिक्त;
        next->वामः = रिक्त;
        next->parent = ग्रन्थिः;
        next->मानम् = NGX_RADIX_NO_मानम्;

        जइ (कुञ्चिका & bit) {
            ग्रन्थिः->दक्षिणः = next;

        } अण्णहा {
            ग्रन्थिः->वामः = next;
        }

        bit >>= 1;
        ग्रन्थिः = next;
    }

    ग्रन्थिः->मानम् = मानम्;

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_वृक्षः_t *वृक्षः, uint32_t कुञ्चिका, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_ग्रन्थिः_t  *ग्रन्थिः;

    bit = 0x80000000;
    ग्रन्थिः = वृक्षः->मूलम्;

    जाव (ग्रन्थिः && (bit & mask)) {
        जइ (कुञ्चिका & bit) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;

        } अण्णहा {
            ग्रन्थिः = ग्रन्थिः->वामः;
        }

        bit >>= 1;
    }

    जइ (ग्रन्थिः == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    जइ (ग्रन्थिः->दक्षिणः || ग्रन्थिः->वामः) {
        जइ (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            ग्रन्थिः->मानम् = NGX_RADIX_NO_मानम्;
            प्रत्यागम NGX_OK;
        }

        प्रत्यागम NGX_ERROR;
    }

    प्रति ( ;; ) {
        जइ (ग्रन्थिः->parent->दक्षिणः == ग्रन्थिः) {
            ग्रन्थिः->parent->दक्षिणः = रिक्त;

        } अण्णहा {
            ग्रन्थिः->parent->वामः = रिक्त;
        }

        ग्रन्थिः->दक्षिणः = वृक्षः->मुक्त;
        वृक्षः->मुक्त = ग्रन्थिः;

        ग्रन्थिः = ग्रन्थिः->parent;

        जइ (ग्रन्थिः->दक्षिणः || ग्रन्थिः->वामः) {
            भङ्ग;
        }

        जइ (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            भङ्ग;
        }

        जइ (ग्रन्थिः->parent == रिक्त) {
            भङ्ग;
        }
    }

    प्रत्यागम NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_वृक्षः_t *वृक्षः, uint32_t कुञ्चिका)
{
    uint32_t           bit;
    uintptr_t          मानम्;
    ngx_radix_ग्रन्थिः_t  *ग्रन्थिः;

    bit = 0x80000000;
    मानम् = NGX_RADIX_NO_मानम्;
    ग्रन्थिः = वृक्षः->मूलम्;

    जाव (ग्रन्थिः) {
        जइ (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            मानम् = ग्रन्थिः->मानम्;
        }

        जइ (कुञ्चिका & bit) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;

        } अण्णहा {
            ग्रन्थिः = ग्रन्थिः->वामः;
        }

        bit >>= 1;
    }

    प्रत्यागम मानम्;
}


#जइ (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_निवेशय(ngx_radix_वृक्षः_t *वृक्षः, u_char *कुञ्चिका, u_char *mask,
    uintptr_t मानम्)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_ग्रन्थिः_t  *ग्रन्थिः, *next;

    i = 0;
    bit = 0x80;

    ग्रन्थिः = वृक्षः->मूलम्;
    next = वृक्षः->मूलम्;

    जाव (bit & mask[i]) {
        जइ (कुञ्चिका[i] & bit) {
            next = ग्रन्थिः->दक्षिणः;

        } अण्णहा {
            next = ग्रन्थिः->वामः;
        }

        जइ (next == रिक्त) {
            भङ्ग;
        }

        bit >>= 1;
        ग्रन्थिः = next;

        जइ (bit == 0) {
            जइ (++i == 16) {
                भङ्ग;
            }

            bit = 0x80;
        }
    }

    जइ (next) {
        जइ (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            प्रत्यागम NGX_BUSY;
        }

        ग्रन्थिः->मानम् = मानम्;
        प्रत्यागम NGX_OK;
    }

    जाव (bit & mask[i]) {
        next = ngx_radix_alloc(वृक्षः);
        जइ (next == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        next->दक्षिणः = रिक्त;
        next->वामः = रिक्त;
        next->parent = ग्रन्थिः;
        next->मानम् = NGX_RADIX_NO_मानम्;

        जइ (कुञ्चिका[i] & bit) {
            ग्रन्थिः->दक्षिणः = next;

        } अण्णहा {
            ग्रन्थिः->वामः = next;
        }

        bit >>= 1;
        ग्रन्थिः = next;

        जइ (bit == 0) {
            जइ (++i == 16) {
                भङ्ग;
            }

            bit = 0x80;
        }
    }

    ग्रन्थिः->मानम् = मानम्;

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_वृक्षः_t *वृक्षः, u_char *कुञ्चिका, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_ग्रन्थिः_t  *ग्रन्थिः;

    i = 0;
    bit = 0x80;
    ग्रन्थिः = वृक्षः->मूलम्;

    जाव (ग्रन्थिः && (bit & mask[i])) {
        जइ (कुञ्चिका[i] & bit) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;

        } अण्णहा {
            ग्रन्थिः = ग्रन्थिः->वामः;
        }

        bit >>= 1;

        जइ (bit == 0) {
            जइ (++i == 16) {
                भङ्ग;
            }

            bit = 0x80;
        }
    }

    जइ (ग्रन्थिः == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    जइ (ग्रन्थिः->दक्षिणः || ग्रन्थिः->वामः) {
        जइ (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            ग्रन्थिः->मानम् = NGX_RADIX_NO_मानम्;
            प्रत्यागम NGX_OK;
        }

        प्रत्यागम NGX_ERROR;
    }

    प्रति ( ;; ) {
        जइ (ग्रन्थिः->parent->दक्षिणः == ग्रन्थिः) {
            ग्रन्थिः->parent->दक्षिणः = रिक्त;

        } अण्णहा {
            ग्रन्थिः->parent->वामः = रिक्त;
        }

        ग्रन्थिः->दक्षिणः = वृक्षः->मुक्त;
        वृक्षः->मुक्त = ग्रन्थिः;

        ग्रन्थिः = ग्रन्थिः->parent;

        जइ (ग्रन्थिः->दक्षिणः || ग्रन्थिः->वामः) {
            भङ्ग;
        }

        जइ (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            भङ्ग;
        }

        जइ (ग्रन्थिः->parent == रिक्त) {
            भङ्ग;
        }
    }

    प्रत्यागम NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_वृक्षः_t *वृक्षः, u_char *कुञ्चिका)
{
    u_char             bit;
    uintptr_t          मानम्;
    ngx_uint_t         i;
    ngx_radix_ग्रन्थिः_t  *ग्रन्थिः;

    i = 0;
    bit = 0x80;
    मानम् = NGX_RADIX_NO_मानम्;
    ग्रन्थिः = वृक्षः->मूलम्;

    जाव (ग्रन्थिः) {
        जइ (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            मानम् = ग्रन्थिः->मानम्;
        }

        जइ (कुञ्चिका[i] & bit) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;

        } अण्णहा {
            ग्रन्थिः = ग्रन्थिः->वामः;
        }

        bit >>= 1;

        जइ (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    प्रत्यागम मानम्;
}

#endif


स्थिर ngx_radix_ग्रन्थिः_t *
ngx_radix_alloc(ngx_radix_वृक्षः_t *वृक्षः)
{
    ngx_radix_ग्रन्थिः_t  *p;

    जइ (वृक्षः->मुक्त) {
        p = वृक्षः->मुक्त;
        वृक्षः->मुक्त = वृक्षः->मुक्त->दक्षिणः;
        प्रत्यागम p;
    }

    जइ (वृक्षः->size < परिमाण(ngx_radix_ग्रन्थिः_t)) {
        वृक्षः->start = ngx_pmemalign(वृक्षः->pool, ngx_pagesize, ngx_pagesize);
        जइ (वृक्षः->start == रिक्त) {
            प्रत्यागम रिक्त;
        }

        वृक्षः->size = ngx_pagesize;
    }

    p = (ngx_radix_ग्रन्थिः_t *) वृक्षः->start;
    वृक्षः->start += परिमाण(ngx_radix_ग्रन्थिः_t);
    वृक्षः->size -= परिमाण(ngx_radix_ग्रन्थिः_t);

    प्रत्यागम p;
}
