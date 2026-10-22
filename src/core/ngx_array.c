
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size)
{
    ngx_array_t *a;

    a = ngx_palloc(p, পরিমাণ(ngx_array_t));
    যদি (a == রিক্ত) {
        ঘূৰাই রিক্ত;
    }

    যদি (ngx_array_init(a, p, n, size) != NGX_OK) {
        ঘূৰাই রিক্ত;
    }

    ঘূৰাই a;
}


শূন্য
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    যদি ((u_char *) a->elts + a->size * a->nalloc == p->d.last) {
        p->d.last -= a->size * a->nalloc;
    }

    যদি ((u_char *) a + পরিমাণ(ngx_array_t) == p->d.last) {
        p->d.last = (u_char *) a;
    }
}


শূন্য *
ngx_array_ঠেলো(ngx_array_t *a)
{
    শূন্য        *elt, *new;
    size_t       size;
    ngx_pool_t  *p;

    যদি (a->nelts == a->nalloc) {

        /* the array is full */

        size = a->size * a->nalloc;

        p = a->pool;

        যদি ((u_char *) a->elts + size == p->d.last
            && p->d.last + a->size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->size;
            a->nalloc++;

        } অন্যথা {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * size);
            যদি (new == রিক্ত) {
                ঘূৰাই রিক্ত;
            }

            ngx_memcpy(new, a->elts, size);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts++;

    ঘূৰাই elt;
}


শূন্য *
ngx_array_ঠেলো_n(ngx_array_t *a, ngx_uint_t n)
{
    শূন্য        *elt, *new;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    size = n * a->size;

    যদি (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        যদি ((u_char *) a->elts + a->size * a->nalloc == p->d.last
            && p->d.last + size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += size;
            a->nalloc += n;

        } অন্যথা {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->size);
            যদি (new == রিক্ত) {
                ঘূৰাই রিক্ত;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->size);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts += n;

    ঘূৰাই elt;
}
