
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#ប្រសិនបើ (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    សម្រាប់ ( ;; ) {

        ប្រសិនបើ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            ត្រឡប់;
        }

        ប្រសិនបើ (ngx_ncpu > 1) {

            សម្រាប់ (n = 1; n < spin; n <<= 1) {

                សម្រាប់ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ប្រសិនបើ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    ត្រឡប់;
                }
            }
        }

        ngx_sched_yield();
    }

#បើមិនដូច្នេះ

#ប្រសិនបើ (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
