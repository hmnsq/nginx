
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ಯದಿ (NGX_HAVE_ATOMIC_OPS)


#define NGX_RWLOCK_SPIN   2048
#define NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


ಶೂನ್ಯ
ngx_rwlock_wlock(ngx_atomic_t *lock)
{
    ngx_uint_t  i, n;

    ಪ್ರತಿ ( ;; ) {

        ಯದಿ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK)) {
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಯದಿ (ngx_ncpu > 1) {

            ಪ್ರತಿ (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                ಪ್ರತಿ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ಯದಿ (*lock == 0
                    && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK))
                {
                    ಪ್ರತ್ಯಾಗಮ;
                }
            }
        }

        ngx_sched_yield();
    }
}


ಶೂನ್ಯ
ngx_rwlock_rlock(ngx_atomic_t *lock)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    ಪ್ರತಿ ( ;; ) {
        readers = *lock;

        ಯದಿ (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_set(lock, readers, readers + 1))
        {
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಯದಿ (ngx_ncpu > 1) {

            ಪ್ರತಿ (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                ಪ್ರತಿ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *lock;

                ಯದಿ (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_set(lock, readers, readers + 1))
                {
                    ಪ್ರತ್ಯಾಗಮ;
                }
            }
        }

        ngx_sched_yield();
    }
}


ಶೂನ್ಯ
ngx_rwlock_unlock(ngx_atomic_t *lock)
{
    ಯದಿ (*lock == NGX_RWLOCK_WLOCK) {
        (ಶೂನ್ಯ) ngx_atomic_cmp_set(lock, NGX_RWLOCK_WLOCK, 0);
    } ಅನ್ಯಥಾ {
        (ಶೂನ್ಯ) ngx_atomic_fetch_add(lock, -1);
    }
}


ಶೂನ್ಯ
ngx_rwlock_downgrade(ngx_atomic_t *lock)
{
    ಯದಿ (*lock == NGX_RWLOCK_WLOCK) {
        *lock = 1;
    }
}


#ಅನ್ಯಥಾ

#ಯದಿ (NGX_HTTP_UPSTREAM_ZONE || NGX_STREAM_UPSTREAM_ZONE)

#error ngx_atomic_cmp_set() is not defined!

#endif

#endif
