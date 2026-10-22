
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਖਾਲੀ
ngx_spinlock(ngx_atomic_t *ਤਾਲਾ, ngx_atomic_int_t ਮੁੱਲ, ngx_uint_t spin)
{

#ਜੇ (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    ਲਈ ( ;; ) {

        ਜੇ (*ਤਾਲਾ == 0 && ngx_atomic_cmp_ਸੈੱਟ(ਤਾਲਾ, 0, ਮੁੱਲ)) {
            ਵਾਪਸ;
        }

        ਜੇ (ngx_ncpu > 1) {

            ਲਈ (n = 1; n < spin; n <<= 1) {

                ਲਈ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ਜੇ (*ਤਾਲਾ == 0 && ngx_atomic_cmp_ਸੈੱਟ(ਤਾਲਾ, 0, ਮੁੱਲ)) {
                    ਵਾਪਸ;
                }
            }
        }

        ngx_sched_yield();
    }

#ਨਹੀਂਤਾਂ

#ਜੇ (NGX_ਧਾਗੇ)

#ਗਲਤੀ ngx_spinlock() or ngx_atomic_cmp_ਸੈੱਟ() are not defined !

#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

}
