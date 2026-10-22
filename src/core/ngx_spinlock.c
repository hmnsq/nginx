
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ
ngx_spinlock(ngx_atomic_t *قفل, ngx_atomic_int_t مقدار, ngx_uint_t spin)
{

#إذا (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    لكل ( ;; ) {

        إذا (*قفل == 0 && ngx_atomic_cmp_مجموعه(قفل, 0, مقدار)) {
            أرجع;
        }

        إذا (ngx_ncpu > 1) {

            لكل (n = 1; n < spin; n <<= 1) {

                لكل (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                إذا (*قفل == 0 && ngx_atomic_cmp_مجموعه(قفل, 0, مقدار)) {
                    أرجع;
                }
            }
        }

        ngx_sched_yield();
    }

#وإلا

#إذا (NGX_نخها)

#خطا ngx_spinlock() or ngx_atomic_cmp_مجموعه() are not defined !

#پایان_اگر

#پایان_اگر

}
