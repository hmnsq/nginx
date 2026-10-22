
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#परिभाषय NGX_SLAB_PAGE_आवरणम्   3
#परिभाषय NGX_SLAB_PAGE        0
#परिभाषय NGX_SLAB_BIG         1
#परिभाषय NGX_SLAB_EXACT       2
#परिभाषय NGX_SLAB_SMALL       3

#यदि (NGX_PTR_आकार == 4)

#परिभाषय NGX_SLAB_PAGE_FREE   0
#परिभाषय NGX_SLAB_PAGE_BUSY   0xffffffff
#परिभाषय NGX_SLAB_PAGE_START  0x80000000

#परिभाषय NGX_SLAB_सरणम्_आवरणम्  0x0000000f
#परिभाषय NGX_SLAB_MAP_आवरणम्    0xffff0000
#परिभाषय NGX_SLAB_MAP_सरणम्   16

#परिभाषय NGX_SLAB_BUSY        0xffffffff

#अन्यथा /* (NGX_PTR_SIZE == 8) */

#परिभाषय NGX_SLAB_PAGE_FREE   0
#परिभाषय NGX_SLAB_PAGE_BUSY   0xffffffffffffffff
#परिभाषय NGX_SLAB_PAGE_START  0x8000000000000000

#परिभाषय NGX_SLAB_सरणम्_आवरणम्  0x000000000000000f
#परिभाषय NGX_SLAB_MAP_आवरणम्    0xffffffff00000000
#परिभाषय NGX_SLAB_MAP_सरणम्   32

#परिभाषय NGX_SLAB_BUSY        0xffffffffffffffff

#यदि_अन्तः


#परिभाषय ngx_slab_slots(pool)                                                  \
    (ngx_slab_page_t *) ((u_char *) (pool) + परिमाण(ngx_slab_pool_t))

#परिभाषय ngx_slab_page_type(page)   ((page)->पूर्व & NGX_SLAB_PAGE_आवरणम्)

#परिभाषय ngx_slab_page_पूर्व(page)                                              \
    (ngx_slab_page_t *) ((page)->पूर्व & ~NGX_SLAB_PAGE_आवरणम्)

#परिभाषय ngx_slab_page_addr(pool, page)                                        \
    ((((page) - (pool)->पृष्ठानि) << ngx_pagesize_सरणम्)                         \
     + (uintptr_t) (pool)->start)


#यदि (NGX_DEBUG_MALLOC)

#परिभाषय ngx_slab_junk(p, आकार)     ngx_memset(p, 0xA5, आकार)

#नोचेद्यदि (NGX_HAVE_DEBUG_MALLOC)

#परिभाषय ngx_slab_junk(p, आकार)                                                \
    यदि (ngx_debug_malloc)          ngx_memset(p, 0xA5, आकार)

#अन्यथा

#परिभाषय ngx_slab_junk(p, आकार)

#यदि_अन्तः

स्थिर ngx_slab_page_t *ngx_slab_alloc_पृष्ठानि(ngx_slab_pool_t *pool,
    ngx_uint_t पृष्ठानि);
स्थिर शून्य ngx_slab_free_पृष्ठानि(ngx_slab_pool_t *pool, ngx_slab_page_t *page,
    ngx_uint_t पृष्ठानि);
स्थिर शून्य ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    वर्ण *text);


स्थिर ngx_uint_t  ngx_slab_max_आकार;
स्थिर ngx_uint_t  ngx_slab_exact_आकार;
स्थिर ngx_uint_t  ngx_slab_exact_सरणम्;


शून्य
ngx_slab_sizes_init(शून्य)
{
    ngx_uint_t  n;

    ngx_slab_max_आकार = ngx_pagesize / 2;
    ngx_slab_exact_आकार = ngx_pagesize / (8 * परिमाण(uintptr_t));
    प्रति (n = ngx_slab_exact_आकार; n >>= 1; ngx_slab_exact_सरणम्++) {
        /* void */
    }
}


