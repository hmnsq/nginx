
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#ꦪꦼꦤ꧀ (NGX_HAVE_ATOMIC_OPS)


#define NGX_RWLOCK_SPIN   2048
#define NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


void
ngx_rwlock_wlock(ngx_atomic_t *lock)
{
    ngx_uint_t  i, n;

    ꦏꦁꦒꦺꦴ ( ;; ) {

        ꦪꦼꦤ꧀ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK)) {
            ꦮꦭꦶ;
        }

        ꦪꦼꦤ꧀ (ngx_ncpu > 1) {

            ꦏꦁꦒꦺꦴ (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                ꦏꦁꦒꦺꦴ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ꦪꦼꦤ꧀ (*lock == 0
                    && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK))
                {
                    ꦮꦭꦶ;
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

    ꦏꦁꦒꦺꦴ ( ;; ) {
        readers = *lock;

        ꦪꦼꦤ꧀ (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_set(lock, readers, readers + 1))
        {
            ꦮꦭꦶ;
        }

        ꦪꦼꦤ꧀ (ngx_ncpu > 1) {

            ꦏꦁꦒꦺꦴ (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                ꦏꦁꦒꦺꦴ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *lock;

                ꦪꦼꦤ꧀ (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_set(lock, readers, readers + 1))
                {
                    ꦮꦭꦶ;
                }
            }
        }

        ngx_sched_yield();
    }
}


void
ngx_rwlock_unlock(ngx_atomic_t *lock)
{
    ꦪꦼꦤ꧀ (*lock == NGX_RWLOCK_WLOCK) {
        (void) ngx_atomic_cmp_set(lock, NGX_RWLOCK_WLOCK, 0);
    } ꦭꦶꦪ {
        (void) ngx_atomic_fetch_add(lock, -1);
    }
}


void
ngx_rwlock_downgrade(ngx_atomic_t *lock)
{
    ꦪꦼꦤ꧀ (*lock == NGX_RWLOCK_WLOCK) {
        *lock = 1;
    }
}


#ꦭꦶꦪ

#ꦪꦼꦤ꧀ (NGX_HTTP_UPSTREAM_ZONE || NGX_STREAM_UPSTREAM_ZONE)

#error ngx_atomic_cmp_set() is not defined!

#endif

#endif
