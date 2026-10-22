
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


static ngx_radix_बुहोल_t *ngx_radix_alloc(ngx_radix_पुनो_t *पुनो);


ngx_radix_पुनो_t *
ngx_radix_पुनो_create(ngx_pool_t *pool, ngx_int_t preallocate)
{
    uint32_t           सुसि, mask, inc;
    ngx_radix_पुनो_t  *पुनो;

    पुनो = ngx_palloc(pool, sizeof(ngx_radix_पुनो_t));
    if (पुनो == NULL) {
        return NULL;
    }

    पुनो->pool = pool;
    पुनो->free = NULL;
    पुनो->start = NULL;
    पुनो->size = 0;

    पुनो->उगात = ngx_radix_alloc(पुनो);
    if (पुनो->उगात == NULL) {
        return NULL;
    }

    पुनो->उगात->कानान = NULL;
    पुनो->उगात->कालिवा = NULL;
    पुनो->उगात->parent = NULL;
    पुनो->उगात->हालागा = NGX_RADIX_NO_हालागा;

    if (preallocate == 0) {
        return पुनो;
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

    if (preallocate == -1) {
        switch (ngx_pagesize / sizeof(ngx_radix_बुहोल_t)) {

        /* amd64 */
        case 128:
            preallocate = 6;
            break;

        /* i386, sparc64 */
        case 256:
            preallocate = 7;
            break;

        /* sparc64 in 32-bit mode */
        default:
            preallocate = 8;
        }
    }

    mask = 0;
    inc = 0x80000000;

    while (preallocate--) {

        सुसि = 0;
        mask >>= 1;
        mask |= 0x80000000;

        do {
            if (ngx_radix32tree_इसिंगित(पुनो, सुसि, mask, NGX_RADIX_NO_हालागा)
                != NGX_OK)
            {
                return NULL;
            }

            सुसि += inc;

        } while (सुसि);

        inc >>= 1;
    }

    return पुनो;
}


ngx_int_t
ngx_radix32tree_इसिंगित(ngx_radix_पुनो_t *पुनो, uint32_t सुसि, uint32_t mask,
    uintptr_t हालागा)
{
    uint32_t           bit;
    ngx_radix_बुहोल_t  *बुहोल, *next;

    bit = 0x80000000;

    बुहोल = पुनो->उगात;
    next = पुनो->उगात;

    while (bit & mask) {
        if (सुसि & bit) {
            next = बुहोल->कानान;

        } else {
            next = बुहोल->कालिवा;
        }

        if (next == NULL) {
            break;
        }

        bit >>= 1;
        बुहोल = next;
    }

    if (next) {
        if (बुहोल->हालागा != NGX_RADIX_NO_हालागा) {
            return NGX_BUSY;
        }

        बुहोल->हालागा = हालागा;
        return NGX_OK;
    }

    while (bit & mask) {
        next = ngx_radix_alloc(पुनो);
        if (next == NULL) {
            return NGX_ERROR;
        }

        next->कानान = NULL;
        next->कालिवा = NULL;
        next->parent = बुहोल;
        next->हालागा = NGX_RADIX_NO_हालागा;

        if (सुसि & bit) {
            बुहोल->कानान = next;

        } else {
            बुहोल->कालिवा = next;
        }

        bit >>= 1;
        बुहोल = next;
    }

    बुहोल->हालागा = हालागा;

    return NGX_OK;
}


ngx_int_t
ngx_radix32tree_delete(ngx_radix_पुनो_t *पुनो, uint32_t सुसि, uint32_t mask)
{
    uint32_t           bit;
    ngx_radix_बुहोल_t  *बुहोल;

    bit = 0x80000000;
    बुहोल = पुनो->उगात;

    while (बुहोल && (bit & mask)) {
        if (सुसि & bit) {
            बुहोल = बुहोल->कानान;

        } else {
            बुहोल = बुहोल->कालिवा;
        }

        bit >>= 1;
    }

    if (बुहोल == NULL) {
        return NGX_ERROR;
    }

    if (बुहोल->कानान || बुहोल->कालिवा) {
        if (बुहोल->हालागा != NGX_RADIX_NO_हालागा) {
            बुहोल->हालागा = NGX_RADIX_NO_हालागा;
            return NGX_OK;
        }

        return NGX_ERROR;
    }

    for ( ;; ) {
        if (बुहोल->parent->कानान == बुहोल) {
            बुहोल->parent->कानान = NULL;

        } else {
            बुहोल->parent->कालिवा = NULL;
        }

        बुहोल->कानान = पुनो->free;
        पुनो->free = बुहोल;

        बुहोल = बुहोल->parent;

        if (बुहोल->कानान || बुहोल->कालिवा) {
            break;
        }

        if (बुहोल->हालागा != NGX_RADIX_NO_हालागा) {
            break;
        }

        if (बुहोल->parent == NULL) {
            break;
        }
    }

    return NGX_OK;
}


uintptr_t
ngx_radix32tree_find(ngx_radix_पुनो_t *पुनो, uint32_t सुसि)
{
    uint32_t           bit;
    uintptr_t          हालागा;
    ngx_radix_बुहोल_t  *बुहोल;

    bit = 0x80000000;
    हालागा = NGX_RADIX_NO_हालागा;
    बुहोल = पुनो->उगात;

    while (बुहोल) {
        if (बुहोल->हालागा != NGX_RADIX_NO_हालागा) {
            हालागा = बुहोल->हालागा;
        }

        if (सुसि & bit) {
            बुहोल = बुहोल->कानान;

        } else {
            बुहोल = बुहोल->कालिवा;
        }

        bit >>= 1;
    }

    return हालागा;
}


#if (NGX_HAVE_INET6)

ngx_int_t
ngx_radix128tree_इसिंगित(ngx_radix_पुनो_t *पुनो, u_char *सुसि, u_char *mask,
    uintptr_t हालागा)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_बुहोल_t  *बुहोल, *next;

    i = 0;
    bit = 0x80;

    बुहोल = पुनो->उगात;
    next = पुनो->उगात;

    while (bit & mask[i]) {
        if (सुसि[i] & bit) {
            next = बुहोल->कानान;

        } else {
            next = बुहोल->कालिवा;
        }

        if (next == NULL) {
            break;
        }

        bit >>= 1;
        बुहोल = next;

        if (bit == 0) {
            if (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    if (next) {
        if (बुहोल->हालागा != NGX_RADIX_NO_हालागा) {
            return NGX_BUSY;
        }

        बुहोल->हालागा = हालागा;
        return NGX_OK;
    }

    while (bit & mask[i]) {
        next = ngx_radix_alloc(पुनो);
        if (next == NULL) {
            return NGX_ERROR;
        }

        next->कानान = NULL;
        next->कालिवा = NULL;
        next->parent = बुहोल;
        next->हालागा = NGX_RADIX_NO_हालागा;

        if (सुसि[i] & bit) {
            बुहोल->कानान = next;

        } else {
            बुहोल->कालिवा = next;
        }

        bit >>= 1;
        बुहोल = next;

        if (bit == 0) {
            if (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    बुहोल->हालागा = हालागा;

    return NGX_OK;
}


ngx_int_t
ngx_radix128tree_delete(ngx_radix_पुनो_t *पुनो, u_char *सुसि, u_char *mask)
{
    u_char             bit;
    ngx_uint_t         i;
    ngx_radix_बुहोल_t  *बुहोल;

    i = 0;
    bit = 0x80;
    बुहोल = पुनो->उगात;

    while (बुहोल && (bit & mask[i])) {
        if (सुसि[i] & bit) {
            बुहोल = बुहोल->कानान;

        } else {
            बुहोल = बुहोल->कालिवा;
        }

        bit >>= 1;

        if (bit == 0) {
            if (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    if (बुहोल == NULL) {
        return NGX_ERROR;
    }

    if (बुहोल->कानान || बुहोल->कालिवा) {
        if (बुहोल->हालागा != NGX_RADIX_NO_हालागा) {
            बुहोल->हालागा = NGX_RADIX_NO_हालागा;
            return NGX_OK;
        }

        return NGX_ERROR;
    }

    for ( ;; ) {
        if (बुहोल->parent->कानान == बुहोल) {
            बुहोल->parent->कानान = NULL;

        } else {
            बुहोल->parent->कालिवा = NULL;
        }

        बुहोल->कानान = पुनो->free;
        पुनो->free = बुहोल;

        बुहोल = बुहोल->parent;

        if (बुहोल->कानान || बुहोल->कालिवा) {
            break;
        }

        if (बुहोल->हालागा != NGX_RADIX_NO_हालागा) {
            break;
        }

        if (बुहोल->parent == NULL) {
            break;
        }
    }

    return NGX_OK;
}


uintptr_t
ngx_radix128tree_find(ngx_radix_पुनो_t *पुनो, u_char *सुसि)
{
    u_char             bit;
    uintptr_t          हालागा;
    ngx_uint_t         i;
    ngx_radix_बुहोल_t  *बुहोल;

    i = 0;
    bit = 0x80;
    हालागा = NGX_RADIX_NO_हालागा;
    बुहोल = पुनो->उगात;

    while (बुहोल) {
        if (बुहोल->हालागा != NGX_RADIX_NO_हालागा) {
            हालागा = बुहोल->हालागा;
        }

        if (सुसि[i] & bit) {
            बुहोल = बुहोल->कानान;

        } else {
            बुहोल = बुहोल->कालिवा;
        }

        bit >>= 1;

        if (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    return हालागा;
}

#endif


static ngx_radix_बुहोल_t *
ngx_radix_alloc(ngx_radix_पुनो_t *पुनो)
{
    ngx_radix_बुहोल_t  *p;

    if (पुनो->free) {
        p = पुनो->free;
        पुनो->free = पुनो->free->कानान;
        return p;
    }

    if (पुनो->size < sizeof(ngx_radix_बुहोल_t)) {
        पुनो->start = ngx_pmemalign(पुनो->pool, ngx_pagesize, ngx_pagesize);
        if (पुनो->start == NULL) {
            return NULL;
        }

        पुनो->size = ngx_pagesize;
    }

    p = (ngx_radix_बुहोल_t *) पुनो->start;
    पुनो->start += sizeof(ngx_radix_बुहोल_t);
    पुनो->size -= sizeof(ngx_radix_बुहोल_t);

    return p;
}
