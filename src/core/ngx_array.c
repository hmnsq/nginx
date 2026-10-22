
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, ناپ_t ناپ)
{
    ngx_array_t *a;

    a = ngx_palloc(p, حجم(ngx_array_t));
    إذا (a == عدم) {
        أرجع عدم;
    }

    إذا (ngx_array_init(a, p, n, ناپ) != NGX_OK) {
        أرجع عدم;
    }

    أرجع a;
}


فراغ
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    إذا ((u_char *) a->elts + a->ناپ * a->nalloc == p->d.آخری) {
        p->d.آخری -= a->ناپ * a->nalloc;
    }

    إذا ((u_char *) a + حجم(ngx_array_t) == p->d.آخری) {
        p->d.آخری = (u_char *) a;
    }
}


فراغ *
ngx_array_ڈالو(ngx_array_t *a)
{
    فراغ        *elt, *new;
    ناپ_t       ناپ;
    ngx_pool_t  *p;

    إذا (a->nelts == a->nalloc) {

        /* the array is full */

        ناپ = a->ناپ * a->nalloc;

        p = a->pool;

        إذا ((u_char *) a->elts + ناپ == p->d.آخری
            && p->d.آخری + a->ناپ <= p->d.سرا)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.آخری += a->ناپ;
            a->nalloc++;

        } وإلا {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * ناپ);
            إذا (new == عدم) {
                أرجع عدم;
            }

            ngx_memcpy(new, a->elts, ناپ);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->ناپ * a->nelts;
    a->nelts++;

    أرجع elt;
}


فراغ *
ngx_array_ڈالو_n(ngx_array_t *a, ngx_uint_t n)
{
    فراغ        *elt, *new;
    ناپ_t       ناپ;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    ناپ = n * a->ناپ;

    إذا (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        إذا ((u_char *) a->elts + a->ناپ * a->nalloc == p->d.آخری
            && p->d.آخری + ناپ <= p->d.سرا)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.آخری += ناپ;
            a->nalloc += n;

        } وإلا {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->ناپ);
            إذا (new == عدم) {
                أرجع عدم;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->ناپ);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->ناپ * a->nelts;
    a->nelts += n;

    أرجع elt;
}
