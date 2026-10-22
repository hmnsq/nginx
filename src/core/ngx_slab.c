
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_ਪੰਨਾ_ਨਕਾਬ   3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_ਪੰਨਾ        0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_BIG         1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_EXACT       2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_SMALL       3

#ਜੇ (NGX_PTR_ਮਾਪ == 4)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_ਪੰਨਾ_FREE   0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_ਪੰਨਾ_BUSY   0xffffffff
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_ਪੰਨਾ_ਸ਼ੁਰੂ  0x80000000

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_ਖਿਸਕਾਓ_ਨਕਾਬ  0x0000000f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_MAP_ਨਕਾਬ    0xffff0000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_MAP_ਖਿਸਕਾਓ   16

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_BUSY        0xffffffff

#ਨਹੀਂਤਾਂ /* (NGX_PTR_SIZE == 8) */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_ਪੰਨਾ_FREE   0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_ਪੰਨਾ_BUSY   0xffffffffffffffff
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_ਪੰਨਾ_ਸ਼ੁਰੂ  0x8000000000000000

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_ਖਿਸਕਾਓ_ਨਕਾਬ  0x000000000000000f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_MAP_ਨਕਾਬ    0xffffffff00000000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_MAP_ਖਿਸਕਾਓ   32

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SLAB_BUSY        0xffffffffffffffff

