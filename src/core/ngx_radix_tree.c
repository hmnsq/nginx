
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
    सचे (वृक्षः == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    वृक्षः->pool = pool;
    वृक्षः->मुक्त = रिक्त;
    वृक्षः->start = रिक्त;
    वृक्षः->size = 0;

    वृक्षः->मूलम् = ngx_radix_alloc(वृक्षः);
    सचे (वृक्षः->मूलम् == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    वृक्षः->मूलम्->दक्षिणः = रिक्त;
    वृक्षः->मूलम्->वामः = रिक्त;
    वृक्षः->मूलम्->parent = रिक्त;
    वृक्षः->मूलम्->मानम् = NGX_RADIX_NO_मानम्;

    सचे (preallocate == 0) {
        पटिनिवत्त वृक्षः;
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

    सचे (preallocate == -1) {
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

    याव (preallocate--) {

        कुञ्चिका = 0;
        mask >>= 1;
        mask |= 0x80000000;

        करोहि {
            सचे (ngx_radix32tree_निवेशय(वृक्षः, कुञ्चिका, mask, NGX_RADIX_NO_मानम्)
                != NGX_OK)
            {
                पटिनिवत्त रिक्त;
            }

            कुञ्चिका += inc;

        } याव (कुञ्चिका);

        inc >>= 1;
    }

    पटिनिवत्त वृक्षः;
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

    याव (bit & mask) {
        सचे (कुञ्चिका & bit) {
            next = ग्रन्थिः->दक्षिणः;

        } अञ्ञथा {
            next = ग्रन्थिः->वामः;
        }

        सचे (next == रिक्त) {
            भङ्ग;
        }

        bit >>= 1;
        ग्रन्थिः = next;
    }

    सचे (next) {
        सचे (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            पटिनिवत्त NGX_BUSY;
        }

        ग्रन्थिः->मानम् = मानम्;
        पटिनिवत्त NGX_OK;
    }

    याव (bit & mask) {
        next = ngx_radix_alloc(वृक्षः);
        सचे (next == रिक्त) {
            पटिनिवत्त NGX_ERROR;
        }

        next->दक्षिणः = रिक्त;
        next->वामः = रिक्त;
        next->parent = ग्रन्थिः;
        next->मानम् = NGX_RADIX_NO_मानम्;

        सचे (कुञ्चिका & bit) {
            ग्रन्थिः->दक्षिणः = next;

        } अञ्ञथा {
            ग्रन्थिः->वामः = next;
        }

        bit >>= 1;
        ग्रन्थिः = next;
    }

    ग्रन्थिः->मानम् = मानम्;

    पटिनिवत्त NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_वृक्षः_t *वृक्षः, uint32_t कुञ्चिका, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_ग्रन्थिः_t  *ग्रन्थिः;

    bit = 0x80000000;
    ग्रन्थिः = वृक्षः->मूलम्;

    याव (ग्रन्थिः && (bit & mask)) {
        सचे (कुञ्चिका & bit) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;

        } अञ्ञथा {
            ग्रन्थिः = ग्रन्थिः->वामः;
        }

        bit >>= 1;
    }

    सचे (ग्रन्थिः == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    सचे (ग्रन्थिः->दक्षिणः || ग्रन्थिः->वामः) {
        सचे (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            ग्रन्थिः->मानम् = NGX_RADIX_NO_मानम्;
            पटिनिवत्त NGX_OK;
        }

        पटिनिवत्त NGX_ERROR;
    }

    अत्थाय ( ;; ) {
        सचे (ग्रन्थिः->parent->दक्षिणः == ग्रन्थिः) {
            ग्रन्थिः->parent->दक्षिणः = रिक्त;

        } अञ्ञथा {
            ग्रन्थिः->parent->वामः = रिक्त;
        }

        ग्रन्थिः->दक्षिणः = वृक्षः->मुक्त;
        वृक्षः->मुक्त = ग्रन्थिः;

        ग्रन्थिः = ग्रन्थिः->parent;

        सचे (ग्रन्थिः->दक्षिणः || ग्रन्थिः->वामः) {
            भङ्ग;
        }

        सचे (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            भङ्ग;
        }

        सचे (ग्रन्थिः->parent == रिक्त) {
            भङ्ग;
        }
    }

    पटिनिवत्त NGX_OK;
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

    याव (ग्रन्थिः) {
        सचे (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            मानम् = ग्रन्थिः->मानम्;
        }

        सचे (कुञ्चिका & bit) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;

        } अञ्ञथा {
            ग्रन्थिः = ग्रन्थिः->वामः;
        }

        bit >>= 1;
    }

    पटिनिवत्त मानम्;
}


#सचे (NGX_HAVE_INET6)

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

    याव (bit & mask[i]) {
        सचे (कुञ्चिका[i] & bit) {
            next = ग्रन्थिः->दक्षिणः;

        } अञ्ञथा {
            next = ग्रन्थिः->वामः;
        }

        सचे (next == रिक्त) {
            भङ्ग;
        }

        bit >>= 1;
        ग्रन्थिः = next;

        सचे (bit == 0) {
            सचे (++i == 16) {
                भङ्ग;
            }

            bit = 0x80;
        }
    }

    सचे (next) {
        सचे (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            पटिनिवत्त NGX_BUSY;
        }

        ग्रन्थिः->मानम् = मानम्;
        पटिनिवत्त NGX_OK;
    }

    याव (bit & mask[i]) {
        next = ngx_radix_alloc(वृक्षः);
        सचे (next == रिक्त) {
            पटिनिवत्त NGX_ERROR;
        }

        next->दक्षिणः = रिक्त;
        next->वामः = रिक्त;
        next->parent = ग्रन्थिः;
        next->मानम् = NGX_RADIX_NO_मानम्;

        सचे (कुञ्चिका[i] & bit) {
            ग्रन्थिः->दक्षिणः = next;

        } अञ्ञथा {
            ग्रन्थिः->वामः = next;
        }

        bit >>= 1;
        ग्रन्थिः = next;

        सचे (bit == 0) {
            सचे (++i == 16) {
                भङ्ग;
            }

            bit = 0x80;
        }
    }

    ग्रन्थिः->मानम् = मानम्;

    पटिनिवत्त NGX_OK;
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

    याव (ग्रन्थिः && (bit & mask[i])) {
        सचे (कुञ्चिका[i] & bit) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;

        } अञ्ञथा {
            ग्रन्थिः = ग्रन्थिः->वामः;
        }

        bit >>= 1;

        सचे (bit == 0) {
            सचे (++i == 16) {
                भङ्ग;
            }

            bit = 0x80;
        }
    }

    सचे (ग्रन्थिः == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    सचे (ग्रन्थिः->दक्षिणः || ग्रन्थिः->वामः) {
        सचे (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            ग्रन्थिः->मानम् = NGX_RADIX_NO_मानम्;
            पटिनिवत्त NGX_OK;
        }

        पटिनिवत्त NGX_ERROR;
    }

    अत्थाय ( ;; ) {
        सचे (ग्रन्थिः->parent->दक्षिणः == ग्रन्थिः) {
            ग्रन्थिः->parent->दक्षिणः = रिक्त;

        } अञ्ञथा {
            ग्रन्थिः->parent->वामः = रिक्त;
        }

        ग्रन्थिः->दक्षिणः = वृक्षः->मुक्त;
        वृक्षः->मुक्त = ग्रन्थिः;

        ग्रन्थिः = ग्रन्थिः->parent;

        सचे (ग्रन्थिः->दक्षिणः || ग्रन्थिः->वामः) {
            भङ्ग;
        }

        सचे (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            भङ्ग;
        }

        सचे (ग्रन्थिः->parent == रिक्त) {
            भङ्ग;
        }
    }

    पटिनिवत्त NGX_OK;
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

    याव (ग्रन्थिः) {
        सचे (ग्रन्थिः->मानम् != NGX_RADIX_NO_मानम्) {
            मानम् = ग्रन्थिः->मानम्;
        }

        सचे (कुञ्चिका[i] & bit) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;

        } अञ्ञथा {
            ग्रन्थिः = ग्रन्थिः->वामः;
        }

        bit >>= 1;

        सचे (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    पटिनिवत्त मानम्;
}

#endif


स्थिर ngx_radix_ग्रन्थिः_t *
ngx_radix_alloc(ngx_radix_वृक्षः_t *वृक्षः)
{
    ngx_radix_ग्रन्थिः_t  *p;

    सचे (वृक्षः->मुक्त) {
        p = वृक्षः->मुक्त;
        वृक्षः->मुक्त = वृक्षः->मुक्त->दक्षिणः;
        पटिनिवत्त p;
    }

    सचे (वृक्षः->size < परिमाण(ngx_radix_ग्रन्थिः_t)) {
        वृक्षः->start = ngx_pmemalign(वृक्षः->pool, ngx_pagesize, ngx_pagesize);
        सचे (वृक्षः->start == रिक्त) {
            पटिनिवत्त रिक्त;
        }

        वृक्षः->size = ngx_pagesize;
    }

    p = (ngx_radix_ग्रन्थिः_t *) वृक्षः->start;
    वृक्षः->start += परिमाण(ngx_radix_ग्रन्थिः_t);
    वृक्षः->size -= परिमाण(ngx_radix_ग्रन्थिः_t);

    पटिनिवत्त p;
}
