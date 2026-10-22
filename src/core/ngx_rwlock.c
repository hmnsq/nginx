
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


#अगर (NGX_HAVE_ATOMIC_OPS)


#घोषणा NGX_RWLOCK_SPIN   2048
#घोषणा NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


व्योम
ngx_rwlock_wlock(ngx_atomic_t *lock)
{
    ngx_uint_t  i, n;

    क्रम ( ;; ) {

        अगर (*lock == 0 && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK)) {
            वापस;
        }

        अगर (ngx_ncpu > 1) {

            क्रम (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                क्रम (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                अगर (*lock == 0
                    && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK))
                {
                    वापस;
                }
            }
        }

        ngx_sched_yield();
    }
}


व्योम
ngx_rwlock_rlock(ngx_atomic_t *lock)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    क्रम ( ;; ) {
        readers = *lock;

        अगर (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_set(lock, readers, readers + 1))
        {
            वापस;
        }

        अगर (ngx_ncpu > 1) {

            क्रम (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                क्रम (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *lock;

                अगर (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_set(lock, readers, readers + 1))
                {
                    वापस;
                }
            }
        }

        ngx_sched_yield();
    }
}


व्योम
ngx_rwlock_unlock(ngx_atomic_t *lock)
{
    अगर (*lock == NGX_RWLOCK_WLOCK) {
        (व्योम) ngx_atomic_cmp_set(lock, NGX_RWLOCK_WLOCK, 0);
    } अन्यथा {
        (व्योम) ngx_atomic_fetch_add(lock, -1);
    }
}


व्योम
ngx_rwlock_downgrade(ngx_atomic_t *lock)
{
    अगर (*lock == NGX_RWLOCK_WLOCK) {
        *lock = 1;
    }
}


#अन्यथा

#अगर (NGX_HTTP_UPSTREAM_ZONE || NGX_STREAM_UPSTREAM_ZONE)

#त्रुटि ngx_atomic_cmp_set() is not defined!

#पूर्ण_अगर

#पूर्ण_अगर
