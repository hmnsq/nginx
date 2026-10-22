
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ ngx_radix_ಗಂಟು_t *ngx_radix_alloc(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ);


ngx_radix_ವೃಕ್ಷ_t *
ngx_radix_ವೃಕ್ಷ_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           ಕೀಲಿ, mask, inc;
    ngx_radix_ವೃಕ್ಷ_t  *ವೃಕ್ಷ;

    ವೃಕ್ಷ = ngx_palloc(pool, ಪರಿಮಾಣ(ngx_radix_ವೃಕ್ಷ_t));
    ಯದಿ (ವೃಕ್ಷ == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ವೃಕ್ಷ->pool = pool;
    ವೃಕ್ಷ->ಮುಕ್ತ = ರಿಕ್ತ;
    ವೃಕ್ಷ->start = ರಿಕ್ತ;
    ವೃಕ್ಷ->size = 0;

    ವೃಕ್ಷ->ಬೇರು = ngx_radix_alloc(ವೃಕ್ಷ);
    ಯದಿ (ವೃಕ್ಷ->ಬೇರು == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ವೃಕ್ಷ->ಬೇರು->ಬಲ = ರಿಕ್ತ;
    ವೃಕ್ಷ->ಬೇರು->ಎಡ = ರಿಕ್ತ;
    ವೃಕ್ಷ->ಬೇರು->parent = ರಿಕ್ತ;
    ವೃಕ್ಷ->ಬೇರು->ಮೌಲ್ಯ = NGX_RADIX_NO_ಮೌಲ್ಯ;

    ಯದಿ (preallocate == 0) {
        ಪ್ರತ್ಯಾಗಮ ವೃಕ್ಷ;
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

    ಯದಿ (preallocate == -1) {
        ವಿಕಲ್ಪ (ngx_pagesize / ಪರಿಮಾಣ(ngx_radix_ಗಂಟು_t)) {

        /* amd64 */
        ಪಕ್ಷ 128:
            preallocate = 6;
            ಭಙ್ಗ;

        /* i386, sparc64 */
        ಪಕ್ಷ 256:
            preallocate = 7;
            ಭಙ್ಗ;

        /* sparc64 in 32-bit mode */
        ಸ್ವಭಾವ:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    ಯಾವತ್ (preallocate--) {

        ಕೀಲಿ = 0;
        mask >>= 1;
        mask |= 0x80000000;

        ಕುರು {
            ಯದಿ (ngx_radix32tree_ಸೇರಿಸು(ವೃಕ್ಷ, ಕೀಲಿ, mask, NGX_RADIX_NO_ಮೌಲ್ಯ)
                != NGX_OK)
            {
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }

            ಕೀಲಿ += inc;

        } ಯಾವತ್ (ಕೀಲಿ);

        inc >>= 1;
    }

    ಪ್ರತ್ಯಾಗಮ ವೃಕ್ಷ;
}


ngx_int_t
ngx_radix32tree_ಸೇರಿಸು(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ, uint32_t ಕೀಲಿ, uint32_t mask,
    uintptr_t ಮೌಲ್ಯ)
{
    uint32_t           bit;
    ngx_radix_ಗಂಟು_t  *ಗಂಟು, *next;

    bit = 0x80000000;

    ಗಂಟು = ವೃಕ್ಷ->ಬೇರು;
    next = ವೃಕ್ಷ->ಬೇರು;

    ಯಾವತ್ (bit & mask) {
        ಯದಿ (ಕೀಲಿ & bit) {
            next = ಗಂಟು->ಬಲ;

        } ಅನ್ಯಥಾ {
            next = ಗಂಟು->ಎಡ;
        }

        ಯದಿ (next == ರಿಕ್ತ) {
            ಭಙ್ಗ;
        }

        bit >>= 1;
        ಗಂಟು = next;
    }

    ಯದಿ (next) {
        ಯದಿ (ಗಂಟು->ಮೌಲ್ಯ != NGX_RADIX_NO_ಮೌಲ್ಯ) {
            ಪ್ರತ್ಯಾಗಮ NGX_BUSY;
        }

        ಗಂಟು->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಯಾವತ್ (bit & mask) {
        next = ngx_radix_alloc(ವೃಕ್ಷ);
        ಯದಿ (next == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        next->ಬಲ = ರಿಕ್ತ;
        next->ಎಡ = ರಿಕ್ತ;
        next->parent = ಗಂಟು;
        next->ಮೌಲ್ಯ = NGX_RADIX_NO_ಮೌಲ್ಯ;

        ಯದಿ (ಕೀಲಿ & bit) {
            ಗಂಟು->ಬಲ = next;

        } ಅನ್ಯಥಾ {
            ಗಂಟು->ಎಡ = next;
        }

        bit >>= 1;
        ಗಂಟು = next;
    }

    ಗಂಟು->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ, uint32_t ಕೀಲಿ, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_ಗಂಟು_t  *ಗಂಟು;

    bit = 0x80000000;
    ಗಂಟು = ವೃಕ್ಷ->ಬೇರು;

    ಯಾವತ್ (ಗಂಟು && (bit & mask)) {
        ಯದಿ (ಕೀಲಿ & bit) {
            ಗಂಟು = ಗಂಟು->ಬಲ;

        } ಅನ್ಯಥಾ {
            ಗಂಟು = ಗಂಟು->ಎಡ;
        }

        bit >>= 1;
    }

    ಯದಿ (ಗಂಟು == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ಗಂಟು->ಬಲ || ಗಂಟು->ಎಡ) {
        ಯದಿ (ಗಂಟು->ಮೌಲ್ಯ != NGX_RADIX_NO_ಮೌಲ್ಯ) {
            ಗಂಟು->ಮೌಲ್ಯ = NGX_RADIX_NO_ಮೌಲ್ಯ;
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಪ್ರತಿ ( ;; ) {
        ಯದಿ (ಗಂಟು->parent->ಬಲ == ಗಂಟು) {
            ಗಂಟು->parent->ಬಲ = ರಿಕ್ತ;

        } ಅನ್ಯಥಾ {
            ಗಂಟು->parent->ಎಡ = ರಿಕ್ತ;
        }

        ಗಂಟು->ಬಲ = ವೃಕ್ಷ->ಮುಕ್ತ;
        ವೃಕ್ಷ->ಮುಕ್ತ = ಗಂಟು;

        ಗಂಟು = ಗಂಟು->parent;

        ಯದಿ (ಗಂಟು->ಬಲ || ಗಂಟು->ಎಡ) {
            ಭಙ್ಗ;
        }

        ಯದಿ (ಗಂಟು->ಮೌಲ್ಯ != NGX_RADIX_NO_ಮೌಲ್ಯ) {
            ಭಙ್ಗ;
        }

        ಯದಿ (ಗಂಟು->parent == ರಿಕ್ತ) {
            ಭಙ್ಗ;
        }
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ, uint32_t ಕೀಲಿ)
{
    uint32_t           bit;
    uintptr_t          ಮೌಲ್ಯ;
    ngx_radix_ಗಂಟು_t  *ಗಂಟು;

    bit = 0x80000000;
    ಮೌಲ್ಯ = NGX_RADIX_NO_ಮೌಲ್ಯ;
    ಗಂಟು = ವೃಕ್ಷ->ಬೇರು;

    ಯಾವತ್ (ಗಂಟು) {
        ಯದಿ (ಗಂಟು->ಮೌಲ್ಯ != NGX_RADIX_NO_ಮೌಲ್ಯ) {
            ಮೌಲ್ಯ = ಗಂಟು->ಮೌಲ್ಯ;
        }

        ಯದಿ (ಕೀಲಿ & bit) {
            ಗಂಟು = ಗಂಟು->ಬಲ;

        } ಅನ್ಯಥಾ {
            ಗಂಟು = ಗಂಟು->ಎಡ;
        }

        bit >>= 1;
    }

    ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
}


#ಯದಿ (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_ಸೇರಿಸು(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ, u_char *ಕೀಲಿ, u_char *mask,
    uintptr_t ಮೌಲ್ಯ)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_ಗಂಟು_t  *ಗಂಟು, *next;

    i = 0;
    bit = 0x80;

    ಗಂಟು = ವೃಕ್ಷ->ಬೇರು;
    next = ವೃಕ್ಷ->ಬೇರು;

    ಯಾವತ್ (bit & mask[i]) {
        ಯದಿ (ಕೀಲಿ[i] & bit) {
            next = ಗಂಟು->ಬಲ;

        } ಅನ್ಯಥಾ {
            next = ಗಂಟು->ಎಡ;
        }

        ಯದಿ (next == ರಿಕ್ತ) {
            ಭಙ್ಗ;
        }

        bit >>= 1;
        ಗಂಟು = next;

        ಯದಿ (bit == 0) {
            ಯದಿ (++i == 16) {
                ಭಙ್ಗ;
            }

            bit = 0x80;
        }
    }

    ಯದಿ (next) {
        ಯದಿ (ಗಂಟು->ಮೌಲ್ಯ != NGX_RADIX_NO_ಮೌಲ್ಯ) {
            ಪ್ರತ್ಯಾಗಮ NGX_BUSY;
        }

        ಗಂಟು->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಯಾವತ್ (bit & mask[i]) {
        next = ngx_radix_alloc(ವೃಕ್ಷ);
        ಯದಿ (next == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        next->ಬಲ = ರಿಕ್ತ;
        next->ಎಡ = ರಿಕ್ತ;
        next->parent = ಗಂಟು;
        next->ಮೌಲ್ಯ = NGX_RADIX_NO_ಮೌಲ್ಯ;

        ಯದಿ (ಕೀಲಿ[i] & bit) {
            ಗಂಟು->ಬಲ = next;

        } ಅನ್ಯಥಾ {
            ಗಂಟು->ಎಡ = next;
        }

        bit >>= 1;
        ಗಂಟು = next;

        ಯದಿ (bit == 0) {
            ಯದಿ (++i == 16) {
                ಭಙ್ಗ;
            }

            bit = 0x80;
        }
    }

    ಗಂಟು->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ, u_char *ಕೀಲಿ, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_ಗಂಟು_t  *ಗಂಟು;

    i = 0;
    bit = 0x80;
    ಗಂಟು = ವೃಕ್ಷ->ಬೇರು;

    ಯಾವತ್ (ಗಂಟು && (bit & mask[i])) {
        ಯದಿ (ಕೀಲಿ[i] & bit) {
            ಗಂಟು = ಗಂಟು->ಬಲ;

        } ಅನ್ಯಥಾ {
            ಗಂಟು = ಗಂಟು->ಎಡ;
        }

        bit >>= 1;

        ಯದಿ (bit == 0) {
            ಯದಿ (++i == 16) {
                ಭಙ್ಗ;
            }

            bit = 0x80;
        }
    }

    ಯದಿ (ಗಂಟು == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ಗಂಟು->ಬಲ || ಗಂಟು->ಎಡ) {
        ಯದಿ (ಗಂಟು->ಮೌಲ್ಯ != NGX_RADIX_NO_ಮೌಲ್ಯ) {
            ಗಂಟು->ಮೌಲ್ಯ = NGX_RADIX_NO_ಮೌಲ್ಯ;
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಪ್ರತಿ ( ;; ) {
        ಯದಿ (ಗಂಟು->parent->ಬಲ == ಗಂಟು) {
            ಗಂಟು->parent->ಬಲ = ರಿಕ್ತ;

        } ಅನ್ಯಥಾ {
            ಗಂಟು->parent->ಎಡ = ರಿಕ್ತ;
        }

        ಗಂಟು->ಬಲ = ವೃಕ್ಷ->ಮುಕ್ತ;
        ವೃಕ್ಷ->ಮುಕ್ತ = ಗಂಟು;

        ಗಂಟು = ಗಂಟು->parent;

        ಯದಿ (ಗಂಟು->ಬಲ || ಗಂಟು->ಎಡ) {
            ಭಙ್ಗ;
        }

        ಯದಿ (ಗಂಟು->ಮೌಲ್ಯ != NGX_RADIX_NO_ಮೌಲ್ಯ) {
            ಭಙ್ಗ;
        }

        ಯದಿ (ಗಂಟು->parent == ರಿಕ್ತ) {
            ಭಙ್ಗ;
        }
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ, u_char *ಕೀಲಿ)
{
    u_char             bit;
    uintptr_t          ಮೌಲ್ಯ;
    ngx_uint_t         i;
    ngx_radix_ಗಂಟು_t  *ಗಂಟು;

    i = 0;
    bit = 0x80;
    ಮೌಲ್ಯ = NGX_RADIX_NO_ಮೌಲ್ಯ;
    ಗಂಟು = ವೃಕ್ಷ->ಬೇರು;

    ಯಾವತ್ (ಗಂಟು) {
        ಯದಿ (ಗಂಟು->ಮೌಲ್ಯ != NGX_RADIX_NO_ಮೌಲ್ಯ) {
            ಮೌಲ್ಯ = ಗಂಟು->ಮೌಲ್ಯ;
        }

        ಯದಿ (ಕೀಲಿ[i] & bit) {
            ಗಂಟು = ಗಂಟು->ಬಲ;

        } ಅನ್ಯಥಾ {
            ಗಂಟು = ಗಂಟು->ಎಡ;
        }

        bit >>= 1;

        ಯದಿ (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
}

#endif


ಸ್ಥಿರ ngx_radix_ಗಂಟು_t *
ngx_radix_alloc(ngx_radix_ವೃಕ್ಷ_t *ವೃಕ್ಷ)
{
    ngx_radix_ಗಂಟು_t  *p;

    ಯದಿ (ವೃಕ್ಷ->ಮುಕ್ತ) {
        p = ವೃಕ್ಷ->ಮುಕ್ತ;
        ವೃಕ್ಷ->ಮುಕ್ತ = ವೃಕ್ಷ->ಮುಕ್ತ->ಬಲ;
        ಪ್ರತ್ಯಾಗಮ p;
    }

    ಯದಿ (ವೃಕ್ಷ->size < ಪರಿಮಾಣ(ngx_radix_ಗಂಟು_t)) {
        ವೃಕ್ಷ->start = ngx_pmemalign(ವೃಕ್ಷ->pool, ngx_pagesize, ngx_pagesize);
        ಯದಿ (ವೃಕ್ಷ->start == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        ವೃಕ್ಷ->size = ngx_pagesize;
    }

    p = (ngx_radix_ಗಂಟು_t *) ವೃಕ್ಷ->start;
    ವೃಕ್ಷ->start += ಪರಿಮಾಣ(ngx_radix_ಗಂಟು_t);
    ವೃಕ್ಷ->size -= ಪರಿಮಾಣ(ngx_radix_ಗಂಟು_t);

    ಪ್ರತ್ಯಾಗಮ p;
}
