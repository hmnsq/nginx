
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर ngx_radix_गाँठो_t *ngx_radix_alloc(ngx_radix_रूख_t *रूख);


ngx_radix_रूख_t *
ngx_radix_रूख_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           साँचो, mask, inc;
    ngx_radix_रूख_t  *रूख;

    रूख = ngx_palloc(pool, परिमाण(ngx_radix_रूख_t));
    यदि (रूख == रिक्त) {
        फर्काऊ रिक्त;
    }

    रूख->pool = pool;
    रूख->मुक्त = रिक्त;
    रूख->start = रिक्त;
    रूख->size = 0;

    रूख->जरा = ngx_radix_alloc(रूख);
    यदि (रूख->जरा == रिक्त) {
        फर्काऊ रिक्त;
    }

    रूख->जरा->दायाँ = रिक्त;
    रूख->जरा->बायाँ = रिक्त;
    रूख->जरा->parent = रिक्त;
    रूख->जरा->मान = NGX_RADIX_NO_मान;

    यदि (preallocate == 0) {
        फर्काऊ रूख;
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
        विकल्प (ngx_pagesize / परिमाण(ngx_radix_गाँठो_t)) {

        /* amd64 */
        पक्ष 128:
            preallocate = 6;
            रोक;

        /* i386, sparc64 */
        पक्ष 256:
            preallocate = 7;
            रोक;

        /* sparc64 in 32-bit mode */
        स्वभाव:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    जबसम्म (preallocate--) {

        साँचो = 0;
        mask >>= 1;
        mask |= 0x80000000;

        गर {
            यदि (ngx_radix32tree_घुसाउ(रूख, साँचो, mask, NGX_RADIX_NO_मान)
                != NGX_OK)
            {
                फर्काऊ रिक्त;
            }

            साँचो += inc;

        } जबसम्म (साँचो);

        inc >>= 1;
    }

    फर्काऊ रूख;
}


ngx_int_t
ngx_radix32tree_घुसाउ(ngx_radix_रूख_t *रूख, uint32_t साँचो, uint32_t mask,
    uintptr_t मान)
{
    uint32_t           bit;
    ngx_radix_गाँठो_t  *गाँठो, *next;

    bit = 0x80000000;

    गाँठो = रूख->जरा;
    next = रूख->जरा;

    जबसम्म (bit & mask) {
        यदि (साँचो & bit) {
            next = गाँठो->दायाँ;

        } अन्यथा {
            next = गाँठो->बायाँ;
        }

        यदि (next == रिक्त) {
            रोक;
        }

        bit >>= 1;
        गाँठो = next;
    }

    यदि (next) {
        यदि (गाँठो->मान != NGX_RADIX_NO_मान) {
            फर्काऊ NGX_BUSY;
        }

        गाँठो->मान = मान;
        फर्काऊ NGX_OK;
    }

    जबसम्म (bit & mask) {
        next = ngx_radix_alloc(रूख);
        यदि (next == रिक्त) {
            फर्काऊ NGX_ERROR;
        }

        next->दायाँ = रिक्त;
        next->बायाँ = रिक्त;
        next->parent = गाँठो;
        next->मान = NGX_RADIX_NO_मान;

        यदि (साँचो & bit) {
            गाँठो->दायाँ = next;

        } अन्यथा {
            गाँठो->बायाँ = next;
        }

        bit >>= 1;
        गाँठो = next;
    }

    गाँठो->मान = मान;

    फर्काऊ NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_रूख_t *रूख, uint32_t साँचो, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_गाँठो_t  *गाँठो;

    bit = 0x80000000;
    गाँठो = रूख->जरा;

    जबसम्म (गाँठो && (bit & mask)) {
        यदि (साँचो & bit) {
            गाँठो = गाँठो->दायाँ;

        } अन्यथा {
            गाँठो = गाँठो->बायाँ;
        }

        bit >>= 1;
    }

    यदि (गाँठो == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    यदि (गाँठो->दायाँ || गाँठो->बायाँ) {
        यदि (गाँठो->मान != NGX_RADIX_NO_मान) {
            गाँठो->मान = NGX_RADIX_NO_मान;
            फर्काऊ NGX_OK;
        }

        फर्काऊ NGX_ERROR;
    }

    लागि ( ;; ) {
        यदि (गाँठो->parent->दायाँ == गाँठो) {
            गाँठो->parent->दायाँ = रिक्त;

        } अन्यथा {
            गाँठो->parent->बायाँ = रिक्त;
        }

        गाँठो->दायाँ = रूख->मुक्त;
        रूख->मुक्त = गाँठो;

        गाँठो = गाँठो->parent;

        यदि (गाँठो->दायाँ || गाँठो->बायाँ) {
            रोक;
        }

        यदि (गाँठो->मान != NGX_RADIX_NO_मान) {
            रोक;
        }

        यदि (गाँठो->parent == रिक्त) {
            रोक;
        }
    }

    फर्काऊ NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_रूख_t *रूख, uint32_t साँचो)
{
    uint32_t           bit;
    uintptr_t          मान;
    ngx_radix_गाँठो_t  *गाँठो;

    bit = 0x80000000;
    मान = NGX_RADIX_NO_मान;
    गाँठो = रूख->जरा;

    जबसम्म (गाँठो) {
        यदि (गाँठो->मान != NGX_RADIX_NO_मान) {
            मान = गाँठो->मान;
        }

        यदि (साँचो & bit) {
            गाँठो = गाँठो->दायाँ;

        } अन्यथा {
            गाँठो = गाँठो->बायाँ;
        }

        bit >>= 1;
    }

    फर्काऊ मान;
}


#यदि (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_घुसाउ(ngx_radix_रूख_t *रूख, u_char *साँचो, u_char *mask,
    uintptr_t मान)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_गाँठो_t  *गाँठो, *next;

    i = 0;
    bit = 0x80;

    गाँठो = रूख->जरा;
    next = रूख->जरा;

    जबसम्म (bit & mask[i]) {
        यदि (साँचो[i] & bit) {
            next = गाँठो->दायाँ;

        } अन्यथा {
            next = गाँठो->बायाँ;
        }

        यदि (next == रिक्त) {
            रोक;
        }

        bit >>= 1;
        गाँठो = next;

        यदि (bit == 0) {
            यदि (++i == 16) {
                रोक;
            }

            bit = 0x80;
        }
    }

    यदि (next) {
        यदि (गाँठो->मान != NGX_RADIX_NO_मान) {
            फर्काऊ NGX_BUSY;
        }

        गाँठो->मान = मान;
        फर्काऊ NGX_OK;
    }

    जबसम्म (bit & mask[i]) {
        next = ngx_radix_alloc(रूख);
        यदि (next == रिक्त) {
            फर्काऊ NGX_ERROR;
        }

        next->दायाँ = रिक्त;
        next->बायाँ = रिक्त;
        next->parent = गाँठो;
        next->मान = NGX_RADIX_NO_मान;

        यदि (साँचो[i] & bit) {
            गाँठो->दायाँ = next;

        } अन्यथा {
            गाँठो->बायाँ = next;
        }

        bit >>= 1;
        गाँठो = next;

        यदि (bit == 0) {
            यदि (++i == 16) {
                रोक;
            }

            bit = 0x80;
        }
    }

    गाँठो->मान = मान;

    फर्काऊ NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_रूख_t *रूख, u_char *साँचो, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_गाँठो_t  *गाँठो;

    i = 0;
    bit = 0x80;
    गाँठो = रूख->जरा;

    जबसम्म (गाँठो && (bit & mask[i])) {
        यदि (साँचो[i] & bit) {
            गाँठो = गाँठो->दायाँ;

        } अन्यथा {
            गाँठो = गाँठो->बायाँ;
        }

        bit >>= 1;

        यदि (bit == 0) {
            यदि (++i == 16) {
                रोक;
            }

            bit = 0x80;
        }
    }

    यदि (गाँठो == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    यदि (गाँठो->दायाँ || गाँठो->बायाँ) {
        यदि (गाँठो->मान != NGX_RADIX_NO_मान) {
            गाँठो->मान = NGX_RADIX_NO_मान;
            फर्काऊ NGX_OK;
        }

        फर्काऊ NGX_ERROR;
    }

    लागि ( ;; ) {
        यदि (गाँठो->parent->दायाँ == गाँठो) {
            गाँठो->parent->दायाँ = रिक्त;

        } अन्यथा {
            गाँठो->parent->बायाँ = रिक्त;
        }

        गाँठो->दायाँ = रूख->मुक्त;
        रूख->मुक्त = गाँठो;

        गाँठो = गाँठो->parent;

        यदि (गाँठो->दायाँ || गाँठो->बायाँ) {
            रोक;
        }

        यदि (गाँठो->मान != NGX_RADIX_NO_मान) {
            रोक;
        }

        यदि (गाँठो->parent == रिक्त) {
            रोक;
        }
    }

    फर्काऊ NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_रूख_t *रूख, u_char *साँचो)
{
    u_char             bit;
    uintptr_t          मान;
    ngx_uint_t         i;
    ngx_radix_गाँठो_t  *गाँठो;

    i = 0;
    bit = 0x80;
    मान = NGX_RADIX_NO_मान;
    गाँठो = रूख->जरा;

    जबसम्म (गाँठो) {
        यदि (गाँठो->मान != NGX_RADIX_NO_मान) {
            मान = गाँठो->मान;
        }

        यदि (साँचो[i] & bit) {
            गाँठो = गाँठो->दायाँ;

        } अन्यथा {
            गाँठो = गाँठो->बायाँ;
        }

        bit >>= 1;

        यदि (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    फर्काऊ मान;
}

#endif


स्थिर ngx_radix_गाँठो_t *
ngx_radix_alloc(ngx_radix_रूख_t *रूख)
{
    ngx_radix_गाँठो_t  *p;

    यदि (रूख->मुक्त) {
        p = रूख->मुक्त;
        रूख->मुक्त = रूख->मुक्त->दायाँ;
        फर्काऊ p;
    }

    यदि (रूख->size < परिमाण(ngx_radix_गाँठो_t)) {
        रूख->start = ngx_pmemalign(रूख->pool, ngx_pagesize, ngx_pagesize);
        यदि (रूख->start == रिक्त) {
            फर्काऊ रिक्त;
        }

        रूख->size = ngx_pagesize;
    }

    p = (ngx_radix_गाँठो_t *) रूख->start;
    रूख->start += परिमाण(ngx_radix_गाँठो_t);
    रूख->size -= परिमाण(ngx_radix_गाँठो_t);

    फर्काऊ p;
}
