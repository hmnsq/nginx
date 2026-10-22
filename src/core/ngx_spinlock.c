
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಶೂನ್ಯ
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t ಮೌಲ್ಯ, ngx_uint_t spin)
{

#ಯದಿ (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    ಪ್ರತಿ ( ;; ) {

        ಯದಿ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, ಮೌಲ್ಯ)) {
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಯದಿ (ngx_ncpu > 1) {

            ಪ್ರತಿ (n = 1; n < spin; n <<= 1) {

                ಪ್ರತಿ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ಯದಿ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, ಮೌಲ್ಯ)) {
                    ಪ್ರತ್ಯಾಗಮ;
                }
            }
        }

        ngx_sched_yield();
    }

#ಅನ್ಯಥಾ

#ಯದಿ (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
