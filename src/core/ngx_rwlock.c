
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


#എങ്കിൽ (NGX_HAVE_ATOMIC_OPS)


#define NGX_RWLOCK_SPIN   2048
#define NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


ശൂന്യം
ngx_rwlock_wlock(ngx_atomic_t *lock)
{
    ngx_uint_t  i, n;

    വേണ്ടി ( ;; ) {

        എങ്കിൽ (*lock == 0 && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK)) {
            തിരികെ;
        }

        എങ്കിൽ (ngx_ncpu > 1) {

            വേണ്ടി (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                വേണ്ടി (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                എങ്കിൽ (*lock == 0
                    && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK))
                {
                    തിരികെ;
                }
            }
        }

        ngx_sched_yield();
    }
}


ശൂന്യം
ngx_rwlock_rlock(ngx_atomic_t *lock)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    വേണ്ടി ( ;; ) {
        readers = *lock;

        എങ്കിൽ (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_set(lock, readers, readers + 1))
        {
            തിരികെ;
        }

        എങ്കിൽ (ngx_ncpu > 1) {

            വേണ്ടി (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                വേണ്ടി (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *lock;

                എങ്കിൽ (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_set(lock, readers, readers + 1))
                {
                    തിരികെ;
                }
            }
        }

        ngx_sched_yield();
    }
}


ശൂന്യം
ngx_rwlock_unlock(ngx_atomic_t *lock)
{
    എങ്കിൽ (*lock == NGX_RWLOCK_WLOCK) {
        (ശൂന്യം) ngx_atomic_cmp_set(lock, NGX_RWLOCK_WLOCK, 0);
    } അല്ലെങ്കിൽ {
        (ശൂന്യം) ngx_atomic_fetch_add(lock, -1);
    }
}


ശൂന്യം
ngx_rwlock_downgrade(ngx_atomic_t *lock)
{
    എങ്കിൽ (*lock == NGX_RWLOCK_WLOCK) {
        *lock = 1;
    }
}


#അല്ലെങ്കിൽ

#എങ്കിൽ (NGX_HTTP_UPSTREAM_ZONE || NGX_STREAM_UPSTREAM_ZONE)

#error ngx_atomic_cmp_set() is not defined!

#endif

#endif