#ਜੇ_ਅੰਤ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_slab_slots(pool)                                                  \
    (ngx_slab_ਪੰਨਾ_t *) ((u_char *) (pool) + ਆਕਾਰ(ngx_slab_pool_t))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_slab_ਪੰਨਾ_type(ਪੰਨਾ)   ((ਪੰਨਾ)->prev & NGX_SLAB_ਪੰਨਾ_ਨਕਾਬ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_slab_ਪੰਨਾ_prev(ਪੰਨਾ)                                              \
    (ngx_slab_ਪੰਨਾ_t *) ((ਪੰਨਾ)->prev & ~NGX_SLAB_ਪੰਨਾ_ਨਕਾਬ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_slab_ਪੰਨਾ_addr(pool, ਪੰਨਾ)                                        \
    ((((ਪੰਨਾ) - (pool)->ਪੰਨੇ) << ngx_pagesize_ਖਿਸਕਾਓ)                         \
     + (uintptr_t) (pool)->ਸ਼ੁਰੂ)


#ਜੇ (NGX_DEBUG_MALLOC)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_slab_junk(p, ਮਾਪ)     ngx_memset(p, 0xA5, ਮਾਪ)

#ਜਾਂਜੇ (NGX_HAVE_DEBUG_MALLOC)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_slab_junk(p, ਮਾਪ)                                                \
    ਜੇ (ngx_debug_malloc)          ngx_memset(p, 0xA5, ਮਾਪ)

#ਨਹੀਂਤਾਂ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_slab_junk(p, ਮਾਪ)

#ਜੇ_ਅੰਤ

ਸਥਿਰ ngx_slab_ਪੰਨਾ_t *ngx_slab_alloc_ਪੰਨੇ(ngx_slab_pool_t *pool,
    ngx_uint_t ਪੰਨੇ);
ਸਥਿਰ ਖਾਲੀ ngx_slab_free_ਪੰਨੇ(ngx_slab_pool_t *pool, ngx_slab_ਪੰਨਾ_t *ਪੰਨਾ,
    ngx_uint_t ਪੰਨੇ);
ਸਥਿਰ ਖਾਲੀ ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    ਅੱਖਰ *ਪਾਠ);


ਸਥਿਰ ngx_uint_t  ngx_slab_max_ਮਾਪ;
ਸਥਿਰ ngx_uint_t  ngx_slab_exact_ਮਾਪ;
ਸਥਿਰ ngx_uint_t  ngx_slab_exact_ਖਿਸਕਾਓ;


ਖਾਲੀ
ngx_slab_sizes_init(ਖਾਲੀ)
{
    ngx_uint_t  n;

    ngx_slab_max_ਮਾਪ = ngx_pagesize / 2;
    ngx_slab_exact_ਮਾਪ = ngx_pagesize / (8 * ਆਕਾਰ(uintptr_t));
    ਲਈ (n = ngx_slab_exact_ਮਾਪ; n >>= 1; ngx_slab_exact_ਖਿਸਕਾਓ++) {
        /* void */
    }
}


ਖਾਲੀ
ngx_slab_init(ngx_slab_pool_t *pool)
{
    u_char           *p;
    ਮਾਪ_t            ਮਾਪ;
    ngx_int_t         m;
    ngx_uint_t        i, n, ਪੰਨੇ;
    ngx_slab_ਪੰਨਾ_t  *slots, *ਪੰਨਾ;

    pool->min_ਮਾਪ = (ਮਾਪ_t) 1 << pool->min_ਖਿਸਕਾਓ;

    slots = ngx_slab_slots(pool);

    p = (u_char *) slots;
    ਮਾਪ = pool->ਸਿਰਾ - p;

    ngx_slab_junk(p, ਮਾਪ);

    n = ngx_pagesize_ਖਿਸਕਾਓ - pool->min_ਖਿਸਕਾਓ;

    ਲਈ (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].ਅਗਲਾ = &slots[i];
        slots[i].prev = 0;
    }

    p += n * ਆਕਾਰ(ngx_slab_ਪੰਨਾ_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * ਆਕਾਰ(ngx_slab_stat_t));

    p += n * ਆਕਾਰ(ngx_slab_stat_t);

    ਮਾਪ -= n * (ਆਕਾਰ(ngx_slab_ਪੰਨਾ_t) + ਆਕਾਰ(ngx_slab_stat_t));

    ਪੰਨੇ = (ngx_uint_t) (ਮਾਪ / (ngx_pagesize + ਆਕਾਰ(ngx_slab_ਪੰਨਾ_t)));

    pool->ਪੰਨੇ = (ngx_slab_ਪੰਨਾ_t *) p;
    ngx_memzero(pool->ਪੰਨੇ, ਪੰਨੇ * ਆਕਾਰ(ngx_slab_ਪੰਨਾ_t));

    ਪੰਨਾ = pool->ਪੰਨੇ;

    /* only "next" is used in list head */
    pool->ਮੁਕਤ.slab = 0;
    pool->ਮੁਕਤ.ਅਗਲਾ = ਪੰਨਾ;
    pool->ਮੁਕਤ.prev = 0;

    ਪੰਨਾ->slab = ਪੰਨੇ;
    ਪੰਨਾ->ਅਗਲਾ = &pool->ਮੁਕਤ;
    ਪੰਨਾ->prev = (uintptr_t) &pool->ਮੁਕਤ;

    pool->ਸ਼ੁਰੂ = ngx_align_ptr(p + ਪੰਨੇ * ਆਕਾਰ(ngx_slab_ਪੰਨਾ_t),
                                ngx_pagesize);

    m = ਪੰਨੇ - (pool->ਸਿਰਾ - pool->ਸ਼ੁਰੂ) / ngx_pagesize;
    ਜੇ (m > 0) {
        ਪੰਨੇ -= m;
        ਪੰਨਾ->slab = ਪੰਨੇ;
    }

    pool->ਆਖਰੀ = pool->ਪੰਨੇ + ਪੰਨੇ;
    pool->pfree = ਪੰਨੇ;

    pool->log_nomem = 1;
    pool->log_ctx = &pool->zero;
    pool->zero = '\0';
}


ਖਾਲੀ *
ngx_slab_alloc(ngx_slab_pool_t *pool, ਮਾਪ_t ਮਾਪ)
{
    ਖਾਲੀ  *p;

    ngx_shmtx_ਤਾਲਾ(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, ਮਾਪ);

    ngx_shmtx_unlock(&pool->mutex);

    ਵਾਪਸ p;
}


