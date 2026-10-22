
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کن NGX_SLAB_صفحه_ماسک   3
#تعریف_کن NGX_SLAB_صفحه        0
#تعریف_کن NGX_SLAB_BIG         1
#تعریف_کن NGX_SLAB_EXACT       2
#تعریف_کن NGX_SLAB_SMALL       3

#إذا (NGX_PTR_اندازه == 4)

#تعریف_کن NGX_SLAB_صفحه_FREE   0
#تعریف_کن NGX_SLAB_صفحه_BUSY   0xffffffff
#تعریف_کن NGX_SLAB_صفحه_شروع  0x80000000

#تعریف_کن NGX_SLAB_لغزش_ماسک  0x0000000f
#تعریف_کن NGX_SLAB_MAP_ماسک    0xffff0000
#تعریف_کن NGX_SLAB_MAP_لغزش   16

#تعریف_کن NGX_SLAB_BUSY        0xffffffff

#وإلا /* (NGX_PTR_SIZE == 8) */

#تعریف_کن NGX_SLAB_صفحه_FREE   0
#تعریف_کن NGX_SLAB_صفحه_BUSY   0xffffffffffffffff
#تعریف_کن NGX_SLAB_صفحه_شروع  0x8000000000000000

#تعریف_کن NGX_SLAB_لغزش_ماسک  0x000000000000000f
#تعریف_کن NGX_SLAB_MAP_ماسک    0xffffffff00000000
#تعریف_کن NGX_SLAB_MAP_لغزش   32

#تعریف_کن NGX_SLAB_BUSY        0xffffffffffffffff

#پایان_اگر


#تعریف_کن ngx_slab_slots(pool)                                                  \
    (ngx_slab_صفحه_t *) ((u_char *) (pool) + حجم(ngx_slab_pool_t))

#تعریف_کن ngx_slab_صفحه_type(صفحه)   ((صفحه)->prev & NGX_SLAB_صفحه_ماسک)

#تعریف_کن ngx_slab_صفحه_prev(صفحه)                                              \
    (ngx_slab_صفحه_t *) ((صفحه)->prev & ~NGX_SLAB_صفحه_ماسک)

#تعریف_کن ngx_slab_صفحه_addr(pool, صفحه)                                        \
    ((((صفحه) - (pool)->صفحات) << ngx_pagesize_لغزش)                         \
     + (uintptr_t) (pool)->شروع)


#إذا (NGX_DEBUG_MALLOC)

#تعریف_کن ngx_slab_junk(p, اندازه)     ngx_memset(p, 0xA5, اندازه)

#وإلاإذا (NGX_HAVE_DEBUG_MALLOC)

#تعریف_کن ngx_slab_junk(p, اندازه)                                                \
    إذا (ngx_debug_malloc)          ngx_memset(p, 0xA5, اندازه)

#وإلا

#تعریف_کن ngx_slab_junk(p, اندازه)

#پایان_اگر

ساكن ngx_slab_صفحه_t *ngx_slab_alloc_صفحات(ngx_slab_pool_t *pool,
    ngx_uint_t صفحات);
ساكن فراغ ngx_slab_free_صفحات(ngx_slab_pool_t *pool, ngx_slab_صفحه_t *صفحه,
    ngx_uint_t صفحات);
ساكن فراغ ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level,
    حرف *متن);


ساكن ngx_uint_t  ngx_slab_max_اندازه;
ساكن ngx_uint_t  ngx_slab_exact_اندازه;
ساكن ngx_uint_t  ngx_slab_exact_لغزش;


فراغ
ngx_slab_sizes_init(فراغ)
{
    ngx_uint_t  n;

    ngx_slab_max_اندازه = ngx_pagesize / 2;
    ngx_slab_exact_اندازه = ngx_pagesize / (8 * حجم(uintptr_t));
    لكل (n = ngx_slab_exact_اندازه; n >>= 1; ngx_slab_exact_لغزش++) {
        /* void */
    }
}


