
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#define NGX_SLAB_PAGE_MASK   3
#define NGX_SLAB_PAGE        0
#define NGX_SLAB_BIG         1
#define NGX_SLAB_EXACT       2
#define NGX_SLAB_SMALL       3

#जर (NGX_PTR_SIZE == 4)

#define NGX_SLAB_PAGE_FREE   0
#define NGX_SLAB_PAGE_BUSY   0xffffffff
#define NGX_SLAB_PAGE_START  0x80000000

#define NGX_SLAB_SHIFT_MASK  0x0000000f
#define NGX_SLAB_MAP_MASK    0xffff0000
#define NGX_SLAB_MAP_SHIFT   16

#define NGX_SLAB_BUSY        0xffffffff

#अन्यथा /* (NGX_PTR_SIZE == 8) */

#define NGX_SLAB_PAGE_FREE   0
#define NGX_SLAB_PAGE_BUSY   0xffffffffffffffff
#define NGX_SLAB_PAGE_START  0x8000000000000000

#define NGX_SLAB_SHIFT_MASK  0x000000000000000f
#define NGX_SLAB_MAP_MASK    0xffffffff00000000
#define NGX_SLAB_MAP_SHIFT   32

#define NGX_SLAB_BUSY        0xffffffffffffffff

#endif


#define ngx_slab_slots(pool)                                                  \
    (ngx_slab_page_t *) ((u_char *) (pool) + आकार(ngx_slab_pool_t))

#define ngx_slab_page_type(page)   ((page)->prev & NGX_SLAB_PAGE_MASK)

#define ngx_slab_page_prev(page)                                              \
    (ngx_slab_page_t *) ((page)->prev & ~NGX_SLAB_PAGE_MASK)

#define ngx_slab_page_addr(pool, page)                                        \
    ((((page) - (pool)->pages) << ngx_pagesize_shift)                         \
     + (uintptr_t) (pool)->start)


#जर (NGX_DEBUG_MALLOC)

#define ngx_slab_junk(p, size)     ngx_memset(p, 0xA5, size)

#किंवाजर (NGX_HAVE_DEBUG_MALLOC)

#define ngx_slab_junk(p, size)                                                \
    जर (ngx_debug_malloc)          ngx_memset(p, 0xA5, size)

#अन्यथा

#define ngx_slab_junk(p, size)

#endif

स्थिर ngx_slab_page_t *ngx_slab_alloc_pages(ngx_slab_pool_t *pool,
    ngx_uint_t pages);
स्थिर शून्य ngx_slab_free_pages(ngx_slab_pool_t *pool, ngx_slab_page_t *page,
    ngx_uint_t pages);
स्थिर शून्य ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    अक्षर *text);


स्थिर ngx_uint_t  ngx_slab_max_size;
स्थिर ngx_uint_t  ngx_slab_exact_size;
स्थिर ngx_uint_t  ngx_slab_exact_shift;


शून्य
ngx_slab_sizes_init(शून्य)
{
    ngx_uint_t  n;

    ngx_slab_max_size = ngx_pagesize / 2;
    ngx_slab_exact_size = ngx_pagesize / (8 * आकार(uintptr_t));
    साठी (n = ngx_slab_exact_size; n >>= 1; ngx_slab_exact_shift++) {
        /* void */
    }
}


