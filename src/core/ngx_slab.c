
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#وصف_ڪريو NGX_SLAB_صفحو_نقاب   3
#وصف_ڪريو NGX_SLAB_صفحو        0
#وصف_ڪريو NGX_SLAB_BIG         1
#وصف_ڪريو NGX_SLAB_EXACT       2
#وصف_ڪريو NGX_SLAB_SMALL       3

#إذا (NGX_PTR_سائيز == 4)

#وصف_ڪريو NGX_SLAB_صفحو_FREE   0
#وصف_ڪريو NGX_SLAB_صفحو_BUSY   0xffffffff
#وصف_ڪريو NGX_SLAB_صفحو_شروعات  0x80000000

#وصف_ڪريو NGX_SLAB_سرڪايو_نقاب  0x0000000f
#وصف_ڪريو NGX_SLAB_MAP_نقاب    0xffff0000
#وصف_ڪريو NGX_SLAB_MAP_سرڪايو   16

#وصف_ڪريو NGX_SLAB_BUSY        0xffffffff

#وإلا /* (NGX_PTR_SIZE == 8) */

#وصف_ڪريو NGX_SLAB_صفحو_FREE   0
#وصف_ڪريو NGX_SLAB_صفحو_BUSY   0xffffffffffffffff
#وصف_ڪريو NGX_SLAB_صفحو_شروعات  0x8000000000000000

#وصف_ڪريو NGX_SLAB_سرڪايو_نقاب  0x000000000000000f
#وصف_ڪريو NGX_SLAB_MAP_نقاب    0xffffffff00000000
#وصف_ڪريو NGX_SLAB_MAP_سرڪايو   32

#وصف_ڪريو NGX_SLAB_BUSY        0xffffffffffffffff

#جيڪڏهن_پورو


#وصف_ڪريو ngx_slab_slots(pool)                                                  \
    (ngx_slab_صفحو_t *) ((u_char *) (pool) + حجم(ngx_slab_pool_t))

#وصف_ڪريو ngx_slab_صفحو_type(صفحو)   ((صفحو)->prev & NGX_SLAB_صفحو_نقاب)

#وصف_ڪريو ngx_slab_صفحو_prev(صفحو)                                              \
    (ngx_slab_صفحو_t *) ((صفحو)->prev & ~NGX_SLAB_صفحو_نقاب)

#وصف_ڪريو ngx_slab_صفحو_addr(pool, صفحو)                                        \
    ((((صفحو) - (pool)->صفحا) << ngx_pagesize_سرڪايو)                         \
     + (uintptr_t) (pool)->شروعات)


#إذا (NGX_DEBUG_MALLOC)

#وصف_ڪريو ngx_slab_junk(p, سائيز)     ngx_memset(p, 0xA5, سائيز)

#وإلاإذا (NGX_HAVE_DEBUG_MALLOC)

#وصف_ڪريو ngx_slab_junk(p, سائيز)                                                \
    إذا (ngx_debug_malloc)          ngx_memset(p, 0xA5, سائيز)

#وإلا

#وصف_ڪريو ngx_slab_junk(p, سائيز)

#جيڪڏهن_پورو

ساكن ngx_slab_صفحو_t *ngx_slab_alloc_صفحا(ngx_slab_pool_t *pool,
    ngx_uint_t صفحا);
ساكن فراغ ngx_slab_free_صفحا(ngx_slab_pool_t *pool, ngx_slab_صفحو_t *صفحو,
    ngx_uint_t صفحا);
ساكن فراغ ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    حرف *متن);


ساكن ngx_uint_t  ngx_slab_max_سائيز;
ساكن ngx_uint_t  ngx_slab_exact_سائيز;
ساكن ngx_uint_t  ngx_slab_exact_سرڪايو;


فراغ
ngx_slab_sizes_init(فراغ)
{
    ngx_uint_t  n;

    ngx_slab_max_سائيز = ngx_pagesize / 2;
    ngx_slab_exact_سائيز = ngx_pagesize / (8 * حجم(uintptr_t));
    لكل (n = ngx_slab_exact_سائيز; n >>= 1; ngx_slab_exact_سرڪايو++) {
        /* void */
    }
}


