
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#אם (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    עבור ( ;; ) {

        אם (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            החזר;
        }

        אם (ngx_ncpu > 1) {

            עבור (n = 1; n < spin; n <<= 1) {

                עבור (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                אם (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    החזר;
                }
            }
        }

        ngx_sched_yield();
    }

#אחרת

#אם (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
