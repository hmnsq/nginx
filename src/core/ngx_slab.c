
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#define NGX_SLAB_PAGE_MASK   3
#define NGX_SLAB_PAGE        0
#define NGX_SLAB_BIG         1
#define NGX_SLAB_EXACT       2
#define NGX_SLAB_SMALL       3

#إذا (NGX_PTR_SIZE == 4)

#define NGX_SLAB_PAGE_FREE   0
#define NGX_SLAB_PAGE_BUSY   0xffffffff
#define NGX_SLAB_PAGE_START  0x80000000

#define NGX_SLAB_SHIFT_MASK  0x0000000f
#define NGX_SLAB_MAP_MASK    0xffff0000
#define NGX_SLAB_MAP_SHIFT   16

#define NGX_SLAB_BUSY        0xffffffff

#وإلا /* (NGX_PTR_SIZE == 8) */

#define NGX_SLAB_PAGE_FREE   0
#define NGX_SLAB_PAGE_BUSY   0xffffffffffffffff
#define NGX_SLAB_PAGE_START  0x8000000000000000

#define NGX_SLAB_SHIFT_MASK  0x000000000000000f
#define NGX_SLAB_MAP_MASK    0xffffffff00000000
#define NGX_SLAB_MAP_SHIFT   32

#define NGX_SLAB_BUSY        0xffffffffffffffff

#endif


#define ngx_slab_slots(pool)                                                  \
    (ngx_slab_page_t *) ((u_char *) (pool) + حجم(ngx_slab_pool_t))

#define ngx_slab_page_type(page)   ((page)->prev & NGX_SLAB_PAGE_MASK)

#define ngx_slab_page_prev(page)                                              \
    (ngx_slab_page_t *) ((page)->prev & ~NGX_SLAB_PAGE_MASK)

#define ngx_slab_page_addr(pool, page)                                        \
    ((((page) - (pool)->pages) << ngx_pagesize_shift)                         \
     + (uintptr_t) (pool)->start)


#إذا (NGX_DEBUG_MALLOC)

#define ngx_slab_junk(p, size)     ngx_memset(p, 0xA5, size)

#وإلاإذا (NGX_HAVE_DEBUG_MALLOC)

#define ngx_slab_junk(p, size)                                                \
    إذا (ngx_debug_malloc)          ngx_memset(p, 0xA5, size)

#وإلا

#define ngx_slab_junk(p, size)

#endif

ساكن ngx_slab_page_t *ngx_slab_alloc_pages(ngx_slab_pool_t *pool,
    ngx_uint_t pages);
ساكن فراغ ngx_slab_free_pages(ngx_slab_pool_t *pool, ngx_slab_page_t *page,
    ngx_uint_t pages);
ساكن فراغ ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    حرف *text);


ساكن ngx_uint_t  ngx_slab_max_size;
ساكن ngx_uint_t  ngx_slab_exact_size;
ساكن ngx_uint_t  ngx_slab_exact_shift;


فراغ
ngx_slab_sizes_init(فراغ)
{
    ngx_uint_t  n;

    ngx_slab_max_size = ngx_pagesize / 2;
    ngx_slab_exact_size = ngx_pagesize / (8 * حجم(uintptr_t));
    لكل (n = ngx_slab_exact_size; n >>= 1; ngx_slab_exact_shift++) {
        /* void */
    }
}


فراغ
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

    لكل (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].next = &slots[i];
        slots[i].prev = 0;
    }

    p += n * حجم(ngx_slab_page_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * حجم(ngx_slab_stat_t));

    p += n * حجم(ngx_slab_stat_t);

    size -= n * (حجم(ngx_slab_page_t) + حجم(ngx_slab_stat_t));

    pages = (ngx_uint_t) (size / (ngx_pagesize + حجم(ngx_slab_page_t)));

    pool->pages = (ngx_slab_page_t *) p;
    ngx_memzero(pool->pages, pages * حجم(ngx_slab_page_t));

    page = pool->pages;

    /* only "next" is used in list head */
    pool->حرر.slab = 0;
    pool->حرر.next = page;
    pool->حرر.prev = 0;

    page->slab = pages;
    page->next = &pool->حرر;
    page->prev = (uintptr_t) &pool->حرر;

    pool->start = ngx_align_ptr(p + pages * حجم(ngx_slab_page_t),
                                ngx_pagesize);

    m = pages - (pool->end - pool->start) / ngx_pagesize;
    إذا (m > 0) {
        pages -= m;
        page->slab = pages;
    }

    pool->last = pool->pages + pages;
    pool->pfree = pages;

    pool->log_nomem = 1;
    pool->log_ctx = &pool->zero;
    pool->zero = '\0';
}


