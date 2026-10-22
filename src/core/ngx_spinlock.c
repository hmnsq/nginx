
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


शून्य
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t मानम्, ngx_uint_t spin)
{

#सचे (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    अत्थाय ( ;; ) {

        सचे (*lock == 0 && ngx_atomic_cmp_set(lock, 0, मानम्)) {
            पटिनिवत्त;
        }

        सचे (ngx_ncpu > 1) {

            अत्थाय (n = 1; n < spin; n <<= 1) {

                अत्थाय (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                सचे (*lock == 0 && ngx_atomic_cmp_set(lock, 0, मानम्)) {
                    पटिनिवत्त;
                }
            }
        }

        ngx_sched_yield();
    }

#अञ्ञथा

#सचे (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
