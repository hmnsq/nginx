
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, కొలత_t కొలత)
{
    ngx_array_t *a;

    a = ngx_palloc(p, పరిమాణం(ngx_array_t));
    ఒకవేళ (a == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    ఒకవేళ (ngx_array_init(a, p, n, కొలత) != NGX_OK) {
        తిరిగి ఖాళీ;
    }

    తిరిగి a;
}


శూన్య
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    ఒకవేళ ((u_char *) a->elts + a->కొలత * a->nalloc == p->d.చివరి) {
        p->d.చివరి -= a->కొలత * a->nalloc;
    }

    ఒకవేళ ((u_char *) a + పరిమాణం(ngx_array_t) == p->d.చివరి) {
        p->d.చివరి = (u_char *) a;
    }
}


శూన్య *
ngx_array_నెట్టు(ngx_array_t *a)
{
    శూన్య        *elt, *new;
    కొలత_t       కొలత;
    ngx_pool_t  *p;

    ఒకవేళ (a->nelts == a->nalloc) {

        /* the array is full */

        కొలత = a->కొలత * a->nalloc;

        p = a->pool;

        ఒకవేళ ((u_char *) a->elts + కొలత == p->d.చివరి
            && p->d.చివరి + a->కొలత <= p->d.చివర)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.చివరి += a->కొలత;
            a->nalloc++;

        } లేకపోతే {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * కొలత);
            ఒకవేళ (new == ఖాళీ) {
                తిరిగి ఖాళీ;
            }

            ngx_memcpy(new, a->elts, కొలత);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->కొలత * a->nelts;
    a->nelts++;

    తిరిగి elt;
}


శూన్య *
ngx_array_నెట్టు_n(ngx_array_t *a, ngx_uint_t n)
{
    శూన్య        *elt, *new;
    కొలత_t       కొలత;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    కొలత = n * a->కొలత;

    ఒకవేళ (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        ఒకవేళ ((u_char *) a->elts + a->కొలత * a->nalloc == p->d.చివరి
            && p->d.చివరి + కొలత <= p->d.చివర)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.చివరి += కొలత;
            a->nalloc += n;

        } లేకపోతే {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->కొలత);
            ఒకవేళ (new == ఖాళీ) {
                తిరిగి ఖాళీ;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->కొలత);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->కొలత * a->nelts;
    a->nelts += n;

    తిరిగి elt;
}