فراغ
ngx_slab_init(ngx_slab_pool_t *pool)
{
    u_char           *p;
    سائيز_t            سائيز;
    ngx_int_t         m;
    ngx_uint_t        i, n, صفحا;
    ngx_slab_صفحو_t  *slots, *صفحو;

    pool->min_سائيز = (سائيز_t) 1 << pool->min_سرڪايو;

    slots = ngx_slab_slots(pool);

    p = (u_char *) slots;
    سائيز = pool->پڇاڙي - p;

    ngx_slab_junk(p, سائيز);

    n = ngx_pagesize_سرڪايو - pool->min_سرڪايو;

    لكل (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].ايندڙ = &slots[i];
        slots[i].prev = 0;
    }

    p += n * حجم(ngx_slab_صفحو_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * حجم(ngx_slab_stat_t));

    p += n * حجم(ngx_slab_stat_t);

    سائيز -= n * (حجم(ngx_slab_صفحو_t) + حجم(ngx_slab_stat_t));

    صفحا = (ngx_uint_t) (سائيز / (ngx_pagesize + حجم(ngx_slab_صفحو_t)));

    pool->صفحا = (ngx_slab_صفحو_t *) p;
    ngx_memzero(pool->صفحا, صفحا * حجم(ngx_slab_صفحو_t));

    صفحو = pool->صفحا;

    /* only "next" is used in list head */
    pool->حرر.slab = 0;
    pool->حرر.ايندڙ = صفحو;
    pool->حرر.prev = 0;

    صفحو->slab = صفحا;
    صفحو->ايندڙ = &pool->حرر;
    صفحو->prev = (uintptr_t) &pool->حرر;

    pool->شروعات = ngx_align_ptr(p + صفحا * حجم(ngx_slab_صفحو_t),
                                ngx_pagesize);

    m = صفحا - (pool->پڇاڙي - pool->شروعات) / ngx_pagesize;
    إذا (m > 0) {
        صفحا -= m;
        صفحو->slab = صفحا;
    }

    pool->آخري = pool->صفحا + صفحا;
    pool->pfree = صفحا;

    pool->log_nomem = 1;
    pool->log_ctx = &pool->zero;
    pool->zero = '\0';
}


فراغ *
ngx_slab_alloc(ngx_slab_pool_t *pool, سائيز_t سائيز)
{
    فراغ  *p;

    ngx_shmtx_تالو(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, سائيز);

    ngx_shmtx_unlock(&pool->mutex);

    أرجع p;
}


