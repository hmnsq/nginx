
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size)
{
    ngx_array_t *a;

    a = ngx_palloc(p, ಪರಿಮಾಣ(ngx_array_t));
    ಯದಿ (a == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಯದಿ (ngx_array_init(a, p, n, size) != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ a;
}


ಶೂನ್ಯ
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    ಯದಿ ((u_char *) a->elts + a->size * a->nalloc == p->d.last) {
        p->d.last -= a->size * a->nalloc;
    }

    ಯದಿ ((u_char *) a + ಪರಿಮಾಣ(ngx_array_t) == p->d.last) {
        p->d.last = (u_char *) a;
    }
}


ಶೂನ್ಯ *
ngx_array_ತಳ್ಳು(ngx_array_t *a)
{
    ಶೂನ್ಯ        *elt, *new;
    size_t       size;
    ngx_pool_t  *p;

    ಯದಿ (a->nelts == a->nalloc) {

        /* the array is full */

        size = a->size * a->nalloc;

        p = a->pool;

        ಯದಿ ((u_char *) a->elts + size == p->d.last
            && p->d.last + a->size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->size;
            a->nalloc++;

        } ಅನ್ಯಥಾ {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * size);
            ಯದಿ (new == ರಿಕ್ತ) {
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }

            ngx_memcpy(new, a->elts, size);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts++;

    ಪ್ರತ್ಯಾಗಮ elt;
}


ಶೂನ್ಯ *
ngx_array_ತಳ್ಳು_n(ngx_array_t *a, ngx_uint_t n)
{
    ಶೂನ್ಯ        *elt, *new;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    size = n * a->size;

    ಯದಿ (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        ಯದಿ ((u_char *) a->elts + a->size * a->nalloc == p->d.last
            && p->d.last + size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += size;
            a->nalloc += n;

        } ಅನ್ಯಥಾ {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->size);
            ಯದಿ (new == ರಿಕ್ತ) {
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->size);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts += n;

    ಪ್ರತ್ಯಾಗಮ elt;
}
