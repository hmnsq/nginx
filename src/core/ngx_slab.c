
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کړه NGX_SLAB_پاڼه_ماسک   3
#تعریف_کړه NGX_SLAB_پاڼه        0
#تعریف_کړه NGX_SLAB_BIG         1
#تعریف_کړه NGX_SLAB_EXACT       2
#تعریف_کړه NGX_SLAB_SMALL       3

#إذا (NGX_PTR_اندازه == 4)

#تعریف_کړه NGX_SLAB_پاڼه_FREE   0
#تعریف_کړه NGX_SLAB_پاڼه_BUSY   0xffffffff
#تعریف_کړه NGX_SLAB_پاڼه_پیل  0x80000000

#تعریف_کړه NGX_SLAB_ښويدنه_ماسک  0x0000000f
#تعریف_کړه NGX_SLAB_MAP_ماسک    0xffff0000
#تعریف_کړه NGX_SLAB_MAP_ښويدنه   16

#تعریف_کړه NGX_SLAB_BUSY        0xffffffff

#وإلا /* (NGX_PTR_SIZE == 8) */

#تعریف_کړه NGX_SLAB_پاڼه_FREE   0
#تعریف_کړه NGX_SLAB_پاڼه_BUSY   0xffffffffffffffff
#تعریف_کړه NGX_SLAB_پاڼه_پیل  0x8000000000000000

#تعریف_کړه NGX_SLAB_ښويدنه_ماسک  0x000000000000000f
#تعریف_کړه NGX_SLAB_MAP_ماسک    0xffffffff00000000
#تعریف_کړه NGX_SLAB_MAP_ښويدنه   32

#تعریف_کړه NGX_SLAB_BUSY        0xffffffffffffffff

#د_که_پای


#تعریف_کړه ngx_slab_slots(pool)                                                  \
    (ngx_slab_پاڼه_t *) ((u_char *) (pool) + حجم(ngx_slab_pool_t))

#تعریف_کړه ngx_slab_پاڼه_type(پاڼه)   ((پاڼه)->prev & NGX_SLAB_پاڼه_ماسک)

#تعریف_کړه ngx_slab_پاڼه_prev(پاڼه)                                              \
    (ngx_slab_پاڼه_t *) ((پاڼه)->prev & ~NGX_SLAB_پاڼه_ماسک)

#تعریف_کړه ngx_slab_پاڼه_addr(pool, پاڼه)                                        \
    ((((پاڼه) - (pool)->پاڼې) << ngx_pagesize_ښويدنه)                         \
     + (uintptr_t) (pool)->پیل)


#إذا (NGX_DEBUG_MALLOC)

#تعریف_کړه ngx_slab_junk(p, اندازه)     ngx_memset(p, 0xA5, اندازه)

#وإلاإذا (NGX_HAVE_DEBUG_MALLOC)

#تعریف_کړه ngx_slab_junk(p, اندازه)                                                \
    إذا (ngx_debug_malloc)          ngx_memset(p, 0xA5, اندازه)

#وإلا

#تعریف_کړه ngx_slab_junk(p, اندازه)

#د_که_پای

ساكن ngx_slab_پاڼه_t *ngx_slab_alloc_پاڼې(ngx_slab_pool_t *pool,
    ngx_uint_t پاڼې);
ساكن فراغ ngx_slab_free_پاڼې(ngx_slab_pool_t *pool, ngx_slab_پاڼه_t *پاڼه,
    ngx_uint_t پاڼې);
ساكن فراغ ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    حرف *متن);


ساكن ngx_uint_t  ngx_slab_max_اندازه;
ساكن ngx_uint_t  ngx_slab_exact_اندازه;
ساكن ngx_uint_t  ngx_slab_exact_ښويدنه;


فراغ
ngx_slab_sizes_init(فراغ)
{
    ngx_uint_t  n;

    ngx_slab_max_اندازه = ngx_pagesize / 2;
    ngx_slab_exact_اندازه = ngx_pagesize / (8 * حجم(uintptr_t));
    لكل (n = ngx_slab_exact_اندازه; n >>= 1; ngx_slab_exact_ښويدنه++) {
        /* void */
    }
}


