
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#यदि (NGX_HAVE_ATOMIC_OPS)


#परिभाषय NGX_RWLOCK_SPIN   2048
#परिभाषय NGX_RWLOCK_WLOCK  ((ngx_atomic_uint_t) -1)


शून्य
ngx_rwlock_wlock(ngx_atomic_t *तालकम्)
{
    ngx_uint_t  i, n;

    प्रति ( ;; ) {

        यदि (*तालकम् == 0 && ngx_atomic_cmp_समुच्चयः(तालकम्, 0, NGX_RWLOCK_WLOCK)) {
            प्रत्यागम;
        }

        यदि (ngx_ncpu > 1) {

            प्रति (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                प्रति (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                यदि (*तालकम् == 0
                    && ngx_atomic_cmp_समुच्चयः(तालकम्, 0, NGX_RWLOCK_WLOCK))
                {
                    प्रत्यागम;
                }
            }
        }

        ngx_sched_yield();
    }
}


शून्य
ngx_rwlock_rlock(ngx_atomic_t *तालकम्)
{
    ngx_uint_t         i, n;
    ngx_atomic_uint_t  readers;

    प्रति ( ;; ) {
        readers = *तालकम्;

        यदि (readers != NGX_RWLOCK_WLOCK
            && ngx_atomic_cmp_समुच्चयः(तालकम्, readers, readers + 1))
        {
            प्रत्यागम;
        }

        यदि (ngx_ncpu > 1) {

            प्रति (n = 1; n < NGX_RWLOCK_SPIN; n <<= 1) {

                प्रति (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                readers = *तालकम्;

                यदि (readers != NGX_RWLOCK_WLOCK
                    && ngx_atomic_cmp_समुच्चयः(तालकम्, readers, readers + 1))
                {
                    प्रत्यागम;
                }
            }
        }

        ngx_sched_yield();
    }
}


शून्य
ngx_rwlock_unlock(ngx_atomic_t *तालकम्)
{
    यदि (*तालकम् == NGX_RWLOCK_WLOCK) {
        (शून्य) ngx_atomic_cmp_समुच्चयः(तालकम्, NGX_RWLOCK_WLOCK, 0);
    } अन्यथा {
        (शून्य) ngx_atomic_fetch_सङ्कलय(तालकम्, -1);
    }
}


शून्य
ngx_rwlock_downgrade(ngx_atomic_t *तालकम्)
{
    यदि (*तालकम् == NGX_RWLOCK_WLOCK) {
        *तालकम् = 1;
    }
}


#अन्यथा

#यदि (NGX_HTTP_UPSTREAM_ZONE || NGX_प्रवाहः_UPSTREAM_ZONE)

#दोषः ngx_atomic_cmp_समुच्चयः() is not defined!

#यदि_अन्तः

#यदि_अन्तः