فراغ
ngx_slab_init(ngx_slab_pool_t *pool)
{
    u_char           *p;
    اندازه_t            اندازه;
    ngx_int_t         m;
    ngx_uint_t        i, n, صفحات;
    ngx_slab_صفحه_t  *slots, *صفحه;

    pool->min_اندازه = (اندازه_t) 1 << pool->min_لغزش;

    slots = ngx_slab_slots(pool);

    p = (u_char *) slots;
    اندازه = pool->انتها - p;

    ngx_slab_junk(p, اندازه);

    n = ngx_pagesize_لغزش - pool->min_لغزش;

    لكل (i = 0; i < n; i++) {
        /* only "next" is used in list head */
        slots[i].slab = 0;
        slots[i].بعدی = &slots[i];
        slots[i].prev = 0;
    }

    p += n * حجم(ngx_slab_صفحه_t);

    pool->stats = (ngx_slab_stat_t *) p;
    ngx_memzero(pool->stats, n * حجم(ngx_slab_stat_t));

    p += n * حجم(ngx_slab_stat_t);

    اندازه -= n * (حجم(ngx_slab_صفحه_t) + حجم(ngx_slab_stat_t));

    صفحات = (ngx_uint_t) (اندازه / (ngx_pagesize + حجم(ngx_slab_صفحه_t)));

    pool->صفحات = (ngx_slab_صفحه_t *) p;
    ngx_memzero(pool->صفحات, صفحات * حجم(ngx_slab_صفحه_t));

    صفحه = pool->صفحات;

    /* only "next" is used in list head */
    pool->حرر.slab = 0;
    pool->حرر.بعدی = صفحه;
    pool->حرر.prev = 0;

    صفحه->slab = صفحات;
    صفحه->بعدی = &pool->حرر;
    صفحه->prev = (uintptr_t) &pool->حرر;

    pool->شروع = ngx_align_ptr(p + صفحات * حجم(ngx_slab_صفحه_t),
                                ngx_pagesize);

    m = صفحات - (pool->انتها - pool->شروع) / ngx_pagesize;
    إذا (m > 0) {
        صفحات -= m;
        صفحه->slab = صفحات;
    }

    pool->آخرین = pool->صفحات + صفحات;
    pool->pfree = صفحات;

    pool->log_nomem = 1;
    pool->log_ctx = &pool->zero;
    pool->zero = '\0';
}


فراغ *
ngx_slab_alloc(ngx_slab_pool_t *pool, اندازه_t اندازه)
{
    فراغ  *p;

    ngx_shmtx_قفل(&pool->mutex);

    p = ngx_slab_alloc_locked(pool, اندازه);

    ngx_shmtx_unlock(&pool->mutex);

    أرجع p;
}


