
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کرو NGX_SLAB_ورقہ_نقاب   3
#تعریف_کرو NGX_SLAB_ورقہ        0
#تعریف_کرو NGX_SLAB_BIG         1
#تعریف_کرو NGX_SLAB_EXACT       2
#تعریف_کرو NGX_SLAB_SMALL       3

#إذا (NGX_PTR_ناپ == 4)

#تعریف_کرو NGX_SLAB_ورقہ_FREE   0
#تعریف_کرو NGX_SLAB_ورقہ_BUSY   0xffffffff
#تعریف_کرو NGX_SLAB_ورقہ_شروع  0x80000000

#تعریف_کرو NGX_SLAB_کھسکاؤ_نقاب  0x0000000f
#تعریف_کرو NGX_SLAB_MAP_نقاب    0xffff0000
#تعریف_کرو NGX_SLAB_MAP_کھسکاؤ   16

#تعریف_کرو NGX_SLAB_BUSY        0xffffffff

#وإلا /* (NGX_PTR_SIZE == 8) */

#تعریف_کرو NGX_SLAB_ورقہ_FREE   0
#تعریف_کرو NGX_SLAB_ورقہ_BUSY   0xffffffffffffffff
#تعریف_کرو NGX_SLAB_ورقہ_شروع  0x8000000000000000

#تعریف_کرو NGX_SLAB_کھسکاؤ_نقاب  0x000000000000000f
#تعریف_کرو NGX_SLAB_MAP_نقاب    0xffffffff00000000
#تعریف_کرو NGX_SLAB_MAP_کھسکاؤ   32

#تعریف_کرو NGX_SLAB_BUSY        0xffffffffffffffff

#جے_انت


#تعریف_کرو ngx_slab_slots(pool)                                                  \
    (ngx_slab_ورقہ_t *) ((u_char *) (pool) + حجم(ngx_slab_pool_t))

#تعریف_کرو ngx_slab_ورقہ_type(ورقہ)   ((ورقہ)->prev & NGX_SLAB_ورقہ_نقاب)

#تعریف_کرو ngx_slab_ورقہ_prev(ورقہ)                                              \
    (ngx_slab_ورقہ_t *) ((ورقہ)->prev & ~NGX_SLAB_ورقہ_نقاب)

#تعریف_کرو ngx_slab_ورقہ_addr(pool, ورقہ)                                        \
    ((((ورقہ) - (pool)->ورقے) << ngx_pagesize_کھسکاؤ)                         \
     + (uintptr_t) (pool)->شروع)


#إذا (NGX_DEBUG_MALLOC)

#تعریف_کرو ngx_slab_junk(p, ناپ)     ngx_memset(p, 0xA5, ناپ)

#وإلاإذا (NGX_HAVE_DEBUG_MALLOC)

#تعریف_کرو ngx_slab_junk(p, ناپ)                                                \
    إذا (ngx_debug_malloc)          ngx_memset(p, 0xA5, ناپ)

#وإلا

#تعریف_کرو ngx_slab_junk(p, ناپ)

#جے_انت

ساكن ngx_slab_ورقہ_t *ngx_slab_alloc_ورقے(ngx_slab_pool_t *pool,
    ngx_uint_t ورقے);
ساكن فراغ ngx_slab_free_ورقے(ngx_slab_pool_t *pool, ngx_slab_ورقہ_t *ورقہ,
    ngx_uint_t ورقے);
ساكن فراغ ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    حرف *متن);


ساكن ngx_uint_t  ngx_slab_max_ناپ;
ساكن ngx_uint_t  ngx_slab_exact_ناپ;
ساكن ngx_uint_t  ngx_slab_exact_کھسکاؤ;


فراغ
ngx_slab_sizes_init(فراغ)
{
    ngx_uint_t  n;

    ngx_slab_max_ناپ = ngx_pagesize / 2;
    ngx_slab_exact_ناپ = ngx_pagesize / (8 * حجم(uintptr_t));
    لكل (n = ngx_slab_exact_ناپ; n >>= 1; ngx_slab_exact_کھسکاؤ++) {
        /* void */
    }
}


