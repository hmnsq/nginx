
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


#যদি (NGX_HAVE_ATOMIC_OPS)


#define NGX_RWLOCK_SPIN   2048
#define NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


শূন্য
ngx_rwlock_wlock(ngx_atomic_t *lock)
{
    ngx_uint_t  i, n;

    প্রতি ( ;; ) {

        যদি (*lock == 0 && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK)) {
            প্রত্যাগম;
        }

        যদি (ngx_ncpu > 1) {

            প্রতি (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                প্রতি (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                যদি (*lock == 0
                    && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK))
                {
                    প্রত্যাগম;
                }
            }
        }

        ngx_sched_yield();
    }
}


শূন্য
ngx_rwlock_rlock(ngx_atomic_t *lock)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    প্রতি ( ;; ) {
        readers = *lock;

        যদি (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_set(lock, readers, readers + 1))
        {
            প্রত্যাগম;
        }

        যদি (ngx_ncpu > 1) {

            প্রতি (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                প্রতি (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *lock;

                যদি (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_set(lock, readers, readers + 1))
                {
                    প্রত্যাগম;
                }
            }
        }

        ngx_sched_yield();
    }
}


শূন্য
ngx_rwlock_unlock(ngx_atomic_t *lock)
{
    যদি (*lock == NGX_RWLOCK_WLOCK) {
        (শূন্য) ngx_atomic_cmp_set(lock, NGX_RWLOCK_WLOCK, 0);
    } অন্যথা {
        (শূন্য) ngx_atomic_fetch_add(lock, -1);
    }
}


শূন্য
ngx_rwlock_downgrade(ngx_atomic_t *lock)
{
    যদি (*lock == NGX_RWLOCK_WLOCK) {
        *lock = 1;
    }
}


#অন্যথা

#যদি (NGX_HTTP_UPSTREAM_ZONE || NGX_STREAM_UPSTREAM_ZONE)

#error ngx_atomic_cmp_set() is not defined!

#endif

#endif
