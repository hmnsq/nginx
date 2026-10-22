
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ
ngx_spinlock(ngx_atomic_t *جندرا, ngx_atomic_int_t قدر, ngx_uint_t spin)
{

#إذا (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    لكل ( ;; ) {

        إذا (*جندرا == 0 && ngx_atomic_cmp_سیٹ(جندرا, 0, قدر)) {
            أرجع;
        }

        إذا (ngx_ncpu > 1) {

            لكل (n = 1; n < spin; n <<= 1) {

                لكل (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                إذا (*جندرا == 0 && ngx_atomic_cmp_سیٹ(جندرا, 0, قدر)) {
                    أرجع;
                }
            }
        }

        ngx_sched_yield();
    }

#وإلا

#إذا (NGX_دھاگے)

#غلطی ngx_spinlock() or ngx_atomic_cmp_سیٹ() are not defined !

#جے_انت

#جے_انت

}
