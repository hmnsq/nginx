
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#ꦪꦼꦤ꧀ (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    ꦏꦁꦒꦺꦴ ( ;; ) {

        ꦪꦼꦤ꧀ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            ꦮꦭꦶ;
        }

        ꦪꦼꦤ꧀ (ngx_ncpu > 1) {

            ꦏꦁꦒꦺꦴ (n = 1; n < spin; n <<= 1) {

                ꦏꦁꦒꦺꦴ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ꦪꦼꦤ꧀ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    ꦮꦭꦶ;
                }
            }
        }

        ngx_sched_yield();
    }

#ꦭꦶꦪ

#ꦪꦼꦤ꧀ (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
