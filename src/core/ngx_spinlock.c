
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


خالی
ngx_spinlock(ngx_atomic_t *تالا, ngx_atomic_int_t قدر, ngx_uint_t spin)
{

#اگر (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    برائے ( ;; ) {

        اگر (*تالا == 0 && ngx_atomic_cmp_سیٹ(تالا, 0, قدر)) {
            واپس;
        }

        اگر (ngx_ncpu > 1) {

            برائے (n = 1; n < spin; n <<= 1) {

                برائے (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                اگر (*تالا == 0 && ngx_atomic_cmp_سیٹ(تالا, 0, قدر)) {
                    واپس;
                }
            }
        }

        ngx_sched_yield();
    }

#ورنہ

#اگر (NGX_دھاگے)

#غلطی ngx_spinlock() or ngx_atomic_cmp_سیٹ() are not defined !

#اگر_ختم

#اگر_ختم

}
