
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


#ଯଦି (NGX_HAVE_ATOMIC_OPS)


#define NGX_RWLOCK_SPIN   2048
#define NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


ଶୂନ୍ୟ
ngx_rwlock_wlock(ngx_atomic_t *lock)
{
    ngx_uint_t  i, n;

    ପାଇଁ ( ;; ) {

        ଯଦି (*lock == 0 && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK)) {
            ଫେରନ୍ତୁ;
        }

        ଯଦି (ngx_ncpu > 1) {

            ପାଇଁ (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                ପାଇଁ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ଯଦି (*lock == 0
                    && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK))
                {
                    ଫେରନ୍ତୁ;
                }
            }
        }

        ngx_sched_yield();
    }
}


ଶୂନ୍ୟ
ngx_rwlock_rlock(ngx_atomic_t *lock)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    ପାଇଁ ( ;; ) {
        readers = *lock;

        ଯଦି (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_set(lock, readers, readers + 1))
        {
            ଫେରନ୍ତୁ;
        }

        ଯଦି (ngx_ncpu > 1) {

            ପାଇଁ (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                ପାଇଁ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *lock;

                ଯଦି (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_set(lock, readers, readers + 1))
                {
                    ଫେରନ୍ତୁ;
                }
            }
        }

        ngx_sched_yield();
    }
}


ଶୂନ୍ୟ
ngx_rwlock_unlock(ngx_atomic_t *lock)
{
    ଯଦି (*lock == NGX_RWLOCK_WLOCK) {
        (ଶୂନ୍ୟ) ngx_atomic_cmp_set(lock, NGX_RWLOCK_WLOCK, 0);
    } ଅନ୍ୟଥା {
        (ଶୂନ୍ୟ) ngx_atomic_fetch_add(lock, -1);
    }
}


ଶୂନ୍ୟ
ngx_rwlock_downgrade(ngx_atomic_t *lock)
{
    ଯଦି (*lock == NGX_RWLOCK_WLOCK) {
        *lock = 1;
    }
}


#ଅନ୍ୟଥା

#ଯଦି (NGX_HTTP_UPSTREAM_ZONE || NGX_STREAM_UPSTREAM_ZONE)

#error ngx_atomic_cmp_set() is not defined!

#endif

#endif
