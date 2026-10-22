
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#ܐܢ (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    لكل ( ;; ) {

        ܐܢ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            ܦܢܐ;
        }

        ܐܢ (ngx_ncpu > 1) {

            لكل (n = 1; n < spin; n <<= 1) {

                لكل (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ܐܢ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    ܦܢܐ;
                }
            }
        }

        ngx_sched_yield();
    }

#ܐܚܪܢܐ

#ܐܢ (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
