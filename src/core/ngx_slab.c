
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


#నిర్వచించు NGX_SLAB_పుట_ముసుగు   3
#నిర్వచించు NGX_SLAB_పుట        0
#నిర్వచించు NGX_SLAB_BIG         1
#నిర్వచించు NGX_SLAB_EXACT       2
#నిర్వచించు NGX_SLAB_SMALL       3

#ఒకవేళ (NGX_PTR_కొలత == 4)

#నిర్వచించు NGX_SLAB_పుట_FREE   0
#నిర్వచించు NGX_SLAB_పుట_BUSY   0xffffffff
#నిర్వచించు NGX_SLAB_పుట_ప్రారంభం  0x80000000

#నిర్వచించు NGX_SLAB_జరుపు_ముసుగు  0x0000000f
#నిర్వచించు NGX_SLAB_MAP_ముసుగు    0xffff0000
#నిర్వచించు NGX_SLAB_MAP_జరుపు   16

#నిర్వచించు NGX_SLAB_BUSY        0xffffffff

#లేకపోతే /* (NGX_PTR_SIZE == 8) */

#నిర్వచించు NGX_SLAB_పుట_FREE   0
#నిర్వచించు NGX_SLAB_పుట_BUSY   0xffffffffffffffff
#నిర్వచించు NGX_SLAB_పుట_ప్రారంభం  0x8000000000000000

#నిర్వచించు NGX_SLAB_జరుపు_ముసుగు  0x000000000000000f
#నిర్వచించు NGX_SLAB_MAP_ముసుగు    0xffffffff00000000
#నిర్వచించు NGX_SLAB_MAP_జరుపు   32

#నిర్వచించు NGX_SLAB_BUSY        0xffffffffffffffff

#అయితే_ముగింపు


#నిర్వచించు ngx_slab_slots(pool)                                                  \
    (ngx_slab_పుట_t *) ((u_char *) (pool) + పరిమాణం(ngx_slab_pool_t))

#నిర్వచించు ngx_slab_పుట_type(పుట)   ((పుట)->prev & NGX_SLAB_పుట_ముసుగు)

#నిర్వచించు ngx_slab_పుట_prev(పుట)                                              \
    (ngx_slab_పుట_t *) ((పుట)->prev & ~NGX_SLAB_పుట_ముసుగు)

#నిర్వచించు ngx_slab_పుట_addr(pool, పుట)                                        \
    ((((పుట) - (pool)->పుటలు) << ngx_pagesize_జరుపు)                         \
     + (uintptr_t) (pool)->ప్రారంభం)


#ఒకవేళ (NGX_DEBUG_MALLOC)

#నిర్వచించు ngx_slab_junk(p, కొలత)     ngx_memset(p, 0xA5, కొలత)

#లేదావేళ (NGX_HAVE_DEBUG_MALLOC)

#నిర్వచించు ngx_slab_junk(p, కొలత)                                                \
    ఒకవేళ (ngx_debug_malloc)          ngx_memset(p, 0xA5, కొలత)

#లేకపోతే

#నిర్వచించు ngx_slab_junk(p, కొలత)

#అయితే_ముగింపు

స్థిర ngx_slab_పుట_t *ngx_slab_alloc_పుటలు(ngx_slab_pool_t *pool,
    ngx_uint_t పుటలు);
స్థిర శూన్య ngx_slab_free_పుటలు(ngx_slab_pool_t *pool, ngx_slab_పుట_t *పుట,
    ngx_uint_t పుటలు);
స్థిర శూన్య ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    అక్షరం *పాఠ్యం);


స్థిర ngx_uint_t  ngx_slab_max_కొలత;
స్థిర ngx_uint_t  ngx_slab_exact_కొలత;
స్థిర ngx_uint_t  ngx_slab_exact_జరుపు;


శూన్య
ngx_slab_sizes_init(శూన్య)
{
    ngx_uint_t  n;

    ngx_slab_max_కొలత = ngx_pagesize / 2;
    ngx_slab_exact_కొలత = ngx_pagesize / (8 * పరిమాణం(uintptr_t));
    కొరకు (n = ngx_slab_exact_కొలత; n >>= 1; ngx_slab_exact_జరుపు++) {
        /* void */
    }
}


