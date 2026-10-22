
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


வெறுமை
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t மதிப்பு, ngx_uint_t spin)
{

#எனில் (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    க்கு ( ;; ) {

        எனில் (*lock == 0 && ngx_atomic_cmp_set(lock, 0, மதிப்பு)) {
            திரும்பு;
        }

        எனில் (ngx_ncpu > 1) {

            க்கு (n = 1; n < spin; n <<= 1) {

                க்கு (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                எனில் (*lock == 0 && ngx_atomic_cmp_set(lock, 0, மதிப்பு)) {
                    திரும்பு;
                }
            }
        }

        ngx_sched_yield();
    }

#இல்லையெனில்

#எனில் (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
