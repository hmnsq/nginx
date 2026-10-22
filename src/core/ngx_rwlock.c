
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


#எனில் (NGX_HAVE_ATOMIC_OPS)


#define NGX_RWLOCK_SPIN   2048
#define NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


வெறுமை
ngx_rwlock_wlock(ngx_atomic_t *lock)
{
    ngx_uint_t  i, n;

    க்கு ( ;; ) {

        எனில் (*lock == 0 && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK)) {
            திரும்பு;
        }

        எனில் (ngx_ncpu > 1) {

            க்கு (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                க்கு (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                எனில் (*lock == 0
                    && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK))
                {
                    திரும்பு;
                }
            }
        }

        ngx_sched_yield();
    }
}


வெறுமை
ngx_rwlock_rlock(ngx_atomic_t *lock)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    க்கு ( ;; ) {
        readers = *lock;

        எனில் (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_set(lock, readers, readers + 1))
        {
            திரும்பு;
        }

        எனில் (ngx_ncpu > 1) {

            க்கு (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                க்கு (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *lock;

                எனில் (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_set(lock, readers, readers + 1))
                {
                    திரும்பு;
                }
            }
        }

        ngx_sched_yield();
    }
}


வெறுமை
ngx_rwlock_unlock(ngx_atomic_t *lock)
{
    எனில் (*lock == NGX_RWLOCK_WLOCK) {
        (வெறுமை) ngx_atomic_cmp_set(lock, NGX_RWLOCK_WLOCK, 0);
    } இல்லையெனில் {
        (வெறுமை) ngx_atomic_fetch_add(lock, -1);
    }
}


வெறுமை
ngx_rwlock_downgrade(ngx_atomic_t *lock)
{
    எனில் (*lock == NGX_RWLOCK_WLOCK) {
        *lock = 1;
    }
}


#இல்லையெனில்

#எனில் (NGX_HTTP_UPSTREAM_ZONE || NGX_STREAM_UPSTREAM_ZONE)

#error ngx_atomic_cmp_set() is not defined!

#endif

#endif
