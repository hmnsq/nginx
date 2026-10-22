
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


સ્થિર ngx_radix_ગાંઠ_t *ngx_radix_alloc(ngx_radix_વૃક્ષ_t *વૃક્ષ);


ngx_radix_વૃક્ષ_t *
ngx_radix_વૃક્ષ_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           ચાવી, mask, inc;
    ngx_radix_વૃક્ષ_t  *વૃક્ષ;

    વૃક્ષ = ngx_palloc(pool, કદ(ngx_radix_વૃક્ષ_t));
    જો (વૃક્ષ == ખાલી) {
        પાછા ખાલી;
    }

    વૃક્ષ->pool = pool;
    વૃક્ષ->મુક્ત = ખાલી;
    વૃક્ષ->start = ખાલી;
    વૃક્ષ->size = 0;

    વૃક્ષ->મૂળ = ngx_radix_alloc(વૃક્ષ);
    જો (વૃક્ષ->મૂળ == ખાલી) {
        પાછા ખાલી;
    }

    વૃક્ષ->મૂળ->જમણું = ખાલી;
    વૃક્ષ->મૂળ->ડાબું = ખાલી;
    વૃક્ષ->મૂળ->parent = ખાલી;
    વૃક્ષ->મૂળ->કિંમત = NGX_RADIX_NO_કિંમત;

    જો (preallocate == 0) {
        પાછા વૃક્ષ;
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

    જો (preallocate == -1) {
        પસંદગી (ngx_pagesize / કદ(ngx_radix_ગાંઠ_t)) {

        /* amd64 */
        સ્થિતિ 128:
            preallocate = 6;
            તોડો;

        /* i386, sparc64 */
        સ્થિતિ 256:
            preallocate = 7;
            તોડો;

        /* sparc64 in 32-bit mode */
        મૂળભૂત:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    જ્યારે (preallocate--) {

        ચાવી = 0;
        mask >>= 1;
        mask |= 0x80000000;

        કરો {
            જો (ngx_radix32tree_ઉમેરો(વૃક્ષ, ચાવી, mask, NGX_RADIX_NO_કિંમત)
                != NGX_OK)
            {
                પાછા ખાલી;
            }

            ચાવી += inc;

        } જ્યારે (ચાવી);

        inc >>= 1;
    }

    પાછા વૃક્ષ;
}


ngx_int_t
ngx_radix32tree_ઉમેરો(ngx_radix_વૃક્ષ_t *વૃક્ષ, uint32_t ચાવી, uint32_t mask,
    uintptr_t કિંમત)
{
    uint32_t           bit;
    ngx_radix_ગાંઠ_t  *ગાંઠ, *next;

    bit = 0x80000000;

    ગાંઠ = વૃક્ષ->મૂળ;
    next = વૃક્ષ->મૂળ;

    જ્યારે (bit & mask) {
        જો (ચાવી & bit) {
            next = ગાંઠ->જમણું;

        } નહીંતર {
            next = ગાંઠ->ડાબું;
        }

        જો (next == ખાલી) {
            તોડો;
        }

        bit >>= 1;
        ગાંઠ = next;
    }

    જો (next) {
        જો (ગાંઠ->કિંમત != NGX_RADIX_NO_કિંમત) {
            પાછા NGX_BUSY;
        }

        ગાંઠ->કિંમત = કિંમત;
        પાછા NGX_OK;
    }

    જ્યારે (bit & mask) {
        next = ngx_radix_alloc(વૃક્ષ);
        જો (next == ખાલી) {
            પાછા NGX_ERROR;
        }

        next->જમણું = ખાલી;
        next->ડાબું = ખાલી;
        next->parent = ગાંઠ;
        next->કિંમત = NGX_RADIX_NO_કિંમત;

        જો (ચાવી & bit) {
            ગાંઠ->જમણું = next;

        } નહીંતર {
            ગાંઠ->ડાબું = next;
        }

        bit >>= 1;
        ગાંઠ = next;
    }

    ગાંઠ->કિંમત = કિંમત;

    પાછા NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_વૃક્ષ_t *વૃક્ષ, uint32_t ચાવી, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_ગાંઠ_t  *ગાંઠ;

    bit = 0x80000000;
    ગાંઠ = વૃક્ષ->મૂળ;

    જ્યારે (ગાંઠ && (bit & mask)) {
        જો (ચાવી & bit) {
            ગાંઠ = ગાંઠ->જમણું;

        } નહીંતર {
            ગાંઠ = ગાંઠ->ડાબું;
        }

        bit >>= 1;
    }

    જો (ગાંઠ == ખાલી) {
        પાછા NGX_ERROR;
    }

    જો (ગાંઠ->જમણું || ગાંઠ->ડાબું) {
        જો (ગાંઠ->કિંમત != NGX_RADIX_NO_કિંમત) {
            ગાંઠ->કિંમત = NGX_RADIX_NO_કિંમત;
            પાછા NGX_OK;
        }

        પાછા NGX_ERROR;
    }

    માટે ( ;; ) {
        જો (ગાંઠ->parent->જમણું == ગાંઠ) {
            ગાંઠ->parent->જમણું = ખાલી;

        } નહીંતર {
            ગાંઠ->parent->ડાબું = ખાલી;
        }

        ગાંઠ->જમણું = વૃક્ષ->મુક્ત;
        વૃક્ષ->મુક્ત = ગાંઠ;

        ગાંઠ = ગાંઠ->parent;

        જો (ગાંઠ->જમણું || ગાંઠ->ડાબું) {
            તોડો;
        }

        જો (ગાંઠ->કિંમત != NGX_RADIX_NO_કિંમત) {
            તોડો;
        }

        જો (ગાંઠ->parent == ખાલી) {
            તોડો;
        }
    }

    પાછા NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_વૃક્ષ_t *વૃક્ષ, uint32_t ચાવી)
{
    uint32_t           bit;
    uintptr_t          કિંમત;
    ngx_radix_ગાંઠ_t  *ગાંઠ;

    bit = 0x80000000;
    કિંમત = NGX_RADIX_NO_કિંમત;
    ગાંઠ = વૃક્ષ->મૂળ;

    જ્યારે (ગાંઠ) {
        જો (ગાંઠ->કિંમત != NGX_RADIX_NO_કિંમત) {
            કિંમત = ગાંઠ->કિંમત;
        }

        જો (ચાવી & bit) {
            ગાંઠ = ગાંઠ->જમણું;

        } નહીંતર {
            ગાંઠ = ગાંઠ->ડાબું;
        }

        bit >>= 1;
    }

    પાછા કિંમત;
}


#જો (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_ઉમેરો(ngx_radix_વૃક્ષ_t *વૃક્ષ, u_char *ચાવી, u_char *mask,
    uintptr_t કિંમત)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_ગાંઠ_t  *ગાંઠ, *next;

    i = 0;
    bit = 0x80;

    ગાંઠ = વૃક્ષ->મૂળ;
    next = વૃક્ષ->મૂળ;

    જ્યારે (bit & mask[i]) {
        જો (ચાવી[i] & bit) {
            next = ગાંઠ->જમણું;

        } નહીંતર {
            next = ગાંઠ->ડાબું;
        }

        જો (next == ખાલી) {
            તોડો;
        }

        bit >>= 1;
        ગાંઠ = next;

        જો (bit == 0) {
            જો (++i == 16) {
                તોડો;
            }

            bit = 0x80;
        }
    }

    જો (next) {
        જો (ગાંઠ->કિંમત != NGX_RADIX_NO_કિંમત) {
            પાછા NGX_BUSY;
        }

        ગાંઠ->કિંમત = કિંમત;
        પાછા NGX_OK;
    }

    જ્યારે (bit & mask[i]) {
        next = ngx_radix_alloc(વૃક્ષ);
        જો (next == ખાલી) {
            પાછા NGX_ERROR;
        }

        next->જમણું = ખાલી;
        next->ડાબું = ખાલી;
        next->parent = ગાંઠ;
        next->કિંમત = NGX_RADIX_NO_કિંમત;

        જો (ચાવી[i] & bit) {
            ગાંઠ->જમણું = next;

        } નહીંતર {
            ગાંઠ->ડાબું = next;
        }

        bit >>= 1;
        ગાંઠ = next;

        જો (bit == 0) {
            જો (++i == 16) {
                તોડો;
            }

            bit = 0x80;
        }
    }

    ગાંઠ->કિંમત = કિંમત;

    પાછા NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_વૃક્ષ_t *વૃક્ષ, u_char *ચાવી, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_ગાંઠ_t  *ગાંઠ;

    i = 0;
    bit = 0x80;
    ગાંઠ = વૃક્ષ->મૂળ;

    જ્યારે (ગાંઠ && (bit & mask[i])) {
        જો (ચાવી[i] & bit) {
            ગાંઠ = ગાંઠ->જમણું;

        } નહીંતર {
            ગાંઠ = ગાંઠ->ડાબું;
        }

        bit >>= 1;

        જો (bit == 0) {
            જો (++i == 16) {
                તોડો;
            }

            bit = 0x80;
        }
    }

    જો (ગાંઠ == ખાલી) {
        પાછા NGX_ERROR;
    }

    જો (ગાંઠ->જમણું || ગાંઠ->ડાબું) {
        જો (ગાંઠ->કિંમત != NGX_RADIX_NO_કિંમત) {
            ગાંઠ->કિંમત = NGX_RADIX_NO_કિંમત;
            પાછા NGX_OK;
        }

        પાછા NGX_ERROR;
    }

    માટે ( ;; ) {
        જો (ગાંઠ->parent->જમણું == ગાંઠ) {
            ગાંઠ->parent->જમણું = ખાલી;

        } નહીંતર {
            ગાંઠ->parent->ડાબું = ખાલી;
        }

        ગાંઠ->જમણું = વૃક્ષ->મુક્ત;
        વૃક્ષ->મુક્ત = ગાંઠ;

        ગાંઠ = ગાંઠ->parent;

        જો (ગાંઠ->જમણું || ગાંઠ->ડાબું) {
            તોડો;
        }

        જો (ગાંઠ->કિંમત != NGX_RADIX_NO_કિંમત) {
            તોડો;
        }

        જો (ગાંઠ->parent == ખાલી) {
            તોડો;
        }
    }

    પાછા NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_વૃક્ષ_t *વૃક્ષ, u_char *ચાવી)
{
    u_char             bit;
    uintptr_t          કિંમત;
    ngx_uint_t         i;
    ngx_radix_ગાંઠ_t  *ગાંઠ;

    i = 0;
    bit = 0x80;
    કિંમત = NGX_RADIX_NO_કિંમત;
    ગાંઠ = વૃક્ષ->મૂળ;

    જ્યારે (ગાંઠ) {
        જો (ગાંઠ->કિંમત != NGX_RADIX_NO_કિંમત) {
            કિંમત = ગાંઠ->કિંમત;
        }

        જો (ચાવી[i] & bit) {
            ગાંઠ = ગાંઠ->જમણું;

        } નહીંતર {
            ગાંઠ = ગાંઠ->ડાબું;
        }

        bit >>= 1;

        જો (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    પાછા કિંમત;
}

#endif


સ્થિર ngx_radix_ગાંઠ_t *
ngx_radix_alloc(ngx_radix_વૃક્ષ_t *વૃક્ષ)
{
    ngx_radix_ગાંઠ_t  *p;

    જો (વૃક્ષ->મુક્ત) {
        p = વૃક્ષ->મુક્ત;
        વૃક્ષ->મુક્ત = વૃક્ષ->મુક્ત->જમણું;
        પાછા p;
    }

    જો (વૃક્ષ->size < કદ(ngx_radix_ગાંઠ_t)) {
        વૃક્ષ->start = ngx_pmemalign(વૃક્ષ->pool, ngx_pagesize, ngx_pagesize);
        જો (વૃક્ષ->start == ખાલી) {
            પાછા ખાલી;
        }

        વૃક્ષ->size = ngx_pagesize;
    }

    p = (ngx_radix_ગાંઠ_t *) વૃક્ષ->start;
    વૃક્ષ->start += કદ(ngx_radix_ગાંઠ_t);
    વૃક્ષ->size -= કદ(ngx_radix_ગાંઠ_t);

    પાછા p;
}
