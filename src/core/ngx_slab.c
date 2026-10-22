
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


#define NGX_SLAB_PAGE_MASK   3
#define NGX_SLAB_PAGE        0
#define NGX_SLAB_BIG         1
#define NGX_SLAB_EXACT       2
#define NGX_SLAB_SMALL       3

#যদি (NGX_PTR_আকার == 4)

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
    (ngx_slab_page_t *) ((u_char *) (pool) + আকার(ngx_slab_pool_t))

#define ngx_slab_page_type(page)   ((page)->পূর্ব & NGX_SLAB_PAGE_MASK)

#define ngx_slab_page_পূর্ব(page)                                              \
    (ngx_slab_page_t *) ((page)->পূর্ব & ~NGX_SLAB_PAGE_MASK)

#define ngx_slab_page_addr(pool, page)                                        \
    ((((page) - (pool)->pages) << ngx_pagesize_shift)                         \
     + (uintptr_t) (pool)->start)


#যদি (NGX_DEBUG_MALLOC)

#define ngx_slab_junk(p, আকার_2)     ngx_memset(p, 0xA5, আকার_2)

#নতুবা_যদি (NGX_HAVE_DEBUG_MALLOC)

#define ngx_slab_junk(p, আকার_2)                                                \
    যদি (ngx_debug_malloc)          ngx_memset(p, 0xA5, আকার_2)

#অন্যথা

#define ngx_slab_junk(p, আকার_2)

#endif

স্থির ngx_slab_page_t *ngx_slab_alloc_pages(ngx_slab_pool_t *pool,
    ngx_uint_t pages);
স্থির শূন্য ngx_slab_free_pages(ngx_slab_pool_t *pool, ngx_slab_page_t *page,
    ngx_uint_t pages);
স্থির শূন্য ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    অক্ষর *text);


স্থির ngx_uint_t  ngx_slab_max_আকার;
স্থির ngx_uint_t  ngx_slab_exact_আকার;
স্থির ngx_uint_t  ngx_slab_exact_shift;


শূন্য
ngx_slab_sizes_init(শূন্য)
{
    ngx_uint_t  n;

    ngx_slab_max_আকার = ngx_pagesize / 2;
    ngx_slab_exact_আকার = ngx_pagesize / (8 * আকার(uintptr_t));
    জন্য (n = ngx_slab_exact_আকার; n >>= 1; ngx_slab_exact_shift++) {
        /* void */
    }
}


