
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#если (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    для ( ;; ) {

        если (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            вернуть;
        }

        если (ngx_ncpu > 1) {

            для (n = 1; n < spin; n <<= 1) {

                для (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                если (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    вернуть;
                }
            }
        }

        ngx_sched_yield();
    }

#иначе

#если (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
