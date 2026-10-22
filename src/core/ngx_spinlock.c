
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#nếu (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    cho ( ;; ) {

        nếu (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            trả_về;
        }

        nếu (ngx_ncpu > 1) {

            cho (n = 1; n < spin; n <<= 1) {

                cho (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                nếu (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    trả_về;
                }
            }
        }

        ngx_sched_yield();
    }

#không_thì

#nếu (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
