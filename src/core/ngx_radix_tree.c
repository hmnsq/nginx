
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

    वृक्ष = ngx_palloc(pool, आकार(ngx_radix_वृक्ष_t));
    जर (वृक्ष == रिक्त) {
        परत रिक्त;
    }

    वृक्ष->pool = pool;
    वृक्ष->मुक्त = रिक्त;
    वृक्ष->start = रिक्त;
    वृक्ष->size = 0;

    वृक्ष->मूळ = ngx_radix_alloc(वृक्ष);
    जर (वृक्ष->मूळ == रिक्त) {
        परत रिक्त;
    }

    वृक्ष->मूळ->उजवा = रिक्त;
    वृक्ष->मूळ->डावा = रिक्त;
    वृक्ष->मूळ->parent = रिक्त;
    वृक्ष->मूळ->मूल्य = NGX_RADIX_NO_मूल्य;

    जर (preallocate == 0) {
        परत वृक्ष;
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

    जर (preallocate == -1) {
        निवड (ngx_pagesize / आकार(ngx_radix_गाठ_t)) {

        /* amd64 */
        स्थिती 128:
            preallocate = 6;
            थांबा;

        /* i386, sparc64 */
        स्थिती 256:
            preallocate = 7;
            थांबा;

        /* sparc64 in 32-bit mode */
        पूर्वनिर्धारित:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    पर्यंत (preallocate--) {

        किल्ली = 0;
        mask >>= 1;
        mask |= 0x80000000;

        करा {
            जर (ngx_radix32tree_घाला(वृक्ष, किल्ली, mask, NGX_RADIX_NO_मूल्य)
                != NGX_OK)
            {
                परत रिक्त;
            }

            किल्ली += inc;

        } पर्यंत (किल्ली);

        inc >>= 1;
    }

    परत वृक्ष;
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

    पर्यंत (bit & mask) {
        जर (किल्ली & bit) {
            next = गाठ->उजवा;

        } अन्यथा {
            next = गाठ->डावा;
        }

        जर (next == रिक्त) {
            थांबा;
        }

        bit >>= 1;
        गाठ = next;
    }

    जर (next) {
        जर (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            परत NGX_BUSY;
        }

        गाठ->मूल्य = मूल्य;
        परत NGX_OK;
    }

    पर्यंत (bit & mask) {
        next = ngx_radix_alloc(वृक्ष);
        जर (next == रिक्त) {
            परत NGX_ERROR;
        }

        next->उजवा = रिक्त;
        next->डावा = रिक्त;
        next->parent = गाठ;
        next->मूल्य = NGX_RADIX_NO_मूल्य;

        जर (किल्ली & bit) {
            गाठ->उजवा = next;

        } अन्यथा {
            गाठ->डावा = next;
        }

        bit >>= 1;
        गाठ = next;
    }

    गाठ->मूल्य = मूल्य;

    परत NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_वृक्ष_t *वृक्ष, uint32_t किल्ली, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_गाठ_t  *गाठ;

    bit = 0x80000000;
    गाठ = वृक्ष->मूळ;

    पर्यंत (गाठ && (bit & mask)) {
        जर (किल्ली & bit) {
            गाठ = गाठ->उजवा;

        } अन्यथा {
            गाठ = गाठ->डावा;
        }

        bit >>= 1;
    }

    जर (गाठ == रिक्त) {
        परत NGX_ERROR;
    }

    जर (गाठ->उजवा || गाठ->डावा) {
        जर (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            गाठ->मूल्य = NGX_RADIX_NO_मूल्य;
            परत NGX_OK;
        }

        परत NGX_ERROR;
    }

    साठी ( ;; ) {
        जर (गाठ->parent->उजवा == गाठ) {
            गाठ->parent->उजवा = रिक्त;

        } अन्यथा {
            गाठ->parent->डावा = रिक्त;
        }

        गाठ->उजवा = वृक्ष->मुक्त;
        वृक्ष->मुक्त = गाठ;

        गाठ = गाठ->parent;

        जर (गाठ->उजवा || गाठ->डावा) {
            थांबा;
        }

        जर (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            थांबा;
        }

        जर (गाठ->parent == रिक्त) {
            थांबा;
        }
    }

    परत NGX_OK;
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

    पर्यंत (गाठ) {
        जर (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            मूल्य = गाठ->मूल्य;
        }

        जर (किल्ली & bit) {
            गाठ = गाठ->उजवा;

        } अन्यथा {
            गाठ = गाठ->डावा;
        }

        bit >>= 1;
    }

    परत मूल्य;
}


#जर (NGX_HAVE_INET6)

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

    पर्यंत (bit & mask[i]) {
        जर (किल्ली[i] & bit) {
            next = गाठ->उजवा;

        } अन्यथा {
            next = गाठ->डावा;
        }

        जर (next == रिक्त) {
            थांबा;
        }

        bit >>= 1;
        गाठ = next;

        जर (bit == 0) {
            जर (++i == 16) {
                थांबा;
            }

            bit = 0x80;
        }
    }

    जर (next) {
        जर (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            परत NGX_BUSY;
        }

        गाठ->मूल्य = मूल्य;
        परत NGX_OK;
    }

    पर्यंत (bit & mask[i]) {
        next = ngx_radix_alloc(वृक्ष);
        जर (next == रिक्त) {
            परत NGX_ERROR;
        }

        next->उजवा = रिक्त;
        next->डावा = रिक्त;
        next->parent = गाठ;
        next->मूल्य = NGX_RADIX_NO_मूल्य;

        जर (किल्ली[i] & bit) {
            गाठ->उजवा = next;

        } अन्यथा {
            गाठ->डावा = next;
        }

        bit >>= 1;
        गाठ = next;

        जर (bit == 0) {
            जर (++i == 16) {
                थांबा;
            }

            bit = 0x80;
        }
    }

    गाठ->मूल्य = मूल्य;

    परत NGX_OK;
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

    पर्यंत (गाठ && (bit & mask[i])) {
        जर (किल्ली[i] & bit) {
            गाठ = गाठ->उजवा;

        } अन्यथा {
            गाठ = गाठ->डावा;
        }

        bit >>= 1;

        जर (bit == 0) {
            जर (++i == 16) {
                थांबा;
            }

            bit = 0x80;
        }
    }

    जर (गाठ == रिक्त) {
        परत NGX_ERROR;
    }

    जर (गाठ->उजवा || गाठ->डावा) {
        जर (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            गाठ->मूल्य = NGX_RADIX_NO_मूल्य;
            परत NGX_OK;
        }

        परत NGX_ERROR;
    }

    साठी ( ;; ) {
        जर (गाठ->parent->उजवा == गाठ) {
            गाठ->parent->उजवा = रिक्त;

        } अन्यथा {
            गाठ->parent->डावा = रिक्त;
        }

        गाठ->उजवा = वृक्ष->मुक्त;
        वृक्ष->मुक्त = गाठ;

        गाठ = गाठ->parent;

        जर (गाठ->उजवा || गाठ->डावा) {
            थांबा;
        }

        जर (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            थांबा;
        }

        जर (गाठ->parent == रिक्त) {
            थांबा;
        }
    }

    परत NGX_OK;
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

    पर्यंत (गाठ) {
        जर (गाठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            मूल्य = गाठ->मूल्य;
        }

        जर (किल्ली[i] & bit) {
            गाठ = गाठ->उजवा;

        } अन्यथा {
            गाठ = गाठ->डावा;
        }

        bit >>= 1;

        जर (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    परत मूल्य;
}

#endif


स्थिर ngx_radix_गाठ_t *
ngx_radix_alloc(ngx_radix_वृक्ष_t *वृक्ष)
{
    ngx_radix_गाठ_t  *p;

    जर (वृक्ष->मुक्त) {
        p = वृक्ष->मुक्त;
        वृक्ष->मुक्त = वृक्ष->मुक्त->उजवा;
        परत p;
    }

    जर (वृक्ष->size < आकार(ngx_radix_गाठ_t)) {
        वृक्ष->start = ngx_pmemalign(वृक्ष->pool, ngx_pagesize, ngx_pagesize);
        जर (वृक्ष->start == रिक्त) {
            परत रिक्त;
        }

        वृक्ष->size = ngx_pagesize;
    }

    p = (ngx_radix_गाठ_t *) वृक्ष->start;
    वृक्ष->start += आकार(ngx_radix_गाठ_t);
    वृक्ष->size -= आकार(ngx_radix_गाठ_t);

    परत p;
}
