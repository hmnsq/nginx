
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size)
{
    ngx_array_t *a;

    a = ngx_palloc(p, حجم(ngx_array_t));
    إذا (a == عدم) {
        أرجع عدم;
    }

    إذا (ngx_array_init(a, p, n, size) != NGX_OK) {
        أرجع عدم;
    }

    أرجع a;
}


فراغ
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    إذا ((u_char *) a->elts + a->size * a->nalloc == p->d.last) {
        p->d.last -= a->size * a->nalloc;
    }

    إذا ((u_char *) a + حجم(ngx_array_t) == p->d.last) {
        p->d.last = (u_char *) a;
    }
}


فراغ *
ngx_array_پاڵنان(ngx_array_t *a)
{
    فراغ        *elt, *new;
    size_t       size;
    ngx_pool_t  *p;

    إذا (a->nelts == a->nalloc) {

        /* the array is full */

        size = a->size * a->nalloc;

        p = a->pool;

        إذا ((u_char *) a->elts + size == p->d.last
            && p->d.last + a->size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->size;
            a->nalloc++;

        } وإلا {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * size);
            إذا (new == عدم) {
                أرجع عدم;
            }

            ngx_memcpy(new, a->elts, size);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts++;

    أرجع elt;
}


فراغ *
ngx_array_پاڵنان_n(ngx_array_t *a, ngx_uint_t n)
{
    فراغ        *elt, *new;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    size = n * a->size;

    إذا (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        إذا ((u_char *) a->elts + a->size * a->nalloc == p->d.last
            && p->d.last + size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += size;
            a->nalloc += n;

        } وإلا {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->size);
            إذا (new == عدم) {
                أرجع عدم;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->size);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts += n;

    أرجع elt;
}
