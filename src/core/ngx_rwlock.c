
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#만약 (NGX_HAVE_ATOMIC_OPS)


#define NGX_RWLOCK_SPIN   2048
#define NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


void
ngx_rwlock_wlock(ngx_atomic_t *lock)
{
    ngx_uint_t  i, n;

    위해 ( ;; ) {

        만약 (*lock == 0 && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK)) {
            반환;
        }

        만약 (ngx_ncpu > 1) {

            위해 (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                위해 (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                만약 (*lock == 0
                    && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK))
                {
                    반환;
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

    위해 ( ;; ) {
        readers = *lock;

        만약 (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_set(lock, readers, readers + 1))
        {
            반환;
        }

        만약 (ngx_ncpu > 1) {

            위해 (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                위해 (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *lock;

                만약 (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_set(lock, readers, readers + 1))
                {
                    반환;
                }
            }
        }

        ngx_sched_yield();
    }
}


void
ngx_rwlock_unlock(ngx_atomic_t *lock)
{
    만약 (*lock == NGX_RWLOCK_WLOCK) {
        (void) ngx_atomic_cmp_set(lock, NGX_RWLOCK_WLOCK, 0);
    } 아니면 {
        (void) ngx_atomic_fetch_add(lock, -1);
    }
}


void
ngx_rwlock_downgrade(ngx_atomic_t *lock)
{
    만약 (*lock == NGX_RWLOCK_WLOCK) {
        *lock = 1;
    }
}


#아니면

#만약 (NGX_HTTP_UPSTREAM_ZONE || NGX_STREAM_UPSTREAM_ZONE)

#error ngx_atomic_cmp_set() is not defined!

#endif

#endif