శూన్య
ngx_slab_init(ngx_slab_pool_t *pool)
{
    u_char           *p;
    కొలత_t            కొలత;
    ngx_int_t         m;
    ngx_uint_t        i, n, పుటలు;
    ngx_slab_పుట_t  *slots, *పుట;

    pool->min_కొలత = (కొలత_t) 1 << pool->min_జరుపు;

    slots = ngx_slab_slots(pool);

    p = (u_char *) slots;
    కొలత = pool->చివర - p;

    ngx_slab_junk(p, కొలత);

    n = ngx_pagesize_జరుపు - pool->min_జరుపు;

    కొరకు (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].తదుపరి = &slots[i];
        slots[i].prev = 0;
    }

    p += n * పరిమాణం(ngx_slab_పుట_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * పరిమాణం(ngx_slab_stat_t));

    p += n * పరిమాణం(ngx_slab_stat_t);

    కొలత -= n * (పరిమాణం(ngx_slab_పుట_t) + పరిమాణం(ngx_slab_stat_t));

    పుటలు = (ngx_uint_t) (కొలత / (ngx_pagesize + పరిమాణం(ngx_slab_పుట_t)));

    pool->పుటలు = (ngx_slab_పుట_t *) p;
    ngx_memzero(pool->పుటలు, పుటలు * పరిమాణం(ngx_slab_పుట_t));

    పుట = pool->పుటలు;

    /* only "next" is used in list head */
    pool->విడుదల.slab = 0;
    pool->విడుదల.తదుపరి = పుట;
    pool->విడుదల.prev = 0;

    పుట->slab = పుటలు;
    పుట->తదుపరి = &pool->విడుదల;
    పుట->prev = (uintptr_t) &pool->విడుదల;

    pool->ప్రారంభం = ngx_align_ptr(p + పుటలు * పరిమాణం(ngx_slab_పుట_t),
                                ngx_pagesize);

    m = పుటలు - (pool->చివర - pool->ప్రారంభం) / ngx_pagesize;
    ఒకవేళ (m > 0) {
        పుటలు -= m;
        పుట->slab = పుటలు;
    }

    pool->చివరి = pool->పుటలు + పుటలు;
    pool->pfree = పుటలు;

    pool->log_nomem = 1;
    pool->log_ctx = &pool->zero;
    pool->zero = '\0';
}


శూన్య *
ngx_slab_alloc(ngx_slab_pool_t *pool, కొలత_t కొలత)
{
    శూన్య  *p;

    ngx_shmtx_తాళం(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, కొలత);

    ngx_shmtx_unlock(&pool->mutex);

    తిరిగి p;
}


