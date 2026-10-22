
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size)
{
    ngx_array_t *a;

    a = ngx_palloc(p, اندازه(ngx_array_t));
    اگر (a == پوچ) {
        بازگشت پوچ;
    }

    اگر (ngx_array_init(a, p, n, size) != NGX_OK) {
        بازگشت پوچ;
    }

    بازگشت a;
}


تهی
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    اگر ((u_char *) a->elts + a->size * a->nalloc == p->d.last) {
        p->d.last -= a->size * a->nalloc;
    }

    اگر ((u_char *) a + اندازه(ngx_array_t) == p->d.last) {
        p->d.last = (u_char *) a;
    }
}


تهی *
ngx_array_افزودن(ngx_array_t *a)
{
    تهی        *elt, *new;
    size_t       size;
    ngx_pool_t  *p;

    اگر (a->nelts == a->nalloc) {

        /* the array is full */

        size = a->size * a->nalloc;

        p = a->pool;

        اگر ((u_char *) a->elts + size == p->d.last
            && p->d.last + a->size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->size;
            a->nalloc++;

        } وگرنه {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * size);
            اگر (new == پوچ) {
                بازگشت پوچ;
            }

            ngx_memcpy(new, a->elts, size);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts++;

    بازگشت elt;
}


تهی *
ngx_array_افزودن_n(ngx_array_t *a, ngx_uint_t n)
{
    تهی        *elt, *new;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    size = n * a->size;

    اگر (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        اگر ((u_char *) a->elts + a->size * a->nalloc == p->d.last
            && p->d.last + size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += size;
            a->nalloc += n;

        } وگرنه {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->size);
            اگر (new == پوچ) {
                بازگشت پوچ;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->size);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts += n;

    بازگشت elt;
}
