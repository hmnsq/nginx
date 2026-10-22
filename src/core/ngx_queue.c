
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਸਥਿਰ ਖਾਲੀ ngx_ਕਤਾਰ_ਮਿਲਾਓ(ngx_ਕਤਾਰ_t *ਕਤਾਰ, ngx_ਕਤਾਰ_t *ਪੂਛ,
    ngx_int_t (*cmp)(ਅਚਲ ngx_ਕਤਾਰ_t *, ਅਚਲ ngx_ਕਤਾਰ_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_ਕਤਾਰ_t *
ngx_ਕਤਾਰ_ਵਿਚਕਾਰ(ngx_ਕਤਾਰ_t *ਕਤਾਰ)
{
    ngx_ਕਤਾਰ_t  *ਵਿਚਕਾਰ, *ਅਗਲਾ;

    ਵਿਚਕਾਰ = ngx_ਕਤਾਰ_ਸਿਰ(ਕਤਾਰ);

    ਜੇ (ਵਿਚਕਾਰ == ngx_ਕਤਾਰ_ਆਖਰੀ(ਕਤਾਰ)) {
        ਵਾਪਸ ਵਿਚਕਾਰ;
    }

    ਅਗਲਾ = ngx_ਕਤਾਰ_ਸਿਰ(ਕਤਾਰ);

    ਲਈ ( ;; ) {
        ਵਿਚਕਾਰ = ngx_ਕਤਾਰ_ਅਗਲਾ(ਵਿਚਕਾਰ);

        ਅਗਲਾ = ngx_ਕਤਾਰ_ਅਗਲਾ(ਅਗਲਾ);

        ਜੇ (ਅਗਲਾ == ngx_ਕਤਾਰ_ਆਖਰੀ(ਕਤਾਰ)) {
            ਵਾਪਸ ਵਿਚਕਾਰ;
        }

        ਅਗਲਾ = ngx_ਕਤਾਰ_ਅਗਲਾ(ਅਗਲਾ);

        ਜੇ (ਅਗਲਾ == ngx_ਕਤਾਰ_ਆਖਰੀ(ਕਤਾਰ)) {
            ਵਾਪਸ ਵਿਚਕਾਰ;
        }
    }
}


/* the stable merge sort */

ਖਾਲੀ
ngx_ਕਤਾਰ_ਛਾਂਟੋ(ngx_ਕਤਾਰ_t *ਕਤਾਰ,
    ngx_int_t (*cmp)(ਅਚਲ ngx_ਕਤਾਰ_t *, ਅਚਲ ngx_ਕਤਾਰ_t *))
{
    ngx_ਕਤਾਰ_t  *q, ਪੂਛ;

    q = ngx_ਕਤਾਰ_ਸਿਰ(ਕਤਾਰ);

    ਜੇ (q == ngx_ਕਤਾਰ_ਆਖਰੀ(ਕਤਾਰ)) {
        ਵਾਪਸ;
    }

    q = ngx_ਕਤਾਰ_ਵਿਚਕਾਰ(ਕਤਾਰ);

    ngx_ਕਤਾਰ_ਵੰਡੋ(ਕਤਾਰ, q, &ਪੂਛ);

    ngx_ਕਤਾਰ_ਛਾਂਟੋ(ਕਤਾਰ, cmp);
    ngx_ਕਤਾਰ_ਛਾਂਟੋ(&ਪੂਛ, cmp);

    ngx_ਕਤਾਰ_ਮਿਲਾਓ(ਕਤਾਰ, &ਪੂਛ, cmp);
}


ਸਥਿਰ ਖਾਲੀ
ngx_ਕਤਾਰ_ਮਿਲਾਓ(ngx_ਕਤਾਰ_t *ਕਤਾਰ, ngx_ਕਤਾਰ_t *ਪੂਛ,
    ngx_int_t (*cmp)(ਅਚਲ ngx_ਕਤਾਰ_t *, ਅਚਲ ngx_ਕਤਾਰ_t *))
{
    ngx_ਕਤਾਰ_t  *q1, *q2;

    q1 = ngx_ਕਤਾਰ_ਸਿਰ(ਕਤਾਰ);
    q2 = ngx_ਕਤਾਰ_ਸਿਰ(ਪੂਛ);

    ਲਈ ( ;; ) {
        ਜੇ (q1 == ngx_ਕਤਾਰ_sentinel(ਕਤਾਰ)) {
            ngx_ਕਤਾਰ_ਜੋੜੋ(ਕਤਾਰ, ਪੂਛ);
            ਰੋਕੋ;
        }

        ਜੇ (q2 == ngx_ਕਤਾਰ_sentinel(ਪੂਛ)) {
            ਰੋਕੋ;
        }

        ਜੇ (cmp(q1, q2) <= 0) {
            q1 = ngx_ਕਤਾਰ_ਅਗਲਾ(q1);
            ਜਾਰੀ;
        }

        ngx_ਕਤਾਰ_remove(q2);
        ngx_ਕਤਾਰ_ਪਾਓ_before(q1, q2);

        q2 = ngx_ਕਤਾਰ_ਸਿਰ(ਪੂਛ);
    }
}