শূন্য
ngx_slab_init(ngx_slab_pool_t *pool)
{
    u_char           *p;
    আকার_t            আকার_2;
    ngx_int_t         m;
    ngx_uint_t        i, n, pages;
    ngx_slab_page_t  *slots, *page;

    pool->min_আকার = (আকার_t) 1 << pool->min_shift;

    slots = ngx_slab_slots(pool);

    p = (u_char *) slots;
    আকার_2 = pool->end - p;

    ngx_slab_junk(p, আকার_2);

    n = ngx_pagesize_shift - pool->min_shift;

    জন্য (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].পরবর্তী = &slots[i];
        slots[i].পূর্ব = 0;
    }

    p += n * আকার(ngx_slab_page_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * আকার(ngx_slab_stat_t));

    p += n * আকার(ngx_slab_stat_t);

    আকার_2 -= n * (আকার(ngx_slab_page_t) + আকার(ngx_slab_stat_t));

    pages = (ngx_uint_t) (আকার_2 / (ngx_pagesize + আকার(ngx_slab_page_t)));

    pool->pages = (ngx_slab_page_t *) p;
    ngx_memzero(pool->pages, pages * আকার(ngx_slab_page_t));

    page = pool->pages;

    /* only "next" is used in list head */
    pool->মুক্ত.slab = 0;
    pool->মুক্ত.পরবর্তী = page;
    pool->মুক্ত.পূর্ব = 0;

    page->slab = pages;
    page->পরবর্তী = &pool->মুক্ত;
    page->পূর্ব = (uintptr_t) &pool->মুক্ত;

    pool->start = ngx_align_ptr(p + pages * আকার(ngx_slab_page_t),
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
ngx_slab_alloc(ngx_slab_pool_t *pool, আকার_t আকার_2)
{
    শূন্য  *p;

    ngx_shmtx_lock(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, আকার_2);

    ngx_shmtx_unlock(&pool->mutex);

    ফেরত p;
}


শূন্য *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, আকার_t আকার_2)
{
    আকার_t            s;
    uintptr_t         p, m, mask, *bitmap;
    ngx_uint_t        i, n, slot, shift, map;
    ngx_slab_page_t  *page, *পূর্ব, *slots;

    যদি (আকার_2 > ngx_slab_max_আকার) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                       "slab alloc: %uz", আকার_2);

        page = ngx_slab_alloc_pages(pool, (আকার_2 >> ngx_pagesize_shift)
                                          + ((আকার_2 % ngx_pagesize) ? 1 : 0));
        যদি (page) {
            p = ngx_slab_page_addr(pool, page);

        } অন্যথা {
            p = 0;
        }

        যাও done;
    }

    যদি (আকার_2 > pool->min_আকার) {
        shift = 1;
        জন্য (s = আকার_2 - 1; s >>= 1; shift++) { /* void */ }
        slot = shift - pool->min_shift;

    } অন্যথা {
        shift = pool->min_shift;
        slot = 0;
    }

    pool->stats[slot].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                   "slab alloc: %uz slot: %ui", আকার_2, slot);

    slots = ngx_slab_slots(pool);
    page = slots[slot].পরবর্তী;

    যদি (page->পরবর্তী != page) {

        যদি (shift < ngx_slab_exact_shift) {

            bitmap = (uintptr_t *) ngx_slab_page_addr(pool, page);

            map = (ngx_pagesize >> shift) / (8 * আকার(uintptr_t));

            জন্য (n = 0; n < map; n++) {

                যদি (bitmap[n] != NGX_SLAB_BUSY) {

                    জন্য (m = 1, i = 0; m; m <<= 1, i++) {
                        যদি (bitmap[n] & m) {
                            চালু;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * আকার(uintptr_t) + i) << shift;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[slot].used++;

                        যদি (bitmap[n] == NGX_SLAB_BUSY) {
                            জন্য (n = n + 1; n < map; n++) {
                                যদি (bitmap[n] != NGX_SLAB_BUSY) {
                                    যাও done;
                                }
                            }

                            পূর্ব = ngx_slab_page_পূর্ব(page);
                            পূর্ব->পরবর্তী = page->পরবর্তী;
                            page->পরবর্তী->পূর্ব = page->পূর্ব;

                            page->পরবর্তী = ফাঁকা;
                            page->পূর্ব = NGX_SLAB_SMALL;
                        }

                        যাও done;
                    }
                }
            }

        } অন্যথা যদি (shift == ngx_slab_exact_shift) {

            জন্য (m = 1, i = 0; m; m <<= 1, i++) {
                যদি (page->slab & m) {
                    চালু;
                }

                page->slab |= m;

                যদি (page->slab == NGX_SLAB_BUSY) {
                    পূর্ব = ngx_slab_page_পূর্ব(page);
                    পূর্ব->পরবর্তী = page->পরবর্তী;
                    page->পরবর্তী->পূর্ব = page->পূর্ব;

                    page->পরবর্তী = ফাঁকা;
                    page->পূর্ব = NGX_SLAB_EXACT;
                }

                p = ngx_slab_page_addr(pool, page) + (i << shift);

                pool->stats[slot].used++;

                যাও done;
            }

        } অন্যথা { /* shift > ngx_slab_exact_shift */

            mask = ((uintptr_t) 1 << (ngx_pagesize >> shift)) - 1;
            mask <<= NGX_SLAB_MAP_SHIFT;

            জন্য (m = (uintptr_t) 1 << NGX_SLAB_MAP_SHIFT, i = 0;
                 m & mask;
                 m <<= 1, i++)
            {
                যদি (page->slab & m) {
                    চালু;
                }

                page->slab |= m;

                যদি ((page->slab & NGX_SLAB_MAP_MASK) == mask) {
                    পূর্ব = ngx_slab_page_পূর্ব(page);
                    পূর্ব->পরবর্তী = page->পরবর্তী;
                    page->পরবর্তী->পূর্ব = page->পূর্ব;

                    page->পরবর্তী = ফাঁকা;
                    page->পূর্ব = NGX_SLAB_BIG;
                }

                p = ngx_slab_page_addr(pool, page) + (i << shift);

                pool->stats[slot].used++;

                যাও done;
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

            জন্য (i = 0; i < (n + 1) / (8 * আকার(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * আকার(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> shift) / (8 * আকার(uintptr_t));

            জন্য (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            page->slab = shift;
            page->পরবর্তী = &slots[slot];
            page->পূর্ব = (uintptr_t) &slots[slot] | NGX_SLAB_SMALL;

            slots[slot].পরবর্তী = page;

            pool->stats[slot].যোগ += (ngx_pagesize >> shift) - n;

            p = ngx_slab_page_addr(pool, page) + (n << shift);

            pool->stats[slot].used++;

            যাও done;

        } অন্যথা যদি (shift == ngx_slab_exact_shift) {

            page->slab = 1;
            page->পরবর্তী = &slots[slot];
            page->পূর্ব = (uintptr_t) &slots[slot] | NGX_SLAB_EXACT;

            slots[slot].পরবর্তী = page;

            pool->stats[slot].যোগ += 8 * আকার(uintptr_t);

            p = ngx_slab_page_addr(pool, page);

            pool->stats[slot].used++;

            যাও done;

        } অন্যথা { /* shift > ngx_slab_exact_shift */

            page->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_SHIFT) | shift;
            page->পরবর্তী = &slots[slot];
            page->পূর্ব = (uintptr_t) &slots[slot] | NGX_SLAB_BIG;

            slots[slot].পরবর্তী = page;

            pool->stats[slot].যোগ += ngx_pagesize >> shift;

            p = ngx_slab_page_addr(pool, page);

            pool->stats[slot].used++;

            যাও done;
        }
    }

    p = 0;

    pool->stats[slot].fails++;

done:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                   "slab alloc: %p", (শূন্য *) p);

    ফেরত (শূন্য *) p;
}


শূন্য *
ngx_slab_calloc(ngx_slab_pool_t *pool, আকার_t আকার_2)
{
    শূন্য  *p;

    ngx_shmtx_lock(&pool->mutex);

    p = ngx_slab_calloc_locked(pool, আকার_2);

    ngx_shmtx_unlock(&pool->mutex);

    ফেরত p;
}


শূন্য *
ngx_slab_calloc_locked(ngx_slab_pool_t *pool, আকার_t আকার_2)
{
    শূন্য  *p;

    p = ngx_slab_alloc_locked(pool, আকার_2);
    যদি (p) {
        ngx_memzero(p, আকার_2);
    }

    ফেরত p;
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
    আকার_t            আকার_2;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, slot, shift, map;
    ngx_slab_page_t  *slots, *page;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0, "slab free: %p", p);

    যদি ((u_char *) p < pool->start || (u_char *) p > pool->end) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        যাও fail;
    }

    n = ((u_char *) p - pool->start) >> ngx_pagesize_shift;
    page = &pool->pages[n];
    slab = page->slab;
    type = ngx_slab_page_type(page);

    নির্বাচন (type) {

    ক্ষেত্র NGX_SLAB_SMALL:

        shift = slab & NGX_SLAB_SHIFT_MASK;
        আকার_2 = (আকার_t) 1 << shift;

        যদি ((uintptr_t) p & (আকার_2 - 1)) {
            যাও wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> shift;
        m = (uintptr_t) 1 << (n % (8 * আকার(uintptr_t)));
        n /= 8 * আকার(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        যদি (bitmap[n] & m) {
            slot = shift - pool->min_shift;

            যদি (page->পরবর্তী == ফাঁকা) {
                slots = ngx_slab_slots(pool);

                page->পরবর্তী = slots[slot].পরবর্তী;
                slots[slot].পরবর্তী = page;

                page->পূর্ব = (uintptr_t) &slots[slot] | NGX_SLAB_SMALL;
                page->পরবর্তী->পূর্ব = (uintptr_t) page | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> shift) / ((1 << shift) * 8);

            যদি (n == 0) {
                n = 1;
            }

            i = n / (8 * আকার(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * আকার(uintptr_t)))) - 1;

            যদি (bitmap[i] & ~m) {
                যাও done;
            }

            map = (ngx_pagesize >> shift) / (8 * আকার(uintptr_t));

            জন্য (i = i + 1; i < map; i++) {
                যদি (bitmap[i]) {
                    যাও done;
                }
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].যোগ -= (ngx_pagesize >> shift) - n;

            যাও done;
        }

        যাও chunk_already_free;

    ক্ষেত্র NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_shift);
        আকার_2 = ngx_slab_exact_আকার;

        যদি ((uintptr_t) p & (আকার_2 - 1)) {
            যাও wrong_chunk;
        }

        যদি (slab & m) {
            slot = ngx_slab_exact_shift - pool->min_shift;

            যদি (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                page->পরবর্তী = slots[slot].পরবর্তী;
                slots[slot].পরবর্তী = page;

                page->পূর্ব = (uintptr_t) &slots[slot] | NGX_SLAB_EXACT;
                page->পরবর্তী->পূর্ব = (uintptr_t) page | NGX_SLAB_EXACT;
            }

            page->slab &= ~m;

            যদি (page->slab) {
                যাও done;
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].যোগ -= 8 * আকার(uintptr_t);

            যাও done;
        }

        যাও chunk_already_free;

    ক্ষেত্র NGX_SLAB_BIG:

        shift = slab & NGX_SLAB_SHIFT_MASK;
        আকার_2 = (আকার_t) 1 << shift;

        যদি ((uintptr_t) p & (আকার_2 - 1)) {
            যাও wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> shift)
                              + NGX_SLAB_MAP_SHIFT);

        যদি (slab & m) {
            slot = shift - pool->min_shift;

            যদি (page->পরবর্তী == ফাঁকা) {
                slots = ngx_slab_slots(pool);

                page->পরবর্তী = slots[slot].পরবর্তী;
                slots[slot].পরবর্তী = page;

                page->পূর্ব = (uintptr_t) &slots[slot] | NGX_SLAB_BIG;
                page->পরবর্তী->পূর্ব = (uintptr_t) page | NGX_SLAB_BIG;
            }

            page->slab &= ~m;

            যদি (page->slab & NGX_SLAB_MAP_MASK) {
                যাও done;
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].যোগ -= ngx_pagesize >> shift;

            যাও done;
        }

        যাও chunk_already_free;

    ক্ষেত্র NGX_SLAB_PAGE:

        যদি ((uintptr_t) p & (ngx_pagesize - 1)) {
            যাও wrong_chunk;
        }

        যদি (!(slab & NGX_SLAB_PAGE_START)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            যাও fail;
        }

        যদি (slab == NGX_SLAB_PAGE_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            যাও fail;
        }

        আকার_2 = slab & ~NGX_SLAB_PAGE_START;

        ngx_slab_free_pages(pool, page, আকার_2);

        ngx_slab_junk(p, আকার_2 << ngx_pagesize_shift);

        ফেরত;
    }

    /* not reached */

    ফেরত;