शून्य
ngx_slab_init(ngx_slab_pool_t *pool)
{
    u_char           *p;
    आकार_t            आकार;
    ngx_int_t         m;
    ngx_uint_t        i, n, पृष्ठानि;
    ngx_slab_page_t  *slots, *page;

    pool->min_आकार = (आकार_t) 1 << pool->min_सरणम्;

    slots = ngx_slab_slots(pool);

    p = (u_char *) slots;
    आकार = pool->अन्तः - p;

    ngx_slab_junk(p, आकार);

    n = ngx_pagesize_सरणम् - pool->min_सरणम्;

    प्रति (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].अग्र = &slots[i];
        slots[i].पूर्व = 0;
    }

    p += n * परिमाण(ngx_slab_page_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * परिमाण(ngx_slab_stat_t));

    p += n * परिमाण(ngx_slab_stat_t);

    आकार -= n * (परिमाण(ngx_slab_page_t) + परिमाण(ngx_slab_stat_t));

    पृष्ठानि = (ngx_uint_t) (आकार / (ngx_pagesize + परिमाण(ngx_slab_page_t)));

    pool->पृष्ठानि = (ngx_slab_page_t *) p;
    ngx_memzero(pool->पृष्ठानि, पृष्ठानि * परिमाण(ngx_slab_page_t));

    page = pool->पृष्ठानि;

    /* only "next" is used in list head */
    pool->मुक्त.slab = 0;
    pool->मुक्त.अग्र = page;
    pool->मुक्त.पूर्व = 0;

    page->slab = पृष्ठानि;
    page->अग्र = &pool->मुक्त;
    page->पूर्व = (uintptr_t) &pool->मुक्त;

    pool->start = ngx_align_ptr(p + पृष्ठानि * परिमाण(ngx_slab_page_t),
                                ngx_pagesize);

    m = पृष्ठानि - (pool->अन्तः - pool->start) / ngx_pagesize;
    यदि (m > 0) {
        पृष्ठानि -= m;
        page->slab = पृष्ठानि;
    }

    pool->चरमः = pool->पृष्ठानि + पृष्ठानि;
    pool->pfree = पृष्ठानि;

    pool->log_nomem = 1;
    pool->log_ctx = &pool->zero;
    pool->zero = '\0';
}


शून्य *
ngx_slab_alloc(ngx_slab_pool_t *pool, आकार_t आकार)
{
    शून्य  *p;

    ngx_shmtx_तालकम्(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, आकार);

    ngx_shmtx_unlock(&pool->mutex);

    प्रत्यागम p;
}


