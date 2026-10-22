
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_ಪುಟ_ಮುಖವಾಡ   3
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_ಪುಟ        0
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_BIG         1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_EXACT       2
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_SMALL       3

#ಆದರೆ (NGX_PTR_ಗಾತ್ರ == 4)

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_ಪುಟ_FREE   0
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_ಪುಟ_BUSY   0xffffffff
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_ಪುಟ_ಆರಂಭ  0x80000000

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_ಸರಿಸು_ಮುಖವಾಡ  0x0000000f
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_MAP_ಮುಖವಾಡ    0xffff0000
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_MAP_ಸರಿಸು   16

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_BUSY        0xffffffff

#ಇಲ್ಲದಿದ್ದರೆ /* (NGX_PTR_SIZE == 8) */

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_ಪುಟ_FREE   0
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_ಪುಟ_BUSY   0xffffffffffffffff
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_ಪುಟ_ಆರಂಭ  0x8000000000000000

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_ಸರಿಸು_ಮುಖವಾಡ  0x000000000000000f
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_MAP_ಮುಖವಾಡ    0xffffffff00000000
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_MAP_ಸರಿಸು   32

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SLAB_BUSY        0xffffffffffffffff

#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_slab_slots(pool)                                                  \
    (ngx_slab_ಪುಟ_t *) ((u_char *) (pool) + ಗಾತ್ರ(ngx_slab_pool_t))

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_slab_ಪುಟ_type(ಪುಟ)   ((ಪುಟ)->prev & NGX_SLAB_ಪುಟ_ಮುಖವಾಡ)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_slab_ಪುಟ_prev(ಪುಟ)                                              \
    (ngx_slab_ಪುಟ_t *) ((ಪುಟ)->prev & ~NGX_SLAB_ಪುಟ_ಮುಖವಾಡ)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_slab_ಪುಟ_addr(pool, ಪುಟ)                                        \
    ((((ಪುಟ) - (pool)->ಪುಟಗಳು) << ngx_pagesize_ಸರಿಸು)                         \
     + (uintptr_t) (pool)->ಆರಂಭ)


#ಆದರೆ (NGX_DEBUG_MALLOC)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_slab_junk(p, ಗಾತ್ರ_2)     ngx_memset(p, 0xA5, ಗಾತ್ರ_2)

#ಅಥವಾಆದರೆ (NGX_HAVE_DEBUG_MALLOC)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_slab_junk(p, ಗಾತ್ರ_2)                                                \
    ಆದರೆ (ngx_debug_malloc)          ngx_memset(p, 0xA5, ಗಾತ್ರ_2)

#ಇಲ್ಲದಿದ್ದರೆ

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_slab_junk(p, ಗಾತ್ರ_2)

#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ngx_slab_ಪುಟ_t *ngx_slab_alloc_ಪುಟಗಳು(ngx_slab_pool_t *pool,
    ngx_uint_t ಪುಟಗಳು);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_slab_free_ಪುಟಗಳು(ngx_slab_pool_t *pool, ngx_slab_ಪುಟ_t *ಪುಟ,
    ngx_uint_t ಪುಟಗಳು);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    ಅಕ್ಷರ *ಪಠ್ಯ);


ಸ್ಥಿರ ngx_uint_t  ngx_slab_max_ಗಾತ್ರ;
ಸ್ಥಿರ ngx_uint_t  ngx_slab_exact_ಗಾತ್ರ;
ಸ್ಥಿರ ngx_uint_t  ngx_slab_exact_ಸರಿಸು;


ಶೂನ್ಯ
ngx_slab_sizes_init(ಶೂನ್ಯ)
{
    ngx_uint_t  n;

    ngx_slab_max_ಗಾತ್ರ = ngx_pagesize / 2;
    ngx_slab_exact_ಗಾತ್ರ = ngx_pagesize / (8 * ಗಾತ್ರ(uintptr_t));
    ಗಾಗಿ (n = ngx_slab_exact_ಗಾತ್ರ; n >>= 1; ngx_slab_exact_ಸರಿಸು++) {
        /* void */
    }
}


