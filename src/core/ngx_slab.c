
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#شامل <ngx_config.h>
#شامل <ngx_core.h>


#تعریف_کرو NGX_SLAB_صفحہ_نقاب   3
#تعریف_کرو NGX_SLAB_صفحہ        0
#تعریف_کرو NGX_SLAB_BIG         1
#تعریف_کرو NGX_SLAB_EXACT       2
#تعریف_کرو NGX_SLAB_SMALL       3

#اگر (NGX_PTR_سائز == 4)

#تعریف_کرو NGX_SLAB_صفحہ_FREE   0
#تعریف_کرو NGX_SLAB_صفحہ_BUSY   0xffffffff
#تعریف_کرو NGX_SLAB_صفحہ_آغاز  0x80000000

#تعریف_کرو NGX_SLAB_سرکاؤ_نقاب  0x0000000f
#تعریف_کرو NGX_SLAB_MAP_نقاب    0xffff0000
#تعریف_کرو NGX_SLAB_MAP_سرکاؤ   16

#تعریف_کرو NGX_SLAB_BUSY        0xffffffff

#ورنہ /* (NGX_PTR_SIZE == 8) */

#تعریف_کرو NGX_SLAB_صفحہ_FREE   0
#تعریف_کرو NGX_SLAB_صفحہ_BUSY   0xffffffffffffffff
#تعریف_کرو NGX_SLAB_صفحہ_آغاز  0x8000000000000000

#تعریف_کرو NGX_SLAB_سرکاؤ_نقاب  0x000000000000000f
#تعریف_کرو NGX_SLAB_MAP_نقاب    0xffffffff00000000
#تعریف_کرو NGX_SLAB_MAP_سرکاؤ   32

#تعریف_کرو NGX_SLAB_BUSY        0xffffffffffffffff

#اگر_ختم


#تعریف_کرو ngx_slab_slots(pool)                                                  \
    (ngx_slab_صفحہ_t *) ((u_char *) (pool) + حجم(ngx_slab_pool_t))

#تعریف_کرو ngx_slab_صفحہ_type(صفحہ)   ((صفحہ)->پچھلا & NGX_SLAB_صفحہ_نقاب)

#تعریف_کرو ngx_slab_صفحہ_پچھلا(صفحہ)                                              \
    (ngx_slab_صفحہ_t *) ((صفحہ)->پچھلا & ~NGX_SLAB_صفحہ_نقاب)

#تعریف_کرو ngx_slab_صفحہ_addr(pool, صفحہ)                                        \
    ((((صفحہ) - (pool)->صفحات) << ngx_pagesize_سرکاؤ)                         \
     + (uintptr_t) (pool)->آغاز)


#اگر (NGX_DEBUG_MALLOC)

#تعریف_کرو ngx_slab_junk(p, سائز)     ngx_memset(p, 0xA5, سائز)

#وگرنہ_اگر (NGX_HAVE_DEBUG_MALLOC)

#تعریف_کرو ngx_slab_junk(p, سائز)                                                \
    اگر (ngx_debug_malloc)          ngx_memset(p, 0xA5, سائز)

#ورنہ

#تعریف_کرو ngx_slab_junk(p, سائز)

#اگر_ختم

ساکن ngx_slab_صفحہ_t *ngx_slab_alloc_صفحات(ngx_slab_pool_t *pool,
    ngx_uint_t صفحات);
ساکن خالی ngx_slab_free_صفحات(ngx_slab_pool_t *pool, ngx_slab_صفحہ_t *صفحہ,
    ngx_uint_t صفحات);
ساکن خالی ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    حرف *متن);


ساکن ngx_uint_t  ngx_slab_max_سائز;
ساکن ngx_uint_t  ngx_slab_exact_سائز;
ساکن ngx_uint_t  ngx_slab_exact_سرکاؤ;


خالی
ngx_slab_sizes_init(خالی)
{
    ngx_uint_t  n;

    ngx_slab_max_سائز = ngx_pagesize / 2;
    ngx_slab_exact_سائز = ngx_pagesize / (8 * حجم(uintptr_t));
    برائے (n = ngx_slab_exact_سائز; n >>= 1; ngx_slab_exact_سرکاؤ++) {
        /* void */
    }
}