शून्य *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, आकार_t आकार)
{
    आकार_t            s;
    uintptr_t         p, m, आवरणम्, *bitmap;
    ngx_uint_t        i, n, slot, सरणम्, map;
    ngx_slab_page_t  *page, *पूर्व, *slots;

    यदि (आकार > ngx_slab_max_आकार) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_आवृत्तिः->लघुगणकः, 0,
                       "slab alloc: %uz", आकार);

        page = ngx_slab_alloc_पृष्ठानि(pool, (आकार >> ngx_pagesize_सरणम्)
                                          + ((आकार % ngx_pagesize) ? 1 : 0));
        यदि (page) {
            p = ngx_slab_page_addr(pool, page);

        } अन्यथा {
            p = 0;
        }

        गच्छ समाप्तम्;
    }

    यदि (आकार > pool->min_आकार) {
        सरणम् = 1;
        प्रति (s = आकार - 1; s >>= 1; सरणम्++) { /* void */ }
        slot = सरणम् - pool->min_सरणम्;

    } अन्यथा {
        सरणम् = pool->min_सरणम्;
        slot = 0;
    }

    pool->stats[slot].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_आवृत्तिः->लघुगणकः, 0,
                   "slab alloc: %uz slot: %ui", आकार, slot);

    slots = ngx_slab_slots(pool);
    page = slots[slot].अग्र;

    यदि (page->अग्र != page) {

        यदि (सरणम् < ngx_slab_exact_सरणम्) {

            bitmap = (uintptr_t *) ngx_slab_page_addr(pool, page);

            map = (ngx_pagesize >> सरणम्) / (8 * परिमाण(uintptr_t));

            प्रति (n = 0; n < map; n++) {

                यदि (bitmap[n] != NGX_SLAB_BUSY) {

                    प्रति (m = 1, i = 0; m; m <<= 1, i++) {
                        यदि (bitmap[n] & m) {
                            अनुवर्त;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * परिमाण(uintptr_t) + i) << सरणम्;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[slot].प्रयुक्तम्++;

                        यदि (bitmap[n] == NGX_SLAB_BUSY) {
                            प्रति (n = n + 1; n < map; n++) {
                                यदि (bitmap[n] != NGX_SLAB_BUSY) {
                                    गच्छ समाप्तम्;
                                }
                            }

                            पूर्व = ngx_slab_page_पूर्व(page);
                            पूर्व->अग्र = page->अग्र;
                            page->अग्र->पूर्व = page->पूर्व;

                            page->अग्र = रिक्त;
                            page->पूर्व = NGX_SLAB_SMALL;
                        }

                        गच्छ समाप्तम्;
                    }
                }
            }

        } अन्यथा यदि (सरणम् == ngx_slab_exact_सरणम्) {

            प्रति (m = 1, i = 0; m; m <<= 1, i++) {
                यदि (page->slab & m) {
                    अनुवर्त;
                }

                page->slab |= m;

                यदि (page->slab == NGX_SLAB_BUSY) {
                    पूर्व = ngx_slab_page_पूर्व(page);
                    पूर्व->अग्र = page->अग्र;
                    page->अग्र->पूर्व = page->पूर्व;

                    page->अग्र = रिक्त;
                    page->पूर्व = NGX_SLAB_EXACT;
                }

                p = ngx_slab_page_addr(pool, page) + (i << सरणम्);

                pool->stats[slot].प्रयुक्तम्++;

                गच्छ समाप्तम्;
            }

        } अन्यथा { /* shift > ngx_slab_exact_shift */

            आवरणम् = ((uintptr_t) 1 << (ngx_pagesize >> सरणम्)) - 1;
            आवरणम् <<= NGX_SLAB_MAP_सरणम्;

            प्रति (m = (uintptr_t) 1 << NGX_SLAB_MAP_सरणम्, i = 0;
                 m & आवरणम्;
                 m <<= 1, i++)
            {
                यदि (page->slab & m) {
                    अनुवर्त;
                }

                page->slab |= m;

                यदि ((page->slab & NGX_SLAB_MAP_आवरणम्) == आवरणम्) {
                    पूर्व = ngx_slab_page_पूर्व(page);
                    पूर्व->अग्र = page->अग्र;
                    page->अग्र->पूर्व = page->पूर्व;

                    page->अग्र = रिक्त;
                    page->पूर्व = NGX_SLAB_BIG;
                }

                p = ngx_slab_page_addr(pool, page) + (i << सरणम्);

                pool->stats[slot].प्रयुक्तम्++;

                गच्छ समाप्तम्;
            }
        }

        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_alloc(): page is busy");
        ngx_debug_point();
    }

    page = ngx_slab_alloc_पृष्ठानि(pool, 1);

    यदि (page) {
        यदि (सरणम् < ngx_slab_exact_सरणम्) {
            bitmap = (uintptr_t *) ngx_slab_page_addr(pool, page);

            n = (ngx_pagesize >> सरणम्) / ((1 << सरणम्) * 8);

            यदि (n == 0) {
                n = 1;
            }

            /* "n" elements for bitmap, plus one requested */

            प्रति (i = 0; i < (n + 1) / (8 * परिमाण(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * परिमाण(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> सरणम्) / (8 * परिमाण(uintptr_t));

            प्रति (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            page->slab = सरणम्;
            page->अग्र = &slots[slot];
            page->पूर्व = (uintptr_t) &slots[slot] | NGX_SLAB_SMALL;

            slots[slot].अग्र = page;

            pool->stats[slot].योग += (ngx_pagesize >> सरणम्) - n;

            p = ngx_slab_page_addr(pool, page) + (n << सरणम्);

            pool->stats[slot].प्रयुक्तम्++;

            गच्छ समाप्तम्;

        } अन्यथा यदि (सरणम् == ngx_slab_exact_सरणम्) {

            page->slab = 1;
            page->अग्र = &slots[slot];
            page->पूर्व = (uintptr_t) &slots[slot] | NGX_SLAB_EXACT;

            slots[slot].अग्र = page;

            pool->stats[slot].योग += 8 * परिमाण(uintptr_t);

            p = ngx_slab_page_addr(pool, page);

            pool->stats[slot].प्रयुक्तम्++;

            गच्छ समाप्तम्;

        } अन्यथा { /* shift > ngx_slab_exact_shift */

            page->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_सरणम्) | सरणम्;
            page->अग्र = &slots[slot];
            page->पूर्व = (uintptr_t) &slots[slot] | NGX_SLAB_BIG;

            slots[slot].अग्र = page;

            pool->stats[slot].योग += ngx_pagesize >> सरणम्;

            p = ngx_slab_page_addr(pool, page);

            pool->stats[slot].प्रयुक्तम्++;

            गच्छ समाप्तम्;
        }
    }

    p = 0;

    pool->stats[slot].fails++;

समाप्तम्:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_आवृत्तिः->लघुगणकः, 0,
                   "slab alloc: %p", (शून्य *) p);

    प्रत्यागम (शून्य *) p;
}


शून्य *
ngx_slab_calloc(ngx_slab_pool_t *pool, आकार_t आकार)
{
    शून्य  *p;

    ngx_shmtx_तालकम्(&pool->mutex);

    p = ngx_slab_calloc_locked(pool, आकार);

    ngx_shmtx_unlock(&pool->mutex);

    प्रत्यागम p;
}


शून्य *
ngx_slab_calloc_locked(ngx_slab_pool_t *pool, आकार_t आकार)
{
    शून्य  *p;

    p = ngx_slab_alloc_locked(pool, आकार);
    यदि (p) {
        ngx_memzero(p, आकार);
    }

    प्रत्यागम p;
}


शून्य
ngx_slab_free(ngx_slab_pool_t *pool, शून्य *p)
{
    ngx_shmtx_तालकम्(&pool->mutex);

    ngx_slab_free_locked(pool, p);

    ngx_shmtx_unlock(&pool->mutex);
}


शून्य
ngx_slab_free_locked(ngx_slab_pool_t *pool, शून्य *p)
{
    आकार_t            आकार;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, slot, सरणम्, map;
    ngx_slab_page_t  *slots, *page;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_आवृत्तिः->लघुगणकः, 0, "slab free: %p", p);

    यदि ((u_char *) p < pool->start || (u_char *) p > pool->अन्तः) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        गच्छ fail;
    }

    n = ((u_char *) p - pool->start) >> ngx_pagesize_सरणम्;
    page = &pool->पृष्ठानि[n];
    slab = page->slab;
    type = ngx_slab_page_type(page);

    विकल्प (type) {

    पक्ष NGX_SLAB_SMALL:

        सरणम् = slab & NGX_SLAB_सरणम्_आवरणम्;
        आकार = (आकार_t) 1 << सरणम्;

        यदि ((uintptr_t) p & (आकार - 1)) {
            गच्छ wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> सरणम्;
        m = (uintptr_t) 1 << (n % (8 * परिमाण(uintptr_t)));
        n /= 8 * परिमाण(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        यदि (bitmap[n] & m) {
            slot = सरणम् - pool->min_सरणम्;

            यदि (page->अग्र == रिक्त) {
                slots = ngx_slab_slots(pool);

                page->अग्र = slots[slot].अग्र;
                slots[slot].अग्र = page;

                page->पूर्व = (uintptr_t) &slots[slot] | NGX_SLAB_SMALL;
                page->अग्र->पूर्व = (uintptr_t) page | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> सरणम्) / ((1 << सरणम्) * 8);

            यदि (n == 0) {
                n = 1;
            }

            i = n / (8 * परिमाण(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * परिमाण(uintptr_t)))) - 1;

            यदि (bitmap[i] & ~m) {
                गच्छ समाप्तम्;
            }

            map = (ngx_pagesize >> सरणम्) / (8 * परिमाण(uintptr_t));

            प्रति (i = i + 1; i < map; i++) {
                यदि (bitmap[i]) {
                    गच्छ समाप्तम्;
                }
            }

            ngx_slab_free_पृष्ठानि(pool, page, 1);

            pool->stats[slot].योग -= (ngx_pagesize >> सरणम्) - n;

            गच्छ समाप्तम्;
        }

        गच्छ chunk_already_free;

    पक्ष NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_सरणम्);
        आकार = ngx_slab_exact_आकार;

        यदि ((uintptr_t) p & (आकार - 1)) {
            गच्छ wrong_chunk;
        }

        यदि (slab & m) {
            slot = ngx_slab_exact_सरणम् - pool->min_सरणम्;

            यदि (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                page->अग्र = slots[slot].अग्र;
                slots[slot].अग्र = page;

                page->पूर्व = (uintptr_t) &slots[slot] | NGX_SLAB_EXACT;
                page->अग्र->पूर्व = (uintptr_t) page | NGX_SLAB_EXACT;
            }

            page->slab &= ~m;

            यदि (page->slab) {
                गच्छ समाप्तम्;
            }

            ngx_slab_free_पृष्ठानि(pool, page, 1);

            pool->stats[slot].योग -= 8 * परिमाण(uintptr_t);

            गच्छ समाप्तम्;
        }

        गच्छ chunk_already_free;

    पक्ष NGX_SLAB_BIG:

        सरणम् = slab & NGX_SLAB_सरणम्_आवरणम्;
        आकार = (आकार_t) 1 << सरणम्;

        यदि ((uintptr_t) p & (आकार - 1)) {
            गच्छ wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> सरणम्)
                              + NGX_SLAB_MAP_सरणम्);

        यदि (slab & m) {
            slot = सरणम् - pool->min_सरणम्;

            यदि (page->अग्र == रिक्त) {
                slots = ngx_slab_slots(pool);

                page->अग्र = slots[slot].अग्र;
                slots[slot].अग्र = page;

                page->पूर्व = (uintptr_t) &slots[slot] | NGX_SLAB_BIG;
                page->अग्र->पूर्व = (uintptr_t) page | NGX_SLAB_BIG;
            }

            page->slab &= ~m;

            यदि (page->slab & NGX_SLAB_MAP_आवरणम्) {
                गच्छ समाप्तम्;
            }

            ngx_slab_free_पृष्ठानि(pool, page, 1);

            pool->stats[slot].योग -= ngx_pagesize >> सरणम्;

            गच्छ समाप्तम्;
        }

        गच्छ chunk_already_free;

    पक्ष NGX_SLAB_PAGE:

        यदि ((uintptr_t) p & (ngx_pagesize - 1)) {
            गच्छ wrong_chunk;
        }

        यदि (!(slab & NGX_SLAB_PAGE_START)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            गच्छ fail;
        }

        यदि (slab == NGX_SLAB_PAGE_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            गच्छ fail;
        }

        आकार = slab & ~NGX_SLAB_PAGE_START;

        ngx_slab_free_पृष्ठानि(pool, page, आकार);

        ngx_slab_junk(p, आकार << ngx_pagesize_सरणम्);

        प्रत्यागम;
    }

    /* not reached */

    प्रत्यागम;

