
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, سائز_t سائز)
{
    ngx_array_t *a;

    a = ngx_palloc(p, حجم(ngx_array_t));
    اگر (a == تہی) {
        واپس تہی;
    }

    اگر (ngx_array_init(a, p, n, سائز) != NGX_OK) {
        واپس تہی;
    }

    واپس a;
}


خالی
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    اگر ((u_char *) a->elts + a->سائز * a->nalloc == p->d.آخری) {
        p->d.آخری -= a->سائز * a->nalloc;
    }

    اگر ((u_char *) a + حجم(ngx_array_t) == p->d.آخری) {
        p->d.آخری = (u_char *) a;
    }
}


خالی *
ngx_array_ڈالو(ngx_array_t *a)
{
    خالی        *elt, *new;
    سائز_t       سائز;
    ngx_pool_t  *p;

    اگر (a->nelts == a->nalloc) {

        /* the array is full */

        سائز = a->سائز * a->nalloc;

        p = a->pool;

        اگر ((u_char *) a->elts + سائز == p->d.آخری
            && p->d.آخری + a->سائز <= p->d.انتہا)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.آخری += a->سائز;
            a->nalloc++;

        } ورنہ {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * سائز);
            اگر (new == تہی) {
                واپس تہی;
            }

            ngx_memcpy(new, a->elts, سائز);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->سائز * a->nelts;
    a->nelts++;

    واپس elt;
}


خالی *
ngx_array_ڈالو_n(ngx_array_t *a, ngx_uint_t n)
{
    خالی        *elt, *new;
    سائز_t       سائز;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    سائز = n * a->سائز;

    اگر (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        اگر ((u_char *) a->elts + a->سائز * a->nalloc == p->d.آخری
            && p->d.آخری + سائز <= p->d.انتہا)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.آخری += سائز;
            a->nalloc += n;

        } ورنہ {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->سائز);
            اگر (new == تہی) {
                واپس تہی;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->سائز);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->سائز * a->nelts;
    a->nelts += n;

    واپس elt;
}