ಶೂನ್ಯ
ngx_slab_init(ngx_slab_pool_t *pool)
{
    u_char           *p;
    ಗಾತ್ರ_t            ಗಾತ್ರ_2;
    ngx_int_t         m;
    ngx_uint_t        i, n, ಪುಟಗಳು;
    ngx_slab_ಪುಟ_t  *slots, *ಪುಟ;

    pool->min_ಗಾತ್ರ = (ಗಾತ್ರ_t) 1 << pool->min_ಸರಿಸು;

    slots = ngx_slab_slots(pool);

    p = (u_char *) slots;
    ಗಾತ್ರ_2 = pool->ಕೊನೆ - p;

    ngx_slab_junk(p, ಗಾತ್ರ_2);

    n = ngx_pagesize_ಸರಿಸು - pool->min_ಸರಿಸು;

    ಗಾಗಿ (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].ಮುಂದಿನ = &slots[i];
        slots[i].prev = 0;
    }

    p += n * ಗಾತ್ರ(ngx_slab_ಪುಟ_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * ಗಾತ್ರ(ngx_slab_stat_t));

    p += n * ಗಾತ್ರ(ngx_slab_stat_t);

    ಗಾತ್ರ_2 -= n * (ಗಾತ್ರ(ngx_slab_ಪುಟ_t) + ಗಾತ್ರ(ngx_slab_stat_t));

    ಪುಟಗಳು = (ngx_uint_t) (ಗಾತ್ರ_2 / (ngx_pagesize + ಗಾತ್ರ(ngx_slab_ಪುಟ_t)));

    pool->ಪುಟಗಳು = (ngx_slab_ಪುಟ_t *) p;
    ngx_memzero(pool->ಪುಟಗಳು, ಪುಟಗಳು * ಗಾತ್ರ(ngx_slab_ಪುಟ_t));

    ಪುಟ = pool->ಪುಟಗಳು;

    /* only "next" is used in list head */
    pool->ಬಿಡುಗಡೆ.slab = 0;
    pool->ಬಿಡುಗಡೆ.ಮುಂದಿನ = ಪುಟ;
    pool->ಬಿಡುಗಡೆ.prev = 0;

    ಪುಟ->slab = ಪುಟಗಳು;
    ಪುಟ->ಮುಂದಿನ = &pool->ಬಿಡುಗಡೆ;
    ಪುಟ->prev = (uintptr_t) &pool->ಬಿಡುಗಡೆ;

    pool->ಆರಂಭ = ngx_align_ptr(p + ಪುಟಗಳು * ಗಾತ್ರ(ngx_slab_ಪುಟ_t),
                                ngx_pagesize);

    m = ಪುಟಗಳು - (pool->ಕೊನೆ - pool->ಆರಂಭ) / ngx_pagesize;
    ಆದರೆ (m > 0) {
        ಪುಟಗಳು -= m;
        ಪುಟ->slab = ಪುಟಗಳು;
    }

    pool->ಕೊನೆಯ = pool->ಪುಟಗಳು + ಪುಟಗಳು;
    pool->pfree = ಪುಟಗಳು;

    pool->log_nomem = 1;
    pool->log_ctx = &pool->zero;
    pool->zero = '\0';
}


ಶೂನ್ಯ *
ngx_slab_alloc(ngx_slab_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಶೂನ್ಯ  *p;

    ngx_shmtx_ಬೀಗ(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, ಗಾತ್ರ_2);

    ngx_shmtx_unlock(&pool->mutex);

    ಹಿಂತಿರುಗಿ p;
}


