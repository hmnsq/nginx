
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#𐡄𐡍 (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    لكل ( ;; ) {

        𐡄𐡍 (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            أرجع;
        }

        𐡄𐡍 (ngx_ncpu > 1) {

            لكل (n = 1; n < spin; n <<= 1) {

                لكل (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                𐡄𐡍 (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    أرجع;
                }
            }
        }

        ngx_sched_yield();
    }

#وإلا

#𐡄𐡍 (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
