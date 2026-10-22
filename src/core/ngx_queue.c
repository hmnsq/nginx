
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن فراغ ngx_قطار_ملاؤ(ngx_قطار_t *قطار, ngx_قطار_t *پوچھ,
    ngx_int_t (*cmp)(ثابت ngx_قطار_t *, ثابت ngx_قطار_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_قطار_t *
ngx_قطار_درمیان(ngx_قطار_t *قطار)
{
    ngx_قطار_t  *درمیان, *اگلا;

    درمیان = ngx_قطار_سر(قطار);

    إذا (درمیان == ngx_قطار_آخری(قطار)) {
        أرجع درمیان;
    }

    اگلا = ngx_قطار_سر(قطار);

    لكل ( ;; ) {
        درمیان = ngx_قطار_اگلا(درمیان);

        اگلا = ngx_قطار_اگلا(اگلا);

        إذا (اگلا == ngx_قطار_آخری(قطار)) {
            أرجع درمیان;
        }

        اگلا = ngx_قطار_اگلا(اگلا);

        إذا (اگلا == ngx_قطار_آخری(قطار)) {
            أرجع درمیان;
        }
    }
}


/* the stable merge sort */

فراغ
ngx_قطار_ترتیب_دیو(ngx_قطار_t *قطار,
    ngx_int_t (*cmp)(ثابت ngx_قطار_t *, ثابت ngx_قطار_t *))
{
    ngx_قطار_t  *q, پوچھ;

    q = ngx_قطار_سر(قطار);

    إذا (q == ngx_قطار_آخری(قطار)) {
        أرجع;
    }

    q = ngx_قطار_درمیان(قطار);

    ngx_قطار_ونڈو(قطار, q, &پوچھ);

    ngx_قطار_ترتیب_دیو(قطار, cmp);
    ngx_قطار_ترتیب_دیو(&پوچھ, cmp);

    ngx_قطار_ملاؤ(قطار, &پوچھ, cmp);
}


ساكن فراغ
ngx_قطار_ملاؤ(ngx_قطار_t *قطار, ngx_قطار_t *پوچھ,
    ngx_int_t (*cmp)(ثابت ngx_قطار_t *, ثابت ngx_قطار_t *))
{
    ngx_قطار_t  *q1, *q2;

    q1 = ngx_قطار_سر(قطار);
    q2 = ngx_قطار_سر(پوچھ);

    لكل ( ;; ) {
        إذا (q1 == ngx_قطار_sentinel(قطار)) {
            ngx_قطار_جوڑو(قطار, پوچھ);
            اكسر;
        }

        إذا (q2 == ngx_قطار_sentinel(پوچھ)) {
            اكسر;
        }

        إذا (cmp(q1, q2) <= 0) {
            q1 = ngx_قطار_اگلا(q1);
            استمر;
        }

        ngx_قطار_remove(q2);
        ngx_قطار_داخل_کرو_before(q1, q2);

        q2 = ngx_قطار_سر(پوچھ);
    }
}
