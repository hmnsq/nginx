
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


शून्य
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t मूल्य, ngx_uint_t spin)
{

#यदि (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    प्रति ( ;; ) {

        यदि (*lock == 0 && ngx_atomic_cmp_set(lock, 0, मूल्य)) {
            प्रत्यागम;
        }

        यदि (ngx_ncpu > 1) {

            प्रति (n = 1; n < spin; n <<= 1) {

                प्रति (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                यदि (*lock == 0 && ngx_atomic_cmp_set(lock, 0, मूल्य)) {
                    प्रत्यागम;
                }
            }
        }

        ngx_sched_yield();
    }

#अन्यथा

#यदि (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
