
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, माप_प्रकार आकार)
{
    ngx_array_t *a;

    a = ngx_palloc(p, माप(ngx_array_t));
    अगर (a == शून्य) {
        वापस शून्य;
    }

    अगर (ngx_array_init(a, p, n, आकार) != NGX_OK) {
        वापस शून्य;
    }

    वापस a;
}


व्योम
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    अगर ((u_char *) a->elts + a->आकार * a->nalloc == p->d.last) {
        p->d.last -= a->आकार * a->nalloc;
    }

    अगर ((u_char *) a + माप(ngx_array_t) == p->d.last) {
        p->d.last = (u_char *) a;
    }
}


व्योम *
ngx_array_धकेलो(ngx_array_t *a)
{
    व्योम        *elt, *new;
    माप_प्रकार       आकार;
    ngx_pool_t  *p;

    अगर (a->nelts == a->nalloc) {

        /* the array is full */

        आकार = a->आकार * a->nalloc;

        p = a->pool;

        अगर ((u_char *) a->elts + आकार == p->d.last
            && p->d.last + a->आकार <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->आकार;
            a->nalloc++;

        } अन्यथा {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * आकार);
            अगर (new == शून्य) {
                वापस शून्य;
            }

            ngx_memcpy(new, a->elts, आकार);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->आकार * a->nelts;
    a->nelts++;

    वापस elt;
}


व्योम *
ngx_array_धकेलो_n(ngx_array_t *a, ngx_uint_t n)
{
    व्योम        *elt, *new;
    माप_प्रकार       आकार;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    आकार = n * a->आकार;

    अगर (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        अगर ((u_char *) a->elts + a->आकार * a->nalloc == p->d.last
            && p->d.last + आकार <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += आकार;
            a->nalloc += n;

        } अन्यथा {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->आकार);
            अगर (new == शून्य) {
                वापस शून्य;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->आकार);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->आकार * a->nelts;
    a->nelts += n;

    वापस elt;
}
