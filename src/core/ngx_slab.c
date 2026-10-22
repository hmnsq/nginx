
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


#घोषणा NGX_SLAB_PAGE_MASK   3
#घोषणा NGX_SLAB_PAGE        0
#घोषणा NGX_SLAB_BIG         1
#घोषणा NGX_SLAB_EXACT       2
#घोषणा NGX_SLAB_SMALL       3

#अगर (NGX_PTR_आकार == 4)

#घोषणा NGX_SLAB_PAGE_FREE   0
#घोषणा NGX_SLAB_PAGE_BUSY   0xffffffff
#घोषणा NGX_SLAB_PAGE_START  0x80000000

#घोषणा NGX_SLAB_SHIFT_MASK  0x0000000f
#घोषणा NGX_SLAB_MAP_MASK    0xffff0000
#घोषणा NGX_SLAB_MAP_SHIFT   16

#घोषणा NGX_SLAB_BUSY        0xffffffff

#अन्यथा /* (NGX_PTR_SIZE == 8) */

#घोषणा NGX_SLAB_PAGE_FREE   0
#घोषणा NGX_SLAB_PAGE_BUSY   0xffffffffffffffff
#घोषणा NGX_SLAB_PAGE_START  0x8000000000000000

#घोषणा NGX_SLAB_SHIFT_MASK  0x000000000000000f
#घोषणा NGX_SLAB_MAP_MASK    0xffffffff00000000
#घोषणा NGX_SLAB_MAP_SHIFT   32

#घोषणा NGX_SLAB_BUSY        0xffffffffffffffff

#पूर्ण_अगर


#घोषणा ngx_slab_slots(pool)                                                  \
    (ngx_slab_page_t *) ((u_char *) (pool) + माप(ngx_slab_pool_t))

#घोषणा ngx_slab_page_type(page)   ((page)->पूर्व & NGX_SLAB_PAGE_MASK)

#घोषणा ngx_slab_page_पूर्व(page)                                              \
    (ngx_slab_page_t *) ((page)->पूर्व & ~NGX_SLAB_PAGE_MASK)

#घोषणा ngx_slab_page_addr(pool, page)                                        \
    ((((page) - (pool)->pages) << ngx_pagesize_shift)                         \
     + (uintptr_t) (pool)->start)


#अगर (NGX_DEBUG_MALLOC)

#घोषणा ngx_slab_junk(p, आकार)     ngx_memset(p, 0xA5, आकार)

#या_अगर (NGX_HAVE_DEBUG_MALLOC)

#घोषणा ngx_slab_junk(p, आकार)                                                \
    अगर (ngx_debug_malloc)          ngx_memset(p, 0xA5, आकार)

#अन्यथा

#घोषणा ngx_slab_junk(p, आकार)

#पूर्ण_अगर

अटल ngx_slab_page_t *ngx_slab_alloc_pages(ngx_slab_pool_t *pool,
    ngx_uint_t pages);
अटल व्योम ngx_slab_free_pages(ngx_slab_pool_t *pool, ngx_slab_page_t *page,
    ngx_uint_t pages);
अटल व्योम ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    अक्षर *text);


अटल ngx_uint_t  ngx_slab_max_आकार;
अटल ngx_uint_t  ngx_slab_exact_आकार;
अटल ngx_uint_t  ngx_slab_exact_shift;


व्योम
ngx_slab_sizes_init(व्योम)
{
    ngx_uint_t  n;

    ngx_slab_max_आकार = ngx_pagesize / 2;
    ngx_slab_exact_आकार = ngx_pagesize / (8 * माप(uintptr_t));
    क्रम (n = ngx_slab_exact_आकार; n >>= 1; ngx_slab_exact_shift++) {
        /* void */
    }
}


