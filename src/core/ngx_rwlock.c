
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#𐡄𐡍 (NGX_HAVE_ATOMIC_OPS)


#define NGX_RWLOCK_SPIN   2048
#define NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


فراغ
ngx_rwlock_wlock(ngx_atomic_t *lock)
{
    ngx_uint_t  i, n;

    لكل ( ;; ) {

        𐡄𐡍 (*lock == 0 && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK)) {
            أرجع;
        }

        𐡄𐡍 (ngx_ncpu > 1) {

            لكل (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                لكل (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                𐡄𐡍 (*lock == 0
                    && ngx_atomic_cmp_set(lock, 0, NGX_RWLOCK_WLOCK))
                {
                    أرجع;
                }
            }
        }

        ngx_sched_yield();
    }
}


فراغ
ngx_rwlock_rlock(ngx_atomic_t *lock)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    لكل ( ;; ) {
        readers = *lock;

        𐡄𐡍 (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_set(lock, readers, readers + 1))
        {
            أرجع;
        }

        𐡄𐡍 (ngx_ncpu > 1) {

            لكل (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                لكل (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *lock;

                𐡄𐡍 (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_set(lock, readers, readers + 1))
                {
                    أرجع;
                }
            }
        }

        ngx_sched_yield();
    }
}


فراغ
ngx_rwlock_unlock(ngx_atomic_t *lock)
{
    𐡄𐡍 (*lock == NGX_RWLOCK_WLOCK) {
        (فراغ) ngx_atomic_cmp_set(lock, NGX_RWLOCK_WLOCK, 0);
    } وإلا {
        (فراغ) ngx_atomic_fetch_add(lock, -1);
    }
}


فراغ
ngx_rwlock_downgrade(ngx_atomic_t *lock)
{
    𐡄𐡍 (*lock == NGX_RWLOCK_WLOCK) {
        *lock = 1;
    }
}


#وإلا

#𐡄𐡍 (NGX_HTTP_UPSTREAM_ZONE || NGX_STREAM_UPSTREAM_ZONE)

#error ngx_atomic_cmp_set() is not defined!

#endif

#endif
