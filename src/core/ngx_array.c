
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size)
{
    ngx_array_t *a;

    a = ngx_palloc(p, કદ(ngx_array_t));
    જો (a == ખાલી) {
        પાછા ખાલી;
    }

    જો (ngx_array_init(a, p, n, size) != NGX_OK) {
        પાછા ખાલી;
    }

    પાછા a;
}


શૂન્ય
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    જો ((u_char *) a->elts + a->size * a->nalloc == p->d.last) {
        p->d.last -= a->size * a->nalloc;
    }

    જો ((u_char *) a + કદ(ngx_array_t) == p->d.last) {
        p->d.last = (u_char *) a;
    }
}


શૂન્ય *
ngx_array_ધકેલો(ngx_array_t *a)
{
    શૂન્ય        *elt, *new;
    size_t       size;
    ngx_pool_t  *p;

    જો (a->nelts == a->nalloc) {

        /* the array is full */

        size = a->size * a->nalloc;

        p = a->pool;

        જો ((u_char *) a->elts + size == p->d.last
            && p->d.last + a->size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->size;
            a->nalloc++;

        } નહીંતર {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * size);
            જો (new == ખાલી) {
                પાછા ખાલી;
            }

            ngx_memcpy(new, a->elts, size);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts++;

    પાછા elt;
}


શૂન્ય *
ngx_array_ધકેલો_n(ngx_array_t *a, ngx_uint_t n)
{
    શૂન્ય        *elt, *new;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    size = n * a->size;

    જો (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        જો ((u_char *) a->elts + a->size * a->nalloc == p->d.last
            && p->d.last + size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += size;
            a->nalloc += n;

        } નહીંતર {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->size);
            જો (new == ખાલી) {
                પાછા ખાલી;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->size);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts += n;

    પાછા elt;
}
