
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#ᬬᬾᬦ᭄ (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    for ( ;; ) {

        ᬬᬾᬦ᭄ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            ᬯᬮᬶ;
        }

        ᬬᬾᬦ᭄ (ngx_ncpu > 1) {

            for (n = 1; n < spin; n <<= 1) {

                for (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ᬬᬾᬦ᭄ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    ᬯᬮᬶ;
                }
            }
        }

        ngx_sched_yield();
    }

#ᬮᬶᬬ

#ᬬᬾᬦ᭄ (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
