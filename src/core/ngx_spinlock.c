
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>


শূন্য
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t মান, ngx_uint_t spin)
{

#যদি (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    বাবে ( ;; ) {

        যদি (*lock == 0 && ngx_atomic_cmp_set(lock, 0, মান)) {
            ঘূৰাই;
        }

        যদি (ngx_ncpu > 1) {

            বাবে (n = 1; n < spin; n <<= 1) {

                বাবে (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                যদি (*lock == 0 && ngx_atomic_cmp_set(lock, 0, মান)) {
                    ঘূৰাই;
                }
            }
        }

        ngx_sched_yield();
    }

#অন্যথা

#যদি (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