शून्य
ngx_slab_init(ngx_slab_pool_t *pool)
{
    u_char           *p;
    माप_प्रकार            size;
    ngx_int_t         m;
    ngx_uint_t        i, n, pages;
    ngx_slab_page_t  *slots, *page;

    pool->min_size = (माप_प्रकार) 1 << pool->min_shift;

    slots = ngx_slab_slots(pool);

    p = (u_char *) slots;
    size = pool->end - p;

    ngx_slab_junk(p, size);

    n = ngx_pagesize_shift - pool->min_shift;

    साठी (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].next = &slots[i];
        slots[i].prev = 0;
    }

    p += n * आकार(ngx_slab_page_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * आकार(ngx_slab_stat_t));

    p += n * आकार(ngx_slab_stat_t);

    size -= n * (आकार(ngx_slab_page_t) + आकार(ngx_slab_stat_t));

    pages = (ngx_uint_t) (size / (ngx_pagesize + आकार(ngx_slab_page_t)));

    pool->pages = (ngx_slab_page_t *) p;
    ngx_memzero(pool->pages, pages * आकार(ngx_slab_page_t));

    page = pool->pages;

    /* only "next" is used in list head */
    pool->मुक्त.slab = 0;
    pool->मुक्त.next = page;
    pool->मुक्त.prev = 0;

    page->slab = pages;
    page->next = &pool->मुक्त;
    page->prev = (uintptr_t) &pool->मुक्त;

    pool->start = ngx_align_ptr(p + pages * आकार(ngx_slab_page_t),
                                ngx_pagesize);

    m = pages - (pool->end - pool->start) / ngx_pagesize;
    जर (m > 0) {
        pages -= m;
        page->slab = pages;
    }

    pool->last = pool->pages + pages;
    pool->pfree = pages;

    pool->log_nomem = 1;
    pool->log_ctx = &pool->zero;
    pool->zero = '\0';
}


शून्य *
ngx_slab_alloc(ngx_slab_pool_t *pool, माप_प्रकार size)
{
    शून्य  *p;

    ngx_shmtx_lock(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, size);

    ngx_shmtx_unlock(&pool->mutex);

    परत p;
}