فراغ *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, سائيز_t سائيز)
{
    سائيز_t            s;
    uintptr_t         p, m, نقاب, *bitmap;
    ngx_uint_t        i, n, جڳهه, سرڪايو, map;
    ngx_slab_صفحو_t  *صفحو, *prev, *slots;

    إذا (سائيز > ngx_slab_max_سائيز) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_چڪر->لاگرٿم, 0,
                       "slab alloc: %uz", سائيز);

        صفحو = ngx_slab_alloc_صفحا(pool, (سائيز >> ngx_pagesize_سرڪايو)
                                          + ((سائيز % ngx_pagesize) ? 1 : 0));
        إذا (صفحو) {
            p = ngx_slab_صفحو_addr(pool, صفحو);

        } وإلا {
            p = 0;
        }

        اذهب پورو;
    }

    إذا (سائيز > pool->min_سائيز) {
        سرڪايو = 1;
        لكل (s = سائيز - 1; s >>= 1; سرڪايو++) { /* void */ }
        جڳهه = سرڪايو - pool->min_سرڪايو;

    } وإلا {
        سرڪايو = pool->min_سرڪايو;
        جڳهه = 0;
    }

    pool->stats[جڳهه].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_چڪر->لاگرٿم, 0,
                   "slab alloc: %uz slot: %ui", سائيز, جڳهه);

    slots = ngx_slab_slots(pool);
    صفحو = slots[جڳهه].ايندڙ;

    إذا (صفحو->ايندڙ != صفحو) {

        إذا (سرڪايو < ngx_slab_exact_سرڪايو) {

            bitmap = (uintptr_t *) ngx_slab_صفحو_addr(pool, صفحو);

            map = (ngx_pagesize >> سرڪايو) / (8 * حجم(uintptr_t));

            لكل (n = 0; n < map; n++) {

                إذا (bitmap[n] != NGX_SLAB_BUSY) {

                    لكل (m = 1, i = 0; m; m <<= 1, i++) {
                        إذا (bitmap[n] & m) {
                            استمر;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * حجم(uintptr_t) + i) << سرڪايو;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[جڳهه].استعمال_ٿيل++;

                        إذا (bitmap[n] == NGX_SLAB_BUSY) {
                            لكل (n = n + 1; n < map; n++) {
                                إذا (bitmap[n] != NGX_SLAB_BUSY) {
                                    اذهب پورو;
                                }
                            }

                            prev = ngx_slab_صفحو_prev(صفحو);
                            prev->ايندڙ = صفحو->ايندڙ;
                            صفحو->ايندڙ->prev = صفحو->prev;

                            صفحو->ايندڙ = عدم;
                            صفحو->prev = NGX_SLAB_SMALL;
                        }

                        اذهب پورو;
                    }
                }
            }

        } وإلا إذا (سرڪايو == ngx_slab_exact_سرڪايو) {

            لكل (m = 1, i = 0; m; m <<= 1, i++) {
                إذا (صفحو->slab & m) {
                    استمر;
                }

                صفحو->slab |= m;

                إذا (صفحو->slab == NGX_SLAB_BUSY) {
                    prev = ngx_slab_صفحو_prev(صفحو);
                    prev->ايندڙ = صفحو->ايندڙ;
                    صفحو->ايندڙ->prev = صفحو->prev;

                    صفحو->ايندڙ = عدم;
                    صفحو->prev = NGX_SLAB_EXACT;
                }

                p = ngx_slab_صفحو_addr(pool, صفحو) + (i << سرڪايو);

                pool->stats[جڳهه].استعمال_ٿيل++;

                اذهب پورو;
            }

        } وإلا { /* shift > ngx_slab_exact_shift */

            نقاب = ((uintptr_t) 1 << (ngx_pagesize >> سرڪايو)) - 1;
            نقاب <<= NGX_SLAB_MAP_سرڪايو;

            لكل (m = (uintptr_t) 1 << NGX_SLAB_MAP_سرڪايو, i = 0;
                 m & نقاب;
                 m <<= 1, i++)
            {
                إذا (صفحو->slab & m) {
                    استمر;
                }

                صفحو->slab |= m;

                إذا ((صفحو->slab & NGX_SLAB_MAP_نقاب) == نقاب) {
                    prev = ngx_slab_صفحو_prev(صفحو);
                    prev->ايندڙ = صفحو->ايندڙ;
                    صفحو->ايندڙ->prev = صفحو->prev;

                    صفحو->ايندڙ = عدم;
                    صفحو->prev = NGX_SLAB_BIG;
                }

                p = ngx_slab_صفحو_addr(pool, صفحو) + (i << سرڪايو);

                pool->stats[جڳهه].استعمال_ٿيل++;

                اذهب پورو;
            }
        }

        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_alloc(): page is busy");
        ngx_debug_نقطو();
    }

    صفحو = ngx_slab_alloc_صفحا(pool, 1);

    إذا (صفحو) {
        إذا (سرڪايو < ngx_slab_exact_سرڪايو) {
            bitmap = (uintptr_t *) ngx_slab_صفحو_addr(pool, صفحو);

            n = (ngx_pagesize >> سرڪايو) / ((1 << سرڪايو) * 8);

            إذا (n == 0) {
                n = 1;
            }

            /* "n" elements for bitmap, plus one requested */

            لكل (i = 0; i < (n + 1) / (8 * حجم(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * حجم(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> سرڪايو) / (8 * حجم(uintptr_t));

            لكل (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            صفحو->slab = سرڪايو;
            صفحو->ايندڙ = &slots[جڳهه];
            صفحو->prev = (uintptr_t) &slots[جڳهه] | NGX_SLAB_SMALL;

            slots[جڳهه].ايندڙ = صفحو;

            pool->stats[جڳهه].ڪل += (ngx_pagesize >> سرڪايو) - n;

            p = ngx_slab_صفحو_addr(pool, صفحو) + (n << سرڪايو);

            pool->stats[جڳهه].استعمال_ٿيل++;

            اذهب پورو;

        } وإلا إذا (سرڪايو == ngx_slab_exact_سرڪايو) {

            صفحو->slab = 1;
            صفحو->ايندڙ = &slots[جڳهه];
            صفحو->prev = (uintptr_t) &slots[جڳهه] | NGX_SLAB_EXACT;

            slots[جڳهه].ايندڙ = صفحو;

            pool->stats[جڳهه].ڪل += 8 * حجم(uintptr_t);

            p = ngx_slab_صفحو_addr(pool, صفحو);

            pool->stats[جڳهه].استعمال_ٿيل++;

            اذهب پورو;

        } وإلا { /* shift > ngx_slab_exact_shift */

            صفحو->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_سرڪايو) | سرڪايو;
            صفحو->ايندڙ = &slots[جڳهه];
            صفحو->prev = (uintptr_t) &slots[جڳهه] | NGX_SLAB_BIG;

            slots[جڳهه].ايندڙ = صفحو;

            pool->stats[جڳهه].ڪل += ngx_pagesize >> سرڪايو;

            p = ngx_slab_صفحو_addr(pool, صفحو);

            pool->stats[جڳهه].استعمال_ٿيل++;

            اذهب پورو;
        }
    }

    p = 0;

    pool->stats[جڳهه].fails++;

پورو:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_چڪر->لاگرٿم, 0,
                   "slab alloc: %p", (فراغ *) p);

    أرجع (فراغ *) p;
}


فراغ *
ngx_slab_calloc(ngx_slab_pool_t *pool, سائيز_t سائيز)
{
    فراغ  *p;

    ngx_shmtx_تالو(&pool->mutex);

    p = ngx_slab_calloc_locked(pool, سائيز);

    ngx_shmtx_unlock(&pool->mutex);

    أرجع p;
}


فراغ *
ngx_slab_calloc_locked(ngx_slab_pool_t *pool, سائيز_t سائيز)
{
    فراغ  *p;

    p = ngx_slab_alloc_locked(pool, سائيز);
    إذا (p) {
        ngx_memzero(p, سائيز);
    }

    أرجع p;
}


فراغ
ngx_slab_free(ngx_slab_pool_t *pool, فراغ *p)
{
    ngx_shmtx_تالو(&pool->mutex);

    ngx_slab_free_locked(pool, p);

    ngx_shmtx_unlock(&pool->mutex);
}


فراغ
ngx_slab_free_locked(ngx_slab_pool_t *pool, فراغ *p)
{
    سائيز_t            سائيز;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, جڳهه, سرڪايو, map;
    ngx_slab_صفحو_t  *slots, *صفحو;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_چڪر->لاگرٿم, 0, "slab free: %p", p);

    إذا ((u_char *) p < pool->شروعات || (u_char *) p > pool->پڇاڙي) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        اذهب fail;
    }

    n = ((u_char *) p - pool->شروعات) >> ngx_pagesize_سرڪايو;
    صفحو = &pool->صفحا[n];
    slab = صفحو->slab;
    type = ngx_slab_صفحو_type(صفحو);

    بدل (type) {

    حالة NGX_SLAB_SMALL:

        سرڪايو = slab & NGX_SLAB_سرڪايو_نقاب;
        سائيز = (سائيز_t) 1 << سرڪايو;

        إذا ((uintptr_t) p & (سائيز - 1)) {
            اذهب wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> سرڪايو;
        m = (uintptr_t) 1 << (n % (8 * حجم(uintptr_t)));
        n /= 8 * حجم(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        إذا (bitmap[n] & m) {
            جڳهه = سرڪايو - pool->min_سرڪايو;

            إذا (صفحو->ايندڙ == عدم) {
                slots = ngx_slab_slots(pool);

                صفحو->ايندڙ = slots[جڳهه].ايندڙ;
                slots[جڳهه].ايندڙ = صفحو;

                صفحو->prev = (uintptr_t) &slots[جڳهه] | NGX_SLAB_SMALL;
                صفحو->ايندڙ->prev = (uintptr_t) صفحو | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> سرڪايو) / ((1 << سرڪايو) * 8);

            إذا (n == 0) {
                n = 1;
            }

            i = n / (8 * حجم(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * حجم(uintptr_t)))) - 1;

            إذا (bitmap[i] & ~m) {
                اذهب پورو;
            }

            map = (ngx_pagesize >> سرڪايو) / (8 * حجم(uintptr_t));

            لكل (i = i + 1; i < map; i++) {
                إذا (bitmap[i]) {
                    اذهب پورو;
                }
            }

            ngx_slab_free_صفحا(pool, صفحو, 1);

            pool->stats[جڳهه].ڪل -= (ngx_pagesize >> سرڪايو) - n;

            اذهب پورو;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_سرڪايو);
        سائيز = ngx_slab_exact_سائيز;

        إذا ((uintptr_t) p & (سائيز - 1)) {
            اذهب wrong_chunk;
        }

        إذا (slab & m) {
            جڳهه = ngx_slab_exact_سرڪايو - pool->min_سرڪايو;

            إذا (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                صفحو->ايندڙ = slots[جڳهه].ايندڙ;
                slots[جڳهه].ايندڙ = صفحو;

                صفحو->prev = (uintptr_t) &slots[جڳهه] | NGX_SLAB_EXACT;
                صفحو->ايندڙ->prev = (uintptr_t) صفحو | NGX_SLAB_EXACT;
            }

            صفحو->slab &= ~m;

            إذا (صفحو->slab) {
                اذهب پورو;
            }

            ngx_slab_free_صفحا(pool, صفحو, 1);

            pool->stats[جڳهه].ڪل -= 8 * حجم(uintptr_t);

            اذهب پورو;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_BIG:

        سرڪايو = slab & NGX_SLAB_سرڪايو_نقاب;
        سائيز = (سائيز_t) 1 << سرڪايو;

        إذا ((uintptr_t) p & (سائيز - 1)) {
            اذهب wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> سرڪايو)
                              + NGX_SLAB_MAP_سرڪايو);

        إذا (slab & m) {
            جڳهه = سرڪايو - pool->min_سرڪايو;

            إذا (صفحو->ايندڙ == عدم) {
                slots = ngx_slab_slots(pool);

                صفحو->ايندڙ = slots[جڳهه].ايندڙ;
                slots[جڳهه].ايندڙ = صفحو;

                صفحو->prev = (uintptr_t) &slots[جڳهه] | NGX_SLAB_BIG;
                صفحو->ايندڙ->prev = (uintptr_t) صفحو | NGX_SLAB_BIG;
            }

            صفحو->slab &= ~m;

            إذا (صفحو->slab & NGX_SLAB_MAP_نقاب) {
                اذهب پورو;
            }

            ngx_slab_free_صفحا(pool, صفحو, 1);

            pool->stats[جڳهه].ڪل -= ngx_pagesize >> سرڪايو;

            اذهب پورو;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_صفحو:

        إذا ((uintptr_t) p & (ngx_pagesize - 1)) {
            اذهب wrong_chunk;
        }

        إذا (!(slab & NGX_SLAB_صفحو_شروعات)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            اذهب fail;
        }

        إذا (slab == NGX_SLAB_صفحو_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            اذهب fail;
        }

        سائيز = slab & ~NGX_SLAB_صفحو_شروعات;

        ngx_slab_free_صفحا(pool, صفحو, سائيز);

        ngx_slab_junk(p, سائيز << ngx_pagesize_سرڪايو);

        أرجع;
    }

    /* not reached */

    أرجع;

پورو:

    pool->stats[جڳهه].استعمال_ٿيل--;

    ngx_slab_junk(p, سائيز);

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


ساكن ngx_slab_صفحو_t *
ngx_slab_alloc_صفحا(ngx_slab_pool_t *pool, ngx_uint_t صفحا)
{
    ngx_slab_صفحو_t  *صفحو, *p;

    لكل (صفحو = pool->حرر.ايندڙ; صفحو != &pool->حرر; صفحو = صفحو->ايندڙ) {

        إذا (صفحو->slab >= صفحا) {

            إذا (صفحو->slab > صفحا) {
                صفحو[صفحو->slab - 1].prev = (uintptr_t) &صفحو[صفحا];

                صفحو[صفحا].slab = صفحو->slab - صفحا;
                صفحو[صفحا].ايندڙ = صفحو->ايندڙ;
                صفحو[صفحا].prev = صفحو->prev;

                p = (ngx_slab_صفحو_t *) صفحو->prev;
                p->ايندڙ = &صفحو[صفحا];
                صفحو->ايندڙ->prev = (uintptr_t) &صفحو[صفحا];

            } وإلا {
                p = (ngx_slab_صفحو_t *) صفحو->prev;
                p->ايندڙ = صفحو->ايندڙ;
                صفحو->ايندڙ->prev = صفحو->prev;
            }

            صفحو->slab = صفحا | NGX_SLAB_صفحو_شروعات;
            صفحو->ايندڙ = عدم;
            صفحو->prev = NGX_SLAB_صفحو;

            pool->pfree -= صفحا;

            إذا (--صفحا == 0) {
                أرجع صفحو;
            }

            لكل (p = صفحو + 1; صفحا; صفحا--) {
                p->slab = NGX_SLAB_صفحو_BUSY;
                p->ايندڙ = عدم;
                p->prev = NGX_SLAB_صفحو;
                p++;
            }

            أرجع صفحو;
        }
    }

    إذا (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    أرجع عدم;
}


ساكن فراغ
ngx_slab_free_صفحا(ngx_slab_pool_t *pool, ngx_slab_صفحو_t *صفحو,
    ngx_uint_t صفحا)
{
    ngx_slab_صفحو_t  *prev, *join;

    pool->pfree += صفحا;

    صفحو->slab = صفحا--;

    إذا (صفحا) {
        ngx_memzero(&صفحو[1], صفحا * حجم(ngx_slab_صفحو_t));
    }

    إذا (صفحو->ايندڙ) {
        prev = ngx_slab_صفحو_prev(صفحو);
        prev->ايندڙ = صفحو->ايندڙ;
        صفحو->ايندڙ->prev = صفحو->prev;
    }

    join = صفحو + صفحو->slab;

    إذا (join < pool->آخري) {

        إذا (ngx_slab_صفحو_type(join) == NGX_SLAB_صفحو) {

            إذا (join->ايندڙ != عدم) {
                صفحا += join->slab;
                صفحو->slab += join->slab;

                prev = ngx_slab_صفحو_prev(join);
                prev->ايندڙ = join->ايندڙ;
                join->ايندڙ->prev = join->prev;

                join->slab = NGX_SLAB_صفحو_FREE;
                join->ايندڙ = عدم;
                join->prev = NGX_SLAB_صفحو;
            }
        }
    }

    إذا (صفحو > pool->صفحا) {
        join = صفحو - 1;

        إذا (ngx_slab_صفحو_type(join) == NGX_SLAB_صفحو) {

            إذا (join->slab == NGX_SLAB_صفحو_FREE) {
                join = ngx_slab_صفحو_prev(join);
            }

            إذا (join->ايندڙ != عدم) {
                صفحا += join->slab;
                join->slab += صفحو->slab;

                prev = ngx_slab_صفحو_prev(join);
                prev->ايندڙ = join->ايندڙ;
                join->ايندڙ->prev = join->prev;

                صفحو->slab = NGX_SLAB_صفحو_FREE;
                صفحو->ايندڙ = عدم;
                صفحو->prev = NGX_SLAB_صفحو;

                صفحو = join;
            }
        }
    }

    إذا (صفحا) {
        صفحو[صفحا].prev = (uintptr_t) صفحو;
    }

    صفحو->prev = (uintptr_t) &pool->حرر;
    صفحو->ايندڙ = pool->حرر.ايندڙ;

    صفحو->ايندڙ->prev = (uintptr_t) صفحو;

    pool->حرر.ايندڙ = صفحو;
}


ساكن فراغ
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, حرف *متن)
{
    ngx_log_error(level, ngx_چڪر->لاگرٿم, 0, "%s%s", متن, pool->log_ctx);
}
