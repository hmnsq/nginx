
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن فراغ ngx_کتار_یوځای(ngx_کتار_t *کتار, ngx_کتار_t *لکۍ,
    ngx_int_t (*cmp)(ثابت ngx_کتار_t *, ثابت ngx_کتار_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_کتار_t *
ngx_کتار_منځ(ngx_کتار_t *کتار)
{
    ngx_کتار_t  *منځ, *بل;

    منځ = ngx_کتار_سر(کتار);

    إذا (منځ == ngx_کتار_وروستی(کتار)) {
        أرجع منځ;
    }

    بل = ngx_کتار_سر(کتار);

    لكل ( ;; ) {
        منځ = ngx_کتار_بل(منځ);

        بل = ngx_کتار_بل(بل);

        إذا (بل == ngx_کتار_وروستی(کتار)) {
            أرجع منځ;
        }

        بل = ngx_کتار_بل(بل);

        إذا (بل == ngx_کتار_وروستی(کتار)) {
            أرجع منځ;
        }
    }
}


/* the stable merge sort */

فراغ
ngx_کتار_ترتیبول(ngx_کتار_t *کتار,
    ngx_int_t (*cmp)(ثابت ngx_کتار_t *, ثابت ngx_کتار_t *))
{
    ngx_کتار_t  *q, لکۍ;

    q = ngx_کتار_سر(کتار);

    إذا (q == ngx_کتار_وروستی(کتار)) {
        أرجع;
    }

    q = ngx_کتار_منځ(کتار);

    ngx_کتار_وېشل(کتار, q, &لکۍ);

    ngx_کتار_ترتیبول(کتار, cmp);
    ngx_کتار_ترتیبول(&لکۍ, cmp);

    ngx_کتار_یوځای(کتار, &لکۍ, cmp);
}


ساكن فراغ
ngx_کتار_یوځای(ngx_کتار_t *کتار, ngx_کتار_t *لکۍ,
    ngx_int_t (*cmp)(ثابت ngx_کتار_t *, ثابت ngx_کتار_t *))
{
    ngx_کتار_t  *q1, *q2;

    q1 = ngx_کتار_سر(کتار);
    q2 = ngx_کتار_سر(لکۍ);

    لكل ( ;; ) {
        إذا (q1 == ngx_کتار_sentinel(کتار)) {
            ngx_کتار_جمع_کړه(کتار, لکۍ);
            اكسر;
        }

        إذا (q2 == ngx_کتار_sentinel(لکۍ)) {
            اكسر;
        }

        إذا (cmp(q1, q2) <= 0) {
            q1 = ngx_کتار_بل(q1);
            استمر;
        }

        ngx_کتار_remove(q2);
        ngx_کتار_ورننباسه_before(q1, q2);

        q2 = ngx_کتار_سر(لکۍ);
    }
}