فراغ *
ngx_slab_alloc(ngx_slab_pool_t *pool, size_t size)
{
    فراغ  *p;

    ngx_shmtx_lock(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, size);

    ngx_shmtx_unlock(&pool->mutex);

    أرجع p;
}


فراغ *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, size_t size)
{
    size_t            s;
    uintptr_t         p, m, mask, *bitmap;
    ngx_uint_t        i, n, slot, shift, map;
    ngx_slab_page_t  *page, *prev, *slots;

    إذا (size > ngx_slab_max_size) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                       "slab alloc: %uz", size);

        page = ngx_slab_alloc_pages(pool, (size >> ngx_pagesize_shift)
                                          + ((size % ngx_pagesize) ? 1 : 0));
        إذا (page) {
            p = ngx_slab_page_addr(pool, page);

        } وإلا {
            p = 0;
        }

        اذهب done;
    }

    إذا (size > pool->min_size) {
        shift = 1;
        لكل (s = size - 1; s >>= 1; shift++) { /* void */ }
        slot = shift - pool->min_shift;

    } وإلا {
        shift = pool->min_shift;
        slot = 0;
    }

    pool->stats[slot].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                   "slab alloc: %uz slot: %ui", size, slot);

    slots = ngx_slab_slots(pool);
    page = slots[slot].next;

    إذا (page->next != page) {

        إذا (shift < ngx_slab_exact_shift) {

            bitmap = (uintptr_t *) ngx_slab_page_addr(pool, page);

            map = (ngx_pagesize >> shift) / (8 * حجم(uintptr_t));

            لكل (n = 0; n < map; n++) {

                إذا (bitmap[n] != NGX_SLAB_BUSY) {

                    لكل (m = 1, i = 0; m; m <<= 1, i++) {
                        إذا (bitmap[n] & m) {
                            استمر;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * حجم(uintptr_t) + i) << shift;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[slot].used++;

                        إذا (bitmap[n] == NGX_SLAB_BUSY) {
                            لكل (n = n + 1; n < map; n++) {
                                إذا (bitmap[n] != NGX_SLAB_BUSY) {
                                    اذهب done;
                                }
                            }

                            prev = ngx_slab_page_prev(page);
                            prev->next = page->next;
                            page->next->prev = page->prev;

                            page->next = عدم;
                            page->prev = NGX_SLAB_SMALL;
                        }

                        اذهب done;
                    }
                }
            }

        } وإلا إذا (shift == ngx_slab_exact_shift) {

            لكل (m = 1, i = 0; m; m <<= 1, i++) {
                إذا (page->slab & m) {
                    استمر;
                }

                page->slab |= m;

                إذا (page->slab == NGX_SLAB_BUSY) {
                    prev = ngx_slab_page_prev(page);
                    prev->next = page->next;
                    page->next->prev = page->prev;

                    page->next = عدم;
                    page->prev = NGX_SLAB_EXACT;
                }

                p = ngx_slab_page_addr(pool, page) + (i << shift);

                pool->stats[slot].used++;

                اذهب done;
            }

        } وإلا { /* shift > ngx_slab_exact_shift */

            mask = ((uintptr_t) 1 << (ngx_pagesize >> shift)) - 1;
            mask <<= NGX_SLAB_MAP_SHIFT;

            لكل (m = (uintptr_t) 1 << NGX_SLAB_MAP_SHIFT, i = 0;
                 m & mask;
                 m <<= 1, i++)
            {
                إذا (page->slab & m) {
                    استمر;
                }

                page->slab |= m;

                إذا ((page->slab & NGX_SLAB_MAP_MASK) == mask) {
                    prev = ngx_slab_page_prev(page);
                    prev->next = page->next;
                    page->next->prev = page->prev;

                    page->next = عدم;
                    page->prev = NGX_SLAB_BIG;
                }

                p = ngx_slab_page_addr(pool, page) + (i << shift);

                pool->stats[slot].used++;

                اذهب done;
            }
        }

        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_alloc(): page is busy");
        ngx_debug_point();
    }

    page = ngx_slab_alloc_pages(pool, 1);

    إذا (page) {
        إذا (shift < ngx_slab_exact_shift) {
            bitmap = (uintptr_t *) ngx_slab_page_addr(pool, page);

            n = (ngx_pagesize >> shift) / ((1 << shift) * 8);

            إذا (n == 0) {
                n = 1;
            }

            /* "n" elements for bitmap, plus one requested */

            لكل (i = 0; i < (n + 1) / (8 * حجم(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * حجم(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> shift) / (8 * حجم(uintptr_t));

            لكل (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            page->slab = shift;
            page->next = &slots[slot];
            page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_SMALL;

            slots[slot].next = page;

            pool->stats[slot].total += (ngx_pagesize >> shift) - n;

            p = ngx_slab_page_addr(pool, page) + (n << shift);

            pool->stats[slot].used++;

            اذهب done;

        } وإلا إذا (shift == ngx_slab_exact_shift) {

            page->slab = 1;
            page->next = &slots[slot];
            page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_EXACT;

            slots[slot].next = page;

            pool->stats[slot].total += 8 * حجم(uintptr_t);

            p = ngx_slab_page_addr(pool, page);

            pool->stats[slot].used++;

            اذهب done;

        } وإلا { /* shift > ngx_slab_exact_shift */

            page->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_SHIFT) | shift;
            page->next = &slots[slot];
            page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_BIG;

            slots[slot].next = page;

            pool->stats[slot].total += ngx_pagesize >> shift;

            p = ngx_slab_page_addr(pool, page);

            pool->stats[slot].used++;

            اذهب done;
        }
    }

    p = 0;

    pool->stats[slot].fails++;

done:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                   "slab alloc: %p", (فراغ *) p);

    أرجع (فراغ *) p;
}


فراغ *
ngx_slab_calloc(ngx_slab_pool_t *pool, size_t size)
{
    فراغ  *p;

    ngx_shmtx_lock(&pool->mutex);

    p = ngx_slab_calloc_locked(pool, size);

    ngx_shmtx_unlock(&pool->mutex);

    أرجع p;
}


فراغ *
ngx_slab_calloc_locked(ngx_slab_pool_t *pool, size_t size)
{
    فراغ  *p;

    p = ngx_slab_alloc_locked(pool, size);
    إذا (p) {
        ngx_memzero(p, size);
    }

    أرجع p;
}


فراغ
ngx_slab_free(ngx_slab_pool_t *pool, فراغ *p)
{
    ngx_shmtx_lock(&pool->mutex);

    ngx_slab_free_locked(pool, p);

    ngx_shmtx_unlock(&pool->mutex);
}


فراغ
ngx_slab_free_locked(ngx_slab_pool_t *pool, فراغ *p)
{
    size_t            size;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, slot, shift, map;
    ngx_slab_page_t  *slots, *page;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0, "slab free: %p", p);

    إذا ((u_char *) p < pool->start || (u_char *) p > pool->end) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        اذهب fail;
    }

    n = ((u_char *) p - pool->start) >> ngx_pagesize_shift;
    page = &pool->pages[n];
    slab = page->slab;
    type = ngx_slab_page_type(page);

    بدل (type) {

    حالة NGX_SLAB_SMALL:

        shift = slab & NGX_SLAB_SHIFT_MASK;
        size = (size_t) 1 << shift;

        إذا ((uintptr_t) p & (size - 1)) {
            اذهب wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> shift;
        m = (uintptr_t) 1 << (n % (8 * حجم(uintptr_t)));
        n /= 8 * حجم(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        إذا (bitmap[n] & m) {
            slot = shift - pool->min_shift;

            إذا (page->next == عدم) {
                slots = ngx_slab_slots(pool);

                page->next = slots[slot].next;
                slots[slot].next = page;

                page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_SMALL;
                page->next->prev = (uintptr_t) page | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> shift) / ((1 << shift) * 8);

            إذا (n == 0) {
                n = 1;
            }

            i = n / (8 * حجم(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * حجم(uintptr_t)))) - 1;

            إذا (bitmap[i] & ~m) {
                اذهب done;
            }

            map = (ngx_pagesize >> shift) / (8 * حجم(uintptr_t));

            لكل (i = i + 1; i < map; i++) {
                إذا (bitmap[i]) {
                    اذهب done;
                }
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].total -= (ngx_pagesize >> shift) - n;

            اذهب done;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_shift);
        size = ngx_slab_exact_size;

        إذا ((uintptr_t) p & (size - 1)) {
            اذهب wrong_chunk;
        }

        إذا (slab & m) {
            slot = ngx_slab_exact_shift - pool->min_shift;

            إذا (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                page->next = slots[slot].next;
                slots[slot].next = page;

                page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_EXACT;
                page->next->prev = (uintptr_t) page | NGX_SLAB_EXACT;
            }

            page->slab &= ~m;

            إذا (page->slab) {
                اذهب done;
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].total -= 8 * حجم(uintptr_t);

            اذهب done;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_BIG:

        shift = slab & NGX_SLAB_SHIFT_MASK;
        size = (size_t) 1 << shift;

        إذا ((uintptr_t) p & (size - 1)) {
            اذهب wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> shift)
                              + NGX_SLAB_MAP_SHIFT);

        إذا (slab & m) {
            slot = shift - pool->min_shift;

            إذا (page->next == عدم) {
                slots = ngx_slab_slots(pool);

                page->next = slots[slot].next;
                slots[slot].next = page;

                page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_BIG;
                page->next->prev = (uintptr_t) page | NGX_SLAB_BIG;
            }

            page->slab &= ~m;

            إذا (page->slab & NGX_SLAB_MAP_MASK) {
                اذهب done;
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].total -= ngx_pagesize >> shift;

            اذهب done;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_PAGE:

        إذا ((uintptr_t) p & (ngx_pagesize - 1)) {
            اذهب wrong_chunk;
        }

        إذا (!(slab & NGX_SLAB_PAGE_START)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            اذهب fail;
        }

        إذا (slab == NGX_SLAB_PAGE_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            اذهب fail;
        }

        size = slab & ~NGX_SLAB_PAGE_START;

        ngx_slab_free_pages(pool, page, size);

        ngx_slab_junk(p, size << ngx_pagesize_shift);

        أرجع;
    }

    /* not reached */

    أرجع;

done:

    pool->stats[slot].used--;

    ngx_slab_junk(p, size);

    أرجع;

wrong_chunk:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): pointer to wrong chunk");

    اذهب fail;

chunk_already_free:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): chunk is already free");

