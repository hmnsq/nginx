
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, আকার_t আকার_2)
{
    ngx_array_t *a;

    a = ngx_palloc(p, আকার(ngx_array_t));
    যদি (a == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    যদি (ngx_array_init(a, p, n, আকার_2) != NGX_OK) {
        ফেরত ফাঁকা;
    }

    ফেরত a;
}


শূন্য
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    যদি ((u_char *) a->elts + a->আকার_2 * a->nalloc == p->d.last) {
        p->d.last -= a->আকার_2 * a->nalloc;
    }

    যদি ((u_char *) a + আকার(ngx_array_t) == p->d.last) {
        p->d.last = (u_char *) a;
    }
}


শূন্য *
ngx_array_ঠেলো(ngx_array_t *a)
{
    শূন্য        *elt, *new;
    আকার_t       আকার_2;
    ngx_pool_t  *p;

    যদি (a->nelts == a->nalloc) {

        /* the array is full */

        আকার_2 = a->আকার_2 * a->nalloc;

        p = a->pool;

        যদি ((u_char *) a->elts + আকার_2 == p->d.last
            && p->d.last + a->আকার_2 <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->আকার_2;
            a->nalloc++;

        } অন্যথা {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * আকার_2);
            যদি (new == ফাঁকা) {
                ফেরত ফাঁকা;
            }

            ngx_memcpy(new, a->elts, আকার_2);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->আকার_2 * a->nelts;
    a->nelts++;

    ফেরত elt;
}


শূন্য *
ngx_array_ঠেলো_n(ngx_array_t *a, ngx_uint_t n)
{
    শূন্য        *elt, *new;
    আকার_t       আকার_2;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    আকার_2 = n * a->আকার_2;

    যদি (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        যদি ((u_char *) a->elts + a->আকার_2 * a->nalloc == p->d.last
            && p->d.last + আকার_2 <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += আকার_2;
            a->nalloc += n;

        } অন্যথা {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->আকার_2);
            যদি (new == ফাঁকা) {
                ফেরত ফাঁকা;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->আকার_2);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->আকার_2 * a->nelts;
    a->nelts += n;

    ফেরত elt;
}
