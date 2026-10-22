
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t درج, ngx_uint_t spin)
{

#إذا (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    لكل ( ;; ) {

        إذا (*lock == 0 && ngx_atomic_cmp_set(lock, 0, درج)) {
            أرجع;
        }

        إذا (ngx_ncpu > 1) {

            لكل (n = 1; n < spin; n <<= 1) {

                لكل (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                إذا (*lock == 0 && ngx_atomic_cmp_set(lock, 0, درج)) {
                    أرجع;
                }
            }
        }

        ngx_sched_yield();
    }

#وإلا

#إذا (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
