
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>


#define NGX_SLAB_PAGE_MASK   3
#define NGX_SLAB_PAGE        0
#define NGX_SLAB_BIG         1
#define NGX_SLAB_EXACT       2
#define NGX_SLAB_SMALL       3

#যদি (NGX_PTR_SIZE == 4)

#define NGX_SLAB_PAGE_FREE   0
#define NGX_SLAB_PAGE_BUSY   0xffffffff
#define NGX_SLAB_PAGE_START  0x80000000

#define NGX_SLAB_SHIFT_MASK  0x0000000f
#define NGX_SLAB_MAP_MASK    0xffff0000
#define NGX_SLAB_MAP_SHIFT   16

#define NGX_SLAB_BUSY        0xffffffff

#অন্যথা /* (NGX_PTR_SIZE == 8) */

#define NGX_SLAB_PAGE_FREE   0
#define NGX_SLAB_PAGE_BUSY   0xffffffffffffffff
#define NGX_SLAB_PAGE_START  0x8000000000000000

#define NGX_SLAB_SHIFT_MASK  0x000000000000000f
#define NGX_SLAB_MAP_MASK    0xffffffff00000000
#define NGX_SLAB_MAP_SHIFT   32

#define NGX_SLAB_BUSY        0xffffffffffffffff

#endif


#define ngx_slab_slots(pool)                                                  \
    (ngx_slab_page_t *) ((u_char *) (pool) + পরিমাণ(ngx_slab_pool_t))

#define ngx_slab_page_type(page)   ((page)->prev & NGX_SLAB_PAGE_MASK)

#define ngx_slab_page_prev(page)                                              \
    (ngx_slab_page_t *) ((page)->prev & ~NGX_SLAB_PAGE_MASK)

#define ngx_slab_page_addr(pool, page)                                        \
    ((((page) - (pool)->pages) << ngx_pagesize_shift)                         \
     + (uintptr_t) (pool)->start)


#যদি (NGX_DEBUG_MALLOC)

#define ngx_slab_junk(p, size)     ngx_memset(p, 0xA5, size)

#নোচেদ্যদি (NGX_HAVE_DEBUG_MALLOC)

#define ngx_slab_junk(p, size)                                                \
    যদি (ngx_debug_malloc)          ngx_memset(p, 0xA5, size)

#অন্যথা

#define ngx_slab_junk(p, size)

#endif

স্থির ngx_slab_page_t *ngx_slab_alloc_pages(ngx_slab_pool_t *pool,
    ngx_uint_t pages);
স্থির শূন্য ngx_slab_free_pages(ngx_slab_pool_t *pool, ngx_slab_page_t *page,
    ngx_uint_t pages);
স্থির শূন্য ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    ৱর্ণ *text);


স্থির ngx_uint_t  ngx_slab_max_size;
স্থির ngx_uint_t  ngx_slab_exact_size;
স্থির ngx_uint_t  ngx_slab_exact_shift;


শূন্য
ngx_slab_sizes_init(শূন্য)
{
    ngx_uint_t  n;

    ngx_slab_max_size = ngx_pagesize / 2;
    ngx_slab_exact_size = ngx_pagesize / (8 * পরিমাণ(uintptr_t));
    বাবে (n = ngx_slab_exact_size; n >>= 1; ngx_slab_exact_shift++) {
        /* void */
    }
}


