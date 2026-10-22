
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ngx_array_t *a;

    a = ngx_palloc(p, ಗಾತ್ರ(ngx_array_t));
    ಆದರೆ (a == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆದರೆ (ngx_array_init(a, p, n, ಗಾತ್ರ_2) != NGX_OK) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ a;
}


ಶೂನ್ಯ
ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t  *p;

    p = a->pool;

    ಆದರೆ ((u_char *) a->elts + a->ಗಾತ್ರ_2 * a->nalloc == p->d.ಕೊನೆಯ) {
        p->d.ಕೊನೆಯ -= a->ಗಾತ್ರ_2 * a->nalloc;
    }

    ಆದರೆ ((u_char *) a + ಗಾತ್ರ(ngx_array_t) == p->d.ಕೊನೆಯ) {
        p->d.ಕೊನೆಯ = (u_char *) a;
    }
}


ಶೂನ್ಯ *
ngx_array_ತಳ್ಳು(ngx_array_t *a)
{
    ಶೂನ್ಯ        *elt, *new;
    ಗಾತ್ರ_t       ಗಾತ್ರ_2;
    ngx_pool_t  *p;

    ಆದರೆ (a->nelts == a->nalloc) {

        /* the array is full */

        ಗಾತ್ರ_2 = a->ಗಾತ್ರ_2 * a->nalloc;

        p = a->pool;

        ಆದರೆ ((u_char *) a->elts + ಗಾತ್ರ_2 == p->d.ಕೊನೆಯ
            && p->d.ಕೊನೆಯ + a->ಗಾತ್ರ_2 <= p->d.ಕೊನೆ)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.ಕೊನೆಯ += a->ಗಾತ್ರ_2;
            a->nalloc++;

        } ಇಲ್ಲದಿದ್ದರೆ {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * ಗಾತ್ರ_2);
            ಆದರೆ (new == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

            ngx_memcpy(new, a->elts, ಗಾತ್ರ_2);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->ಗಾತ್ರ_2 * a->nelts;
    a->nelts++;

    ಹಿಂತಿರುಗಿ elt;
}


ಶೂನ್ಯ *
ngx_array_ತಳ್ಳು_n(ngx_array_t *a, ngx_uint_t n)
{
    ಶೂನ್ಯ        *elt, *new;
    ಗಾತ್ರ_t       ಗಾತ್ರ_2;
    ngx_uint_t   nalloc;
    ngx_pool_t  *p;

    ಗಾತ್ರ_2 = n * a->ಗಾತ್ರ_2;

    ಆದರೆ (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        ಆದರೆ ((u_char *) a->elts + a->ಗಾತ್ರ_2 * a->nalloc == p->d.ಕೊನೆಯ
            && p->d.ಕೊನೆಯ + ಗಾತ್ರ_2 <= p->d.ಕೊನೆ)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.ಕೊನೆಯ += ಗಾತ್ರ_2;
            a->nalloc += n;

        } ಇಲ್ಲದಿದ್ದರೆ {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->ಗಾತ್ರ_2);
            ಆದರೆ (new == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->ಗಾತ್ರ_2);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *) a->elts + a->ಗಾತ್ರ_2 * a->nelts;
    a->nelts += n;

    ಹಿಂತಿರುಗಿ elt;
}
