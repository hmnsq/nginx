
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


शून्य
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t मूल्य, ngx_uint_t spin)
{

#जर (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    साठी ( ;; ) {

        जर (*lock == 0 && ngx_atomic_cmp_set(lock, 0, मूल्य)) {
            परत;
        }

        जर (ngx_ncpu > 1) {

            साठी (n = 1; n < spin; n <<= 1) {

                साठी (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                जर (*lock == 0 && ngx_atomic_cmp_set(lock, 0, मूल्य)) {
                    परत;
                }
            }
        }

        ngx_sched_yield();
    }

#अन्यथा

#जर (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