ਖਾਲੀ *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, ਮਾਪ_t ਮਾਪ)
{
    ਮਾਪ_t            s;
    uintptr_t         p, m, ਨਕਾਬ, *bitmap;
    ngx_uint_t        i, n, ਥਾਂ, ਖਿਸਕਾਓ, map;
    ngx_slab_ਪੰਨਾ_t  *ਪੰਨਾ, *prev, *slots;

    ਜੇ (ਮਾਪ > ngx_slab_max_ਮਾਪ) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                       "slab alloc: %uz", ਮਾਪ);

        ਪੰਨਾ = ngx_slab_alloc_ਪੰਨੇ(pool, (ਮਾਪ >> ngx_pagesize_ਖਿਸਕਾਓ)
                                          + ((ਮਾਪ % ngx_pagesize) ? 1 : 0));
        ਜੇ (ਪੰਨਾ) {
            p = ngx_slab_ਪੰਨਾ_addr(pool, ਪੰਨਾ);

        } ਨਹੀਂਤਾਂ {
            p = 0;
        }

        ਜਾਓ ਪੂਰਾ;
    }

    ਜੇ (ਮਾਪ > pool->min_ਮਾਪ) {
        ਖਿਸਕਾਓ = 1;
        ਲਈ (s = ਮਾਪ - 1; s >>= 1; ਖਿਸਕਾਓ++) { /* void */ }
        ਥਾਂ = ਖਿਸਕਾਓ - pool->min_ਖਿਸਕਾਓ;

    } ਨਹੀਂਤਾਂ {
        ਖਿਸਕਾਓ = pool->min_ਖਿਸਕਾਓ;
        ਥਾਂ = 0;
    }

    pool->stats[ਥਾਂ].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                   "slab alloc: %uz slot: %ui", ਮਾਪ, ਥਾਂ);

    slots = ngx_slab_slots(pool);
    ਪੰਨਾ = slots[ਥਾਂ].ਅਗਲਾ;

    ਜੇ (ਪੰਨਾ->ਅਗਲਾ != ਪੰਨਾ) {

        ਜੇ (ਖਿਸਕਾਓ < ngx_slab_exact_ਖਿਸਕਾਓ) {

            bitmap = (uintptr_t *) ngx_slab_ਪੰਨਾ_addr(pool, ਪੰਨਾ);

            map = (ngx_pagesize >> ਖਿਸਕਾਓ) / (8 * ਆਕਾਰ(uintptr_t));

            ਲਈ (n = 0; n < map; n++) {

                ਜੇ (bitmap[n] != NGX_SLAB_BUSY) {

                    ਲਈ (m = 1, i = 0; m; m <<= 1, i++) {
                        ਜੇ (bitmap[n] & m) {
                            ਜਾਰੀ;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * ਆਕਾਰ(uintptr_t) + i) << ਖਿਸਕਾਓ;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[ਥਾਂ].ਵਰਤਿਆ++;

                        ਜੇ (bitmap[n] == NGX_SLAB_BUSY) {
                            ਲਈ (n = n + 1; n < map; n++) {
                                ਜੇ (bitmap[n] != NGX_SLAB_BUSY) {
                                    ਜਾਓ ਪੂਰਾ;
                                }
                            }

                            prev = ngx_slab_ਪੰਨਾ_prev(ਪੰਨਾ);
                            prev->ਅਗਲਾ = ਪੰਨਾ->ਅਗਲਾ;
                            ਪੰਨਾ->ਅਗਲਾ->prev = ਪੰਨਾ->prev;

                            ਪੰਨਾ->ਅਗਲਾ = ਨਿਰਾ;
                            ਪੰਨਾ->prev = NGX_SLAB_SMALL;
                        }

                        ਜਾਓ ਪੂਰਾ;
                    }
                }
            }

        } ਨਹੀਂਤਾਂ ਜੇ (ਖਿਸਕਾਓ == ngx_slab_exact_ਖਿਸਕਾਓ) {

            ਲਈ (m = 1, i = 0; m; m <<= 1, i++) {
                ਜੇ (ਪੰਨਾ->slab & m) {
                    ਜਾਰੀ;
                }

                ਪੰਨਾ->slab |= m;

                ਜੇ (ਪੰਨਾ->slab == NGX_SLAB_BUSY) {
                    prev = ngx_slab_ਪੰਨਾ_prev(ਪੰਨਾ);
                    prev->ਅਗਲਾ = ਪੰਨਾ->ਅਗਲਾ;
                    ਪੰਨਾ->ਅਗਲਾ->prev = ਪੰਨਾ->prev;

                    ਪੰਨਾ->ਅਗਲਾ = ਨਿਰਾ;
                    ਪੰਨਾ->prev = NGX_SLAB_EXACT;
                }

                p = ngx_slab_ਪੰਨਾ_addr(pool, ਪੰਨਾ) + (i << ਖਿਸਕਾਓ);

                pool->stats[ਥਾਂ].ਵਰਤਿਆ++;

                ਜਾਓ ਪੂਰਾ;
            }

        } ਨਹੀਂਤਾਂ { /* shift > ngx_slab_exact_shift */

            ਨਕਾਬ = ((uintptr_t) 1 << (ngx_pagesize >> ਖਿਸਕਾਓ)) - 1;
            ਨਕਾਬ <<= NGX_SLAB_MAP_ਖਿਸਕਾਓ;

            ਲਈ (m = (uintptr_t) 1 << NGX_SLAB_MAP_ਖਿਸਕਾਓ, i = 0;
                 m & ਨਕਾਬ;
                 m <<= 1, i++)
            {
                ਜੇ (ਪੰਨਾ->slab & m) {
                    ਜਾਰੀ;
                }

                ਪੰਨਾ->slab |= m;

                ਜੇ ((ਪੰਨਾ->slab & NGX_SLAB_MAP_ਨਕਾਬ) == ਨਕਾਬ) {
                    prev = ngx_slab_ਪੰਨਾ_prev(ਪੰਨਾ);
                    prev->ਅਗਲਾ = ਪੰਨਾ->ਅਗਲਾ;
                    ਪੰਨਾ->ਅਗਲਾ->prev = ਪੰਨਾ->prev;

                    ਪੰਨਾ->ਅਗਲਾ = ਨਿਰਾ;
                    ਪੰਨਾ->prev = NGX_SLAB_BIG;
                }

                p = ngx_slab_ਪੰਨਾ_addr(pool, ਪੰਨਾ) + (i << ਖਿਸਕਾਓ);

                pool->stats[ਥਾਂ].ਵਰਤਿਆ++;

                ਜਾਓ ਪੂਰਾ;
            }
        }

        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_alloc(): page is busy");
        ngx_debug_ਬਿੰਦੂ();
    }

    ਪੰਨਾ = ngx_slab_alloc_ਪੰਨੇ(pool, 1);

    ਜੇ (ਪੰਨਾ) {
        ਜੇ (ਖਿਸਕਾਓ < ngx_slab_exact_ਖਿਸਕਾਓ) {
            bitmap = (uintptr_t *) ngx_slab_ਪੰਨਾ_addr(pool, ਪੰਨਾ);

            n = (ngx_pagesize >> ਖਿਸਕਾਓ) / ((1 << ਖਿਸਕਾਓ) * 8);

            ਜੇ (n == 0) {
                n = 1;
            }

            /* "n" elements for bitmap, plus one requested */

            ਲਈ (i = 0; i < (n + 1) / (8 * ਆਕਾਰ(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * ਆਕਾਰ(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> ਖਿਸਕਾਓ) / (8 * ਆਕਾਰ(uintptr_t));

            ਲਈ (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            ਪੰਨਾ->slab = ਖਿਸਕਾਓ;
            ਪੰਨਾ->ਅਗਲਾ = &slots[ਥਾਂ];
            ਪੰਨਾ->prev = (uintptr_t) &slots[ਥਾਂ] | NGX_SLAB_SMALL;

            slots[ਥਾਂ].ਅਗਲਾ = ਪੰਨਾ;

            pool->stats[ਥਾਂ].ਕੁੱਲ += (ngx_pagesize >> ਖਿਸਕਾਓ) - n;

            p = ngx_slab_ਪੰਨਾ_addr(pool, ਪੰਨਾ) + (n << ਖਿਸਕਾਓ);

            pool->stats[ਥਾਂ].ਵਰਤਿਆ++;

            ਜਾਓ ਪੂਰਾ;

        } ਨਹੀਂਤਾਂ ਜੇ (ਖਿਸਕਾਓ == ngx_slab_exact_ਖਿਸਕਾਓ) {

            ਪੰਨਾ->slab = 1;
            ਪੰਨਾ->ਅਗਲਾ = &slots[ਥਾਂ];
            ਪੰਨਾ->prev = (uintptr_t) &slots[ਥਾਂ] | NGX_SLAB_EXACT;

            slots[ਥਾਂ].ਅਗਲਾ = ਪੰਨਾ;

            pool->stats[ਥਾਂ].ਕੁੱਲ += 8 * ਆਕਾਰ(uintptr_t);

            p = ngx_slab_ਪੰਨਾ_addr(pool, ਪੰਨਾ);

            pool->stats[ਥਾਂ].ਵਰਤਿਆ++;

            ਜਾਓ ਪੂਰਾ;

        } ਨਹੀਂਤਾਂ { /* shift > ngx_slab_exact_shift */

            ਪੰਨਾ->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_ਖਿਸਕਾਓ) | ਖਿਸਕਾਓ;
            ਪੰਨਾ->ਅਗਲਾ = &slots[ਥਾਂ];
            ਪੰਨਾ->prev = (uintptr_t) &slots[ਥਾਂ] | NGX_SLAB_BIG;

            slots[ਥਾਂ].ਅਗਲਾ = ਪੰਨਾ;

            pool->stats[ਥਾਂ].ਕੁੱਲ += ngx_pagesize >> ਖਿਸਕਾਓ;

            p = ngx_slab_ਪੰਨਾ_addr(pool, ਪੰਨਾ);

            pool->stats[ਥਾਂ].ਵਰਤਿਆ++;

            ਜਾਓ ਪੂਰਾ;
        }
    }

    p = 0;

    pool->stats[ਥਾਂ].fails++;

ਪੂਰਾ:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                   "slab alloc: %p", (ਖਾਲੀ *) p);

    ਵਾਪਸ (ਖਾਲੀ *) p;
}


ਖਾਲੀ *
ngx_slab_calloc(ngx_slab_pool_t *pool, ਮਾਪ_t ਮਾਪ)
{
    ਖਾਲੀ  *p;

    ngx_shmtx_ਤਾਲਾ(&pool->mutex);

    p = ngx_slab_calloc_locked(pool, ਮਾਪ);

    ngx_shmtx_unlock(&pool->mutex);

    ਵਾਪਸ p;
}


ਖਾਲੀ *
ngx_slab_calloc_locked(ngx_slab_pool_t *pool, ਮਾਪ_t ਮਾਪ)
{
    ਖਾਲੀ  *p;

    p = ngx_slab_alloc_locked(pool, ਮਾਪ);
    ਜੇ (p) {
        ngx_memzero(p, ਮਾਪ);
    }

    ਵਾਪਸ p;
}


ਖਾਲੀ
ngx_slab_free(ngx_slab_pool_t *pool, ਖਾਲੀ *p)
{
    ngx_shmtx_ਤਾਲਾ(&pool->mutex);

    ngx_slab_free_locked(pool, p);

    ngx_shmtx_unlock(&pool->mutex);
}


ਖਾਲੀ
ngx_slab_free_locked(ngx_slab_pool_t *pool, ਖਾਲੀ *p)
{
    ਮਾਪ_t            ਮਾਪ;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, ਥਾਂ, ਖਿਸਕਾਓ, map;
    ngx_slab_ਪੰਨਾ_t  *slots, *ਪੰਨਾ;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "slab free: %p", p);

    ਜੇ ((u_char *) p < pool->ਸ਼ੁਰੂ || (u_char *) p > pool->ਸਿਰਾ) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        ਜਾਓ fail;
    }

    n = ((u_char *) p - pool->ਸ਼ੁਰੂ) >> ngx_pagesize_ਖਿਸਕਾਓ;
    ਪੰਨਾ = &pool->ਪੰਨੇ[n];
    slab = ਪੰਨਾ->slab;
    type = ngx_slab_ਪੰਨਾ_type(ਪੰਨਾ);

    ਚੋਣ (type) {

    ਹਾਲਤ NGX_SLAB_SMALL:

        ਖਿਸਕਾਓ = slab & NGX_SLAB_ਖਿਸਕਾਓ_ਨਕਾਬ;
        ਮਾਪ = (ਮਾਪ_t) 1 << ਖਿਸਕਾਓ;

        ਜੇ ((uintptr_t) p & (ਮਾਪ - 1)) {
            ਜਾਓ wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> ਖਿਸਕਾਓ;
        m = (uintptr_t) 1 << (n % (8 * ਆਕਾਰ(uintptr_t)));
        n /= 8 * ਆਕਾਰ(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        ਜੇ (bitmap[n] & m) {
            ਥਾਂ = ਖਿਸਕਾਓ - pool->min_ਖਿਸਕਾਓ;

            ਜੇ (ਪੰਨਾ->ਅਗਲਾ == ਨਿਰਾ) {
                slots = ngx_slab_slots(pool);

                ਪੰਨਾ->ਅਗਲਾ = slots[ਥਾਂ].ਅਗਲਾ;
                slots[ਥਾਂ].ਅਗਲਾ = ਪੰਨਾ;

                ਪੰਨਾ->prev = (uintptr_t) &slots[ਥਾਂ] | NGX_SLAB_SMALL;
                ਪੰਨਾ->ਅਗਲਾ->prev = (uintptr_t) ਪੰਨਾ | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> ਖਿਸਕਾਓ) / ((1 << ਖਿਸਕਾਓ) * 8);

            ਜੇ (n == 0) {
                n = 1;
            }

            i = n / (8 * ਆਕਾਰ(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * ਆਕਾਰ(uintptr_t)))) - 1;

            ਜੇ (bitmap[i] & ~m) {
                ਜਾਓ ਪੂਰਾ;
            }

            map = (ngx_pagesize >> ਖਿਸਕਾਓ) / (8 * ਆਕਾਰ(uintptr_t));

            ਲਈ (i = i + 1; i < map; i++) {
                ਜੇ (bitmap[i]) {
                    ਜਾਓ ਪੂਰਾ;
                }
            }

            ngx_slab_free_ਪੰਨੇ(pool, ਪੰਨਾ, 1);

            pool->stats[ਥਾਂ].ਕੁੱਲ -= (ngx_pagesize >> ਖਿਸਕਾਓ) - n;

            ਜਾਓ ਪੂਰਾ;
        }

        ਜਾਓ chunk_already_free;

    ਹਾਲਤ NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_ਖਿਸਕਾਓ);
        ਮਾਪ = ngx_slab_exact_ਮਾਪ;

        ਜੇ ((uintptr_t) p & (ਮਾਪ - 1)) {
            ਜਾਓ wrong_chunk;
        }

        ਜੇ (slab & m) {
            ਥਾਂ = ngx_slab_exact_ਖਿਸਕਾਓ - pool->min_ਖਿਸਕਾਓ;

            ਜੇ (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                ਪੰਨਾ->ਅਗਲਾ = slots[ਥਾਂ].ਅਗਲਾ;
                slots[ਥਾਂ].ਅਗਲਾ = ਪੰਨਾ;

                ਪੰਨਾ->prev = (uintptr_t) &slots[ਥਾਂ] | NGX_SLAB_EXACT;
                ਪੰਨਾ->ਅਗਲਾ->prev = (uintptr_t) ਪੰਨਾ | NGX_SLAB_EXACT;
            }

            ਪੰਨਾ->slab &= ~m;

            ਜੇ (ਪੰਨਾ->slab) {
                ਜਾਓ ਪੂਰਾ;
            }

            ngx_slab_free_ਪੰਨੇ(pool, ਪੰਨਾ, 1);

            pool->stats[ਥਾਂ].ਕੁੱਲ -= 8 * ਆਕਾਰ(uintptr_t);

            ਜਾਓ ਪੂਰਾ;
        }

        ਜਾਓ chunk_already_free;

    ਹਾਲਤ NGX_SLAB_BIG:

        ਖਿਸਕਾਓ = slab & NGX_SLAB_ਖਿਸਕਾਓ_ਨਕਾਬ;
        ਮਾਪ = (ਮਾਪ_t) 1 << ਖਿਸਕਾਓ;

        ਜੇ ((uintptr_t) p & (ਮਾਪ - 1)) {
            ਜਾਓ wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> ਖਿਸਕਾਓ)
                              + NGX_SLAB_MAP_ਖਿਸਕਾਓ);

        ਜੇ (slab & m) {
            ਥਾਂ = ਖਿਸਕਾਓ - pool->min_ਖਿਸਕਾਓ;

            ਜੇ (ਪੰਨਾ->ਅਗਲਾ == ਨਿਰਾ) {
                slots = ngx_slab_slots(pool);

                ਪੰਨਾ->ਅਗਲਾ = slots[ਥਾਂ].ਅਗਲਾ;
                slots[ਥਾਂ].ਅਗਲਾ = ਪੰਨਾ;

                ਪੰਨਾ->prev = (uintptr_t) &slots[ਥਾਂ] | NGX_SLAB_BIG;
                ਪੰਨਾ->ਅਗਲਾ->prev = (uintptr_t) ਪੰਨਾ | NGX_SLAB_BIG;
            }

            ਪੰਨਾ->slab &= ~m;

            ਜੇ (ਪੰਨਾ->slab & NGX_SLAB_MAP_ਨਕਾਬ) {
                ਜਾਓ ਪੂਰਾ;
            }

            ngx_slab_free_ਪੰਨੇ(pool, ਪੰਨਾ, 1);

            pool->stats[ਥਾਂ].ਕੁੱਲ -= ngx_pagesize >> ਖਿਸਕਾਓ;

            ਜਾਓ ਪੂਰਾ;
        }

        ਜਾਓ chunk_already_free;

    ਹਾਲਤ NGX_SLAB_ਪੰਨਾ:

        ਜੇ ((uintptr_t) p & (ngx_pagesize - 1)) {
            ਜਾਓ wrong_chunk;
        }

        ਜੇ (!(slab & NGX_SLAB_ਪੰਨਾ_ਸ਼ੁਰੂ)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            ਜਾਓ fail;
        }

        ਜੇ (slab == NGX_SLAB_ਪੰਨਾ_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            ਜਾਓ fail;
        }

        ਮਾਪ = slab & ~NGX_SLAB_ਪੰਨਾ_ਸ਼ੁਰੂ;

        ngx_slab_free_ਪੰਨੇ(pool, ਪੰਨਾ, ਮਾਪ);

        ngx_slab_junk(p, ਮਾਪ << ngx_pagesize_ਖਿਸਕਾਓ);

        ਵਾਪਸ;
    }

    /* not reached */

    ਵਾਪਸ;

