
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#ຖ້າ (NGX_HAVE_ATOMIC_OPS)


#define NGX_RWLOCK_SPIN   2048
#define NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


void
ngx_rwlock_wlock(ngx_atomic_t *lock)
{
    ngx_uint_t  i, n;

    ສຳລັບ ( ;; ) {

        ຖ້າ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK)) {
            ສົ່ງຄືນ;
        }

        ຖ້າ (ngx_ncpu > 1) {

            ສຳລັບ (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                ສຳລັບ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ຖ້າ (*lock == 0
                    && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK))
                {
                    ສົ່ງຄືນ;
                }
            }
        }

        ngx_sched_yield();
    }
}


void
ngx_rwlock_rlock(ngx_atomic_t *lock)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    ສຳລັບ ( ;; ) {
        readers = *lock;

        ຖ້າ (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_set(lock, readers, readers + 1))
        {
            ສົ່ງຄືນ;
        }

        ຖ້າ (ngx_ncpu > 1) {

            ສຳລັບ (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                ສຳລັບ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *lock;

                ຖ້າ (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_set(lock, readers, readers + 1))
                {
                    ສົ່ງຄືນ;
                }
            }
        }

        ngx_sched_yield();
    }
}


void
ngx_rwlock_unlock(ngx_atomic_t *lock)
{
    ຖ້າ (*lock == NGX_RWLOCK_WLOCK) {
        (void) ngx_atomic_cmp_set(lock, NGX_RWLOCK_WLOCK, 0);
    } ບໍ່ດັ່ງນັ້ນ {
        (void) ngx_atomic_fetch_add(lock, -1);
    }
}


void
ngx_rwlock_downgrade(ngx_atomic_t *lock)
{
    ຖ້າ (*lock == NGX_RWLOCK_WLOCK) {
        *lock = 1;
    }
}


#ບໍ່ດັ່ງນັ້ນ

#ຖ້າ (NGX_HTTP_UPSTREAM_ZONE || NGX_STREAM_UPSTREAM_ZONE)

#error ngx_atomic_cmp_set() is not defined!

#endif

#endif
