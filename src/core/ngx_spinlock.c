
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#གལ་ཏེ (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    ཆེད་དུ ( ;; ) {

        གལ་ཏེ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            ཕྱིར;
        }

        གལ་ཏེ (ngx_ncpu > 1) {

            ཆེད་དུ (n = 1; n < spin; n <<= 1) {

                ཆེད་དུ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                གལ་ཏེ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    ཕྱིར;
                }
            }
        }

        ngx_sched_yield();
    }

#དེ་མིན

#གལ་ཏེ (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
