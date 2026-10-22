
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ଶୂନ୍ୟ
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t ମୂଲ୍ୟ, ngx_uint_t spin)
{

#ଯଦି (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    ପାଇଁ ( ;; ) {

        ଯଦି (*lock == 0 && ngx_atomic_cmp_set(lock, 0, ମୂଲ୍ୟ)) {
            ଫେରନ୍ତୁ;
        }

        ଯଦି (ngx_ncpu > 1) {

            ପାଇଁ (n = 1; n < spin; n <<= 1) {

                ପାଇଁ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ଯଦି (*lock == 0 && ngx_atomic_cmp_set(lock, 0, ମୂଲ୍ୟ)) {
                    ଫେରନ୍ତୁ;
                }
            }
        }

        ngx_sched_yield();
    }

#ଅନ୍ୟଥା

#ଯଦି (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
