
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#define NGX_SLAB_PAGE_MASK   3
#define NGX_SLAB_PAGE        0
#define NGX_SLAB_BIG         1
#define NGX_SLAB_EXACT       2
#define NGX_SLAB_SMALL       3

#ಯದಿ (NGX_PTR_SIZE == 4)

#define NGX_SLAB_PAGE_FREE   0
#define NGX_SLAB_PAGE_BUSY   0xffffffff
#define NGX_SLAB_PAGE_START  0x80000000

#define NGX_SLAB_SHIFT_MASK  0x0000000f
#define NGX_SLAB_MAP_MASK    0xffff0000
#define NGX_SLAB_MAP_SHIFT   16

#define NGX_SLAB_BUSY        0xffffffff

#ಅನ್ಯಥಾ /* (NGX_PTR_SIZE == 8) */

#define NGX_SLAB_PAGE_FREE   0
#define NGX_SLAB_PAGE_BUSY   0xffffffffffffffff
#define NGX_SLAB_PAGE_START  0x8000000000000000

#define NGX_SLAB_SHIFT_MASK  0x000000000000000f
#define NGX_SLAB_MAP_MASK    0xffffffff00000000
#define NGX_SLAB_MAP_SHIFT   32

#define NGX_SLAB_BUSY        0xffffffffffffffff

#endif


#define ngx_slab_slots(pool)                                                  \
    (ngx_slab_page_t *) ((u_char *) (pool) + ಪರಿಮಾಣ(ngx_slab_pool_t))

#define ngx_slab_page_type(page)   ((page)->prev & NGX_SLAB_PAGE_MASK)

#define ngx_slab_page_prev(page)                                              \
    (ngx_slab_page_t *) ((page)->prev & ~NGX_SLAB_PAGE_MASK)

#define ngx_slab_page_addr(pool, page)                                        \
    ((((page) - (pool)->pages) << ngx_pagesize_shift)                         \
     + (uintptr_t) (pool)->start)


#ಯದಿ (NGX_DEBUG_MALLOC)

#define ngx_slab_junk(p, size)     ngx_memset(p, 0xA5, size)

#ನೋಚೇದ್ಯದಿ (NGX_HAVE_DEBUG_MALLOC)

#define ngx_slab_junk(p, size)                                                \
    ಯದಿ (ngx_debug_malloc)          ngx_memset(p, 0xA5, size)

#ಅನ್ಯಥಾ

#define ngx_slab_junk(p, size)

#endif

ಸ್ಥಿರ ngx_slab_page_t *ngx_slab_alloc_pages(ngx_slab_pool_t *pool,
    ngx_uint_t pages);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_slab_free_pages(ngx_slab_pool_t *pool, ngx_slab_page_t *page,
    ngx_uint_t pages);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    ವರ್ಣ *text);


ಸ್ಥಿರ ngx_uint_t  ngx_slab_max_size;
ಸ್ಥಿರ ngx_uint_t  ngx_slab_exact_size;
ಸ್ಥಿರ ngx_uint_t  ngx_slab_exact_shift;


ಶೂನ್ಯ
ngx_slab_sizes_init(ಶೂನ್ಯ)
{
    ngx_uint_t  n;

    ngx_slab_max_size = ngx_pagesize / 2;
    ngx_slab_exact_size = ngx_pagesize / (8 * ಪರಿಮಾಣ(uintptr_t));
    ಪ್ರತಿ (n = ngx_slab_exact_size; n >>= 1; ngx_slab_exact_shift++) {
        /* void */
    }
}