ಶೂನ್ಯ *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಗಾತ್ರ_t            s;
    uintptr_t         p, m, ಮುಖವಾಡ, *bitmap;
    ngx_uint_t        i, n, ಸ್ಥಾನ, ಸರಿಸು, map;
    ngx_slab_ಪುಟ_t  *ಪುಟ, *prev, *slots;

    ಆದರೆ (ಗಾತ್ರ_2 > ngx_slab_max_ಗಾತ್ರ) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                       "slab alloc: %uz", ಗಾತ್ರ_2);

        ಪುಟ = ngx_slab_alloc_ಪುಟಗಳು(pool, (ಗಾತ್ರ_2 >> ngx_pagesize_ಸರಿಸು)
                                          + ((ಗಾತ್ರ_2 % ngx_pagesize) ? 1 : 0));
        ಆದರೆ (ಪುಟ) {
            p = ngx_slab_ಪುಟ_addr(pool, ಪುಟ);

        } ಇಲ್ಲದಿದ್ದರೆ {
            p = 0;
        }

        ಹೋಗು ಮುಗಿದ;
    }

    ಆದರೆ (ಗಾತ್ರ_2 > pool->min_ಗಾತ್ರ) {
        ಸರಿಸು = 1;
        ಗಾಗಿ (s = ಗಾತ್ರ_2 - 1; s >>= 1; ಸರಿಸು++) { /* void */ }
        ಸ್ಥಾನ = ಸರಿಸು - pool->min_ಸರಿಸು;

    } ಇಲ್ಲದಿದ್ದರೆ {
        ಸರಿಸು = pool->min_ಸರಿಸು;
        ಸ್ಥಾನ = 0;
    }

    pool->stats[ಸ್ಥಾನ].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                   "slab alloc: %uz slot: %ui", ಗಾತ್ರ_2, ಸ್ಥಾನ);

    slots = ngx_slab_slots(pool);
    ಪುಟ = slots[ಸ್ಥಾನ].ಮುಂದಿನ;

    ಆದರೆ (ಪುಟ->ಮುಂದಿನ != ಪುಟ) {

        ಆದರೆ (ಸರಿಸು < ngx_slab_exact_ಸರಿಸು) {

            bitmap = (uintptr_t *) ngx_slab_ಪುಟ_addr(pool, ಪುಟ);

            map = (ngx_pagesize >> ಸರಿಸು) / (8 * ಗಾತ್ರ(uintptr_t));

            ಗಾಗಿ (n = 0; n < map; n++) {

                ಆದರೆ (bitmap[n] != NGX_SLAB_BUSY) {

                    ಗಾಗಿ (m = 1, i = 0; m; m <<= 1, i++) {
                        ಆದರೆ (bitmap[n] & m) {
                            ಮುಂದುವರಿ;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * ಗಾತ್ರ(uintptr_t) + i) << ಸರಿಸು;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[ಸ್ಥಾನ].ಬಳಸಿದ++;

                        ಆದರೆ (bitmap[n] == NGX_SLAB_BUSY) {
                            ಗಾಗಿ (n = n + 1; n < map; n++) {
                                ಆದರೆ (bitmap[n] != NGX_SLAB_BUSY) {
                                    ಹೋಗು ಮುಗಿದ;
                                }
                            }

                            prev = ngx_slab_ಪುಟ_prev(ಪುಟ);
                            prev->ಮುಂದಿನ = ಪುಟ->ಮುಂದಿನ;
                            ಪುಟ->ಮುಂದಿನ->prev = ಪುಟ->prev;

                            ಪುಟ->ಮುಂದಿನ = ಖಾಲಿ;
                            ಪುಟ->prev = NGX_SLAB_SMALL;
                        }

                        ಹೋಗು ಮುಗಿದ;
                    }
                }
            }

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸರಿಸು == ngx_slab_exact_ಸರಿಸು) {

            ಗಾಗಿ (m = 1, i = 0; m; m <<= 1, i++) {
                ಆದರೆ (ಪುಟ->slab & m) {
                    ಮುಂದುವರಿ;
                }

                ಪುಟ->slab |= m;

                ಆದರೆ (ಪುಟ->slab == NGX_SLAB_BUSY) {
                    prev = ngx_slab_ಪುಟ_prev(ಪುಟ);
                    prev->ಮುಂದಿನ = ಪುಟ->ಮುಂದಿನ;
                    ಪುಟ->ಮುಂದಿನ->prev = ಪುಟ->prev;

                    ಪುಟ->ಮುಂದಿನ = ಖಾಲಿ;
                    ಪುಟ->prev = NGX_SLAB_EXACT;
                }

                p = ngx_slab_ಪುಟ_addr(pool, ಪುಟ) + (i << ಸರಿಸು);

                pool->stats[ಸ್ಥಾನ].ಬಳಸಿದ++;

                ಹೋಗು ಮುಗಿದ;
            }

        } ಇಲ್ಲದಿದ್ದರೆ { /* shift > ngx_slab_exact_shift */

            ಮುಖವಾಡ = ((uintptr_t) 1 << (ngx_pagesize >> ಸರಿಸು)) - 1;
            ಮುಖವಾಡ <<= NGX_SLAB_MAP_ಸರಿಸು;

            ಗಾಗಿ (m = (uintptr_t) 1 << NGX_SLAB_MAP_ಸರಿಸು, i = 0;
                 m & ಮುಖವಾಡ;
                 m <<= 1, i++)
            {
                ಆದರೆ (ಪುಟ->slab & m) {
                    ಮುಂದುವರಿ;
                }

                ಪುಟ->slab |= m;

                ಆದರೆ ((ಪುಟ->slab & NGX_SLAB_MAP_ಮುಖವಾಡ) == ಮುಖವಾಡ) {
                    prev = ngx_slab_ಪುಟ_prev(ಪುಟ);
                    prev->ಮುಂದಿನ = ಪುಟ->ಮುಂದಿನ;
                    ಪುಟ->ಮುಂದಿನ->prev = ಪುಟ->prev;

                    ಪುಟ->ಮುಂದಿನ = ಖಾಲಿ;
                    ಪುಟ->prev = NGX_SLAB_BIG;
                }

                p = ngx_slab_ಪುಟ_addr(pool, ಪುಟ) + (i << ಸರಿಸು);

                pool->stats[ಸ್ಥಾನ].ಬಳಸಿದ++;

                ಹೋಗು ಮುಗಿದ;
            }
        }

        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_alloc(): page is busy");
        ngx_debug_ಬಿಂದು();
    }

    ಪುಟ = ngx_slab_alloc_ಪುಟಗಳು(pool, 1);

    ಆದರೆ (ಪುಟ) {
        ಆದರೆ (ಸರಿಸು < ngx_slab_exact_ಸರಿಸು) {
            bitmap = (uintptr_t *) ngx_slab_ಪುಟ_addr(pool, ಪುಟ);

            n = (ngx_pagesize >> ಸರಿಸು) / ((1 << ಸರಿಸು) * 8);

            ಆದರೆ (n == 0) {
                n = 1;
            }

            /* "n" elements for bitmap, plus one requested */

            ಗಾಗಿ (i = 0; i < (n + 1) / (8 * ಗಾತ್ರ(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * ಗಾತ್ರ(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> ಸರಿಸು) / (8 * ಗಾತ್ರ(uintptr_t));

            ಗಾಗಿ (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            ಪುಟ->slab = ಸರಿಸು;
            ಪುಟ->ಮುಂದಿನ = &slots[ಸ್ಥಾನ];
            ಪುಟ->prev = (uintptr_t) &slots[ಸ್ಥಾನ] | NGX_SLAB_SMALL;

            slots[ಸ್ಥಾನ].ಮುಂದಿನ = ಪುಟ;

            pool->stats[ಸ್ಥಾನ].ಒಟ್ಟು += (ngx_pagesize >> ಸರಿಸು) - n;

            p = ngx_slab_ಪುಟ_addr(pool, ಪುಟ) + (n << ಸರಿಸು);

            pool->stats[ಸ್ಥಾನ].ಬಳಸಿದ++;

            ಹೋಗು ಮುಗಿದ;

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸರಿಸು == ngx_slab_exact_ಸರಿಸು) {

            ಪುಟ->slab = 1;
            ಪುಟ->ಮುಂದಿನ = &slots[ಸ್ಥಾನ];
            ಪುಟ->prev = (uintptr_t) &slots[ಸ್ಥಾನ] | NGX_SLAB_EXACT;

            slots[ಸ್ಥಾನ].ಮುಂದಿನ = ಪುಟ;

            pool->stats[ಸ್ಥಾನ].ಒಟ್ಟು += 8 * ಗಾತ್ರ(uintptr_t);

            p = ngx_slab_ಪುಟ_addr(pool, ಪುಟ);

            pool->stats[ಸ್ಥಾನ].ಬಳಸಿದ++;

            ಹೋಗು ಮುಗಿದ;

        } ಇಲ್ಲದಿದ್ದರೆ { /* shift > ngx_slab_exact_shift */

            ಪುಟ->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_ಸರಿಸು) | ಸರಿಸು;
            ಪುಟ->ಮುಂದಿನ = &slots[ಸ್ಥಾನ];
            ಪುಟ->prev = (uintptr_t) &slots[ಸ್ಥಾನ] | NGX_SLAB_BIG;

            slots[ಸ್ಥಾನ].ಮುಂದಿನ = ಪುಟ;

            pool->stats[ಸ್ಥಾನ].ಒಟ್ಟು += ngx_pagesize >> ಸರಿಸು;

            p = ngx_slab_ಪುಟ_addr(pool, ಪುಟ);

            pool->stats[ಸ್ಥಾನ].ಬಳಸಿದ++;

            ಹೋಗು ಮುಗಿದ;
        }
    }

    p = 0;

    pool->stats[ಸ್ಥಾನ].fails++;

ಮುಗಿದ:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                   "slab alloc: %p", (ಶೂನ್ಯ *) p);

    ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ *) p;
}


