
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर ngx_radix_गाठ_t *ngx_radix_alloc(ngx_radix_वृक्ष_t *वृक्ष);


ngx_radix_वृक्ष_t *
ngx_radix_वृक्ष_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           किल्ली, mask, inc;
    ngx_radix_वृक्ष_t  *वृक्ष;

    वृक्ष = ngx_palloc(pool, परिमाण(ngx_radix_वृक्ष_t));
    यदि (वृक्ष == रिक्त) {
        प्रत्यागम रिक्त;
    }

    वृक्ष->pool = pool;
    वृक्ष->मुक्त = रिक्त;
    वृक्ष->start = रिक्त;
    वृक्ष->size = 0;

    वृक्ष->मूळ = ngx_radix_alloc(वृक्ष);
    यदि (वृक्ष->मूळ == रिक्त) {
        प्रत्यागम रिक्त;
    }

    वृक्ष->मूळ->उजवा = रिक्त;
    वृक्ष->मूळ->डावा = रिक्त;
    वृक्ष->मूळ->parent = रिक्त;
    वृक्ष->मूळ->मूल्य = NGX_RADIX_NO_मूल्य;

    यदि (preallocate == 0) {
        प्रत्यागम वृक्ष;
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

    यदि (preallocate == -1) {
        विकल्प (ngx_pagesize / परिमाण(ngx_radix_गाठ_t)) {

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

    यावत् (preallocate--) {

        किल्ली = 0;
        mask >>= 1;
        mask |= 0x80000000;

        कुरु {
            यदि (ngx_radix32tree_घाला(वृक्ष, किल्ली, mask, NGX_RADIX_NO_मूल्य)
                != NGX_OK)
            {
                प्रत्यागम रिक्त;
            }

            किल्ली += inc;

        } यावत् (किल्ली);

        inc >>= 1;
    }

    प्रत्यागम वृक्ष;
}


ngx_int_t
ngx_radix32tree_घाला(ngx_radix_वृक्ष_t *वृक्ष, uint32_t किल्ली, uint32_t mask,
    uintptr_t मूल्य)
{
    uint32_t           bit;
    ngx_radix_गाठ_t  *गाठ, *next;

    bit = 0x80000000;

    गाठ = वृक्ष->मूळ;
    next = वृक्ष->मूळ;

    यावत् (bit & mask) {
        यदि (किल्ली & bit) {
            next = गाठ->उजवा;

        } अन्यथा {
            next = गाठ->डावा;
        }

        यदि (next == रिक्त) {
            भङ्ग;
        }

        bit >>= 1;
        गाठ = next;
    }

    यदि (next) {
        यदि (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            प्रत्यागम NGX_BUSY;
        }

        गाठ->मूल्य = मूल्य;
        प्रत्यागम NGX_OK;
    }

    यावत् (bit & mask) {
        next = ngx_radix_alloc(वृक्ष);
        यदि (next == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        next->उजवा = रिक्त;
        next->डावा = रिक्त;
        next->parent = गाठ;
        next->मूल्य = NGX_RADIX_NO_मूल्य;

        यदि (किल्ली & bit) {
            गाठ->उजवा = next;

        } अन्यथा {
            गाठ->डावा = next;
        }

        bit >>= 1;
        गाठ = next;
    }

    गाठ->मूल्य = मूल्य;

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_वृक्ष_t *वृक्ष, uint32_t किल्ली, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_गाठ_t  *गाठ;

    bit = 0x80000000;
    गाठ = वृक्ष->मूळ;

    यावत् (गाठ && (bit & mask)) {
        यदि (किल्ली & bit) {
            गाठ = गाठ->उजवा;

        } अन्यथा {
            गाठ = गाठ->डावा;
        }

        bit >>= 1;
    }

    यदि (गाठ == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    यदि (गाठ->उजवा || गाठ->डावा) {
        यदि (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            गाठ->मूल्य = NGX_RADIX_NO_मूल्य;
            प्रत्यागम NGX_OK;
        }

        प्रत्यागम NGX_ERROR;
    }

    प्रति ( ;; ) {
        यदि (गाठ->parent->उजवा == गाठ) {
            गाठ->parent->उजवा = रिक्त;

        } अन्यथा {
            गाठ->parent->डावा = रिक्त;
        }

        गाठ->उजवा = वृक्ष->मुक्त;
        वृक्ष->मुक्त = गाठ;

        गाठ = गाठ->parent;

        यदि (गाठ->उजवा || गाठ->डावा) {
            भङ्ग;
        }

        यदि (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            भङ्ग;
        }

        यदि (गाठ->parent == रिक्त) {
            भङ्ग;
        }
    }

    प्रत्यागम NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_वृक्ष_t *वृक्ष, uint32_t किल्ली)
{
    uint32_t           bit;
    uintptr_t          मूल्य;
    ngx_radix_गाठ_t  *गाठ;

    bit = 0x80000000;
    मूल्य = NGX_RADIX_NO_मूल्य;
    गाठ = वृक्ष->मूळ;

    यावत् (गाठ) {
        यदि (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            मूल्य = गाठ->मूल्य;
        }

        यदि (किल्ली & bit) {
            गाठ = गाठ->उजवा;

        } अन्यथा {
            गाठ = गाठ->डावा;
        }

        bit >>= 1;
    }

    प्रत्यागम मूल्य;
}


#यदि (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_घाला(ngx_radix_वृक्ष_t *वृक्ष, u_char *किल्ली, u_char *mask,
    uintptr_t मूल्य)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_गाठ_t  *गाठ, *next;

    i = 0;
    bit = 0x80;

    गाठ = वृक्ष->मूळ;
    next = वृक्ष->मूळ;

    यावत् (bit & mask[i]) {
        यदि (किल्ली[i] & bit) {
            next = गाठ->उजवा;

        } अन्यथा {
            next = गाठ->डावा;
        }

        यदि (next == रिक्त) {
            भङ्ग;
        }

        bit >>= 1;
        गाठ = next;

        यदि (bit == 0) {
            यदि (++i == 16) {
                भङ्ग;
            }

            bit = 0x80;
        }
    }

    यदि (next) {
        यदि (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            प्रत्यागम NGX_BUSY;
        }

        गाठ->मूल्य = मूल्य;
        प्रत्यागम NGX_OK;
    }

    यावत् (bit & mask[i]) {
        next = ngx_radix_alloc(वृक्ष);
        यदि (next == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        next->उजवा = रिक्त;
        next->डावा = रिक्त;
        next->parent = गाठ;
        next->मूल्य = NGX_RADIX_NO_मूल्य;

        यदि (किल्ली[i] & bit) {
            गाठ->उजवा = next;

        } अन्यथा {
            गाठ->डावा = next;
        }

        bit >>= 1;
        गाठ = next;

        यदि (bit == 0) {
            यदि (++i == 16) {
                भङ्ग;
            }

            bit = 0x80;
        }
    }

    गाठ->मूल्य = मूल्य;

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_वृक्ष_t *वृक्ष, u_char *किल्ली, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_गाठ_t  *गाठ;

    i = 0;
    bit = 0x80;
    गाठ = वृक्ष->मूळ;

    यावत् (गाठ && (bit & mask[i])) {
        यदि (किल्ली[i] & bit) {
            गाठ = गाठ->उजवा;

        } अन्यथा {
            गाठ = गाठ->डावा;
        }

        bit >>= 1;

        यदि (bit == 0) {
            यदि (++i == 16) {
                भङ्ग;
            }

            bit = 0x80;
        }
    }

    यदि (गाठ == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    यदि (गाठ->उजवा || गाठ->डावा) {
        यदि (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            गाठ->मूल्य = NGX_RADIX_NO_मूल्य;
            प्रत्यागम NGX_OK;
        }

        प्रत्यागम NGX_ERROR;
    }

    प्रति ( ;; ) {
        यदि (गाठ->parent->उजवा == गाठ) {
            गाठ->parent->उजवा = रिक्त;

        } अन्यथा {
            गाठ->parent->डावा = रिक्त;
        }

        गाठ->उजवा = वृक्ष->मुक्त;
        वृक्ष->मुक्त = गाठ;

        गाठ = गाठ->parent;

        यदि (गाठ->उजवा || गाठ->डावा) {
            भङ्ग;
        }

        यदि (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            भङ्ग;
        }

        यदि (गाठ->parent == रिक्त) {
            भङ्ग;
        }
    }

    प्रत्यागम NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_वृक्ष_t *वृक्ष, u_char *किल्ली)
{
    u_char             bit;
    uintptr_t          मूल्य;
    ngx_uint_t         i;
    ngx_radix_गाठ_t  *गाठ;

    i = 0;
    bit = 0x80;
    मूल्य = NGX_RADIX_NO_मूल्य;
    गाठ = वृक्ष->मूळ;

    यावत् (गाठ) {
        यदि (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            मूल्य = गाठ->मूल्य;
        }

        यदि (किल्ली[i] & bit) {
            गाठ = गाठ->उजवा;

        } अन्यथा {
            गाठ = गाठ->डावा;
        }

        bit >>= 1;

        यदि (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    प्रत्यागम मूल्य;
}

#endif


स्थिर ngx_radix_गाठ_t *
ngx_radix_alloc(ngx_radix_वृक्ष_t *वृक्ष)
{
    ngx_radix_गाठ_t  *p;

    यदि (वृक्ष->मुक्त) {
        p = वृक्ष->मुक्त;
        वृक्ष->मुक्त = वृक्ष->मुक्त->उजवा;
        प्रत्यागम p;
    }

    यदि (वृक्ष->size < परिमाण(ngx_radix_गाठ_t)) {
        वृक्ष->start = ngx_pmemalign(वृक्ष->pool, ngx_pagesize, ngx_pagesize);
        यदि (वृक्ष->start == रिक्त) {
            प्रत्यागम रिक्त;
        }

        वृक्ष->size = ngx_pagesize;
    }

    p = (ngx_radix_गाठ_t *) वृक्ष->start;
    वृक्ष->start += परिमाण(ngx_radix_गाठ_t);
    वृक्ष->size -= परिमाण(ngx_radix_गाठ_t);

    प्रत्यागम p;
}