ಶೂನ್ಯ
ngx_slab_init(ngx_slab_pool_t *pool)
{
    u_char           *p;
    size_t            size;
    ngx_int_t         m;
    ngx_uint_t        i, n, pages;
    ngx_slab_page_t  *slots, *page;

    pool->min_size = (size_t) 1 << pool->min_shift;

    slots = ngx_slab_slots(pool);

    p = (u_char *) slots;
    size = pool->end - p;

    ngx_slab_junk(p, size);

    n = ngx_pagesize_shift - pool->min_shift;

    ಪ್ರತಿ (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].next = &slots[i];
        slots[i].prev = 0;
    }

    p += n * ಪರಿಮಾಣ(ngx_slab_page_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * ಪರಿಮಾಣ(ngx_slab_stat_t));

    p += n * ಪರಿಮಾಣ(ngx_slab_stat_t);

    size -= n * (ಪರಿಮಾಣ(ngx_slab_page_t) + ಪರಿಮಾಣ(ngx_slab_stat_t));

    pages = (ngx_uint_t) (size / (ngx_pagesize + ಪರಿಮಾಣ(ngx_slab_page_t)));

    pool->pages = (ngx_slab_page_t *) p;
    ngx_memzero(pool->pages, pages * ಪರಿಮಾಣ(ngx_slab_page_t));

    page = pool->pages;

    /* only "next" is used in list head */
    pool->ಮುಕ್ತ.slab = 0;
    pool->ಮುಕ್ತ.next = page;
    pool->ಮುಕ್ತ.prev = 0;

    page->slab = pages;
    page->next = &pool->ಮುಕ್ತ;
    page->prev = (uintptr_t) &pool->ಮುಕ್ತ;

    pool->start = ngx_align_ptr(p + pages * ಪರಿಮಾಣ(ngx_slab_page_t),
                                ngx_pagesize);

    m = pages - (pool->end - pool->start) / ngx_pagesize;
    ಯದಿ (m > 0) {
        pages -= m;
        page->slab = pages;
    }

    pool->last = pool->pages + pages;
    pool->pfree = pages;

    pool->log_nomem = 1;
    pool->log_ctx = &pool->zero;
    pool->zero = '\0';
}


ಶೂನ್ಯ *
ngx_slab_alloc(ngx_slab_pool_t *pool, size_t size)
{
    ಶೂನ್ಯ  *p;

    ngx_shmtx_lock(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, size);

    ngx_shmtx_unlock(&pool->mutex);

    ಪ್ರತ್ಯಾಗಮ p;
}