ಶೂನ್ಯ *
ngx_slab_calloc(ngx_slab_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಶೂನ್ಯ  *p;

    ngx_shmtx_ಬೀಗ(&pool->mutex);

    p = ngx_slab_calloc_locked(pool, ಗಾತ್ರ_2);

    ngx_shmtx_unlock(&pool->mutex);

    ಹಿಂತಿರುಗಿ p;
}


ಶೂನ್ಯ *
ngx_slab_calloc_locked(ngx_slab_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಶೂನ್ಯ  *p;

    p = ngx_slab_alloc_locked(pool, ಗಾತ್ರ_2);
    ಆದರೆ (p) {
        ngx_memzero(p, ಗಾತ್ರ_2);
    }

    ಹಿಂತಿರುಗಿ p;
}


ಶೂನ್ಯ
ngx_slab_free(ngx_slab_pool_t *pool, ಶೂನ್ಯ *p)
{
    ngx_shmtx_ಬೀಗ(&pool->mutex);

    ngx_slab_free_locked(pool, p);

    ngx_shmtx_unlock(&pool->mutex);
}


ಶೂನ್ಯ
ngx_slab_free_locked(ngx_slab_pool_t *pool, ಶೂನ್ಯ *p)
{
    ಗಾತ್ರ_t            ಗಾತ್ರ_2;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, ಸ್ಥಾನ, ಸರಿಸು, map;
    ngx_slab_ಪುಟ_t  *slots, *ಪುಟ;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0, "slab free: %p", p);

    ಆದರೆ ((u_char *) p < pool->ಆರಂಭ || (u_char *) p > pool->ಕೊನೆ) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        ಹೋಗು fail;
    }

    n = ((u_char *) p - pool->ಆರಂಭ) >> ngx_pagesize_ಸರಿಸು;
    ಪುಟ = &pool->ಪುಟಗಳು[n];
    slab = ಪುಟ->slab;
    type = ngx_slab_ಪುಟ_type(ಪುಟ);

    ಆಯ್ಕೆ (type) {

    ಸ್ಥಿತಿ NGX_SLAB_SMALL:

        ಸರಿಸು = slab & NGX_SLAB_ಸರಿಸು_ಮುಖವಾಡ;
        ಗಾತ್ರ_2 = (ಗಾತ್ರ_t) 1 << ಸರಿಸು;

        ಆದರೆ ((uintptr_t) p & (ಗಾತ್ರ_2 - 1)) {
            ಹೋಗು wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> ಸರಿಸು;
        m = (uintptr_t) 1 << (n % (8 * ಗಾತ್ರ(uintptr_t)));
        n /= 8 * ಗಾತ್ರ(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        ಆದರೆ (bitmap[n] & m) {
            ಸ್ಥಾನ = ಸರಿಸು - pool->min_ಸರಿಸು;

            ಆದರೆ (ಪುಟ->ಮುಂದಿನ == ಖಾಲಿ) {
                slots = ngx_slab_slots(pool);

                ಪುಟ->ಮುಂದಿನ = slots[ಸ್ಥಾನ].ಮುಂದಿನ;
                slots[ಸ್ಥಾನ].ಮುಂದಿನ = ಪುಟ;

                ಪುಟ->prev = (uintptr_t) &slots[ಸ್ಥಾನ] | NGX_SLAB_SMALL;
                ಪುಟ->ಮುಂದಿನ->prev = (uintptr_t) ಪುಟ | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> ಸರಿಸು) / ((1 << ಸರಿಸು) * 8);

            ಆದರೆ (n == 0) {
                n = 1;
            }

            i = n / (8 * ಗಾತ್ರ(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * ಗಾತ್ರ(uintptr_t)))) - 1;

            ಆದರೆ (bitmap[i] & ~m) {
                ಹೋಗು ಮುಗಿದ;
            }

            map = (ngx_pagesize >> ಸರಿಸು) / (8 * ಗಾತ್ರ(uintptr_t));

            ಗಾಗಿ (i = i + 1; i < map; i++) {
                ಆದರೆ (bitmap[i]) {
                    ಹೋಗು ಮುಗಿದ;
                }
            }

            ngx_slab_free_ಪುಟಗಳು(pool, ಪುಟ, 1);

            pool->stats[ಸ್ಥಾನ].ಒಟ್ಟು -= (ngx_pagesize >> ಸರಿಸು) - n;

            ಹೋಗು ಮುಗಿದ;
        }

        ಹೋಗು chunk_already_free;

    ಸ್ಥಿತಿ NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_ಸರಿಸು);
        ಗಾತ್ರ_2 = ngx_slab_exact_ಗಾತ್ರ;

        ಆದರೆ ((uintptr_t) p & (ಗಾತ್ರ_2 - 1)) {
            ಹೋಗು wrong_chunk;
        }

        ಆದರೆ (slab & m) {
            ಸ್ಥಾನ = ngx_slab_exact_ಸರಿಸು - pool->min_ಸರಿಸು;

            ಆದರೆ (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                ಪುಟ->ಮುಂದಿನ = slots[ಸ್ಥಾನ].ಮುಂದಿನ;
                slots[ಸ್ಥಾನ].ಮುಂದಿನ = ಪುಟ;

                ಪುಟ->prev = (uintptr_t) &slots[ಸ್ಥಾನ] | NGX_SLAB_EXACT;
                ಪುಟ->ಮುಂದಿನ->prev = (uintptr_t) ಪುಟ | NGX_SLAB_EXACT;
            }

            ಪುಟ->slab &= ~m;

            ಆದರೆ (ಪುಟ->slab) {
                ಹೋಗು ಮುಗಿದ;
            }

            ngx_slab_free_ಪುಟಗಳು(pool, ಪುಟ, 1);

            pool->stats[ಸ್ಥಾನ].ಒಟ್ಟು -= 8 * ಗಾತ್ರ(uintptr_t);

            ಹೋಗು ಮುಗಿದ;
        }

        ಹೋಗು chunk_already_free;

    ಸ್ಥಿತಿ NGX_SLAB_BIG:

        ಸರಿಸು = slab & NGX_SLAB_ಸರಿಸು_ಮುಖವಾಡ;
        ಗಾತ್ರ_2 = (ಗಾತ್ರ_t) 1 << ಸರಿಸು;

        ಆದರೆ ((uintptr_t) p & (ಗಾತ್ರ_2 - 1)) {
            ಹೋಗು wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> ಸರಿಸು)
                              + NGX_SLAB_MAP_ಸರಿಸು);

        ಆದರೆ (slab & m) {
            ಸ್ಥಾನ = ಸರಿಸು - pool->min_ಸರಿಸು;

            ಆದರೆ (ಪುಟ->ಮುಂದಿನ == ಖಾಲಿ) {
                slots = ngx_slab_slots(pool);

                ಪುಟ->ಮುಂದಿನ = slots[ಸ್ಥಾನ].ಮುಂದಿನ;
                slots[ಸ್ಥಾನ].ಮುಂದಿನ = ಪುಟ;

                ಪುಟ->prev = (uintptr_t) &slots[ಸ್ಥಾನ] | NGX_SLAB_BIG;
                ಪುಟ->ಮುಂದಿನ->prev = (uintptr_t) ಪುಟ | NGX_SLAB_BIG;
            }

            ಪುಟ->slab &= ~m;

            ಆದರೆ (ಪುಟ->slab & NGX_SLAB_MAP_ಮುಖವಾಡ) {
                ಹೋಗು ಮುಗಿದ;
            }

            ngx_slab_free_ಪುಟಗಳು(pool, ಪುಟ, 1);

            pool->stats[ಸ್ಥಾನ].ಒಟ್ಟು -= ngx_pagesize >> ಸರಿಸು;

            ಹೋಗು ಮುಗಿದ;
        }

        ಹೋಗು chunk_already_free;

    ಸ್ಥಿತಿ NGX_SLAB_ಪುಟ:

        ಆದರೆ ((uintptr_t) p & (ngx_pagesize - 1)) {
            ಹೋಗು wrong_chunk;
        }

        ಆದರೆ (!(slab & NGX_SLAB_ಪುಟ_ಆರಂಭ)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            ಹೋಗು fail;
        }

        ಆದರೆ (slab == NGX_SLAB_ಪುಟ_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            ಹೋಗು fail;
        }

        ಗಾತ್ರ_2 = slab & ~NGX_SLAB_ಪುಟ_ಆರಂಭ;

        ngx_slab_free_ಪುಟಗಳು(pool, ಪುಟ, ಗಾತ್ರ_2);

        ngx_slab_junk(p, ಗಾತ್ರ_2 << ngx_pagesize_ಸರಿಸು);

        ಹಿಂತಿರುಗಿ;
    }

    /* not reached */

    ಹಿಂತಿರುಗಿ;