خالی
ngx_slab_init(ngx_slab_pool_t *pool)
{
    u_char           *p;
    سائز_t            سائز;
    ngx_int_t         m;
    ngx_uint_t        i, n, صفحات;
    ngx_slab_صفحہ_t  *slots, *صفحہ;

    pool->min_سائز = (سائز_t) 1 << pool->min_سرکاؤ;

    slots = ngx_slab_slots(pool);

    p = (u_char *) slots;
    سائز = pool->انتہا - p;

    ngx_slab_junk(p, سائز);

    n = ngx_pagesize_سرکاؤ - pool->min_سرکاؤ;

    برائے (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].اگلا = &slots[i];
        slots[i].پچھلا = 0;
    }

    p += n * حجم(ngx_slab_صفحہ_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * حجم(ngx_slab_stat_t));

    p += n * حجم(ngx_slab_stat_t);

    سائز -= n * (حجم(ngx_slab_صفحہ_t) + حجم(ngx_slab_stat_t));

    صفحات = (ngx_uint_t) (سائز / (ngx_pagesize + حجم(ngx_slab_صفحہ_t)));

    pool->صفحات = (ngx_slab_صفحہ_t *) p;
    ngx_memzero(pool->صفحات, صفحات * حجم(ngx_slab_صفحہ_t));

    صفحہ = pool->صفحات;

    /* only "next" is used in list head */
    pool->آزاد.slab = 0;
    pool->آزاد.اگلا = صفحہ;
    pool->آزاد.پچھلا = 0;

    صفحہ->slab = صفحات;
    صفحہ->اگلا = &pool->آزاد;
    صفحہ->پچھلا = (uintptr_t) &pool->آزاد;

    pool->آغاز = ngx_align_ptr(p + صفحات * حجم(ngx_slab_صفحہ_t),
                                ngx_pagesize);

    m = صفحات - (pool->انتہا - pool->آغاز) / ngx_pagesize;
    اگر (m > 0) {
        صفحات -= m;
        صفحہ->slab = صفحات;
    }

    pool->آخری = pool->صفحات + صفحات;
    pool->pfree = صفحات;

    pool->log_nomem = 1;
    pool->log_ctx = &pool->zero;
    pool->zero = '\0';
}


خالی *
ngx_slab_alloc(ngx_slab_pool_t *pool, سائز_t سائز)
{
    خالی  *p;

    ngx_shmtx_تالا(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, سائز);

    ngx_shmtx_unlock(&pool->mutex);

    واپس p;
}


