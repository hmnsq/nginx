
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size)
{
    ngx_array_t *a;

    a = ngx_palloc(p, அளவு(ngx_array_t));
    எனில் (a == வெற்று) {
        திரும்பு வெற்று;
    }

    எனில் (ngx_array_init(a, p, n, size) != NGX_OK) {
        திரும்பு வெற்று;
    }

    திரும்பு a;
}


வெறுமை
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    எனில் ((u_char *) a->elts + a->size * a->nalloc == p->d.last) {
        p->d.last -= a->size * a->nalloc;
    }

    எனில் ((u_char *) a + அளவு(ngx_array_t) == p->d.last) {
        p->d.last = (u_char *) a;
    }
}


வெறுமை *
ngx_array_தள்ளு(ngx_array_t *a)
{
    வெறுமை        *elt, *new;
    size_t       size;
    ngx_pool_t  *p;

    எனில் (a->nelts == a->nalloc) {

        /* the array is full */

        size = a->size * a->nalloc;

        p = a->pool;

        எனில் ((u_char *) a->elts + size == p->d.last
            && p->d.last + a->size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->size;
            a->nalloc++;

        } இல்லையெனில் {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * size);
            எனில் (new == வெற்று) {
                திரும்பு வெற்று;
            }

            ngx_memcpy(new, a->elts, size);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts++;

    திரும்பு elt;
}


வெறுமை *
ngx_array_தள்ளு_n(ngx_array_t *a, ngx_uint_t n)
{
    வெறுமை        *elt, *new;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    size = n * a->size;

    எனில் (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        எனில் ((u_char *) a->elts + a->size * a->nalloc == p->d.last
            && p->d.last + size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += size;
            a->nalloc += n;

        } இல்லையெனில் {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->size);
            எனில் (new == வெற்று) {
                திரும்பு வெற்று;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->size);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts += n;

    திரும்பு elt;
}