শূন্য
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

    বাবে (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].next = &slots[i];
        slots[i].prev = 0;
    }

    p += n * পরিমাণ(ngx_slab_page_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * পরিমাণ(ngx_slab_stat_t));

    p += n * পরিমাণ(ngx_slab_stat_t);

    size -= n * (পরিমাণ(ngx_slab_page_t) + পরিমাণ(ngx_slab_stat_t));

    pages = (ngx_uint_t) (size / (ngx_pagesize + পরিমাণ(ngx_slab_page_t)));

    pool->pages = (ngx_slab_page_t *) p;
    ngx_memzero(pool->pages, pages * পরিমাণ(ngx_slab_page_t));

    page = pool->pages;

    /* only "next" is used in list head */
    pool->মুক্ত.slab = 0;
    pool->মুক্ত.next = page;
    pool->মুক্ত.prev = 0;

    page->slab = pages;
    page->next = &pool->মুক্ত;
    page->prev = (uintptr_t) &pool->মুক্ত;

    pool->start = ngx_align_ptr(p + pages * পরিমাণ(ngx_slab_page_t),
                                ngx_pagesize);

    m = pages - (pool->end - pool->start) / ngx_pagesize;
    যদি (m > 0) {
        pages -= m;
        page->slab = pages;
    }

    pool->last = pool->pages + pages;
    pool->pfree = pages;

    pool->log_nomem = 1;
    pool->log_ctx = &pool->zero;
    pool->zero = '\0';
}


শূন্য *
ngx_slab_alloc(ngx_slab_pool_t *pool, size_t size)
{
    শূন্য  *p;

    ngx_shmtx_lock(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, size);

    ngx_shmtx_unlock(&pool->mutex);

    ঘূৰাই p;
}


