
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


#ఒకవేళ (NGX_HAVE_ATOMIC_OPS)


#నిర్వచించు NGX_RWLOCK_SPIN   2048
#నిర్వచించు NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


శూన్య
ngx_rwlock_wlock(ngx_atomic_t *తాళం)
{
    ngx_uint_t  i, n;

    కొరకు ( ;; ) {

        ఒకవేళ (*తాళం == 0 && ngx_atomic_cmp_సమితి(తాళం, 0, NGX_RWLOCK_WLOCK)) {
            తిరిగి;
        }

        ఒకవేళ (ngx_ncpu > 1) {

            కొరకు (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                కొరకు (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ఒకవేళ (*తాళం == 0
                    && ngx_atomic_cmp_సమితి(తాళం, 0, NGX_RWLOCK_WLOCK))
                {
                    తిరిగి;
                }
            }
        }

        ngx_sched_yield();
    }
}


శూన్య
ngx_rwlock_rlock(ngx_atomic_t *తాళం)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    కొరకు ( ;; ) {
        readers = *తాళం;

        ఒకవేళ (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_సమితి(తాళం, readers, readers + 1))
        {
            తిరిగి;
        }

        ఒకవేళ (ngx_ncpu > 1) {

            కొరకు (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                కొరకు (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *తాళం;

                ఒకవేళ (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_సమితి(తాళం, readers, readers + 1))
                {
                    తిరిగి;
                }
            }
        }

        ngx_sched_yield();
    }
}


శూన్య
ngx_rwlock_unlock(ngx_atomic_t *తాళం)
{
    ఒకవేళ (*తాళం == NGX_RWLOCK_WLOCK) {
        (శూన్య) ngx_atomic_cmp_సమితి(తాళం, NGX_RWLOCK_WLOCK, 0);
    } లేకపోతే {
        (శూన్య) ngx_atomic_fetch_కూడు(తాళం, -1);
    }
}


శూన్య
ngx_rwlock_downgrade(ngx_atomic_t *తాళం)
{
    ఒకవేళ (*తాళం == NGX_RWLOCK_WLOCK) {
        *తాళం = 1;
    }
}


#లేకపోతే

#ఒకవేళ (NGX_HTTP_UPSTREAM_ZONE || NGX_ప్రవాహం_UPSTREAM_ZONE)

#దోషం ngx_atomic_cmp_సమితి() is not defined!

#అయితే_ముగింపు

#అయితే_ముగింపు
