
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#གལ་སྲིད (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    དོན་ལུ ( ;; ) {

        གལ་སྲིད (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            ལོག;
        }

        གལ་སྲིད (ngx_ncpu > 1) {

            དོན་ལུ (n = 1; n < spin; n <<= 1) {

                དོན་ལུ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                གལ་སྲིད (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    ལོག;
                }
            }
        }

        ngx_sched_yield();
    }

#དེ་མིན

#གལ་སྲིད (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