فراغ *
ngx_slab_alloc_locked(ngx_slab_pool_t *pool, اندازه_t اندازه)
{
    اندازه_t            s;
    uintptr_t         p, m, ماسک, *bitmap;
    ngx_uint_t        i, n, جایگاه, لغزش, map;
    ngx_slab_صفحه_t  *صفحه, *prev, *slots;

    إذا (اندازه > ngx_slab_max_اندازه) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_چرخه->لگاریتم, 0,
                       "slab alloc: %uz", اندازه);

        صفحه = ngx_slab_alloc_صفحات(pool, (اندازه >> ngx_pagesize_لغزش)
                                          + ((اندازه % ngx_pagesize) ? 1 : 0));
        إذا (صفحه) {
            p = ngx_slab_صفحه_addr(pool, صفحه);

        } وإلا {
            p = 0;
        }

        اذهب انجامشده;
    }

    إذا (اندازه > pool->min_اندازه) {
        لغزش = 1;
        لكل (s = اندازه - 1; s >>= 1; لغزش++) { /* void */ }
        جایگاه = لغزش - pool->min_لغزش;

    } وإلا {
        لغزش = pool->min_لغزش;
        جایگاه = 0;
    }

    pool->stats[جایگاه].reqs++;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, ngx_چرخه->لگاریتم, 0,
                   "slab alloc: %uz slot: %ui", اندازه, جایگاه);

    slots = ngx_slab_slots(pool);
    صفحه = slots[جایگاه].بعدی;

    إذا (صفحه->بعدی != صفحه) {

        إذا (لغزش < ngx_slab_exact_لغزش) {

            bitmap = (uintptr_t *) ngx_slab_صفحه_addr(pool, صفحه);

            map = (ngx_pagesize >> لغزش) / (8 * حجم(uintptr_t));

            لكل (n = 0; n < map; n++) {

                إذا (bitmap[n] != NGX_SLAB_BUSY) {

                    لكل (m = 1, i = 0; m; m <<= 1, i++) {
                        إذا (bitmap[n] & m) {
                            استمر;
                        }

                        bitmap[n] |= m;

                        i = (n * 8 * حجم(uintptr_t) + i) << لغزش;

                        p = (uintptr_t) bitmap + i;

                        pool->stats[جایگاه].استفاده_شده++;

                        إذا (bitmap[n] == NGX_SLAB_BUSY) {
                            لكل (n = n + 1; n < map; n++) {
                                إذا (bitmap[n] != NGX_SLAB_BUSY) {
                                    اذهب انجامشده;
                                }
                            }

                            prev = ngx_slab_صفحه_prev(صفحه);
                            prev->بعدی = صفحه->بعدی;
                            صفحه->بعدی->prev = صفحه->prev;

                            صفحه->بعدی = عدم;
                            صفحه->prev = NGX_SLAB_SMALL;
                        }

                        اذهب انجامشده;
                    }
                }
            }

        } وإلا إذا (لغزش == ngx_slab_exact_لغزش) {

            لكل (m = 1, i = 0; m; m <<= 1, i++) {
                إذا (صفحه->slab & m) {
                    استمر;
                }

                صفحه->slab |= m;

                إذا (صفحه->slab == NGX_SLAB_BUSY) {
                    prev = ngx_slab_صفحه_prev(صفحه);
                    prev->بعدی = صفحه->بعدی;
                    صفحه->بعدی->prev = صفحه->prev;

                    صفحه->بعدی = عدم;
                    صفحه->prev = NGX_SLAB_EXACT;
                }

                p = ngx_slab_صفحه_addr(pool, صفحه) + (i << لغزش);

                pool->stats[جایگاه].استفاده_شده++;

                اذهب انجامشده;
            }

        } وإلا { /* shift > ngx_slab_exact_shift */

            ماسک = ((uintptr_t) 1 << (ngx_pagesize >> لغزش)) - 1;
            ماسک <<= NGX_SLAB_MAP_لغزش;

            لكل (m = (uintptr_t) 1 << NGX_SLAB_MAP_لغزش, i = 0;
                 m & ماسک;
                 m <<= 1, i++)
            {
                إذا (صفحه->slab & m) {
                    استمر;
                }

                صفحه->slab |= m;

                إذا ((صفحه->slab & NGX_SLAB_MAP_ماسک) == ماسک) {
                    prev = ngx_slab_صفحه_prev(صفحه);
                    prev->بعدی = صفحه->بعدی;
                    صفحه->بعدی->prev = صفحه->prev;

                    صفحه->بعدی = عدم;
                    صفحه->prev = NGX_SLAB_BIG;
                }

                p = ngx_slab_صفحه_addr(pool, صفحه) + (i << لغزش);

                pool->stats[جایگاه].استفاده_شده++;

                اذهب انجامشده;
            }
        }

        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_alloc(): page is busy");
        ngx_debug_نقطه();
    }

    صفحه = ngx_slab_alloc_صفحات(pool, 1);

    إذا (صفحه) {
        إذا (لغزش < ngx_slab_exact_لغزش) {
            bitmap = (uintptr_t *) ngx_slab_صفحه_addr(pool, صفحه);

            n = (ngx_pagesize >> لغزش) / ((1 << لغزش) * 8);

            إذا (n == 0) {
                n = 1;
            }

            /* "n" elements for bitmap, plus one requested */

            لكل (i = 0; i < (n + 1) / (8 * حجم(uintptr_t)); i++) {
                bitmap[i] = NGX_SLAB_BUSY;
            }

            m = ((uintptr_t) 1 << ((n + 1) % (8 * حجم(uintptr_t)))) - 1;
            bitmap[i] = m;

            map = (ngx_pagesize >> لغزش) / (8 * حجم(uintptr_t));

            لكل (i = i + 1; i < map; i++) {
                bitmap[i] = 0;
            }

            صفحه->slab = لغزش;
            صفحه->بعدی = &slots[جایگاه];
            صفحه->prev = (uintptr_t) &slots[جایگاه] | NGX_SLAB_SMALL;

            slots[جایگاه].بعدی = صفحه;

            pool->stats[جایگاه].مجموع += (ngx_pagesize >> لغزش) - n;

            p = ngx_slab_صفحه_addr(pool, صفحه) + (n << لغزش);

            pool->stats[جایگاه].استفاده_شده++;

            اذهب انجامشده;

        } وإلا إذا (لغزش == ngx_slab_exact_لغزش) {

            صفحه->slab = 1;
            صفحه->بعدی = &slots[جایگاه];
            صفحه->prev = (uintptr_t) &slots[جایگاه] | NGX_SLAB_EXACT;

            slots[جایگاه].بعدی = صفحه;

            pool->stats[جایگاه].مجموع += 8 * حجم(uintptr_t);

            p = ngx_slab_صفحه_addr(pool, صفحه);

            pool->stats[جایگاه].استفاده_شده++;

            اذهب انجامشده;

        } وإلا { /* shift > ngx_slab_exact_shift */

            صفحه->slab = ((uintptr_t) 1 << NGX_SLAB_MAP_لغزش) | لغزش;
            صفحه->بعدی = &slots[جایگاه];
            صفحه->prev = (uintptr_t) &slots[جایگاه] | NGX_SLAB_BIG;

            slots[جایگاه].بعدی = صفحه;

            pool->stats[جایگاه].مجموع += ngx_pagesize >> لغزش;

            p = ngx_slab_صفحه_addr(pool, صفحه);

            pool->stats[جایگاه].استفاده_شده++;

            اذهب انجامشده;
        }
    }

    p = 0;

    pool->stats[جایگاه].fails++;

