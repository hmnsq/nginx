
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ಆದರೆ (NGX_HAVE_ATOMIC_OPS)


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RWLOCK_SPIN   2048
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


ಶೂನ್ಯ
ngx_rwlock_wlock(ngx_atomic_t *ಬೀಗ)
{
    ngx_uint_t  i, n;

    ಗಾಗಿ ( ;; ) {

        ಆದರೆ (*ಬೀಗ == 0 && ngx_atomic_cmp_ಗಣ(ಬೀಗ, 0, NGX_RWLOCK_WLOCK)) {
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ (ngx_ncpu > 1) {

            ಗಾಗಿ (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                ಗಾಗಿ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                ಆದರೆ (*ಬೀಗ == 0
                    && ngx_atomic_cmp_ಗಣ(ಬೀಗ, 0, NGX_RWLOCK_WLOCK))
                {
                    ಹಿಂತಿರುಗಿ;
                }
            }
        }

        ngx_sched_yield();
    }
}


ಶೂನ್ಯ
ngx_rwlock_rlock(ngx_atomic_t *ಬೀಗ)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    ಗಾಗಿ ( ;; ) {
        readers = *ಬೀಗ;

        ಆದರೆ (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_ಗಣ(ಬೀಗ, readers, readers + 1))
        {
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ (ngx_ncpu > 1) {

            ಗಾಗಿ (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                ಗಾಗಿ (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *ಬೀಗ;

                ಆದರೆ (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_ಗಣ(ಬೀಗ, readers, readers + 1))
                {
                    ಹಿಂತಿರುಗಿ;
                }
            }
        }

        ngx_sched_yield();
    }
}


ಶೂನ್ಯ
ngx_rwlock_unlock(ngx_atomic_t *ಬೀಗ)
{
    ಆದರೆ (*ಬೀಗ == NGX_RWLOCK_WLOCK) {
        (ಶೂನ್ಯ) ngx_atomic_cmp_ಗಣ(ಬೀಗ, NGX_RWLOCK_WLOCK, 0);
    } ಇಲ್ಲದಿದ್ದರೆ {
        (ಶೂನ್ಯ) ngx_atomic_fetch_ಕೂಡಿಸು(ಬೀಗ, -1);
    }
}


ಶೂನ್ಯ
ngx_rwlock_downgrade(ngx_atomic_t *ಬೀಗ)
{
    ಆದರೆ (*ಬೀಗ == NGX_RWLOCK_WLOCK) {
        *ಬೀಗ = 1;
    }
}


#ಇಲ್ಲದಿದ್ದರೆ

#ಆದರೆ (NGX_HTTP_UPSTREAM_ZONE || NGX_ಹರಿವು_UPSTREAM_ZONE)

#ದೋಷ ngx_atomic_cmp_ಗಣ() is not defined!

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
