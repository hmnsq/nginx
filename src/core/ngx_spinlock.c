
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


ശൂന്യം
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t മൂല്യം, ngx_uint_t spin)
{

#എങ്കിൽ (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    വേണ്ടി ( ;; ) {

        എങ്കിൽ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, മൂല്യം)) {
            തിരികെ;
        }

        എങ്കിൽ (ngx_ncpu > 1) {

            വേണ്ടി (n = 1; n < spin; n <<= 1) {

                വേണ്ടി (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                എങ്കിൽ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, മൂല്യം)) {
                    തിരികെ;
                }
            }
        }

        ngx_sched_yield();
    }

#അല്ലെങ്കിൽ

#എങ്കിൽ (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
