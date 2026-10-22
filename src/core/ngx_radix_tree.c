
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


अटल ngx_radix_गाँठ_t *ngx_radix_alloc(ngx_radix_वृक्ष_t *वृक्ष);


ngx_radix_वृक्ष_t *
ngx_radix_वृक्ष_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           कुंजी, mask, inc;
    ngx_radix_वृक्ष_t  *वृक्ष;

    वृक्ष = ngx_palloc(pool, माप(ngx_radix_वृक्ष_t));
    अगर (वृक्ष == शून्य) {
        वापस शून्य;
    }

    वृक्ष->pool = pool;
    वृक्ष->मुक्त = शून्य;
    वृक्ष->start = शून्य;
    वृक्ष->आकार = 0;

    वृक्ष->मूल = ngx_radix_alloc(वृक्ष);
    अगर (वृक्ष->मूल == शून्य) {
        वापस शून्य;
    }

    वृक्ष->मूल->दायाँ = शून्य;
    वृक्ष->मूल->बायाँ = शून्य;
    वृक्ष->मूल->parent = शून्य;
    वृक्ष->मूल->मूल्य = NGX_RADIX_NO_मूल्य;

    अगर (preallocate == 0) {
        वापस वृक्ष;
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

    अगर (preallocate == -1) {
        चयन (ngx_pagesize / माप(ngx_radix_गाँठ_t)) {

        /* amd64 */
        हाल 128:
            preallocate = 6;
            अवरोध;

        /* i386, sparc64 */
        हाल 256:
            preallocate = 7;
            अवरोध;

        /* sparc64 in 32-bit mode */
        शेष:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    जबतक (preallocate--) {

        कुंजी = 0;
        mask >>= 1;
        mask |= 0x80000000;

        करो {
            अगर (ngx_radix32tree_जोड़ो(वृक्ष, कुंजी, mask, NGX_RADIX_NO_मूल्य)
                != NGX_OK)
            {
                वापस शून्य;
            }

            कुंजी += inc;

        } जबतक (कुंजी);

        inc >>= 1;
    }

    वापस वृक्ष;
}


ngx_int_t
ngx_radix32tree_जोड़ो(ngx_radix_वृक्ष_t *वृक्ष, uint32_t कुंजी, uint32_t mask,
    uintptr_t मूल्य)
{
    uint32_t           bit;
    ngx_radix_गाँठ_t  *गाँठ, *अग्र;

    bit = 0x80000000;

    गाँठ = वृक्ष->मूल;
    अग्र = वृक्ष->मूल;

    जबतक (bit & mask) {
        अगर (कुंजी & bit) {
            अग्र = गाँठ->दायाँ;

        } अन्यथा {
            अग्र = गाँठ->बायाँ;
        }

        अगर (अग्र == शून्य) {
            अवरोध;
        }

        bit >>= 1;
        गाँठ = अग्र;
    }

    अगर (अग्र) {
        अगर (गाँठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            वापस NGX_BUSY;
        }

        गाँठ->मूल्य = मूल्य;
        वापस NGX_OK;
    }

    जबतक (bit & mask) {
        अग्र = ngx_radix_alloc(वृक्ष);
        अगर (अग्र == शून्य) {
            वापस NGX_ERROR;
        }

        अग्र->दायाँ = शून्य;
        अग्र->बायाँ = शून्य;
        अग्र->parent = गाँठ;
        अग्र->मूल्य = NGX_RADIX_NO_मूल्य;

        अगर (कुंजी & bit) {
            गाँठ->दायाँ = अग्र;

        } अन्यथा {
            गाँठ->बायाँ = अग्र;
        }

        bit >>= 1;
        गाँठ = अग्र;
    }

    गाँठ->मूल्य = मूल्य;

    वापस NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_वृक्ष_t *वृक्ष, uint32_t कुंजी, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_गाँठ_t  *गाँठ;

    bit = 0x80000000;
    गाँठ = वृक्ष->मूल;

    जबतक (गाँठ && (bit & mask)) {
        अगर (कुंजी & bit) {
            गाँठ = गाँठ->दायाँ;

        } अन्यथा {
            गाँठ = गाँठ->बायाँ;
        }

        bit >>= 1;
    }

    अगर (गाँठ == शून्य) {
        वापस NGX_ERROR;
    }

    अगर (गाँठ->दायाँ || गाँठ->बायाँ) {
        अगर (गाँठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            गाँठ->मूल्य = NGX_RADIX_NO_मूल्य;
            वापस NGX_OK;
        }

        वापस NGX_ERROR;
    }

    क्रम ( ;; ) {
        अगर (गाँठ->parent->दायाँ == गाँठ) {
            गाँठ->parent->दायाँ = शून्य;

        } अन्यथा {
            गाँठ->parent->बायाँ = शून्य;
        }

        गाँठ->दायाँ = वृक्ष->मुक्त;
        वृक्ष->मुक्त = गाँठ;

        गाँठ = गाँठ->parent;

        अगर (गाँठ->दायाँ || गाँठ->बायाँ) {
            अवरोध;
        }

        अगर (गाँठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            अवरोध;
        }

        अगर (गाँठ->parent == शून्य) {
            अवरोध;
        }
    }

    वापस NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_वृक्ष_t *वृक्ष, uint32_t कुंजी)
{
    uint32_t           bit;
    uintptr_t          मूल्य;
    ngx_radix_गाँठ_t  *गाँठ;

    bit = 0x80000000;
    मूल्य = NGX_RADIX_NO_मूल्य;
    गाँठ = वृक्ष->मूल;

    जबतक (गाँठ) {
        अगर (गाँठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            मूल्य = गाँठ->मूल्य;
        }

        अगर (कुंजी & bit) {
            गाँठ = गाँठ->दायाँ;

        } अन्यथा {
            गाँठ = गाँठ->बायाँ;
        }

        bit >>= 1;
    }

    वापस मूल्य;
}


#अगर (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_जोड़ो(ngx_radix_वृक्ष_t *वृक्ष, u_char *कुंजी, u_char *mask,
    uintptr_t मूल्य)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_गाँठ_t  *गाँठ, *अग्र;

    i = 0;
    bit = 0x80;

    गाँठ = वृक्ष->मूल;
    अग्र = वृक्ष->मूल;

    जबतक (bit & mask[i]) {
        अगर (कुंजी[i] & bit) {
            अग्र = गाँठ->दायाँ;

        } अन्यथा {
            अग्र = गाँठ->बायाँ;
        }

        अगर (अग्र == शून्य) {
            अवरोध;
        }

        bit >>= 1;
        गाँठ = अग्र;

        अगर (bit == 0) {
            अगर (++i == 16) {
                अवरोध;
            }

            bit = 0x80;
        }
    }

    अगर (अग्र) {
        अगर (गाँठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            वापस NGX_BUSY;
        }

        गाँठ->मूल्य = मूल्य;
        वापस NGX_OK;
    }

    जबतक (bit & mask[i]) {
        अग्र = ngx_radix_alloc(वृक्ष);
        अगर (अग्र == शून्य) {
            वापस NGX_ERROR;
        }

        अग्र->दायाँ = शून्य;
        अग्र->बायाँ = शून्य;
        अग्र->parent = गाँठ;
        अग्र->मूल्य = NGX_RADIX_NO_मूल्य;

        अगर (कुंजी[i] & bit) {
            गाँठ->दायाँ = अग्र;

        } अन्यथा {
            गाँठ->बायाँ = अग्र;
        }

        bit >>= 1;
        गाँठ = अग्र;

        अगर (bit == 0) {
            अगर (++i == 16) {
                अवरोध;
            }

            bit = 0x80;
        }
    }

    गाँठ->मूल्य = मूल्य;

    वापस NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_वृक्ष_t *वृक्ष, u_char *कुंजी, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_गाँठ_t  *गाँठ;

    i = 0;
    bit = 0x80;
    गाँठ = वृक्ष->मूल;

    जबतक (गाँठ && (bit & mask[i])) {
        अगर (कुंजी[i] & bit) {
            गाँठ = गाँठ->दायाँ;

        } अन्यथा {
            गाँठ = गाँठ->बायाँ;
        }

        bit >>= 1;

        अगर (bit == 0) {
            अगर (++i == 16) {
                अवरोध;
            }

            bit = 0x80;
        }
    }

    अगर (गाँठ == शून्य) {
        वापस NGX_ERROR;
    }

    अगर (गाँठ->दायाँ || गाँठ->बायाँ) {
        अगर (गाँठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            गाँठ->मूल्य = NGX_RADIX_NO_मूल्य;
            वापस NGX_OK;
        }

        वापस NGX_ERROR;
    }

    क्रम ( ;; ) {
        अगर (गाँठ->parent->दायाँ == गाँठ) {
            गाँठ->parent->दायाँ = शून्य;

        } अन्यथा {
            गाँठ->parent->बायाँ = शून्य;
        }

        गाँठ->दायाँ = वृक्ष->मुक्त;
        वृक्ष->मुक्त = गाँठ;

        गाँठ = गाँठ->parent;

        अगर (गाँठ->दायाँ || गाँठ->बायाँ) {
            अवरोध;
        }

        अगर (गाँठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            अवरोध;
        }

        अगर (गाँठ->parent == शून्य) {
            अवरोध;
        }
    }

    वापस NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_वृक्ष_t *वृक्ष, u_char *कुंजी)
{
    u_char             bit;
    uintptr_t          मूल्य;
    ngx_uint_t         i;
    ngx_radix_गाँठ_t  *गाँठ;

    i = 0;
    bit = 0x80;
    मूल्य = NGX_RADIX_NO_मूल्य;
    गाँठ = वृक्ष->मूल;

    जबतक (गाँठ) {
        अगर (गाँठ->मूल्य != NGX_RADIX_NO_मूल्य) {
            मूल्य = गाँठ->मूल्य;
        }

        अगर (कुंजी[i] & bit) {
            गाँठ = गाँठ->दायाँ;

        } अन्यथा {
            गाँठ = गाँठ->बायाँ;
        }

        bit >>= 1;

        अगर (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    वापस मूल्य;
}

#पूर्ण_अगर


अटल ngx_radix_गाँठ_t *
ngx_radix_alloc(ngx_radix_वृक्ष_t *वृक्ष)
{
    ngx_radix_गाँठ_t  *p;

    अगर (वृक्ष->मुक्त) {
        p = वृक्ष->मुक्त;
        वृक्ष->मुक्त = वृक्ष->मुक्त->दायाँ;
        वापस p;
    }

    अगर (वृक्ष->आकार < माप(ngx_radix_गाँठ_t)) {
        वृक्ष->start = ngx_pmemalign(वृक्ष->pool, ngx_pagesize, ngx_pagesize);
        अगर (वृक्ष->start == शून्य) {
            वापस शून्य;
        }

        वृक्ष->आकार = ngx_pagesize;
    }

    p = (ngx_radix_गाँठ_t *) वृक्ष->start;
    वृक्ष->start += माप(ngx_radix_गाँठ_t);
    वृक्ष->आकार -= माप(ngx_radix_गाँठ_t);

    वापस p;
}
