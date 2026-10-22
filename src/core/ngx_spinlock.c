
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


व्योम
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t मूल्य, ngx_uint_t spin)
{

#अगर (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    क्रम ( ;; ) {

        अगर (*lock == 0 && ngx_atomic_cmp_set(lock, 0, मूल्य)) {
            वापस;
        }

        अगर (ngx_ncpu > 1) {

            क्रम (n = 1; n < spin; n <<= 1) {

                क्रम (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                अगर (*lock == 0 && ngx_atomic_cmp_set(lock, 0, मूल्य)) {
                    वापस;
                }
            }
        }

        ngx_sched_yield();
    }

#अन्यथा

#अगर (NGX_THREADS)

#त्रुटि ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#पूर्ण_अगर

#पूर्ण_अगर

}
