
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#إذا (NGX_HAVE_ATOMIC_OPS)


#وصف_ڪريو NGX_RWLOCK_SPIN   2048
#وصف_ڪريو NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


فراغ
ngx_rwlock_wlock(ngx_atomic_t *تالو)
{
    ngx_uint_t  i, n;

    لكل ( ;; ) {

        إذا (*تالو == 0 && ngx_atomic_cmp_سيٽ(تالو, 0, NGX_RWLOCK_WLOCK)) {
            أرجع;
        }

        إذا (ngx_ncpu > 1) {

            لكل (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                لكل (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                إذا (*تالو == 0
                    && ngx_atomic_cmp_سيٽ(تالو, 0, NGX_RWLOCK_WLOCK))
                {
                    أرجع;
                }
            }
        }

        ngx_sched_yield();
    }
}


فراغ
ngx_rwlock_rlock(ngx_atomic_t *تالو)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    لكل ( ;; ) {
        readers = *تالو;

        إذا (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_سيٽ(تالو, readers, readers + 1))
        {
            أرجع;
        }

        إذا (ngx_ncpu > 1) {

            لكل (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                لكل (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *تالو;

                إذا (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_سيٽ(تالو, readers, readers + 1))
                {
                    أرجع;
                }
            }
        }

        ngx_sched_yield();
    }
}


فراغ
ngx_rwlock_unlock(ngx_atomic_t *تالو)
{
    إذا (*تالو == NGX_RWLOCK_WLOCK) {
        (فراغ) ngx_atomic_cmp_سيٽ(تالو, NGX_RWLOCK_WLOCK, 0);
    } وإلا {
        (فراغ) ngx_atomic_fetch_جوڙيو(تالو, -1);
    }
}


فراغ
ngx_rwlock_downgrade(ngx_atomic_t *تالو)
{
    إذا (*تالو == NGX_RWLOCK_WLOCK) {
        *تالو = 1;
    }
}


#وإلا

#إذا (NGX_HTTP_UPSTREAM_ZONE || NGX_وهڪرو_UPSTREAM_ZONE)

#غلطي ngx_atomic_cmp_سيٽ() is not defined!

#جيڪڏهن_پورو

#جيڪڏهن_پورو
