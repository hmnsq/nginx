
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#만약 (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    위해 ( ;; ) {

        만약 (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            반환;
        }

        만약 (ngx_ncpu > 1) {

            위해 (n = 1; n < spin; n <<= 1) {

                위해 (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                만약 (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    반환;
                }
            }
        }

        ngx_sched_yield();
    }

#아니면

#만약 (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