শূন্য *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, size_t size)
{
    size_t            s;
    uintptr_t         p, m, mask, *bitmap;
    ngx_uint_t        i, n, slot, shift, map;
    ngx_slab_page_t  *page, *prev, *slots;

    যদি (size > ngx_slab_max_size) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                       "slab alloc: %uz", size);

        page = ngx_slab_alloc_pages(pool, (size >> ngx_pagesize_shift)
                                          + ((size % ngx_pagesize) ? 1 : 0));
        যদি (page) {
            p = ngx_slab_page_addr(pool, page);

        } অন্যথা {
            p = 0;
        }

        গচ্ছ done;
    }

    যদি (size > pool->min_size) {
        shift = 1;
        বাবে (s = size - 1; s >>= 1; shift++) { /* void */ }
        slot = shift - pool->min_shift;

    } অন্যথা {
        shift = pool->min_shift;
        slot = 0;
    }

    pool->stats[slot].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                   "slab alloc: %uz slot: %ui", size, slot);

    slots = ngx_slab_slots(pool);
    page = slots[slot].next;

    যদি (page->next != page) {

        যদি (shift < ngx_slab_exact_shift) {

            bitmap = (uintptr_t *) ngx_slab_page_addr(pool, page);

            map = (ngx_pagesize >> shift) / (8 * পরিমাণ(uintptr_t));

            বাবে (n = 0; n < map; n++) {

                যদি (bitmap[n] != NGX_SLAB_BUSY) {

                    বাবে (m = 1, i = 0; m; m <<= 1, i++) {
                        যদি (bitmap[n] & m) {
                            অনুৱর্ত;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * পরিমাণ(uintptr_t) + i) << shift;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[slot].used++;

                        যদি (bitmap[n] == NGX_SLAB_BUSY) {
                            বাবে (n = n + 1; n < map; n++) {
                                যদি (bitmap[n] != NGX_SLAB_BUSY) {
                                    গচ্ছ done;
                                }
                            }

                            prev = ngx_slab_page_prev(page);
                            prev->next = page->next;
                            page->next->prev = page->prev;

                            page->next = রিক্ত;
                            page->prev = NGX_SLAB_SMALL;
                        }

                        গচ্ছ done;
                    }
                }
            }

        } অন্যথা যদি (shift == ngx_slab_exact_shift) {

            বাবে (m = 1, i = 0; m; m <<= 1, i++) {
                যদি (page->slab & m) {
                    অনুৱর্ত;
                }

                page->slab |= m;

                যদি (page->slab == NGX_SLAB_BUSY) {
                    prev = ngx_slab_page_prev(page);
                    prev->next = page->next;
                    page->next->prev = page->prev;

                    page->next = রিক্ত;
                    page->prev = NGX_SLAB_EXACT;
                }

                p = ngx_slab_page_addr(pool, page) + (i << shift);

                pool->stats[slot].used++;

                গচ্ছ done;
            }

        } অন্যথা { /* shift > ngx_slab_exact_shift */

            mask = ((uintptr_t) 1 << (ngx_pagesize >> shift)) - 1;
            mask <<= NGX_SLAB_MAP_SHIFT;

            বাবে (m = (uintptr_t) 1 << NGX_SLAB_MAP_SHIFT, i = 0;
                 m & mask;
                 m <<= 1, i++)
            {
                যদি (page->slab & m) {
                    অনুৱর্ত;
                }

                page->slab |= m;

                যদি ((page->slab & NGX_SLAB_MAP_MASK) == mask) {
                    prev = ngx_slab_page_prev(page);
                    prev->next = page->next;
                    page->next->prev = page->prev;

                    page->next = রিক্ত;
                    page->prev = NGX_SLAB_BIG;
                }

                p = ngx_slab_page_addr(pool, page) + (i << shift);

                pool->stats[slot].used++;

                গচ্ছ done;
            }
        }

        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_alloc(): page is busy");
        ngx_debug_point();
    }

    page = ngx_slab_alloc_pages(pool, 1);

    যদি (page) {
        যদি (shift < ngx_slab_exact_shift) {
            bitmap = (uintptr_t *) ngx_slab_page_addr(pool, page);

            n = (ngx_pagesize >> shift) / ((1 << shift) * 8);

            যদি (n == 0) {
                n = 1;
            }

            /* "n" elements for bitmap, plus one requested */

            বাবে (i = 0; i < (n + 1) / (8 * পরিমাণ(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * পরিমাণ(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> shift) / (8 * পরিমাণ(uintptr_t));

            বাবে (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            page->slab = shift;
            page->next = &slots[slot];
            page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_SMALL;

            slots[slot].next = page;

            pool->stats[slot].total += (ngx_pagesize >> shift) - n;

            p = ngx_slab_page_addr(pool, page) + (n << shift);

            pool->stats[slot].used++;

            গচ্ছ done;

        } অন্যথা যদি (shift == ngx_slab_exact_shift) {

            page->slab = 1;
            page->next = &slots[slot];
            page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_EXACT;

            slots[slot].next = page;

            pool->stats[slot].total += 8 * পরিমাণ(uintptr_t);

            p = ngx_slab_page_addr(pool, page);

            pool->stats[slot].used++;

            গচ্ছ done;

        } অন্যথা { /* shift > ngx_slab_exact_shift */

            page->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_SHIFT) | shift;
            page->next = &slots[slot];
            page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_BIG;

            slots[slot].next = page;

            pool->stats[slot].total += ngx_pagesize >> shift;

            p = ngx_slab_page_addr(pool, page);

            pool->stats[slot].used++;

            গচ্ছ done;
        }
    }

    p = 0;

    pool->stats[slot].fails++;

done:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                   "slab alloc: %p", (শূন্য *) p);

    ঘূৰাই (শূন্য *) p;
}


শূন্য *
ngx_slab_calloc(ngx_slab_pool_t *pool, size_t size)
{
    শূন্য  *p;

    ngx_shmtx_lock(&pool->mutex);

    p = ngx_slab_calloc_locked(pool, size);

    ngx_shmtx_unlock(&pool->mutex);

    ঘূৰাই p;
}


শূন্য *
ngx_slab_calloc_locked(ngx_slab_pool_t *pool, size_t size)
{
    শূন্য  *p;

    p = ngx_slab_alloc_locked(pool, size);
    যদি (p) {
        ngx_memzero(p, size);
    }

    ঘূৰাই p;
}


শূন্য
ngx_slab_free(ngx_slab_pool_t *pool, শূন্য *p)
{
    ngx_shmtx_lock(&pool->mutex);

    ngx_slab_free_locked(pool, p);

    ngx_shmtx_unlock(&pool->mutex);
}


শূন্য
ngx_slab_free_locked(ngx_slab_pool_t *pool, শূন্য *p)
{
    size_t            size;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, slot, shift, map;
    ngx_slab_page_t  *slots, *page;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0, "slab free: %p", p);

    যদি ((u_char *) p < pool->start || (u_char *) p > pool->end) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        গচ্ছ fail;
    }

    n = ((u_char *) p - pool->start) >> ngx_pagesize_shift;
    page = &pool->pages[n];
    slab = page->slab;
    type = ngx_slab_page_type(page);

    ৱিকল্প (type) {

    পক্ষ NGX_SLAB_SMALL:

        shift = slab & NGX_SLAB_SHIFT_MASK;
        size = (size_t) 1 << shift;

        যদি ((uintptr_t) p & (size - 1)) {
            গচ্ছ wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> shift;
        m = (uintptr_t) 1 << (n % (8 * পরিমাণ(uintptr_t)));
        n /= 8 * পরিমাণ(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        যদি (bitmap[n] & m) {
            slot = shift - pool->min_shift;

            যদি (page->next == রিক্ত) {
                slots = ngx_slab_slots(pool);

                page->next = slots[slot].next;
                slots[slot].next = page;

                page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_SMALL;
                page->next->prev = (uintptr_t) page | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> shift) / ((1 << shift) * 8);

            যদি (n == 0) {
                n = 1;
            }

            i = n / (8 * পরিমাণ(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * পরিমাণ(uintptr_t)))) - 1;

            যদি (bitmap[i] & ~m) {
                গচ্ছ done;
            }

            map = (ngx_pagesize >> shift) / (8 * পরিমাণ(uintptr_t));

            বাবে (i = i + 1; i < map; i++) {
                যদি (bitmap[i]) {
                    গচ্ছ done;
                }
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].total -= (ngx_pagesize >> shift) - n;

            গচ্ছ done;
        }

        গচ্ছ chunk_already_free;

    পক্ষ NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_shift);
        size = ngx_slab_exact_size;

        যদি ((uintptr_t) p & (size - 1)) {
            গচ্ছ wrong_chunk;
        }

        যদি (slab & m) {
            slot = ngx_slab_exact_shift - pool->min_shift;

            যদি (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                page->next = slots[slot].next;
                slots[slot].next = page;

                page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_EXACT;
                page->next->prev = (uintptr_t) page | NGX_SLAB_EXACT;
            }

            page->slab &= ~m;

            যদি (page->slab) {
                গচ্ছ done;
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].total -= 8 * পরিমাণ(uintptr_t);

            গচ্ছ done;
        }

        গচ্ছ chunk_already_free;

    পক্ষ NGX_SLAB_BIG:

        shift = slab & NGX_SLAB_SHIFT_MASK;
        size = (size_t) 1 << shift;

        যদি ((uintptr_t) p & (size - 1)) {
            গচ্ছ wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> shift)
                              + NGX_SLAB_MAP_SHIFT);

        যদি (slab & m) {
            slot = shift - pool->min_shift;

            যদি (page->next == রিক্ত) {
                slots = ngx_slab_slots(pool);

                page->next = slots[slot].next;
                slots[slot].next = page;

                page->prev = (uintptr_t) &slots[slot] | NGX_SLAB_BIG;
                page->next->prev = (uintptr_t) page | NGX_SLAB_BIG;
            }

            page->slab &= ~m;

            যদি (page->slab & NGX_SLAB_MAP_MASK) {
                গচ্ছ done;
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].total -= ngx_pagesize >> shift;

            গচ্ছ done;
        }

        গচ্ছ chunk_already_free;

    পক্ষ NGX_SLAB_PAGE:

        যদি ((uintptr_t) p & (ngx_pagesize - 1)) {
            গচ্ছ wrong_chunk;
        }

        যদি (!(slab & NGX_SLAB_PAGE_START)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            গচ্ছ fail;
        }

        যদি (slab == NGX_SLAB_PAGE_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            গচ্ছ fail;
        }

        size = slab & ~NGX_SLAB_PAGE_START;

        ngx_slab_free_pages(pool, page, size);

        ngx_slab_junk(p, size << ngx_pagesize_shift);

        ঘূৰাই;
    }

    /* not reached */

    ঘূৰাই;