శూన్య *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, కొలత_t కొలత)
{
    కొలత_t            s;
    uintptr_t         p, m, ముసుగు, *bitmap;
    ngx_uint_t        i, n, స్థానం, జరుపు, map;
    ngx_slab_పుట_t  *పుట, *prev, *slots;

    ఒకవేళ (కొలత > ngx_slab_max_కొలత) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_చక్రం->సంవర్గమానం, 0,
                       "slab alloc: %uz", కొలత);

        పుట = ngx_slab_alloc_పుటలు(pool, (కొలత >> ngx_pagesize_జరుపు)
                                          + ((కొలత % ngx_pagesize) ? 1 : 0));
        ఒకవేళ (పుట) {
            p = ngx_slab_పుట_addr(pool, పుట);

        } లేకపోతే {
            p = 0;
        }

        వెళ్ళు పూర్తి;
    }

    ఒకవేళ (కొలత > pool->min_కొలత) {
        జరుపు = 1;
        కొరకు (s = కొలత - 1; s >>= 1; జరుపు++) { /* void */ }
        స్థానం = జరుపు - pool->min_జరుపు;

    } లేకపోతే {
        జరుపు = pool->min_జరుపు;
        స్థానం = 0;
    }

    pool->stats[స్థానం].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_చక్రం->సంవర్గమానం, 0,
                   "slab alloc: %uz slot: %ui", కొలత, స్థానం);

    slots = ngx_slab_slots(pool);
    పుట = slots[స్థానం].తదుపరి;

    ఒకవేళ (పుట->తదుపరి != పుట) {

        ఒకవేళ (జరుపు < ngx_slab_exact_జరుపు) {

            bitmap = (uintptr_t *) ngx_slab_పుట_addr(pool, పుట);

            map = (ngx_pagesize >> జరుపు) / (8 * పరిమాణం(uintptr_t));

            కొరకు (n = 0; n < map; n++) {

                ఒకవేళ (bitmap[n] != NGX_SLAB_BUSY) {

                    కొరకు (m = 1, i = 0; m; m <<= 1, i++) {
                        ఒకవేళ (bitmap[n] & m) {
                            కొనసాగు;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * పరిమాణం(uintptr_t) + i) << జరుపు;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[స్థానం].వాడిన++;

                        ఒకవేళ (bitmap[n] == NGX_SLAB_BUSY) {
                            కొరకు (n = n + 1; n < map; n++) {
                                ఒకవేళ (bitmap[n] != NGX_SLAB_BUSY) {
                                    వెళ్ళు పూర్తి;
                                }
                            }

                            prev = ngx_slab_పుట_prev(పుట);
                            prev->తదుపరి = పుట->తదుపరి;
                            పుట->తదుపరి->prev = పుట->prev;

                            పుట->తదుపరి = ఖాళీ;
                            పుట->prev = NGX_SLAB_SMALL;
                        }

                        వెళ్ళు పూర్తి;
                    }
                }
            }

        } లేకపోతే ఒకవేళ (జరుపు == ngx_slab_exact_జరుపు) {

            కొరకు (m = 1, i = 0; m; m <<= 1, i++) {
                ఒకవేళ (పుట->slab & m) {
                    కొనసాగు;
                }

                పుట->slab |= m;

                ఒకవేళ (పుట->slab == NGX_SLAB_BUSY) {
                    prev = ngx_slab_పుట_prev(పుట);
                    prev->తదుపరి = పుట->తదుపరి;
                    పుట->తదుపరి->prev = పుట->prev;

                    పుట->తదుపరి = ఖాళీ;
                    పుట->prev = NGX_SLAB_EXACT;
                }

                p = ngx_slab_పుట_addr(pool, పుట) + (i << జరుపు);

                pool->stats[స్థానం].వాడిన++;

                వెళ్ళు పూర్తి;
            }

        } లేకపోతే { /* shift > ngx_slab_exact_shift */

            ముసుగు = ((uintptr_t) 1 << (ngx_pagesize >> జరుపు)) - 1;
            ముసుగు <<= NGX_SLAB_MAP_జరుపు;

            కొరకు (m = (uintptr_t) 1 << NGX_SLAB_MAP_జరుపు, i = 0;
                 m & ముసుగు;
                 m <<= 1, i++)
            {
                ఒకవేళ (పుట->slab & m) {
                    కొనసాగు;
                }

                పుట->slab |= m;

                ఒకవేళ ((పుట->slab & NGX_SLAB_MAP_ముసుగు) == ముసుగు) {
                    prev = ngx_slab_పుట_prev(పుట);
                    prev->తదుపరి = పుట->తదుపరి;
                    పుట->తదుపరి->prev = పుట->prev;

                    పుట->తదుపరి = ఖాళీ;
                    పుట->prev = NGX_SLAB_BIG;
                }

                p = ngx_slab_పుట_addr(pool, పుట) + (i << జరుపు);

                pool->stats[స్థానం].వాడిన++;

                వెళ్ళు పూర్తి;
            }
        }

        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_alloc(): page is busy");
        ngx_debug_బిందువు();
    }

    పుట = ngx_slab_alloc_పుటలు(pool, 1);

    ఒకవేళ (పుట) {
        ఒకవేళ (జరుపు < ngx_slab_exact_జరుపు) {
            bitmap = (uintptr_t *) ngx_slab_పుట_addr(pool, పుట);

            n = (ngx_pagesize >> జరుపు) / ((1 << జరుపు) * 8);

            ఒకవేళ (n == 0) {
                n = 1;
            }

            /* "n" elements for bitmap, plus one requested */

            కొరకు (i = 0; i < (n + 1) / (8 * పరిమాణం(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * పరిమాణం(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> జరుపు) / (8 * పరిమాణం(uintptr_t));

            కొరకు (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            పుట->slab = జరుపు;
            పుట->తదుపరి = &slots[స్థానం];
            పుట->prev = (uintptr_t) &slots[స్థానం] | NGX_SLAB_SMALL;

            slots[స్థానం].తదుపరి = పుట;

            pool->stats[స్థానం].మొత్తం += (ngx_pagesize >> జరుపు) - n;

            p = ngx_slab_పుట_addr(pool, పుట) + (n << జరుపు);

            pool->stats[స్థానం].వాడిన++;

            వెళ్ళు పూర్తి;

        } లేకపోతే ఒకవేళ (జరుపు == ngx_slab_exact_జరుపు) {

            పుట->slab = 1;
            పుట->తదుపరి = &slots[స్థానం];
            పుట->prev = (uintptr_t) &slots[స్థానం] | NGX_SLAB_EXACT;

            slots[స్థానం].తదుపరి = పుట;

            pool->stats[స్థానం].మొత్తం += 8 * పరిమాణం(uintptr_t);

            p = ngx_slab_పుట_addr(pool, పుట);

            pool->stats[స్థానం].వాడిన++;

            వెళ్ళు పూర్తి;

        } లేకపోతే { /* shift > ngx_slab_exact_shift */

            పుట->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_జరుపు) | జరుపు;
            పుట->తదుపరి = &slots[స్థానం];
            పుట->prev = (uintptr_t) &slots[స్థానం] | NGX_SLAB_BIG;

            slots[స్థానం].తదుపరి = పుట;

            pool->stats[స్థానం].మొత్తం += ngx_pagesize >> జరుపు;

            p = ngx_slab_పుట_addr(pool, పుట);

            pool->stats[స్థానం].వాడిన++;

            వెళ్ళు పూర్తి;
        }
    }

    p = 0;

    pool->stats[స్థానం].fails++;

పూర్తి:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_చక్రం->సంవర్గమానం, 0,
                   "slab alloc: %p", (శూన్య *) p);

    తిరిగి (శూన్య *) p;
}


శూన్య *
ngx_slab_calloc(ngx_slab_pool_t *pool, కొలత_t కొలత)
{
    శూన్య  *p;

    ngx_shmtx_తాళం(&pool->mutex);

    p = ngx_slab_calloc_locked(pool, కొలత);

    ngx_shmtx_unlock(&pool->mutex);

    తిరిగి p;
}


శూన్య *
ngx_slab_calloc_locked(ngx_slab_pool_t *pool, కొలత_t కొలత)
{
    శూన్య  *p;

    p = ngx_slab_alloc_locked(pool, కొలత);
    ఒకవేళ (p) {
        ngx_memzero(p, కొలత);
    }

    తిరిగి p;
}


శూన్య
ngx_slab_free(ngx_slab_pool_t *pool, శూన్య *p)
{
    ngx_shmtx_తాళం(&pool->mutex);

    ngx_slab_free_locked(pool, p);

    ngx_shmtx_unlock(&pool->mutex);
}


శూన్య
ngx_slab_free_locked(ngx_slab_pool_t *pool, శూన్య *p)
{
    కొలత_t            కొలత;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, స్థానం, జరుపు, map;
    ngx_slab_పుట_t  *slots, *పుట;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_చక్రం->సంవర్గమానం, 0, "slab free: %p", p);

    ఒకవేళ ((u_char *) p < pool->ప్రారంభం || (u_char *) p > pool->చివర) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        వెళ్ళు fail;
    }

    n = ((u_char *) p - pool->ప్రారంభం) >> ngx_pagesize_జరుపు;
    పుట = &pool->పుటలు[n];
    slab = పుట->slab;
    type = ngx_slab_పుట_type(పుట);

    ఎంపిక (type) {

    స్థితి NGX_SLAB_SMALL:

        జరుపు = slab & NGX_SLAB_జరుపు_ముసుగు;
        కొలత = (కొలత_t) 1 << జరుపు;

        ఒకవేళ ((uintptr_t) p & (కొలత - 1)) {
            వెళ్ళు wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> జరుపు;
        m = (uintptr_t) 1 << (n % (8 * పరిమాణం(uintptr_t)));
        n /= 8 * పరిమాణం(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        ఒకవేళ (bitmap[n] & m) {
            స్థానం = జరుపు - pool->min_జరుపు;

            ఒకవేళ (పుట->తదుపరి == ఖాళీ) {
                slots = ngx_slab_slots(pool);

                పుట->తదుపరి = slots[స్థానం].తదుపరి;
                slots[స్థానం].తదుపరి = పుట;

                పుట->prev = (uintptr_t) &slots[స్థానం] | NGX_SLAB_SMALL;
                పుట->తదుపరి->prev = (uintptr_t) పుట | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> జరుపు) / ((1 << జరుపు) * 8);

            ఒకవేళ (n == 0) {
                n = 1;
            }

            i = n / (8 * పరిమాణం(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * పరిమాణం(uintptr_t)))) - 1;

            ఒకవేళ (bitmap[i] & ~m) {
                వెళ్ళు పూర్తి;
            }

            map = (ngx_pagesize >> జరుపు) / (8 * పరిమాణం(uintptr_t));

            కొరకు (i = i + 1; i < map; i++) {
                ఒకవేళ (bitmap[i]) {
                    వెళ్ళు పూర్తి;
                }
            }

            ngx_slab_free_పుటలు(pool, పుట, 1);

            pool->stats[స్థానం].మొత్తం -= (ngx_pagesize >> జరుపు) - n;

            వెళ్ళు పూర్తి;
        }

        వెళ్ళు chunk_already_free;

    స్థితి NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_జరుపు);
        కొలత = ngx_slab_exact_కొలత;

        ఒకవేళ ((uintptr_t) p & (కొలత - 1)) {
            వెళ్ళు wrong_chunk;
        }

        ఒకవేళ (slab & m) {
            స్థానం = ngx_slab_exact_జరుపు - pool->min_జరుపు;

            ఒకవేళ (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                పుట->తదుపరి = slots[స్థానం].తదుపరి;
                slots[స్థానం].తదుపరి = పుట;

                పుట->prev = (uintptr_t) &slots[స్థానం] | NGX_SLAB_EXACT;
                పుట->తదుపరి->prev = (uintptr_t) పుట | NGX_SLAB_EXACT;
            }

            పుట->slab &= ~m;

            ఒకవేళ (పుట->slab) {
                వెళ్ళు పూర్తి;
            }

            ngx_slab_free_పుటలు(pool, పుట, 1);

            pool->stats[స్థానం].మొత్తం -= 8 * పరిమాణం(uintptr_t);

            వెళ్ళు పూర్తి;
        }

        వెళ్ళు chunk_already_free;

    స్థితి NGX_SLAB_BIG:

        జరుపు = slab & NGX_SLAB_జరుపు_ముసుగు;
        కొలత = (కొలత_t) 1 << జరుపు;

        ఒకవేళ ((uintptr_t) p & (కొలత - 1)) {
            వెళ్ళు wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> జరుపు)
                              + NGX_SLAB_MAP_జరుపు);

        ఒకవేళ (slab & m) {
            స్థానం = జరుపు - pool->min_జరుపు;

            ఒకవేళ (పుట->తదుపరి == ఖాళీ) {
                slots = ngx_slab_slots(pool);

                పుట->తదుపరి = slots[స్థానం].తదుపరి;
                slots[స్థానం].తదుపరి = పుట;

                పుట->prev = (uintptr_t) &slots[స్థానం] | NGX_SLAB_BIG;
                పుట->తదుపరి->prev = (uintptr_t) పుట | NGX_SLAB_BIG;
            }

            పుట->slab &= ~m;

            ఒకవేళ (పుట->slab & NGX_SLAB_MAP_ముసుగు) {
                వెళ్ళు పూర్తి;
            }

            ngx_slab_free_పుటలు(pool, పుట, 1);

            pool->stats[స్థానం].మొత్తం -= ngx_pagesize >> జరుపు;

            వెళ్ళు పూర్తి;
        }

        వెళ్ళు chunk_already_free;

    స్థితి NGX_SLAB_పుట:

        ఒకవేళ ((uintptr_t) p & (ngx_pagesize - 1)) {
            వెళ్ళు wrong_chunk;
        }

        ఒకవేళ (!(slab & NGX_SLAB_పుట_ప్రారంభం)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            వెళ్ళు fail;
        }

        ఒకవేళ (slab == NGX_SLAB_పుట_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            వెళ్ళు fail;
        }

        కొలత = slab & ~NGX_SLAB_పుట_ప్రారంభం;

        ngx_slab_free_పుటలు(pool, పుట, కొలత);

        ngx_slab_junk(p, కొలత << ngx_pagesize_జరుపు);

        తిరిగి;
    }

    /* not reached */

    తిరిగి;

