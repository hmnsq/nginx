
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


تهی
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t مقدار, ngx_uint_t spin)
{

#اگر (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    برای ( ;; ) {

        اگر (*lock == 0 && ngx_atomic_cmp_set(lock, 0, مقدار)) {
            بازگشت;
        }

        اگر (ngx_ncpu > 1) {

            برای (n = 1; n < spin; n <<= 1) {

                برای (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                اگر (*lock == 0 && ngx_atomic_cmp_set(lock, 0, مقدار)) {
                    بازگشت;
                }
            }
        }

        ngx_sched_yield();
    }

#وگرنه

#اگر (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
