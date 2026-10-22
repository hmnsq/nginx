
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, आकार_t आकार)
{
    ngx_array_t *a;

    a = ngx_palloc(p, परिमाण(ngx_array_t));
    यदि (a == रिक्त) {
        प्रत्यागम रिक्त;
    }

    यदि (ngx_array_init(a, p, n, आकार) != NGX_OK) {
        प्रत्यागम रिक्त;
    }

    प्रत्यागम a;
}


शून्य
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    यदि ((u_char *) a->elts + a->आकार * a->nalloc == p->d.चरमः) {
        p->d.चरमः -= a->आकार * a->nalloc;
    }

    यदि ((u_char *) a + परिमाण(ngx_array_t) == p->d.चरमः) {
        p->d.चरमः = (u_char *) a;
    }
}


शून्य *
ngx_array_प्रक्षिप(ngx_array_t *a)
{
    शून्य        *elt, *new;
    आकार_t       आकार;
    ngx_pool_t  *p;

    यदि (a->nelts == a->nalloc) {

        /* the array is full */

        आकार = a->आकार * a->nalloc;

        p = a->pool;

        यदि ((u_char *) a->elts + आकार == p->d.चरमः
            && p->d.चरमः + a->आकार <= p->d.अन्तः)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.चरमः += a->आकार;
            a->nalloc++;

        } अन्यथा {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * आकार);
            यदि (new == रिक्त) {
                प्रत्यागम रिक्त;
            }

            ngx_memcpy(new, a->elts, आकार);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->आकार * a->nelts;
    a->nelts++;

    प्रत्यागम elt;
}


शून्य *
ngx_array_प्रक्षिप_n(ngx_array_t *a, ngx_uint_t n)
{
    शून्य        *elt, *new;
    आकार_t       आकार;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    आकार = n * a->आकार;

    यदि (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        यदि ((u_char *) a->elts + a->आकार * a->nalloc == p->d.चरमः
            && p->d.चरमः + आकार <= p->d.अन्तः)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.चरमः += आकार;
            a->nalloc += n;

        } अन्यथा {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->आकार);
            यदि (new == रिक्त) {
                प्रत्यागम रिक्त;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->आकार);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->आकार * a->nelts;
    a->nelts += n;

    प्रत्यागम elt;
}
