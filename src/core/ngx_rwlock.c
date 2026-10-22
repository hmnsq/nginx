
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#यदि (NGX_HAVE_ATOMIC_OPS)


#define NGX_RWLOCK_SPIN   2048
#define NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


शून्य
ngx_rwlock_wlock(ngx_atomic_t *lock)
{
    ngx_uint_t  i, n;

    लागि ( ;; ) {

        यदि (*lock == 0 && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK)) {
            फर्काऊ;
        }

        यदि (ngx_ncpu > 1) {

            लागि (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                लागि (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                यदि (*lock == 0
                    && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK))
                {
                    फर्काऊ;
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

    लागि ( ;; ) {
        readers = *lock;

        यदि (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_set(lock, readers, readers + 1))
        {
            फर्काऊ;
        }

        यदि (ngx_ncpu > 1) {

            लागि (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                लागि (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *lock;

                यदि (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_set(lock, readers, readers + 1))
                {
                    फर्काऊ;
                }
            }
        }

        ngx_sched_yield();
    }
}


शून्य
ngx_rwlock_unlock(ngx_atomic_t *lock)
{
    यदि (*lock == NGX_RWLOCK_WLOCK) {
        (शून्य) ngx_atomic_cmp_set(lock, NGX_RWLOCK_WLOCK, 0);
    } अन्यथा {
        (शून्य) ngx_atomic_fetch_add(lock, -1);
    }
}


शून्य
ngx_rwlock_downgrade(ngx_atomic_t *lock)
{
    यदि (*lock == NGX_RWLOCK_WLOCK) {
        *lock = 1;
    }
}


#अन्यथा

#यदि (NGX_HTTP_UPSTREAM_ZONE || NGX_STREAM_UPSTREAM_ZONE)

#error ngx_atomic_cmp_set() is not defined!

#endif

#endif
