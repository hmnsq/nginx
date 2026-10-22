
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, اندازه_t اندازه)
{
    ngx_array_t *a;

    a = ngx_palloc(p, حجم(ngx_array_t));
    إذا (a == عدم) {
        أرجع عدم;
    }

    إذا (ngx_array_init(a, p, n, اندازه) != NGX_OK) {
        أرجع عدم;
    }

    أرجع a;
}


فراغ
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    إذا ((u_char *) a->elts + a->اندازه * a->nalloc == p->d.وروستی) {
        p->d.وروستی -= a->اندازه * a->nalloc;
    }

    إذا ((u_char *) a + حجم(ngx_array_t) == p->d.وروستی) {
        p->d.وروستی = (u_char *) a;
    }
}


فراغ *
ngx_array_ټېل(ngx_array_t *a)
{
    فراغ        *elt, *new;
    اندازه_t       اندازه;
    ngx_pool_t  *p;

    إذا (a->nelts == a->nalloc) {

        /* the array is full */

        اندازه = a->اندازه * a->nalloc;

        p = a->pool;

        إذا ((u_char *) a->elts + اندازه == p->d.وروستی
            && p->d.وروستی + a->اندازه <= p->d.پای_ټکی)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.وروستی += a->اندازه;
            a->nalloc++;

        } وإلا {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * اندازه);
            إذا (new == عدم) {
                أرجع عدم;
            }

            ngx_memcpy(new, a->elts, اندازه);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->اندازه * a->nelts;
    a->nelts++;

    أرجع elt;
}


فراغ *
ngx_array_ټېل_n(ngx_array_t *a, ngx_uint_t n)
{
    فراغ        *elt, *new;
    اندازه_t       اندازه;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    اندازه = n * a->اندازه;

    إذا (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        إذا ((u_char *) a->elts + a->اندازه * a->nalloc == p->d.وروستی
            && p->d.وروستی + اندازه <= p->d.پای_ټکی)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.وروستی += اندازه;
            a->nalloc += n;

        } وإلا {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->اندازه);
            إذا (new == عدم) {
                أرجع عدم;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->اندازه);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->اندازه * a->nelts;
    a->nelts += n;

    أرجع elt;
}