ಮುಗಿದ:

    pool->stats[ಸ್ಥಾನ].ಬಳಸಿದ--;

    ngx_slab_junk(p, ಗಾತ್ರ_2);

    ಹಿಂತಿರುಗಿ;

wrong_chunk:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): pointer to wrong chunk");

    ಹೋಗು fail;

chunk_already_free:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): chunk is already free");

fail:

    ಹಿಂತಿರುಗಿ;
}


ಸ್ಥಿರ ngx_slab_ಪುಟ_t *
ngx_slab_alloc_ಪುಟಗಳು(ngx_slab_pool_t *pool, ngx_uint_t ಪುಟಗಳು)
{
    ngx_slab_ಪುಟ_t  *ಪುಟ, *p;

    ಗಾಗಿ (ಪುಟ = pool->ಬಿಡುಗಡೆ.ಮುಂದಿನ; ಪುಟ != &pool->ಬಿಡುಗಡೆ; ಪುಟ = ಪುಟ->ಮುಂದಿನ) {

        ಆದರೆ (ಪುಟ->slab >= ಪುಟಗಳು) {

            ಆದರೆ (ಪುಟ->slab > ಪುಟಗಳು) {
                ಪುಟ[ಪುಟ->slab - 1].prev = (uintptr_t) &ಪುಟ[ಪುಟಗಳು];

                ಪುಟ[ಪುಟಗಳು].slab = ಪುಟ->slab - ಪುಟಗಳು;
                ಪುಟ[ಪುಟಗಳು].ಮುಂದಿನ = ಪುಟ->ಮುಂದಿನ;
                ಪುಟ[ಪುಟಗಳು].prev = ಪುಟ->prev;

                p = (ngx_slab_ಪುಟ_t *) ಪುಟ->prev;
                p->ಮುಂದಿನ = &ಪುಟ[ಪುಟಗಳು];
                ಪುಟ->ಮುಂದಿನ->prev = (uintptr_t) &ಪುಟ[ಪುಟಗಳು];

            } ಇಲ್ಲದಿದ್ದರೆ {
                p = (ngx_slab_ಪುಟ_t *) ಪುಟ->prev;
                p->ಮುಂದಿನ = ಪುಟ->ಮುಂದಿನ;
                ಪುಟ->ಮುಂದಿನ->prev = ಪುಟ->prev;
            }

            ಪುಟ->slab = ಪುಟಗಳು | NGX_SLAB_ಪುಟ_ಆರಂಭ;
            ಪುಟ->ಮುಂದಿನ = ಖಾಲಿ;
            ಪುಟ->prev = NGX_SLAB_ಪುಟ;

            pool->pfree -= ಪುಟಗಳು;

            ಆದರೆ (--ಪುಟಗಳು == 0) {
                ಹಿಂತಿರುಗಿ ಪುಟ;
            }

            ಗಾಗಿ (p = ಪುಟ + 1; ಪುಟಗಳು; ಪುಟಗಳು--) {
                p->slab = NGX_SLAB_ಪುಟ_BUSY;
                p->ಮುಂದಿನ = ಖಾಲಿ;
                p->prev = NGX_SLAB_ಪುಟ;
                p++;
            }

            ಹಿಂತಿರುಗಿ ಪುಟ;
        }
    }

    ಆದರೆ (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_slab_free_ಪುಟಗಳು(ngx_slab_pool_t *pool, ngx_slab_ಪುಟ_t *ಪುಟ,
    ngx_uint_t ಪುಟಗಳು)
{
    ngx_slab_ಪುಟ_t  *prev, *join;

    pool->pfree += ಪುಟಗಳು;

    ಪುಟ->slab = ಪುಟಗಳು--;

    ಆದರೆ (ಪುಟಗಳು) {
        ngx_memzero(&ಪುಟ[1], ಪುಟಗಳು * ಗಾತ್ರ(ngx_slab_ಪುಟ_t));
    }

    ಆದರೆ (ಪುಟ->ಮುಂದಿನ) {
        prev = ngx_slab_ಪುಟ_prev(ಪುಟ);
        prev->ಮುಂದಿನ = ಪುಟ->ಮುಂದಿನ;
        ಪುಟ->ಮುಂದಿನ->prev = ಪುಟ->prev;
    }

    join = ಪುಟ + ಪುಟ->slab;

    ಆದರೆ (join < pool->ಕೊನೆಯ) {

        ಆದರೆ (ngx_slab_ಪುಟ_type(join) == NGX_SLAB_ಪುಟ) {

            ಆದರೆ (join->ಮುಂದಿನ != ಖಾಲಿ) {
                ಪುಟಗಳು += join->slab;
                ಪುಟ->slab += join->slab;

                prev = ngx_slab_ಪುಟ_prev(join);
                prev->ಮುಂದಿನ = join->ಮುಂದಿನ;
                join->ಮುಂದಿನ->prev = join->prev;

                join->slab = NGX_SLAB_ಪುಟ_FREE;
                join->ಮುಂದಿನ = ಖಾಲಿ;
                join->prev = NGX_SLAB_ಪುಟ;
            }
        }
    }

    ಆದರೆ (ಪುಟ > pool->ಪುಟಗಳು) {
        join = ಪುಟ - 1;

        ಆದರೆ (ngx_slab_ಪುಟ_type(join) == NGX_SLAB_ಪುಟ) {

            ಆದರೆ (join->slab == NGX_SLAB_ಪುಟ_FREE) {
                join = ngx_slab_ಪುಟ_prev(join);
            }

            ಆದರೆ (join->ಮುಂದಿನ != ಖಾಲಿ) {
                ಪುಟಗಳು += join->slab;
                join->slab += ಪುಟ->slab;

                prev = ngx_slab_ಪುಟ_prev(join);
                prev->ಮುಂದಿನ = join->ಮುಂದಿನ;
                join->ಮುಂದಿನ->prev = join->prev;

                ಪುಟ->slab = NGX_SLAB_ಪುಟ_FREE;
                ಪುಟ->ಮುಂದಿನ = ಖಾಲಿ;
                ಪುಟ->prev = NGX_SLAB_ಪುಟ;

                ಪುಟ = join;
            }
        }
    }

    ಆದರೆ (ಪುಟಗಳು) {
        ಪುಟ[ಪುಟಗಳು].prev = (uintptr_t) ಪುಟ;
    }

    ಪುಟ->prev = (uintptr_t) &pool->ಬಿಡುಗಡೆ;
    ಪುಟ->ಮುಂದಿನ = pool->ಬಿಡುಗಡೆ.ಮುಂದಿನ;

    ಪುಟ->ಮುಂದಿನ->prev = (uintptr_t) ಪುಟ;

    pool->ಬಿಡುಗಡೆ.ಮುಂದಿನ = ಪುಟ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, ಅಕ್ಷರ *ಪಠ್ಯ)
{
    ngx_log_error(level, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0, "%s%s", ಪಠ್ಯ, pool->log_ctx);
}
