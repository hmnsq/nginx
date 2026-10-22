
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


நிலையான ngx_radix_முனை_t *ngx_radix_alloc(ngx_radix_மரம்_t *மரம்);


ngx_radix_மரம்_t *
ngx_radix_மரம்_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           சாவி, mask, inc;
    ngx_radix_மரம்_t  *மரம்;

    மரம் = ngx_palloc(pool, அளவு(ngx_radix_மரம்_t));
    எனில் (மரம் == வெற்று) {
        திரும்பு வெற்று;
    }

    மரம்->pool = pool;
    மரம்->விடுவி = வெற்று;
    மரம்->start = வெற்று;
    மரம்->size = 0;

    மரம்->வேர் = ngx_radix_alloc(மரம்);
    எனில் (மரம்->வேர் == வெற்று) {
        திரும்பு வெற்று;
    }

    மரம்->வேர்->வலது = வெற்று;
    மரம்->வேர்->இடது = வெற்று;
    மரம்->வேர்->parent = வெற்று;
    மரம்->வேர்->மதிப்பு = NGX_RADIX_NO_மதிப்பு;

    எனில் (preallocate == 0) {
        திரும்பு மரம்;
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

    எனில் (preallocate == -1) {
        தேர்வு (ngx_pagesize / அளவு(ngx_radix_முனை_t)) {

        /* amd64 */
        நிலை 128:
            preallocate = 6;
            நிறுத்து;

        /* i386, sparc64 */
        நிலை 256:
            preallocate = 7;
            நிறுத்து;

        /* sparc64 in 32-bit mode */
        இயல்பு:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    போது (preallocate--) {

        சாவி = 0;
        mask >>= 1;
        mask |= 0x80000000;

        செய் {
            எனில் (ngx_radix32tree_செருகு(மரம், சாவி, mask, NGX_RADIX_NO_மதிப்பு)
                != NGX_OK)
            {
                திரும்பு வெற்று;
            }

            சாவி += inc;

        } போது (சாவி);

        inc >>= 1;
    }

    திரும்பு மரம்;
}


ngx_int_t
ngx_radix32tree_செருகு(ngx_radix_மரம்_t *மரம், uint32_t சாவி, uint32_t mask,
    uintptr_t மதிப்பு)
{
    uint32_t           bit;
    ngx_radix_முனை_t  *முனை, *next;

    bit = 0x80000000;

    முனை = மரம்->வேர்;
    next = மரம்->வேர்;

    போது (bit & mask) {
        எனில் (சாவி & bit) {
            next = முனை->வலது;

        } இல்லையெனில் {
            next = முனை->இடது;
        }

        எனில் (next == வெற்று) {
            நிறுத்து;
        }

        bit >>= 1;
        முனை = next;
    }

    எனில் (next) {
        எனில் (முனை->மதிப்பு != NGX_RADIX_NO_மதிப்பு) {
            திரும்பு NGX_BUSY;
        }

        முனை->மதிப்பு = மதிப்பு;
        திரும்பு NGX_OK;
    }

    போது (bit & mask) {
        next = ngx_radix_alloc(மரம்);
        எனில் (next == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        next->வலது = வெற்று;
        next->இடது = வெற்று;
        next->parent = முனை;
        next->மதிப்பு = NGX_RADIX_NO_மதிப்பு;

        எனில் (சாவி & bit) {
            முனை->வலது = next;

        } இல்லையெனில் {
            முனை->இடது = next;
        }

        bit >>= 1;
        முனை = next;
    }

    முனை->மதிப்பு = மதிப்பு;

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_மரம்_t *மரம், uint32_t சாவி, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_முனை_t  *முனை;

    bit = 0x80000000;
    முனை = மரம்->வேர்;

    போது (முனை && (bit & mask)) {
        எனில் (சாவி & bit) {
            முனை = முனை->வலது;

        } இல்லையெனில் {
            முனை = முனை->இடது;
        }

        bit >>= 1;
    }

    எனில் (முனை == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    எனில் (முனை->வலது || முனை->இடது) {
        எனில் (முனை->மதிப்பு != NGX_RADIX_NO_மதிப்பு) {
            முனை->மதிப்பு = NGX_RADIX_NO_மதிப்பு;
            திரும்பு NGX_OK;
        }

        திரும்பு NGX_ERROR;
    }

    க்கு ( ;; ) {
        எனில் (முனை->parent->வலது == முனை) {
            முனை->parent->வலது = வெற்று;

        } இல்லையெனில் {
            முனை->parent->இடது = வெற்று;
        }

        முனை->வலது = மரம்->விடுவி;
        மரம்->விடுவி = முனை;

        முனை = முனை->parent;

        எனில் (முனை->வலது || முனை->இடது) {
            நிறுத்து;
        }

        எனில் (முனை->மதிப்பு != NGX_RADIX_NO_மதிப்பு) {
            நிறுத்து;
        }

        எனில் (முனை->parent == வெற்று) {
            நிறுத்து;
        }
    }

    திரும்பு NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_மரம்_t *மரம், uint32_t சாவி)
{
    uint32_t           bit;
    uintptr_t          மதிப்பு;
    ngx_radix_முனை_t  *முனை;

    bit = 0x80000000;
    மதிப்பு = NGX_RADIX_NO_மதிப்பு;
    முனை = மரம்->வேர்;

    போது (முனை) {
        எனில் (முனை->மதிப்பு != NGX_RADIX_NO_மதிப்பு) {
            மதிப்பு = முனை->மதிப்பு;
        }

        எனில் (சாவி & bit) {
            முனை = முனை->வலது;

        } இல்லையெனில் {
            முனை = முனை->இடது;
        }

        bit >>= 1;
    }

    திரும்பு மதிப்பு;
}


#எனில் (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_செருகு(ngx_radix_மரம்_t *மரம், u_char *சாவி, u_char *mask,
    uintptr_t மதிப்பு)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_முனை_t  *முனை, *next;

    i = 0;
    bit = 0x80;

    முனை = மரம்->வேர்;
    next = மரம்->வேர்;

    போது (bit & mask[i]) {
        எனில் (சாவி[i] & bit) {
            next = முனை->வலது;

        } இல்லையெனில் {
            next = முனை->இடது;
        }

        எனில் (next == வெற்று) {
            நிறுத்து;
        }

        bit >>= 1;
        முனை = next;

        எனில் (bit == 0) {
            எனில் (++i == 16) {
                நிறுத்து;
            }

            bit = 0x80;
        }
    }

    எனில் (next) {
        எனில் (முனை->மதிப்பு != NGX_RADIX_NO_மதிப்பு) {
            திரும்பு NGX_BUSY;
        }

        முனை->மதிப்பு = மதிப்பு;
        திரும்பு NGX_OK;
    }

    போது (bit & mask[i]) {
        next = ngx_radix_alloc(மரம்);
        எனில் (next == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        next->வலது = வெற்று;
        next->இடது = வெற்று;
        next->parent = முனை;
        next->மதிப்பு = NGX_RADIX_NO_மதிப்பு;

        எனில் (சாவி[i] & bit) {
            முனை->வலது = next;

        } இல்லையெனில் {
            முனை->இடது = next;
        }

        bit >>= 1;
        முனை = next;

        எனில் (bit == 0) {
            எனில் (++i == 16) {
                நிறுத்து;
            }

            bit = 0x80;
        }
    }

    முனை->மதிப்பு = மதிப்பு;

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_மரம்_t *மரம், u_char *சாவி, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_முனை_t  *முனை;

    i = 0;
    bit = 0x80;
    முனை = மரம்->வேர்;

    போது (முனை && (bit & mask[i])) {
        எனில் (சாவி[i] & bit) {
            முனை = முனை->வலது;

        } இல்லையெனில் {
            முனை = முனை->இடது;
        }

        bit >>= 1;

        எனில் (bit == 0) {
            எனில் (++i == 16) {
                நிறுத்து;
            }

            bit = 0x80;
        }
    }

    எனில் (முனை == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    எனில் (முனை->வலது || முனை->இடது) {
        எனில் (முனை->மதிப்பு != NGX_RADIX_NO_மதிப்பு) {
            முனை->மதிப்பு = NGX_RADIX_NO_மதிப்பு;
            திரும்பு NGX_OK;
        }

        திரும்பு NGX_ERROR;
    }

    க்கு ( ;; ) {
        எனில் (முனை->parent->வலது == முனை) {
            முனை->parent->வலது = வெற்று;

        } இல்லையெனில் {
            முனை->parent->இடது = வெற்று;
        }

        முனை->வலது = மரம்->விடுவி;
        மரம்->விடுவி = முனை;

        முனை = முனை->parent;

        எனில் (முனை->வலது || முனை->இடது) {
            நிறுத்து;
        }

        எனில் (முனை->மதிப்பு != NGX_RADIX_NO_மதிப்பு) {
            நிறுத்து;
        }

        எனில் (முனை->parent == வெற்று) {
            நிறுத்து;
        }
    }

    திரும்பு NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_மரம்_t *மரம், u_char *சாவி)
{
    u_char             bit;
    uintptr_t          மதிப்பு;
    ngx_uint_t         i;
    ngx_radix_முனை_t  *முனை;

    i = 0;
    bit = 0x80;
    மதிப்பு = NGX_RADIX_NO_மதிப்பு;
    முனை = மரம்->வேர்;

    போது (முனை) {
        எனில் (முனை->மதிப்பு != NGX_RADIX_NO_மதிப்பு) {
            மதிப்பு = முனை->மதிப்பு;
        }

        எனில் (சாவி[i] & bit) {
            முனை = முனை->வலது;

        } இல்லையெனில் {
            முனை = முனை->இடது;
        }

        bit >>= 1;

        எனில் (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    திரும்பு மதிப்பு;
}

#endif


நிலையான ngx_radix_முனை_t *
ngx_radix_alloc(ngx_radix_மரம்_t *மரம்)
{
    ngx_radix_முனை_t  *p;

    எனில் (மரம்->விடுவி) {
        p = மரம்->விடுவி;
        மரம்->விடுவி = மரம்->விடுவி->வலது;
        திரும்பு p;
    }

    எனில் (மரம்->size < அளவு(ngx_radix_முனை_t)) {
        மரம்->start = ngx_pmemalign(மரம்->pool, ngx_pagesize, ngx_pagesize);
        எனில் (மரம்->start == வெற்று) {
            திரும்பு வெற்று;
        }

        மரம்->size = ngx_pagesize;
    }

    p = (ngx_radix_முனை_t *) மரம்->start;
    மரம்->start += அளவு(ngx_radix_முனை_t);
    மரம்->size -= அளவு(ngx_radix_முனை_t);

    திரும்பு p;
}