فراغ
ngx_slab_init(ngx_slab_pool_t *pool)
{
    u_char           *p;
    اندازه_t            اندازه;
    ngx_int_t         m;
    ngx_uint_t        i, n, پاڼې;
    ngx_slab_پاڼه_t  *slots, *پاڼه;

    pool->min_اندازه = (اندازه_t) 1 << pool->min_ښويدنه;

    slots = ngx_slab_slots(pool);

    p = (u_char *) slots;
    اندازه = pool->پای_ټکی - p;

    ngx_slab_junk(p, اندازه);

    n = ngx_pagesize_ښويدنه - pool->min_ښويدنه;

    لكل (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].بل = &slots[i];
        slots[i].prev = 0;
    }

    p += n * حجم(ngx_slab_پاڼه_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * حجم(ngx_slab_stat_t));

    p += n * حجم(ngx_slab_stat_t);

    اندازه -= n * (حجم(ngx_slab_پاڼه_t) + حجم(ngx_slab_stat_t));

    پاڼې = (ngx_uint_t) (اندازه / (ngx_pagesize + حجم(ngx_slab_پاڼه_t)));

    pool->پاڼې = (ngx_slab_پاڼه_t *) p;
    ngx_memzero(pool->پاڼې, پاڼې * حجم(ngx_slab_پاڼه_t));

    پاڼه = pool->پاڼې;

    /* only "next" is used in list head */
    pool->حرر.slab = 0;
    pool->حرر.بل = پاڼه;
    pool->حرر.prev = 0;

    پاڼه->slab = پاڼې;
    پاڼه->بل = &pool->حرر;
    پاڼه->prev = (uintptr_t) &pool->حرر;

    pool->پیل = ngx_align_ptr(p + پاڼې * حجم(ngx_slab_پاڼه_t),
                                ngx_pagesize);

    m = پاڼې - (pool->پای_ټکی - pool->پیل) / ngx_pagesize;
    إذا (m > 0) {
        پاڼې -= m;
        پاڼه->slab = پاڼې;
    }

    pool->وروستی = pool->پاڼې + پاڼې;
    pool->pfree = پاڼې;

    pool->log_nomem = 1;
    pool->log_ctx = &pool->zero;
    pool->zero = '\0';
}


فراغ *
ngx_slab_alloc(ngx_slab_pool_t *pool, اندازه_t اندازه)
{
    فراغ  *p;

    ngx_shmtx_قلف(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, اندازه);

    ngx_shmtx_unlock(&pool->mutex);

    أرجع p;
}