فراغ
ngx_slab_init(ngx_slab_pool_t *pool)
{
    u_char           *p;
    ناپ_t            ناپ;
    ngx_int_t         m;
    ngx_uint_t        i, n, ورقے;
    ngx_slab_ورقہ_t  *slots, *ورقہ;

    pool->min_ناپ = (ناپ_t) 1 << pool->min_کھسکاؤ;

    slots = ngx_slab_slots(pool);

    p = (u_char *) slots;
    ناپ = pool->سرا - p;

    ngx_slab_junk(p, ناپ);

    n = ngx_pagesize_کھسکاؤ - pool->min_کھسکاؤ;

    لكل (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].اگلا = &slots[i];
        slots[i].prev = 0;
    }

    p += n * حجم(ngx_slab_ورقہ_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * حجم(ngx_slab_stat_t));

    p += n * حجم(ngx_slab_stat_t);

    ناپ -= n * (حجم(ngx_slab_ورقہ_t) + حجم(ngx_slab_stat_t));

    ورقے = (ngx_uint_t) (ناپ / (ngx_pagesize + حجم(ngx_slab_ورقہ_t)));

    pool->ورقے = (ngx_slab_ورقہ_t *) p;
    ngx_memzero(pool->ورقے, ورقے * حجم(ngx_slab_ورقہ_t));

    ورقہ = pool->ورقے;

    /* only "next" is used in list head */
    pool->حرر.slab = 0;
    pool->حرر.اگلا = ورقہ;
    pool->حرر.prev = 0;

    ورقہ->slab = ورقے;
    ورقہ->اگلا = &pool->حرر;
    ورقہ->prev = (uintptr_t) &pool->حرر;

    pool->شروع = ngx_align_ptr(p + ورقے * حجم(ngx_slab_ورقہ_t),
                                ngx_pagesize);

    m = ورقے - (pool->سرا - pool->شروع) / ngx_pagesize;
    إذا (m > 0) {
        ورقے -= m;
        ورقہ->slab = ورقے;
    }

    pool->آخری = pool->ورقے + ورقے;
    pool->pfree = ورقے;

    pool->log_nomem = 1;
    pool->log_ctx = &pool->zero;
    pool->zero = '\0';
}


فراغ *
ngx_slab_alloc(ngx_slab_pool_t *pool, ناپ_t ناپ)
{
    فراغ  *p;

    ngx_shmtx_جندرا(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, ناپ);

    ngx_shmtx_unlock(&pool->mutex);

    أرجع p;
}


