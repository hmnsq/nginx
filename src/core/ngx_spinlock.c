
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


શૂન્ય
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t કિંમત, ngx_uint_t spin)
{

#જો (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    માટે ( ;; ) {

        જો (*lock == 0 && ngx_atomic_cmp_set(lock, 0, કિંમત)) {
            પાછા;
        }

        જો (ngx_ncpu > 1) {

            માટે (n = 1; n < spin; n <<= 1) {

                માટે (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                જો (*lock == 0 && ngx_atomic_cmp_set(lock, 0, કિંમત)) {
                    પાછા;
                }
            }
        }

        ngx_sched_yield();
    }

#નહીંતર

#જો (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