فراغ *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, اندازه_t اندازه)
{
    اندازه_t            s;
    uintptr_t         p, m, ماسک, *bitmap;
    ngx_uint_t        i, n, خونه, ښويدنه, map;
    ngx_slab_پاڼه_t  *پاڼه, *prev, *slots;

    إذا (اندازه > ngx_slab_max_اندازه) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_دوره->لوګاریتم, 0,
                       "slab alloc: %uz", اندازه);

        پاڼه = ngx_slab_alloc_پاڼې(pool, (اندازه >> ngx_pagesize_ښويدنه)
                                          + ((اندازه % ngx_pagesize) ? 1 : 0));
        إذا (پاڼه) {
            p = ngx_slab_پاڼه_addr(pool, پاڼه);

        } وإلا {
            p = 0;
        }

        اذهب بشپړ;
    }

    إذا (اندازه > pool->min_اندازه) {
        ښويدنه = 1;
        لكل (s = اندازه - 1; s >>= 1; ښويدنه++) { /* void */ }
        خونه = ښويدنه - pool->min_ښويدنه;

    } وإلا {
        ښويدنه = pool->min_ښويدنه;
        خونه = 0;
    }

    pool->stats[خونه].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_دوره->لوګاریتم, 0,
                   "slab alloc: %uz slot: %ui", اندازه, خونه);

    slots = ngx_slab_slots(pool);
    پاڼه = slots[خونه].بل;

    إذا (پاڼه->بل != پاڼه) {

        إذا (ښويدنه < ngx_slab_exact_ښويدنه) {

            bitmap = (uintptr_t *) ngx_slab_پاڼه_addr(pool, پاڼه);

            map = (ngx_pagesize >> ښويدنه) / (8 * حجم(uintptr_t));

            لكل (n = 0; n < map; n++) {

                إذا (bitmap[n] != NGX_SLAB_BUSY) {

                    لكل (m = 1, i = 0; m; m <<= 1, i++) {
                        إذا (bitmap[n] & m) {
                            استمر;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * حجم(uintptr_t) + i) << ښويدنه;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[خونه].کارول_شوی++;

                        إذا (bitmap[n] == NGX_SLAB_BUSY) {
                            لكل (n = n + 1; n < map; n++) {
                                إذا (bitmap[n] != NGX_SLAB_BUSY) {
                                    اذهب بشپړ;
                                }
                            }

                            prev = ngx_slab_پاڼه_prev(پاڼه);
                            prev->بل = پاڼه->بل;
                            پاڼه->بل->prev = پاڼه->prev;

                            پاڼه->بل = عدم;
                            پاڼه->prev = NGX_SLAB_SMALL;
                        }

                        اذهب بشپړ;
                    }
                }
            }

        } وإلا إذا (ښويدنه == ngx_slab_exact_ښويدنه) {

            لكل (m = 1, i = 0; m; m <<= 1, i++) {
                إذا (پاڼه->slab & m) {
                    استمر;
                }

                پاڼه->slab |= m;

                إذا (پاڼه->slab == NGX_SLAB_BUSY) {
                    prev = ngx_slab_پاڼه_prev(پاڼه);
                    prev->بل = پاڼه->بل;
                    پاڼه->بل->prev = پاڼه->prev;

                    پاڼه->بل = عدم;
                    پاڼه->prev = NGX_SLAB_EXACT;
                }

                p = ngx_slab_پاڼه_addr(pool, پاڼه) + (i << ښويدنه);

                pool->stats[خونه].کارول_شوی++;

                اذهب بشپړ;
            }

        } وإلا { /* shift > ngx_slab_exact_shift */

            ماسک = ((uintptr_t) 1 << (ngx_pagesize >> ښويدنه)) - 1;
            ماسک <<= NGX_SLAB_MAP_ښويدنه;

            لكل (m = (uintptr_t) 1 << NGX_SLAB_MAP_ښويدنه, i = 0;
                 m & ماسک;
                 m <<= 1, i++)
            {
                إذا (پاڼه->slab & m) {
                    استمر;
                }

                پاڼه->slab |= m;

                إذا ((پاڼه->slab & NGX_SLAB_MAP_ماسک) == ماسک) {
                    prev = ngx_slab_پاڼه_prev(پاڼه);
                    prev->بل = پاڼه->بل;
                    پاڼه->بل->prev = پاڼه->prev;

                    پاڼه->بل = عدم;
                    پاڼه->prev = NGX_SLAB_BIG;
                }

                p = ngx_slab_پاڼه_addr(pool, پاڼه) + (i << ښويدنه);

                pool->stats[خونه].کارول_شوی++;

                اذهب بشپړ;
            }
        }

        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_alloc(): page is busy");
        ngx_debug_ټکی();
    }

    پاڼه = ngx_slab_alloc_پاڼې(pool, 1);

    إذا (پاڼه) {
        إذا (ښويدنه < ngx_slab_exact_ښويدنه) {
            bitmap = (uintptr_t *) ngx_slab_پاڼه_addr(pool, پاڼه);

            n = (ngx_pagesize >> ښويدنه) / ((1 << ښويدنه) * 8);

            إذا (n == 0) {
                n = 1;
            }

            /* "n" elements for bitmap, plus one requested */

            لكل (i = 0; i < (n + 1) / (8 * حجم(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * حجم(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> ښويدنه) / (8 * حجم(uintptr_t));

            لكل (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            پاڼه->slab = ښويدنه;
            پاڼه->بل = &slots[خونه];
            پاڼه->prev = (uintptr_t) &slots[خونه] | NGX_SLAB_SMALL;

            slots[خونه].بل = پاڼه;

            pool->stats[خونه].ټول += (ngx_pagesize >> ښويدنه) - n;

            p = ngx_slab_پاڼه_addr(pool, پاڼه) + (n << ښويدنه);

            pool->stats[خونه].کارول_شوی++;

            اذهب بشپړ;

        } وإلا إذا (ښويدنه == ngx_slab_exact_ښويدنه) {

            پاڼه->slab = 1;
            پاڼه->بل = &slots[خونه];
            پاڼه->prev = (uintptr_t) &slots[خونه] | NGX_SLAB_EXACT;

            slots[خونه].بل = پاڼه;

            pool->stats[خونه].ټول += 8 * حجم(uintptr_t);

            p = ngx_slab_پاڼه_addr(pool, پاڼه);

            pool->stats[خونه].کارول_شوی++;

            اذهب بشپړ;

        } وإلا { /* shift > ngx_slab_exact_shift */

            پاڼه->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_ښويدنه) | ښويدنه;
            پاڼه->بل = &slots[خونه];
            پاڼه->prev = (uintptr_t) &slots[خونه] | NGX_SLAB_BIG;

            slots[خونه].بل = پاڼه;

            pool->stats[خونه].ټول += ngx_pagesize >> ښويدنه;

            p = ngx_slab_پاڼه_addr(pool, پاڼه);

            pool->stats[خونه].کارول_شوی++;

            اذهب بشپړ;
        }
    }

    p = 0;

    pool->stats[خونه].fails++;

بشپړ:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_دوره->لوګاریتم, 0,
                   "slab alloc: %p", (فراغ *) p);

    أرجع (فراغ *) p;
}


فراغ *
ngx_slab_calloc(ngx_slab_pool_t *pool, اندازه_t اندازه)
{
    فراغ  *p;

    ngx_shmtx_قلف(&pool->mutex);

    p = ngx_slab_calloc_locked(pool, اندازه);

    ngx_shmtx_unlock(&pool->mutex);

    أرجع p;
}


فراغ *
ngx_slab_calloc_locked(ngx_slab_pool_t *pool, اندازه_t اندازه)
{
    فراغ  *p;

    p = ngx_slab_alloc_locked(pool, اندازه);
    إذا (p) {
        ngx_memzero(p, اندازه);
    }

    أرجع p;
}


فراغ
ngx_slab_free(ngx_slab_pool_t *pool, فراغ *p)
{
    ngx_shmtx_قلف(&pool->mutex);

    ngx_slab_free_locked(pool, p);

    ngx_shmtx_unlock(&pool->mutex);
}


فراغ
ngx_slab_free_locked(ngx_slab_pool_t *pool, فراغ *p)
{
    اندازه_t            اندازه;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, خونه, ښويدنه, map;
    ngx_slab_پاڼه_t  *slots, *پاڼه;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_دوره->لوګاریتم, 0, "slab free: %p", p);

    إذا ((u_char *) p < pool->پیل || (u_char *) p > pool->پای_ټکی) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        اذهب fail;
    }

    n = ((u_char *) p - pool->پیل) >> ngx_pagesize_ښويدنه;
    پاڼه = &pool->پاڼې[n];
    slab = پاڼه->slab;
    type = ngx_slab_پاڼه_type(پاڼه);

    بدل (type) {

    حالة NGX_SLAB_SMALL:

        ښويدنه = slab & NGX_SLAB_ښويدنه_ماسک;
        اندازه = (اندازه_t) 1 << ښويدنه;

        إذا ((uintptr_t) p & (اندازه - 1)) {
            اذهب wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> ښويدنه;
        m = (uintptr_t) 1 << (n % (8 * حجم(uintptr_t)));
        n /= 8 * حجم(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        إذا (bitmap[n] & m) {
            خونه = ښويدنه - pool->min_ښويدنه;

            إذا (پاڼه->بل == عدم) {
                slots = ngx_slab_slots(pool);

                پاڼه->بل = slots[خونه].بل;
                slots[خونه].بل = پاڼه;

                پاڼه->prev = (uintptr_t) &slots[خونه] | NGX_SLAB_SMALL;
                پاڼه->بل->prev = (uintptr_t) پاڼه | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> ښويدنه) / ((1 << ښويدنه) * 8);

            إذا (n == 0) {
                n = 1;
            }

            i = n / (8 * حجم(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * حجم(uintptr_t)))) - 1;

            إذا (bitmap[i] & ~m) {
                اذهب بشپړ;
            }

            map = (ngx_pagesize >> ښويدنه) / (8 * حجم(uintptr_t));

            لكل (i = i + 1; i < map; i++) {
                إذا (bitmap[i]) {
                    اذهب بشپړ;
                }
            }

            ngx_slab_free_پاڼې(pool, پاڼه, 1);

            pool->stats[خونه].ټول -= (ngx_pagesize >> ښويدنه) - n;

            اذهب بشپړ;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_ښويدنه);
        اندازه = ngx_slab_exact_اندازه;

        إذا ((uintptr_t) p & (اندازه - 1)) {
            اذهب wrong_chunk;
        }

        إذا (slab & m) {
            خونه = ngx_slab_exact_ښويدنه - pool->min_ښويدنه;

            إذا (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                پاڼه->بل = slots[خونه].بل;
                slots[خونه].بل = پاڼه;

                پاڼه->prev = (uintptr_t) &slots[خونه] | NGX_SLAB_EXACT;
                پاڼه->بل->prev = (uintptr_t) پاڼه | NGX_SLAB_EXACT;
            }

            پاڼه->slab &= ~m;

            إذا (پاڼه->slab) {
                اذهب بشپړ;
            }

            ngx_slab_free_پاڼې(pool, پاڼه, 1);

            pool->stats[خونه].ټول -= 8 * حجم(uintptr_t);

            اذهب بشپړ;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_BIG:

        ښويدنه = slab & NGX_SLAB_ښويدنه_ماسک;
        اندازه = (اندازه_t) 1 << ښويدنه;

        إذا ((uintptr_t) p & (اندازه - 1)) {
            اذهب wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> ښويدنه)
                              + NGX_SLAB_MAP_ښويدنه);

        إذا (slab & m) {
            خونه = ښويدنه - pool->min_ښويدنه;

            إذا (پاڼه->بل == عدم) {
                slots = ngx_slab_slots(pool);

                پاڼه->بل = slots[خونه].بل;
                slots[خونه].بل = پاڼه;

                پاڼه->prev = (uintptr_t) &slots[خونه] | NGX_SLAB_BIG;
                پاڼه->بل->prev = (uintptr_t) پاڼه | NGX_SLAB_BIG;
            }

            پاڼه->slab &= ~m;

            إذا (پاڼه->slab & NGX_SLAB_MAP_ماسک) {
                اذهب بشپړ;
            }

            ngx_slab_free_پاڼې(pool, پاڼه, 1);

            pool->stats[خونه].ټول -= ngx_pagesize >> ښويدنه;

            اذهب بشپړ;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_پاڼه:

        إذا ((uintptr_t) p & (ngx_pagesize - 1)) {
            اذهب wrong_chunk;
        }

        إذا (!(slab & NGX_SLAB_پاڼه_پیل)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            اذهب fail;
        }

        إذا (slab == NGX_SLAB_پاڼه_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            اذهب fail;
        }

        اندازه = slab & ~NGX_SLAB_پاڼه_پیل;

        ngx_slab_free_پاڼې(pool, پاڼه, اندازه);

        ngx_slab_junk(p, اندازه << ngx_pagesize_ښويدنه);

        أرجع;
    }

    /* not reached */

    أرجع;

بشپړ:

    pool->stats[خونه].کارول_شوی--;

    ngx_slab_junk(p, اندازه);

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


ساكن ngx_slab_پاڼه_t *
ngx_slab_alloc_پاڼې(ngx_slab_pool_t *pool, ngx_uint_t پاڼې)
{
    ngx_slab_پاڼه_t  *پاڼه, *p;

    لكل (پاڼه = pool->حرر.بل; پاڼه != &pool->حرر; پاڼه = پاڼه->بل) {

        إذا (پاڼه->slab >= پاڼې) {

            إذا (پاڼه->slab > پاڼې) {
                پاڼه[پاڼه->slab - 1].prev = (uintptr_t) &پاڼه[پاڼې];

                پاڼه[پاڼې].slab = پاڼه->slab - پاڼې;
                پاڼه[پاڼې].بل = پاڼه->بل;
                پاڼه[پاڼې].prev = پاڼه->prev;

                p = (ngx_slab_پاڼه_t *) پاڼه->prev;
                p->بل = &پاڼه[پاڼې];
                پاڼه->بل->prev = (uintptr_t) &پاڼه[پاڼې];

            } وإلا {
                p = (ngx_slab_پاڼه_t *) پاڼه->prev;
                p->بل = پاڼه->بل;
                پاڼه->بل->prev = پاڼه->prev;
            }

            پاڼه->slab = پاڼې | NGX_SLAB_پاڼه_پیل;
            پاڼه->بل = عدم;
            پاڼه->prev = NGX_SLAB_پاڼه;

            pool->pfree -= پاڼې;

            إذا (--پاڼې == 0) {
                أرجع پاڼه;
            }

            لكل (p = پاڼه + 1; پاڼې; پاڼې--) {
                p->slab = NGX_SLAB_پاڼه_BUSY;
                p->بل = عدم;
                p->prev = NGX_SLAB_پاڼه;
                p++;
            }

            أرجع پاڼه;
        }
    }

    إذا (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    أرجع عدم;
}


ساكن فراغ
ngx_slab_free_پاڼې(ngx_slab_pool_t *pool, ngx_slab_پاڼه_t *پاڼه,
    ngx_uint_t پاڼې)
{
    ngx_slab_پاڼه_t  *prev, *join;

    pool->pfree += پاڼې;

    پاڼه->slab = پاڼې--;

    إذا (پاڼې) {
        ngx_memzero(&پاڼه[1], پاڼې * حجم(ngx_slab_پاڼه_t));
    }

    إذا (پاڼه->بل) {
        prev = ngx_slab_پاڼه_prev(پاڼه);
        prev->بل = پاڼه->بل;
        پاڼه->بل->prev = پاڼه->prev;
    }

    join = پاڼه + پاڼه->slab;

    إذا (join < pool->وروستی) {

        إذا (ngx_slab_پاڼه_type(join) == NGX_SLAB_پاڼه) {

            إذا (join->بل != عدم) {
                پاڼې += join->slab;
                پاڼه->slab += join->slab;

                prev = ngx_slab_پاڼه_prev(join);
                prev->بل = join->بل;
                join->بل->prev = join->prev;

                join->slab = NGX_SLAB_پاڼه_FREE;
                join->بل = عدم;
                join->prev = NGX_SLAB_پاڼه;
            }
        }
    }

    إذا (پاڼه > pool->پاڼې) {
        join = پاڼه - 1;

        إذا (ngx_slab_پاڼه_type(join) == NGX_SLAB_پاڼه) {

            إذا (join->slab == NGX_SLAB_پاڼه_FREE) {
                join = ngx_slab_پاڼه_prev(join);
            }

            إذا (join->بل != عدم) {
                پاڼې += join->slab;
                join->slab += پاڼه->slab;

                prev = ngx_slab_پاڼه_prev(join);
                prev->بل = join->بل;
                join->بل->prev = join->prev;

                پاڼه->slab = NGX_SLAB_پاڼه_FREE;
                پاڼه->بل = عدم;
                پاڼه->prev = NGX_SLAB_پاڼه;

                پاڼه = join;
            }
        }
    }

    إذا (پاڼې) {
        پاڼه[پاڼې].prev = (uintptr_t) پاڼه;
    }

    پاڼه->prev = (uintptr_t) &pool->حرر;
    پاڼه->بل = pool->حرر.بل;

    پاڼه->بل->prev = (uintptr_t) پاڼه;

    pool->حرر.بل = پاڼه;
}


ساكن فراغ
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, حرف *متن)
{
    ngx_log_error(level, ngx_دوره->لوګاریتم, 0, "%s%s", متن, pool->log_ctx);
}