done:

    pool->stats[slot].used--;

    ngx_slab_junk(p, আকার_2);

    ফেরত;

wrong_chunk:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): pointer to wrong chunk");

    যাও fail;

chunk_already_free:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): chunk is already free");

fail:

    ফেরত;
}


স্থির ngx_slab_page_t *
ngx_slab_alloc_pages(ngx_slab_pool_t *pool, ngx_uint_t pages)
{
    ngx_slab_page_t  *page, *p;

    জন্য (page = pool->মুক্ত.পরবর্তী; page != &pool->মুক্ত; page = page->পরবর্তী) {

        যদি (page->slab >= pages) {

            যদি (page->slab > pages) {
                page[page->slab - 1].পূর্ব = (uintptr_t) &page[pages];

                page[pages].slab = page->slab - pages;
                page[pages].পরবর্তী = page->পরবর্তী;
                page[pages].পূর্ব = page->পূর্ব;

                p = (ngx_slab_page_t *) page->পূর্ব;
                p->পরবর্তী = &page[pages];
                page->পরবর্তী->পূর্ব = (uintptr_t) &page[pages];

            } অন্যথা {
                p = (ngx_slab_page_t *) page->পূর্ব;
                p->পরবর্তী = page->পরবর্তী;
                page->পরবর্তী->পূর্ব = page->পূর্ব;
            }

            page->slab = pages | NGX_SLAB_PAGE_START;
            page->পরবর্তী = ফাঁকা;
            page->পূর্ব = NGX_SLAB_PAGE;

            pool->pfree -= pages;

            যদি (--pages == 0) {
                ফেরত page;
            }

            জন্য (p = page + 1; pages; pages--) {
                p->slab = NGX_SLAB_PAGE_BUSY;
                p->পরবর্তী = ফাঁকা;
                p->পূর্ব = NGX_SLAB_PAGE;
                p++;
            }

            ফেরত page;
        }
    }

    যদি (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    ফেরত ফাঁকা;
}


স্থির শূন্য
ngx_slab_free_pages(ngx_slab_pool_t *pool, ngx_slab_page_t *page,
    ngx_uint_t pages)
{
    ngx_slab_page_t  *পূর্ব, *join;

    pool->pfree += pages;

    page->slab = pages--;

    যদি (pages) {
        ngx_memzero(&page[1], pages * আকার(ngx_slab_page_t));
    }

    যদি (page->পরবর্তী) {
        পূর্ব = ngx_slab_page_পূর্ব(page);
        পূর্ব->পরবর্তী = page->পরবর্তী;
        page->পরবর্তী->পূর্ব = page->পূর্ব;
    }

    join = page + page->slab;

    যদি (join < pool->last) {

        যদি (ngx_slab_page_type(join) == NGX_SLAB_PAGE) {

            যদি (join->পরবর্তী != ফাঁকা) {
                pages += join->slab;
                page->slab += join->slab;

                পূর্ব = ngx_slab_page_পূর্ব(join);
                পূর্ব->পরবর্তী = join->পরবর্তী;
                join->পরবর্তী->পূর্ব = join->পূর্ব;

                join->slab = NGX_SLAB_PAGE_FREE;
                join->পরবর্তী = ফাঁকা;
                join->পূর্ব = NGX_SLAB_PAGE;
            }
        }
    }

    যদি (page > pool->pages) {
        join = page - 1;

        যদি (ngx_slab_page_type(join) == NGX_SLAB_PAGE) {

            যদি (join->slab == NGX_SLAB_PAGE_FREE) {
                join = ngx_slab_page_পূর্ব(join);
            }

            যদি (join->পরবর্তী != ফাঁকা) {
                pages += join->slab;
                join->slab += page->slab;

                পূর্ব = ngx_slab_page_পূর্ব(join);
                পূর্ব->পরবর্তী = join->পরবর্তী;
                join->পরবর্তী->পূর্ব = join->পূর্ব;

                page->slab = NGX_SLAB_PAGE_FREE;
                page->পরবর্তী = ফাঁকা;
                page->পূর্ব = NGX_SLAB_PAGE;

                page = join;
            }
        }
    }

    যদি (pages) {
        page[pages].পূর্ব = (uintptr_t) page;
    }

    page->পূর্ব = (uintptr_t) &pool->মুক্ত;
    page->পরবর্তী = pool->মুক্ত.পরবর্তী;

    page->পরবর্তী->পূর্ব = (uintptr_t) page;

    pool->মুক্ত.পরবর্তী = page;
}


স্থির শূন্য
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, অক্ষর *text)
{
    ngx_log_error(level, ngx_cycle->log, 0, "%s%s", text, pool->log_ctx);
}
