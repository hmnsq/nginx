
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#නම් (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    සඳහා ( ;; ) {

        නම් (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            ආපසු;
        }

        නම් (ngx_ncpu > 1) {

            සඳහා (n = 1; n < spin; n <<= 1) {

                සඳහා (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                නම් (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    ආපසු;
                }
            }
        }

        ngx_sched_yield();
    }

#නැතහොත්

#නම් (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
