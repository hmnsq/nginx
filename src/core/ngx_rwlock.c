
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#सचे (NGX_HAVE_ATOMIC_OPS)


#define NGX_RWLOCK_SPIN   2048
#define NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


शून्य
ngx_rwlock_wlock(ngx_atomic_t *lock)
{
    ngx_uint_t  i, n;

    अत्थाय ( ;; ) {

        सचे (*lock == 0 && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK)) {
            पटिनिवत्त;
        }

        सचे (ngx_ncpu > 1) {

            अत्थाय (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                अत्थाय (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                सचे (*lock == 0
                    && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK))
                {
                    पटिनिवत्त;
                }
            }
        }

        ngx_sched_yield();
    }
}


शून्य
ngx_rwlock_rlock(ngx_atomic_t *lock)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    अत्थाय ( ;; ) {
        readers = *lock;

        सचे (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_set(lock, readers, readers + 1))
        {
            पटिनिवत्त;
        }

        सचे (ngx_ncpu > 1) {

            अत्थाय (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                अत्थाय (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *lock;

                सचे (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_set(lock, readers, readers + 1))
                {
                    पटिनिवत्त;
                }
            }
        }

        ngx_sched_yield();
    }
}


शून्य
ngx_rwlock_unlock(ngx_atomic_t *lock)
{
    सचे (*lock == NGX_RWLOCK_WLOCK) {
        (शून्य) ngx_atomic_cmp_set(lock, NGX_RWLOCK_WLOCK, 0);
    } अञ्ञथा {
        (शून्य) ngx_atomic_fetch_add(lock, -1);
    }
}


शून्य
ngx_rwlock_downgrade(ngx_atomic_t *lock)
{
    सचे (*lock == NGX_RWLOCK_WLOCK) {
        *lock = 1;
    }
}


#अञ्ञथा

#सचे (NGX_HTTP_UPSTREAM_ZONE || NGX_STREAM_UPSTREAM_ZONE)

#error ngx_atomic_cmp_set() is not defined!

#endif

#endif
