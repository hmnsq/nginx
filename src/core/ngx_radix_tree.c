
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


സ്ഥിതം ngx_radix_കണ്ണി_t *ngx_radix_alloc(ngx_radix_വൃക്ഷം_t *വൃക്ഷം);


ngx_radix_വൃക്ഷം_t *
ngx_radix_വൃക്ഷം_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           താക്കോൽ, mask, inc;
    ngx_radix_വൃക്ഷം_t  *വൃക്ഷം;

    വൃക്ഷം = ngx_palloc(pool, വലുപ്പം(ngx_radix_വൃക്ഷം_t));
    എങ്കിൽ (വൃക്ഷം == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    വൃക്ഷം->pool = pool;
    വൃക്ഷം->സ്വതന്ത്രം = ശൂന്യ;
    വൃക്ഷം->start = ശൂന്യ;
    വൃക്ഷം->size = 0;

    വൃക്ഷം->വേര് = ngx_radix_alloc(വൃക്ഷം);
    എങ്കിൽ (വൃക്ഷം->വേര് == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    വൃക്ഷം->വേര്->വലത് = ശൂന്യ;
    വൃക്ഷം->വേര്->ഇടത് = ശൂന്യ;
    വൃക്ഷം->വേര്->parent = ശൂന്യ;
    വൃക്ഷം->വേര്->മൂല്യം = NGX_RADIX_NO_മൂല്യം;

    എങ്കിൽ (preallocate == 0) {
        തിരികെ വൃക്ഷം;
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

    എങ്കിൽ (preallocate == -1) {
        തിരഞ്ഞെടുപ്പ് (ngx_pagesize / വലുപ്പം(ngx_radix_കണ്ണി_t)) {

        /* amd64 */
        അവസ്ഥ 128:
            preallocate = 6;
            നിർത്തുക;

        /* i386, sparc64 */
        അവസ്ഥ 256:
            preallocate = 7;
            നിർത്തുക;

        /* sparc64 in 32-bit mode */
        സ്ഥിരസ്ഥിതി:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    ആകുമ്പോൾ (preallocate--) {

        താക്കോൽ = 0;
        mask >>= 1;
        mask |= 0x80000000;

        ചെയ്യുക {
            എങ്കിൽ (ngx_radix32tree_തിരുകുക(വൃക്ഷം, താക്കോൽ, mask, NGX_RADIX_NO_മൂല്യം)
                != NGX_OK)
            {
                തിരികെ ശൂന്യ;
            }

            താക്കോൽ += inc;

        } ആകുമ്പോൾ (താക്കോൽ);

        inc >>= 1;
    }

    തിരികെ വൃക്ഷം;
}


ngx_int_t
ngx_radix32tree_തിരുകുക(ngx_radix_വൃക്ഷം_t *വൃക്ഷം, uint32_t താക്കോൽ, uint32_t mask,
    uintptr_t മൂല്യം)
{
    uint32_t           bit;
    ngx_radix_കണ്ണി_t  *കണ്ണി, *next;

    bit = 0x80000000;

    കണ്ണി = വൃക്ഷം->വേര്;
    next = വൃക്ഷം->വേര്;

    ആകുമ്പോൾ (bit & mask) {
        എങ്കിൽ (താക്കോൽ & bit) {
            next = കണ്ണി->വലത്;

        } അല്ലെങ്കിൽ {
            next = കണ്ണി->ഇടത്;
        }

        എങ്കിൽ (next == ശൂന്യ) {
            നിർത്തുക;
        }

        bit >>= 1;
        കണ്ണി = next;
    }

    എങ്കിൽ (next) {
        എങ്കിൽ (കണ്ണി->മൂല്യം != NGX_RADIX_NO_മൂല്യം) {
            തിരികെ NGX_BUSY;
        }

        കണ്ണി->മൂല്യം = മൂല്യം;
        തിരികെ NGX_OK;
    }

    ആകുമ്പോൾ (bit & mask) {
        next = ngx_radix_alloc(വൃക്ഷം);
        എങ്കിൽ (next == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        next->വലത് = ശൂന്യ;
        next->ഇടത് = ശൂന്യ;
        next->parent = കണ്ണി;
        next->മൂല്യം = NGX_RADIX_NO_മൂല്യം;

        എങ്കിൽ (താക്കോൽ & bit) {
            കണ്ണി->വലത് = next;

        } അല്ലെങ്കിൽ {
            കണ്ണി->ഇടത് = next;
        }

        bit >>= 1;
        കണ്ണി = next;
    }

    കണ്ണി->മൂല്യം = മൂല്യം;

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_വൃക്ഷം_t *വൃക്ഷം, uint32_t താക്കോൽ, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_കണ്ണി_t  *കണ്ണി;

    bit = 0x80000000;
    കണ്ണി = വൃക്ഷം->വേര്;

    ആകുമ്പോൾ (കണ്ണി && (bit & mask)) {
        എങ്കിൽ (താക്കോൽ & bit) {
            കണ്ണി = കണ്ണി->വലത്;

        } അല്ലെങ്കിൽ {
            കണ്ണി = കണ്ണി->ഇടത്;
        }

        bit >>= 1;
    }

    എങ്കിൽ (കണ്ണി == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (കണ്ണി->വലത് || കണ്ണി->ഇടത്) {
        എങ്കിൽ (കണ്ണി->മൂല്യം != NGX_RADIX_NO_മൂല്യം) {
            കണ്ണി->മൂല്യം = NGX_RADIX_NO_മൂല്യം;
            തിരികെ NGX_OK;
        }

        തിരികെ NGX_ERROR;
    }

    വേണ്ടി ( ;; ) {
        എങ്കിൽ (കണ്ണി->parent->വലത് == കണ്ണി) {
            കണ്ണി->parent->വലത് = ശൂന്യ;

        } അല്ലെങ്കിൽ {
            കണ്ണി->parent->ഇടത് = ശൂന്യ;
        }

        കണ്ണി->വലത് = വൃക്ഷം->സ്വതന്ത്രം;
        വൃക്ഷം->സ്വതന്ത്രം = കണ്ണി;

        കണ്ണി = കണ്ണി->parent;

        എങ്കിൽ (കണ്ണി->വലത് || കണ്ണി->ഇടത്) {
            നിർത്തുക;
        }

        എങ്കിൽ (കണ്ണി->മൂല്യം != NGX_RADIX_NO_മൂല്യം) {
            നിർത്തുക;
        }

        എങ്കിൽ (കണ്ണി->parent == ശൂന്യ) {
            നിർത്തുക;
        }
    }

    തിരികെ NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_വൃക്ഷം_t *വൃക്ഷം, uint32_t താക്കോൽ)
{
    uint32_t           bit;
    uintptr_t          മൂല്യം;
    ngx_radix_കണ്ണി_t  *കണ്ണി;

    bit = 0x80000000;
    മൂല്യം = NGX_RADIX_NO_മൂല്യം;
    കണ്ണി = വൃക്ഷം->വേര്;

    ആകുമ്പോൾ (കണ്ണി) {
        എങ്കിൽ (കണ്ണി->മൂല്യം != NGX_RADIX_NO_മൂല്യം) {
            മൂല്യം = കണ്ണി->മൂല്യം;
        }

        എങ്കിൽ (താക്കോൽ & bit) {
            കണ്ണി = കണ്ണി->വലത്;

        } അല്ലെങ്കിൽ {
            കണ്ണി = കണ്ണി->ഇടത്;
        }

        bit >>= 1;
    }

    തിരികെ മൂല്യം;
}


#എങ്കിൽ (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_തിരുകുക(ngx_radix_വൃക്ഷം_t *വൃക്ഷം, u_char *താക്കോൽ, u_char *mask,
    uintptr_t മൂല്യം)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_കണ്ണി_t  *കണ്ണി, *next;

    i = 0;
    bit = 0x80;

    കണ്ണി = വൃക്ഷം->വേര്;
    next = വൃക്ഷം->വേര്;

    ആകുമ്പോൾ (bit & mask[i]) {
        എങ്കിൽ (താക്കോൽ[i] & bit) {
            next = കണ്ണി->വലത്;

        } അല്ലെങ്കിൽ {
            next = കണ്ണി->ഇടത്;
        }

        എങ്കിൽ (next == ശൂന്യ) {
            നിർത്തുക;
        }

        bit >>= 1;
        കണ്ണി = next;

        എങ്കിൽ (bit == 0) {
            എങ്കിൽ (++i == 16) {
                നിർത്തുക;
            }

            bit = 0x80;
        }
    }

    എങ്കിൽ (next) {
        എങ്കിൽ (കണ്ണി->മൂല്യം != NGX_RADIX_NO_മൂല്യം) {
            തിരികെ NGX_BUSY;
        }

        കണ്ണി->മൂല്യം = മൂല്യം;
        തിരികെ NGX_OK;
    }

    ആകുമ്പോൾ (bit & mask[i]) {
        next = ngx_radix_alloc(വൃക്ഷം);
        എങ്കിൽ (next == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        next->വലത് = ശൂന്യ;
        next->ഇടത് = ശൂന്യ;
        next->parent = കണ്ണി;
        next->മൂല്യം = NGX_RADIX_NO_മൂല്യം;

        എങ്കിൽ (താക്കോൽ[i] & bit) {
            കണ്ണി->വലത് = next;

        } അല്ലെങ്കിൽ {
            കണ്ണി->ഇടത് = next;
        }

        bit >>= 1;
        കണ്ണി = next;

        എങ്കിൽ (bit == 0) {
            എങ്കിൽ (++i == 16) {
                നിർത്തുക;
            }

            bit = 0x80;
        }
    }

    കണ്ണി->മൂല്യം = മൂല്യം;

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_വൃക്ഷം_t *വൃക്ഷം, u_char *താക്കോൽ, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_കണ്ണി_t  *കണ്ണി;

    i = 0;
    bit = 0x80;
    കണ്ണി = വൃക്ഷം->വേര്;

    ആകുമ്പോൾ (കണ്ണി && (bit & mask[i])) {
        എങ്കിൽ (താക്കോൽ[i] & bit) {
            കണ്ണി = കണ്ണി->വലത്;

        } അല്ലെങ്കിൽ {
            കണ്ണി = കണ്ണി->ഇടത്;
        }

        bit >>= 1;

        എങ്കിൽ (bit == 0) {
            എങ്കിൽ (++i == 16) {
                നിർത്തുക;
            }

            bit = 0x80;
        }
    }

    എങ്കിൽ (കണ്ണി == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (കണ്ണി->വലത് || കണ്ണി->ഇടത്) {
        എങ്കിൽ (കണ്ണി->മൂല്യം != NGX_RADIX_NO_മൂല്യം) {
            കണ്ണി->മൂല്യം = NGX_RADIX_NO_മൂല്യം;
            തിരികെ NGX_OK;
        }

        തിരികെ NGX_ERROR;
    }

    വേണ്ടി ( ;; ) {
        എങ്കിൽ (കണ്ണി->parent->വലത് == കണ്ണി) {
            കണ്ണി->parent->വലത് = ശൂന്യ;

        } അല്ലെങ്കിൽ {
            കണ്ണി->parent->ഇടത് = ശൂന്യ;
        }

        കണ്ണി->വലത് = വൃക്ഷം->സ്വതന്ത്രം;
        വൃക്ഷം->സ്വതന്ത്രം = കണ്ണി;

        കണ്ണി = കണ്ണി->parent;

        എങ്കിൽ (കണ്ണി->വലത് || കണ്ണി->ഇടത്) {
            നിർത്തുക;
        }

        എങ്കിൽ (കണ്ണി->മൂല്യം != NGX_RADIX_NO_മൂല്യം) {
            നിർത്തുക;
        }

        എങ്കിൽ (കണ്ണി->parent == ശൂന്യ) {
            നിർത്തുക;
        }
    }

    തിരികെ NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_വൃക്ഷം_t *വൃക്ഷം, u_char *താക്കോൽ)
{
    u_char             bit;
    uintptr_t          മൂല്യം;
    ngx_uint_t         i;
    ngx_radix_കണ്ണി_t  *കണ്ണി;

    i = 0;
    bit = 0x80;
    മൂല്യം = NGX_RADIX_NO_മൂല്യം;
    കണ്ണി = വൃക്ഷം->വേര്;

    ആകുമ്പോൾ (കണ്ണി) {
        എങ്കിൽ (കണ്ണി->മൂല്യം != NGX_RADIX_NO_മൂല്യം) {
            മൂല്യം = കണ്ണി->മൂല്യം;
        }

        എങ്കിൽ (താക്കോൽ[i] & bit) {
            കണ്ണി = കണ്ണി->വലത്;

        } അല്ലെങ്കിൽ {
            കണ്ണി = കണ്ണി->ഇടത്;
        }

        bit >>= 1;

        എങ്കിൽ (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    തിരികെ മൂല്യം;
}

#endif


സ്ഥിതം ngx_radix_കണ്ണി_t *
ngx_radix_alloc(ngx_radix_വൃക്ഷം_t *വൃക്ഷം)
{
    ngx_radix_കണ്ണി_t  *p;

    എങ്കിൽ (വൃക്ഷം->സ്വതന്ത്രം) {
        p = വൃക്ഷം->സ്വതന്ത്രം;
        വൃക്ഷം->സ്വതന്ത്രം = വൃക്ഷം->സ്വതന്ത്രം->വലത്;
        തിരികെ p;
    }

    എങ്കിൽ (വൃക്ഷം->size < വലുപ്പം(ngx_radix_കണ്ണി_t)) {
        വൃക്ഷം->start = ngx_pmemalign(വൃക്ഷം->pool, ngx_pagesize, ngx_pagesize);
        എങ്കിൽ (വൃക്ഷം->start == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }

        വൃക്ഷം->size = ngx_pagesize;
    }

    p = (ngx_radix_കണ്ണി_t *) വൃക്ഷം->start;
    വൃക്ഷം->start += വലുപ്പം(ngx_radix_കണ്ണി_t);
    വൃക്ഷം->size -= വലുപ്പം(ngx_radix_കണ്ണി_t);

    തിരികെ p;
}
