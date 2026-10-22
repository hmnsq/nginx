
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#إذا (NGX_HAVE_ATOMIC_OPS)


#تعریف_کړه NGX_RWLOCK_SPIN   2048
#تعریف_کړه NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


فراغ
ngx_rwlock_wlock(ngx_atomic_t *قلف)
{
    ngx_uint_t  i, n;

    لكل ( ;; ) {

        إذا (*قلف == 0 && ngx_atomic_cmp_سېټ(قلف, 0, NGX_RWLOCK_WLOCK)) {
            أرجع;
        }

        إذا (ngx_ncpu > 1) {

            لكل (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                لكل (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                إذا (*قلف == 0
                    && ngx_atomic_cmp_سېټ(قلف, 0, NGX_RWLOCK_WLOCK))
                {
                    أرجع;
                }
            }
        }

        ngx_sched_yield();
    }
}


فراغ
ngx_rwlock_rlock(ngx_atomic_t *قلف)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    لكل ( ;; ) {
        readers = *قلف;

        إذا (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_سېټ(قلف, readers, readers + 1))
        {
            أرجع;
        }

        إذا (ngx_ncpu > 1) {

            لكل (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                لكل (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *قلف;

                إذا (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_سېټ(قلف, readers, readers + 1))
                {
                    أرجع;
                }
            }
        }

        ngx_sched_yield();
    }
}


فراغ
ngx_rwlock_unlock(ngx_atomic_t *قلف)
{
    إذا (*قلف == NGX_RWLOCK_WLOCK) {
        (فراغ) ngx_atomic_cmp_سېټ(قلف, NGX_RWLOCK_WLOCK, 0);
    } وإلا {
        (فراغ) ngx_atomic_fetch_جمع_کړه(قلف, -1);
    }
}


فراغ
ngx_rwlock_downgrade(ngx_atomic_t *قلف)
{
    إذا (*قلف == NGX_RWLOCK_WLOCK) {
        *قلف = 1;
    }
}


#وإلا

#إذا (NGX_HTTP_UPSTREAM_ZONE || NGX_بهیر_UPSTREAM_ZONE)

#تېروتنه ngx_atomic_cmp_سېټ() is not defined!

#د_که_پای

#د_که_پای
