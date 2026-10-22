
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size)
{
    ngx_array_t *a;

    a = ngx_palloc(p, ଆକାର(ngx_array_t));
    ଯଦି (a == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯଦି (ngx_array_init(a, p, n, size) != NGX_OK) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ a;
}


ଶୂନ୍ୟ
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    ଯଦି ((u_char *) a->elts + a->size * a->nalloc == p->d.last) {
        p->d.last -= a->size * a->nalloc;
    }

    ଯଦି ((u_char *) a + ଆକାର(ngx_array_t) == p->d.last) {
        p->d.last = (u_char *) a;
    }
}


ଶୂନ୍ୟ *
ngx_array_ଠେଲ(ngx_array_t *a)
{
    ଶୂନ୍ୟ        *elt, *new;
    size_t       size;
    ngx_pool_t  *p;

    ଯଦି (a->nelts == a->nalloc) {

        /* the array is full */

        size = a->size * a->nalloc;

        p = a->pool;

        ଯଦି ((u_char *) a->elts + size == p->d.last
            && p->d.last + a->size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->size;
            a->nalloc++;

        } ଅନ୍ୟଥା {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * size);
            ଯଦି (new == ଖାଲି) {
                ଫେରନ୍ତୁ ଖାଲି;
            }

            ngx_memcpy(new, a->elts, size);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts++;

    ଫେରନ୍ତୁ elt;
}


ଶୂନ୍ୟ *
ngx_array_ଠେଲ_n(ngx_array_t *a, ngx_uint_t n)
{
    ଶୂନ୍ୟ        *elt, *new;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    size = n * a->size;

    ଯଦି (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        ଯଦି ((u_char *) a->elts + a->size * a->nalloc == p->d.last
            && p->d.last + size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += size;
            a->nalloc += n;

        } ଅନ୍ୟଥା {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->size);
            ଯଦି (new == ଖାଲି) {
                ଫେରନ୍ତୁ ଖାଲି;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->size);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts += n;

    ଫେରନ୍ତୁ elt;
}