పూర్తి:

    pool->stats[స్థానం].వాడిన--;

    ngx_slab_junk(p, కొలత);

    తిరిగి;

wrong_chunk:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): pointer to wrong chunk");

    వెళ్ళు fail;

chunk_already_free:

    ngx_slab_error(pool, NGX_LOG_ALERT,
                   "ngx_slab_free(): chunk is already free");

fail:

    తిరిగి;
}


స్థిర ngx_slab_పుట_t *
ngx_slab_alloc_పుటలు(ngx_slab_pool_t *pool, ngx_uint_t పుటలు)
{
    ngx_slab_పుట_t  *పుట, *p;

    కొరకు (పుట = pool->విడుదల.తదుపరి; పుట != &pool->విడుదల; పుట = పుట->తదుపరి) {

        ఒకవేళ (పుట->slab >= పుటలు) {

            ఒకవేళ (పుట->slab > పుటలు) {
                పుట[పుట->slab - 1].prev = (uintptr_t) &పుట[పుటలు];

                పుట[పుటలు].slab = పుట->slab - పుటలు;
                పుట[పుటలు].తదుపరి = పుట->తదుపరి;
                పుట[పుటలు].prev = పుట->prev;

                p = (ngx_slab_పుట_t *) పుట->prev;
                p->తదుపరి = &పుట[పుటలు];
                పుట->తదుపరి->prev = (uintptr_t) &పుట[పుటలు];

            } లేకపోతే {
                p = (ngx_slab_పుట_t *) పుట->prev;
                p->తదుపరి = పుట->తదుపరి;
                పుట->తదుపరి->prev = పుట->prev;
            }

            పుట->slab = పుటలు | NGX_SLAB_పుట_ప్రారంభం;
            పుట->తదుపరి = ఖాళీ;
            పుట->prev = NGX_SLAB_పుట;

            pool->pfree -= పుటలు;

            ఒకవేళ (--పుటలు == 0) {
                తిరిగి పుట;
            }

            కొరకు (p = పుట + 1; పుటలు; పుటలు--) {
                p->slab = NGX_SLAB_పుట_BUSY;
                p->తదుపరి = ఖాళీ;
                p->prev = NGX_SLAB_పుట;
                p++;
            }

            తిరిగి పుట;
        }
    }

    ఒకవేళ (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    తిరిగి ఖాళీ;
}


స్థిర శూన్య
ngx_slab_free_పుటలు(ngx_slab_pool_t *pool, ngx_slab_పుట_t *పుట,
    ngx_uint_t పుటలు)
{
    ngx_slab_పుట_t  *prev, *join;

    pool->pfree += పుటలు;

    పుట->slab = పుటలు--;

    ఒకవేళ (పుటలు) {
        ngx_memzero(&పుట[1], పుటలు * పరిమాణం(ngx_slab_పుట_t));
    }

    ఒకవేళ (పుట->తదుపరి) {
        prev = ngx_slab_పుట_prev(పుట);
        prev->తదుపరి = పుట->తదుపరి;
        పుట->తదుపరి->prev = పుట->prev;
    }

    join = పుట + పుట->slab;

    ఒకవేళ (join < pool->చివరి) {

        ఒకవేళ (ngx_slab_పుట_type(join) == NGX_SLAB_పుట) {

            ఒకవేళ (join->తదుపరి != ఖాళీ) {
                పుటలు += join->slab;
                పుట->slab += join->slab;

                prev = ngx_slab_పుట_prev(join);
                prev->తదుపరి = join->తదుపరి;
                join->తదుపరి->prev = join->prev;

                join->slab = NGX_SLAB_పుట_FREE;
                join->తదుపరి = ఖాళీ;
                join->prev = NGX_SLAB_పుట;
            }
        }
    }

    ఒకవేళ (పుట > pool->పుటలు) {
        join = పుట - 1;

        ఒకవేళ (ngx_slab_పుట_type(join) == NGX_SLAB_పుట) {

            ఒకవేళ (join->slab == NGX_SLAB_పుట_FREE) {
                join = ngx_slab_పుట_prev(join);
            }

            ఒకవేళ (join->తదుపరి != ఖాళీ) {
                పుటలు += join->slab;
                join->slab += పుట->slab;

                prev = ngx_slab_పుట_prev(join);
                prev->తదుపరి = join->తదుపరి;
                join->తదుపరి->prev = join->prev;

                పుట->slab = NGX_SLAB_పుట_FREE;
                పుట->తదుపరి = ఖాళీ;
                పుట->prev = NGX_SLAB_పుట;

                పుట = join;
            }
        }
    }

    ఒకవేళ (పుటలు) {
        పుట[పుటలు].prev = (uintptr_t) పుట;
    }

    పుట->prev = (uintptr_t) &pool->విడుదల;
    పుట->తదుపరి = pool->విడుదల.తదుపరి;

    పుట->తదుపరి->prev = (uintptr_t) పుట;

    pool->విడుదల.తదుపరి = పుట;
}


స్థిర శూన్య
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, అక్షరం *పాఠ్యం)
{
    ngx_log_error(level, ngx_చక్రం->సంవర్గమానం, 0, "%s%s", పాఠ్యం, pool->log_ctx);
}
