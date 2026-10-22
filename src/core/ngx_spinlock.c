
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


శూన్య
ngx_spinlock(ngx_atomic_t *తాళం, ngx_atomic_int_t విలువ, ngx_uint_t spin)
{

#ఒకవేళ (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    కొరకు ( ;; ) {

        ఒకవేళ (*తాళం == 0 && ngx_atomic_cmp_సమితి(తాళం, 0, విలువ)) {
            తిరిగి;
        }

        ఒకవేళ (ngx_ncpu > 1) {

            కొరకు (n = 1; n < spin; n <<= 1) {

                కొరకు (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ఒకవేళ (*తాళం == 0 && ngx_atomic_cmp_సమితి(తాళం, 0, విలువ)) {
                    తిరిగి;
                }
            }
        }

        ngx_sched_yield();
    }

#లేకపోతే

#ఒకవేళ (NGX_దారాలు)

#దోషం ngx_spinlock() or ngx_atomic_cmp_సమితి() are not defined !

#అయితే_ముగింపు

#అయితే_ముగింపు

}