व्योम
ngx_slab_init(ngx_slab_pool_t *pool)
{
    u_char           *p;
    माप_प्रकार            आकार;
    ngx_int_t         m;
    ngx_uint_t        i, n, pages;
    ngx_slab_page_t  *slots, *page;

    pool->min_आकार = (माप_प्रकार) 1 << pool->min_shift;

    slots = ngx_slab_slots(pool);

    p = (u_char *) slots;
    आकार = pool->end - p;

    ngx_slab_junk(p, आकार);

    n = ngx_pagesize_shift - pool->min_shift;

    क्रम (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].अग्र = &slots[i];
        slots[i].पूर्व = 0;
    }

    p += n * माप(ngx_slab_page_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * माप(ngx_slab_stat_t));

    p += n * माप(ngx_slab_stat_t);

    आकार -= n * (माप(ngx_slab_page_t) + माप(ngx_slab_stat_t));

    pages = (ngx_uint_t) (आकार / (ngx_pagesize + माप(ngx_slab_page_t)));

    pool->pages = (ngx_slab_page_t *) p;
    ngx_memzero(pool->pages, pages * माप(ngx_slab_page_t));

    page = pool->pages;

    /* only "next" is used in list head */
    pool->मुक्त.slab = 0;
    pool->मुक्त.अग्र = page;
    pool->मुक्त.पूर्व = 0;

    page->slab = pages;
    page->अग्र = &pool->मुक्त;
    page->पूर्व = (uintptr_t) &pool->मुक्त;

    pool->start = ngx_align_ptr(p + pages * माप(ngx_slab_page_t),
                                ngx_pagesize);

    m = pages - (pool->end - pool->start) / ngx_pagesize;
    अगर (m > 0) {
        pages -= m;
        page->slab = pages;
    }

    pool->last = pool->pages + pages;
    pool->pfree = pages;

    pool->log_nomem = 1;
    pool->log_ctx = &pool->zero;
    pool->zero = '\0';
}


व्योम *
ngx_slab_alloc(ngx_slab_pool_t *pool, माप_प्रकार आकार)
{
    व्योम  *p;

    ngx_shmtx_lock(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, आकार);

    ngx_shmtx_unlock(&pool->mutex);

    वापस p;
}