done:

    pool->stats[slot].used--;

    ngx_slab_junk(p, size);

    ঘূৰাই;

wrong_chunk:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): pointer to wrong chunk");

    গচ্ছ fail;

chunk_already_free:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): chunk is already free");

fail:

    ঘূৰাই;
}


স্থির ngx_slab_page_t *
ngx_slab_alloc_pages(ngx_slab_pool_t *pool, ngx_uint_t pages)
{
    ngx_slab_page_t  *page, *p;

    বাবে (page = pool->মুক্ত.next; page != &pool->মুক্ত; page = page->next) {

        যদি (page->slab >= pages) {

            যদি (page->slab > pages) {
                page[page->slab - 1].prev = (uintptr_t) &page[pages];

                page[pages].slab = page->slab - pages;
                page[pages].next = page->next;
                page[pages].prev = page->prev;

                p = (ngx_slab_page_t *) page->prev;
                p->next = &page[pages];
                page->next->prev = (uintptr_t) &page[pages];

            } অন্যথা {
                p = (ngx_slab_page_t *) page->prev;
                p->next = page->next;
                page->next->prev = page->prev;
            }

            page->slab = pages | NGX_SLAB_PAGE_START;
            page->next = রিক্ত;
            page->prev = NGX_SLAB_PAGE;

            pool->pfree -= pages;

            যদি (--pages == 0) {
                ঘূৰাই page;
            }

            বাবে (p = page + 1; pages; pages--) {
                p->slab = NGX_SLAB_PAGE_BUSY;
                p->next = রিক্ত;
                p->prev = NGX_SLAB_PAGE;
                p++;
            }

            ঘূৰাই page;
        }
    }

    যদি (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    ঘূৰাই রিক্ত;
}


স্থির শূন্য
ngx_slab_free_pages(ngx_slab_pool_t *pool, ngx_slab_page_t *page,
    ngx_uint_t pages)
{
    ngx_slab_page_t  *prev, *join;

    pool->pfree += pages;

    page->slab = pages--;

    যদি (pages) {
        ngx_memzero(&page[1], pages * পরিমাণ(ngx_slab_page_t));
    }

    যদি (page->next) {
        prev = ngx_slab_page_prev(page);
        prev->next = page->next;
        page->next->prev = page->prev;
    }

    join = page + page->slab;

    যদি (join < pool->last) {

        যদি (ngx_slab_page_type(join) == NGX_SLAB_PAGE) {

            যদি (join->next != রিক্ত) {
                pages += join->slab;
                page->slab += join->slab;

                prev = ngx_slab_page_prev(join);
                prev->next = join->next;
                join->next->prev = join->prev;

                join->slab = NGX_SLAB_PAGE_FREE;
                join->next = রিক্ত;
                join->prev = NGX_SLAB_PAGE;
            }
        }
    }

    যদি (page > pool->pages) {
        join = page - 1;

        যদি (ngx_slab_page_type(join) == NGX_SLAB_PAGE) {

            যদি (join->slab == NGX_SLAB_PAGE_FREE) {
                join = ngx_slab_page_prev(join);
            }

            যদি (join->next != রিক্ত) {
                pages += join->slab;
                join->slab += page->slab;

                prev = ngx_slab_page_prev(join);
                prev->next = join->next;
                join->next->prev = join->prev;

                page->slab = NGX_SLAB_PAGE_FREE;
                page->next = রিক্ত;
                page->prev = NGX_SLAB_PAGE;

                page = join;
            }
        }
    }

    যদি (pages) {
        page[pages].prev = (uintptr_t) page;
    }

    page->prev = (uintptr_t) &pool->মুক্ত;
    page->next = pool->মুক্ত.next;

    page->next->prev = (uintptr_t) page;

    pool->মুক্ত.next = page;
}


স্থির শূন্য
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, ৱর্ণ *text)
{
    ngx_log_error(level, ngx_cycle->log, 0, "%s%s", text, pool->log_ctx);
}
