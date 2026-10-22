
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن فراغ ngx_قطار_ملايو(ngx_قطار_t *قطار, ngx_قطار_t *پڇ,
    ngx_int_t (*cmp)(ثابت ngx_قطار_t *, ثابت ngx_قطار_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_قطار_t *
ngx_قطار_وچ(ngx_قطار_t *قطار)
{
    ngx_قطار_t  *وچ, *ايندڙ;

    وچ = ngx_قطار_سر(قطار);

    إذا (وچ == ngx_قطار_آخري(قطار)) {
        أرجع وچ;
    }

    ايندڙ = ngx_قطار_سر(قطار);

    لكل ( ;; ) {
        وچ = ngx_قطار_ايندڙ(وچ);

        ايندڙ = ngx_قطار_ايندڙ(ايندڙ);

        إذا (ايندڙ == ngx_قطار_آخري(قطار)) {
            أرجع وچ;
        }

        ايندڙ = ngx_قطار_ايندڙ(ايندڙ);

        إذا (ايندڙ == ngx_قطار_آخري(قطار)) {
            أرجع وچ;
        }
    }
}


/* the stable merge sort */

فراغ
ngx_قطار_ترتيب_ڏيو(ngx_قطار_t *قطار,
    ngx_int_t (*cmp)(ثابت ngx_قطار_t *, ثابت ngx_قطار_t *))
{
    ngx_قطار_t  *q, پڇ;

    q = ngx_قطار_سر(قطار);

    إذا (q == ngx_قطار_آخري(قطار)) {
        أرجع;
    }

    q = ngx_قطار_وچ(قطار);

    ngx_قطار_ورهايو(قطار, q, &پڇ);

    ngx_قطار_ترتيب_ڏيو(قطار, cmp);
    ngx_قطار_ترتيب_ڏيو(&پڇ, cmp);

    ngx_قطار_ملايو(قطار, &پڇ, cmp);
}


ساكن فراغ
ngx_قطار_ملايو(ngx_قطار_t *قطار, ngx_قطار_t *پڇ,
    ngx_int_t (*cmp)(ثابت ngx_قطار_t *, ثابت ngx_قطار_t *))
{
    ngx_قطار_t  *q1, *q2;

    q1 = ngx_قطار_سر(قطار);
    q2 = ngx_قطار_سر(پڇ);

    لكل ( ;; ) {
        إذا (q1 == ngx_قطار_sentinel(قطار)) {
            ngx_قطار_جوڙيو(قطار, پڇ);
            اكسر;
        }

        إذا (q2 == ngx_قطار_sentinel(پڇ)) {
            اكسر;
        }

        إذا (cmp(q1, q2) <= 0) {
            q1 = ngx_قطار_ايندڙ(q1);
            استمر;
        }

        ngx_قطار_remove(q2);
        ngx_قطار_وجهو_before(q1, q2);

        q2 = ngx_قطار_سر(پڇ);
    }
}
