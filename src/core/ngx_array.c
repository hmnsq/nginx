
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size)
{
    ngx_array_t *a;

    a = ngx_palloc(p, parimaa_na(ngx_array_t));
    ꦪꦼꦤ꧀ (a == NULL) {
        ꦮꦭꦶ NULL;
    }

    ꦪꦼꦤ꧀ (ngx_array_init(a, p, n, size) != NGX_OK) {
        ꦮꦭꦶ NULL;
    }

    ꦮꦭꦶ a;
}


void
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    ꦪꦼꦤ꧀ ((u_char *) a->elts + a->size * a->nalloc == p->d.last) {
        p->d.last -= a->size * a->nalloc;
    }

    ꦪꦼꦤ꧀ ((u_char *) a + parimaa_na(ngx_array_t) == p->d.last) {
        p->d.last = (u_char *) a;
    }
}


void *
ngx_array_push(ngx_array_t *a)
{
    void        *elt, *new;
    size_t       size;
    ngx_pool_t  *p;

    ꦪꦼꦤ꧀ (a->nelts == a->nalloc) {

        /* the array is full */

        size = a->size * a->nalloc;

        p = a->pool;

        ꦪꦼꦤ꧀ ((u_char *) a->elts + size == p->d.last
            && p->d.last + a->size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->size;
            a->nalloc++;

        } ꦭꦶꦪ {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * size);
            ꦪꦼꦤ꧀ (new == NULL) {
                ꦮꦭꦶ NULL;
            }

            ngx_memcpy(new, a->elts, size);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts++;

    ꦮꦭꦶ elt;
}


void *
ngx_array_push_n(ngx_array_t *a, ngx_uint_t n)
{
    void        *elt, *new;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    size = n * a->size;

    ꦪꦼꦤ꧀ (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        ꦪꦼꦤ꧀ ((u_char *) a->elts + a->size * a->nalloc == p->d.last
            && p->d.last + size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += size;
            a->nalloc += n;

        } ꦭꦶꦪ {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->size);
            ꦪꦼꦤ꧀ (new == NULL) {
                ꦮꦭꦶ NULL;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->size);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts += n;

    ꦮꦭꦶ elt;
}
