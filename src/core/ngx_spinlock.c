
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#もし (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    ために ( ;; ) {

        もし (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            戻る;
        }

        もし (ngx_ncpu > 1) {

            ために (n = 1; n < spin; n <<= 1) {

                ために (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                もし (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    戻る;
                }
            }
        }

        ngx_sched_yield();
    }

#否则

#もし (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