fail:

    أرجع;
}


ساكن ngx_slab_page_t *
ngx_slab_alloc_pages(ngx_slab_pool_t *pool, ngx_uint_t pages)
{
    ngx_slab_page_t  *page, *p;

    لكل (page = pool->حرر.next; page != &pool->حرر; page = page->next) {

        إذا (page->slab >= pages) {

            إذا (page->slab > pages) {
                page[page->slab - 1].prev = (uintptr_t) &page[pages];

                page[pages].slab = page->slab - pages;
                page[pages].next = page->next;
                page[pages].prev = page->prev;

                p = (ngx_slab_page_t *) page->prev;
                p->next = &page[pages];
                page->next->prev = (uintptr_t) &page[pages];

            } وإلا {
                p = (ngx_slab_page_t *) page->prev;
                p->next = page->next;
                page->next->prev = page->prev;
            }

            page->slab = pages | NGX_SLAB_PAGE_START;
            page->next = عدم;
            page->prev = NGX_SLAB_PAGE;

            pool->pfree -= pages;

            إذا (--pages == 0) {
                أرجع page;
            }

            لكل (p = page + 1; pages; pages--) {
                p->slab = NGX_SLAB_PAGE_BUSY;
                p->next = عدم;
                p->prev = NGX_SLAB_PAGE;
                p++;
            }

            أرجع page;
        }
    }

    إذا (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    أرجع عدم;
}


ساكن فراغ
ngx_slab_free_pages(ngx_slab_pool_t *pool, ngx_slab_page_t *page,
    ngx_uint_t pages)
{
    ngx_slab_page_t  *prev, *join;

    pool->pfree += pages;

    page->slab = pages--;

    إذا (pages) {
        ngx_memzero(&page[1], pages * حجم(ngx_slab_page_t));
    }

    إذا (page->next) {
        prev = ngx_slab_page_prev(page);
        prev->next = page->next;
        page->next->prev = page->prev;
    }

    join = page + page->slab;

    إذا (join < pool->last) {

        إذا (ngx_slab_page_type(join) == NGX_SLAB_PAGE) {

            إذا (join->next != عدم) {
                pages += join->slab;
                page->slab += join->slab;

                prev = ngx_slab_page_prev(join);
                prev->next = join->next;
                join->next->prev = join->prev;

                join->slab = NGX_SLAB_PAGE_FREE;
                join->next = عدم;
                join->prev = NGX_SLAB_PAGE;
            }
        }
    }

    إذا (page > pool->pages) {
        join = page - 1;

        إذا (ngx_slab_page_type(join) == NGX_SLAB_PAGE) {

            إذا (join->slab == NGX_SLAB_PAGE_FREE) {
                join = ngx_slab_page_prev(join);
            }

            إذا (join->next != عدم) {
                pages += join->slab;
                join->slab += page->slab;

                prev = ngx_slab_page_prev(join);
                prev->next = join->next;
                join->next->prev = join->prev;

                page->slab = NGX_SLAB_PAGE_FREE;
                page->next = عدم;
                page->prev = NGX_SLAB_PAGE;

                page = join;
            }
        }
    }

    إذا (pages) {
        page[pages].prev = (uintptr_t) page;
    }

    page->prev = (uintptr_t) &pool->حرر;
    page->next = pool->حرر.next;

    page->next->prev = (uintptr_t) page;

    pool->حرر.next = page;
}


ساكن فراغ
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, حرف *text)
{
    ngx_log_error(level, ngx_cycle->log, 0, "%s%s", text, pool->log_ctx);
}
