
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


#اگر (NGX_HAVE_ATOMIC_OPS)


#تعریف_کرو NGX_RWLOCK_SPIN   2048
#تعریف_کرو NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


خالی
ngx_rwlock_wlock(ngx_atomic_t *تالا)
{
    ngx_uint_t  i, n;

    برائے ( ;; ) {

        اگر (*تالا == 0 && ngx_atomic_cmp_سیٹ(تالا, 0, NGX_RWLOCK_WLOCK)) {
            واپس;
        }

        اگر (ngx_ncpu > 1) {

            برائے (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                برائے (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                اگر (*تالا == 0
                    && ngx_atomic_cmp_سیٹ(تالا, 0, NGX_RWLOCK_WLOCK))
                {
                    واپس;
                }
            }
        }

        ngx_sched_yield();
    }
}


خالی
ngx_rwlock_rlock(ngx_atomic_t *تالا)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    برائے ( ;; ) {
        readers = *تالا;

        اگر (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_سیٹ(تالا, readers, readers + 1))
        {
            واپس;
        }

        اگر (ngx_ncpu > 1) {

            برائے (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                برائے (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *تالا;

                اگر (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_سیٹ(تالا, readers, readers + 1))
                {
                    واپس;
                }
            }
        }

        ngx_sched_yield();
    }
}


خالی
ngx_rwlock_unlock(ngx_atomic_t *تالا)
{
    اگر (*تالا == NGX_RWLOCK_WLOCK) {
        (خالی) ngx_atomic_cmp_سیٹ(تالا, NGX_RWLOCK_WLOCK, 0);
    } ورنہ {
        (خالی) ngx_atomic_fetch_جوڑو(تالا, -1);
    }
}


خالی
ngx_rwlock_downgrade(ngx_atomic_t *تالا)
{
    اگر (*تالا == NGX_RWLOCK_WLOCK) {
        *تالا = 1;
    }
}


#ورنہ

#اگر (NGX_HTTP_UPSTREAM_ZONE || NGX_دھارا_UPSTREAM_ZONE)

#غلطی ngx_atomic_cmp_سیٹ() is not defined!

#اگر_ختم

#اگر_ختم
