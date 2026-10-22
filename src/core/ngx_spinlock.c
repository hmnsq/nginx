
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#ถ้า (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    สำหรับ ( ;; ) {

        ถ้า (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            คืน;
        }

        ถ้า (ngx_ncpu > 1) {

            สำหรับ (n = 1; n < spin; n <<= 1) {

                สำหรับ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ถ้า (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    คืน;
                }
            }
        }

        ngx_sched_yield();
    }

#มิฉะนั้น

#ถ้า (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