व्योम *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, माप_प्रकार आकार)
{
    माप_प्रकार            s;
    uintptr_t         p, m, mask, *bitmap;
    ngx_uint_t        i, n, slot, shift, map;
    ngx_slab_page_t  *page, *पूर्व, *slots;

    अगर (आकार > ngx_slab_max_आकार) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                       "slab alloc: %uz", आकार);

        page = ngx_slab_alloc_pages(pool, (आकार >> ngx_pagesize_shift)
                                          + ((आकार % ngx_pagesize) ? 1 : 0));
        अगर (page) {
            p = ngx_slab_page_addr(pool, page);

        } अन्यथा {
            p = 0;
        }

        जाओ done;
    }

    अगर (आकार > pool->min_आकार) {
        shift = 1;
        क्रम (s = आकार - 1; s >>= 1; shift++) { /* void */ }
        slot = shift - pool->min_shift;

    } अन्यथा {
        shift = pool->min_shift;
        slot = 0;
    }

    pool->stats[slot].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                   "slab alloc: %uz slot: %ui", आकार, slot);

    slots = ngx_slab_slots(pool);
    page = slots[slot].अग्र;

    अगर (page->अग्र != page) {

        अगर (shift < ngx_slab_exact_shift) {

            bitmap = (uintptr_t *) ngx_slab_page_addr(pool, page);

            map = (ngx_pagesize >> shift) / (8 * माप(uintptr_t));

            क्रम (n = 0; n < map; n++) {

                अगर (bitmap[n] != NGX_SLAB_BUSY) {

                    क्रम (m = 1, i = 0; m; m <<= 1, i++) {
                        अगर (bitmap[n] & m) {
                            जारी;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * माप(uintptr_t) + i) << shift;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[slot].used++;

                        अगर (bitmap[n] == NGX_SLAB_BUSY) {
                            क्रम (n = n + 1; n < map; n++) {
                                अगर (bitmap[n] != NGX_SLAB_BUSY) {
                                    जाओ done;
                                }
                            }

                            पूर्व = ngx_slab_page_पूर्व(page);
                            पूर्व->अग्र = page->अग्र;
                            page->अग्र->पूर्व = page->पूर्व;

                            page->अग्र = शून्य;
                            page->पूर्व = NGX_SLAB_SMALL;
                        }

                        जाओ done;
                    }
                }
            }

        } अन्यथा अगर (shift == ngx_slab_exact_shift) {

            क्रम (m = 1, i = 0; m; m <<= 1, i++) {
                अगर (page->slab & m) {
                    जारी;
                }

                page->slab |= m;

                अगर (page->slab == NGX_SLAB_BUSY) {
                    पूर्व = ngx_slab_page_पूर्व(page);
                    पूर्व->अग्र = page->अग्र;
                    page->अग्र->पूर्व = page->पूर्व;

                    page->अग्र = शून्य;
                    page->पूर्व = NGX_SLAB_EXACT;
                }

                p = ngx_slab_page_addr(pool, page) + (i << shift);

                pool->stats[slot].used++;

                जाओ done;
            }

        } अन्यथा { /* shift > ngx_slab_exact_shift */

            mask = ((uintptr_t) 1 << (ngx_pagesize >> shift)) - 1;
            mask <<= NGX_SLAB_MAP_SHIFT;

            क्रम (m = (uintptr_t) 1 << NGX_SLAB_MAP_SHIFT, i = 0;
                 m & mask;
                 m <<= 1, i++)
            {
                अगर (page->slab & m) {
                    जारी;
                }

                page->slab |= m;

                अगर ((page->slab & NGX_SLAB_MAP_MASK) == mask) {
                    पूर्व = ngx_slab_page_पूर्व(page);
                    पूर्व->अग्र = page->अग्र;
                    page->अग्र->पूर्व = page->पूर्व;

                    page->अग्र = शून्य;
                    page->पूर्व = NGX_SLAB_BIG;
                }

                p = ngx_slab_page_addr(pool, page) + (i << shift);

                pool->stats[slot].used++;

                जाओ done;
            }
        }

        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_alloc(): page is busy");
        ngx_debug_point();
    }

    page = ngx_slab_alloc_pages(pool, 1);

    अगर (page) {
        अगर (shift < ngx_slab_exact_shift) {
            bitmap = (uintptr_t *) ngx_slab_page_addr(pool, page);

            n = (ngx_pagesize >> shift) / ((1 << shift) * 8);

            अगर (n == 0) {
                n = 1;
            }

            /* "n" elements for bitmap, plus one requested */

            क्रम (i = 0; i < (n + 1) / (8 * माप(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * माप(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> shift) / (8 * माप(uintptr_t));

            क्रम (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            page->slab = shift;
            page->अग्र = &slots[slot];
            page->पूर्व = (uintptr_t) &slots[slot] | NGX_SLAB_SMALL;

            slots[slot].अग्र = page;

            pool->stats[slot].योग += (ngx_pagesize >> shift) - n;

            p = ngx_slab_page_addr(pool, page) + (n << shift);

            pool->stats[slot].used++;

            जाओ done;

        } अन्यथा अगर (shift == ngx_slab_exact_shift) {

            page->slab = 1;
            page->अग्र = &slots[slot];
            page->पूर्व = (uintptr_t) &slots[slot] | NGX_SLAB_EXACT;

            slots[slot].अग्र = page;

            pool->stats[slot].योग += 8 * माप(uintptr_t);

            p = ngx_slab_page_addr(pool, page);

            pool->stats[slot].used++;

            जाओ done;

        } अन्यथा { /* shift > ngx_slab_exact_shift */

            page->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_SHIFT) | shift;
            page->अग्र = &slots[slot];
            page->पूर्व = (uintptr_t) &slots[slot] | NGX_SLAB_BIG;

            slots[slot].अग्र = page;

            pool->stats[slot].योग += ngx_pagesize >> shift;

            p = ngx_slab_page_addr(pool, page);

            pool->stats[slot].used++;

            जाओ done;
        }
    }

    p = 0;

    pool->stats[slot].fails++;

done:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0,
                   "slab alloc: %p", (व्योम *) p);

    वापस (व्योम *) p;
}


व्योम *
ngx_slab_calloc(ngx_slab_pool_t *pool, माप_प्रकार आकार)
{
    व्योम  *p;

    ngx_shmtx_lock(&pool->mutex);

    p = ngx_slab_calloc_locked(pool, आकार);

    ngx_shmtx_unlock(&pool->mutex);

    वापस p;
}


व्योम *
ngx_slab_calloc_locked(ngx_slab_pool_t *pool, माप_प्रकार आकार)
{
    व्योम  *p;

    p = ngx_slab_alloc_locked(pool, आकार);
    अगर (p) {
        ngx_memzero(p, आकार);
    }

    वापस p;
}


व्योम
ngx_slab_free(ngx_slab_pool_t *pool, व्योम *p)
{
    ngx_shmtx_lock(&pool->mutex);

    ngx_slab_free_locked(pool, p);

    ngx_shmtx_unlock(&pool->mutex);
}


व्योम
ngx_slab_free_locked(ngx_slab_pool_t *pool, व्योम *p)
{
    माप_प्रकार            आकार;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, slot, shift, map;
    ngx_slab_page_t  *slots, *page;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_cycle->log, 0, "slab free: %p", p);

    अगर ((u_char *) p < pool->start || (u_char *) p > pool->end) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        जाओ fail;
    }

    n = ((u_char *) p - pool->start) >> ngx_pagesize_shift;
    page = &pool->pages[n];
    slab = page->slab;
    type = ngx_slab_page_type(page);

    चयन (type) {

    हाल NGX_SLAB_SMALL:

        shift = slab & NGX_SLAB_SHIFT_MASK;
        आकार = (माप_प्रकार) 1 << shift;

        अगर ((uintptr_t) p & (आकार - 1)) {
            जाओ wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> shift;
        m = (uintptr_t) 1 << (n % (8 * माप(uintptr_t)));
        n /= 8 * माप(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        अगर (bitmap[n] & m) {
            slot = shift - pool->min_shift;

            अगर (page->अग्र == शून्य) {
                slots = ngx_slab_slots(pool);

                page->अग्र = slots[slot].अग्र;
                slots[slot].अग्र = page;

                page->पूर्व = (uintptr_t) &slots[slot] | NGX_SLAB_SMALL;
                page->अग्र->पूर्व = (uintptr_t) page | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> shift) / ((1 << shift) * 8);

            अगर (n == 0) {
                n = 1;
            }

            i = n / (8 * माप(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * माप(uintptr_t)))) - 1;

            अगर (bitmap[i] & ~m) {
                जाओ done;
            }

            map = (ngx_pagesize >> shift) / (8 * माप(uintptr_t));

            क्रम (i = i + 1; i < map; i++) {
                अगर (bitmap[i]) {
                    जाओ done;
                }
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].योग -= (ngx_pagesize >> shift) - n;

            जाओ done;
        }

        जाओ chunk_already_free;

    हाल NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_shift);
        आकार = ngx_slab_exact_आकार;

        अगर ((uintptr_t) p & (आकार - 1)) {
            जाओ wrong_chunk;
        }

        अगर (slab & m) {
            slot = ngx_slab_exact_shift - pool->min_shift;

            अगर (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                page->अग्र = slots[slot].अग्र;
                slots[slot].अग्र = page;

                page->पूर्व = (uintptr_t) &slots[slot] | NGX_SLAB_EXACT;
                page->अग्र->पूर्व = (uintptr_t) page | NGX_SLAB_EXACT;
            }

            page->slab &= ~m;

            अगर (page->slab) {
                जाओ done;
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].योग -= 8 * माप(uintptr_t);

            जाओ done;
        }

        जाओ chunk_already_free;

    हाल NGX_SLAB_BIG:

        shift = slab & NGX_SLAB_SHIFT_MASK;
        आकार = (माप_प्रकार) 1 << shift;

        अगर ((uintptr_t) p & (आकार - 1)) {
            जाओ wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> shift)
                              + NGX_SLAB_MAP_SHIFT);

        अगर (slab & m) {
            slot = shift - pool->min_shift;

            अगर (page->अग्र == शून्य) {
                slots = ngx_slab_slots(pool);

                page->अग्र = slots[slot].अग्र;
                slots[slot].अग्र = page;

                page->पूर्व = (uintptr_t) &slots[slot] | NGX_SLAB_BIG;
                page->अग्र->पूर्व = (uintptr_t) page | NGX_SLAB_BIG;
            }

            page->slab &= ~m;

            अगर (page->slab & NGX_SLAB_MAP_MASK) {
                जाओ done;
            }

            ngx_slab_free_pages(pool, page, 1);

            pool->stats[slot].योग -= ngx_pagesize >> shift;

            जाओ done;
        }

        जाओ chunk_already_free;

    हाल NGX_SLAB_PAGE:

        अगर ((uintptr_t) p & (ngx_pagesize - 1)) {
            जाओ wrong_chunk;
        }

        अगर (!(slab & NGX_SLAB_PAGE_START)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            जाओ fail;
        }

        अगर (slab == NGX_SLAB_PAGE_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            जाओ fail;
        }

        आकार = slab & ~NGX_SLAB_PAGE_START;

        ngx_slab_free_pages(pool, page, आकार);

        ngx_slab_junk(p, आकार << ngx_pagesize_shift);

        वापस;
    }

    /* not reached */

    वापस;

