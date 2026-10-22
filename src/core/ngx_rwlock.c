
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


#ਜੇ (NGX_HAVE_ATOMIC_OPS)


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RWLOCK_SPIN   2048
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


ਖਾਲੀ
ngx_rwlock_wlock(ngx_atomic_t *ਤਾਲਾ)
{
    ngx_uint_t  i, n;

    ਲਈ ( ;; ) {

        ਜੇ (*ਤਾਲਾ == 0 && ngx_atomic_cmp_ਸੈੱਟ(ਤਾਲਾ, 0, NGX_RWLOCK_WLOCK)) {
            ਵਾਪਸ;
        }

        ਜੇ (ngx_ncpu > 1) {

            ਲਈ (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                ਲਈ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ਜੇ (*ਤਾਲਾ == 0
                    && ngx_atomic_cmp_ਸੈੱਟ(ਤਾਲਾ, 0, NGX_RWLOCK_WLOCK))
                {
                    ਵਾਪਸ;
                }
            }
        }

        ngx_sched_yield();
    }
}


ਖਾਲੀ
ngx_rwlock_rlock(ngx_atomic_t *ਤਾਲਾ)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    ਲਈ ( ;; ) {
        readers = *ਤਾਲਾ;

        ਜੇ (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_ਸੈੱਟ(ਤਾਲਾ, readers, readers + 1))
        {
            ਵਾਪਸ;
        }

        ਜੇ (ngx_ncpu > 1) {

            ਲਈ (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                ਲਈ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *ਤਾਲਾ;

                ਜੇ (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_ਸੈੱਟ(ਤਾਲਾ, readers, readers + 1))
                {
                    ਵਾਪਸ;
                }
            }
        }

        ngx_sched_yield();
    }
}


ਖਾਲੀ
ngx_rwlock_unlock(ngx_atomic_t *ਤਾਲਾ)
{
    ਜੇ (*ਤਾਲਾ == NGX_RWLOCK_WLOCK) {
        (ਖਾਲੀ) ngx_atomic_cmp_ਸੈੱਟ(ਤਾਲਾ, NGX_RWLOCK_WLOCK, 0);
    } ਨਹੀਂਤਾਂ {
        (ਖਾਲੀ) ngx_atomic_fetch_ਜੋੜੋ(ਤਾਲਾ, -1);
    }
}


ਖਾਲੀ
ngx_rwlock_downgrade(ngx_atomic_t *ਤਾਲਾ)
{
    ਜੇ (*ਤਾਲਾ == NGX_RWLOCK_WLOCK) {
        *ਤਾਲਾ = 1;
    }
}


#ਨਹੀਂਤਾਂ

#ਜੇ (NGX_HTTP_UPSTREAM_ZONE || NGX_ਧਾਰਾ_UPSTREAM_ZONE)

#ਗਲਤੀ ngx_atomic_cmp_ਸੈੱਟ() is not defined!

#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