ಶೂನ್ಯ *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, size_t size)
{
    size_t            s;
    uintptr_t         p, m, mask, *bitmap;
    ngx_uint_t        i, n, slot, shift, map;
    ngx_slab_page_t  *page, *prev, *slots;

    ಯದಿ (size > ngx_slab_max_size) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                       "slab alloc: %uz", size);

        page = ngx_slab_alloc_pages(pool, (size >> ngx_pagesize_shift)
                                          + ((size % ngx_pagesize) ? 1 : 0));
        ಯದಿ (page) {
            p = ngx_slab_page_addr(pool, page);

        } ಅನ್ಯಥಾ {
            p = 0;
        }

        ಗಚ್ಛ done;
    }

    ಯದಿ (size > pool->min_size) {
        shift = 1;
        ಪ್ರತಿ (s = size - 1; s >>= 1; shift++) { /* void */ }
        slot = shift - pool->min_shift;

    } ಅನ್ಯಥಾ {
        shift = pool->min_shift;
        slot = 0;
    }

    pool->stats[slot].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                   "slab alloc: %uz slot: %ui", size, slot);

    slots = ngx_slab_slots(pool);
    page = slots[slot].next;

    ಯದಿ (page->next != page) {

        ಯದಿ (shift < ngx_slab_exact_shift) {

            bitmap = (uintptr_t *) ngx_slab_page_addr(pool, page);

            map = (ngx_pagesize >> shift) / (8 * ಪರಿಮಾಣ(uintptr_t));

            ಪ್ರತಿ (n = 0; n < map; n++) {

                ಯದಿ (bitmap[n] != NGX_SLAB_BUSY) {

                    ಪ್ರತಿ (m = 1, i = 0; m; m <<= 1, i++) {
                        ಯದಿ (bitmap[n] & m) {
                            ಅನುವರ್ತ;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * ಪರಿಮಾಣ(uintptr_t) + i) << shift;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[slot].used++;

                        ಯದಿ (bitmap[n] == NGX_SLAB_BUSY) {
                            ಪ್ರತಿ (n = n + 1; n < map; n++) {
                                ಯದಿ (bitmap[n] != NGX_SLAB_BUSY) {
                                    ಗಚ್ಛ done;
                                }
                            }

                            prev = ngx_slab_page_prev(page);
                            prev->next = page->next;
                            page->next->prev = page->prev;

                            page->next = ರಿಕ್ತ;
                            page->prev = NGX_SLAB_SMALL;
                        }

                        ಗಚ್ಛ done;
                    }
                }
            }

        } ಅನ್ಯಥಾ ಯದಿ (shift == ngx_slab_exact_shift) {

            ಪ್ರತಿ (m = 1, i = 0; m; m <<= 1, i++) {
                ಯದಿ (page->slab & m) {
                    ಅನುವರ್ತ;
                }

                page->slab |= m;

                ಯದಿ (page->slab == NGX_SLAB_BUSY) {
                    prev = ngx_slab_page_prev(page);
                    prev->next = page->next;
                    page->next->prev = page->prev;

                    page->next = ರಿಕ್ತ;
                    page->prev = NGX_SLAB_EXACT;
                }

                p = ngx_slab_page_addr(pool, page) + (i << shift);

                pool->stats[slot].used++;

                ಗಚ್ಛ done;
            }

        } ಅನ್ಯಥಾ { /* shift > ngx_slab_exact_shift */

            mask = ((uintptr_t) 1 << (ngx_pagesize >> shift)) - 1;
            mask <<= NGX_SLAB_MAP_SHIFT;

            ಪ್ರತಿ (m = (uintptr_t) 1 << NGX_SLAB_MAP_SHIFT, i = 0;
                 m & mask;
                 m <<= 1, i++)
            {
                ಯದಿ (page->slab & m) {
                    ಅನುವರ್ತ;
                }

                page->slab |= m;

                ಯದಿ ((page->slab & NGX_SLAB_MAP_MASK) == mask) {
                    prev = ngx_slab_page_prev(page);
                    prev->next = page->next;
                    page->next->prev = page->prev;

                    page->next = ರಿಕ್ತ;
                    page->prev = NGX_SLAB_BIG;
                }

                p = ngx_slab_page_addr(pool, page) + (i << shift);

                pool->stats[slot].used++;

                ಗಚ್ಛ done;
            }
        }

        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_alloc(): page is busy");
        ngx_debug_point();
    }

    page = ngx_slab_alloc_pages(pool, 1);

    ಯದಿ (page) {
        ಯದಿ (shift < ngx_slab_exact_shift) {
            bitmap = (uintptr_t *) ngx_slab_page_addr(pool, page);

            n = (ngx_pagesize >> shift) / ((1 << shift) * 8);

            ಯದಿ (n == 0) {
                n = 1;
            }

            /* "n" elements for bitmap, plus one requested */

            ಪ್ರತಿ (i = 0; i < (n + 1) / (8 * ಪರಿಮಾಣ(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * ಪರಿಮಾಣ(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> shift) / (8 * ಪರಿಮಾಣ(uintptr_t));

            ಪ್ರತಿ (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            page->slab = shift;
            page->next = &slots[slot];
            page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_SMALL;

            slots[slot].next = page;

            pool->stats[slot].total += (ngx_pagesize >> shift) - n;

            p = ngx_slab_page_addr(pool, page) + (n << shift);

            pool->stats[slot].used++;

            ಗಚ್ಛ done;

        } ಅನ್ಯಥಾ ಯದಿ (shift == ngx_slab_exact_shift) {

            page->slab = 1;
            page->next = &slots[slot];
            page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_EXACT;

            slots[slot].next = page;

            pool->stats[slot].total += 8 * ಪರಿಮಾಣ(uintptr_t);

            p = ngx_slab_page_addr(pool, page);

            pool->stats[slot].used++;

            ಗಚ್ಛ done;

        } ಅನ್ಯಥಾ { /* shift > ngx_slab_exact_shift */

            page->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_SHIFT) | shift;
            page->next = &slots[slot];
            page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_BIG;

            slots[slot].next = page;

            pool->stats[slot].total += ngx_pagesize >> shift;

            p = ngx_slab_page_addr(pool, page);

            pool->stats[slot].used++;

            ಗಚ್ಛ done;
        }
    }

    p = 0;

    pool->stats[slot].fails++;

done:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                   "slab alloc: %p", (ಶೂನ್ಯ *) p);

    ಪ್ರತ್ಯಾಗಮ (ಶೂನ್ಯ *) p;
}


ಶೂನ್ಯ *
ngx_slab_calloc(ngx_slab_pool_t *pool, size_t size)
{
    ಶೂನ್ಯ  *p;

    ngx_shmtx_lock(&pool->mutex);

    p = ngx_slab_calloc_locked(pool, size);

    ngx_shmtx_unlock(&pool->mutex);

    ಪ್ರತ್ಯಾಗಮ p;
}


ಶೂನ್ಯ *
ngx_slab_calloc_locked(ngx_slab_pool_t *pool, size_t size)
{
    ಶೂನ್ಯ  *p;

    p = ngx_slab_alloc_locked(pool, size);
    ಯದಿ (p) {
        ngx_memzero(p, size);
    }

    ಪ್ರತ್ಯಾಗಮ p;
}


ಶೂನ್ಯ
ngx_slab_free(ngx_slab_pool_t *pool, ಶೂನ್ಯ *p)
{
    ngx_shmtx_lock(&pool->mutex);

    ngx_slab_free_locked(pool, p);

    ngx_shmtx_unlock(&pool->mutex);
}


ಶೂನ್ಯ
ngx_slab_free_locked(ngx_slab_pool_t *pool, ಶೂನ್ಯ *p)
{
    size_t            size;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, slot, shift, map;
    ngx_slab_page_t  *slots, *page;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0, "slab free: %p", p);

    ಯದಿ ((u_char *) p < pool->start || (u_char *) p > pool->end) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        ಗಚ್ಛ fail;
    }

    n = ((u_char *) p - pool->start) >> ngx_pagesize_shift;
    page = &pool->pages[n];
    slab = page->slab;
    type = ngx_slab_page_type(page);

    ವಿಕಲ್ಪ (type) {

    ಪಕ್ಷ NGX_SLAB_SMALL:

        shift = slab & NGX_SLAB_SHIFT_MASK;
        size = (size_t) 1 << shift;

        ಯದಿ ((uintptr_t) p & (size - 1)) {
            ಗಚ್ಛ wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> shift;
        m = (uintptr_t) 1 << (n % (8 * ಪರಿಮಾಣ(uintptr_t)));
        n /= 8 * ಪರಿಮಾಣ(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        ಯದಿ (bitmap[n] & m) {
            slot = shift - pool->min_shift;

            ಯದಿ (page->next == ರಿಕ್ತ) {
                slots = ngx_slab_slots(pool);

                page->next = slots[slot].next;
                slots[slot].next = page;

                page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_SMALL;
                page->next->prev = (uintptr_t) page | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> shift) / ((1 << shift) * 8);

            ಯದಿ (n == 0) {
                n = 1;
            }

            i = n / (8 * ಪರಿಮಾಣ(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * ಪರಿಮಾಣ(uintptr_t)))) - 1;

            ಯದಿ (bitmap[i] & ~m) {
                ಗಚ್ಛ done;
            }

            map = (ngx_pagesize >> shift) / (8 * ಪರಿಮಾಣ(uintptr_t));

            ಪ್ರತಿ (i = i + 1; i < map; i++) {
                ಯದಿ (bitmap[i]) {
                    ಗಚ್ಛ done;
                }
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].total -= (ngx_pagesize >> shift) - n;

            ಗಚ್ಛ done;
        }

        ಗಚ್ಛ chunk_already_free;

    ಪಕ್ಷ NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_shift);
        size = ngx_slab_exact_size;

        ಯದಿ ((uintptr_t) p & (size - 1)) {
            ಗಚ್ಛ wrong_chunk;
        }

        ಯದಿ (slab & m) {
            slot = ngx_slab_exact_shift - pool->min_shift;

            ಯದಿ (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                page->next = slots[slot].next;
                slots[slot].next = page;

                page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_EXACT;
                page->next->prev = (uintptr_t) page | NGX_SLAB_EXACT;
            }

            page->slab &= ~m;

            ಯದಿ (page->slab) {
                ಗಚ್ಛ done;
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].total -= 8 * ಪರಿಮಾಣ(uintptr_t);

            ಗಚ್ಛ done;
        }

        ಗಚ್ಛ chunk_already_free;

    ಪಕ್ಷ NGX_SLAB_BIG:

        shift = slab & NGX_SLAB_SHIFT_MASK;
        size = (size_t) 1 << shift;

        ಯದಿ ((uintptr_t) p & (size - 1)) {
            ಗಚ್ಛ wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> shift)
                              + NGX_SLAB_MAP_SHIFT);

        ಯದಿ (slab & m) {
            slot = shift - pool->min_shift;

            ಯದಿ (page->next == ರಿಕ್ತ) {
                slots = ngx_slab_slots(pool);

                page->next = slots[slot].next;
                slots[slot].next = page;

                page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_BIG;
                page->next->prev = (uintptr_t) page | NGX_SLAB_BIG;
            }

            page->slab &= ~m;

            ಯದಿ (page->slab & NGX_SLAB_MAP_MASK) {
                ಗಚ್ಛ done;
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].total -= ngx_pagesize >> shift;

            ಗಚ್ಛ done;
        }

        ಗಚ್ಛ chunk_already_free;

    ಪಕ್ಷ NGX_SLAB_PAGE:

        ಯದಿ ((uintptr_t) p & (ngx_pagesize - 1)) {
            ಗಚ್ಛ wrong_chunk;
        }

        ಯದಿ (!(slab & NGX_SLAB_PAGE_START)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            ಗಚ್ಛ fail;
        }

        ಯದಿ (slab == NGX_SLAB_PAGE_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            ಗಚ್ಛ fail;
        }

        size = slab & ~NGX_SLAB_PAGE_START;

        ngx_slab_free_pages(pool, page, size);

        ngx_slab_junk(p, size << ngx_pagesize_shift);

        ಪ್ರತ್ಯಾಗಮ;
    }

    /* not reached */

    ಪ್ರತ್ಯಾಗಮ;

