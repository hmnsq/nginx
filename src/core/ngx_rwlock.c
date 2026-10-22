
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#གལ་སྲིད (NGX_HAVE_ATOMIC_OPS)


#define NGX_RWLOCK_SPIN   2048
#define NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


void
ngx_rwlock_wlock(ngx_atomic_t *lock)
{
    ngx_uint_t  i, n;

    དོན་ལུ ( ;; ) {

        གལ་སྲིད (*lock == 0 && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK)) {
            ལོག;
        }

        གལ་སྲིད (ngx_ncpu > 1) {

            དོན་ལུ (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                དོན་ལུ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                གལ་སྲིད (*lock == 0
                    && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK))
                {
                    ལོག;
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

    དོན་ལུ ( ;; ) {
        readers = *lock;

        གལ་སྲིད (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_set(lock, readers, readers + 1))
        {
            ལོག;
        }

        གལ་སྲིད (ngx_ncpu > 1) {

            དོན་ལུ (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                དོན་ལུ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *lock;

                གལ་སྲིད (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_set(lock, readers, readers + 1))
                {
                    ལོག;
                }
            }
        }

        ngx_sched_yield();
    }
}


void
ngx_rwlock_unlock(ngx_atomic_t *lock)
{
    གལ་སྲིད (*lock == NGX_RWLOCK_WLOCK) {
        (void) ngx_atomic_cmp_set(lock, NGX_RWLOCK_WLOCK, 0);
    } དེ་མིན {
        (void) ngx_atomic_fetch_add(lock, -1);
    }
}


void
ngx_rwlock_downgrade(ngx_atomic_t *lock)
{
    གལ་སྲིད (*lock == NGX_RWLOCK_WLOCK) {
        *lock = 1;
    }
}


#དེ་མིན

#གལ་སྲིད (NGX_HTTP_UPSTREAM_ZONE || NGX_STREAM_UPSTREAM_ZONE)

#error ngx_atomic_cmp_set() is not defined!

#endif

#endif
