
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, माप_प्रकार size)
{
    ngx_array_t *a;

    a = ngx_palloc(p, परिमाण(ngx_array_t));
    यदि (a == रिक्त) {
        फर्काऊ रिक्त;
    }

    यदि (ngx_array_init(a, p, n, size) != NGX_OK) {
        फर्काऊ रिक्त;
    }

    फर्काऊ a;
}


शून्य
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    यदि ((u_char *) a->elts + a->size * a->nalloc == p->d.last) {
        p->d.last -= a->size * a->nalloc;
    }

    यदि ((u_char *) a + परिमाण(ngx_array_t) == p->d.last) {
        p->d.last = (u_char *) a;
    }
}


शून्य *
ngx_array_थप(ngx_array_t *a)
{
    शून्य        *elt, *new;
    माप_प्रकार       size;
    ngx_pool_t  *p;

    यदि (a->nelts == a->nalloc) {

        /* the array is full */

        size = a->size * a->nalloc;

        p = a->pool;

        यदि ((u_char *) a->elts + size == p->d.last
            && p->d.last + a->size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->size;
            a->nalloc++;

        } अन्यथा {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * size);
            यदि (new == रिक्त) {
                फर्काऊ रिक्त;
            }

            ngx_memcpy(new, a->elts, size);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts++;

    फर्काऊ elt;
}


शून्य *
ngx_array_थप_n(ngx_array_t *a, ngx_uint_t n)
{
    शून्य        *elt, *new;
    माप_प्रकार       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    size = n * a->size;

    यदि (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        यदि ((u_char *) a->elts + a->size * a->nalloc == p->d.last
            && p->d.last + size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += size;
            a->nalloc += n;

        } अन्यथा {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->size);
            यदि (new == रिक्त) {
                फर्काऊ रिक्त;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->size);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts += n;

    फर्काऊ elt;
}