انجامشده:

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_چرخه->لگاریتم, 0,
                   "slab alloc: %p", (فراغ *) p);

    أرجع (فراغ *) p;
}


فراغ *
ngx_slab_calloc(ngx_slab_pool_t *pool, اندازه_t اندازه)
{
    فراغ  *p;

    ngx_shmtx_قفل(&pool->mutex);

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
    ngx_shmtx_قفل(&pool->mutex);

    ngx_slab_free_locked(pool, p);

    ngx_shmtx_unlock(&pool->mutex);
}


فراغ
ngx_slab_free_locked(ngx_slab_pool_t *pool, فراغ *p)
{
    اندازه_t            اندازه;
    uintptr_t         slab, m, *bitmap;
    ngx_uint_t        i, n, type, جایگاه, لغزش, map;
    ngx_slab_صفحه_t  *slots, *صفحه;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, ngx_چرخه->لگاریتم, 0, "slab free: %p", p);

    إذا ((u_char *) p < pool->شروع || (u_char *) p > pool->انتها) {
        ngx_slab_error(pool, NGX_LOG_ALERT, "ngx_slab_free(): outside of pool");
        اذهب fail;
    }

    n = ((u_char *) p - pool->شروع) >> ngx_pagesize_لغزش;
    صفحه = &pool->صفحات[n];
    slab = صفحه->slab;
    type = ngx_slab_صفحه_type(صفحه);

    بدل (type) {

    حالة NGX_SLAB_SMALL:

        لغزش = slab & NGX_SLAB_لغزش_ماسک;
        اندازه = (اندازه_t) 1 << لغزش;

        إذا ((uintptr_t) p & (اندازه - 1)) {
            اذهب wrong_chunk;
        }

        n = ((uintptr_t) p & (ngx_pagesize - 1)) >> لغزش;
        m = (uintptr_t) 1 << (n % (8 * حجم(uintptr_t)));
        n /= 8 * حجم(uintptr_t);
        bitmap = (uintptr_t *)
                             ((uintptr_t) p & ~((uintptr_t) ngx_pagesize - 1));

        إذا (bitmap[n] & m) {
            جایگاه = لغزش - pool->min_لغزش;

            إذا (صفحه->بعدی == عدم) {
                slots = ngx_slab_slots(pool);

                صفحه->بعدی = slots[جایگاه].بعدی;
                slots[جایگاه].بعدی = صفحه;

                صفحه->prev = (uintptr_t) &slots[جایگاه] | NGX_SLAB_SMALL;
                صفحه->بعدی->prev = (uintptr_t) صفحه | NGX_SLAB_SMALL;
            }

            bitmap[n] &= ~m;

            n = (ngx_pagesize >> لغزش) / ((1 << لغزش) * 8);

            إذا (n == 0) {
                n = 1;
            }

            i = n / (8 * حجم(uintptr_t));
            m = ((uintptr_t) 1 << (n % (8 * حجم(uintptr_t)))) - 1;

            إذا (bitmap[i] & ~m) {
                اذهب انجامشده;
            }

            map = (ngx_pagesize >> لغزش) / (8 * حجم(uintptr_t));

            لكل (i = i + 1; i < map; i++) {
                إذا (bitmap[i]) {
                    اذهب انجامشده;
                }
            }

            ngx_slab_free_صفحات(pool, صفحه, 1);

            pool->stats[جایگاه].مجموع -= (ngx_pagesize >> لغزش) - n;

            اذهب انجامشده;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_EXACT:

        m = (uintptr_t) 1 <<
                (((uintptr_t) p & (ngx_pagesize - 1)) >> ngx_slab_exact_لغزش);
        اندازه = ngx_slab_exact_اندازه;

        إذا ((uintptr_t) p & (اندازه - 1)) {
            اذهب wrong_chunk;
        }

        إذا (slab & m) {
            جایگاه = ngx_slab_exact_لغزش - pool->min_لغزش;

            إذا (slab == NGX_SLAB_BUSY) {
                slots = ngx_slab_slots(pool);

                صفحه->بعدی = slots[جایگاه].بعدی;
                slots[جایگاه].بعدی = صفحه;

                صفحه->prev = (uintptr_t) &slots[جایگاه] | NGX_SLAB_EXACT;
                صفحه->بعدی->prev = (uintptr_t) صفحه | NGX_SLAB_EXACT;
            }

            صفحه->slab &= ~m;

            إذا (صفحه->slab) {
                اذهب انجامشده;
            }

            ngx_slab_free_صفحات(pool, صفحه, 1);

            pool->stats[جایگاه].مجموع -= 8 * حجم(uintptr_t);

            اذهب انجامشده;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_BIG:

        لغزش = slab & NGX_SLAB_لغزش_ماسک;
        اندازه = (اندازه_t) 1 << لغزش;

        إذا ((uintptr_t) p & (اندازه - 1)) {
            اذهب wrong_chunk;
        }

        m = (uintptr_t) 1 << ((((uintptr_t) p & (ngx_pagesize - 1)) >> لغزش)
                              + NGX_SLAB_MAP_لغزش);

        إذا (slab & m) {
            جایگاه = لغزش - pool->min_لغزش;

            إذا (صفحه->بعدی == عدم) {
                slots = ngx_slab_slots(pool);

                صفحه->بعدی = slots[جایگاه].بعدی;
                slots[جایگاه].بعدی = صفحه;

                صفحه->prev = (uintptr_t) &slots[جایگاه] | NGX_SLAB_BIG;
                صفحه->بعدی->prev = (uintptr_t) صفحه | NGX_SLAB_BIG;
            }

            صفحه->slab &= ~m;

            إذا (صفحه->slab & NGX_SLAB_MAP_ماسک) {
                اذهب انجامشده;
            }

            ngx_slab_free_صفحات(pool, صفحه, 1);

            pool->stats[جایگاه].مجموع -= ngx_pagesize >> لغزش;

            اذهب انجامشده;
        }

        اذهب chunk_already_free;

    حالة NGX_SLAB_صفحه:

        إذا ((uintptr_t) p & (ngx_pagesize - 1)) {
            اذهب wrong_chunk;
        }

        إذا (!(slab & NGX_SLAB_صفحه_شروع)) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): page is already free");
            اذهب fail;
        }

        إذا (slab == NGX_SLAB_صفحه_BUSY) {
            ngx_slab_error(pool, NGX_LOG_ALERT,
                           "ngx_slab_free(): pointer to wrong page");
            اذهب fail;
        }

        اندازه = slab & ~NGX_SLAB_صفحه_شروع;

        ngx_slab_free_صفحات(pool, صفحه, اندازه);

        ngx_slab_junk(p, اندازه << ngx_pagesize_لغزش);

        أرجع;
    }

    /* not reached */

    أرجع;

