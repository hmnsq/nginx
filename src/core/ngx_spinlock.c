
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಶೂನ್ಯ
ngx_spinlock(ngx_atomic_t *ಬೀಗ, ngx_atomic_int_t ಮೌಲ್ಯ, ngx_uint_t spin)
{

#ಆದರೆ (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    ಗಾಗಿ ( ;; ) {

        ಆದರೆ (*ಬೀಗ == 0 && ngx_atomic_cmp_ಗಣ(ಬೀಗ, 0, ಮೌಲ್ಯ)) {
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ (ngx_ncpu > 1) {

            ಗಾಗಿ (n = 1; n < spin; n <<= 1) {

                ಗಾಗಿ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ಆದರೆ (*ಬೀಗ == 0 && ngx_atomic_cmp_ಗಣ(ಬೀಗ, 0, ಮೌಲ್ಯ)) {
                    ಹಿಂತಿರುಗಿ;
                }
            }
        }

        ngx_sched_yield();
    }

#ಇಲ್ಲದಿದ್ದರೆ

#ಆದರೆ (NGX_ಎಳೆಗಳು)

#ದೋಷ ngx_spinlock() or ngx_atomic_cmp_ಗಣ() are not defined !

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

}