done:

    pool->stats[slot].used--;

    ngx_slab_junk(p, आकार);

    वापस;

wrong_chunk:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): pointer to wrong chunk");

    जाओ fail;

chunk_already_free:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): chunk is already free");

fail:

    वापस;
}


अटल ngx_slab_page_t *
ngx_slab_alloc_pages(ngx_slab_pool_t *pool, ngx_uint_t pages)
{
    ngx_slab_page_t  *page, *p;

    क्रम (page = pool->मुक्त.अग्र; page != &pool->मुक्त; page = page->अग्र) {

        अगर (page->slab >= pages) {

            अगर (page->slab > pages) {
                page[page->slab - 1].पूर्व = (uintptr_t) &page[pages];

                page[pages].slab = page->slab - pages;
                page[pages].अग्र = page->अग्र;
                page[pages].पूर्व = page->पूर्व;

                p = (ngx_slab_page_t *) page->पूर्व;
                p->अग्र = &page[pages];
                page->अग्र->पूर्व = (uintptr_t) &page[pages];

            } अन्यथा {
                p = (ngx_slab_page_t *) page->पूर्व;
                p->अग्र = page->अग्र;
                page->अग्र->पूर्व = page->पूर्व;
            }

            page->slab = pages | NGX_SLAB_PAGE_START;
            page->अग्र = शून्य;
            page->पूर्व = NGX_SLAB_PAGE;

            pool->pfree -= pages;

            अगर (--pages == 0) {
                वापस page;
            }

            क्रम (p = page + 1; pages; pages--) {
                p->slab = NGX_SLAB_PAGE_BUSY;
                p->अग्र = शून्य;
                p->पूर्व = NGX_SLAB_PAGE;
                p++;
            }

            वापस page;
        }
    }

    अगर (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    वापस शून्य;
}


अटल व्योम
ngx_slab_free_pages(ngx_slab_pool_t *pool, ngx_slab_page_t *page,
    ngx_uint_t pages)
{
    ngx_slab_page_t  *पूर्व, *join;

    pool->pfree += pages;

    page->slab = pages--;

    अगर (pages) {
        ngx_memzero(&page[1], pages * माप(ngx_slab_page_t));
    }

    अगर (page->अग्र) {
        पूर्व = ngx_slab_page_पूर्व(page);
        पूर्व->अग्र = page->अग्र;
        page->अग्र->पूर्व = page->पूर्व;
    }

    join = page + page->slab;

    अगर (join < pool->last) {

        अगर (ngx_slab_page_type(join) == NGX_SLAB_PAGE) {

            अगर (join->अग्र != शून्य) {
                pages += join->slab;
                page->slab += join->slab;

                पूर्व = ngx_slab_page_पूर्व(join);
                पूर्व->अग्र = join->अग्र;
                join->अग्र->पूर्व = join->पूर्व;

                join->slab = NGX_SLAB_PAGE_FREE;
                join->अग्र = शून्य;
                join->पूर्व = NGX_SLAB_PAGE;
            }
        }
    }

    अगर (page > pool->pages) {
        join = page - 1;

        अगर (ngx_slab_page_type(join) == NGX_SLAB_PAGE) {

            अगर (join->slab == NGX_SLAB_PAGE_FREE) {
                join = ngx_slab_page_पूर्व(join);
            }

            अगर (join->अग्र != शून्य) {
                pages += join->slab;
                join->slab += page->slab;

                पूर्व = ngx_slab_page_पूर्व(join);
                पूर्व->अग्र = join->अग्र;
                join->अग्र->पूर्व = join->पूर्व;

                page->slab = NGX_SLAB_PAGE_FREE;
                page->अग्र = शून्य;
                page->पूर्व = NGX_SLAB_PAGE;

                page = join;
            }
        }
    }

    अगर (pages) {
        page[pages].पूर्व = (uintptr_t) page;
    }

    page->पूर्व = (uintptr_t) &pool->मुक्त;
    page->अग्र = pool->मुक्त.अग्र;

    page->अग्र->पूर्व = (uintptr_t) page;

    pool->मुक्त.अग्र = page;
}


अटल व्योम
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, अक्षर *text)
{
    ngx_log_error(level, ngx_cycle->log, 0, "%s%s", text, pool->log_ctx);
}