خالی *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, سائز_t سائز)
{
    سائز_t            s;
    uintptr_t         p, m, نقاب, *bitmap;
    ngx_uint_t        i, n, خانہ, سرکاؤ, map;
    ngx_slab_صفحہ_t  *صفحہ, *پچھلا, *slots;

    اگر (سائز > ngx_slab_max_سائز) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_چکر->لوگارتھم, 0,
                       "slab alloc: %uz", سائز);

        صفحہ = ngx_slab_alloc_صفحات(pool, (سائز >> ngx_pagesize_سرکاؤ)
                                          + ((سائز % ngx_pagesize) ? 1 : 0));
        اگر (صفحہ) {
            p = ngx_slab_صفحہ_addr(pool, صفحہ);

        } ورنہ {
            p = 0;
        }

        جاؤ مکمل;
    }

    اگر (سائز > pool->min_سائز) {
        سرکاؤ = 1;
        برائے (s = سائز - 1; s >>= 1; سرکاؤ++) { /* void */ }
        خانہ = سرکاؤ - pool->min_سرکاؤ;

    } ورنہ {
        سرکاؤ = pool->min_سرکاؤ;
        خانہ = 0;
    }

    pool->stats[خانہ].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_چکر->لوگارتھم, 0,
                   "slab alloc: %uz slot: %ui", سائز, خانہ);

    slots = ngx_slab_slots(pool);
    صفحہ = slots[خانہ].اگلا;

    اگر (صفحہ->اگلا != صفحہ) {

        اگر (سرکاؤ < ngx_slab_exact_سرکاؤ) {

            bitmap = (uintptr_t *) ngx_slab_صفحہ_addr(pool, صفحہ);

            map = (ngx_pagesize >> سرکاؤ) / (8 * حجم(uintptr_t));

            برائے (n = 0; n < map; n++) {

                اگر (bitmap[n] != NGX_SLAB_BUSY) {

                    برائے (m = 1, i = 0; m; m <<= 1, i++) {
                        اگر (bitmap[n] & m) {
                            جاری;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * حجم(uintptr_t) + i) << سرکاؤ;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[خانہ].مستعمل++;

                        اگر (bitmap[n] == NGX_SLAB_BUSY) {
                            برائے (n = n + 1; n < map; n++) {
                                اگر (bitmap[n] != NGX_SLAB_BUSY) {
                                    جاؤ مکمل;
                                }
                            }

                            پچھلا = ngx_slab_صفحہ_پچھلا(صفحہ);
                            پچھلا->اگلا = صفحہ->اگلا;
                            صفحہ->اگلا->پچھلا = صفحہ->پچھلا;

                            صفحہ->اگلا = تہی;
                            صفحہ->پچھلا = NGX_SLAB_SMALL;
                        }

                        جاؤ مکمل;
                    }
                }
            }

        } ورنہ اگر (سرکاؤ == ngx_slab_exact_سرکاؤ) {

            برائے (m = 1, i = 0; m; m <<= 1, i++) {
                اگر (صفحہ->slab & m) {
                    جاری;
                }

                صفحہ->slab |= m;

                اگر (صفحہ->slab == NGX_SLAB_BUSY) {
                    پچھلا = ngx_slab_صفحہ_پچھلا(صفحہ);
                    پچھلا->اگلا = صفحہ->اگلا;
                    صفحہ->اگلا->پچھلا = صفحہ->پچھلا;

                    صفحہ->اگلا = تہی;
                    صفحہ->پچھلا = NGX_SLAB_EXACT;
                }

                p = ngx_slab_صفحہ_addr(pool, صفحہ) + (i << سرکاؤ);

                pool->stats[خانہ].مستعمل++;

                جاؤ مکمل;
            }

        } ورنہ { /* shift > ngx_slab_exact_shift */

            نقاب = ((uintptr_t) 1 << (ngx_pagesize >> سرکاؤ)) - 1;
            نقاب <<= NGX_SLAB_MAP_سرکاؤ;

            برائے (m = (uintptr_t) 1 << NGX_SLAB_MAP_سرکاؤ, i = 0;
                 m & نقاب;
                 m <<= 1, i++)
            {
                اگر (صفحہ->slab & m) {
                    جاری;
                }

                صفحہ->slab |= m;

                اگر ((صفحہ->slab & NGX_SLAB_MAP_نقاب) == نقاب) {
                    پچھلا = ngx_slab_صفحہ_پچھلا(صفحہ);
                    پچھلا->اگلا = صفحہ->اگلا;
                    صفحہ->اگلا->پچھلا = صفحہ->پچھلا;

                    صفحہ->اگلا = تہی;
                    صفحہ->پچھلا = NGX_SLAB_BIG;
                }

                p = ngx_slab_صفحہ_addr(pool, صفحہ) + (i << سرکاؤ);

                pool->stats[خانہ].مستعمل++;

                جاؤ مکمل;
            }
        }

        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_alloc(): page is busy");
        ngx_debug_نقطہ();
    }

    صفحہ = ngx_slab_alloc_صفحات(pool, 1);

    اگر (صفحہ) {
        اگر (سرکاؤ < ngx_slab_exact_سرکاؤ) {
            bitmap = (uintptr_t *) ngx_slab_صفحہ_addr(pool, صفحہ);

            n = (ngx_pagesize >> سرکاؤ) / ((1 << سرکاؤ) * 8);

            اگر (n == 0) {
                n = 1;
            }

            /* "n" elements for bitmap, plus one requested */

            برائے (i = 0; i < (n + 1) / (8 * حجم(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * حجم(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> سرکاؤ) / (8 * حجم(uintptr_t));

            برائے (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            صفحہ->slab = سرکاؤ;
            صفحہ->اگلا = &slots[خانہ];
            صفحہ->پچھلا = (uintptr_t) &slots[خانہ] | NGX_SLAB_SMALL;

            slots[خانہ].اگلا = صفحہ;

            pool->stats[خانہ].کل += (ngx_pagesize >> سرکاؤ) - n;

            p = ngx_slab_صفحہ_addr(pool, صفحہ) + (n << سرکاؤ);

            pool->stats[خانہ].مستعمل++;

            جاؤ مکمل;

        } ورنہ اگر (سرکاؤ == ngx_slab_exact_سرکاؤ) {

            صفحہ->slab = 1;
            صفحہ->اگلا = &slots[خانہ];
            صفحہ->پچھلا = (uintptr_t) &slots[خانہ] | NGX_SLAB_EXACT;

            slots[خانہ].اگلا = صفحہ;

            pool->stats[خانہ].کل += 8 * حجم(uintptr_t);

            p = ngx_slab_صفحہ_addr(pool, صفحہ);

            pool->stats[خانہ].مستعمل++;

            جاؤ مکمل;

        } ورنہ { /* shift > ngx_slab_exact_shift */

            صفحہ->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_سرکاؤ) | سرکاؤ;
            صفحہ->اگلا = &slots[خانہ];
            صفحہ->پچھلا = (uintptr_t) &slots[خانہ] | NGX_SLAB_BIG;

            slots[خانہ].اگلا = صفحہ;

            pool->stats[خانہ].کل += ngx_pagesize >> سرکاؤ;

            p = ngx_slab_صفحہ_addr(pool, صفحہ);

            pool->stats[خانہ].مستعمل++;

            جاؤ مکمل;
        }
    }

    p = 0;

    pool->stats[خانہ].fails++;

مکمل:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_چکر->لوگارتھم, 0,
                   "slab alloc: %p", (خالی *) p);

    واپس (خالی *) p;
}


خالی *
ngx_slab_calloc(ngx_slab_pool_t *pool, سائز_t سائز)
{
    خالی  *p;

    ngx_shmtx_تالا(&pool->mutex);

    p = ngx_slab_calloc_locked(pool, سائز);

    ngx_shmtx_unlock(&pool->mutex);

    واپس p;
}


خالی *
ngx_slab_calloc_locked(ngx_slab_pool_t *pool, سائز_t سائز)
{
    خالی  *p;

    p = ngx_slab_alloc_locked(pool, سائز);
    اگر (p) {
        ngx_memzero(p, سائز);
    }

    واپس p;
}


خالی
ngx_slab_free(ngx_slab_pool_t *pool, خالی *p)
{
    ngx_shmtx_تالا(&pool->mutex);

    ngx_slab_free_locked(pool, p);

    ngx_shmtx_unlock(&pool->mutex);
}


خالی
ngx_slab_free_locked(ngx_slab_pool_t *pool, خالی *p)
{
    سائز_t            سائز;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, خانہ, سرکاؤ, map;
    ngx_slab_صفحہ_t  *slots, *صفحہ;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_چکر->لوگارتھم, 0, "slab free: %p", p);

    اگر ((u_char *) p < pool->آغاز || (u_char *) p > pool->انتہا) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        جاؤ fail;
    }

    n = ((u_char *) p - pool->آغاز) >> ngx_pagesize_سرکاؤ;
    صفحہ = &pool->صفحات[n];
    slab = صفحہ->slab;
    type = ngx_slab_صفحہ_type(صفحہ);

    انتخاب (type) {

    حالت NGX_SLAB_SMALL:

        سرکاؤ = slab & NGX_SLAB_سرکاؤ_نقاب;
        سائز = (سائز_t) 1 << سرکاؤ;

        اگر ((uintptr_t) p & (سائز - 1)) {
            جاؤ wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> سرکاؤ;
        m = (uintptr_t) 1 << (n % (8 * حجم(uintptr_t)));
        n /= 8 * حجم(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        اگر (bitmap[n] & m) {
            خانہ = سرکاؤ - pool->min_سرکاؤ;

            اگر (صفحہ->اگلا == تہی) {
                slots = ngx_slab_slots(pool);

                صفحہ->اگلا = slots[خانہ].اگلا;
                slots[خانہ].اگلا = صفحہ;

                صفحہ->پچھلا = (uintptr_t) &slots[خانہ] | NGX_SLAB_SMALL;
                صفحہ->اگلا->پچھلا = (uintptr_t) صفحہ | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> سرکاؤ) / ((1 << سرکاؤ) * 8);

            اگر (n == 0) {
                n = 1;
            }

            i = n / (8 * حجم(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * حجم(uintptr_t)))) - 1;

            اگر (bitmap[i] & ~m) {
                جاؤ مکمل;
            }

            map = (ngx_pagesize >> سرکاؤ) / (8 * حجم(uintptr_t));

            برائے (i = i + 1; i < map; i++) {
                اگر (bitmap[i]) {
                    جاؤ مکمل;
                }
            }

            ngx_slab_free_صفحات(pool, صفحہ, 1);

            pool->stats[خانہ].کل -= (ngx_pagesize >> سرکاؤ) - n;

            جاؤ مکمل;
        }

        جاؤ chunk_already_free;

    حالت NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_سرکاؤ);
        سائز = ngx_slab_exact_سائز;

        اگر ((uintptr_t) p & (سائز - 1)) {
            جاؤ wrong_chunk;
        }

        اگر (slab & m) {
            خانہ = ngx_slab_exact_سرکاؤ - pool->min_سرکاؤ;

            اگر (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                صفحہ->اگلا = slots[خانہ].اگلا;
                slots[خانہ].اگلا = صفحہ;

                صفحہ->پچھلا = (uintptr_t) &slots[خانہ] | NGX_SLAB_EXACT;
                صفحہ->اگلا->پچھلا = (uintptr_t) صفحہ | NGX_SLAB_EXACT;
            }

            صفحہ->slab &= ~m;

            اگر (صفحہ->slab) {
                جاؤ مکمل;
            }

            ngx_slab_free_صفحات(pool, صفحہ, 1);

            pool->stats[خانہ].کل -= 8 * حجم(uintptr_t);

            جاؤ مکمل;
        }

        جاؤ chunk_already_free;

    حالت NGX_SLAB_BIG:

        سرکاؤ = slab & NGX_SLAB_سرکاؤ_نقاب;
        سائز = (سائز_t) 1 << سرکاؤ;

        اگر ((uintptr_t) p & (سائز - 1)) {
            جاؤ wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> سرکاؤ)
                              + NGX_SLAB_MAP_سرکاؤ);

        اگر (slab & m) {
            خانہ = سرکاؤ - pool->min_سرکاؤ;

            اگر (صفحہ->اگلا == تہی) {
                slots = ngx_slab_slots(pool);

                صفحہ->اگلا = slots[خانہ].اگلا;
                slots[خانہ].اگلا = صفحہ;

                صفحہ->پچھلا = (uintptr_t) &slots[خانہ] | NGX_SLAB_BIG;
                صفحہ->اگلا->پچھلا = (uintptr_t) صفحہ | NGX_SLAB_BIG;
            }

            صفحہ->slab &= ~m;

            اگر (صفحہ->slab & NGX_SLAB_MAP_نقاب) {
                جاؤ مکمل;
            }

            ngx_slab_free_صفحات(pool, صفحہ, 1);

            pool->stats[خانہ].کل -= ngx_pagesize >> سرکاؤ;

            جاؤ مکمل;
        }

        جاؤ chunk_already_free;

    حالت NGX_SLAB_صفحہ:

        اگر ((uintptr_t) p & (ngx_pagesize - 1)) {
            جاؤ wrong_chunk;
        }

        اگر (!(slab & NGX_SLAB_صفحہ_آغاز)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            جاؤ fail;
        }

        اگر (slab == NGX_SLAB_صفحہ_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            جاؤ fail;
        }

        سائز = slab & ~NGX_SLAB_صفحہ_آغاز;

        ngx_slab_free_صفحات(pool, صفحہ, سائز);

        ngx_slab_junk(p, سائز << ngx_pagesize_سرکاؤ);

        واپس;
    }

    /* not reached */

    واپس;

