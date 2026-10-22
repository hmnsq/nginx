
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, ਮਾਪ_t ਮਾਪ)
{
    ngx_array_t *a;

    a = ngx_palloc(p, ਆਕਾਰ(ngx_array_t));
    ਜੇ (a == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜੇ (ngx_array_init(a, p, n, ਮਾਪ) != NGX_OK) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ a;
}


ਖਾਲੀ
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    ਜੇ ((u_char *) a->elts + a->ਮਾਪ * a->nalloc == p->d.ਆਖਰੀ) {
        p->d.ਆਖਰੀ -= a->ਮਾਪ * a->nalloc;
    }

    ਜੇ ((u_char *) a + ਆਕਾਰ(ngx_array_t) == p->d.ਆਖਰੀ) {
        p->d.ਆਖਰੀ = (u_char *) a;
    }
}


ਖਾਲੀ *
ngx_array_ਧੱਕੋ(ngx_array_t *a)
{
    ਖਾਲੀ        *elt, *new;
    ਮਾਪ_t       ਮਾਪ;
    ngx_pool_t  *p;

    ਜੇ (a->nelts == a->nalloc) {

        /* the array is full */

        ਮਾਪ = a->ਮਾਪ * a->nalloc;

        p = a->pool;

        ਜੇ ((u_char *) a->elts + ਮਾਪ == p->d.ਆਖਰੀ
            && p->d.ਆਖਰੀ + a->ਮਾਪ <= p->d.ਸਿਰਾ)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.ਆਖਰੀ += a->ਮਾਪ;
            a->nalloc++;

        } ਨਹੀਂਤਾਂ {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * ਮਾਪ);
            ਜੇ (new == ਨਿਰਾ) {
                ਵਾਪਸ ਨਿਰਾ;
            }

            ngx_memcpy(new, a->elts, ਮਾਪ);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->ਮਾਪ * a->nelts;
    a->nelts++;

    ਵਾਪਸ elt;
}


ਖਾਲੀ *
ngx_array_ਧੱਕੋ_n(ngx_array_t *a, ngx_uint_t n)
{
    ਖਾਲੀ        *elt, *new;
    ਮਾਪ_t       ਮਾਪ;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    ਮਾਪ = n * a->ਮਾਪ;

    ਜੇ (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        ਜੇ ((u_char *) a->elts + a->ਮਾਪ * a->nalloc == p->d.ਆਖਰੀ
            && p->d.ਆਖਰੀ + ਮਾਪ <= p->d.ਸਿਰਾ)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.ਆਖਰੀ += ਮਾਪ;
            a->nalloc += n;

        } ਨਹੀਂਤਾਂ {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->ਮਾਪ);
            ਜੇ (new == ਨਿਰਾ) {
                ਵਾਪਸ ਨਿਰਾ;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->ਮਾਪ);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->ਮਾਪ * a->nelts;
    a->nelts += n;

    ਵਾਪਸ elt;
}