शून्य *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, माप_प्रकार size)
{
    माप_प्रकार            s;
    uintptr_t         p, m, mask, *bitmap;
    ngx_uint_t        i, n, slot, shift, map;
    ngx_slab_page_t  *page, *prev, *slots;

    जर (size > ngx_slab_max_size) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                       "slab alloc: %uz", size);

        page = ngx_slab_alloc_pages(pool, (size >> ngx_pagesize_shift)
                                          + ((size % ngx_pagesize) ? 1 : 0));
        जर (page) {
            p = ngx_slab_page_addr(pool, page);

        } अन्यथा {
            p = 0;
        }

        जा done;
    }

    जर (size > pool->min_size) {
        shift = 1;
        साठी (s = size - 1; s >>= 1; shift++) { /* void */ }
        slot = shift - pool->min_shift;

    } अन्यथा {
        shift = pool->min_shift;
        slot = 0;
    }

    pool->stats[slot].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                   "slab alloc: %uz slot: %ui", size, slot);

    slots = ngx_slab_slots(pool);
    page = slots[slot].next;

    जर (page->next != page) {

        जर (shift < ngx_slab_exact_shift) {

            bitmap = (uintptr_t *) ngx_slab_page_addr(pool, page);

            map = (ngx_pagesize >> shift) / (8 * आकार(uintptr_t));

            साठी (n = 0; n < map; n++) {

                जर (bitmap[n] != NGX_SLAB_BUSY) {

                    साठी (m = 1, i = 0; m; m <<= 1, i++) {
                        जर (bitmap[n] & m) {
                            सुरूठेवा;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * आकार(uintptr_t) + i) << shift;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[slot].used++;

                        जर (bitmap[n] == NGX_SLAB_BUSY) {
                            साठी (n = n + 1; n < map; n++) {
                                जर (bitmap[n] != NGX_SLAB_BUSY) {
                                    जा done;
                                }
                            }

                            prev = ngx_slab_page_prev(page);
                            prev->next = page->next;
                            page->next->prev = page->prev;

                            page->next = रिक्त;
                            page->prev = NGX_SLAB_SMALL;
                        }

                        जा done;
                    }
                }
            }

        } अन्यथा जर (shift == ngx_slab_exact_shift) {

            साठी (m = 1, i = 0; m; m <<= 1, i++) {
                जर (page->slab & m) {
                    सुरूठेवा;
                }

                page->slab |= m;

                जर (page->slab == NGX_SLAB_BUSY) {
                    prev = ngx_slab_page_prev(page);
                    prev->next = page->next;
                    page->next->prev = page->prev;

                    page->next = रिक्त;
                    page->prev = NGX_SLAB_EXACT;
                }

                p = ngx_slab_page_addr(pool, page) + (i << shift);

                pool->stats[slot].used++;

                जा done;
            }

        } अन्यथा { /* shift > ngx_slab_exact_shift */

            mask = ((uintptr_t) 1 << (ngx_pagesize >> shift)) - 1;
            mask <<= NGX_SLAB_MAP_SHIFT;

            साठी (m = (uintptr_t) 1 << NGX_SLAB_MAP_SHIFT, i = 0;
                 m & mask;
                 m <<= 1, i++)
            {
                जर (page->slab & m) {
                    सुरूठेवा;
                }

                page->slab |= m;

                जर ((page->slab & NGX_SLAB_MAP_MASK) == mask) {
                    prev = ngx_slab_page_prev(page);
                    prev->next = page->next;
                    page->next->prev = page->prev;

                    page->next = रिक्त;
                    page->prev = NGX_SLAB_BIG;
                }

                p = ngx_slab_page_addr(pool, page) + (i << shift);

                pool->stats[slot].used++;

                जा done;
            }
        }

        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_alloc(): page is busy");
        ngx_debug_point();
    }

    page = ngx_slab_alloc_pages(pool, 1);

    जर (page) {
        जर (shift < ngx_slab_exact_shift) {
            bitmap = (uintptr_t *) ngx_slab_page_addr(pool, page);

            n = (ngx_pagesize >> shift) / ((1 << shift) * 8);

            जर (n == 0) {
                n = 1;
            }

            /* "n" elements for bitmap, plus one requested */

            साठी (i = 0; i < (n + 1) / (8 * आकार(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * आकार(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> shift) / (8 * आकार(uintptr_t));

            साठी (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            page->slab = shift;
            page->next = &slots[slot];
            page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_SMALL;

            slots[slot].next = page;

            pool->stats[slot].total += (ngx_pagesize >> shift) - n;

            p = ngx_slab_page_addr(pool, page) + (n << shift);

            pool->stats[slot].used++;

            जा done;

        } अन्यथा जर (shift == ngx_slab_exact_shift) {

            page->slab = 1;
            page->next = &slots[slot];
            page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_EXACT;

            slots[slot].next = page;

            pool->stats[slot].total += 8 * आकार(uintptr_t);

            p = ngx_slab_page_addr(pool, page);

            pool->stats[slot].used++;

            जा done;

        } अन्यथा { /* shift > ngx_slab_exact_shift */

            page->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_SHIFT) | shift;
            page->next = &slots[slot];
            page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_BIG;

            slots[slot].next = page;

            pool->stats[slot].total += ngx_pagesize >> shift;

            p = ngx_slab_page_addr(pool, page);

            pool->stats[slot].used++;

            जा done;
        }
    }

    p = 0;

    pool->stats[slot].fails++;

done:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                   "slab alloc: %p", (शून्य *) p);

    परत (शून्य *) p;
}


शून्य *
ngx_slab_calloc(ngx_slab_pool_t *pool, माप_प्रकार size)
{
    शून्य  *p;

    ngx_shmtx_lock(&pool->mutex);

    p = ngx_slab_calloc_locked(pool, size);

    ngx_shmtx_unlock(&pool->mutex);

    परत p;
}


शून्य *
ngx_slab_calloc_locked(ngx_slab_pool_t *pool, माप_प्रकार size)
{
    शून्य  *p;

    p = ngx_slab_alloc_locked(pool, size);
    जर (p) {
        ngx_memzero(p, size);
    }

    परत p;
}


शून्य
ngx_slab_free(ngx_slab_pool_t *pool, शून्य *p)
{
    ngx_shmtx_lock(&pool->mutex);

    ngx_slab_free_locked(pool, p);

    ngx_shmtx_unlock(&pool->mutex);
}


शून्य
ngx_slab_free_locked(ngx_slab_pool_t *pool, शून्य *p)
{
    माप_प्रकार            size;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, slot, shift, map;
    ngx_slab_page_t  *slots, *page;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0, "slab free: %p", p);

    जर ((u_char *) p < pool->start || (u_char *) p > pool->end) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        जा fail;
    }

    n = ((u_char *) p - pool->start) >> ngx_pagesize_shift;
    page = &pool->pages[n];
    slab = page->slab;
    type = ngx_slab_page_type(page);

    निवड (type) {

    स्थिती NGX_SLAB_SMALL:

        shift = slab & NGX_SLAB_SHIFT_MASK;
        size = (माप_प्रकार) 1 << shift;

        जर ((uintptr_t) p & (size - 1)) {
            जा wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> shift;
        m = (uintptr_t) 1 << (n % (8 * आकार(uintptr_t)));
        n /= 8 * आकार(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        जर (bitmap[n] & m) {
            slot = shift - pool->min_shift;

            जर (page->next == रिक्त) {
                slots = ngx_slab_slots(pool);

                page->next = slots[slot].next;
                slots[slot].next = page;

                page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_SMALL;
                page->next->prev = (uintptr_t) page | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> shift) / ((1 << shift) * 8);

            जर (n == 0) {
                n = 1;
            }

            i = n / (8 * आकार(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * आकार(uintptr_t)))) - 1;

            जर (bitmap[i] & ~m) {
                जा done;
            }

            map = (ngx_pagesize >> shift) / (8 * आकार(uintptr_t));

            साठी (i = i + 1; i < map; i++) {
                जर (bitmap[i]) {
                    जा done;
                }
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].total -= (ngx_pagesize >> shift) - n;

            जा done;
        }

        जा chunk_already_free;

    स्थिती NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_shift);
        size = ngx_slab_exact_size;

        जर ((uintptr_t) p & (size - 1)) {
            जा wrong_chunk;
        }

        जर (slab & m) {
            slot = ngx_slab_exact_shift - pool->min_shift;

            जर (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                page->next = slots[slot].next;
                slots[slot].next = page;

                page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_EXACT;
                page->next->prev = (uintptr_t) page | NGX_SLAB_EXACT;
            }

            page->slab &= ~m;

            जर (page->slab) {
                जा done;
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].total -= 8 * आकार(uintptr_t);

            जा done;
        }

        जा chunk_already_free;

    स्थिती NGX_SLAB_BIG:

        shift = slab & NGX_SLAB_SHIFT_MASK;
        size = (माप_प्रकार) 1 << shift;

        जर ((uintptr_t) p & (size - 1)) {
            जा wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> shift)
                              + NGX_SLAB_MAP_SHIFT);

        जर (slab & m) {
            slot = shift - pool->min_shift;

            जर (page->next == रिक्त) {
                slots = ngx_slab_slots(pool);

                page->next = slots[slot].next;
                slots[slot].next = page;

                page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_BIG;
                page->next->prev = (uintptr_t) page | NGX_SLAB_BIG;
            }

            page->slab &= ~m;

            जर (page->slab & NGX_SLAB_MAP_MASK) {
                जा done;
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].total -= ngx_pagesize >> shift;

            जा done;
        }

        जा chunk_already_free;

    स्थिती NGX_SLAB_PAGE:

        जर ((uintptr_t) p & (ngx_pagesize - 1)) {
            जा wrong_chunk;
        }

        जर (!(slab & NGX_SLAB_PAGE_START)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            जा fail;
        }

        जर (slab == NGX_SLAB_PAGE_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            जा fail;
        }

        size = slab & ~NGX_SLAB_PAGE_START;

        ngx_slab_free_pages(pool, page, size);

        ngx_slab_junk(p, size << ngx_pagesize_shift);

        परत;
    }

    /* not reached */

    परत;