فراغ *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, ناپ_t ناپ)
{
    ناپ_t            s;
    uintptr_t         p, m, نقاب, *bitmap;
    ngx_uint_t        i, n, تھاں, کھسکاؤ, map;
    ngx_slab_ورقہ_t  *ورقہ, *prev, *slots;

    إذا (ناپ > ngx_slab_max_ناپ) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_چکر->لوگارتھم, 0,
                       "slab alloc: %uz", ناپ);

        ورقہ = ngx_slab_alloc_ورقے(pool, (ناپ >> ngx_pagesize_کھسکاؤ)
                                          + ((ناپ % ngx_pagesize) ? 1 : 0));
        إذا (ورقہ) {
            p = ngx_slab_ورقہ_addr(pool, ورقہ);

        } وإلا {
            p = 0;
        }

        اذهب پورا;
    }

    إذا (ناپ > pool->min_ناپ) {
        کھسکاؤ = 1;
        لكل (s = ناپ - 1; s >>= 1; کھسکاؤ++) { /* void */ }
        تھاں = کھسکاؤ - pool->min_کھسکاؤ;

    } وإلا {
        کھسکاؤ = pool->min_کھسکاؤ;
        تھاں = 0;
    }

    pool->stats[تھاں].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_چکر->لوگارتھم, 0,
                   "slab alloc: %uz slot: %ui", ناپ, تھاں);

    slots = ngx_slab_slots(pool);
    ورقہ = slots[تھاں].اگلا;

    إذا (ورقہ->اگلا != ورقہ) {

        إذا (کھسکاؤ < ngx_slab_exact_کھسکاؤ) {

            bitmap = (uintptr_t *) ngx_slab_ورقہ_addr(pool, ورقہ);

            map = (ngx_pagesize >> کھسکاؤ) / (8 * حجم(uintptr_t));

            لكل (n = 0; n < map; n++) {

                إذا (bitmap[n] != NGX_SLAB_BUSY) {

                    لكل (m = 1, i = 0; m; m <<= 1, i++) {
                        إذا (bitmap[n] & m) {
                            استمر;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * حجم(uintptr_t) + i) << کھسکاؤ;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[تھاں].ورتیا++;

                        إذا (bitmap[n] == NGX_SLAB_BUSY) {
                            لكل (n = n + 1; n < map; n++) {
                                إذا (bitmap[n] != NGX_SLAB_BUSY) {
                                    اذهب پورا;
                                }
                            }

                            prev = ngx_slab_ورقہ_prev(ورقہ);
                            prev->اگلا = ورقہ->اگلا;
                            ورقہ->اگلا->prev = ورقہ->prev;

                            ورقہ->اگلا = عدم;
                            ورقہ->prev = NGX_SLAB_SMALL;
                        }

                        اذهب پورا;
                    }
                }
            }

        } وإلا إذا (کھسکاؤ == ngx_slab_exact_کھسکاؤ) {

            لكل (m = 1, i = 0; m; m <<= 1, i++) {
                إذا (ورقہ->slab & m) {
                    استمر;
                }

                ورقہ->slab |= m;

                إذا (ورقہ->slab == NGX_SLAB_BUSY) {
                    prev = ngx_slab_ورقہ_prev(ورقہ);
                    prev->اگلا = ورقہ->اگلا;
                    ورقہ->اگلا->prev = ورقہ->prev;

                    ورقہ->اگلا = عدم;
                    ورقہ->prev = NGX_SLAB_EXACT;
                }

                p = ngx_slab_ورقہ_addr(pool, ورقہ) + (i << کھسکاؤ);

                pool->stats[تھاں].ورتیا++;

                اذهب پورا;
            }

        } وإلا { /* shift > ngx_slab_exact_shift */

            نقاب = ((uintptr_t) 1 << (ngx_pagesize >> کھسکاؤ)) - 1;
            نقاب <<= NGX_SLAB_MAP_کھسکاؤ;

            لكل (m = (uintptr_t) 1 << NGX_SLAB_MAP_کھسکاؤ, i = 0;
                 m & نقاب;
                 m <<= 1, i++)
            {
                إذا (ورقہ->slab & m) {
                    استمر;
                }

                ورقہ->slab |= m;

                إذا ((ورقہ->slab & NGX_SLAB_MAP_نقاب) == نقاب) {
                    prev = ngx_slab_ورقہ_prev(ورقہ);
                    prev->اگلا = ورقہ->اگلا;
                    ورقہ->اگلا->prev = ورقہ->prev;

                    ورقہ->اگلا = عدم;
                    ورقہ->prev = NGX_SLAB_BIG;
                }

                p = ngx_slab_ورقہ_addr(pool, ورقہ) + (i << کھسکاؤ);

                pool->stats[تھاں].ورتیا++;

                اذهب پورا;
            }
        }

        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_alloc(): page is busy");
        ngx_debug_نقطہ();
    }

    ورقہ = ngx_slab_alloc_ورقے(pool, 1);

    إذا (ورقہ) {
        إذا (کھسکاؤ < ngx_slab_exact_کھسکاؤ) {
            bitmap = (uintptr_t *) ngx_slab_ورقہ_addr(pool, ورقہ);

            n = (ngx_pagesize >> کھسکاؤ) / ((1 << کھسکاؤ) * 8);

            إذا (n == 0) {
                n = 1;
            }

            /* "n" elements for bitmap, plus one requested */

            لكل (i = 0; i < (n + 1) / (8 * حجم(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * حجم(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> کھسکاؤ) / (8 * حجم(uintptr_t));

            لكل (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            ورقہ->slab = کھسکاؤ;
            ورقہ->اگلا = &slots[تھاں];
            ورقہ->prev = (uintptr_t) &slots[تھاں] | NGX_SLAB_SMALL;

            slots[تھاں].اگلا = ورقہ;

            pool->stats[تھاں].کل += (ngx_pagesize >> کھسکاؤ) - n;

            p = ngx_slab_ورقہ_addr(pool, ورقہ) + (n << کھسکاؤ);

            pool->stats[تھاں].ورتیا++;

            اذهب پورا;

        } وإلا إذا (کھسکاؤ == ngx_slab_exact_کھسکاؤ) {

            ورقہ->slab = 1;
            ورقہ->اگلا = &slots[تھاں];
            ورقہ->prev = (uintptr_t) &slots[تھاں] | NGX_SLAB_EXACT;

            slots[تھاں].اگلا = ورقہ;

            pool->stats[تھاں].کل += 8 * حجم(uintptr_t);

            p = ngx_slab_ورقہ_addr(pool, ورقہ);

            pool->stats[تھاں].ورتیا++;

            اذهب پورا;

        } وإلا { /* shift > ngx_slab_exact_shift */

            ورقہ->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_کھسکاؤ) | کھسکاؤ;
            ورقہ->اگلا = &slots[تھاں];
            ورقہ->prev = (uintptr_t) &slots[تھاں] | NGX_SLAB_BIG;

            slots[تھاں].اگلا = ورقہ;

            pool->stats[تھاں].کل += ngx_pagesize >> کھسکاؤ;

            p = ngx_slab_ورقہ_addr(pool, ورقہ);

            pool->stats[تھاں].ورتیا++;

            اذهب پورا;
        }
    }

    p = 0;

    pool->stats[تھاں].fails++;

پورا:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_چکر->لوگارتھم, 0,
                   "slab alloc: %p", (فراغ *) p);

    أرجع (فراغ *) p;
}


فراغ *
ngx_slab_calloc(ngx_slab_pool_t *pool, ناپ_t ناپ)
{
    فراغ  *p;

    ngx_shmtx_جندرا(&pool->mutex);

    p = ngx_slab_calloc_locked(pool, ناپ);

    ngx_shmtx_unlock(&pool->mutex);

    أرجع p;
}


فراغ *
ngx_slab_calloc_locked(ngx_slab_pool_t *pool, ناپ_t ناپ)
{
    فراغ  *p;

    p = ngx_slab_alloc_locked(pool, ناپ);
    إذا (p) {
        ngx_memzero(p, ناپ);
    }

    أرجع p;
}


فراغ
ngx_slab_free(ngx_slab_pool_t *pool, فراغ *p)
{
    ngx_shmtx_جندرا(&pool->mutex);

    ngx_slab_free_locked(pool, p);

    ngx_shmtx_unlock(&pool->mutex);
}


فراغ
ngx_slab_free_locked(ngx_slab_pool_t *pool, فراغ *p)
{
    ناپ_t            ناپ;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, تھاں, کھسکاؤ, map;
    ngx_slab_ورقہ_t  *slots, *ورقہ;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_چکر->لوگارتھم, 0, "slab free: %p", p);

    إذا ((u_char *) p < pool->شروع || (u_char *) p > pool->سرا) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        اذهب fail;
    }

    n = ((u_char *) p - pool->شروع) >> ngx_pagesize_کھسکاؤ;
    ورقہ = &pool->ورقے[n];
    slab = ورقہ->slab;
    type = ngx_slab_ورقہ_type(ورقہ);

    بدل (type) {

    حالة NGX_SLAB_SMALL:

        کھسکاؤ = slab & NGX_SLAB_کھسکاؤ_نقاب;
        ناپ = (ناپ_t) 1 << کھسکاؤ;

        إذا ((uintptr_t) p & (ناپ - 1)) {
            اذهب wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> کھسکاؤ;
        m = (uintptr_t) 1 << (n % (8 * حجم(uintptr_t)));
        n /= 8 * حجم(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        إذا (bitmap[n] & m) {
            تھاں = کھسکاؤ - pool->min_کھسکاؤ;

            إذا (ورقہ->اگلا == عدم) {
                slots = ngx_slab_slots(pool);

                ورقہ->اگلا = slots[تھاں].اگلا;
                slots[تھاں].اگلا = ورقہ;

                ورقہ->prev = (uintptr_t) &slots[تھاں] | NGX_SLAB_SMALL;
                ورقہ->اگلا->prev = (uintptr_t) ورقہ | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> کھسکاؤ) / ((1 << کھسکاؤ) * 8);

            إذا (n == 0) {
                n = 1;
            }

            i = n / (8 * حجم(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * حجم(uintptr_t)))) - 1;

            إذا (bitmap[i] & ~m) {
                اذهب پورا;
            }

            map = (ngx_pagesize >> کھسکاؤ) / (8 * حجم(uintptr_t));

            لكل (i = i + 1; i < map; i++) {
                إذا (bitmap[i]) {
                    اذهب پورا;
                }
            }

            ngx_slab_free_ورقے(pool, ورقہ, 1);

            pool->stats[تھاں].کل -= (ngx_pagesize >> کھسکاؤ) - n;

            اذهب پورا;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_کھسکاؤ);
        ناپ = ngx_slab_exact_ناپ;

        إذا ((uintptr_t) p & (ناپ - 1)) {
            اذهب wrong_chunk;
        }

        إذا (slab & m) {
            تھاں = ngx_slab_exact_کھسکاؤ - pool->min_کھسکاؤ;

            إذا (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                ورقہ->اگلا = slots[تھاں].اگلا;
                slots[تھاں].اگلا = ورقہ;

                ورقہ->prev = (uintptr_t) &slots[تھاں] | NGX_SLAB_EXACT;
                ورقہ->اگلا->prev = (uintptr_t) ورقہ | NGX_SLAB_EXACT;
            }

            ورقہ->slab &= ~m;

            إذا (ورقہ->slab) {
                اذهب پورا;
            }

            ngx_slab_free_ورقے(pool, ورقہ, 1);

            pool->stats[تھاں].کل -= 8 * حجم(uintptr_t);

            اذهب پورا;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_BIG:

        کھسکاؤ = slab & NGX_SLAB_کھسکاؤ_نقاب;
        ناپ = (ناپ_t) 1 << کھسکاؤ;

        إذا ((uintptr_t) p & (ناپ - 1)) {
            اذهب wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> کھسکاؤ)
                              + NGX_SLAB_MAP_کھسکاؤ);

        إذا (slab & m) {
            تھاں = کھسکاؤ - pool->min_کھسکاؤ;

            إذا (ورقہ->اگلا == عدم) {
                slots = ngx_slab_slots(pool);

                ورقہ->اگلا = slots[تھاں].اگلا;
                slots[تھاں].اگلا = ورقہ;

                ورقہ->prev = (uintptr_t) &slots[تھاں] | NGX_SLAB_BIG;
                ورقہ->اگلا->prev = (uintptr_t) ورقہ | NGX_SLAB_BIG;
            }

            ورقہ->slab &= ~m;

            إذا (ورقہ->slab & NGX_SLAB_MAP_نقاب) {
                اذهب پورا;
            }

            ngx_slab_free_ورقے(pool, ورقہ, 1);

            pool->stats[تھاں].کل -= ngx_pagesize >> کھسکاؤ;

            اذهب پورا;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_ورقہ:

        إذا ((uintptr_t) p & (ngx_pagesize - 1)) {
            اذهب wrong_chunk;
        }

        إذا (!(slab & NGX_SLAB_ورقہ_شروع)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            اذهب fail;
        }

        إذا (slab == NGX_SLAB_ورقہ_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            اذهب fail;
        }

        ناپ = slab & ~NGX_SLAB_ورقہ_شروع;

        ngx_slab_free_ورقے(pool, ورقہ, ناپ);

        ngx_slab_junk(p, ناپ << ngx_pagesize_کھسکاؤ);

        أرجع;
    }

    /* not reached */

    أرجع;

پورا:

    pool->stats[تھاں].ورتیا--;

    ngx_slab_junk(p, ناپ);

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


ساكن ngx_slab_ورقہ_t *
ngx_slab_alloc_ورقے(ngx_slab_pool_t *pool, ngx_uint_t ورقے)
{
    ngx_slab_ورقہ_t  *ورقہ, *p;

    لكل (ورقہ = pool->حرر.اگلا; ورقہ != &pool->حرر; ورقہ = ورقہ->اگلا) {

        إذا (ورقہ->slab >= ورقے) {

            إذا (ورقہ->slab > ورقے) {
                ورقہ[ورقہ->slab - 1].prev = (uintptr_t) &ورقہ[ورقے];

                ورقہ[ورقے].slab = ورقہ->slab - ورقے;
                ورقہ[ورقے].اگلا = ورقہ->اگلا;
                ورقہ[ورقے].prev = ورقہ->prev;

                p = (ngx_slab_ورقہ_t *) ورقہ->prev;
                p->اگلا = &ورقہ[ورقے];
                ورقہ->اگلا->prev = (uintptr_t) &ورقہ[ورقے];

            } وإلا {
                p = (ngx_slab_ورقہ_t *) ورقہ->prev;
                p->اگلا = ورقہ->اگلا;
                ورقہ->اگلا->prev = ورقہ->prev;
            }

            ورقہ->slab = ورقے | NGX_SLAB_ورقہ_شروع;
            ورقہ->اگلا = عدم;
            ورقہ->prev = NGX_SLAB_ورقہ;

            pool->pfree -= ورقے;

            إذا (--ورقے == 0) {
                أرجع ورقہ;
            }

            لكل (p = ورقہ + 1; ورقے; ورقے--) {
                p->slab = NGX_SLAB_ورقہ_BUSY;
                p->اگلا = عدم;
                p->prev = NGX_SLAB_ورقہ;
                p++;
            }

            أرجع ورقہ;
        }
    }

    إذا (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    أرجع عدم;
}


ساكن فراغ
ngx_slab_free_ورقے(ngx_slab_pool_t *pool, ngx_slab_ورقہ_t *ورقہ,
    ngx_uint_t ورقے)
{
    ngx_slab_ورقہ_t  *prev, *join;

    pool->pfree += ورقے;

    ورقہ->slab = ورقے--;

    إذا (ورقے) {
        ngx_memzero(&ورقہ[1], ورقے * حجم(ngx_slab_ورقہ_t));
    }

    إذا (ورقہ->اگلا) {
        prev = ngx_slab_ورقہ_prev(ورقہ);
        prev->اگلا = ورقہ->اگلا;
        ورقہ->اگلا->prev = ورقہ->prev;
    }

    join = ورقہ + ورقہ->slab;

    إذا (join < pool->آخری) {

        إذا (ngx_slab_ورقہ_type(join) == NGX_SLAB_ورقہ) {

            إذا (join->اگلا != عدم) {
                ورقے += join->slab;
                ورقہ->slab += join->slab;

                prev = ngx_slab_ورقہ_prev(join);
                prev->اگلا = join->اگلا;
                join->اگلا->prev = join->prev;

                join->slab = NGX_SLAB_ورقہ_FREE;
                join->اگلا = عدم;
                join->prev = NGX_SLAB_ورقہ;
            }
        }
    }

    إذا (ورقہ > pool->ورقے) {
        join = ورقہ - 1;

        إذا (ngx_slab_ورقہ_type(join) == NGX_SLAB_ورقہ) {

            إذا (join->slab == NGX_SLAB_ورقہ_FREE) {
                join = ngx_slab_ورقہ_prev(join);
            }

            إذا (join->اگلا != عدم) {
                ورقے += join->slab;
                join->slab += ورقہ->slab;

                prev = ngx_slab_ورقہ_prev(join);
                prev->اگلا = join->اگلا;
                join->اگلا->prev = join->prev;

                ورقہ->slab = NGX_SLAB_ورقہ_FREE;
                ورقہ->اگلا = عدم;
                ورقہ->prev = NGX_SLAB_ورقہ;

                ورقہ = join;
            }
        }
    }

    إذا (ورقے) {
        ورقہ[ورقے].prev = (uintptr_t) ورقہ;
    }

    ورقہ->prev = (uintptr_t) &pool->حرر;
    ورقہ->اگلا = pool->حرر.اگلا;

    ورقہ->اگلا->prev = (uintptr_t) ورقہ;

    pool->حرر.اگلا = ورقہ;
}


ساكن فراغ
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, حرف *متن)
{
    ngx_log_error(level, ngx_چکر->لوگارتھم, 0, "%s%s", متن, pool->log_ctx);
}