مکمل:

    pool->stats[خانہ].مستعمل--;

    ngx_slab_junk(p, سائز);

    واپس;

wrong_chunk:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): pointer to wrong chunk");

    جاؤ fail;

chunk_already_free:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): chunk is already free");

fail:

    واپس;
}


ساکن ngx_slab_صفحہ_t *
ngx_slab_alloc_صفحات(ngx_slab_pool_t *pool, ngx_uint_t صفحات)
{
    ngx_slab_صفحہ_t  *صفحہ, *p;

    برائے (صفحہ = pool->آزاد.اگلا; صفحہ != &pool->آزاد; صفحہ = صفحہ->اگلا) {

        اگر (صفحہ->slab >= صفحات) {

            اگر (صفحہ->slab > صفحات) {
                صفحہ[صفحہ->slab - 1].پچھلا = (uintptr_t) &صفحہ[صفحات];

                صفحہ[صفحات].slab = صفحہ->slab - صفحات;
                صفحہ[صفحات].اگلا = صفحہ->اگلا;
                صفحہ[صفحات].پچھلا = صفحہ->پچھلا;

                p = (ngx_slab_صفحہ_t *) صفحہ->پچھلا;
                p->اگلا = &صفحہ[صفحات];
                صفحہ->اگلا->پچھلا = (uintptr_t) &صفحہ[صفحات];

            } ورنہ {
                p = (ngx_slab_صفحہ_t *) صفحہ->پچھلا;
                p->اگلا = صفحہ->اگلا;
                صفحہ->اگلا->پچھلا = صفحہ->پچھلا;
            }

            صفحہ->slab = صفحات | NGX_SLAB_صفحہ_آغاز;
            صفحہ->اگلا = تہی;
            صفحہ->پچھلا = NGX_SLAB_صفحہ;

            pool->pfree -= صفحات;

            اگر (--صفحات == 0) {
                واپس صفحہ;
            }

            برائے (p = صفحہ + 1; صفحات; صفحات--) {
                p->slab = NGX_SLAB_صفحہ_BUSY;
                p->اگلا = تہی;
                p->پچھلا = NGX_SLAB_صفحہ;
                p++;
            }

            واپس صفحہ;
        }
    }

    اگر (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    واپس تہی;
}


ساکن خالی
ngx_slab_free_صفحات(ngx_slab_pool_t *pool, ngx_slab_صفحہ_t *صفحہ,
    ngx_uint_t صفحات)
{
    ngx_slab_صفحہ_t  *پچھلا, *join;

    pool->pfree += صفحات;

    صفحہ->slab = صفحات--;

    اگر (صفحات) {
        ngx_memzero(&صفحہ[1], صفحات * حجم(ngx_slab_صفحہ_t));
    }

    اگر (صفحہ->اگلا) {
        پچھلا = ngx_slab_صفحہ_پچھلا(صفحہ);
        پچھلا->اگلا = صفحہ->اگلا;
        صفحہ->اگلا->پچھلا = صفحہ->پچھلا;
    }

    join = صفحہ + صفحہ->slab;

    اگر (join < pool->آخری) {

        اگر (ngx_slab_صفحہ_type(join) == NGX_SLAB_صفحہ) {

            اگر (join->اگلا != تہی) {
                صفحات += join->slab;
                صفحہ->slab += join->slab;

                پچھلا = ngx_slab_صفحہ_پچھلا(join);
                پچھلا->اگلا = join->اگلا;
                join->اگلا->پچھلا = join->پچھلا;

                join->slab = NGX_SLAB_صفحہ_FREE;
                join->اگلا = تہی;
                join->پچھلا = NGX_SLAB_صفحہ;
            }
        }
    }

    اگر (صفحہ > pool->صفحات) {
        join = صفحہ - 1;

        اگر (ngx_slab_صفحہ_type(join) == NGX_SLAB_صفحہ) {

            اگر (join->slab == NGX_SLAB_صفحہ_FREE) {
                join = ngx_slab_صفحہ_پچھلا(join);
            }

            اگر (join->اگلا != تہی) {
                صفحات += join->slab;
                join->slab += صفحہ->slab;

                پچھلا = ngx_slab_صفحہ_پچھلا(join);
                پچھلا->اگلا = join->اگلا;
                join->اگلا->پچھلا = join->پچھلا;

                صفحہ->slab = NGX_SLAB_صفحہ_FREE;
                صفحہ->اگلا = تہی;
                صفحہ->پچھلا = NGX_SLAB_صفحہ;

                صفحہ = join;
            }
        }
    }

    اگر (صفحات) {
        صفحہ[صفحات].پچھلا = (uintptr_t) صفحہ;
    }

    صفحہ->پچھلا = (uintptr_t) &pool->آزاد;
    صفحہ->اگلا = pool->آزاد.اگلا;

    صفحہ->اگلا->پچھلا = (uintptr_t) صفحہ;

    pool->آزاد.اگلا = صفحہ;
}


ساکن خالی
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, حرف *متن)
{
    ngx_log_error(level, ngx_چکر->لوگارتھم, 0, "%s%s", متن, pool->log_ctx);
}