done:

    pool->stats[slot].used--;

    ngx_slab_junk(p, size);

    परत;

wrong_chunk:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): pointer to wrong chunk");

    जा fail;

chunk_already_free:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): chunk is already free");

fail:

    परत;
}


स्थिर ngx_slab_page_t *
ngx_slab_alloc_pages(ngx_slab_pool_t *pool, ngx_uint_t pages)
{
    ngx_slab_page_t  *page, *p;

    साठी (page = pool->मुक्त.next; page != &pool->मुक्त; page = page->next) {

        जर (page->slab >= pages) {

            जर (page->slab > pages) {
                page[page->slab - 1].prev = (uintptr_t) &page[pages];

                page[pages].slab = page->slab - pages;
                page[pages].next = page->next;
                page[pages].prev = page->prev;

                p = (ngx_slab_page_t *) page->prev;
                p->next = &page[pages];
                page->next->prev = (uintptr_t) &page[pages];

            } अन्यथा {
                p = (ngx_slab_page_t *) page->prev;
                p->next = page->next;
                page->next->prev = page->prev;
            }

            page->slab = pages | NGX_SLAB_PAGE_START;
            page->next = रिक्त;
            page->prev = NGX_SLAB_PAGE;

            pool->pfree -= pages;

            जर (--pages == 0) {
                परत page;
            }

            साठी (p = page + 1; pages; pages--) {
                p->slab = NGX_SLAB_PAGE_BUSY;
                p->next = रिक्त;
                p->prev = NGX_SLAB_PAGE;
                p++;
            }

            परत page;
        }
    }

    जर (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    परत रिक्त;
}


स्थिर शून्य
ngx_slab_free_pages(ngx_slab_pool_t *pool, ngx_slab_page_t *page,
    ngx_uint_t pages)
{
    ngx_slab_page_t  *prev, *join;

    pool->pfree += pages;

    page->slab = pages--;

    जर (pages) {
        ngx_memzero(&page[1], pages * आकार(ngx_slab_page_t));
    }

    जर (page->next) {
        prev = ngx_slab_page_prev(page);
        prev->next = page->next;
        page->next->prev = page->prev;
    }

    join = page + page->slab;

    जर (join < pool->last) {

        जर (ngx_slab_page_type(join) == NGX_SLAB_PAGE) {

            जर (join->next != रिक्त) {
                pages += join->slab;
                page->slab += join->slab;

                prev = ngx_slab_page_prev(join);
                prev->next = join->next;
                join->next->prev = join->prev;

                join->slab = NGX_SLAB_PAGE_FREE;
                join->next = रिक्त;
                join->prev = NGX_SLAB_PAGE;
            }
        }
    }

    जर (page > pool->pages) {
        join = page - 1;

        जर (ngx_slab_page_type(join) == NGX_SLAB_PAGE) {

            जर (join->slab == NGX_SLAB_PAGE_FREE) {
                join = ngx_slab_page_prev(join);
            }

            जर (join->next != रिक्त) {
                pages += join->slab;
                join->slab += page->slab;

                prev = ngx_slab_page_prev(join);
                prev->next = join->next;
                join->next->prev = join->prev;

                page->slab = NGX_SLAB_PAGE_FREE;
                page->next = रिक्त;
                page->prev = NGX_SLAB_PAGE;

                page = join;
            }
        }
    }

    जर (pages) {
        page[pages].prev = (uintptr_t) page;
    }

    page->prev = (uintptr_t) &pool->मुक्त;
    page->next = pool->मुक्त.next;

    page->next->prev = (uintptr_t) page;

    pool->मुक्त.next = page;
}


स्थिर शून्य
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, अक्षर *text)
{
    ngx_log_error(level, ngx_cycle->log, 0, "%s%s", text, pool->log_ctx);
}