समाप्तम्:

    pool->stats[slot].प्रयुक्तम्--;

    ngx_slab_junk(p, आकार);

    प्रत्यागम;

wrong_chunk:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): pointer to wrong chunk");

    गच्छ fail;

chunk_already_free:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): chunk is already free");

fail:

    प्रत्यागम;
}


स्थिर ngx_slab_page_t *
ngx_slab_alloc_पृष्ठानि(ngx_slab_pool_t *pool, ngx_uint_t पृष्ठानि)
{
    ngx_slab_page_t  *page, *p;

    प्रति (page = pool->मुक्त.अग्र; page != &pool->मुक्त; page = page->अग्र) {

        यदि (page->slab >= पृष्ठानि) {

            यदि (page->slab > पृष्ठानि) {
                page[page->slab - 1].पूर्व = (uintptr_t) &page[पृष्ठानि];

                page[पृष्ठानि].slab = page->slab - पृष्ठानि;
                page[पृष्ठानि].अग्र = page->अग्र;
                page[पृष्ठानि].पूर्व = page->पूर्व;

                p = (ngx_slab_page_t *) page->पूर्व;
                p->अग्र = &page[पृष्ठानि];
                page->अग्र->पूर्व = (uintptr_t) &page[पृष्ठानि];

            } अन्यथा {
                p = (ngx_slab_page_t *) page->पूर्व;
                p->अग्र = page->अग्र;
                page->अग्र->पूर्व = page->पूर्व;
            }

            page->slab = पृष्ठानि | NGX_SLAB_PAGE_START;
            page->अग्र = रिक्त;
            page->पूर्व = NGX_SLAB_PAGE;

            pool->pfree -= पृष्ठानि;

            यदि (--पृष्ठानि == 0) {
                प्रत्यागम page;
            }

            प्रति (p = page + 1; पृष्ठानि; पृष्ठानि--) {
                p->slab = NGX_SLAB_PAGE_BUSY;
                p->अग्र = रिक्त;
                p->पूर्व = NGX_SLAB_PAGE;
                p++;
            }

            प्रत्यागम page;
        }
    }

    यदि (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    प्रत्यागम रिक्त;
}


स्थिर शून्य
ngx_slab_free_पृष्ठानि(ngx_slab_pool_t *pool, ngx_slab_page_t *page,
    ngx_uint_t पृष्ठानि)
{
    ngx_slab_page_t  *पूर्व, *join;

    pool->pfree += पृष्ठानि;

    page->slab = पृष्ठानि--;

    यदि (पृष्ठानि) {
        ngx_memzero(&page[1], पृष्ठानि * परिमाण(ngx_slab_page_t));
    }

    यदि (page->अग्र) {
        पूर्व = ngx_slab_page_पूर्व(page);
        पूर्व->अग्र = page->अग्र;
        page->अग्र->पूर्व = page->पूर्व;
    }

    join = page + page->slab;

    यदि (join < pool->चरमः) {

        यदि (ngx_slab_page_type(join) == NGX_SLAB_PAGE) {

            यदि (join->अग्र != रिक्त) {
                पृष्ठानि += join->slab;
                page->slab += join->slab;

                पूर्व = ngx_slab_page_पूर्व(join);
                पूर्व->अग्र = join->अग्र;
                join->अग्र->पूर्व = join->पूर्व;

                join->slab = NGX_SLAB_PAGE_FREE;
                join->अग्र = रिक्त;
                join->पूर्व = NGX_SLAB_PAGE;
            }
        }
    }

    यदि (page > pool->पृष्ठानि) {
        join = page - 1;

        यदि (ngx_slab_page_type(join) == NGX_SLAB_PAGE) {

            यदि (join->slab == NGX_SLAB_PAGE_FREE) {
                join = ngx_slab_page_पूर्व(join);
            }

            यदि (join->अग्र != रिक्त) {
                पृष्ठानि += join->slab;
                join->slab += page->slab;

                पूर्व = ngx_slab_page_पूर्व(join);
                पूर्व->अग्र = join->अग्र;
                join->अग्र->पूर्व = join->पूर्व;

                page->slab = NGX_SLAB_PAGE_FREE;
                page->अग्र = रिक्त;
                page->पूर्व = NGX_SLAB_PAGE;

                page = join;
            }
        }
    }

    यदि (पृष्ठानि) {
        page[पृष्ठानि].पूर्व = (uintptr_t) page;
    }

    page->पूर्व = (uintptr_t) &pool->मुक्त;
    page->अग्र = pool->मुक्त.अग्र;

    page->अग्र->पूर्व = (uintptr_t) page;

    pool->मुक्त.अग्र = page;
}


स्थिर शून्य
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, वर्ण *text)
{
    ngx_log_error(level, ngx_आवृत्तिः->लघुगणकः, 0, "%s%s", text, pool->log_ctx);
}
