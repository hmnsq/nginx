
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#ຖ້າ (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    ສຳລັບ ( ;; ) {

        ຖ້າ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            ສົ່ງຄືນ;
        }

        ຖ້າ (ngx_ncpu > 1) {

            ສຳລັບ (n = 1; n < spin; n <<= 1) {

                ສຳລັບ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ຖ້າ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    ສົ່ງຄືນ;
                }
            }
        }

        ngx_sched_yield();
    }

#ບໍ່ດັ່ງນັ້ນ

#ຖ້າ (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
