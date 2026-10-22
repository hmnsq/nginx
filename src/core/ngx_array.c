
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size)
{
    ngx_array_t *a;

    a = ngx_palloc(p, വലുപ്പം(ngx_array_t));
    എങ്കിൽ (a == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    എങ്കിൽ (ngx_array_init(a, p, n, size) != NGX_OK) {
        തിരികെ ശൂന്യ;
    }

    തിരികെ a;
}


ശൂന്യം
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    എങ്കിൽ ((u_char *) a->elts + a->size * a->nalloc == p->d.last) {
        p->d.last -= a->size * a->nalloc;
    }

    എങ്കിൽ ((u_char *) a + വലുപ്പം(ngx_array_t) == p->d.last) {
        p->d.last = (u_char *) a;
    }
}


ശൂന്യം *
ngx_array_തള്ളുക(ngx_array_t *a)
{
    ശൂന്യം        *elt, *new;
    size_t       size;
    ngx_pool_t  *p;

    എങ്കിൽ (a->nelts == a->nalloc) {

        /* the array is full */

        size = a->size * a->nalloc;

        p = a->pool;

        എങ്കിൽ ((u_char *) a->elts + size == p->d.last
            && p->d.last + a->size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->size;
            a->nalloc++;

        } അല്ലെങ്കിൽ {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * size);
            എങ്കിൽ (new == ശൂന്യ) {
                തിരികെ ശൂന്യ;
            }

            ngx_memcpy(new, a->elts, size);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts++;

    തിരികെ elt;
}


ശൂന്യം *
ngx_array_തള്ളുക_n(ngx_array_t *a, ngx_uint_t n)
{
    ശൂന്യം        *elt, *new;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    size = n * a->size;

    എങ്കിൽ (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        എങ്കിൽ ((u_char *) a->elts + a->size * a->nalloc == p->d.last
            && p->d.last + size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += size;
            a->nalloc += n;

        } അല്ലെങ്കിൽ {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->size);
            എങ്കിൽ (new == ശൂന്യ) {
                തിരികെ ശൂന്യ;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->size);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts += n;

    തിരികെ elt;
}