انجامشده:

    pool->stats[جایگاه].استفاده_شده--;

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


ساكن ngx_slab_صفحه_t *
ngx_slab_alloc_صفحات(ngx_slab_pool_t *pool, ngx_uint_t صفحات)
{
    ngx_slab_صفحه_t  *صفحه, *p;

    لكل (صفحه = pool->حرر.بعدی; صفحه != &pool->حرر; صفحه = صفحه->بعدی) {

        إذا (صفحه->slab >= صفحات) {

            إذا (صفحه->slab > صفحات) {
                صفحه[صفحه->slab - 1].prev = (uintptr_t) &صفحه[صفحات];

                صفحه[صفحات].slab = صفحه->slab - صفحات;
                صفحه[صفحات].بعدی = صفحه->بعدی;
                صفحه[صفحات].prev = صفحه->prev;

                p = (ngx_slab_صفحه_t *) صفحه->prev;
                p->بعدی = &صفحه[صفحات];
                صفحه->بعدی->prev = (uintptr_t) &صفحه[صفحات];

            } وإلا {
                p = (ngx_slab_صفحه_t *) صفحه->prev;
                p->بعدی = صفحه->بعدی;
                صفحه->بعدی->prev = صفحه->prev;
            }

            صفحه->slab = صفحات | NGX_SLAB_صفحه_شروع;
            صفحه->بعدی = عدم;
            صفحه->prev = NGX_SLAB_صفحه;

            pool->pfree -= صفحات;

            إذا (--صفحات == 0) {
                أرجع صفحه;
            }

            لكل (p = صفحه + 1; صفحات; صفحات--) {
                p->slab = NGX_SLAB_صفحه_BUSY;
                p->بعدی = عدم;
                p->prev = NGX_SLAB_صفحه;
                p++;
            }

            أرجع صفحه;
        }
    }

    إذا (pool->log_nomem) {
        ngx_slab_error(pool, NGX_LOG_CRIT,
                       "ngx_slab_alloc() failed: no memory");
    }

    أرجع عدم;
}


