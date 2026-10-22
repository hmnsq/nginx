
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ
ngx_spinlock(ngx_atomic_t *قلف, ngx_atomic_int_t ارزښت, ngx_uint_t spin)
{

#إذا (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    لكل ( ;; ) {

        إذا (*قلف == 0 && ngx_atomic_cmp_سېټ(قلف, 0, ارزښت)) {
            أرجع;
        }

        إذا (ngx_ncpu > 1) {

            لكل (n = 1; n < spin; n <<= 1) {

                لكل (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                إذا (*قلف == 0 && ngx_atomic_cmp_سېټ(قلف, 0, ارزښت)) {
                    أرجع;
                }
            }
        }

        ngx_sched_yield();
    }

#وإلا

#إذا (NGX_تارونه)

#تېروتنه ngx_spinlock() or ngx_atomic_cmp_سېټ() are not defined !

#د_که_پای

#د_که_پای

}
