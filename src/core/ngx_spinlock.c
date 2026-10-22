
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#如果 (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    为 ( ;; ) {

        如果 (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            返回;
        }

        如果 (ngx_ncpu > 1) {

            为 (n = 1; n < spin; n <<= 1) {

                为 (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                如果 (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    返回;
                }
            }
        }

        ngx_sched_yield();
    }

#否则

#如果 (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