ਪੂਰਾ:

    pool->stats[ਥਾਂ].ਵਰਤਿਆ--;

    ngx_slab_junk(p, ਮਾਪ);

    ਵਾਪਸ;

wrong_chunk:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): pointer to wrong chunk");

    ਜਾਓ fail;

chunk_already_free:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): chunk is already free");

fail:

    ਵਾਪਸ;
}


ਸਥਿਰ ngx_slab_ਪੰਨਾ_t *
ngx_slab_alloc_ਪੰਨੇ(ngx_slab_pool_t *pool, ngx_uint_t ਪੰਨੇ)
{
    ngx_slab_ਪੰਨਾ_t  *ਪੰਨਾ, *p;

    ਲਈ (ਪੰਨਾ = pool->ਮੁਕਤ.ਅਗਲਾ; ਪੰਨਾ != &pool->ਮੁਕਤ; ਪੰਨਾ = ਪੰਨਾ->ਅਗਲਾ) {

        ਜੇ (ਪੰਨਾ->slab >= ਪੰਨੇ) {

            ਜੇ (ਪੰਨਾ->slab > ਪੰਨੇ) {
                ਪੰਨਾ[ਪੰਨਾ->slab - 1].prev = (uintptr_t) &ਪੰਨਾ[ਪੰਨੇ];

                ਪੰਨਾ[ਪੰਨੇ].slab = ਪੰਨਾ->slab - ਪੰਨੇ;
                ਪੰਨਾ[ਪੰਨੇ].ਅਗਲਾ = ਪੰਨਾ->ਅਗਲਾ;
                ਪੰਨਾ[ਪੰਨੇ].prev = ਪੰਨਾ->prev;

                p = (ngx_slab_ਪੰਨਾ_t *) ਪੰਨਾ->prev;
                p->ਅਗਲਾ = &ਪੰਨਾ[ਪੰਨੇ];
                ਪੰਨਾ->ਅਗਲਾ->prev = (uintptr_t) &ਪੰਨਾ[ਪੰਨੇ];

            } ਨਹੀਂਤਾਂ {
                p = (ngx_slab_ਪੰਨਾ_t *) ਪੰਨਾ->prev;
                p->ਅਗਲਾ = ਪੰਨਾ->ਅਗਲਾ;
                ਪੰਨਾ->ਅਗਲਾ->prev = ਪੰਨਾ->prev;
            }

            ਪੰਨਾ->slab = ਪੰਨੇ | NGX_SLAB_ਪੰਨਾ_ਸ਼ੁਰੂ;
            ਪੰਨਾ->ਅਗਲਾ = ਨਿਰਾ;
            ਪੰਨਾ->prev = NGX_SLAB_ਪੰਨਾ;

            pool->pfree -= ਪੰਨੇ;

            ਜੇ (--ਪੰਨੇ == 0) {
                ਵਾਪਸ ਪੰਨਾ;
            }

            ਲਈ (p = ਪੰਨਾ + 1; ਪੰਨੇ; ਪੰਨੇ--) {
                p->slab = NGX_SLAB_ਪੰਨਾ_BUSY;
                p->ਅਗਲਾ = ਨਿਰਾ;
                p->prev = NGX_SLAB_ਪੰਨਾ;
                p++;
            }

            ਵਾਪਸ ਪੰਨਾ;
        }
    }

    ਜੇ (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    ਵਾਪਸ ਨਿਰਾ;
}


ਸਥਿਰ ਖਾਲੀ
ngx_slab_free_ਪੰਨੇ(ngx_slab_pool_t *pool, ngx_slab_ਪੰਨਾ_t *ਪੰਨਾ,
    ngx_uint_t ਪੰਨੇ)
{
    ngx_slab_ਪੰਨਾ_t  *prev, *join;

    pool->pfree += ਪੰਨੇ;

    ਪੰਨਾ->slab = ਪੰਨੇ--;

    ਜੇ (ਪੰਨੇ) {
        ngx_memzero(&ਪੰਨਾ[1], ਪੰਨੇ * ਆਕਾਰ(ngx_slab_ਪੰਨਾ_t));
    }

    ਜੇ (ਪੰਨਾ->ਅਗਲਾ) {
        prev = ngx_slab_ਪੰਨਾ_prev(ਪੰਨਾ);
        prev->ਅਗਲਾ = ਪੰਨਾ->ਅਗਲਾ;
        ਪੰਨਾ->ਅਗਲਾ->prev = ਪੰਨਾ->prev;
    }

    join = ਪੰਨਾ + ਪੰਨਾ->slab;

    ਜੇ (join < pool->ਆਖਰੀ) {

        ਜੇ (ngx_slab_ਪੰਨਾ_type(join) == NGX_SLAB_ਪੰਨਾ) {

            ਜੇ (join->ਅਗਲਾ != ਨਿਰਾ) {
                ਪੰਨੇ += join->slab;
                ਪੰਨਾ->slab += join->slab;

                prev = ngx_slab_ਪੰਨਾ_prev(join);
                prev->ਅਗਲਾ = join->ਅਗਲਾ;
                join->ਅਗਲਾ->prev = join->prev;

                join->slab = NGX_SLAB_ਪੰਨਾ_FREE;
                join->ਅਗਲਾ = ਨਿਰਾ;
                join->prev = NGX_SLAB_ਪੰਨਾ;
            }
        }
    }

    ਜੇ (ਪੰਨਾ > pool->ਪੰਨੇ) {
        join = ਪੰਨਾ - 1;

        ਜੇ (ngx_slab_ਪੰਨਾ_type(join) == NGX_SLAB_ਪੰਨਾ) {

            ਜੇ (join->slab == NGX_SLAB_ਪੰਨਾ_FREE) {
                join = ngx_slab_ਪੰਨਾ_prev(join);
            }

            ਜੇ (join->ਅਗਲਾ != ਨਿਰਾ) {
                ਪੰਨੇ += join->slab;
                join->slab += ਪੰਨਾ->slab;

                prev = ngx_slab_ਪੰਨਾ_prev(join);
                prev->ਅਗਲਾ = join->ਅਗਲਾ;
                join->ਅਗਲਾ->prev = join->prev;

                ਪੰਨਾ->slab = NGX_SLAB_ਪੰਨਾ_FREE;
                ਪੰਨਾ->ਅਗਲਾ = ਨਿਰਾ;
                ਪੰਨਾ->prev = NGX_SLAB_ਪੰਨਾ;

                ਪੰਨਾ = join;
            }
        }
    }

    ਜੇ (ਪੰਨੇ) {
        ਪੰਨਾ[ਪੰਨੇ].prev = (uintptr_t) ਪੰਨਾ;
    }

    ਪੰਨਾ->prev = (uintptr_t) &pool->ਮੁਕਤ;
    ਪੰਨਾ->ਅਗਲਾ = pool->ਮੁਕਤ.ਅਗਲਾ;

    ਪੰਨਾ->ਅਗਲਾ->prev = (uintptr_t) ਪੰਨਾ;

    pool->ਮੁਕਤ.ਅਗਲਾ = ਪੰਨਾ;
}


ਸਥਿਰ ਖਾਲੀ
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, ਅੱਖਰ *ਪਾਠ)
{
    ngx_log_error(level, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "%s%s", ਪਾਠ, pool->log_ctx);
}
