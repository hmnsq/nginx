
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#ако (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    за ( ;; ) {

        ако (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            върни;
        }

        ако (ngx_ncpu > 1) {

            за (n = 1; n < spin; n <<= 1) {

                за (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ако (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    върни;
                }
            }
        }

        ngx_sched_yield();
    }

#иначе

#ако (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
