
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, سائيز_t سائيز)
{
    ngx_array_t *a;

    a = ngx_palloc(p, حجم(ngx_array_t));
    إذا (a == عدم) {
        أرجع عدم;
    }

    إذا (ngx_array_init(a, p, n, سائيز) != NGX_OK) {
        أرجع عدم;
    }

    أرجع a;
}


فراغ
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    إذا ((u_char *) a->elts + a->سائيز * a->nalloc == p->d.آخري) {
        p->d.آخري -= a->سائيز * a->nalloc;
    }

    إذا ((u_char *) a + حجم(ngx_array_t) == p->d.آخري) {
        p->d.آخري = (u_char *) a;
    }
}


فراغ *
ngx_array_ڌڪو(ngx_array_t *a)
{
    فراغ        *elt, *new;
    سائيز_t       سائيز;
    ngx_pool_t  *p;

    إذا (a->nelts == a->nalloc) {

        /* the array is full */

        سائيز = a->سائيز * a->nalloc;

        p = a->pool;

        إذا ((u_char *) a->elts + سائيز == p->d.آخري
            && p->d.آخري + a->سائيز <= p->d.پڇاڙي)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.آخري += a->سائيز;
            a->nalloc++;

        } وإلا {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * سائيز);
            إذا (new == عدم) {
                أرجع عدم;
            }

            ngx_memcpy(new, a->elts, سائيز);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->سائيز * a->nelts;
    a->nelts++;

    أرجع elt;
}


فراغ *
ngx_array_ڌڪو_n(ngx_array_t *a, ngx_uint_t n)
{
    فراغ        *elt, *new;
    سائيز_t       سائيز;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    سائيز = n * a->سائيز;

    إذا (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        إذا ((u_char *) a->elts + a->سائيز * a->nalloc == p->d.آخري
            && p->d.آخري + سائيز <= p->d.پڇاڙي)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.آخري += سائيز;
            a->nalloc += n;

        } وإلا {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->سائيز);
            إذا (new == عدم) {
                أرجع عدم;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->سائيز);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->سائيز * a->nelts;
    a->nelts += n;

    أرجع elt;
}