done:

    pool->stats[slot].used--;

    ngx_slab_junk(p, size);

    ಪ್ರತ್ಯಾಗಮ;

wrong_chunk:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): pointer to wrong chunk");

    ಗಚ್ಛ fail;

chunk_already_free:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): chunk is already free");

fail:

    ಪ್ರತ್ಯಾಗಮ;
}


ಸ್ಥಿರ ngx_slab_page_t *
ngx_slab_alloc_pages(ngx_slab_pool_t *pool, ngx_uint_t pages)
{
    ngx_slab_page_t  *page, *p;

    ಪ್ರತಿ (page = pool->ಮುಕ್ತ.next; page != &pool->ಮುಕ್ತ; page = page->next) {

        ಯದಿ (page->slab >= pages) {

            ಯದಿ (page->slab > pages) {
                page[page->slab - 1].prev = (uintptr_t) &page[pages];

                page[pages].slab = page->slab - pages;
                page[pages].next = page->next;
                page[pages].prev = page->prev;

                p = (ngx_slab_page_t *) page->prev;
                p->next = &page[pages];
                page->next->prev = (uintptr_t) &page[pages];

            } ಅನ್ಯಥಾ {
                p = (ngx_slab_page_t *) page->prev;
                p->next = page->next;
                page->next->prev = page->prev;
            }

            page->slab = pages | NGX_SLAB_PAGE_START;
            page->next = ರಿಕ್ತ;
            page->prev = NGX_SLAB_PAGE;

            pool->pfree -= pages;

            ಯದಿ (--pages == 0) {
                ಪ್ರತ್ಯಾಗಮ page;
            }

            ಪ್ರತಿ (p = page + 1; pages; pages--) {
                p->slab = NGX_SLAB_PAGE_BUSY;
                p->next = ರಿಕ್ತ;
                p->prev = NGX_SLAB_PAGE;
                p++;
            }

            ಪ್ರತ್ಯಾಗಮ page;
        }
    }

    ಯದಿ (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_slab_free_pages(ngx_slab_pool_t *pool, ngx_slab_page_t *page,
    ngx_uint_t pages)
{
    ngx_slab_page_t  *prev, *join;

    pool->pfree += pages;

    page->slab = pages--;

    ಯದಿ (pages) {
        ngx_memzero(&page[1], pages * ಪರಿಮಾಣ(ngx_slab_page_t));
    }

    ಯದಿ (page->next) {
        prev = ngx_slab_page_prev(page);
        prev->next = page->next;
        page->next->prev = page->prev;
    }

    join = page + page->slab;

    ಯದಿ (join < pool->last) {

        ಯದಿ (ngx_slab_page_type(join) == NGX_SLAB_PAGE) {

            ಯದಿ (join->next != ರಿಕ್ತ) {
                pages += join->slab;
                page->slab += join->slab;

                prev = ngx_slab_page_prev(join);
                prev->next = join->next;
                join->next->prev = join->prev;

                join->slab = NGX_SLAB_PAGE_FREE;
                join->next = ರಿಕ್ತ;
                join->prev = NGX_SLAB_PAGE;
            }
        }
    }

    ಯದಿ (page > pool->pages) {
        join = page - 1;

        ಯದಿ (ngx_slab_page_type(join) == NGX_SLAB_PAGE) {

            ಯದಿ (join->slab == NGX_SLAB_PAGE_FREE) {
                join = ngx_slab_page_prev(join);
            }

            ಯದಿ (join->next != ರಿಕ್ತ) {
                pages += join->slab;
                join->slab += page->slab;

                prev = ngx_slab_page_prev(join);
                prev->next = join->next;
                join->next->prev = join->prev;

                page->slab = NGX_SLAB_PAGE_FREE;
                page->next = ರಿಕ್ತ;
                page->prev = NGX_SLAB_PAGE;

                page = join;
            }
        }
    }

    ಯದಿ (pages) {
        page[pages].prev = (uintptr_t) page;
    }

    page->prev = (uintptr_t) &pool->ಮುಕ್ತ;
    page->next = pool->ಮುಕ್ತ.next;

    page->next->prev = (uintptr_t) page;

    pool->ಮುಕ್ತ.next = page;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, ವರ್ಣ *text)
{
    ngx_log_error(level, ngx_cycle->log, 0, "%s%s", text, pool->log_ctx);
}