ساكن فراغ
ngx_slab_free_صفحات(ngx_slab_pool_t *pool, ngx_slab_صفحه_t *صفحه,
    ngx_uint_t صفحات)
{
    ngx_slab_صفحه_t  *prev, *join;

    pool->pfree += صفحات;

    صفحه->slab = صفحات--;

    إذا (صفحات) {
        ngx_memzero(&صفحه[1], صفحات * حجم(ngx_slab_صفحه_t));
    }

    إذا (صفحه->بعدی) {
        prev = ngx_slab_صفحه_prev(صفحه);
        prev->بعدی = صفحه->بعدی;
        صفحه->بعدی->prev = صفحه->prev;
    }

    join = صفحه + صفحه->slab;

    إذا (join < pool->آخرین) {

        إذا (ngx_slab_صفحه_type(join) == NGX_SLAB_صفحه) {

            إذا (join->بعدی != عدم) {
                صفحات += join->slab;
                صفحه->slab += join->slab;

                prev = ngx_slab_صفحه_prev(join);
                prev->بعدی = join->بعدی;
                join->بعدی->prev = join->prev;

                join->slab = NGX_SLAB_صفحه_FREE;
                join->بعدی = عدم;
                join->prev = NGX_SLAB_صفحه;
            }
        }
    }

    إذا (صفحه > pool->صفحات) {
        join = صفحه - 1;

        إذا (ngx_slab_صفحه_type(join) == NGX_SLAB_صفحه) {

            إذا (join->slab == NGX_SLAB_صفحه_FREE) {
                join = ngx_slab_صفحه_prev(join);
            }

            إذا (join->بعدی != عدم) {
                صفحات += join->slab;
                join->slab += صفحه->slab;

                prev = ngx_slab_صفحه_prev(join);
                prev->بعدی = join->بعدی;
                join->بعدی->prev = join->prev;

                صفحه->slab = NGX_SLAB_صفحه_FREE;
                صفحه->بعدی = عدم;
                صفحه->prev = NGX_SLAB_صفحه;

                صفحه = join;
            }
        }
    }

    إذا (صفحات) {
        صفحه[صفحات].prev = (uintptr_t) صفحه;
    }

    صفحه->prev = (uintptr_t) &pool->حرر;
    صفحه->بعدی = pool->حرر.بعدی;

    صفحه->بعدی->prev = (uintptr_t) صفحه;

    pool->حرر.بعدی = صفحه;
}


ساكن فراغ
ngx_slab_error(ngx_slab_pool_t *pool, ngx_uint_t level, حرف *متن)
{
    ngx_log_error(level, ngx_چرخه->لگاریتم, 0, "%s%s", متن, pool->log_ctx);
}
